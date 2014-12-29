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

#include <a_samp>
#include <3rdparty/streamer>
#define FILTERSCRIPT
#include <3rdparty/dc_cmd>

#define LAMPION_ODLEGLOSC_Z 300
#define LAMPION_ODLEGLOSC_X 525
#define LAMPION_ODLEGLOSC_Y 925
#define LAMPION_OBIEKT  3534
#define LAMPION_CZAS 10000
#define LAMPION_CZAS2 600000

new lampion_lu[MAX_PLAYERS];

forward lampion_usun(obiekt);

CMD:lampion(playerid){
  lampion_pusc(playerid);
  return 1;
}

lampion_pusc(playerid){
  if (GetTickCount()-lampion_lu[playerid]<LAMPION_CZAS)
    return SendClientMessage(playerid,-1,"Lampiony mozesz puszczac co 10 sekund!");
  else
    lampion_lu[playerid]=GetTickCount();

  new Float:P[4];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  GetPlayerFacingAngle(playerid,P[3]);

  new obiekt=CreateDynamicObject(LAMPION_OBIEKT, P[0],P[1]+1.2,P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);


  P[0]+=(LAMPION_ODLEGLOSC_X*floatsin(-P[3],degrees));
  P[1]+=(LAMPION_ODLEGLOSC_Y*floatcos(-P[3],degrees));
  P[2]+=LAMPION_ODLEGLOSC_Z;
  //P[2]=P[2]-2.8;

  MoveDynamicObject(obiekt, P[0],P[1],P[2], 3,0,0,0);

  Streamer_Update(playerid);

  SetTimerEx("lampion_usun", LAMPION_CZAS2, false, "i", obiekt);

  return 1;

}

public lampion_usun(obiekt){
  DestroyDynamicObject(obiekt);
}
// EOF
