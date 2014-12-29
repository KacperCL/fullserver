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
#include <3rdparty/mapandreas>

#define CASH 1000

new gstr[128];

enum e_CelLotniczy {
  Float:ecl_loc[3],
  ecl_radius,
  ecl_nazwa[32],
  ecl_cp,
  Text3D:ecl_3dlabel,
  ecl_lastplayerid,
  bool:ecl_paraDropActive,
  ecl_paraObject,
  ecl_mapic
}

enum e_BazaSpedycyjna {
  Float:ebs_loc[4],
  ebs_nazwa[32],
  ebs_cp,
  Text3D:ebs_3dlabel,
  ebs_lastplayerid,
  ebs_mapic
}

new BazySpedycyjne[3][e_BazaSpedycyjna]={
  {{1570.22,1394.46,12.22},"/LVLOT"},
  {{-1467.3226,-180.9501,14.1484},"/SFLOT"},
  {{1796.9084,-2444.0566,13.5547},"/LSLOT"}
};

new CeleLotnicze[6][e_CelLotniczy]={
  {{2123.0,1044.0,96.0}, 10, "/LV"},
  {{2601.3,2760.3,91.0}, 100, "/Minigun"},
  {{2507.0,-1666.3,50.77}, 50,  "Groove Street"},
  {{110.2,1779.3,95.55}, 50,  "/Wojsko"},
  {{-744.33,-1961.57,190.37}, 25, "/Wietnam"},
  {{-1949.29,894.23,130.61}, 100, "/SF"}
};

new mg_spedycja_model[MAX_PLAYERS];


public OnPlayerConnect(playerid){
  SetPVarInt(playerid,"Zaladowano",0);
  return 1;
}

