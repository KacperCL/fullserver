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
    37=>Array(  'dni'=> 37, 'numer'=> 7636, 'kod'=> 'KOD.FSVIP', 'koszt'=> 7.38 ),
    52=>Array( 'dni'=> 52, 'numer'=> 7936, 'kod'=> 'KOD.FSVIP', 'koszt'=> 11.07 ),
    97=>Array(  'dni'=> 97, 'numer'=> 91955, 'kod'=> 'KOD.FSVIP', 'koszt'=> 23.37 ),
    127=>Array( 'dni'=> 127, 'numer'=> 92555, 'kod'=> 'KOD.FSVIP', 'koszt'=>30.75 )
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
}
// EOF