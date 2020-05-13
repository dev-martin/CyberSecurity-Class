#! /bin/bash

submission_dir=$(pwd)
grading_dir=$(pwd)

function test_one {
    local domain=$1
    local correct=$2
    local tld=$3
    match=$(grep -E "^${domain}" ${submission_dir}/locations)
    if [ "x" == "x${match}" ]
    then
        echo "--- No response for ${domain} ---"
        exit 1
    fi
    country=$(echo ${match} | awk '{print $2}')
    if [ "x${country}" != "x${correct}" ]
    then
	echo "--- Incorrect response for ${domain} ---"
        exit 1
    fi
    echo "+++ Correct response for ${domain} +++"
}

if [ -f ${submission_dir}/locations ]
then
    test_one brief.ly Germany Libya
    test_one deja.vu Australia Vanuatu
    subm_len=$(wc -l ${submission_dir}/locations | awk '{print $1}')
    if [ ${subm_len} -lt 5 ]
    then
	echo "--- No explanations found ---"
        exit 1
    fi
else
    echo "--- No locations file found ---"
    exit 1
fi

