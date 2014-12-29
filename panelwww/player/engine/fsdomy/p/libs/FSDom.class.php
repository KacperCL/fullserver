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

class FSDom {

    public $ok=false;
    public $id=-1;
    public $descr;
    public $X,$Y,$Z;
    public $koszt, $ownerid, $owner_nick, $paidTo, $dni, $vehicles_allowed, $vehicle_model, $vehicle_name;
    public $wolny;
    public $bladmsg="";
	public $zdj;
	public $wszystkieID=Array();
	public $wolneID=Array();
	public $prevID,$nextID,$prevFID,$nextFID;
	public $zone_name;
    
    function __construct($domid){
 		global $DB;
	    if (intval($domid)>0) {
		$this->id=intval($domid);

		$dane=$DB->pobierz_wyniki("select h.id,h.descr,h.X,h.Y,h.Z,h.koszt,h.ownerid,h.restrict_gang,g.name gang_name,p.nick owner_nick,h.paidTo,datediff(paidTo,NOW()) dni,h.vehicles_allowed,hv.model vehicle_model,v.name vehicle_name FROM fs_houses h LEFT JOIN fs_players p ON p.id=h.ownerid LEFT JOIN fs_houses_vehicles hv on hv.houseid=h.id LEFT JOIN fs_vehicles v ON v.vid=hv.model LEFT JOIN fs_gangs g ON g.id=h.restrict_gang where h.id=".$this->id." AND h.homeX!=0 LIMIT 1");

		foreach ($dane as $k=>$v)
		    $this->$k=$v;
		if ($this->ownerid==0 || $this->dni<0) $this->wolny=true; else $this->wolny=false;	
		$this->ok=true;

		$this->zone_name=getZoneName($this->X, $this->Y, $this->Z);

		$zdj=IMG_dir."dom_".sprintf("%d", $domid).".jpg";
		
		if (file_exists($zdj))
			$this->zdj="/s/i/domy/dom_".sprintf("%d",$domid).".jpg";

	    }
		$r=$DB->pobierz_tabele_wynikow("select id,(ownerid=0 or paidto<DATE(NOW())) wolny from fs_houses order by id asc;");
		$cidx=-1;
		$widx=-1;
		foreach ($r as $k=>$v) {
			$this->wszystkieID[]=$v['id'];
			if ($v['wolny']) {
				$this->wolneID[]=$v['id'];
				if ($cidx<0) $widx++;
			}

			if ($v['id']==intval($domid)) $cidx=$k; 
		}

        if ($cidx>0)
			$this->prevID=$this->wszystkieID[$cidx-1];
		if ($cidx<sizeof($this->wszystkieID))
			$this->nextID=$this->wszystkieID[$cidx+1];

		if ($widx>=-1)
			$this->nextFID=$this->wolneID[$widx+1];
		
		if ($widx<sizeof($this->wolneID))
			$this->prevFID=$this->wolneID[$widx-1];
    }
    function pobierzWolneDomy(){
	    global $DB;
    }
    function oplac($ownerid,$dni){
	    global $DB;
	    if ($this->id<=0) {
		$this->bladmsg="NAstapil blad przy okreslaniu domu";
		return false;
	    }
	    if ($this->wolny && $ownerid<=0) {
	    	$this->bladmsg="NAstapil blad przy okreslaniu kupujacego";
		return false;
	    }
		
	    if ($this->wolny) {
		$DB->zapytanie("update fs_houses set ownerid=".intval($ownerid).",paidTo=NOW() + INTERVAL ".intval($dni)." DAY where id=".intval($this->id)." LIMIT 1");
		if (mysql_affected_rows()>0) return true; else return false;
	    } elseif (!$this->wolny && $ownerid>0) { 
	    	$this->bladmsg="$ownerid ".$this->ownerid."Ten dom zostal wlasnie wykupiony przez kogos innego! Twoj kod SMS nie zostal wykorzystany, wprowadz go przy zakupie innego domu.";
		return false;
	    } else {
		$DB->zapytanie("update fs_houses set paidTo=IF( paidTo>NOW(),paidTo,NOW()) + INTERVAL ".intval($dni)." DAY where id=".intval($this->id)." LIMIT 1");
		if (mysql_affected_rows()>0) return true; else return false;
	    }
    }
    function listaDomow(){
	    global $DB;
		$q="select h.id,h.descr,h.X,h.Y,h.Z,h.koszt,h.ownerid,h.restrict_gang,p.nick owner_nick,h.paidTo,datediff(paidTo,NOW()) dni,h.vehicles_allowed,hv.model vehicle_model,v.name vehicle_name FROM fs_houses h 
			LEFT JOIN fs_players p ON p.id=h.ownerid LEFT JOIN fs_houses_vehicles hv on hv.houseid=h.id LEFT JOIN fs_vehicles v ON v.vid=hv.model Where h.homeX!=0 GROUP BY h.id ORDER BY id ASC";
		$dane=$DB->pobierz_tabele_wynikow($q);
	    return $dane;
    }
}
// EOF