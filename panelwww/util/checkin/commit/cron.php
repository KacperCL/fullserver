<?
require_once("/home/fullserv/public_html/fsutil.i32.pl/checkin/__libs/BazaDanych.class.php");
require_once("/home/fullserv/public_html/fsutil.i32.pl/checkin/__libs/__config.php");
$RDB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);

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

if(date("d")==01 && date("h")==03){
	$RDB->zapytanie("TRUNCATE `fs_races_records`");
	$RDB->zapytanie("TRUNCATE `fs_drift_records`");
	$RDB->zapytanie("TRUNCATE TABLE  `fs_zones_gangscore`");
	$RDB->zapytanie("UPDATE `fs_gangs` SET `respect`=0");
	print 'month cleanup';
}

if(date("w")==01 && date("h")==03){
	$RDB->zapytanie("TRUNCATE TABLE  `fs_players_arenascore_week`");
	print 'week cleanup';
}
?>