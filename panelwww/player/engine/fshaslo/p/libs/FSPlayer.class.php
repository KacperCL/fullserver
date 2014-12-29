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
 
class FSPlayer {
	public $accountID;
	public $respect;
	
	function createRandomPassword() {
		$chars = "abcdefghijkmnopqrstuvwxyz023456789";
		srand((double)microtime()*1000000);
		$i = 0;
		$pass = '' ;
		while ($i <= 7) {
			$num = rand() % 33;
			$tmp = substr($chars, $num, 1);
			$pass = $pass . $tmp;
			$i++;
		}
		return $pass;
	}
	function fetchByNick($nickname) {
		global $DB;
		$d=$DB->pobierz_wyniki("select id,respect,nick from fs_players where nick ='".mysql_real_escape_string($nickname)."' LIMIT 1;");
		if (!$d || !(intval($d[id])>0)) {
			$this->accountID=-1;
			$this->respect=0;
		} else {
			$this->accountID=intval($d[id]);
			$this->respect=intval($d[respect]);
			$this->nick=$d[nick];
		}
	}
	function fetchByNickOffline($nickname) {
		global $DB;
		$d=$DB->pobierz_wyniki("select id,respect,nick from fs_players where nick ='".mysql_real_escape_string($nickname)."' AND active_server=0 LIMIT 1;");
		if (!$d || !(intval($d[id])>0)) {
			$this->accountID=-1;
			$this->respect=0;
		} else {
			$this->accountID=intval($d[id]);
			$this->respect=intval($d[respect]);
			$this->nick=$d[nick];
		}
	}
	function fetchByemail($email) {
		global $DB;
		$d=$DB->pobierz_wyniki("select id,respect,nick from fs_players where email ='".mysql_real_escape_string($email)."' LIMIT 1;");
		if (!$d || !(intval($d[id])>0)) {
			$this->accountID=-1;
			$this->respect=0;
		} else {
			$this->accountID=intval($d[id]);
			$this->respect=intval($d[respect]);
			$this->nick=$d[nick];
		}
	}
	function RestartHasla($nick,$email){
	    if ($this->accountID>0) {
		global $DB;
		$password=$this->createRandomPassword();
		$DB->zapytanie("UPDATE fs_players SET password='".strtoupper(md5($this->nick."nhugdij89^Dhcd".$password))."' WHERE id=".$this->accountID." LIMIT 1");
		
		$subject = 'Przypomnienie hasła do Twojego konta na serwerze SA-MP';

		$message = '
		<html>
		<head>
		<title>Twoje nowe hasło do konta na FullServerze</title>
		</head>
		<body>
		<p>Ktoś wykonał procedurę przypomnienia hasła dla Twojego konta i podał Twój nick oraz ten adres email, który był przypisany do Twojego konta na serwerze.</p>
		<p>Jeżeli to nie byłeś Ty - zgłoś to jak najszybciej do Administracji!</p><br/><br/>
		<p>Twoje nowe hasło do konta na serwerze: <strong>'.$password.'</strong></p>
		</body>
		</html>
		';
		
		$message_normal = '
		Twoje nowe hasło do konta na FullServerze

		Ktoś wykonał procedurę przypomnienia hasła dla Twojego konta i podał Twój nick oraz ten adres email, który był przypisany do Twojego konta na serwerze.
		Jeżeli to nie byłeś Ty - zgłoś to jak najszybciej do Administracji!
		Twoje nowe hasło do konta na serwerze: '.$password.'
		';
		
		sendmail($email,$subject,$message_normal,$message);
		return mysql_affected_rows()>0?true:false;
	    } else return false;
	}
}
// EOF