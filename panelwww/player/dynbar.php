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
 
$pid=intval($_REQUEST['pid']);
if (!($pid>0)) $pid=32;
$hash=md5($pid);
$filename="tmp/dynbar-".$hash.".png";

if(file_exists($filename)){
    $ctime=filectime($filename);
    if (time()-$ctime<60*60*6)	{ // 12h
	header('Content-Type: image/png');
	readfile($filename);
	exit;
    }
    @unlink($filename);
}

require_once("p/libs/BazaDanych.class.php");
require_once("p/__config.php");
$RDB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);

$pd=$RDB->pobierz_wyniki("SELECT p.nick,p.level,p.last_skin,p.respect,p.skill,p.vip>NOW() vip,pig.id_gang, g.color gangcolor, g.name gangname,pig.rank gangrank FROM fs_players p LEFT JOIN fs_players_in_gangs pig ON pig.id_player=p.id AND pig.rank!='suspended' LEFT JOIN fs_gangs g ON pig.id_gang=g.id WHERE p.id=$pid LIMIT 1");

$ban=$RDB->pobierz_wyniki("SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) pozostalo FROM fs_bans WHERE player_banned = $pid AND date_end > NOW() AND date_created < NOW() LIMIT 1");

$im=imagecreatefrompng("s/i/2.png");
imagesavealpha($im,true);
$white = imagecolorallocate($im, 255, 255, 255);
$gray = imagecolorallocate($im, 67, 67, 67);
$red = imagecolorallocate($im, 255, 60, 60);
$yellow = imagecolorallocate($im, 0xFF,0xD7,0x00);
$black = imagecolorallocate($im, 0x00,0x00,0x00);
$orange = imagecolorallocate($im, 0xff,0x66,0x00);

$blue = imagecolorallocate($im, 0x2b,0x7c,0xd5);
$courier = 's/fonts/cour.ttf';
$veramobd = 's/fonts/VeraMoBd.ttf';
$arial = 's/fonts/arial.ttf';
$arialb = 's/fonts/arialb.ttf';

if($pd['level']>1 && $pd['level']<5)
    imagettftext($im, 9, 0, 5, 13, $red, $veramobd, $pd['nick']);
else if ($pd['vip']>0)
    imagettftext($im, 9, 0, 5, 13, $yellow, $veramobd, $pd['nick']);
else
    imagettftext($im, 9, 0, 5, 13, $white, $veramobd, $pd['nick']);
    
$bbox=imagettfbbox(9, 0, $veramobd, $pd['nick']."       ");
$offset=5+$bbox[2];

$r=""; $color=$black;
if ($pid==702) {
			$r="Maper";
			$color=$blue;
} else 

switch($pd['level']){
	case 4:
		$r="RCON";
		$color=$red;
		break;
	case 3:
	$r="Administrator";
		$color=$red;
		break;
	case 2:
		$r="Administrator";
		$color=$red;
		break;
	case 1:
		$r="Moderator";
		$color=$orange;
		break;
	default:
	if ($pd['vip']) {
		$r="V.I.P.";
		$color=$yellow;
	}
}

if($ban['pozostalo']>0){
	$r2="Zbanowany";
	$color2=$white;
}
if(strlen($r)>0)
	imagettftext($im, 7, 0, $offset, 13, $color, $arialb, $r);
if(strlen($r)>0) $offset=60+$bbox[2];
	else $offset=5+$bbox[2];
if($ban['pozostalo']>0){
	if(strlen($r2)>0)
		imagettftext($im, 8, 0, $offset, 13, $color2, $veramobd, $r2);
}
$bbox[2]=0;

if ($pd['id_gang']>0) {
	switch($pd['gangrank']){
		case "member": $r="Czlonek"; break;
		case "leader": $r="Lider"; break;
		case "owner": $r="Zalozyciel"; break;
		case "viceowner": $r="Współzalozyciel"; break;
	}
	imagettftext($im, 7,0,5,27,$white,$arialb,$r." ".$pd['gangname'].", ");
	$bbox=imagettfbbox(7, 0, $arialb, $r." ".$pd['gangname'].",  ");
}

$offset=5+$bbox[2];

imagettftext($im, 7, 0, $offset, 27, $white, $arialb, "respekt:");
$bbox=imagettfbbox(7, 0, $arialb, "respekt: ");
$offset+=$bbox[2];
imagettftext($im, 8,0,$offset,27, $white, $arial, $pd['respect']);
$bbox=imagettfbbox(8,0,$arialb, $pd['respect']."   ");
$offset+=$bbox[2];

if($pd['skill']>0) {
    imagettftext($im, 7, 0, $offset, 27, $white, $arialb, "skill: ");
    $bbox=imagettfbbox(7,0,$arialb, "skill: ");
    $offset+=$bbox[2];
    imagettftext($im, 8,0,$offset,27, $white, $arial, $pd['skill']);
}
header('Content-Type: image/png');
imagepng($im,$filename);
imagedestroy($im);
readfile($filename);
// EOF