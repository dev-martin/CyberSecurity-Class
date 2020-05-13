#! /bin/bash

submission_dir=$(pwd)
grading_dir=$(pwd)


cd ${submission_dir}

if [ -f Makefile ]
then
    make
fi

if [ ! -x poison ]
then
    echo "--- No executable poison found ---"
    exit 1
fi

gcap=test_google.pcap
whcap=test_whitehouse.pcap
wpcap=test_wikipedia.pcap

function test_one {
    local target=$1
    local spoof=$2
    local infile=$3
    local outfile=$4

    ./poison ${target}. ${spoof} ${infile} ${outfile}

    if [ ! -f ${outfile} ]
    then
        echo "--- No output file ${outfile} produced ---"
        exit 1
    fi

    target_out=$(tshark -r ${outfile} -T fields -e dns.a -e dns.flags "dns.resp.name == ${target}" 2>/dev/null)
    echo ${target_out}
    if [ "x" == "x${target_out}" ]
    then
        echo "--- No response produced ---"
        exit 1
    fi
    echo "+++ Response found +++"

    addr=$(echo ${target_out} | awk '{print $1}')
    flags=$(echo ${target_out} | awk '{print $2}')
    aa=$(( ${flags:-0} & 0x400 ))
    rq=$(( ${flags:-0} & 0x8000 ))

    if [ ${rq} -eq 0 ]
    then
        echo "--- Response flag not set ---"
        exit 1
    fi
    echo "+++ Response flag set +++"

    if [ "x${addr}" != "x${spoof}" ]
    then
        echo "--- Response not spoofed ---"
        exit 1
    fi
    echo "+++ Response spoofed +++"

    if [ ${aa} -eq 0 ]
    then
        echo "--- Response not authoritative ---"
        exit 1
    fi
    echo "+++ Response authoritative +++"

    num_rec=$(tshark -r ${outfile} | wc -l)
    if [ 1 -lt ${num_rec} ]
    then
        echo "--- Extra output found ---"
        exit 1
    fi
}

test_one www.google.com 1.2.3.4 google.pcap ${gcap}
test_one www.whitehouse.gov 9.8.7.6 whitehouse.pcap ${whcap}
test_one www.wikipedia.org 12.34.56.78 wikipedia.pcap ${wpcap}

