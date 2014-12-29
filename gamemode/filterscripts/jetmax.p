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

new mg_spedycja_cel[MAX_PLAYERS]={-1,...};
new mg_spedycja_pociag[MAX_PLAYERS]={INVALID_VEHICLE_ID, ... };
new mg_spedycja_mapicon[MAX_PLAYERS];
new Float:mg_spedycja_odleglosc[MAX_PLAYERS];

new paczki[MAX_PLAYERS];

enum e_BazaTransportowa {
  Float:ebt_loc[4],
  ebt_nazwa[72],
  ebt_mapicon,
  ebt_cp,

}

new BazyTransportowe[21][e_BazaTransportowa]={
  { {-1518.91, -2306.20, -0.12,10.0}, "Zatoka w Shady Creeks"},
  { {-1886.58, -1442.11, -0.16,10.0}, "Zlomowisko w Whetstone"},
  { {-2534.8, -894.26, -0.14,10.0}, "Gora Chiliad - czesc polnocna"},
  { {-2712.26, -2182.99, -0.08,10.0}, "Plaza obok gory Chiliad (czesc polnocnozachodnia)"},
  { {-2367.8, -2868.95, -0.18,10.0}, "Plaza obok gory Chiliad (czesc poludniowa)"},
  { {-1053.74, -2890.29, -0.3,10.0}, "Baza gangu pod autostrada, Back o Beyond"},
  { {1.27, -2431.97, -0.24,10.0}, "Motel, wschodnie Back o Beyond"},
  { {-245.57, -1847.93, -0.35,10.0}, "Los Santos Inlet"},
  { {24.63, -1106.27, -0.2,10.0}, "Baza transportowa RS Haul"},
  { {-107.72, -545.39, -0.25,10.0}, "Flint Water"},
  { {904.51, -131.8, -0.21,10.0}, "Fern Ridge"},
  { {2102.46, -109.53, -0.14,10.0}, "Fisher's Lagoon, Palomino Creek"},
  { {2291.86, 517.03, -0.1,10.0}, "Molo, poludniowe LV"},
  { {511.32, 365.98, -0.07,10.0}, "Fallow Bridge"},
  { {-797.2, 217.88, -0.17,10.0}, "The Panopticon"},
  { {-1073.1, -209.01, -0.19,10.0}, "Lotnisko w SF"},
  { {-333.92, -470.07, -0.11,10.0}, "Lesna baza transportowa"},
  { {71.92, -1144.85, -0.15,10.0}, "Baza transportowa RS Haul"},
  { {-247.44, -1887.48, -0.18,10.0}, "Los Santos Inlet"},
  { {72.95, -2597.95, -0.27,10.0}, "CPN, wschodnie Back o Beyond"},
  { {-1018.09, -2895.04, -0.17,10.0}, "Baza gangu pod autostrada, Back o Beyond"}
};

public OnFilterScriptInit(){
  for(new i=0;i<sizeof BazyTransportowe;i++) {
    BazyTransportowe[i][ebt_cp]=CreateDynamicCP( BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2],BazyTransportowe[i][ebt_loc][3], 0, 0, -1);
    BazyTransportowe[i][ebt_mapicon]=CreateDynamicMapIcon(BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2], 9, 1, 0,0,-1,100);
  }
  SendClientMessageToAll(-1,"[JETMAX] Misje spedycyjne (plywanie jetmaxem) zostaly przeladowane");
}

public OnPlayerEnterDynamicCP(playerid,checkpointid){
  for(new i=0;i<sizeof BazyTransportowe;i++)
    if (checkpointid==BazyTransportowe[i][ebt_cp]){
      if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return 1;
      SendClientMessage(playerid,-1," ");
      new vid=GetPlayerVehicleID(playerid);
      new vmodel;
      new buf[128];
      GameTextForPlayer(playerid, BazyTransportowe[i][ebt_nazwa], 1000, 1);

      if (vid>0)
        vmodel=GetVehicleModel(vid);

      if (vid==0 || vid==INVALID_VEHICLE_ID || vmodel!=493) { // jetmax
        SendClientMessage(playerid,-1,"[JETMAX] Podplyn lodzia (JETMAX), aby rozpoczac przemyt dla dilerow");
      } else if (mg_spedycja_cel[playerid]==i && paczki[playerid]>0) {  // klient dojechal do celu!
        mg_spedycja_Cel(playerid,i);
      } else if (mg_spedycja_cel[playerid]>-1 && paczki[playerid]>0) {
        format(buf,sizeof buf,"[JETMAX] Dotarles do niewlasciwej bazy! Masz dostarczyc ladunek do {ffff00}%s",BazyTransportowe[mg_spedycja_cel[playerid]][ebt_nazwa]);
        SendClientMessage(playerid,-1,buf);
      } else {
        // rozpoczynamy transport
        mg_spedycja_Inicjuj(playerid,i);
      }

      SendClientMessage(playerid,-1," ");
      return 1;
    }

  return 0;
}

