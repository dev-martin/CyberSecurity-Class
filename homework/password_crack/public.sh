#! /bin/bash

submission_dir=$(pwd)
grading_dir=$(pwd)

cd ${submission_dir}

if [ -f Makefile ]
then
    docker run --rm -v ${submission_dir}:/opt baseline make -C /opt
fi

if [ ! -x crack ]
then
    echo "--- No executable crack found ---"
    exit 1
fi

function test_one {
    local passwd=$1
    local hash=$(echo -n ${passwd} | md5sum | awk '{print $1}')
    local test_output=$(docker run --rm -v ${submission_dir}:/opt -w /opt baseline timeout 5m ./crack ${hash})
    if [ "x" == "x${test_output}" ]
    then
        echo "--- No password recovered for ${passwd} ---"
        exit 1
    fi
    local recovered=$(echo ${test_output} | awk '{print $2}')
    if [ "x${passwd}" != "x${recovered}" ]
    then
        echo "--- Incorrect password ---"
        echo "    expected: ${passwd}"
        echo "    received: ${recovered}"
        exit 1
    fi
    echo "+++ Recovered ${recovered} +++"
}

# Untransformed
test_one 'password'
test_one 'letmein'
test_one 'welcome'

# One transform
test_one 's+arwars'
test_one 'm0nkey'
test_one 'footBall'

# Two transforms
test_one 'fr33dom'
test_one 'wh4tEver'

# Three transforms
test_one '1z34%6t8'

