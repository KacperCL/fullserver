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

// pobieramy id adminow
$dane_loginy=$RDB->pobierz_tabele_wynikow("SELECT nick,level,id FROM fs_players WHERE level>=2 AND level<4 ORDER BY id ASC");
foreach ($dane_loginy as $v) $loginy[]=Array('id'=>$v['id'], 'nick'=>$v['nick'], 'level'=>$v['level']);
// pobieramy bany
$dane=$RDB->pobierz_tabele_wynikow("
SELECT COUNT( * ) AS bany, b.nick, b.id, b.level
FROM (

SELECT id, player_given
FROM  `fs_bans` 
UNION SELECT id, player_given
FROM  `fs_ipbans`
)c
INNER JOIN fs_players AS b ON b.id = c.player_given
AND b.level >=2
AND b.level <4
GROUP BY c.player_given
ORDER BY bany DESC 
");

foreach ($dane as $v)
	foreach ($loginy as $login)
		$bany[$login['nick']]=Array('id'=>$login['id'], 'level'=>$login['level'], 'bany'=>0);
	foreach ($dane as $v) {
	  	$bany[$v['nick']]=Array('id'=>$v['id'], 'level'=>$v['level'], 'bany'=>$v['bany']);
	}

$smarty->assign('bany',$bany);	
$smarty->display('rankb.tpl');
// EOF