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
if ($LVL<2) die("Nie masz dostepu do tej zakladki.");

if ($_SERVER['REQUEST_METHOD']=='GET') {
    $_POST['aktywne']=1;
    $_POST['tl']='48h';
}

$q_select="";
$q_where="";

if ($_POST['aktywne']) {
    $q_where.=" AND b.date_end>NOW() ";
    $smarty->assign('k_aktywne',1);
}

if ($_POST['playerinfo']) {
    $q_select=',p1.respect,p1.skill,round(p1.session/60) minut';
    $smarty->assign('k_playerinfo',1);
}

switch($_POST['tl']){
    case '24h':
	$q_where.=" AND date_created>=(NOW()-INTERVAL 1 DAY) ";	break;
    case '48h':
    	$q_where.=" AND date_created>=(NOW()-INTERVAL 2 DAY) ";	break;
    case '4d':
    	$q_where.=" AND date_created>=(NOW()-INTERVAL 4 DAY) ";	break;
    case '1w':
    	$q_where.=" AND date_created>=(NOW()-INTERVAL 7 DAY) ";	break;
    case '1m':
    	$q_where.=" AND date_created>=(NOW()-INTERVAL 1 MONTH) ";break;
    default:
	$_POST['tl']="";
}

$smarty->assign('k_tl',$_POST['tl']);

$q="select 
    b.id,b.date_created,b.date_end,(b.date_end>NOW()) aktywny,reason,p1.nick zbanowany,p2.nick przez, HOUR(TIMEDIFF(NOW(),b.date_end)) pozostaloh
    $q_select
    
    from fs_bans b 
    
    JOIN fs_players p1 on b.player_banned=p1.id 
    JOIN fs_players p2 ON b.player_given=p2.id
	WHERE 1 $q_where
    order by b.id desc;";
    

$bany=$RDB->pobierz_tabele_wynikow($q);
$smarty->assign('bany',$bany);
$smarty->display('bany.tpl');
// EOF