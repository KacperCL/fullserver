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

$CFG_page_name="FullServer - Statystyki";
require_once("p/libs/misc.php");
require_once("p/libs/Szablon.class.php");
$szablon=new Szablon;

$szablon->setCaching(Smarty::CACHING_LIFETIME_SAVED);
$szablon->dodaj_js('/s/js/jquery.dataTables.min.js');
$szablon->dodaj_js('/s/js/statystyki.js');

require_once("p/libs/BazaDanych.class.php");
require_once("p/__config.php");
$RDB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);
$szablon->assign('RDB',$RDB);
$szablon->setCacheLifetime(3600);
switch($_REQUEST['m']){
	case "mapa":
		$szablon->setCacheLifetime(36000);
		$szablon->dodaj_js('http://maps.google.com/maps/api/js?sensor=false');
		$szablon->dodaj_js('/s/js/SanMap.min.js');
		$szablon->plik="m.statystyki.mapa.tpl";
		$szablon->assign("sel",8);
		break;
	case "change":
		$szablon->plik="m.statystyki.changelog.tpl";
		$szablon->assign("sel",7);
		break;
	case "stats-player":
		$szablon->dodaj_js('http://maps.google.com/maps/api/js?sensor=false');
		$szablon->dodaj_js('/s/js/SanMap.min.js');
		$pid=intval($_REQUEST['pid']);
		$szablon->assign("pid",$pid);
		$szablon->seed=$pid;
		$szablon->plik="m.statystyki.gracz.tpl";
		break;
	/*case "api-player":
		// $szablon->setCacheLifetime(0);
		$pid=intval($_REQUEST['pid']);
		$szablon->assign("pid",$pid);
		$szablon->assign("format",$_REQUEST['format']);
		$szablon->seed=$pid.md5($_REQUEST['format']);
		$szablon->plik="m.api.gracz.tpl";
		break;*/
	case "stats-gang":
		$szablon->setCacheLifetime(360);
		$gid=intval($_REQUEST['gid']);
		$szablon->assign("gid",$gid);
		$szablon->seed=$gid;
		$szablon->assign("sel",3);
		$szablon->plik="m.statystyki.gang.tpl";
		break;
	case "stats-race":
		$szablon->dodaj_js('http://maps.google.com/maps?file=api&amp;v=2&amp;key=AIzaSyD9Gx7SAYZigEUXgiKdoQKe_pLwTp9HFUs');
		$szablon->dodaj_js('/s/js/fsmap.js');
		$rid=intval($_REQUEST['rid']);
		$szablon->assign("rid",$rid);
		$szablon->assign("sel",4);
		$szablon->seed=$rid;
		$szablon->plik="m.statystyki.race.tpl";
		break;
	case "stats-drifts":
		$szablon->plik="m.statystyki.drifts.tpl";
		$szablon->assign("sel",5);
		break;
	case "stats-drift":
		$szablon->dodaj_js('http://maps.google.com/maps?file=api&amp;v=2&amp;key=AIzaSyD9Gx7SAYZigEUXgiKdoQKe_pLwTp9HFUs');
		$szablon->dodaj_js('/s/js/fsmap.js');
		$rid=intval($_REQUEST['rid']);
		$szablon->assign("rid",$rid);
		$szablon->assign("sel",5);
		$szablon->seed=$rid;
		$szablon->plik="m.statystyki.drift.tpl";
		break;
	case "stats-gangs":
		$szablon->setCacheLifetime(360);
		$szablon->dodaj_js('http://maps.google.com/maps/api/js?sensor=false');
		$szablon->dodaj_js('/s/js/SanMap.min.js');
		$szablon->assign("sel",3);
		$szablon->plik="m.statystyki.gangs.tpl";
		break;
	case "stats-races":
		$szablon->assign("sel",4);
		$szablon->plik="m.statystyki.races.tpl";
		break;
	case "stats-areny":
		$szablon->assign("sel",2);
		$szablon->plik="m.statystyki.areny.tpl";
		break;
	case "stats-osiagniecia":
		$szablon->assign("sel",1);
		$szablon->plik='m.statystyki.osiagniecia.tpl';
		break;
	case "obsluga":
		$szablon->assign("sel",9);
		$szablon->plik='m.obsluga.tpl';
		break;
	case "polec":
		$szablon->add_bc('Poleć FullServer znajomemu!','/polec/');
		$szablon->assign("sel",6);
		$szablon->plik='m.polec.tpl';
		$szablon->dodaj_js('/s/js/polec.js');
		$szablon->setCacheLifetime(0);
		$polecenia_log=$RDB->pobierz_tabele_wynikow("SELECT po.uniewazniony,po.uniewazniony_powod,po.polecil,p2.nick polecil_nick,po.polecona,p1.nick polecona_nick,p1.respect polecona_respect,po.ts FROM fs_polecenia po JOIN fs_players p1 ON p1.id=po.polecona JOIN fs_players p2 ON p2.id=po.polecil ORDER BY po.ts DESC limit 20;");
		$szablon->assign('polecenia_log',$polecenia_log);

		if ($_SERVER['REQUEST_METHOD']=='POST') {
			$polecona_nick=$_POST['polecona_nick'];
			$polecil_nick=$_POST['polecil_nick'];
			
			if (!(strlen($polecona_nick)>=3)) {
				$szablon->assign('polec_blad','Podano nieprawidlowy nick osoby poleconej');
				break;
			}
			if (!(strlen($polecil_nick)>=3)) {
				$szablon->assign('polec_blad','Podano nieprawidlowy nick osoby polecającej');
				break;
			}
			// pobieramy adres IP i respekt
			$polecona_dane=$RDB->pobierz_wyniki("select id,respect,datediff(now(),datetime_last) kiedy, ip_last from fs_players where nick='".mysql_real_escape_string($polecona_nick)."' LIMIT 1");

			if (!is_array($polecona_dane) or !$polecona_dane['id']) {
				$szablon->assign('polec_blad','Nie znaleziono danych gracza o podanym nicku (osoba polecona). Spróbuj ponownie.');
				break;
			}
			if ($polecona_dane['respect']<25) {
				$szablon->assign('polec_blad','Musisz miec przynajmniej 25 punktów respektu aby wypełnić ten formularz. Wejdź do gry, nabij trochę (nie więcej niż 1000!) i wróc tu.');
				break;
			}
			if ($polecona_dane['respect']>1000) {
				$szablon->assign('polec_blad','Niestety masz więcej niż 1000 punktów respektu, nie możesz już wziąść udziału w tym programie.');
				break;
			}
			if ($polecona_dane['kiedy']>3) {
				$szablon->assign('polec_blad','Twoje konto nie było używane od 3 dni. Zaloguj się na nie, wyloguj i wróć tu - abyśmy mogli potwierdzić, że jest Twoje.');
				break;
			}
			if ($polecona_dane['ip_last']!=$_SERVER['REMOTE_ADDR']) {
				$szablon->assign('polec_blad','Ostatnie logowanie na podane konto było z innego adresu IP niż Twoj obecny. Abyśmy mogli potwierdzić, że to Twoje konto, zaloguj się do gry, wyloguj i wypełnij ponownie ten formularz.');
				break;
			}
			//  pobieramy id osoby polecającej
			$polecil_id=$RDB->wynik("SELECT id FROM fs_players WHERE nick='".mysql_real_escape_string($polecil_nick)."' LIMIT 1");
			if (!($polecil_id>0)) {
				$szablon->assign('polec_blad','Nie znaleziono danych gracza o podanym nicku (osoba polecająca). Spróbuj ponownie.');
				break;
			}
			if ($polecil_id==$polecona_dane['id']) {
				$szablon->assign('polec_blad','Nie mozna polecic samego siebie! :-)');
				break;
			}

			// sprawdzamy czy mamy juz jakies informacje o poleceniu
			$stare_polecenie=$RDB->pobierz_wyniki("select p.polecil,pl.nick polecil_nick,p.ts from fs_polecenia p JOIN fs_players pl ON pl.id=p.polecil where p.polecona=".intval($polecona_dane['id']));
			if (is_array($stare_polecenie)) {
				if ($stare_polecenie['polecil']==$polecil_id) {
					$szablon->assign('polec_blad','Posiadamy już informacje o tym poleceniu :-). Nie musisz wypełniać tego formularza ponownie. Dobij do 2000 respektu, aby otrzymać nagrodę.');
					break;
				} elseif ($stare_polecenie['polecil']>0) {
					$szablon->assign('polec_blad','Posiadamy już zapisaną informacje o poleceniu z innym polecającym. Nie da się jej zmienić.');
					break;
				}
			}

			// zapisujemy informacje o poleceniu
			$RDB->zapytanie("INSERT INTO fs_polecenia SET polecona=".intval($polecona_dane['id']).",polecil=".intval($polecil_id).",".
						"ip='".mysql_real_escape_string($_SERVER['REMOTE_ADDR'])."',ua='".mysql_real_escape_string($_SERVER['HTTP_USER_AGENT'])."'");
			$szablon->assign('polecenia_done',true);
		} else {
			$prawdopodobny_nick=$RDB->wynik("SELECT nick FROM fs_players WHERE ip_last='".mysql_real_escape_string($_SERVER['REMOTE_ADDR'])."' AND respect>=25 AND respect<=1000 ORDER BY datetime_last DESC LIMIT 1");
			$_POST['polecona_nick']=$prawdopodobny_nick;
		}
		break;

	default:
		$szablon->plik='m.404.tpl';

}
$szablon->clearCache('_menu.tpl');
$szablon->wyswietl();
// EOF