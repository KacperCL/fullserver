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

#define MYJNIA_KOSZT  4000
#define DRIFTUPGRADE_KOSZT  10000

forward warsztat_streambackvehicle(vid, Float:vP0, Float:vP1, Float: vP2, Float: vP3);
forward warsztat_respray(playerid,vehicleid);

warsztat_zmianaTablicy(playerid, tablica[]=""){

  new vid=GetNearestVehicleFromXYZ(1042.68,1782.55,11.52,5,true);
  if (vid==INVALID_VEHICLE_ID)
    return Msg(playerid,COLOR_ERROR,__("Najpierw zaparkuj samochod w garazu."));
  if (strlen(tablica)==0)
    return ShowPlayerDialog(playerid,DIALOG_WARSZTAT_TABLICE, DIALOG_STYLE_INPUT, __("Zmiana tablicy"), __("Wprowadz tresc nowe tablicy rejestracyjnej"), __("OK"), __("Anuluj"));
  else if (!IsNickCorrect(tablica))
    return ShowPlayerDialog(playerid,DIALOG_WARSZTAT_TABLICE, DIALOG_STYLE_INPUT, __("Zmiana tablicy"), __("Wprowadz tresc nowe tablicy rejestracyjnej.\n{ff3030}Uzyles niedozwolonych znakow. Sprobuj ponownie."), __("OK"), __("Anuluj"));

  SetVehicleNumberPlate(vid,tablica);
  tVehicles[vid][vo_licensePlateSet]=true;

  Msg(playerid,COLOR_INFO,__("Tablica ustawiona"));

  new Float:vP[4];
  GetVehiclePos(vid,vP[0],vP[1],vP[2]);
  LinkVehicleToInterior(vid,17);
  SetVehicleVirtualWorld(vid,17);

  SetTimerEx("warsztat_streambackvehicle",  1000, false, "dffff", vid, vP[0],vP[1],vP[2], vP[3]);

  return 1;
}

stock warsztat_holowanieMG_start(playerid){
  skupypojazdowCP[playerid]=CreateDynamicCP(skupypojazdow[0][0], skupypojazdow[0][1], skupypojazdow[0][2], skupypojazdow[0][3],0,0,playerid,2000);
  Streamer_UpdateEx(playerid, skupypojazdow[0][0], skupypojazdow[0][1], skupypojazdow[0][2]);
}

stock warsztat_holowanieMG_stop(playerid){
  DestroyDynamicCP(skupypojazdowCP[playerid]);
}

public warsztat_streambackvehicle(vid,Float:vP0, Float:vP1, Float: vP2, Float: vP3){
  SetVehiclePos(vid,vP0,vP1,vP2);
  LinkVehicleToInterior(vid,0);
  SetVehicleVirtualWorld(vid,0);
}

warsztat_DialogResponse(playerid, dialogid, response, listitem, inputtext[]){
#pragma unused listitem
  if (dialogid==DIALOG_WARSZTAT_TABLICE) {
    if (!response) return 1;
    warsztat_zmianaTablicy(playerid, inputtext);
    return 1;
  } else if (dialogid==DIALOG_WARSZTAT_LIFT) {
    if (!response) return Msg(playerid,COLOR_ERROR,Kobieta(playerid)?(__("Zrezygnowalas")):(__("Zrezygnowales")));
    warsztat_moveLift(listitem);
  }
  return 0;
}

warsztat_CPmyjnia(playerid){
  GameTextForPlayer(playerid,__("~y~Myjnia"),500,0);
  if (!IsPlayerInAnyVehicle(playerid))
    return Msg(playerid,COLOR_ERROR,__("Musisz tu przyjechac pojazdem :-)"));
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER)
    return 1;

  new vid,Float:VZ;
  vid=GetPlayerVehicleID(playerid);
  GetVehicleZAngle(vid,VZ);
  if (VZ<269 && VZ>90)
    return Msg(playerid,COLOR_ERROR,"Wjechales ze zlej strony!");

  if (GetPlayerMoney(playerid)<MYJNIA_KOSZT)
    return Msg(playerid,COLOR_ERROR,"Nie stac Cie na myjnie!");

  _lock(playerid);
  GivePlayerMoney(playerid,-MYJNIA_KOSZT);

  RepairVehicle(vid);
  SetVehicleHealth(vid,VEHICLE_DEFAULT_HP);

  return 1;
}

warsztat_CPskup(playerid){
  GameTextForPlayer(playerid,"~y~Skup pojazdow",2000,3);
  if (!IsPlayerInAnyVehicle(playerid)){
    GameTextForPlayer(playerid,"Mozesz tu przyholowywac pojazdy do skupu/utylizacji", 2000,5);
    return 1;
  }
  new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)!=525)
    return GameTextForPlayer(playerid,"Pojazdy musza byc przyholowane!", 2000,5);

  if (!IsTrailerAttachedToVehicle(vid))
    return GameTextForPlayer(playerid,"Przyholuj pojazd do skupu", 2000,5);
  new tvid=GetVehicleTrailer(vid);
  DetachTrailerFromVehicle(vid);
  new buf[128],cash;
  RespawnVehicle(tvid);
  cash=500+random(500);

  format(buf,sizeof buf,"Pojazd przyjety do skupu~n~~g~~h~+~g~%d~h~$", cash);
  GameTextForPlayer(playerid,buf,2000,5);
  GivePlayerMoney(playerid,cash);
  PlayRandomMusic(playerid,6000);

  return 1;
}

