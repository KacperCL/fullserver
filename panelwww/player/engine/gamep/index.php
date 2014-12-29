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

require_once("p/config.php");
require_once("p/init.php");
global $pakiety;
$DB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);
$GFS=new FSPlayer();
$Szablon=new Szablon();

$Szablon->assign('pakiety',$pakiety);

if ($_SERVER['REQUEST_METHOD']=='POST') {
    $bledy=Array();
    if (!($_POST['regulamin']))	
	    $bledy[]="Akceptacja regulaminu jest wymagana.";
    if (strlen($_POST['kod'])!=8) 
	    $bledy[]="Błędny kod SMS!";
    else $kod=strtr($_POST['kod'],'&?=\\/@"','_______');
    $nick=$_POST['nick'];
    if (strlen($nick)<3 || strlen($nick)>24)
	    $bledy[]="Błędny nick!";
    $nick=mysql_real_escape_string($nick);
    $email=mysql_real_escape_string($_POST['email']);
    $gamep=intval($_POST['gamep']);
    if (!(array_key_exists($gamep, $pakiety))) 
	$bledy[]="Nie odnaleziono informacji o wybranym pakiecie!";
    $pakiet=$pakiety[$gamep];
    
    $GFS=new FSPlayer();
    $GFS->fetchByNickOffline($nick);
    if (!($GFS->accountID>0))
	    $bledy[]="Nie odnaleziono zadnego gracza o podanym nicku! Być moze jestes ONLINE?";
	
    if (count($bledy)==0) {
        $JSMS=new JustpaySMS();
		$sprawdzenie=$JSMS->validate($pakiet['kod'],$pakiet['numer'],$kod);
        if (intval($sprawdzenie)!=1) {
    	    $bledy[]="Wystąpił błąd podczas weryfikacji kodu SMS. Sprawdź czy podany kod jest prawidłowy! Jesli jestes pewien ze robisz wszystko poprawnie, zapisz sobie otrzymany kod i skontaktuj sie z administracja.";
		} else {
            if (!$GFS->AddGamePoints($gamep))
    		$bledy[]="Doladowanie GamePoints nie powiodlo sie z nieznanej przyczyzny. Zapisz otrzymany kod aktywacyjny i skontaktuj sie z administracja! ".mysql_error();
		}
    }
    
    if (count($bledy)>0) {
		$Szablon->assign('bledy',$bledy);
		sendmail("hostmaster@fullserver.eu","[GAMEP] Bledne ".$gamep." ".$nick,"Blad: ".print_r($bledy,1)."\n\n".print_r($_POST,1)."\n\n".print_r($_SERVER,1));
		$Szablon->display('bledy.tpl'); exit;
    }
    
    $Szablon->assign('f_gamep',$gamep);
    $Szablon->assign('f_nick',$nick);
    $Szablon->assign('f_email',$email);
    $Szablon->assign('f_kod',$kod);
    sendmail("hostmaster@fullserver.eu","[GAMEP] ".$gamep." ".$nick,print_r($_POST,1)."\n\n".print_r($_SERVER,1));
    $Szablon->display("ok.tpl");exit;

};

$Szablon->display("fullpage.tpl");
// EOF