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

new mg_spedycja_cel[MAX_PLAYERS]={-1,...};
new mg_spedycja_naczepa[MAX_PLAYERS]={INVALID_VEHICLE_ID, ... };
new mg_spedycja_mapicon[MAX_PLAYERS];
new Float:mg_spedycja_odleglosc[MAX_PLAYERS];

enum e_BazaTransportowa {
  Float:ebt_loc[4],
  ebt_nazwa[32],
  ebt_mapicon,
  ebt_cp,

}

new BazyTransportowe[23][e_BazaTransportowa]={
  { {667.6860,1247.6492,11.4534,20.0}, "Pustynna baza transportowa" },
  { {-490.30,-556.37,25.52,10.0}, "Lesna baza transportowa" },
  { {2228.13,-2222.40,13.54,10.0}, "Baza Ocean Docks" },
  { {-170.75,-324.31,1.42,10.0},  "Baza Red County" },
  { {2791.18,912.81,10.75,10.0},"Magazyn Rockshore LV"  },
  { {-1721.3445,0.5007,3.2075,10.0},"Easter Basin SF" },
  { {-2126.96,-88.05,35.32,10.0},"Doherty SF" },
  { {-1987.60,-1034.48,32.02,10.0},"Foster Valley SF" },
  { {-2465.56,2238.66,4.79,10.0}, "Bayside Marina"  },
  { {-1967.44,-2436.49,30.62,10.0}, "Angel Pine"  },
  { {2597.02,-2426.39,13.49,10.0},  "Ocean Docks II LS" },
  { {1545.91,1670.92,10.82,10.0}, "Lotnisko LV" },
  { {2072.34,-2284.84,13.10,10.0},  "Lotnisko LS" },
  { {-1240.57,-93.24,14.14,10.0}, "Lotnisko SF" },
  { {848.88,-1208.38,16.97,10.0}, "Studio filmowe LS"},
  { {-1005.66,-1662.45,76.37,10.0}, "Forteca"},
  { {2370.08,2740.72,10.82,10.0}, "Spinybed LV"},
  { {1139.08,1946.88,10.82,10.0}, "Whitewood estates LV"},
  { {1041.67,2115.72,10.82,10.0}, "Whitewood estates LV"},
  { {1635.37,718.51,10.82,10.0},  "Randolph IE LV"},
  { {2867.67,2613.79,10.87,10.0}, "Centrum handlowe Creek LV" },
  { {1454.70,2824.22,10.82,10.0}, "Pole golfowe LV"},
  { {-754.21,1587.95,26.96,10.0}, "Las Barrancas, pustynia" }
};


public OnFilterScriptInit(){
  for(new i=0;i<sizeof BazyTransportowe;i++) {
    BazyTransportowe[i][ebt_cp]=CreateDynamicCP( BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2],BazyTransportowe[i][ebt_loc][3], 0, 0, -1);
    BazyTransportowe[i][ebt_mapicon]=CreateDynamicMapIcon(BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2], 51, 1, 0,0,-1,100);
  }
  SendClientMessageToAll(-1,"[SPEDYCJA] Misje spedycyjne (tiry) zostaly przeladowane");
}


