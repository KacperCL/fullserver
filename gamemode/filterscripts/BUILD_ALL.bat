@echo off
echo Starting sequence...
del *.amx
cd .
for /f "tokens=*" %%G in ('dir /b /a "*.p"') do (
   echo Building %%G
   ..\pawno\pawncc.exe -A4 -v2 -d2 -i..\\include\\samp -i..\\include\3rdparty %%G  -\;\+ -\\ -\(\+
)
echo Sequence completed!
pause