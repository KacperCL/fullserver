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

#define FACTION_NONE  -1
#define MAX_FACTIONS  16
#define FACTION_POLICE  0
#define FACTION_ARMY  1
#define FACTION_DILERZY 2
#define FACTION_MECHANICY 3
#define FACTION_KIEROWCY  4
#define FACTION_PILOCI      5
#define FACTION_MASZYNISCI  6

enum e_faction{
  ef_name[16],
  ef_color,
  bool:ef_open
}

new Factions[][e_faction]={
  {"Policja", 0x54F0F0, true},
  {"Wojsko", 0xA60A14, true},
  {"Dilerzy", 0xFFFF10, true},
  {"Mechanicy", 0x1818b2, true},
  {"Kierowcy",  0xFFFFFF, true},
  {"Piloci",      0xFFFFFF, true},
  {"Maszynisci",      0xFFFFFF, true}
};


CMD:policja(playerid)
  return _komisariat(playerid);

OutputFactionChat(playerid, szMsg[])
{
    if(isPlayerMuted(playerid)){
        Msg(playerid,COLOR_ERROR,"Jesteœ wyciszony - nie mo¿esz pisaæ na czacie.");
    return;
  }
  format(gstr, sizeof gstr, "[%s] %s: %s", Factions[pTemp[playerid][faction]][ef_name], GetPlayerProperName(playerid), szMsg);

  foreach(i)
    if (IsPlayerConnected(i) && pTemp[i][faction]==pTemp[playerid][faction])
    {

      Msg(i, 0x618796FF, gstr);
      PlaySound(i, 1027);
    }
}

factions_CountPlayers(factionid){
  new count;
  foreach (playerid)
    if (IsPlayerConnected(playerid) && pTemp[playerid][faction]==factionid)
      count++;
  return count;
}

factions_PlayerJoin(playerid,factionid){
  if (pTemp[playerid][faction]==factionid) return;

  // warunki dolaczenia do frakcji
  switch (factionid){
    case FACTION_POLICE: {
      if (pData[playerid][hitman]>0) {  // hitman na graczu
        if (GetPlayerMoney(playerid)>=pData[playerid][hitman]) {
          GivePlayerMoney(playerid,-1 * pData[playerid][hitman]);
          pData[playerid][hitman]=0;
          Msg(playerid, COLOR_INFO, "Stawka za Twoja glowe zostaje automatycznie splacona.");
        } else {
          Msg(playerid, COLOR_ERROR, "Niestety, nie mozesz dolaczyc - posiadasz niesplacone mandaty/grzywne i nie stac Cie na ich splate!");
          return;
        }
      }
    }
    case FACTION_MECHANICY: {
      if (factions_CountPlayers(FACTION_MECHANICY)>=4) {
        Msg(playerid, COLOR_ERROR, "W warsztacie jest juz komplet mechanikow, nie mozesz dolaczyc");
        return;
      }
    }
  }

  pTemp[playerid][faction]=factionid;
  format(gstr, sizeof gstr, "_~n~_~n~_~n~%s",Factions[pTemp[playerid][faction]][ef_name]);
  GameTextForPlayer(playerid, gstr, 3000, 6);

  format(gstr, sizeof gstr, "Dolaczasz do frakcji {b}%s{/b}. Komunikacja z innymi czlonkami - {b}#tekst{/b}.", Factions[pTemp[playerid][faction]][ef_name]);
  Msg(playerid,COLOR_INFO,gstr);

  Msg(playerid,COLOR_INFO,"Aby opuscic frakcje uzyj komendy {b}/fe{/b}");

  OutputFactionChat(playerid, "zglaszam sie.");
  factions_SetPlayerAttObject(playerid);
  UpdatePlayerFactionName(playerid);
}

factions_SetPlayerAttObject(playerid){

  if (pTemp[playerid][faction]==FACTION_NONE) return;
  switch(pTemp[playerid][faction]){
    case FACTION_POLICE:
      SetPlayerAttachedObject( playerid, ATTACH_FACTION, 18637, 13, 0.360721, 0.000000, 0.000000, 0.000000, 0.000000, 211.167129, 1.000000, 1.000000, 1.000000 ); // PoliceShield1 - tarcza
  }

}

CMD:fe(playerid){
  if(pTemp[playerid][faction]==FACTION_NONE) return Msg(playerid,COLOR_ERROR,"Nie jestes w zadnej frakcji");
  OutputFactionChat(playerid, "opuszczam frakcje");
  RemovePlayerAttachedObject(playerid, ATTACH_FACTION);
  Msg(playerid,COLOR_INFO,"Opuœci³eœ swoj¹ frakcje");
  pTemp[playerid][faction]=FACTION_NONE;
  UpdatePlayerFactionName(playerid);
  return 1;
}
// EOF
