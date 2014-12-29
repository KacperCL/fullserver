#!/bin/bash

PWD=`pwd | grep l10n`
if [ $PWD ]; then
	print "Ten skrypt przeznaczony jest dla oryginalnej (polskiej) wersji jezykowej."
	exit
fi

date

SVER=`svnversion ../..`
_SVER=${SVER:0:2}
LOG=`echo "l($_SVER)/l(2)" | bc -l`
LOG=${LOG:0:3}

echo "/**
The MIT License (MIT)

Copyright (c) 2014 Mateusz Cichon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
 " > ../include/fullserver/version.inc
 
echo \#define GMVERSION \"$LOG\r$SVER\" >> ../include/fullserver/version.inc
KIEDY=`date +%x\ %T`
GDZIE=`hostname`
GMHOST="178.19.105.98"
GMPORT="3500"

echo \#define GMCOMPILED \"skompilowana $KIEDY przez $USER@$GDZIE\" >> ../include/fullserver/version.inc
echo \#define GMHOST \"$GMHOST\" >> ../include/fullserver/version.inc
echo \#define GMPORT $GMPORT >> ../include/fullserver/version.inc
echo \#define SERVER_NUM 2 >> ../include/fullserver/version.inc

../pawno/pawncc.exe -A4 -v2 -d2 -i..\\include -i..\\include_3z fs.pwn  -\;\+ -\\ -\(\+
date
