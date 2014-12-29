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
if ($LVL<1) die("Nie masz dostepu do tej zakladki.");

if ($_SERVER['REQUEST_METHOD']=='GET'){
    $_POST['k_level']=Array(1,3);
    $k_level=Array(1,3);  
} 
$k_level=@array_keys($_POST['k_level']);
if (sizeof($k_level)==0) $k_level[]=1;

if(isset($_POST['k_view_tyg3']) && isset($_POST['k_view_tyg4'])) die("Niepoprawne wywolanie! Zaznacz jedna z opcji: (3+2+1) lub (2+1+b), a nie obie na raz!");

$smarty->assign('k_level',$_POST['k_level']);
$smarty->assign('k_view_tyg3',$_POST['k_view_tyg3']);
$smarty->assign('k_view_tyg4',$_POST['k_view_tyg4']);

foreach($admini as $k=>$v) {
    if (!in_array($v['level'], $k_level))
	    unset($admini[$k]);
}

// pobieramy id adminow
$dane_loginy=$RDB->pobierz_tabele_wynikow("SELECT nick,level,id FROM fs_players WHERE level>=1 ORDER BY id ASC");
foreach ($dane_loginy as $v) if (in_array($v['level'], $k_level)) $loginy[]=Array('id'=>$v['id'], 'nick'=>$v['nick'], 'level'=>$v['level']);
// pobieramy aktywnosc
if(isset($_POST['k_view_tyg3']) || isset($_POST['k_view_tyg4'])){
	$do_wyrobienia=3*420; // ilosc minut do wyrobienia w widoku 3tyg!
}else{
	$do_wyrobienia=420; // ilosc minut do wyrobienia!
}
if(isset($_POST['k_view_tyg4'])){
	$dane_aktywnosci=$RDB->pobierz_tabele_wynikow("SELECT WEEK(NOW())+1 tydzien, ua.id_player,u.nick,sum(ua.minut) minut, u.id id, u.level level FROM fs_admin_activity ua JOIN fs_players u ON u.id=ua.id_player WHERE (YEAR(ua.data)=YEAR(NOW()-INTERVAL 2 WEEK) OR YEAR(ua.data)=YEAR(NOW()) OR YEAR(ua.data)=YEAR(NOW()-INTERVAL 1 WEEK)) AND (WEEK(ua.data)=WEEK(NOW()) OR WEEK(ua.data)=WEEK(NOW()-INTERVAL 2 WEEK) OR WEEK(ua.data)=WEEK(NOW()-INTERVAL 1 WEEK)) GROUP BY ua.id_player ORDER BY id ASC;");
}else if(isset($_POST['k_view_tyg3'])){
	$dane_aktywnosci=$RDB->pobierz_tabele_wynikow("SELECT WEEK(NOW())+1 tydzien, ua.id_player,u.nick,sum(ua.minut) minut, u.id id, u.level level FROM fs_admin_activity ua JOIN fs_players u ON u.id=ua.id_player WHERE (YEAR(ua.data)=YEAR(NOW()-INTERVAL 2 WEEK) OR YEAR(ua.data)=YEAR(NOW()-INTERVAL 3 WEEK) OR YEAR(ua.data)=YEAR(NOW()-INTERVAL 1 WEEK)) AND (WEEK(ua.data)=WEEK(NOW()-INTERVAL 3 WEEK) OR WEEK(ua.data)=WEEK(NOW()-INTERVAL 2 WEEK) OR WEEK(ua.data)=WEEK(NOW()-INTERVAL 1 WEEK)) GROUP BY ua.id_player ORDER BY id ASC;");
}else{
	$dane_aktywnosci=$RDB->pobierz_tabele_wynikow("SELECT WEEK(ua.data)+1 tydzien ,YEAR(ua.data) rok,ua.id_player,u.nick,sum(ua.minut) minut, u.id id, u.level level FROM fs_admin_activity ua JOIN fs_players u ON u.id=ua.id_player GROUP BY WEEK(ua.data),ua.id_player ORDER BY ua.data DESC,id ASC;");
}
foreach ($dane_aktywnosci as $v)
	if (!isset($aktywnosc_tygodniowa[$v['tydzien']]))
		foreach ($loginy as $login)
			$aktywnosc_tygodniowa[$v['tydzien']][$login['nick']]=Array('id'=>$login['id'], 'level'=>$login['level'], 'minutabs'=>0, 'pozostalo'=>$do_wyrobienia, 'norma'=>0);
	foreach ($dane_aktywnosci as $v) {
		if (in_array($v['level'], $k_level)){
			$pozostalo=$do_wyrobienia-intval($v['minut']);
	  		if ($pozostalo<0) $pozostalo=0;
	  		$norma=(intval($v['minut'])/$do_wyrobienia)*100;
	  		if ($norma>100) $norma=100;
	  		$aktywnosc_tygodniowa[$v['tydzien']][$v['nick']]=Array('id'=>$v['id'], 'level'=>$v['level'], 'minutabs'=>$v['minut'], 'pozostalo'=>$pozostalo, 'norma'=>$norma);
		}
	}

$smarty->assign('aktywnosc_tygodniowa',$aktywnosc_tygodniowa);	
$smarty->display('aktywnosc.tpl');
// EOF