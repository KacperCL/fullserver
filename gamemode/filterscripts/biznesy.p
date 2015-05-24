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

enum e_biznes {
  eb_nazwa[64],
  eb_koszt,     // koszt zakupu
  eb_dochod,      // dochod co X minut
  Float:eb_loc[3],  // polozenie x,y,z
  eb_depozyt,     // zebrana ilosc gotowki
  Text3D:eb_3dlabel,
  eb_pickup,
  eb_owner
}

#define DLG_BIZNESY     8000
#define DLG_BIZNESY_BUYCONFIRM  (DLG_BIZNESY)
#define DLG_BIZNESY_LIST    (DLG_BIZNESY+1)
#define DLG_BIZNESY_AKCJA       (DLG_BIZNESY+2)
#define MAX_BIZNESOW_NA_GRACZA  2
#define MAX_BIZNESOW_NA_VIPA  4
#define BIZNESOW    32

#define BIZNES_INTERVAL (1000*60*10)
#define BIZNES_DOCHOW_EACH_RANDOM (random(1500)+250)*2
new dochod_timer;
new biznes_akcja[MAX_PLAYERS];

new Biznesy[BIZNESOW][e_biznes]={
//    nazwa         koszt   dochod    polozenie
  { "Dziupla",        1000,   100,    {811.06,-564.51,16.33} },
  { "Stare lotnisko",     90000,    9000,   {414.01,2537.1,19.15} },
  { "Kemping",                4000,      400,       {-65.1575,-1575.2144,2.6107} },
  { "Lotnisko SF",      170000, 17000,    {-1543.9,-441.2,6.0} },
  { "Salon samochodowy SF", 40000,  4000,   {-1952.25,291.22,41.05} },
  { "Historyczny Statek Piratow", 25000,  2500,   {2000.6,1527.6,14.6} },
  { "Lotnisko LV",      180000,   18000,    {1672.56,1447.87,10.79} },
  { "Program SETI",       250000,    25000,   {-329.1968,1536.5391,76.6117} },
  { "Lotnisko LS",      220000,   22000,    {1643.6,-2285.8,-1.2} },
  { "Piramida",       38000,  3800,   {2238.3,1285.6,10.8} },
  { "Port w LS",        90000,  9000,   {2730.7,-2451.6,17.5} },
  { "Burdel w LS",      52000,  5200,   {2413.0,-1221.1,25.2} },
  { "Warsztat samochodowy w LV", 55000,   5500,   {1056.0,1749.6,12.4} },
  { "Baza projektu Skynet",     99000,    9900,   {211.9246,1807.2947,21.8672} },
  { "Marynarka wojskowa",       90000,    9000,   {-1521.8,480.3,7.1} },
  // { "Baza NASA",       250000,    12000,   {-2917.4,437.2,4.9} },
  { "Baza radiowo-telewizyjna 'Sky News'",        90000,    9000,   {-2521.2,-624.1,132.7} },
  { "Elektrownia",      95000,    9500,   {292.9,1430.5,10.2} },
  { "Stacja paliw w San Fierro",        6000,   600,    {-2034.90,148.63,28.84}},
  { "Sklep z bronia w Los Santos",    7200,   750,    {1368.99,-1279.70,13.54}},
  { "TVN24",          82000,    8200,   {1154.95,-1180.90,32.81}},
  { "Budka z hotdogami",    850,    120,   {-2200.97,  -2386.62,30.62}},
  { "Urzad miejski Los Santos",   15000,  1500,   {1481.02,-1772.31,18.79}},
  { "Elektrocieplownia",    40000,    4000,   {-1029.48,  -590.52,  32.01}},
  { "Sklep rowerowy",     3800,   380,    {701.68,  -518.47,  16.32}},
  { "Paliwa ekologiczne",   29000,  2900,   {1352.07, 348.38, 20.50}},
  { "Hodowla slimakow",   3900, 390,    {1925.56,170.03,37.28}},
  { "Hodowla szczupaka",    4500, 450,    {-1644.44,-2233.46,29.57}},
  { "Bar ze striptizem",    6500, 650,    {-1562.66,-2733.11,48.74}},
  { "Fabryka gumek",      24000,  2400,   {-1827.10,42.27,15.12}},
  { "Amber Gold",             390000, 19000,      {2301.33, -10.10, 26.48}},
  { "Reaktor atomowy",        150000, 15000,      {1054.34, 1045.28, 10.27}},
  { "Zloza naftowe",          220000, 22000,           {583.50, 1219.84, 11.71}}
};
new gstr[2024];

