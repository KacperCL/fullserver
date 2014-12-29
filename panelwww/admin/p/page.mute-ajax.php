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

switch($_REQUEST['a2']) {
    case "unban": 
	$banid=intval($_POST['banid']);
	if (!($banid>0)) die("Nieprawidlowe dane!");
	$RDB->zapytanie("UPDATE fs_mutes SET date_end=NOW() WHERE id=$banid AND date_end>NOW() LIMIT 1");
	if ($RDB->affected_rows()==1) { print "OK"; exit; }
	else die("Nie znaleziono wyciszenia, byc moze juz wygaslo?");
	
    default:
	die("Nieznana akcja!");
}
// EOF