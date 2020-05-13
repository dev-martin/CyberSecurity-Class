#! /bin/bash

submission_dir=$(pwd)
grading_dir=$(pwd)

function is_prime {
    local p=$1
    local f=$(factor ${p} | cut -d' ' -f 2)
    if [ $p -eq $f ]
    then
	return 1
    fi
    return 0
}

function test_sqrt {
    local x=$1
    local p=$2
    local max_score=$3

    local val1=$(echo ${max_score} | awk '{print $1/3.0}')
    local val2=$(echo ${max_score} ${val1} | awk '{print $1 - $2 }')

    local outp=$(docker run --rm -v ${submission_dir}:/opt -w /opt baseline timeout 300s ./sqrt $x $p)
    echo $outp
    local res=$(echo $outp | cut -d' ' -f3)
    if [ ${res} -ge $p ]
    then
	echo "--- Result $res is >= $p ---"
        exit 1
    fi
    local xmodp=$(( $x % $p ))
    local res2modp=$(( $res * $res % $p ))
    if [ ${xmodp} -eq ${res2modp} ]
    then
	echo "+++ ${res}^2 mod $p = $x mod $p +++"
    else
	echo "--- ${res}^2 mod $p != $x mod $p ---"
        exit 1
    fi
}

function test_square {
    local a=$1
    local p1=$2
    local p2=$3
    local k=$4
    local n=$(( $p1 * $p2 ))
    local phi=$(( ( $p1 - 1 ) * ( $p2 - 1 ) ))
    local e=$(python -c "print( ( 1 << $k ) % $phi )" )
    local b=$(python -c "print( ( $a ** $e ) % $n )" )
    echo "Correct value: $b"
    echo "a=$a n=$n k=$k"
    local outp=$(docker run --rm -v ${submission_dir}:/opt -w /opt baseline timeout 300s ./square $a $n $k)
    echo ${outp}
    local val=$(echo ${outp} | cut -d' ' -f3)
    if [ $val -eq $b ]
    then
	echo "+++ Correct result +++"
    else
	echo "--- Incorrect result ---"
        exit 1
    fi
}

cd ${submission_dir}

if [ ! -x sqrt ]
then
    echo "--- No executable sqrt found ---"
    exit 1
fi

if [ ! -x square ]
then
    echo "--- No executable square found ---"
    exit 1
fi

test_sqrt 14 31 1.0
test_sqrt 443 7151 1.0
test_sqrt 20742 2946901 1.0

test_square 15 7 13 18
test_square 13 17 43 19
test_square 75 113 211 20

exit 0

