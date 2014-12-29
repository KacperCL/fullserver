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

require_once('p/libs/smarty/Smarty.class.php');
class Szablon extends Smarty {
 public $plik="";
 public $seed=-1;
 public $allow_php_tag = true;
 private $html_title="";
 private $html_js=Array();
 private $html_css=Array();
 private $bc=Array();
 
 public function __construct(){
	parent::__construct();
	global $CFG_page_name;

  $this->template_dir='p/templates/';
  $this->compile_dir='p/templates_c/';
  
  global $CFG_page_name;
  $this->html_title=$CFG_page_name;
 }

 public function dodaj_tytul($txt) {
    $this->html_title.=" - ".$txt;
 }
 public function dodaj_js($plik) {
    $this->html_js[]=$plik;
 }
 
 public function dodaj_css($plik) {
    $this->html_css[]=$plik;
 }
 
 public function blad_krytyczny($txt) {
    $this->append('_error_msgs',$txt);
    $this->display('_error.tpl');
    exit;
 }
 public function add_bc($txt,$url){
	$this->bc[]=Array('txt'=>$txt,'url'=>$url);
	$this->dodaj_tytul($txt);
 }
 
 public function wyswietl() {
    if ($this->plik=="") 
	$this->blad_krytyczny('Nie wybrano szablonu!');

    $this->assign('html_title',$this->html_title);
    $this->assign('html_js',$this->html_js);
    $this->assign('html_css',$this->html_css);    
	$this->assign('breadcrumbs',$this->bc);
	if ($this->seed>=0)
	    $this->display($this->plik,$this->seed);
	else
	    $this->display($this->plik);
 }
 };
// EOF