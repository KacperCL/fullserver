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
#include <3rdparty/sscanf2>

new blowjobCP;
new blowjobCP2;
new spowiedzCP;

public OnFilterScriptInit(){
  blowjobCP=CreateDynamicCP(949.97,-48.24,1001.0,1,3,3,-1);
  blowjobCP2=CreateDynamicCP(943.52,-42.81,1001.11,1,3,3,-1);
  spowiedzCP=CreateDynamicCP(2280.54,-50.67,26.5,3,0,0,-1);
  CreateDynamic3DTextLabel("Ekspresowa spowiedz DB\n$5000+1 respektu",0xffffffff,2280.54,-50.67,27.5,25);
  return 1;
}

public OnPlayerEnterDynamicCP(playerid,checkpointid){
  if(checkpointid==blowjobCP) {
    if (GetPlayerState(playerid)!=PLAYER_STATE_ONFOOT) return 1;
    // szukamy czy w CP jest inny gracz tez
    for(new i=0; i<MAX_PLAYERS;i++)
      if (i!=playerid && GetPlayerState(i)==PLAYER_STATE_ONFOOT && IsPlayerInDynamicCP(i,blowjobCP)) {
        burdel_Blowjob(playerid,i,0);
        return 1;
      }
    SendClientMessage(playerid,-1,"[BURDELE] Musisz tu wejsc razem z druga osoba");
    return 1;
  }
  if(checkpointid==blowjobCP2) {
    if (GetPlayerState(playerid)!=PLAYER_STATE_ONFOOT) return 1;
    // szukamy czy w CP jest inny gracz tez
    for(new i=0; i<MAX_PLAYERS;i++)
      if (i!=playerid && GetPlayerState(i)==PLAYER_STATE_ONFOOT && IsPlayerInDynamicCP(i,blowjobCP2)) {
        burdel_Blowjob2(playerid,i,0);
        return 1;
      }
    SendClientMessage(playerid,-1,"[BURDELE] Musisz tu wejsc razem z druga osoba");
    return 1;
  }
  if (checkpointid==spowiedzCP) {
    if (GetPlayerState(playerid)==PLAYER_STATE_ONFOOT)
      return GameTextForPlayer(playerid,"~w~Ta spowiedz mozliwa jest tylko w pojezdzie.",2000,3);
    if(GetPlayerMoney(playerid)<5000 || CallRemoteFunction("GetPlayerScoreFS","d",playerid)<1)
      return GameTextForPlayer(playerid,"~w~Nie masz wystarczajaco gotowki/respektu",2000,3);
    SetVehicleVelocity(GetPlayerVehicleID(playerid),0.0,0.0,0.0);
    GameTextForPlayer(playerid,"~r~Twoje grzechy zostaly odpuszczone.~n~~w~Idz w pokoju", 4000, 3);
    CallRemoteFunction("GivePlayerScoreEx","dd",playerid,-1);
    CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,-5000);
    return 1;
  }
  return 0;

}

bool:SkinKobiecy(s){
    switch(s){
        case 9,10..13,31,38..41, 53..56, 63, 64, 69, 75..77, 85, 87, 88..93,129, 130, 131,138..141,145,148,150..152,157,169,172,178,190..199,201,205,207,211,214,215,
            216,218,219,224..226,231,232,233,237,238,243,244..246, 251,256,257,263,298:
            return true;
    }
    return false;
}

bool:Kobieta(playerid)
  return SkinKobiecy(GetPlayerSkin(playerid));