public OnFilterScriptInit(){
  for (new i=0;i<sizeof CeleLotnicze;i++) {
    CeleLotnicze[i][ecl_cp]=CreateDynamicRaceCP(3, CeleLotnicze[i][ecl_loc][0], CeleLotnicze[i][ecl_loc][1], CeleLotnicze[i][ecl_loc][2], 0, 0, 0, 14, 0,0,-1,1200);
        CeleLotnicze[i][ecl_lastplayerid]=-1;
    CeleLotnicze[i][ecl_3dlabel]=CreateDynamic3DTextLabel("Wlec tu aby\ndokonac zrzutu", -1, CeleLotnicze[i][ecl_loc][0], CeleLotnicze[i][ecl_loc][1], CeleLotnicze[i][ecl_loc][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1);
        CeleLotnicze[i][ecl_mapic]=CreateDynamicMapIcon(CeleLotnicze[i][ecl_loc][0], CeleLotnicze[i][ecl_loc][1], CeleLotnicze[i][ecl_loc][2], 57, 1, 0,0,-1,100);
  }
  for (new i=0;i<sizeof BazySpedycyjne;i++) {
    BazySpedycyjne[i][ebs_cp]=CreateDynamicCP(BazySpedycyjne[i][ebs_loc][0], BazySpedycyjne[i][ebs_loc][1],  BazySpedycyjne[i][ebs_loc][2],10, 0,0,-1,100);
        BazySpedycyjne[i][ebs_lastplayerid]=-1;
    BazySpedycyjne[i][ebs_3dlabel]=CreateDynamic3DTextLabel("Zaladunek\nlotniczy", -1, BazySpedycyjne[i][ebs_loc][0], BazySpedycyjne[i][ebs_loc][1],  BazySpedycyjne[i][ebs_loc][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1);
        BazySpedycyjne[i][ebs_mapic]=CreateDynamicMapIcon(BazySpedycyjne[i][ebs_loc][0], BazySpedycyjne[i][ebs_loc][1],  BazySpedycyjne[i][ebs_loc][2], 5, 1, 0,0,-1,100);
  }
  for (new i=0;i<MAX_PLAYERS;i++) SetPVarInt(i,"Zaladowano",0);
  SendClientMessageToAll(-1,"[SPEDYCJA] Misje spedycyjne (latanie) zostaly przeladowane");
  SetTimer("restartCeleLotniczePid", 360000, true);
}

public OnPlayerUpdate(playerid){
    if (GetTickCount()%2==0) return 1;    // raz na 2 razy kontynuujemy

  if (GetPVarInt(playerid,"Zaladowano")>0)
  {
    if(GetVehicleModel(GetPlayerVehicleID(playerid)) != mg_spedycja_model[playerid]){
        SetPVarInt(playerid,"Zaladowano",0);
      CallRemoteFunction("MsgExport","iis",playerid,1,"Zostales okradziony i straciles paczki z samolotu!");
    }
  }
  return 1;
}

public OnPlayerEnterDynamicCP(playerid,checkpointid){
    new model = GetVehicleModel(GetPlayerVehicleID(playerid));
  for (new i=0;i<sizeof BazySpedycyjne;i++){
    if (checkpointid==BazySpedycyjne[i][ebs_cp]){
      if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) {
        return 1;
      } else if (model !=553 && model !=593 && model !=577 && model !=476){
        GameTextForPlayer(playerid,"~r~||~w~Wymagany samolot Nevada, Dodo, Rustler lub AT-400~r~||",1000,1);
      } else if (BazySpedycyjne[i][ebs_lastplayerid] == playerid) {
          if (GetPVarInt(playerid,"Zaladowano")>0) SendClientMessage(playerid,-1,"[SPEDYCJA] Masz juz zaladowane paczki. Zrzuc je najpierw a potem wroc po nastepne");
          else{
          SendClientMessage(playerid,-1,"[SPEDYCJA] Dokona³eœ za³adunku w tym miejscu ostatnim razem. Udaj siê do innego punktu.");
          SendClientMessage(playerid,-1,"[SPEDYCJA] Dostepne punkty zaladunku: /LVLOT /SFLOT /LSLOT");
        }
      } else {
          new paczki;
          if(model == 593 || model==476){
          paczki=random(2)+3;
        } else if (model == 553){
          paczki=random(4)+6;
        } else if (model == 577){
          paczki=random(5)+10;
        }
          format(gstr,sizeof gstr,"~g~||~w~Zaladowano %d paczki~g~||",paczki);
        GameTextForPlayer(playerid,gstr,1000,1);
        format(gstr,sizeof gstr,"[SPEDYCJA] Zaladowano %d paczki, mozesz leciec dokonac zrzutu",paczki);
        SendClientMessage(playerid,-1,gstr);
        SendClientMessage(playerid,-1,"[SPEDYCJA] Dostepne punkty zrzutu nad: /LV /SF /WOJSKO /MINIGUN /WIETNAM /GS");
        SetPVarInt(playerid,"Zaladowano",paczki);
        mg_spedycja_model[playerid]=model;
        for (new cele=0;cele<sizeof CeleLotnicze;cele++)
          if(CeleLotnicze[cele][ecl_lastplayerid]==playerid) CeleLotnicze[cele][ecl_lastplayerid]=-1;
                for (new bazy=0;bazy<sizeof BazySpedycyjne;bazy++)
          if(BazySpedycyjne[bazy][ebs_lastplayerid]==playerid) BazySpedycyjne[bazy][ebs_lastplayerid]=-1;
        BazySpedycyjne[i][ebs_lastplayerid] = playerid;
      }
      return 1;
    }
  }
  return 0;
}

public OnPlayerEnterDynamicRaceCP(playerid,checkpointid){
  new model = GetVehicleModel(GetPlayerVehicleID(playerid));
  for (new i=0;i<sizeof CeleLotnicze;i++)
    if (CeleLotnicze[i][ecl_cp]==checkpointid) {
      if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) {
        return 1;
      } else if (model !=553 && model !=593 && model !=577 && model !=476){
        GameTextForPlayer(playerid,"~r~||~w~Wymagany samolot Nevada, Dodo, Rustler lub AT-400~r~||",1000,1);
      } else if (GetPVarInt(playerid,"Zaladowano")==0) {
        GameTextForPlayer(playerid,"~r~||~w~Brak paczek!~r~||",1000,1);
        SendClientMessage(playerid,-1,"[SPEDYCJA] Niestety, nie masz paczek do zrzucenia");
        SendClientMessage(playerid,-1,"[SPEDYCJA] Wracaj na lotnisko /LVLOT /SFLOT /LSLOT do punktu zaladunku.");
      } else if (CeleLotnicze[i][ecl_lastplayerid]==playerid) {
        GameTextForPlayer(playerid,"~r~||~w~Zrzut niemozliwy~r~||",1000,1);
        SendClientMessage(playerid,-1,"[SPEDYCJA] Juz dokonales zrzutu w tym punkcie. Lec do nastepnego");
        SendClientMessage(playerid,-1,"[SPEDYCJA] Miejsca zrzutu nad teleportami: /LV /SF /GS /MINIGUN /WOJSKO /WIETNAM");
      } else if (CeleLotnicze[i][ecl_paraDropActive]) {
        GameTextForPlayer(playerid,"~r~||~w~Zrzut niemozliwy~r~||",1000,1);
        SendClientMessage(playerid,-1,"[SPEDYCJA] Na tym terenie niedawno byla zrzucona paczka. Sprobuj za chwile lub lec do kolejnego punktu");
      } else {
        new Float:NP[3];
        GetPlayerPos(playerid, NP[0], NP[1],NP[2]);

          SetPVarInt(playerid,"Zaladowano",GetPVarInt(playerid,"Zaladowano")-1);
          if (GetPVarInt(playerid,"Zaladowano")==0)
          SendClientMessage(playerid,-1,"[SPEDYCJA] Zrzuciles ostatnia paczke. Udaj sie na lotnisko po nowe");

        GameTextForPlayer(playerid,"~b~||~w~Zrzut~b~||",1000,1);
        CeleLotnicze[i][ecl_paraDropActive]=true;
        CeleLotnicze[i][ecl_lastplayerid]=playerid;
        CeleLotnicze[i][ecl_paraObject]=CreateDynamicObject(18849,NP[0],NP[1],NP[2],  0,0,0,  0,0,-1,300);
        NP[0]=NP[0]+random(CeleLotnicze[i][ecl_radius]*2)-CeleLotnicze[i][ecl_radius];
        NP[1]=NP[1]+random(CeleLotnicze[i][ecl_radius]*2)-CeleLotnicze[i][ecl_radius];
        MapAndreas_FindZ_For2DCoord(NP[0],NP[1],NP[2]);
        // printf("%f %f %f",NP[0],NP[1],NP[2]);
        SetTimerEx("ParaLadowanie",
          MoveDynamicObject(CeleLotnicze[i][ecl_paraObject], NP[0], NP[1], NP[2]+7.5,3+random(2)),
          false,"dfff",i,NP[0],NP[1],NP[2]+1);

        if(model == 593 || model==476){
            if(CallRemoteFunction("GetPlayerFactionID","d",playerid)==5){
            CallRemoteFunction("GivePlayerScoreEx","dd",playerid,2+1);
            CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,CASH+800);
          }else{
              CallRemoteFunction("GivePlayerScoreEx","dd",playerid,2);
            CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,CASH+800);
          }
        } else if (model == 553){
          if(CallRemoteFunction("GetPlayerFactionID","d",playerid)==5){
            CallRemoteFunction("GivePlayerScoreEx","dd",playerid,2*3);
            CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,CASH*3);
          }else{
              CallRemoteFunction("GivePlayerScoreEx","dd",playerid,2*2);
            CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,CASH*2);
          }
        } else if (model == 577){
          if(CallRemoteFunction("GetPlayerFactionID","d",playerid)==5){
            CallRemoteFunction("GivePlayerScoreEx","dd",playerid,2*5);
            CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,CASH*5);
          }else{
              CallRemoteFunction("GivePlayerScoreEx","dd",playerid,2*4);
            CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,CASH*4);
          }
        }
      }
      return 1;
    }
  return 0;

}

