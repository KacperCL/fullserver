#!/bin/sh

DATA=`date "+%Y%m%d-%H%M"`

echo "=== SERVER ==="
mkdir -p ./server_backups/$DATA
cd ./server_backups/$DATA
echo "Dumping SQL DB..."
mysqldump -u fullserver -p -h 127.0.0.1 -pPASSWORD -f DBNAME > ./fullserver.sql
echo "Compressing..."
bzip2 ./fullserver.sql
echo "Done"
