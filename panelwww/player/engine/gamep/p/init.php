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

$pakiety=Array(
	69=>Array(  'gamep'=> 69, 'numer'=> 7136, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 1.23 ),// 69, 3
	161=>Array(  'gamep'=> 161, 'numer'=> 7255, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 2.46 ),// 161, 7
	345=>Array(  'gamep'=> 345, 'numer'=> 7455, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 4.92 ), // 345, 15
    690=>Array(  'gamep'=> 690, 'numer'=> 7636, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 7.38 ), // 690, 30
    1035=>Array( 'gamep'=> 1035, 'numer'=> 7936, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 11.07 ), // 1035, 45
	1380=>Array( 'gamep'=> 1380, 'numer'=> 91455, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 17.22 ), // 1380, 60
    2070=>Array(  'gamep'=> 2070, 'numer'=> 91955, 'kod'=> 'KOD.FSEXTRA', 'koszt'=> 23.37 ), // 2070, 90
    2760=>Array( 'gamep'=> 2760, 'numer'=> 92555, 'kod'=> 'KOD.FSEXTRA', 'koszt'=>30.75 ) // 2760, 120
);
		
require_once("config.php");
require_once("../../../p/__config.php");

function autoload($class_name) {
        if (file_exists(dirname(__FILE__)."/libs/".$class_name.".class.php"))
        require_once(dirname(__FILE__)."/libs/".$class_name.".class.php");
}
spl_autoload_register('autoload');

function http_303($url,$message=false) {
    Header("HTTP/1.1 303 See Other");
    Header("Location: $url");
    exit;
}
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
// EOF