forward ParaLadowanie(i,Float:X,Float:Y,Float:Z);
public ParaLadowanie(i,Float:X,Float:Y,Float:Z) {
  new pickupid=CreateDynamicPickup(1240+random(2),2,X,Y,Z,0,0,-1,300);
  DestroyDynamicObject(CeleLotnicze[i][ecl_paraObject]);
  CeleLotnicze[i][ecl_paraDropActive]=false;
  SetTimerEx("UsunPickup",10000,false,"d",pickupid);

  return 1;
}

forward UsunPickup(pickupid);
public UsunPickup(pickupid){
  DestroyDynamicPickup(pickupid);
}

forward restartCeleLotniczePid();
public restartCeleLotniczePid(){
  // remove it first
  for (new i=0;i<sizeof CeleLotnicze;i++) {
    DestroyDynamicRaceCP(CeleLotnicze[i][ecl_cp]);
    DestroyDynamic3DTextLabel(CeleLotnicze[i][ecl_3dlabel]);
        DestroyDynamicMapIcon(CeleLotnicze[i][ecl_mapic]);
  }
  for (new i=0;i<sizeof BazySpedycyjne;i++) {
    DestroyDynamicCP(BazySpedycyjne[i][ebs_cp]);
    DestroyDynamic3DTextLabel(BazySpedycyjne[i][ebs_3dlabel]);
        DestroyDynamicMapIcon(BazySpedycyjne[i][ebs_mapic]);
  }
  // recreate now
    for (new i=0;i<sizeof CeleLotnicze;i++) {
    CeleLotnicze[i][ecl_cp]=CreateDynamicRaceCP(3, CeleLotnicze[i][ecl_loc][0], CeleLotnicze[i][ecl_loc][1], CeleLotnicze[i][ecl_loc][2], 0, 0, 0, 14, 0,0,-1,1200);
        CeleLotnicze[i][ecl_lastplayerid]=-1;
    CeleLotnicze[i][ecl_3dlabel]=CreateDynamic3DTextLabel("Wlec tu aby\ndokonac zrzutu", -1, CeleLotnicze[i][ecl_loc][0], CeleLotnicze[i][ecl_loc][1], CeleLotnicze[i][ecl_loc][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1);
        CeleLotnicze[i][ecl_mapic]=CreateDynamicMapIcon(CeleLotnicze[i][ecl_loc][0], CeleLotnicze[i][ecl_loc][1], CeleLotnicze[i][ecl_loc][2], 57, 1, 0,0,-1,100);
  }
  for (new i=0;i<sizeof BazySpedycyjne;i++) {
    BazySpedycyjne[i][ebs_cp]=CreateDynamicCP(BazySpedycyjne[i][ebs_loc][0], BazySpedycyjne[i][ebs_loc][1],  BazySpedycyjne[i][ebs_loc][2],10, 0,0,-1,100);
        BazySpedycyjne[i][ebs_lastplayerid]=-1;
    BazySpedycyjne[i][ebs_3dlabel]=CreateDynamic3DTextLabel("Zaladunek\nlotniczy", -1, BazySpedycyjne[i][ebs_loc][0], BazySpedycyjne[i][ebs_loc][1],  BazySpedycyjne[i][ebs_loc][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1);
        BazySpedycyjne[i][ebs_mapic]=CreateDynamicMapIcon(BazySpedycyjne[i][ebs_loc][0], BazySpedycyjne[i][ebs_loc][1],  BazySpedycyjne[i][ebs_loc][2], 5, 1, 0,0,-1,100);
  }
}
// EOF
