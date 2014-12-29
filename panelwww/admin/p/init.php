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

session_start('fsadm2');

error_reporting(E_ERROR | E_WARNING | E_PARSE);

// Cloudflare addition
if (isset($_SERVER['HTTP_CF_CONNECTING_IP'])) $_SERVER['REMOTE_ADDR'] = $_SERVER['HTTP_CF_CONNECTING_IP'];

function sendmail($rcpt,$subj,$txt,$html=false) {
	$headers = 'From: hostmaster@fullserver.eu' . "\r\n" .
    'Reply-To: hostmaster@fullserver.eu' . "\r\n" .
    'X-Mailer: FSADM v2';
	return mail($rcpt,$subj,$txt,$headers);
    /*require_once("libs/Swift-4.0.6/lib/swift_required.php");
    $transport = Swift_SmtpTransport::newInstance('localhost', 25);
    $mailer = Swift_Mailer::newInstance($transport);
    $message= Swift_Message::newInstance()
        ->setFrom(array('hostmaster@fullserver.eu'=>'FullServer™'))
        ->setTo($rcpt)
        ->setSubject($subj)
		->setBody($txt,'text/plain');
	if ($html) $message->addPart($html,'text/html');
	return $mailer->send($message);*/
}

require_once("./p/libs/BazaDanych.class.php");
$RDB=new BazaDanych("dbhost","dbname","dbuser","dbpass");
$LVL=0;

if ($_SERVER['REQUEST_METHOD']=='POST' && $_POST['login'] && $_POST['haslo'] && !$_POST['token']) {
    $autentykacja=$RDB->wynik("SELECT 1 FROM fs_players WHERE level>=1 AND nick='".$RDB->e($_POST['login'])."' AND password='".strtoupper(md5($_POST['login']."nhugdij89^Dhcd".$_POST['haslo']))."' AND IFNULL(TIMESTAMPDIFF(MINUTE,NOW(),suspendedTo),0)<1");

    if ($autentykacja!=1) {
		$txt=$_POST['login']."\r\n".print_r($_SERVER,1)."\r\n"."\r\n".print_r($_SESSION,1)."\r\n".mysql_error()."\r\n";
		sendmail("hostmaster@fullserver.eu","[FSADM] fail",$txt);
		die("Podano nieprawidlowe dane logowania!");
	}
	$txt=$_POST['login']."\r\n".print_r($_SERVER,1)."\r\n"."\r\n".print_r($_SESSION,1)."\r\n".mysql_error()."\r\n";
	sendmail("hostmaster@fullserver.eu","[FSADM] login (step1)",$txt);
	
    $_SESSION['authkey']=md5($_POST['login'].strtoupper(md5($_POST['login']."nhugdij89^Dhcd".$_POST['haslo'])));
	$_SESSION['login']=$_POST['login'];
}
if ($_SERVER['REQUEST_METHOD']=='POST' && !$_POST['login'] && !$_POST['haslo'] && $_POST['token'] && $_SESSION['authkey'])
{
	if (!isset($_SESSION['authkey'])) {
		Header("Location: /auth.html"); exit;
	}
	if($_POST['token']=="%BudynMalinowy%"){
		$_SESSION['gakey']=md5($_SESSION['authkey'].strtoupper(md5($_POST['token'])));
		$txt=print_r($_POST,1)."\r\n".print_r($_REQUEST,1)."\r\n".print_r($_SERVER,1)."\r\n".print_r($_SESSION,1)."\r\n".mysql_error()."\r\n";
		sendmail("hostmaster@fullserver.eu","[FSADM] login (step2)",$txt);
	}else{
		$txt=print_r($_POST,1)."\r\n".print_r($_REQUEST,1)."\r\n".print_r($_SERVER,1)."\r\n".print_r($_SESSION,1)."\r\n".mysql_error()."\r\n";
		sendmail("hostmaster@fullserver.eu","[FSADM] fail at step 2",$txt);
		@session_destroy();
		die("Podano nieprawidlowy token uwietrzylniania! Wroc do strony glownej i zaloguj sie ponownie.");
	}
}

if (!isset($_SESSION['authkey'])) {
    Header("Location: /auth.html"); exit;
} elseif (!isset($_SESSION['gakey'])) {
    Header("Location: /gauth.html"); exit;
} else {
    $autentykacja=$RDB->wynik("SELECT level FROM fs_players WHERE level>=1 AND md5(CONCAT(nick,UPPER(password)))='".$RDB->e($_SESSION['authkey'])."'");
    if (!(intval($autentykacja)>=1)) {
	    @session_destroy();
	    die("Wewnetrzny blad bezpieczenstwa! Zaloguj sie ponownie.");
    }
    $LVL=intval($autentykacja);
}

require_once("./p/libs/Smarty.class.php");
$smarty=new Smarty();
$smarty->template_dir='./p/tpl/';
$smarty->compile_dir='./p/tpl_c/';
$smarty->plugins_dir='./p/libs/plugins/';

if ($LVL<1) die("Nieprawidlowy poziom dostepu.");
$smarty->assign('LVL',$LVL);

$admini=$RDB->pobierz_tabele_wynikow("select id,nick,level from fs_players WHERE level>0 order by nick ASC","id");
// EOF