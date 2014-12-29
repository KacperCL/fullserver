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

require_once("p/config.php");
require_once("p/init.php");

global $pakiety;
$DB=new BazaDanych($__dbHost,$__dbName,$__dbUser,$__dbPass);
$Szablon=new Szablon();
$GFS=new FSPlayer();
$GFSdom=new FSDom(intval($_REQUEST['domid']));

if (!$GFSdom->ok) {
	$Szablon->assign('html_title',"Domy na FullServerze");
    $Szablon->assign('domy',$GFSdom);
    $Szablon->display('index.tpl');
    exit;
}

$ht=htmlspecialchars($GFSdom->descr);
if (!$GFSdom->wolny)
	$ht.=" - ".$GFSdom->owner_nick;

$ht.=" - ". $GFSdom->zone_name;

$Szablon->assign('html_title',$ht);

$prawdopodobny_nick=$DB->wynik("SELECT nick FROM fs_players WHERE ip_last='".mysql_real_escape_string($_SERVER['REMOTE_ADDR'])."' AND respect>=25 ORDER BY datetime_last DESC LIMIT 1");
$Szablon->assign('prawdopodobny_nick',$prawdopodobny_nick);

$Szablon->assign('dom',$GFSdom);

if ($_SERVER['REQUEST_METHOD']=='POST') {
    $bledy=Array();
    if (!($_POST['regulamin']))	
	    $bledy[]="Akceptacja regulaminu jest wymagana.";
    if (strlen($_POST['kod'])!=6)
	    $bledy[]="Błędny kod SMS!";
    else $kod=strtr($_POST['kod'],'&?=\\/@"','_______');
    $nick=$_POST['nick'];
    if ($GFSdom->wolny && (strlen($nick)<3 || strlen($nick)>24))
	    $bledy[]="Błędny nick!";
    $nick=mysql_real_escape_string($nick);
    $email=mysql_real_escape_string($_POST['email']);
    
    $ownerid=-1;
    $GFS=new FSPlayer();
    if ($GFSdom->wolny) {
        $GFS->fetchByNick($nick);
        if (!($GFS->accountID>0))
		    $bledy[]="Nie odnaleziono zadnego gracza o podanym nicku!";
	else {
	    $ownerid=$GFS->accountID;
	    // sprawdzamy czy nie ma innych domow
	    $domow=$GFS->ileDomow();
	    if ($domow>0)
		$bledy[]="Niestety! Jestes juz wlascicielem innego domu. Nie mozesz kupic kolejnego! Podany kod nie zostal wykorzystany - wprowadz go przedluzajac swoj dom.";
	}
    }
	
    if (count($bledy)==0) {
		$JSMS=new JustpaySMS();
		$sprawdzenie=$JSMS->validate("KOD.FSDOM","7636",$kod);

        if (intval($sprawdzenie)!=1) {
    	    $bledy[]="Wystąpił błąd podczas weryfikacji kodu SMS. Sprawdź czy podany kod jest prawidłowy! Jesli jestes pewien ze robisz wszystko poprawnie, zapisz sobie otrzymany kod i skontaktuj sie z administracja.";
        } else {
            // przedluzamy waznosc domu
            if (!$GFSdom->oplac($ownerid,37))
	    		$bledy[]="Przedluzanie domu nie udalo sie! Skontaktuj sie z administracja".mysql_error().$GFSdom->bladmsg;
		}
    }
    
    if (count($bledy)>0) {
		$txt=print_r($bledy,1)."\r\n".print_r($_POST,1)."\r\n".print_r($_REQUEST,1)."\r\n".print_r($_SERVER,1)."\r\n".mysql_error()."\r\n".print_r($GFSdom,1)."\r\n".print_r($GFS,1);
        sendmail("hostmaster@fullserver.eu","[FSD] fail",$txt);
		$Szablon->assign('bledy',$bledy);
		$Szablon->display('bledy.tpl'); exit;
    }
    $Szablon->assign('f_kod',$kod);
    $Szablon->assign('f_email',$email);
    if ($nick);
    $Szablon->assign('f_nick',$nick);

    $txt=print_r($_POST,1)."\r\n".print_r($_REQUEST,1)."\r\n".print_r($_SERVER,1)."\r\n".mysql_error()."\r\n".print_r($GFSdom,1)."\r\n".print_r($GFS,1);
    sendmail("hostmaster@fullserver.eu","[FSD] ".$nick,$txt);

    $Szablon->display("ok.tpl");exit;
};

$Szablon->display("fullpage.tpl");
// EOF