forward burdel_Blowjob(facet,baba,step);
public burdel_Blowjob(facet,baba,step){
 switch(step){
  case 0: {

    if (!Kobieta(baba) && Kobieta(facet)) {
      new tmp;
      tmp=baba; baba=facet; facet=tmp;
    }
    SetPlayerPos(facet, 952.21, -45.5,1001.12);
    SetPlayerPos(baba, 952.21,-46.05,1001.12);

    SetPlayerFacingAngle(facet,180);
    SetPlayerFacingAngle(baba,0);
    TogglePlayerControllable(facet,0);
    TogglePlayerControllable(baba,0);

    ApplyAnimation(facet, "BLOWJOBZ", "BJ_COUCH_START_P", 4.1, 0, 1, 1, 1, 1, 1);
    ApplyAnimation(baba, "BLOWJOBZ", "BJ_COUCH_START_W", 4.1, 0, 1, 1, 1, 1, 1);

    SetTimerEx("burdel_Blowjob",1000,false,"ddd",facet,baba,step+1);

    return;
  }
  case 1: {
    ApplyAnimation(facet, "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.1, 1, 1, 1, 1, 1, 1);
    ApplyAnimation(baba, "BLOWJOBZ", "BJ_COUCH_LOOP_W", 4.1, 1, 1, 1, 1, 1, 1);
    SetTimerEx("burdel_Blowjob",2500+random(3000),false,"ddd",facet,baba,step+1);
    return;
  }
  case 2: {
    ApplyAnimation(facet, "BLOWJOBZ", "BJ_COUCH_END_P", 4.1, 0, 1, 1, 1, 1, 1);
    ApplyAnimation(baba, "BLOWJOBZ", "BJ_COUCH_END_W", 4.1, 0, 1, 1, 1, 1, 1);
    SetTimerEx("burdel_Blowjob",1000,false,"ddd",facet,baba,step+1);
    return;
  }
  case 3: {
    ApplyAnimation(facet, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
    ApplyAnimation(baba, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
    ClearAnimations(facet);
    ClearAnimations(baba);
    TogglePlayerControllable(facet,1);
    TogglePlayerControllable(baba,1);
    return;
  }
 }
 return;
}

forward burdel_Blowjob2(facet,baba,step);
public burdel_Blowjob2(facet,baba,step){
 switch(step){
  case 0: {

    if (!Kobieta(baba) && Kobieta(facet)) {
      new tmp;
      tmp=baba; baba=facet; facet=tmp;
    }
    SetPlayerPos(facet, 945.17, -44.0,1001.12);
    SetPlayerPos(baba, 945.17,-45.0,1001.12);

    SetPlayerFacingAngle(facet,180);
    SetPlayerFacingAngle(baba,0);
    TogglePlayerControllable(facet,0);
    TogglePlayerControllable(baba,0);

    ApplyAnimation(facet, "BLOWJOBZ", "BJ_STAND_START_P", 4.1, 0, 1, 1, 1, 1, 1);
    ApplyAnimation(baba, "BLOWJOBZ", "BJ_STAND_START_W", 4.1, 0, 1, 1, 1, 1, 1);

    SetTimerEx("burdel_Blowjob2",1000,false,"ddd",facet,baba,step+1);

    return;
  }
  case 1: {
    SetPlayerPos(facet, 945.17, -44.0,1001.12);
    SetPlayerPos(baba, 945.17,-45.0,1001.12);

    ApplyAnimation(facet, "BLOWJOBZ", "BJ_STAND_LOOP_P", 4.1, 1, 1, 1, 1, 1, 1);
    ApplyAnimation(baba, "BLOWJOBZ", "BJ_STAND_LOOP_W", 4.1, 1, 1, 1, 1, 1, 1);
    SetTimerEx("burdel_Blowjob2",2500+random(3000),false,"ddd",facet,baba,step+1);
    return;
  }
  case 2: {
    ApplyAnimation(facet, "BLOWJOBZ", "BJ_STAND_END_P", 4.1, 0, 1, 1, 1, 1, 1);
    ApplyAnimation(baba, "BLOWJOBZ", "BJ_STAND_END_W", 4.1, 0, 1, 1, 1, 1, 1);
    SetTimerEx("burdel_Blowjob2",1000,false,"ddd",facet,baba,step+1);
    return;
  }
  case 3: {
    ApplyAnimation(facet, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
    ApplyAnimation(baba, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
    ClearAnimations(facet);
    ClearAnimations(baba);
    TogglePlayerControllable(facet,1);
    TogglePlayerControllable(baba,1);
    return;
  }
 }
 return;
}
// EOF
