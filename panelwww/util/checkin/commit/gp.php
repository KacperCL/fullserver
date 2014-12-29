<?
require_once("/home/fullserv/public_html/fsutil.i32.pl/checkin/__libs/BazaDanych.class.php");
require_once("/home/fullserv/public_html/fsutil.i32.pl/checkin/__libs/__config.php");
$RDB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);

if(!isset($_GET['id'])) die();

$d=$RDB->wynik("SELECT gamep FROM fs_players WHERE id=".$_GET['id']." LIMIT 1");

echo "<br/>";
echo "Obecnie " . $d ."<br/>";

$RDB->zapytanie("UPDATE fs_players SET gamep = gamep+".$_GET['gp']." WHERE id=".$_GET['id']." LIMIT 1");

$d=$RDB->wynik("SELECT gamep FROM fs_players WHERE id=".$_GET['id']." LIMIT 1");
	 
echo "Po zmianie " . $d ."<br/>";

?>
