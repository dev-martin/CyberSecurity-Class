#! /bin/bash

name=""
if [ $# -gt 0 ]
then
    name="--name $1"
fi

img=sqli

ctr=$(docker run --rm -d ${name} -p 3306:3306 ${img} tail -f /etc/issue)
docker cp my.cnf ${ctr}:/etc/mysql/my.cnf
docker cp fix_db.sh ${ctr}:/usr/local/bin/fix_db.sh
docker exec ${ctr} chown -R mysql:mysql /var/lib/mysql
docker exec ${ctr} chmod -R ug+rw,o-rwx /var/lib/mysql
docker exec ${ctr} service mysql start
docker exec ${ctr} /usr/local/bin/fix_db.sh
docker exec ${ctr} mysqladmin update mysql.user set Host='%' WHERE Host='localhost'

echo "*** Ignore the error relating to mysqladmin ***"

echo $ctr

