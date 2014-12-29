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
// END OF LAMPION

new uPojazdy[5],
  pElekt[10];

public OnFilterScriptInit()
{
  uPojazdy[0]=AddStaticVehicle(582,206.7998000,-1777.3994100,3.8917800,90.0000000,111,103); //Newsvan
  uPojazdy[2]=AddStaticVehicle(416,156.7000000,-1758.0000000,5.0000000,0.0000000,1,3); //Ambulance
  uPojazdy[3]=AddStaticVehicle(596,156.8999900,-1766.3000500,4.4000000,0.0000000,0,1); //Police Car (LSPD)
  uPojazdy[4]=AddStaticVehicle(407,150.2000000,-1761.8000500,5.2000000,0.0000000,3,1); //Firetruck

  SetVehicleParamsEx(uPojazdy[0], 0, 0, 0, 1, 0, 0, 0);
  SetVehicleParamsEx(uPojazdy[1], 0, 0, 0, 1, 0, 0, 0);
  SetVehicleParamsEx(uPojazdy[2], 0, 0, 0, 1, 0, 0, 0);
  SetVehicleParamsEx(uPojazdy[3], 0, 0, 0, 1, 0, 0, 0);
  SetVehicleParamsEx(uPojazdy[4], 0, 0, 0, 1, 0, 0, 0);

  pElekt[0]=AddStaticVehicle(539,343.3994100,-1830.1992200,3.3000000,177.2590000,145,115); //Vortex
  pElekt[1]=AddStaticVehicle(539,338.6000100,-1830.3000500,3.3000000,179.7040000,145,115); //Vortex
  pElekt[2]=AddStaticVehicle(539,334.2999900,-1830.3000500,3.3000000,180.4820000,145,115); //Vortex
  pElekt[3]=AddStaticVehicle(539,330.0000000,-1830.3000500,3.3000000,179.7150000,145,115); //Vortex
  pElekt[4]=AddStaticVehicle(539,325.1000100,-1830.3000500,3.3000000,179.4780000,145,115); //Vortex
  pElekt[5]=AddStaticVehicle(539,341.5000000,-1866.9000200,2.6000000,357.9780000,145,115); //Vortex
  pElekt[6]=AddStaticVehicle(539,337.6000100,-1866.8000500,2.6000000,358.4480000,145,115); //Vortex
  pElekt[7]=AddStaticVehicle(539,333.2999900,-1866.6999500,2.3000000,357.9730000,145,115); //Vortex
  pElekt[8]=AddStaticVehicle(539,329.2000100,-1866.6999500,2.4000000,357.9730000,145,115); //Vortex
  pElekt[9]=AddStaticVehicle(539,325.1000100,-1866.5000000,2.4000000,357.9730000,145,115); //Vortex

  SetVehicleHealth(pElekt[0],999999);
  SetVehicleHealth(pElekt[1],999999);
  SetVehicleHealth(pElekt[2],999999);
  SetVehicleHealth(pElekt[3],999999);
  SetVehicleHealth(pElekt[4],999999);
  SetVehicleHealth(pElekt[5],999999);
  SetVehicleHealth(pElekt[6],999999);
  SetVehicleHealth(pElekt[7],999999);
  SetVehicleHealth(pElekt[8],999999);
  SetVehicleHealth(pElekt[9],999999);
  return 1;
}

public OnFilterScriptExit()
{
  DestroyVehicle(uPojazdy[0]);
  DestroyVehicle(uPojazdy[1]);
  DestroyVehicle(uPojazdy[2]);
  DestroyVehicle(uPojazdy[3]);
  DestroyVehicle(uPojazdy[4]);

  DestroyVehicle(pElekt[0]);
  DestroyVehicle(pElekt[1]);
  DestroyVehicle(pElekt[2]);
  DestroyVehicle(pElekt[3]);
  DestroyVehicle(pElekt[4]);
  DestroyVehicle(pElekt[5]);
  DestroyVehicle(pElekt[6]);
  DestroyVehicle(pElekt[7]);
  DestroyVehicle(pElekt[8]);
  DestroyVehicle(pElekt[9]);
  return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
  if(vehicleid==uPojazdy[0] || vehicleid==uPojazdy[1] || vehicleid==uPojazdy[2] || vehicleid==uPojazdy[3] || vehicleid==uPojazdy[4]) SetVehicleParamsForPlayer(vehicleid, forplayerid, 0, 1);
  return 1;
}
// END OF VEHICLES

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
// END OF RAKIETA
