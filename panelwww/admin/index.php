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

require_once("./p/init.php");

switch($_REQUEST['a']){
    case 'aktywnosc':
	    require("./p/page.aktywnosc.php");
	    break;
    case 'aktywnosc-adv':
	    require("./p/page.aktywnosc-adv.php");
	    break;
	case 'rankb':
	    require("./p/page.rankb.php");
	    break;
    case 'bany':
	    require("./p/page.bany.php");
	    break;
	case 'mute':
	    require("./p/page.mute.php");
	    break;
    case 'ipbany':
	    require("./p/page.ipbany.php");
	    break;
    case 'bany-ajax':
	    if($_SERVER['REQUEST_METHOD']!="POST") die("Bledne wywolanie");
	    require("./p/page.bany-ajax.php");
	    break;
	case 'mute-ajax':
	    if($_SERVER['REQUEST_METHOD']!="POST") die("Bledne wywolanie");
	    require("./p/page.mute-ajax.php");
	    break;
    case 'ipbany-ajax':
	    if($_SERVER['REQUEST_METHOD']!="POST") die("Bledne wywolanie");
	    require("./p/page.ipbany-ajax.php");
	    break;
	case 'nowyban':
	    require("./p/page.nowyban.php");
	    break;
	case 'szukaj':
	    require("./p/page.szukaj.php");
	    break;
    case 'wyloguj':
	    @session_destroy();
	    Header("Location: auth.html");
	    exit; break;
    default:
	    $smarty->display('start.tpl');
}
// EOF