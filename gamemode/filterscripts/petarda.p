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

#define KEY_1 (KEY_FIRE)
#define KEY_2 (KEY_CROUCH)
#define SCMA SendClientMessageToAll
#define PETARDA_ODLEGLOSC 6
#define PETARDA_OBIEKT  18718
#define PETARDA_CZAS 3000

#undef MAX_PLAYERS
#define MAX_PLAYERS 314

new petarda_lu[MAX_PLAYERS];

forward petarda_wybuch(Float:X,Float:Y,Float:Z, obiekt);

public OnPlayerKeyStateChange(playerid,newkeys,oldkeys){
  if ((newkeys & (KEY_1 | KEY_2)) == (KEY_1 | KEY_2) && (oldkeys & (KEY_1 | KEY_2)) != (KEY_1 | KEY_2) && GetPlayerVirtualWorld(playerid)==0 ) {
    new weapon=GetPlayerWeapon(playerid);
    if (weapon==0 || weapon==1)   // piesci lub kastet
      petarda_rzuc(playerid);
  }
}

petarda_rzuc(playerid){
  if (GetTickCount()-petarda_lu[playerid]<PETARDA_CZAS)
    return;
  else
    petarda_lu[playerid]=GetTickCount();

  new Float:P[4];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  GetPlayerFacingAngle(playerid,P[3]);

  new obiekt=CreateDynamicObject(PETARDA_OBIEKT, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);


  P[0]+=(PETARDA_ODLEGLOSC*floatsin(-P[3],degrees));
  P[1]+=(PETARDA_ODLEGLOSC*floatcos(-P[3],degrees));
  P[2]=P[2]-2.8;

  MoveDynamicObject(obiekt, P[0],P[1],P[2], 10,0,0,0);

  SetTimerEx("petarda_wybuch", PETARDA_CZAS, false, "fffi", P[0],P[1],P[2],obiekt);
}

public petarda_wybuch(Float:X,Float:Y,Float:Z, obiekt){
  DestroyDynamicObject(obiekt);
  CreateExplosion(X,Y,Z, 10,5);
}
// EOF