warsztat_CPpodnosnik(playerid){
  if (pTemp[playerid][faction]!=FACTION_MECHANICY) {
    Msg(playerid,COLOR_ERROR,"Podnosnik moga obslugiwac tylko {b}mechanicy{/b}.");
    return GameTextForPlayer(playerid,"~r~Tylko dla wykwalifikowanego personelu!", 500,0);
  }
  format(gstr,sizeof gstr,"{%06x}Podnosnik I\n{%06x}Podnosnik II\n{%06x}Podnosnik III",
    podnosniki_stan[0]?0xaa1010:0x30ff30,
    podnosniki_stan[1]?0xaa1010:0x30ff30,
    podnosniki_stan[2]?0xaa1010:0x30ff30);
  ShowPlayerDialog(playerid,DIALOG_WARSZTAT_LIFT, DIALOG_STYLE_LIST, "Sterowanie podnosnikami", gstr, "OK", "Anuluj");
  return 1;
}
#define WPMS (0.5)
warsztat_moveLift(podnosnik){
  if (podnosnik>sizeof podnosniki_stan) return 0;
  switch(podnosnik){
    case 2: { // III
      if (podnosniki_stan[podnosnik]) { // opuszczamy
        MoveDynamicObject(podnosniki[2][0],1070.65234375,1735.26416016,12.0,WPMS);
        MoveDynamicObject(podnosniki[2][1],1070.65234375,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[2][2],1070.68310547,1735.17016602,11.20584679,WPMS);
        MoveDynamicObject(podnosniki[2][3],1074.68457031,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[2][4],1074.68457031,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[2][5],1074.71484375,1735.17016602,11.20584679,WPMS);
      } else { // podnosimy
        MoveDynamicObject(podnosniki[2][0],1070.65234375,1735.26416016,14.3,WPMS);
        MoveDynamicObject(podnosniki[2][1],1070.65234375,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[2][2],1070.68310547,1735.17016602,13.5,WPMS);
        MoveDynamicObject(podnosniki[2][3],1074.68457031,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[2][4],1074.68457031,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[2][5],1074.71484375,1735.17016602,13.5,WPMS);
      }
    }
    case 1: { // II
      if (podnosniki_stan[podnosnik]) { // opuszczamy
        MoveDynamicObject(podnosniki[1][0],1077.13085938,1735.26416016,12.0,WPMS);
        MoveDynamicObject(podnosniki[1][1],1077.13085938,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[1][2],1077.17089844,1735.17016602,11.20584679,WPMS);
        MoveDynamicObject(podnosniki[1][3],1079.63183594,1735.26416016,12.0,WPMS);
        MoveDynamicObject(podnosniki[1][4],1079.63183594,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[1][5],1079.67309570,1735.17016602,11.20584679,WPMS);
      } else {
        MoveDynamicObject(podnosniki[1][0],1077.13085938,1735.26416016,14.3,WPMS);
        MoveDynamicObject(podnosniki[1][1],1077.13085938,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[1][2],1077.17089844,1735.17016602,13.5,WPMS);
        MoveDynamicObject(podnosniki[1][3],1079.63183594,1735.26416016,14.3,WPMS);
        MoveDynamicObject(podnosniki[1][4],1079.63183594,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[1][5],1079.67309570,1735.17016602,13.5,WPMS);
      }
    }
    case 0: {
      if (podnosniki_stan[podnosnik]) { // opuszczamy
        MoveDynamicObject(podnosniki[0][0],1083.61914062,1735.26416016,12.0,WPMS);
        MoveDynamicObject(podnosniki[0][1],1083.61914062,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[0][2],1083.65625000,1735.17016602,11.20584679,WPMS);
        MoveDynamicObject(podnosniki[0][3],1086.31323242,1735.26416016,12.0,WPMS);
        MoveDynamicObject(podnosniki[0][4],1086.31250000,1735.26367188,12.0,WPMS);
        MoveDynamicObject(podnosniki[0][5],1086.34497070,1735.17016602,11.20584679,WPMS);
      } else {
        MoveDynamicObject(podnosniki[0][0],1083.61914062,1735.26416016,14.3,WPMS);
        MoveDynamicObject(podnosniki[0][1],1083.61914062,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[0][2],1083.65625000,1735.17016602,13.5,WPMS);
        MoveDynamicObject(podnosniki[0][3],1086.31323242,1735.26416016,14.3,WPMS);
        MoveDynamicObject(podnosniki[0][4],1086.31250000,1735.26367188,14.3,WPMS);
        MoveDynamicObject(podnosniki[0][5],1086.34497070,1735.17016602,13.5,WPMS);
      }
    }
  }
  podnosniki_stan[podnosnik]=!podnosniki_stan[podnosnik];
  return 1;
}
#undef WPMS
// EOF
