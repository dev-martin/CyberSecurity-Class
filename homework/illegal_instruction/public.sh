#! /bin/bash

submission_dir=$(pwd)

if [ ! -f ${submission_dir}/args ]
then
    echo "--- No args found ---"
    exit 1
fi

args=$(cat ${submission_dir}/args | awk '{$1=$1};1')

i=0
while [ $i -lt 30 ]
do
    docker run --rm -v ${submission_dir}:/opt -w /opt baseline ./target ${args} >/dev/null 2>/dev/null
    prog_status=$?
    if [ ${prog_status} -eq 132 ]
    then
        echo "+++ Illegal Instruction generated +++"
        exit 0
    fi
    i=$(( $i + 1 ))
done

echo "--- No Illegal Instruction produced ---"
exit 1

