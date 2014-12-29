<?
require_once("/home/fullserv/public_html/fsutil.i32.pl/checkin/__libs/BazaDanych.class.php");
require_once("/home/fullserv/public_html/fsutil.i32.pl/checkin/__libs/__config.php");
$RDB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);

if(!isset($_GET['id'])) die();

$d=$RDB->wynik("SELECT vip FROM fs_players WHERE id=".$_GET['id']." LIMIT 1");

echo "<br/>";
echo "Obecnie " . $d ."<br/>";

$RDB->zapytanie("UPDATE fs_players SET vip = IF(vip>NOW(),vip,NOW())+INTERVAL ".$_GET['dni']." DAY WHERE id=".$_GET['id']." LIMIT 1");

$d=$RDB->wynik("SELECT vip FROM fs_players WHERE id=".$_GET['id']." LIMIT 1");
	 
echo "Po zmianie " . $d ."<br/>";

?>
