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

global $LVL;
if ($LVL<4) die("Nie masz dostepu do tej zakladki.");

$q_select="";
$q_where="";

if(($_POST['nick'] || $_POST['serial'] || $_POST['ip']) && $_POST['typ'])
{	
	$nick=$_POST['nick'];
	$serial=$_POST['serial'];
	$ip=$_POST['ip'];
	if(!$serial) $serial="NONE";
	if(!$nick) $nick="sdsdsdsdwr3eftrasfds";
	if(!$ip) $ip="NONE";
	if($_POST['typ']) $q="SELECT id,nick,ip_last,serial_last,serial_registered,ip_registered,respect,round(session/60) minut,datetime_registered,datetime_last, skill FROM `fs_players` WHERE nick LIKE '%$nick%' OR serial_last LIKE '%$serial%' OR ip_last LIKE '%$ip%'";
	else $q="SELECT id,nick,ip_last,serial_last serial_registered,ip_registered,respect,round(session/60) minut,datetime_registered,datetime_last, skill FROM `fs_players` WHERE nick LIKE '%$nick%' AND serial_last LIKE '%$serial%' AND ip_last LIKE '%$ip%'";
	$lista=$RDB->pobierz_tabele_wynikow($q);
	$smarty->assign('lista',$lista);
	$smarty->assign('serial',$_POST['serial']);
	$smarty->assign('ip',$_POST['ip']);
	$smarty->display('szukaj.tpl');
	exit;
}

$smarty->display('szukaj.tpl');
// EOF