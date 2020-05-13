#! /bin/bash

chown -R mysql:mysql /var/lib/mysql
chmod -R ug+rw,o-rwx /var/lib/mysql
service mysql start
mysql --user=root --password=seedubuntu <<EOFMYSQL
SELECT user, host from mysql.user;
UPDATE mysql.user SET host='%' WHERE host='ubuntu';
FLUSH PRIVILEGES;

EOFMYSQL

