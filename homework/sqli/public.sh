#! /bin/bash

export PYTHONUNBUFFERED=on

client_injection=0
symlinked_client=0
symlinked_s1=0
symlinked_s2=0

submission_dir=$(pwd)

ctr_name=foo

if [ -f ${submission_dir}/Makefile ]
then
    docker run --rm -v ${submission_dir}:/opt baseline make -C /opt
fi

# Start by blowing away any containers
docker ps -q | xargs docker kill
docker ps -a -q | xargs docker rm

function tear_down {
    # We started the container with --rm, so kill is sufficient.
    docker kill ${ctr_name}
}

function extract_sql_resp {
    local capfile=$1
    if [ -f ${capfile} ]
    then
	local output=$(tshark -r ${capfile} -T fields -e mysql.row.text "mysql.num_fields>0")
	echo "XXX ${output}" >&2
	echo ${output}
    else
	echo "XXX No capfile ${capfile}" >&2
    fi
}

function extract_client_req {
    local capfile=$1
    if [ ! -f ${capfile} ]
    then
	echo "--- No file ${capfile} found ---"
	tear_down
        exit 1
    else
	local output=$(tshark -r ${capfile} -T fields -e http.file_data -e http.request.uri.query -Y http | grep user)
	local grout=$(echo ${output} | grep -iE "'|--|%27|%2d%2d")
	if [ "x" == "x${grout}" ]
	then
	    echo "--- No injection seen ---"
	    echo "    ${output}"
	    tear_down
	    exit 1
	fi
	echo ${output} >&2
	echo ${grout} >&2
    fi
}

# Return 0 on success, 1 on failure, 2 on error
function success {
    local logfile1=$1
    local logfile2=$2
    local capfile=$3
    local have_success=0
    local have_failure=0
    local resp=$(extract_sql_resp ${capfile})
    local grep_res=$(grep -iE "Succ?ess?full?y logged in" ${logfile1} ${logfile2})
    if [ "x" != "x${grep_res}" ]
    then
	if [ "x" != "x${resp}" ]
	then
	    echo "*** Saw success message ***"
	    echo "    ${grep_res}"
	    have_success=1
	else
	    echo "*** Server claimed success, but no response observed ***"
	    echo "    ${resp}"
	    echo "    ${grep_res}"
	    return 2
	fi
    fi
    local grep_resp=$(grep -iE "Failed to log ?in" ${logfile1} ${logfile2})
    if [ "x" != "x${grep_resp}" ]
    then
	if [ "x" == "x${resp}" ]
	then
	    echo "*** Saw failure message ***"
	    echo "    ${grep_resp}"
	    have_failure=1
	else
	    echo "*** Server claimed failure, but response observed ***"
	    echo "    ${resp}"
	    echo "    ${grep_resp}"
	    return 2
	fi
    fi
    tot=$(( ${have_success} + ${have_failure} ))
    if [ ${tot} -ne 1 ]
    then
	echo "*** No success/failure message, or both ***"
	return 2
    elif [ ${have_success} -eq 1 ]
    then
	return 0
    else
	return 1
    fi
}

curdir=${PWD}
cd ${submission_dir}

if [ ! -x client.sh ]
then
    echo "--- No executable client provided ---"
    exit 1
fi

if [ ! -x ./server1.sh ]
then
    echo "--- No executable server1 provided ---"
    exit 1
fi

if [ ! -x ./server2.sh ]
then
    echo "--- No executable server2 provided ---"
    exit 1
fi

${curdir}/start_db.sh ${ctr_name}
sleep 5

# Wait until the most-likely port is free.
while (netstat -taun | grep 127.0.0.1:8000)
do
    sleep 5
done
while (netstat -taun | grep 127.0.0.1:8080)
do
    sleep 5
done

# We're going to start a container and run our tests in it. It will use
# the host's network stack.
capfile="${ctr_name}.foo.cap"
sctr_name="${ctr_name}-srv"
rm -f ${capfile}
touch ${capfile}
chmod a+rw ${capfile}
docker run --rm -d --name ${sctr_name} --network host -v ${submission_dir}:/opt -u root baseline tcpdump -w /opt/${capfile} -U -i lo
sleep 5
docker exec -w /opt -t --env PYTHONUNBUFFERED=on -u root ${sctr_name} ./server1.sh > srv_out.foo 2>&1 &
sleep 5
docker exec -w /opt -t --env PYTHONUNBUFFERED=on ${sctr_name} timeout 10s ./client.sh >cli_out.foo 2>&1
sleep 15
docker kill ${sctr_name}
success srv_out.foo cli_out.foo ${capfile}
if [ $? -ne 0 ]
then
    echo "--- Vulnerable server did not produce success ---"
    tear_down
    exit 1
fi
extract_client_req ${capfile}

# Wait until the most-likely port is free.
while (netstat -taun | grep 127.0.0.1:8000)
do
    sleep 5
done

capfile="${ctr_name}.bar.cap"
rm -f ${capfile}
touch ${capfile}
chmod a+rw ${capfile}
docker run --rm -d --name ${sctr_name} --network host -v ${submission_dir}:/opt -u root baseline tcpdump -w /opt/${capfile} -U -i lo
sleep 5
docker exec -w /opt -t --env PYTHONUNBUFFERED=on -u root ${sctr_name} ./server2.sh >srv_out.bar 2>&1 &
sleep 5
docker exec -w /opt -t --env PYTHONUNBUFFERED=on ${sctr_name} timeout 10s ./client.sh >cli_out.bar 2>&1
sleep 15
docker kill ${sctr_name}
success srv_out.bar cli_out.bar ${capfile}
if [ $? -ne 1 ]
then
    echo "--- Non-vulnerable server did not produce failure ---"
    tear_down
    exit 1
fi
extract_client_req ${capfile}

tear_down
exit 0

