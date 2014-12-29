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

function kiedy($data) {
        $ts=strtotime($data);
        $ts=time()-$ts;
		if ($ts<0) return "teraz";
        // sekundy
        if ($ts<60) return dli($ts,'sekundę','sekundy','sekund')." temu";
        $ts=round($ts/60);
        // minuty
        if ($ts<60) return dli($ts,'minutę','minuty','minut')." temu";
        $ts=round($ts/60);
        // godziny
        if ($ts<24) return dli($ts,'godzinę', 'godziny','godzin')." temu";
        $ts=round($ts/24);
        // dni
        if ($ts<7) return dli($ts,'dzień','dni','dni')." temu";
        // tygodnie
        //if (($ts/7)<4) return dli(round($ts/7),'tydzie�','tygodnie','tygodni')." temu";
        if (($ts/7)<4) return dli($ts,'dzień','dni','dni')." temu";
        $ts=round($ts/30.5);
        // miesiace
        if ($ts<12) return dli($ts,'miesiąc','miesiące','miesięcy')." temu";
        $ts=round($ts/12);
        // lata
        return dli($ts,'rok','lata','lat')." temu";
};

function kiedy2($data) {
        $ts=strtotime($data);
        $ts=time()-$ts;
        // sekundy
        if ($ts<60) return dli($ts,'sekundy','sekund','sekund',true);
        $ts=round($ts/60);
        // minuty
        if ($ts<60) return dli($ts,'minuty','minut','minut',true);
        $ts=round($ts/60);
        // godziny
        if ($ts<24) return dli($ts,'godziny', 'godzin','godzin',true);
        $ts=round($ts/24);
        // dni
        if ($ts<7) return dli($ts,'dnia','dni','dni',true);
        // tygodnie
        //if (($ts/7)<4) return dli(round($ts/7),'tygodnia','tygodni','tygodni');
        if (($ts/7)<4) return dli($ts,'dnia','dni','dni',true);
        $ts=round($ts/30.5);
        // miesiace
        if ($ts<12) return dli($ts,'miesiąca','miesięcy','miesięcy',true);
        $ts=round($ts/12);
        // lata
        return dli($ts,'roku','lat','lat',true);
};

function kiedy_short($data) {
        $ts=strtotime($data);
        $ts=time()-$ts;
        // sekundy
        if ($ts<60) return "-".$ts."s"; 
        $ts=round($ts/60);
        // minuty
        if ($ts<60) return "-".$ts."m";
        $ts=round($ts/60);
        // godziny
        if ($ts<24) return "-".$ts."h";
        $ts=round($ts/24);
        // dni
        if ($ts<7) return "-".$ts."d";
        // tygodnie
        //if (($ts/7)<4) return dli(round($ts/7),'tydzie�','tygodnie','tygodni')." temu";
        if (($ts/7)<4) return "-".$ts."d";
        $ts=round($ts/30.5);
        // miesiace
        if ($ts<12) return "-".dli($ts,'m-c','m-ce','m-cy')."";
        $ts=round($ts/12);
        // lata
        return "-".$ts."r";
};

function dli($x,$a,$b,$c,$adc=false) {
        if ($x==1) return ($adc==false?"":$x." ").$a;
        elseif (
        ($x%10>1) && ($x%10<5)  &&
        !(($x%100>=10) && ($x%100<=21))
        ) return $x." ".$b;
        return $x." ".$c;
};

function dli2($x,$a,$b,$c) {
        if ($x==1) return $a;
        elseif (
        ($x%10>1) && ($x%10<5)  &&
        !(($x%100>=10) && ($x%100<=21))
        ) return $b;
        return $c;
};
// EOF