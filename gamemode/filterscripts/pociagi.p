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
new pociag1,pociag2,pociag3,pociag4; //trzeci jest juz postawiony

enum e_BazaTransportowa {
  Float:ebt_loc[4],
  ebt_nazwa[36],
  ebt_mapicon,
  ebt_cp,

}

new BazyTransportowe[5][e_BazaTransportowa]={
  { {815.61,-1366.93,-2.11,20.0}, "Stacja w Los Santos" },
  { {2864.94, 1273.78, 10.38,20.0}, "Stacja w Las Venturas #1"  },
  { {1424.10, 2636.38, 10.38,20.0}, "Stacja w Las Venturas #2"  },
  { {-1948.51,-20.11,25.27,20.0}, "Stacja w San Fierro" },
  { {738.97,1958.31,4.90,20.0},"Stacja na pustynii w Las venturas"  }
};

public OnFilterScriptInit(){
  for(new i=0;i<sizeof BazyTransportowe;i++) {
    BazyTransportowe[i][ebt_cp]=CreateDynamicCP( BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2],BazyTransportowe[i][ebt_loc][3], 0, 0, -1);
    BazyTransportowe[i][ebt_mapicon]=CreateDynamicMapIcon(BazyTransportowe[i][ebt_loc][0], BazyTransportowe[i][ebt_loc][1],BazyTransportowe[i][ebt_loc][2], 11, 1, 0,0,-1,100);
  }
  SendClientMessageToAll(-1,"[POCIAGI] Misje spedycyjne (pociagi) zostaly przeladowane");
  pociag1=AddStaticVehicle(537, -1765.15, -1400.54, 13.94,44.59, 14, 14);
  pociag2=AddStaticVehicle(537, 1728.81, -1953.93, 13.10, 88.66, 14, 14);
  pociag3=AddStaticVehicle(537, 1768.3071, -1953.7418, 13.5469, 265.9507, 14, 14);
  pociag4=AddStaticVehicle(537, 2863.5732, 1654.5111, 10.8203, 354.8528, 14, 14);
}

public OnVehicleStreamIn(vehicleid,forplayerid)
{
  if(vehicleid==pociag1) SetVehicleParamsForPlayer(pociag1, forplayerid, 0, 0);
  if(vehicleid==pociag2) SetVehicleParamsForPlayer(pociag2, forplayerid, 0, 0);
  if(vehicleid==pociag3) SetVehicleParamsForPlayer(pociag3, forplayerid, 0, 0);
  if(vehicleid==pociag4) SetVehicleParamsForPlayer(pociag4, forplayerid, 0, 0);
}

public OnFilterScriptExit(){
    DestroyVehicle(pociag1);
    DestroyVehicle(pociag2);
    DestroyVehicle(pociag3);
    DestroyVehicle(pociag4);
}

public OnPlayerEnterDynamicCP(playerid,checkpointid){
  for(new i=0;i<sizeof BazyTransportowe;i++)
    if (checkpointid==BazyTransportowe[i][ebt_cp]){
      if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return 1;
      SendClientMessage(playerid,-1," ");
      new vid=GetPlayerVehicleID(playerid);
      new vmodel;
      GameTextForPlayer(playerid, BazyTransportowe[i][ebt_nazwa], 1000, 1);

      if (vid>0)
        vmodel=GetVehicleModel(vid);

      if (vid==0 || vid==INVALID_VEHICLE_ID || vmodel!=537) { // freight
        SendClientMessage(playerid,-1,"[POCIAGI] Podjedz pociagiem towarowym (Freight), aby rozpoczac zadanie spedycyjne");
      } else if (mg_spedycja_cel[playerid]==i) {  // klient dojechal do celu!
        mg_spedycja_Cel(playerid);
      } else if (mg_spedycja_cel[playerid]>-1) {
        SendClientMessage(playerid,-1,"[POCIAGI] To jescze nie ta stacja... Jedz dalej!");
      } else {
        // rozpoczynamy transport
        mg_spedycja_Inicjuj(playerid,i);
      }

      SendClientMessage(playerid,-1," ");
      return 1;
    }

  return 0;
}

