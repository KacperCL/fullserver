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

echo \#define GMVERSION \"$LOG\r$SVER\" > ../include/fullserver/version.inc
KIEDY=`date +%x\ %T`
GDZIE=`hostname`
GMHOST="91.204.162.80"
GMPORT="7777"
#GMHOST="10.39.7.2"
#GMPORT="8888"

echo \#define GMCOMPILED \"skompilowana $KIEDY przez $USER@$GDZIE\" >> ../include/fullserver/version.inc
echo \#define GMHOST \"$GMHOST\" >> ../include/fullserver/version.inc
echo \#define GMPORT $GMPORT >> ../include/fullserver/version.inc

WINEPREFIX=/vol/n/.wg/ wine /pawno/pawncc -A4 -v2 -d2 -i..\\include_3z -i..\\include fs.pwn  -\;\+ -\\ -\(\+