public OnPlayerUpdate(playerid){
  if (GetTickCount()%2==0) return 1;    // raz na 2 razy kontynuujemy

  if(mg_spedycja_cel[playerid]>=0)
  {
    if(GetVehicleModel(GetPlayerVehicleID(playerid)) != 493){
      SendClientMessage(playerid,-1," ");
      SendClientMessage(playerid,-1,"[JETMAX] Zadanie spedycyjne zostalo przerwane!");
      SendClientMessage(playerid,-1," ");
      mg_spedycja_cel[playerid]=-1;
      paczki[playerid]=0;
      SetPVarInt(playerid,"jetmax",0);
      mg_spedycja_pociag[playerid]=INVALID_VEHICLE_ID;
      if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
        DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
        mg_spedycja_mapicon[playerid]=-1;
      }
    }
  }
  return 1;
}

public OnPlayerConnect(playerid) {
  if (mg_spedycja_cel[playerid]>=0) {
    mg_spedycja_cel[playerid]=-1;
    paczki[playerid]=0;
    SetPVarInt(playerid,"jetmax",0);
    mg_spedycja_pociag[playerid]=INVALID_VEHICLE_ID;
    if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
      DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
      mg_spedycja_mapicon[playerid]=-1;
    }
  }
  return 1;
}

mg_spedycja_Inicjuj(playerid,bazastartowa){
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return;
  new bazadocelowa=bazastartowa;
  while (bazadocelowa==bazastartowa)
    bazadocelowa=random(sizeof BazyTransportowe);

  // okreslamy rodzaj przyczepy i informujemy o przewozonym towarze ;]
  mg_spedycja_pociag[playerid]=GetPlayerVehicleID(playerid);
  switch (random(5)){
    case 0: SendClientMessage(playerid,-1,"[JETMAX] Przemycasz {ffff00}kokaine");
    case 1: SendClientMessage(playerid,-1,"[JETMAX] Przemycasz {ffff00}bron");
    case 2: SendClientMessage(playerid,-1,"[JETMAX] Przemycasz {ffff00}heroine");
    case 3: SendClientMessage(playerid,-1,"[JETMAX] Przemycasz {ffff00}marihuane");
    case 4: SendClientMessage(playerid,-1,"[JETMAX] Przemycasz {ffff00}kradzione komputery");
  }

  paczki[playerid]=random(10)+10;

  SetPVarInt(playerid,"jetmax",1);

  mg_spedycja_odleglosc[playerid] = GetVehicleDistanceFromPoint(GetPlayerVehicleID(playerid), BazyTransportowe[bazadocelowa][ebt_loc][0], BazyTransportowe[bazadocelowa][ebt_loc][1],BazyTransportowe[bazadocelowa][ebt_loc][2]);
  mg_spedycja_cel[playerid]=bazadocelowa;
  new buf[128];
  format(buf,sizeof buf,"[JETMAX] Pierwsze miejsce docelowe: {ffff00}%s{ffffff}, odleglosc: {ffff00}%.2f", BazyTransportowe[bazadocelowa][ebt_nazwa],mg_spedycja_odleglosc[playerid]);
  SendClientMessage(playerid, -1, buf);

  format(buf,sizeof buf,"[JETMAX] Ilosc paczek do dostarczenia, aby otrzymax extra bonus: {ffff00}%d", paczki[playerid]);
  SendClientMessage(playerid, -1, buf);

  mg_spedycja_mapicon[playerid]=CreateDynamicMapIcon(BazyTransportowe[bazadocelowa][ebt_loc][0], BazyTransportowe[bazadocelowa][ebt_loc][1],BazyTransportowe[bazadocelowa][ebt_loc][2], 16, 1, 0,0,playerid,6000);
  Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, mg_spedycja_mapicon[playerid], E_STREAMER_STYLE, 3);

  SendClientMessage(playerid, -1, "[JETMAX] Punkt docelowy zostal zaznaczony na Twoim radarze literka C");
}

