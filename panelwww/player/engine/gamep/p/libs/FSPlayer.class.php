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
	
	function fetchByNick($nickname) {
		global $DB;
		$d=$DB->pobierz_wyniki("select id,respect from fs_players where nick ='".mysql_real_escape_string($nickname)."' LIMIT 1;");
		if (!$d || !(intval($d[id])>0)) {
			$this->accountID=-1;
			$this->respect=0;
		} else {
			$this->accountID=intval($d[id]);
			$this->respect=intval($d[respect]);
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
	function VIPPrzedluz($dni){
	    $dni=intval($dni);
	    if ($this->accountID>0 && $dni>0) {
		global $DB;
		$DB->zapytanie("UPDATE fs_players SET vip = IF(vip>NOW(),vip,NOW())+INTERVAL ".$dni." DAY WHERE id=".intval($this->accountID)." LIMIT 1");
		return mysql_affected_rows()>0?true:false;
	    } else return false;
	}
	function AddGamePoints($gamep){
	    $gamep=intval($gamep);
	    if ($this->accountID>0 && $gamep>0) {
		global $DB;
		$DB->zapytanie("UPDATE fs_players SET gamep = gamep+".$gamep." WHERE id=".$this->accountID." LIMIT 1");
		return mysql_affected_rows()>0?true:false;
	    } else return false;
	}
}
// EOF