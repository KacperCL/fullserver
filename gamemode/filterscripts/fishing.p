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
#include <3rdparty/progress>
#include <3rdparty/streamer>

#define VW  0
#define MAX_FISHING 16 // ilosc CP
#define MAX_FISHING_NAMES 13 // ilosc nazw rybek
#define MAX_FISHING_NAME_LEN 16 // max. dlugosc nazwy rybki

#define KEY_PRESSED(%0) \
  (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#define KEY_RELEASED(%0) \
  (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

static Rybki[MAX_FISHING_NAMES][MAX_FISHING_NAME_LEN]={
  "uklejê", "karasia", "jazgarza", "p³otkê", "lina", "makrelê", "okonia", "œledzia", "karpia", "szczupaka", "³ososia",
  "sandacza", "wêgorza"};

new mg_fishing[MAX_PLAYERS];  // id checkpointu w ktorym lowi gracz

enum e_FishingCP {
  Float:ef_pos[4],  // X,Y,Z,A
  ef_mnoznik, // skala: 1-10
  ef_lastplayer,
  ef_reallastplayer,
  Bar:ef_pbar,
  ef_progress,
  ef_CP,
  Text3D:ef_3dLabel
}

new FishingCP[MAX_FISHING][e_FishingCP]= {
  {{-752.54,-2046.95,9.0,27.67},10},
  {{1930.37,-1200.19,19.85,268.7},5},
  {{1995.73,1521.45,17.06,96.0},1}, //statek
  {{2287.84,536.67,1.79,90.60},4},    // molo lv
  {{403.82,-2088.78,7.83,179.47},4},    // molo ls
  {{2942.19,-2051.58,3.54,270.45},5},
  {{-658.06,2151.11,60.37,57.20},4},    //tama
  {{-411.95,1160.85,1.11,112.98},5},  // fort carson
  {{-1500.02,-2337.20,3.34,35.17},4}, //shady creek
  {{-1376.94,2112.92,42.19,314.03},5},  // tierra robada
  {{-940.38,2641.96,42.35,134.82},4},   // tierra robada, kolo bylej bazy ps
  {{-324.81,-468.41,1.99,228.04},4},    // red county
  {{-2462.78,-269.11,39.3,207.0},4},    // pole golfowe kolo /kosciol
  {{1227.55,-2351.25,10.14,10.14},3},
  {{2584.64,2825.37,19.99,91.67},9},  // minigun
  {{75.20,2022.38,27.44,270.47},9}  // wojsko
};

public OnFilterScriptInit()
{
  for (new i=0; i < sizeof FishingCP; i++)
  {
    FishingCP[i][ef_CP] = CreateDynamicCP(FishingCP[i][ef_pos][0], FishingCP[i][ef_pos][1], FishingCP[i][ef_pos][2], 1.2, VW, 0, -1, 100);
    FishingCP[i][ef_3dLabel] = CreateDynamic3DTextLabel("Wedkowanie", -1, FishingCP[i][ef_pos][0], FishingCP[i][ef_pos][1],  FishingCP[i][ef_pos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, VW, 0, -1);
    FishingCP[i][ef_lastplayer]=INVALID_PLAYER_ID;
    FishingCP[i][ef_reallastplayer]=INVALID_PLAYER_ID;
    FishingCP[i][ef_pbar]=CreateProgressBar(19.00, 327.00, 101.50, 5.19, 869072810, 100.0);
    FishingCP[i][ef_progress]=0;
  }
  for (new i=0;i<MAX_PLAYERS;i++)
    mg_fishing[i]=-1;
  return 1;
}

public OnFilterScriptExit(){
  for (new i=0; i < sizeof FishingCP; i++){
    DestroyProgressBar(FishingCP[i][ef_pbar]);
  }
  return 1;
}

public OnPlayerConnect(playerid)
{
  mg_fishing[playerid]=-1;
  return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
  mg_fishing[playerid]=-1;
  return 1;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
  for (new i=0; i < sizeof FishingCP; i++)
    if (checkpointid == FishingCP[i][ef_CP])
      if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
          return 1;
      else {
        mg_fishing_Start(playerid,i);
        return 1;
      }
  return 0;
}

public OnPlayerLeaveDynamicCP(playerid, checkpointid)
{
  for (new i=0; i < sizeof FishingCP; i++)
    if (checkpointid == FishingCP[i][ef_CP]) {
      if (mg_fishing[playerid]==i) {
        mg_fishing_Stop(playerid,i);
        GameTextForPlayer(playerid,"~r~Rezygnujesz",500,1);
        SendClientMessage(playerid,-1,"[WEDKOWANIE] Zrezygnowa³eœ z po³owu i wystraszyles ryby, musisz isc poszukac innego miejsca");
      }
      return 1;
    }
  return 0;

}

mg_fishing_Start(playerid,i){
  if (FishingCP[i][ef_lastplayer]!=INVALID_PLAYER_ID)   // cp jest zajety, sprawdzmy czy na pewno
    if (!IsPlayerConnected(FishingCP[i][ef_lastplayer]) ||
      mg_fishing[FishingCP[i][ef_lastplayer]]==-1 ||
      !IsPlayerInDynamicCP(FishingCP[i][ef_lastplayer], FishingCP[mg_fishing[FishingCP[i][ef_lastplayer]]][ef_CP])) {
      // jednak jest wolny!
      mg_fishing_Stop(FishingCP[i][ef_lastplayer],mg_fishing[FishingCP[i][ef_lastplayer]]);
    }
  if (FishingCP[i][ef_lastplayer]!=INVALID_PLAYER_ID) {
    // dorobic sprawdzanie czy gracz nadal lowi
    GameTextForPlayer(playerid,"~r~Miejsce zajete",500,1);
    SendClientMessage(playerid,-1,"[WÊDKOWANIE] Ktoœ ju¿ tutaj ³owi. Lepiej poszukaj innego miejsca albo poczekaj a¿ skonczy");
    return;
  }
  if (FishingCP[i][ef_reallastplayer]==playerid) {
    SendClientMessage(playerid,-1,"[WÊDKOWANIE] Coœ ryby tu nie bior¹, mo¿e trzeba poszukaæ innego miejsca?");
    GameTextForPlayer(playerid,"~y~Nie ma brania!",500,1);
    return;
  }
  FishingCP[i][ef_reallastplayer]=playerid;
  FishingCP[i][ef_lastplayer]=playerid;
  mg_fishing[playerid]=i;
  GameTextForPlayer(playerid,"~w~Zarzucasz wedke",500,1);
  SendClientMessage(playerid,-1,"[WÊDKOWANIE] Aby wy³owiæ jak¹œ rybkê, naciskaj {ffff00}spacje/sprint{ffffff} a¿ do zape³nienia paska");
  SetPlayerPos(playerid,FishingCP[i][ef_pos][0],FishingCP[i][ef_pos][1],FishingCP[i][ef_pos][2]);
  SetPlayerFacingAngle(playerid,FishingCP[i][ef_pos][3]);
  SetPlayerAttachedObject(playerid, 4, 18632, 5, 0.079999, 0.009999, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000 ); // FishingRod - wedka2
  SetPlayerArmedWeapon(playerid,0);
  TogglePlayerControllable(playerid, 0);
  if (random(2) == 1)
    ApplyAnimation(playerid, "SWORD", "sword_IDLE", 4.1, 0, 1, 1, 1, 1, 1);
  else
    ApplyAnimation(playerid, "SWORD", "sword_block", 4.1, 0, 1, 1, 1, 1, 1);

  FishingCP[i][ef_progress]=0;
  SetProgressBarValue(FishingCP[i][ef_pbar], FishingCP[i][ef_progress]);
  ShowProgressBarForPlayer(playerid, FishingCP[i][ef_pbar]);
  return;
}

mg_fishing_Stop(playerid,i){
    if (i>-1)
    FishingCP[i][ef_lastplayer]=INVALID_PLAYER_ID;
    if(playerid>=0 && playerid!=INVALID_PLAYER_ID) {
      TogglePlayerControllable(playerid,1);
      mg_fishing[playerid]=-1;
      RemovePlayerAttachedObject(playerid,4);
      ClearAnimations(playerid);
      CallRemoteFunction("_rsp","d",playerid);
      HideProgressBarForPlayer(playerid, FishingCP[i][ef_pbar]);
    }
}

public OnPlayerKeyStateChange(playerid,newkeys,oldkeys){
  if (mg_fishing[playerid]==-1)
    return 1;
  new i=mg_fishing[playerid];
  if(KEY_PRESSED(KEY_SPRINT) || KEY_PRESSED(KEY_JUMP)) {
    if (random(2) == 1)
      ApplyAnimation(playerid, "SWORD", "sword_IDLE", 4.1, 0, 1, 1, 1, 1, 1);
    else
      ApplyAnimation(playerid, "SWORD", "sword_block", 4.1, 0, 1, 1, 1, 1, 1);
    if (FishingCP[i][ef_progress]>100) {  // koniec wedkowania
      mg_fishing_Stop(playerid,i);
      if (random(FishingCP[i][ef_mnoznik])>4) {
        GameTextForPlayer(playerid,"~r~Urwales zylke!",500,1);
        SendClientMessage(playerid,-1,"[WEDKOWANIE] Niestety {ffff00}urwa³eœ{ffffff} ¿y³ke i nici z po³owu");
        return 1;
      }

      GameTextForPlayer(playerid,"~g~Zlowiles rybke!",500,1);
      new los=random(FishingCP[i][ef_mnoznik]);
      new gstr[127];

      format(gstr,sizeof gstr,"[WEDKOWANIE] Z³owileœ {ffff00}%s{ffffff} - otrzymujesz {ffff00}%d{ffffff} respektu", Rybki[los],los+1);
      SendClientMessage(playerid,-1,gstr);
      CallRemoteFunction("GivePlayerScoreEx","dd",playerid,los+1);
      return 1;
    }
    new val=floatround(random(12-FishingCP[i][ef_mnoznik])/3)+1;
    FishingCP[i][ef_progress]+=val;
    SetProgressBarValue(FishingCP[i][ef_pbar], FishingCP[i][ef_progress]);
    UpdateProgressBar(FishingCP[i][ef_pbar], playerid);
  } else if(KEY_PRESSED(KEY_SECONDARY_ATTACK)) {  // rezygnacja
    mg_fishing_Stop(playerid,i);
    GameTextForPlayer(playerid,"~r~Rezygnujesz",500,1);
    SendClientMessage(playerid,-1,"[WEDKOWANIE] Zrezygnowa³eœ z po³owu i wystraszyles ryby, musisz isc poszukac innego miejsca");
  } else if (KEY_RELEASED(KEY_SPRINT)) {  // nic nie robimy
  } else {  // cos innego wciska? A moze juz w ogole gdzies indziej sie przeniosl?
    if (!IsPlayerInDynamicCP(playerid, FishingCP[i][ef_CP])) {
      mg_fishing_Stop(playerid,i);
      GameTextForPlayer(playerid,"~r~Rezygnujesz",500,1);
      SendClientMessage(playerid,-1,"[WEDKOWANIE] Zrezygnowa³eœ z po³owu i wystraszyles ryby, musisz isc poszukac innego miejsca");
    }
  }
  return 1;
}
// EOF
