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

forward rakieta_strzal(Float:X, Float:Y, Float:Z, r1, r2);
forward rakieta_wybuch(Float:X, Float:Y, Float:Z, r1, r2);

#undef MAX_PLAYERS
#define MAX_PLAYERS 314

new petarda_lu[MAX_PLAYERS];

CMD:rakieta(playerid){
  if (GetPlayerVirtualWorld(playerid)!=0 || GetPlayerInterior(playerid)!=0)
    return SendClientMessage(playerid,-1,"Nie mozesz tu odpalic rakiety");

  if (GetTickCount()-petarda_lu[playerid]<60000)
    return SendClientMessage(playerid,-1,"Rakiete mozna odpalic tylko raz na minute");
  else
    petarda_lu[playerid]=GetTickCount();

  rakieta_odpal(playerid);
  return 1;
}

rakieta_odpal(playerid){
  new Float:P[4];
  new Float:C[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  GetPlayerFacingAngle(playerid,P[3]);

  P[0]+=(3*floatsin(-P[3],degrees));
  P[1]+=(3*floatcos(-P[3],degrees));
  P[2]=P[2]-2.8;

  new r1=CreateDynamicObject(18718, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);
  new r2=CreateDynamicObject(18728, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);

  C[2]=P[2]+45;
  C[0]=P[0]-6+random(12);
  C[1]=P[1]-6+random(12);
  SetTimerEx("rakieta_strzal", 2000, false, "fffdd", C[0], C[1], C[2], r1, r2);

  r1=CreateDynamicObject(18718, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);
  r2=CreateDynamicObject(18728, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);

  C[2]=P[2]+75;
  C[0]=P[0]-12+random(24);
  C[1]=P[1]-12+random(24);
  SetTimerEx("rakieta_strzal", 2500, false, "fffdd", C[0], C[1], C[2], r1, r2);

  r1=CreateDynamicObject(18718, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);
  r2=CreateDynamicObject(18728, P[0],P[1],P[2],0,0,0,0,GetPlayerInterior(playerid),-1,200);

  C[2]=P[2]+55;
  C[0]=P[0]-10+random(20);
  C[1]=P[1]-10+random(20);
  SetTimerEx("rakieta_strzal", 3000, false, "fffdd", C[0], C[1], C[2], r1, r2);
  Streamer_Update(playerid);

}

public rakieta_strzal(Float:X, Float:Y, Float:Z, r1, r2) {
  new czas=MoveDynamicObject(r1, X,Y,Z,20);
  MoveDynamicObject(r2, X,Y,Z,15);
  SetTimerEx("rakieta_wybuch", czas, false, "fffdd", X,Y,Z,r1,r2);
}

public rakieta_wybuch(Float:X, Float:Y, Float:Z, r1, r2) {
  DestroyDynamicObject(r1);
  DestroyDynamicObject(r2);
  CreateExplosion(X,Y,Z, 10,5);
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
  if(success)
  {
    CallRemoteFunction("PlayerCommandPerformedOutside", "isi", playerid, cmdtext, success);
  }
}
// EOF