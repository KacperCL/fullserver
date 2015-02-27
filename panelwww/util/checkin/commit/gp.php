<?
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
