<?php
if($_SERVER['REQUEST_METHOD']!="POST") { header("HTTP/1.0 405 Method Not Allowed"); exit; }

if(!isset($_POST['authId']) || !isset($_POST['action']) || !isset($_POST['prm'])) { header("HTTP/1.0 400 Bad Request"); exit; }

// authId (na razie hardcoded, pozniej w bazie)
if($_POST['authId']!='df2214d5a64757184fdfeeccce' /* internal use only */ && $_POST['authId']!='hu876sdy8s7gdyuist83bhvdvv' /* 7hd */) { header("HTTP/1.0 401 Unauthorized"); exit; }

require_once("/home/fullserv/public_html/fs.i32.pl/p/libs/BazaDanych.class.php");
require_once("/home/fullserv/public_html/fs.i32.pl/p/__config.php");
$RDB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);
  
if($_POST['action']=='player'){
	$pid=intval($_POST['prm']);
	if($pid<32) { header("HTTP/1.0 420 Method Failure"); exit; }
	$result=$RDB->pobierz_wyniki("select p.id,p.nick,p.last_skin,p.respect,p.skill,p.gamep,p.datetime_last,p.datetime_registered,p.ban_count,p.kick_count,p.login_count,p.kill_count,p.teamkill_count,p.death_count,p.suicide_count,p.bank_money,p.wallet_money,p.hitman_prize,IF(p.level>4,0,p.level) level,(p.vip>=NOW()) vip, (select id from fs_houses where ownerid=$pid and paidTo>=DATE(NOW())) dom, (select g.id from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_id, (select g.color from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_color, (select g.name from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_name, (select g.tag from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_tag, (select pig.rank from fs_players_in_gangs pig JOIN fs_gangs g ON g.id=pig.id_gang where id_player=$pid) gang_rank, (SELECT date_end FROM fs_bans WHERE player_banned = $pid AND date_end > NOW() AND date_created < NOW() LIMIT 1) ban_datetime FROM fs_players p WHERE p.id=$pid");
	if($_POST['opt']=='json' || !isset($_POST['opt'])) { print(json_encode($result)); exit; }
	else if($_POST['opt']=='xml') { /*header('Content-type: application/xml; charset="utf-8"');*/ print("ERR: NOT IMPLEMENTED"); exit; }
}else if($_POST['action']=='gang'){
  $gid=intval($_POST['prm']);
	if($gid>20) { header("HTTP/1.0 420 Method Failure"); exit; }
  $result=$RDB->pobierz_tabele_wynikow("SELECT p.id,p.nick,p.respect,p.skill,p.datetime_last,pig.rank,pig.join_date FROM fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player WHERE pig.id_gang=$gid ORDER  BY pig.rank DESC");
  if($_POST['opt']=='json' || !isset($_POST['opt'])) { print(json_encode($result)); exit; }
	else if($_POST['opt']=='xml') { /*header('Content-type: application/xml; charset="utf-8"');*/ print("ERR: NOT IMPLEMENTED"); exit; }
}
?>