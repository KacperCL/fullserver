@echo off
echo Starting sequence...
del *.amx
cd .
for /f "tokens=*" %%G in ('dir /b /a "."') do (
   echo Building %%G
   ..\pawno\pawncc.exe -A4 -v2 -d2 -i..\\include_3z %%G  -\;\+ -\\ -\(\+
)
echo sequence completed!
pause