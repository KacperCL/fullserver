<?php
/**
 * <pre>
 * FullServer.eu
 * Part of FSADM v2
 * </pre>
 *
 * @author 		Wielebny, eider
 * @copyright	(c) 2011 - 2013 FullServer.eu
 * @package		FSADM v2
 * @link		http://fullserver.eu
 *
 */

require_once("./p/libs/BazaDanych.class.php");
$RDB=new BazaDanych("178.19.106.171","db_2152","db_2152","Nca7Jzq6LK2em4DK");

$donagrodzenia=$RDB->pobierz_tabele_wynikow("SELECT po.polecil,p2.nick polecil_nick,po.polecona,p1.nick polecona_nick,p1.active_server polecona_active,p2.active_server polecil_active
         FROM fs_polecenia po JOIN fs_players p1 ON p1.id=po.polecona JOIN fs_players p2 ON p2.id=po.polecil WHERE p1.respect>=2000 AND uniewazniony=0 AND rewarded=0");
	 
foreach ($donagrodzenia as $d){ 
	print "\n";
	print "Polecona: " . $d['polecona_nick'] . " ".  $d['polecona']." a: ".$d['polecona_active']."\n";
	print "Polecil: " . $d['polecil_nick'] . " " . $d['polecil']." a: ".$d['polecil_active']."\n";

	if (intval($d['polecona_active'])==0 && intval($d['polecil_active'])==0) {
	    $q1="UPDATE fs_players SET respect=respect+1000,vip=IF(vip>NOW(),vip,NOW())+INTERVAL 11 DAY WHERE id IN (".intval($d['polecil']).",".intval($d['polecona']).")";
		$RDB->zapytanie($q1);
	    print $q1."\n";

	    $q2="UPDATE fs_polecenia SET rewarded=1,reward_date=NOW() WHERE polecona=".intval($d['polecona'])." LIMIT 1";
		$RDB->zapytanie($q2);
	    print $q2."\n";
	}
}
// EOF