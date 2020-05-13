#! /bin/bash

submission_dir=$(pwd)

function compare_files {
    out=$(diff -q $1 $2)
    echo $?
}

docker run --rm -v "${submission_dir}":/opt baseline make -C /opt

if [ ! -f "${submission_dir}/base.s" ]
then
    echo "--- No base.s produced by Makefile ---"
    exit 1
fi

size=$(stat "${submission_dir}/base.s" | grep Size | awk '{print $2}')
out=$(docker run --rm -v "${submission_dir}":/opt baseline /opt/base)
if [ ${size} -lt 1500 ]
then
    echo "--- base is smaller than the minumum required size ---"
    echo "    minimum:  1500"
    echo "    measured: ${size}"
    exit 1
fi
if [ ${#out} -lt 20 ]
then
    echo "--- The output of base is too short ---"
    echo "    minimum:  20"
    echo "    measured: ${#out}"
    exit 1
fi

nmax=3
for n in $(seq ${nmax})
do
    if [ ! -f "${submission_dir}/morph${n}.s" ]
    then
        echo "--- morph${n}.s not found ---"
        exit 1
    fi
    this_size=$(stat "${submission_dir}/morph${n}.s" | grep Size | awk '{print $2}')
    this_out=""
    if [ -x "${submission_dir}/morph${n}" ]
    then
        this_out=$(docker run --rm -v "${submission_dir}":/opt baseline /opt/morph${n})
    fi
    if [ "x${out}" != "x${this_out}" ]
    then
	echo "--- morph${n} produces different output than base ---"
        exit 1
    elif [ 0 -eq $(compare_files "${submission_dir}/base.s" "${submission_dir}/morph${n}.s") ]
    then
	echo "--- morph${n}.s is identical to base.s ---"
        exit 1
    fi
    if [ ${n} -lt ${nmax} ]
    then
        for m in $(seq $(( $n + 1 )) ${nmax})
        do
            if [ 0 -eq $(compare_files "${submission_dir}/morph${m}.s" "${submission_dir}/morph${n}.s") ]
            then
        	echo "--- morph${n}.s is identical to morph${m}.s ---"
                exit 1
            fi
        done
    fi
done

echo "+++ ${nmax} successful metamorphic generations +++"
exit 0

