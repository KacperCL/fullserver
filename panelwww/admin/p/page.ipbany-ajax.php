<?php
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
 
switch($_REQUEST['a2']) {
    case "unban": 
	global $LVL;
	if ($LVL<4) die("Nie masz dostepu do tej zakladki.");
	$banid=intval($_POST['banid']);
	if (!($banid>0)) die("Nieprawidlowe dane!");
	$RDB->zapytanie("UPDATE fs_ipbans SET date_end=NOW() WHERE id=$banid AND date_end>NOW() LIMIT 1");
	if ($RDB->affected_rows()==1) { print "OK"; exit; }
	else die("Nie znaleziono bana, byc moze juz wygasl?");
    case "rbl1":
	$ip=$_POST['ip'];
	exit;
	break;
    case "rbl2":
	$ip=$_POST['ip'];
	exit;
	break;	
    case "host":
	$ip=$_POST['ip'];
	print gethostbyaddr($ip);
	exit;
	break;	
    case "hist":
	$ip=mysql_real_escape_string($_POST['ip']);
	$suma_banow_ip=$RDB->wynik("SELECT count(*) FROM fs_ipbans WHERE ip='$ip'");
	if ($suma_banow_ip>1) print "Banów na ten IP: <b>".$suma_banow_ip."</b><br />";
	$ip24= preg_replace("/\d+$/","%",$ip);
	$suma_banow_24=$RDB->wynik("SELECT count(*) FROM fs_ipbans WHERE ip LIKE '$ip24'");
	if ($suma_banow_24>1) print "Banów na $ip24: <b>".$suma_banow_24."</b><br />";

	$nicki_24=$RDB->pobierz_tabele_wynikow("select id,nick from fs_players where ip_last like '$ip24' or ip_registered like '$ip24' limit 5;");
	if (count($nicki_24)>0) {
	    print "Gracze laczacy sie z tej /24: ";
	    foreach ($nicki_24 as $l=>$k)
		print "<a href='http://fs.i128.pl/stats/player/".$k['id']."'>".htmlspecialchars($k['nick'])."</a> ";
	}
	exit;
	break;	
	
    default:
	die("Nieznana akcja!");
}
// EOF