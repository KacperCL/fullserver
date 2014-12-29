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

if ($_SERVER['REQUEST_METHOD']=='GET'){
    $_POST['k_level']=Array(1,3);
    $k_level=Array(1,3);    
}

$k_level=@array_keys($_POST['k_level']);
if (sizeof($k_level)==0) $k_level[]=1;
$k_viewd=isset($_POST['k_viewd'])?1:0;


$smarty->assign('k_level',$_POST['k_level']);
$smarty->assign('k_viewd',$k_viewd);

foreach($admini as $k=>$v) {
    if (!in_array($v['level'], $k_level))
	    unset($admini[$k]);
}

// pobieramy dane aktywnosci
$week3=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE YEAR(data)=YEAR(NOW()-INTERVAL 3 WEEK) AND WEEK(data)=WEEK(NOW()-INTERVAL 3 WEEK) GROUP BY id_player;");
foreach ($week3 as $l) {
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['week3']=round($l['czas']);
	}


$week2=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE YEAR(data)=YEAR(NOW()-INTERVAL 2 WEEK) AND WEEK(data)=WEEK(NOW()-INTERVAL 2 WEEK) GROUP BY id_player;");
foreach ($week2 as $l) {
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['week2']=round($l['czas']);
	}

$week1=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE YEAR(data)=YEAR(NOW()-INTERVAL 1 WEEK) AND WEEK(data)=WEEK(NOW()-INTERVAL 1 WEEK) GROUP BY id_player;");
foreach ($week1 as $l) {
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['week1']=round($l['czas']);
}	

$week0=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE YEAR(data)=YEAR(NOW()) AND WEEK(data)=WEEK(NOW()) GROUP BY id_player;");
foreach ($week0 as $l)
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['week0']=round($l['czas']);

if ($k_viewd) {
$day2=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE DATE(data)=DATE(NOW()-INTERVAL 2 DAY) GROUP BY id_player;");
foreach ($day2 as $l)
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['day2']=round($l['czas']);


$day1=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE DATE(data)=DATE(NOW()-INTERVAL 1 DAY) GROUP BY id_player;");
foreach ($day1 as $l)
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['day1']=round($l['czas']);


$day0=$RDB->pobierz_tabele_wynikow("select id_player,sum(minut) czas from fs_admin_activity WHERE DATE(data)=DATE(NOW()) GROUP BY id_player;");
foreach ($day0 as $l){
	if ($admini[$l['id_player']] && in_array($admini[$l['id_player']]['level'], $k_level))
	$admini[$l['id_player']]['day0']=round($l['czas']);
}
}

$smarty->assign('aa',$admini);
		
$smarty->display('aktywnosc_adv.tpl');
// EOF