#!/bin/sh

OLD_TEMPLATE=`stat -tc"%Y %Z %s" obiekty-template.txt`
while true
do

NEW_TEMPLATE=`stat -tc"%Y %Z %s" obiekty-template.txt`
[ "$NEW_TEMPLATE" == "$OLD_TEMPLATE" ] || ncftpput -u USERNAME -p "PASSWORD" -S .tmp-template -C 127.0.0.1 obiekty-template.txt scriptfiles/obiekty-template.txt
OLD_SEBOX=`stat -tc"%Y %Z %s" obiekty-template.txt`
sleep 1

done

