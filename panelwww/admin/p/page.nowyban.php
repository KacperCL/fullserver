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
if ($LVL<3) die("Nie masz dostepu do tej zakladki.");

if ($_POST['data'] && $_POST['nick'] && $_POST['powod']) 
{
	$data=$_POST['data'];
	$nick=$_POST['nick'];
	$powod=$_POST['powod'];
	
	$dataf = new DateTime($data);
	$dataff=$dataf->format('Y-m-d H:i:s');

	if(mktime(0, 0, 0, date("m")+3,   date("d"),   date("Y"))<strtotime($dataff)){
		$smarty->assign('blad',"Podana data przekracza maksymalny zakres 3 miesiecy!");
		$smarty->display('nowyban.tpl');
		exit;
	}
	
	if (filter_var($nick, FILTER_VALIDATE_IP) == FALSE){
		$q="SELECT id FROM fs_players WHERE nick=\"$nick\"";
		$id=$RDB->wynik($q);
		if($id==0) {
			$smarty->assign('blad',"Nie znaleziono gracza o podanym przez Ciebie nicku, a podany tekst nie jest poprawnym adresem IP! Spróbuj ponownie.");
			$smarty->display('nowyban.tpl');
			exit;
		}
		$q="INSERT INTO fs_bans (player_banned, player_given, date_created, date_end, reason) VALUES ($id, (SELECT id FROM fs_players WHERE nick='".$_SESSION['login']."'), NOW(), '$dataff', '$powod')";
		$RDB->zapytanie($q);
	}else{
		$q="INSERT INTO fs_ipbans SET ip='$nick',player_given=(SELECT id FROM fs_players WHERE nick='".$_SESSION['login']."'),date_end='$dataff',reason='$powod',target_nick='<brak nicku>'";
		$RDB->zapytanie($q);
	}
	$smarty->assign('success',"Super! Ban został poprawnie nadany :-)");
	$smarty->display('nowyban.tpl');
	exit;
}

$smarty->display('nowyban.tpl');
// EOF