public OnPlayerEnterDynamicCP(playerid,checkpointid){
  for(new i=0;i<sizeof BazyTransportowe;i++)
    if (checkpointid==BazyTransportowe[i][ebt_cp]){
      if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return 1;
      new buf[128];
      SendClientMessage(playerid,-1," ");
      new vid=GetPlayerVehicleID(playerid);
      new vmodel;
      GameTextForPlayer(playerid, BazyTransportowe[i][ebt_nazwa], 1000, 1);

      if (vid>0)
        vmodel=GetVehicleModel(vid);

      if (vid==0 || vid==INVALID_VEHICLE_ID || (vmodel!=514 && vmodel!=515 && vmodel!=403)) { // linerunner, roadtrain
        SendClientMessage(playerid,-1,"[SPEDYCJA] Podjedz tirem z naczepa, aby rozpoczac zadanie spedycyjne");
      } else if (mg_spedycja_cel[playerid]==i) {  // klient dojechal do celu!
        mg_spedycja_Cel(playerid);
      } else if (!IsTrailerAttachedToVehicle(vid)) {
        SendClientMessage(playerid,-1,"[SPEDYCJA] Musisz miec naczepe aby rozpoczac spedycje");
      } else if (mg_spedycja_cel[playerid]>-1) {
        format(buf,sizeof buf,"[SPEDYCJA] Zajechales do niewlasciwej bazy! Masz zawiezc ladunek do {ffff00}%s{ffffff}",BazyTransportowe[mg_spedycja_cel[playerid]][ebt_nazwa]);
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

public OnPlayerDisconnect(playerid,reason) {
  if (mg_spedycja_cel[playerid]>=0) {
    mg_spedycja_cel[playerid]=-1;
    mg_spedycja_naczepa[playerid]=INVALID_VEHICLE_ID;
    if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
      DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
      mg_spedycja_mapicon[playerid]=-1;
    }
  }
  return 1;
}

public OnPlayerConnect(playerid) {
  if (mg_spedycja_cel[playerid]>=0) {
    mg_spedycja_cel[playerid]=-1;
    mg_spedycja_naczepa[playerid]=INVALID_VEHICLE_ID;
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
  mg_spedycja_naczepa[playerid]=GetVehicleTrailer(GetPlayerVehicleID(playerid));
  switch(GetVehicleModel(mg_spedycja_naczepa[playerid])) {
    case 450: {
      switch (random(5)){
        case 0: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}wegiel");
        case 1: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}ziemie do kwiatkow");
        case 2: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}lajno");
        case 3: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}kakao");
        case 4: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}odpadki");
      }
    }
    case 584: {
      switch (random(7)){
        case 0: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}mleko");
        case 1: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}benzyne");
        case 2: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}rope naftowa");
        case 3: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}gaz");
        case 4: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}kasze w plynie");
        case 5: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}alkohol");
        case 6: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}niebezpieczne chemikalia");

      }
    }
    default: {
      switch (random(7)){
        case 0: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}ciuchy");
        case 1: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}przetwory miesne");
        case 2: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}czesci do rowerow");
        case 3: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}sznurek do snopowiazalek");
        case 4: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}czesci komputerowe");
        case 5: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}bron i amunicje");
        case 6: SendClientMessage(playerid,-1,"[SPEDYCJA] Wieziesz {ffff00}mrozonki");
      }
    }
  }


  mg_spedycja_cel[playerid]=bazadocelowa;
  mg_spedycja_odleglosc[playerid] = GetVehicleDistanceFromPoint(GetPlayerVehicleID(playerid), BazyTransportowe[bazadocelowa][ebt_loc][0], BazyTransportowe[bazadocelowa][ebt_loc][1],BazyTransportowe[bazadocelowa][ebt_loc][2]);
  new buf[128];
  format(buf,sizeof buf,"[SPEDYCJA] Miejsce docelowe: {ffff00}%s{ffffff}, odleglosc: {ffff00}%.2fm", BazyTransportowe[bazadocelowa][ebt_nazwa],mg_spedycja_odleglosc[playerid]);

  SendClientMessage(playerid, -1, buf);

  mg_spedycja_mapicon[playerid]=CreateDynamicMapIcon(BazyTransportowe[bazadocelowa][ebt_loc][0], BazyTransportowe[bazadocelowa][ebt_loc][1],BazyTransportowe[bazadocelowa][ebt_loc][2], 16, 1, 0,0,playerid,6000);
  Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, mg_spedycja_mapicon[playerid], E_STREAMER_STYLE, 3);


  SendClientMessage(playerid, -1, "[SPEDYCJA] Punkt docelowy zostal zaznaczony na Twoim radarze literka C");
}

mg_spedycja_Cel(playerid){
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return;
  new curtrailer=GetVehicleTrailer(GetPlayerVehicleID(playerid));
  if (mg_spedycja_naczepa[playerid]!=curtrailer) {
    SendClientMessage(playerid,-1,"[SPEDYCJA] Dojechales do celu, ale najwyrazniej zgubiles swoj ladunek!");

  } else {
    SendClientMessage(playerid,-1,"[SPEDYCJA] Dowiozles towar do celu! Dostajesz wyplate!");
    new kwota=0;
    new score = 0;
    if(CallRemoteFunction("GetPlayerFactionID","d",playerid)==4) {
      kwota=8000+random(700);
      score = 8;
    }
    else {
      kwota=6000+random(500);
      score = 6;
    }
    ////// distance processing //////
    kwota = kwota+(floatround(mg_spedycja_odleglosc[playerid])*2)
    score = score+(floatround(mg_spedycja_odleglosc[playerid])/400);
    ////// end distance processing ///////
    CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,kwota);
    new buf[128];
    format(buf,sizeof buf,"_~n~_~n~_~n~_~n~towar dowieziony~n~~g~+%d$",kwota);
    GameTextForPlayer(playerid,buf,4000,6);
    new pnick[24];
    GetPlayerName(playerid,pnick,24);
    format(buf,sizeof buf,"[SPEDYCJA] {ffff00}%s{ffffff} dostarczyl towar do bazy - zarobil {ffff00}%d${ffffff} i {ffff00}%dp. respektu", pnick, kwota, score);
    CallRemoteFunction("GivePlayerScoreEx","dd",playerid,score);
    SendClientMessageToAll(-1, buf);
  }

  CallRemoteFunction("MarkVehicleAsUsed","d",mg_spedycja_naczepa[playerid]);
  if (curtrailer>0 && curtrailer!=INVALID_VEHICLE_ID)
    CallRemoteFunction("RespawnVehicle","d",curtrailer);

  if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
    DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
    mg_spedycja_mapicon[playerid]=-1;
  }

  mg_spedycja_cel[playerid]=-1;
  mg_spedycja_naczepa[playerid]=INVALID_VEHICLE_ID;

}

public OnPlayerStateChange(playerid,newstate,oldstate){
  if (newstate!=PLAYER_STATE_DRIVER && mg_spedycja_cel[playerid]>-1) {
    SendClientMessage(playerid,-1," ");
    SendClientMessage(playerid,-1,"[SPEDYCJA] Zadanie spedycyjne zostalo przerwane!");
    SendClientMessage(playerid,-1," ");
    CallRemoteFunction("MarkVehicleAsUsed","d",mg_spedycja_naczepa[playerid]);
    mg_spedycja_cel[playerid]=-1;
    mg_spedycja_naczepa[playerid]=INVALID_VEHICLE_ID;
    if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
      DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
      mg_spedycja_mapicon[playerid]=-1;
    }
  }
  return 1;
}
// EOF
