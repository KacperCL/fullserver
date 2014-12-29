@echo off

time /T
 
echo #define GMVERSION "GITr2981" > ../include/fullserver/version.inc

set KIEDY=date /T
set USER=none
set GDZIE=builder
set GMHOST=127.0.0.1
set GMPORT=7777

echo #define GMCOMPILED "skompilowana %KIEDY% przez %USER%@%GDZIE%" >> ../include/fullserver/version.inc
echo #define GMHOST "%GMHOST%" >> ../include/fullserver/version.inc
echo #define GMPORT %GMPORT% >> ../include/fullserver/version.inc
echo #define SERVER_NUM 1 >> ../include/fullserver/version.inc

echo Please wait...
..\pawno\pawncc.exe -A4 -v2 -d2 -i..\\include -i..\\include_3z fs.pwn  -\;\+ -\\ -\(\+ > build.log
time /T
pause