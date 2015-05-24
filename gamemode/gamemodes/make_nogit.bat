@echo off

time /T

setlocal enabledelayedexpansion
FOR /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set DATE=%%c-%%a-%%b)
FOR /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set TIME=%%a:%%b)

set USER=%username%
set GDZIE=%userdomain%

echo /** > ../include/fullserver/version.inc
echo The MIT License (MIT) >> ../include/fullserver/version.inc
echo. >> ../include/fullserver/version.inc
echo Copyright (c) 2014 Mateusz Cichon >> ../include/fullserver/version.inc
echo. >> ../include/fullserver/version.inc
echo Permission is hereby granted, free of charge, to any person obtaining a copy >> ../include/fullserver/version.inc
echo of this software and associated documentation files (the "Software"), to deal >> ../include/fullserver/version.inc
echo in the Software without restriction, including without limitation the rights >> ../include/fullserver/version.inc
echo to use, copy, modify, merge, publish, distribute, sublicense, and/or sell >> ../include/fullserver/version.inc
echo copies of the Software, and to permit persons to whom the Software is >> ../include/fullserver/version.inc
echo furnished to do so, subject to the following conditions: >> ../include/fullserver/version.inc
echo. >> ../include/fullserver/version.inc
echo The above copyright notice and this permission notice shall be included in all >> ../include/fullserver/version.inc
echo copies or substantial portions of the Software. >> ../include/fullserver/version.inc
echo. >> ../include/fullserver/version.inc
echo THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR >> ../include/fullserver/version.inc
echo IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, >> ../include/fullserver/version.inc
echo FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE >> ../include/fullserver/version.inc
echo AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER >> ../include/fullserver/version.inc
echo LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, >> ../include/fullserver/version.inc
echo OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE >> ../include/fullserver/version.inc
echo SOFTWARE. >> ../include/fullserver/version.inc
echo */ >> ../include/fullserver/version.inc
echo. >> ../include/fullserver/version.inc

echo #define GMVERSION "PUBLICrGIT" >> ../include/fullserver/version.inc
echo #define GMCOMPILED "skompilowana %DATE% %TIME% przez %USER%@%GDZIE%" >> ../include/fullserver/version.inc
echo #define SERVER_NUM 1 >> ../include/fullserver/version.inc
cat settings.ini >> ../include/fullserver/version.inc
endlocal

echo Please wait...
..\pawno\pawncc.exe -A4 -v2 -d2 -i..\\include\\fullserver -i..\\include\\3rdparty -i..\\include\\samp fs.pwn  -\;\+ -\\ -\(\+
time /T
pause