mg_spedycja_Cel(playerid,bazastartowa){
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return 1;
  new curvehid=GetPlayerVehicleID(playerid);
  if (mg_spedycja_pociag[playerid]!=curvehid) {
    SendClientMessage(playerid,-1,"[JETMAX] Dotarles do celu, ale najwyrazniej zgubiles swoj ladunek!");

  } else {
    new kwota;
    new score;
    SendClientMessage(playerid,-1,"[JETMAX] Przemyciles towar do celu! Dostajesz wyplate!");
    if(CallRemoteFunction("GetPlayerFactionID","d",playerid)==2) {
      kwota=1800+random(700);
      score = 6;
    }
    else {
      kwota=1000+random(500);
      score = 4;
    }
    ////// distance processing //////
    kwota = kwota+(floatround(mg_spedycja_odleglosc[playerid])*2)
    score = score+(floatround(mg_spedycja_odleglosc[playerid])/400);
    ////// end distance processing ///////
    CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,kwota);
    new buf[128];
    format(buf,sizeof buf,"_~n~_~n~_~n~_~n~towar przemycony~n~~g~+%d$",kwota);
    GameTextForPlayer(playerid,buf,4000,6);
    new pnick[24];
    GetPlayerName(playerid,pnick,24);
    format(buf,sizeof buf,"[JETMAX] {ffff00}%s{ffffff} przemycil towar do bazy - zarobil {ffff00}%d${ffffff} i {ffff00}%dp. respektu", pnick, kwota, score);
    CallRemoteFunction("GivePlayerScoreEx","dd",playerid,score);
    SendClientMessageToAll(-1, buf);
  }

  if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
    DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
    mg_spedycja_mapicon[playerid]=-1;
  }

  paczki[playerid]--;

  if(paczki[playerid]==1) SendClientMessage(playerid,-1,"[JETMAX] Zostala Ci {ffff00}ostatnia paczka{ffffff}. Dostarcz ja, aby otrzymac extra bonus");
  else if(paczki[playerid]>1){
    new buf[128];
    format(buf,sizeof buf,"[JETMAX] Pozostalo Ci {ffff00}%d{ffffff} paczek, aby otrzymac extra bonus!", paczki[playerid]);
    SendClientMessage(playerid,-1,buf);
  }


  if(paczki[playerid]==0){
    SendClientMessage(playerid,-1,"[JETMAX] Przemyciles wszystkie towary! Otrzymujesz extra bonus!");
    new kwota=3000+random(1050);
    new rand=random(3)+1;
    new respekt= 5 * rand;
    CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,kwota);
    new buf[128];
    format(buf,sizeof buf,"_~n~_~n~_~n~_~n~extra bonus~n~~g~+%d$",kwota);
    GameTextForPlayer(playerid,buf,4000,6);
    CallRemoteFunction("GivePlayerScoreEx","dd",playerid,respekt);
    new pnick[24];
    GetPlayerName(playerid,pnick,24);
    format(buf,sizeof buf,"[JETMAX] {ffff00}%s{ffffff} przemycil wszystkie paczki i otrzymuje bonus {ffff00}%d${ffffff} i {ffff00}%dp. respektu", pnick, kwota, respekt);
    SendClientMessageToAll(-1, buf);

    mg_spedycja_cel[playerid]=-1;
    paczki[playerid]=0;
    mg_spedycja_pociag[playerid]=INVALID_VEHICLE_ID;
    if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
      DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
      mg_spedycja_mapicon[playerid]=-1;
    }
    SetPVarInt(playerid,"jetmax",0);

    return 1;
  }

  new bazadocelowa=bazastartowa;
  while (bazadocelowa==bazastartowa)
    bazadocelowa=random(sizeof BazyTransportowe);

  mg_spedycja_cel[playerid]=bazadocelowa;
  new buf[128];
  format(buf,sizeof buf,"[JETMAX] Nastepne miejsce docelowe: {ffff00}%s", BazyTransportowe[bazadocelowa][ebt_nazwa]);

  SendClientMessage(playerid, -1, buf);

  mg_spedycja_mapicon[playerid]=CreateDynamicMapIcon(BazyTransportowe[bazadocelowa][ebt_loc][0], BazyTransportowe[bazadocelowa][ebt_loc][1],BazyTransportowe[bazadocelowa][ebt_loc][2], 16, 1, 0,0,playerid,6000);
  Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, mg_spedycja_mapicon[playerid], E_STREAMER_STYLE, 3);

  return 1;
}

public OnPlayerStateChange(playerid,newstate,oldstate){
  if (newstate!=PLAYER_STATE_DRIVER && mg_spedycja_cel[playerid]>-1) {
    SendClientMessage(playerid,-1," ");
    SendClientMessage(playerid,-1,"[JETMAX] Zadanie spedycyjne zostalo przerwane!");
    SendClientMessage(playerid,-1," ");
    mg_spedycja_cel[playerid]=-1;
    paczki[playerid]=0;
    SetPVarInt(playerid,"jetmax",0);
    mg_spedycja_pociag[playerid]=INVALID_VEHICLE_ID;
    if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
      DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
      mg_spedycja_mapicon[playerid]=-1;
    }
  }
  return 1;
}
// EOF
