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
echo "Dumping LOGS..."
ncftpget -u sUSERNAME -p PASSWORD ftp://127.0.0.1/server_log.txt
echo "Compressing..."
bzip2 ./server_log.txt
echo "Done"

echo "=== FORUM ==="
mkdir -p ../../forum_backups/$DATA
cd ../../forum_backups/$DATA
echo "Dumping SQL DB..."
mysqldump -u USERNAME -p -h 127.0.0.1 -pPASSWORD -f DBNAME > ./fullserv_forum.sql
echo "Compressing..."
bzip2 ./fullserv_forum.sql
echo "Done"
