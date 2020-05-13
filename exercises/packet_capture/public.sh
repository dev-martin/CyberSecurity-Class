#! /bin/bash

submission_dir=$(pwd)

if [ ! -x ${submission_dir}/capture ]
then
    echo "--- No executable file capture found ---"
    exit 1
fi

cd ${submission_dir}

rm -f capture.cap

timeout 20s ./capture &
cap_pid=$!

sleep 2

host en.wikipedia.org
wget -O /dev/null https://en.wikipedia.org/wiki/foobar

wait ${cap_pid}

if [ ! -f capture.cap ]
then
    echo "--- No capture file capture.cap found ---"
    exit 1
fi

# Are there exactly 20 packets?
npkts=$(tshark -r capture.cap | wc -l)
if [ 20 -ne ${npkts} ]
then
    echo "--- Expected 20 packets, found ${npkts} ---"
    exit 1
fi
echo "+++ Correct number of packets found +++"

# Is there DNS traffic?
ndns=$(tshark -r capture.cap dns | wc -l)
if [ 0 -eq ${ndns} ]
then
    echo "--- No DNS packets found ---"
    exit 1
fi
echo "+++ DNS packets found +++"

# Is there HTTPS traffic >= 500 bytes?
nhttps_large=$(tshark -r capture.cap "tcp.srcport == 443 and ip.len >= 500" | wc -l)
if [ 0 -eq ${nhttps_large} ]
then
    echo "--- No HTTPS packets >= 500B found ---"
    exit 1
fi
echo "+++ HTTPS packets >= 500B found +++"

# Is there HTTPS traffic < 500 bytes?
nhttps_small=$(tshark -r capture.cap "tcp.srcport == 443 and ip.len < 500" | wc -l)
if [ 0 -ne ${nhttps_small} ]
then
    echo "--- HTTPS packets < 500B found ---"
    exit 1
fi
echo "+++ No HTTPS packets < 500B found +++"

# Is there any other traffic?
nother=$(( ${npkts} - ${ndns} - ${nhttps_large} - ${nhttps_small} ))
if [ ${nother} -ne 0 ]
then
    echo "--- Other packets found ---"
    exit 1
fi

echo "+++ All packets correct +++"
exit 0


