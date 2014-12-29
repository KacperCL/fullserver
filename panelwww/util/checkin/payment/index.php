<?
/**
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

require_once("init.php");
global $pakiety;
$DB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);
$GFS=new FSPlayer();

if ($_SERVER['REQUEST_METHOD']=='GET') {
    $bledy=Array();
    if (strlen($_GET['kod'])!=8) 
	    die("0");
    else $kod=strtr($_GET['kod'],'&?=\\/@"','_______');
    $gamep=intval($_GET['gamep']);
    if (!(array_key_exists($gamep, $pakiety))) 
	die("0 TYPE NOT FOUND!");
    $pakiet=$pakiety[$gamep];
    
    $GFS=new FSPlayer();
    $GFS->fetchById($_GET['id']);
    if (!($GFS->accountID>0))
	    die("0 PLAYERID NOT FOUND!");

	$JSMS=new JustpaySMS();
	$sprawdzenie=$JSMS->validate($pakiet['kod'],$pakiet['numer'],$kod);
    if (intval($sprawdzenie)!=1) {
		sendmail("hostmaster@fullserver.eu","[GAMEP] Bledne ".$gamep." ".$_GET['id'],"Blad: ".print_r($bledy,1)."\n\n".print_r($_GET,1)."\n\n".print_r($_SERVER,1));
    	die("0 BAD CODE");
	} else {
		sendmail("hostmaster@fullserver.eu","[GAMEP] ".$gamep." ".$_GET['id'],print_r($_GET,1)."\n\n".print_r($_SERVER,1));
        die("1 $gamep");
	}
};
// EOF