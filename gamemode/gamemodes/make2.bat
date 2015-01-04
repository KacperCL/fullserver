@echo off

time /T
 
echo #define GMVERSION "PUBLICrGIT" > ../include/fullserver/version.inc

setlocal enabledelayedexpansion
FOR /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set DATE=%%c-%%a-%%b)
FOR /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set TIME=%%a:%%b)

set USER=none
set GDZIE=builder
set GMHOST=127.0.0.1
set GMPORT=7777

echo #define GMCOMPILED "skompilowana %DATE% %TIME% przez %USER%@%GDZIE%" >> ../include/fullserver/version.inc
echo #define GMHOST "%GMHOST%" >> ../include/fullserver/version.inc
echo #define GMPORT %GMPORT% >> ../include/fullserver/version.inc
echo #define SERVER_NUM 1 >> ../include/fullserver/version.inc
endlocal

echo Please wait...
..\pawno\pawncc.exe -A4 -v2 -d2 -i..\\include -i..\\include_3z fs.pwn  -\;\+ -\\ -\(\+
time /T
pause