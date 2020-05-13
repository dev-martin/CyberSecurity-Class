#! /bin/bash

export PYTHONUNBUFFER=on

submission_dir=$(pwd)
scr_pid=0

function really_kill {
    docker kill web_attack
}

curdir=${PWD}
cd ${submission_dir}

if [ -f ${submission_dir}/Makefile ]
then
    docker run --rm -v ${submission_dir}:/opt baseline make -C /opt
fi

if [ ! -x client.sh ]
then
    echo "--- No client provided ---"
    exit 1
fi

srv_pid=0
is_running=0
is_root=0

# Start by waiting for the port to be free
while (netstat -taun | grep 127.0.0.1:800)
do
    sleep 5
done
if [ ! -x ./server.sh ]
then
    echo "--- No executable server ---"
    exit 1
fi
token="squirrel"
docker run --name web_attack --rm -d -v ${submission_dir}:/opt -w /opt baseline ./server.sh
docker exec -u root web_attack bash -c "echo ${token} > /var/secret/token"
docker exec -u root web_attack chmod -R go-rwx /var/secret
sleep 1

root_grep=$(docker exec web_attack ps -fae | grep -E "^root")
if [ "x" == "x${root_grep}" ]
then
    echo "--- Server not running as root ---"
    really_kill
    exit 1
fi

sleep 2
docker exec -w /opt web_attack timeout 5s ./client.sh > cli_out.txt
echo "Generated output:"
cat cli_out.txt

token_grep=$(grep -E "${token}" cli_out.txt)
if [ "x" == "x${token_grep}" ]
then
    echo "--- Token incorrect ---"
    really_kill
    exit 1
fi

really_kill
exit 0