public OnPlayerConnect(playerid) {
  if (mg_spedycja_cel[playerid]>=0) {
    mg_spedycja_cel[playerid]=-1;
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
    case 0: SendClientMessage(playerid,-1,"[POCIAGI] Wieziesz {ffff00}wegiel");
    case 1: SendClientMessage(playerid,-1,"[POCIAGI] Wieziesz {ffff00}paczki wojskowe");
    case 2: SendClientMessage(playerid,-1,"[POCIAGI] Wieziesz {ffff00}towar to sklepow");
    case 3: SendClientMessage(playerid,-1,"[POCIAGI] Wieziesz {ffff00}zabawki");
    case 4: SendClientMessage(playerid,-1,"[POCIAGI] Wieziesz {ffff00}czesci do samochodow");
  }

  mg_spedycja_cel[playerid]=bazadocelowa;
  new buf[128];
  format(buf,sizeof buf,"[POCIAGI] Miejsce docelowe: {ffff00}%s", BazyTransportowe[bazadocelowa][ebt_nazwa]);

  SendClientMessage(playerid, -1, buf);

  mg_spedycja_mapicon[playerid]=CreateDynamicMapIcon(BazyTransportowe[bazadocelowa][ebt_loc][0], BazyTransportowe[bazadocelowa][ebt_loc][1],BazyTransportowe[bazadocelowa][ebt_loc][2], 16, 1, 0,0,playerid,6000);
  Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, mg_spedycja_mapicon[playerid], E_STREAMER_STYLE, 3);

  SendClientMessage(playerid, -1, "[POCIAGI] Punkt docelowy zostal zaznaczony na Twoim radarze literka C");
}

mg_spedycja_Cel(playerid){
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return;
  new curvehid=GetPlayerVehicleID(playerid);
  if (mg_spedycja_pociag[playerid]!=curvehid) {
    SendClientMessage(playerid,-1,"[POCIAGI] Dojechales do celu, ale najwyrazniej zgubiles swoj ladunek!");

  } else {
    SendClientMessage(playerid,-1,"[POCIAGI] Dowiozles towar do celu! Dostajesz wyplate!");
    new kwota=3500+random(4500);
    CallRemoteFunction("GivePlayerMoneyEx","dd",playerid,kwota);
    new buf[128];
    format(buf,sizeof buf,"_~n~_~n~_~n~_~n~towar dowieziony~n~~g~+%d$",kwota);
    GameTextForPlayer(playerid,buf,4000,6);
    new pnick[24];
    GetPlayerName(playerid,pnick,24);
    format(buf,sizeof buf,"[POCIAGI] {ffff00}%s{ffffff} dostarczyl towar do bazy - zarobil {ffff00}%d${ffffff} i {ffff00}5p. respektu", pnick, kwota);
    CallRemoteFunction("GivePlayerScoreEx","dd",playerid,5);
    SendClientMessageToAll(-1, buf);
  }

  if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
    DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
    mg_spedycja_mapicon[playerid]=-1;
  }

  mg_spedycja_cel[playerid]=-1;
  mg_spedycja_pociag[playerid]=INVALID_VEHICLE_ID;

}

public OnPlayerStateChange(playerid,newstate,oldstate){
  if (newstate!=PLAYER_STATE_DRIVER && mg_spedycja_cel[playerid]>-1) {
    SendClientMessage(playerid,-1," ");
    SendClientMessage(playerid,-1,"[POCIAGI] Zadanie spedycyjne zostalo przerwane!");
    SendClientMessage(playerid,-1," ");
    mg_spedycja_cel[playerid]=-1;
    mg_spedycja_pociag[playerid]=INVALID_VEHICLE_ID;
    if (IsValidDynamicMapIcon(mg_spedycja_mapicon[playerid])) {
      DestroyDynamicMapIcon(mg_spedycja_mapicon[playerid]);
      mg_spedycja_mapicon[playerid]=-1;
    }
  }
  return 1;
}
// EOF
