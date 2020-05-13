#! /bin/bash

submission_dir=$(pwd)
grading_dir=$(pwd)

function abs {
  echo $1 | awk '{ if ($1 >= 0) { print $1 } else { print -1*$1 } }'
}

function freq_diff {
    local d=$(echo $1 | awk '{ print ( $1 - 0.5 ) }')
    abs $d
}

function test_one {
    local input=$1
    local ofile=$2
    docker run --rm -v ${submission_dir}:/opt -w /opt baseline timeout 5s /opt/sp-nrounds ${input} 32 > ${ofile} 2>/dev/null
    for b in $(seq 0 31)
    do
        local flip=$(( 1 << $b ))
        local new_val=$((16#${input} ^ ${flip} ))
        local new_inp=$(printf "%08x" ${new_val})
        docker run --rm -v ${submission_dir}:/opt -w /opt baseline timeout 5s /opt/sp-nrounds ${new_inp} 32 >> ${ofile} 2>/dev/null
    done
    local minmax=$(./bit_freq ${ofile} 2>/dev/null)
    local min_val=$(echo ${minmax} | cut -d' ' -f1 | cut -d: -f2)
    local max_val=$(echo ${minmax} | cut -d' ' -f2 | cut -d: -f2)
    local min_diff=$(freq_diff ${min_val}) 
    local max_diff=$(freq_diff ${max_val}) 
    local outlier=$(echo ${min_diff} ${max_diff} | awk '{ if ($1 < $2) { print $2 } else { print $1 } }')
    local s=0
    s=$(echo ${outlier} $s | awk '{ if ( $1 <= 0.35 ) { print 1 } else { print 0 } }')
    if [ $s -eq 0 ]
    then
        echo "--- Outlier differs from 0.5 by ${outlier} ---"
        exit 1
    fi
    echo "+++ Outlier differs from 0.5 by ${outlier} +++"
}

cd ${submission_dir}

if [ -f Makefile ]
then
    docker run --rm -v ${submission_dir}:/opt baseline make -C /opt
fi

if [ ! -x sp ]
then
    echo "--- No sp found ---"
    exit 1
fi

test_one 00000000 test_00.out
test_one FFFFFFFF test_FF.out
test_one 0F0F0F0F test_0F.out
test_one 55555555 test_55.out

exit 0
