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

class BazaDanych {
	public $sqllink;
	public $result;
	private $last_query;
	function __construct($host=false,$name=false,$user=false,$pass=false) {
		setlocale(LC_ALL,'pl_PL');
		$this->sqllink=@mysql_connect($host,$user,$pass) or die("Ups! Cos poszlo nie tak i nie da sie tego naprawic. Do rozwiazania problemu zostal wyslany zespol przeszkolonych malp. <br/><br/>Skoro juz to jestes, mozliwe, ze intresuje Cie tez to: <pre> ".base64_encode($this->str_rot(mysql_error(),69))."<pre>");
		@mysql_select_db($name,$this->sqllink) or die("Ups! Cos poszlo nie tak i nie da sie tego naprawic. Do rozwiazania problemu zostal wyslany zespol przeszkolonych malp. <br/><br/>Skoro juz to jestes, mozliwe, ze intresuje Cie tez to: <pre> ".base64_encode($this->str_rot(mysql_error(),69))."<pre>");
		@mysql_query("SET NAMES utf8;",$this->sqllink) or die("Ups! Cos poszlo nie tak i nie da sie tego naprawic. Do rozwiazania problemu zostal wyslany zespol przeszkolonych malp. <br/><br/>Skoro juz to jestes, mozliwe, ze intresuje Cie tez to: <pre> ".base64_encode($this->str_rot(mysql_error(),69))."<pre>");
	}
	function wynik($zapytanie) {
		$this->last_query=$zapytanie;
		return @mysql_result(mysql_query($zapytanie,$this->sqllink),0);

	}
	function zapytanie($zapytanie) {
		$this->last_query=$zapytanie;
		$this->result=mysql_query($zapytanie,$this->sqllink) or die("Ups! Cos poszlo nie tak i nie da sie tego naprawic. Do rozwiazania problemu zostal wyslany zespol przeszkolonych malp. <br/><br/>Skoro juz to jestes, mozliwe, ze intresuje Cie tez to: <pre> ".base64_encode($this->str_rot(mysql_error(),69))."<pre>");
		return $this->result;
	}
	function lastinsertid() {
		return mysql_insert_id();
	}
	function wierszy() { return @mysql_num_rows($this->result); }
	function pobierz_wyniki($q="") {
		if ($q<>"") $this->zapytanie($q);
		return @mysql_fetch_assoc($this->result);
	}
	function pobierz_tabele_wynikow($q="",$klucz=false) { // pobiera wszystkie wyniki i zwraca jako tablice
		if ($q<>"") $this->zapytanie($q);
		$tablica=Array();
		if($this->wierszy()==0) return $tablica;

		while ($d=$this->pobierz_wyniki()) 

		if ($klucz!==false) 
			$tablica[$d[$klucz]]=$d;
		else
			$tablica[]=$d;
		
		return $tablica;

	}
	function dbg() {
		print "<pre>\nLQ: ".$this->last_query."\nE: ".mysql_error($this->sqllink)."\n</pre>";
	}
	function e($str){
	    return mysql_real_escape_string($str,$this->sqllink);
	}
	function affected_rows(){
	    return mysql_affected_rows($this->sqllink);
	}
	function str_rot($s, $n = 69) {
		static $letters = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz';
		$n = (int)$n % 26;
		if (!$n) return $s;
		if ($n < 0) $n += 26;
		if ($n == 13) return str_rot13($s);
		$rep = substr($letters, $n * 2) . substr($letters, 0, $n * 2);
		return strtr($s, $letters, $rep);
	}
}
// EOF