public OnFilterScriptInit(){
  for (new i=0;i<sizeof Biznesy;i++) {
    format(gstr,sizeof gstr,"%s\n{3030FF}%d$", Biznesy[i][eb_nazwa],Biznesy[i][eb_koszt]);
    Biznesy[i][eb_3dlabel]=CreateDynamic3DTextLabel(gstr, -1, Biznesy[i][eb_loc][0], Biznesy[i][eb_loc][1], Biznesy[i][eb_loc][2]+1, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1);
    Biznesy[i][eb_pickup]=CreateDynamicPickup(1274, 1, Biznesy[i][eb_loc][0], Biznesy[i][eb_loc][1], Biznesy[i][eb_loc][2], 0, 0, -1);
    Biznesy[i][eb_owner]=INVALID_PLAYER_ID;
  }
  dochod_timer=SetTimer("biznesy_Dochody", BIZNES_INTERVAL,true);
  SendClientMessageToAll(-1,"[BIZNESY] Biznesy zostaly przeladowane");
  return 1;
}

public OnFilterScriptExit(){
  KillTimer(dochod_timer);
}

CMD:biznesy(playerid){
  format(gstr,sizeof gstr,"Nazwa\tKoszt\tWlasciciel\n");
  for (new i=0;i<sizeof Biznesy;i++) {
    if (IsPlayerConnected(Biznesy[i][eb_owner])) {
        if(Biznesy[i][eb_owner]==playerid){
          format(gstr,sizeof gstr,"%s%s{FFFF00}%24s\t%d$\tWLASCICIEL", gstr, i>0?("\n"):(""), Biznesy[i][eb_nazwa], Biznesy[i][eb_koszt]);
        }else{
          format(gstr,sizeof gstr,"%s%s{FF0000}%24s\t%d$\t%s", gstr, i>0?("\n"):(""), Biznesy[i][eb_nazwa], Biznesy[i][eb_koszt], GetPlayerNick(Biznesy[i][eb_owner]));
      }
    } else {
      format(gstr,sizeof gstr,"%s%s{00FF00}%24s\t%d$\tWOLNY", gstr, i>0?("\n"):(""), Biznesy[i][eb_nazwa], Biznesy[i][eb_koszt]);
    }
  }
  ShowPlayerDialog(playerid,DLG_BIZNESY_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Lista biznesow do kupienia", gstr, "Zamknij", "");
  return 1;

}

getBiznesPickupID(element){
  for (new i=0;i<sizeof Biznesy;i++)
    if (element==Biznesy[i][eb_pickup])
      return i;
  return -1;
}

getNearbyBiznesID(playerid){
  new foundid=-1;
  new Float:lastdist,Float:dist;

  for (new i=0;i<sizeof Biznesy;i++) {
    dist=GetPlayerDistanceFromPoint(playerid, Biznesy[i][eb_loc][0], Biznesy[i][eb_loc][1], Biznesy[i][eb_loc][2]);
    if (dist>10) continue;
    if (foundid==-1 || lastdist>dist) {
      foundid=i;
      lastdist=dist;
    }
  }
  return foundid;
}

public OnPlayerConnect(playerid){
  new preserveData = CallRemoteFunction("IsPlayerCountedAndNotExpired", "d",playerid);
  if(!preserveData){
    for (new i=0;i<sizeof Biznesy;i++){
      if (Biznesy[i][eb_owner]==playerid) {
        Biznesy[i][eb_owner]=INVALID_PLAYER_ID;
        Biznesy[i][eb_depozyt]=0;
        biznesy_UpdateLabel(i);
      }
    }
    biznes_akcja[playerid]=-1;
  }
  return 1;
}

ileMaBiznesow(playerid) {
  new ilosc=0;
  for (new i=0;i<sizeof Biznesy;i++)
    if (Biznesy[i][eb_owner]==playerid)
      ilosc++;
  return ilosc;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid) {
  new biznes=getBiznesPickupID(pickupid);
  if (pickupid<0) return 1;
  if (biznes<0) return 1;

  if (Biznesy[biznes][eb_owner]!=INVALID_PLAYER_ID) {
      if (Biznesy[biznes][eb_owner]==playerid) {  // wlasciciel odwiedza swoj biznes
          format(gstr,sizeof gstr,"Odbierz depozyt ({ffff00}$%d{ffffff})\nSprzedaj",Biznesy[biznes][eb_depozyt]);
          ShowPlayerDialog(playerid, DLG_BIZNESY_AKCJA, DIALOG_STYLE_LIST, "Wybierz opcje", gstr, "Wybierz", "Zamknij");
          biznes_akcja[playerid]=biznes;
          return 1;
      }

      // ten biznes jest kogos innego
      format(gstr,sizeof gstr,"_~n~_~n~~y~%s~n~_~n~_~w~Wlasciciel: ~b~_%s_~n~_~n~_~n~~g~%d$", Biznesy[biznes][eb_nazwa], GetPlayerNick(Biznesy[biznes][eb_owner]), Biznesy[biznes][eb_koszt]);
      GameTextForPlayer(playerid, gstr, 5000, 6);
      return 1;
  }
  format(gstr,sizeof gstr,"_~n~_~n~~y~%s~n~_~n~_~n~_~n~_~n~~g~%d$", Biznesy[biznes][eb_nazwa], Biznesy[biznes][eb_koszt]);
  GameTextForPlayer(playerid, gstr, 5000, 6);

  if (CallRemoteFunction("GetPlayerMoneyEx", "d", playerid)<Biznesy[biznes][eb_koszt]) {
    format(gstr, sizeof gstr, "[BIZNESY] Ta posiadlosc kosztuje {ffff00}$%d{ffffff}, wroc tu z taka gotowka, aby dokonac zakupu.", Biznesy[biznes][eb_koszt]);
    SendClientMessage(playerid,-1,gstr);
    return 1;
  }
  // gosc ma kase, zapytajmy czy chce kupic
  format(gstr, sizeof gstr, "{ffffff}Czy chcesz kupic {ffff00}%s{ffffff} za {ffff00}$%d{ffffff}?", Biznesy[biznes][eb_nazwa], Biznesy[biznes][eb_koszt]);
  ShowPlayerDialog(playerid,DLG_BIZNESY_BUYCONFIRM, DIALOG_STYLE_MSGBOX, "Potwierdz zakup biznesu", gstr, "TAK", "NIE");
  return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
  switch (dialogid) {
      case DLG_BIZNESY_AKCJA: {
        if (!response) return 1;
        if(listitem==1){
          if (Biznesy[biznes_akcja[playerid]][eb_owner]==playerid && biznes_akcja[playerid]!=-1) { //nie powinno sie zdarzyc, ale sprawdzmy
            Biznesy[biznes_akcja[playerid]][eb_owner]=INVALID_PLAYER_ID;
            Biznesy[biznes_akcja[playerid]][eb_depozyt]=0;
            biznesy_UpdateLabel(biznes_akcja[playerid]);
            CallRemoteFunction("GivePlayerMoneyEx", "dd",playerid, (Biznesy[biznes_akcja[playerid]][eb_koszt])/2);
            SendClientMessage(playerid,-1,"[BIZNESY] Sprzedales swoj biznes.");
          }
        }else if(listitem==0){
          if (Biznesy[biznes_akcja[playerid]][eb_depozyt]>0) {
            format(gstr,sizeof gstr,"_~n~_~n~_~n~_~n~_~n~Odbierasz~n~~g~~h~$~g~%d", Biznesy[biznes_akcja[playerid]][eb_depozyt]);
            GameTextForPlayer(playerid, gstr, 5000, 6);
            CallRemoteFunction("GivePlayerMoneyEx", "dd",playerid, (Biznesy[biznes_akcja[playerid]][eb_depozyt]));
            Biznesy[biznes_akcja[playerid]][eb_depozyt]=0;
            biznesy_UpdateLabel(biznes_akcja[playerid]);
          } else {
            SendClientMessage(playerid, -1, "[BIZNESY] Brak dochodu do odebrania.");
            GameTextForPlayer(playerid, "_~n~_~n~_~n~_~n~_~n~Brak dochodu do odebrania.", 5000, 6);
          }
        }
    }
    case DLG_BIZNESY_BUYCONFIRM: {
      if (!response) return 1;
      new biznes=getNearbyBiznesID(playerid);
      if (biznes<0) {
        SendClientMessage(playerid, -1, "[BIZNESY] Nie odnaleziono zadnego biznesu w poblizu");
        return 1;
      }
      // ma kase na biznes?
      if (CallRemoteFunction("GetPlayerMoneyEx", "d", playerid)<Biznesy[biznes][eb_koszt]) {
        format(gstr, sizeof gstr, "[BIZNESY] Ta posiadlosc kosztuje{ffff00}$%d{ffffff}, wroc tu z taka gotowka, aby dokonac zakupu.", Biznesy[biznes][eb_koszt]);
        SendClientMessage(playerid,-1,gstr);
        return 1;
      }

      if(CallRemoteFunction("IsPlayeRVIP", "d", playerid)){
        if (ileMaBiznesow(playerid)>=MAX_BIZNESOW_NA_VIPA){
          SendClientMessage(playerid,-1,"[BIZNESY] Posiadasz juz maksymalna ilosc biznesow.");
          return 1;
        }
      }else{
          if (ileMaBiznesow(playerid)>=MAX_BIZNESOW_NA_GRACZA){
          SendClientMessage(playerid,-1,"[BIZNESY] Posiadasz juz maksymalna ilosc biznesow.");
          return 1;
        }
      }

      if (Biznesy[biznes][eb_owner]!=INVALID_PLAYER_ID && IsPlayerConnected(Biznesy[biznes][eb_owner])) {
        SendClientMessage(playerid,-1,"[BIZNESY] Ten biznes juz ma wlasciciela!");
        return 1;
      }

      // ma i chce kupic
      CallRemoteFunction("GivePlayerMoneyEx", "dd",playerid, -1*(Biznesy[biznes][eb_koszt]));

      Biznesy[biznes][eb_owner]=playerid;
      Biznesy[biznes][eb_depozyt]=0;
      biznesy_UpdateLabel(biznes);
      format(gstr,sizeof gstr,"_~n~_~n~~y~%s~n~_~n~_~w~Wlasciciel: ~b~%s~n~_~n~_~n~~g~$%d", Biznesy[biznes][eb_nazwa], GetPlayerNick(Biznesy[biznes][eb_owner]), Biznesy[biznes][eb_koszt]);
      GameTextForPlayer(playerid, gstr, 5000, 6);
      return 1;
    }
  }
  return 0;
}

GetPlayerNick(playerid)
{
  new
   szPlayerName[24];

  GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

  return szPlayerName;
}

biznesy_UpdateLabel(i) {
  DestroyDynamic3DTextLabel(Biznesy[i][eb_3dlabel]);
  if (Biznesy[i][eb_owner]!=INVALID_PLAYER_ID)
    format(gstr,sizeof gstr,"{FFFF00}%s\n{FFFFFF}Wlasciciel: %s\n\n{3030FF}$%d", Biznesy[i][eb_nazwa], GetPlayerNick(Biznesy[i][eb_owner]), Biznesy[i][eb_depozyt]);
  else
    format(gstr,sizeof gstr,"%s\n{3030FF}%d$", Biznesy[i][eb_nazwa],Biznesy[i][eb_koszt]);
  Biznesy[i][eb_3dlabel]=CreateDynamic3DTextLabel(gstr, -1, Biznesy[i][eb_loc][0], Biznesy[i][eb_loc][1], Biznesy[i][eb_loc][2]+1, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1);
}


forward biznesy_Dochody();
public biznesy_Dochody(){
  for (new i=0;i<sizeof Biznesy;i++)
    if (Biznesy[i][eb_owner]!=INVALID_PLAYER_ID) {
      if (!IsPlayerConnected(Biznesy[i][eb_owner])) {
        Biznesy[i][eb_owner]=INVALID_PLAYER_ID;
      } else {
        Biznesy[i][eb_depozyt]+=(Biznesy[i][eb_dochod]+BIZNES_DOCHOW_EACH_RANDOM);
        biznesy_UpdateLabel(i);
        format(gstr,sizeof gstr,"[BIZNESY] {ffffff}Dochod do odebrania - {ffff00}$%d {ffffff}w {ffff00}%s{ffffff}.", Biznesy[i][eb_depozyt], Biznesy[i][eb_nazwa]);
        SendClientMessage(Biznesy[i][eb_owner],-1,gstr);
      }
    }
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
  if(success)
  {
    CallRemoteFunction("PlayerCommandPerformedOutside", "isi", playerid, cmdtext, success);
  }
}
// EOF