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

#define GANG_HOLDSLOT 2

new bool:gangi_sojusze[MAX_GANGS][MAX_GANGS];

forward GetPlayerGang(playerid);
public GetPlayerGang(playerid)
  return IsPlayerConnected(playerid)?pData[playerid][gang]:NO_GANG;

gangs_OnPlayerLogin(playerid){
  if (pData[playerid][gang]==NO_GANG)
    return 0;
  new buf[127];
  if (pData[playerid][gangRank]==GANG_RANK_OWNER) {
    format(buf,sizeof buf,__("*** Zalozyciel %s - %s (%d) dolacza do gry."), gData[pData[playerid][gang]][name], GetPlayerProperName(playerid), playerid);
    SendClientMessageToAll(gData[pData[playerid][gang]][gColor] * 256, buf);
  } else if (pData[playerid][gangRank]==GANG_RANK_VICEOWNER) {
    format(buf,sizeof buf,__("*** Wspó³za³o¿yciel %s - %s (%d) dolacza do gry."), gData[pData[playerid][gang]][name], GetPlayerProperName(playerid), playerid);
    SendClientMessageToAll(gData[pData[playerid][gang]][gColor] * 256, buf);
  } else if (pData[playerid][gangRank]==GANG_RANK_LEADER) {
    format(buf,sizeof buf,__("*** Lider %s - %s (%d) dolacza do gry."), gData[pData[playerid][gang]][name], GetPlayerProperName(playerid), playerid);
    SendClientMessageToAll(gData[pData[playerid][gang]][gColor] * 256, buf);
  } else {
    format(buf,sizeof buf,__("*** Cz³onek twojego gangu - %s (%d) dolacza do gry."), GetPlayerProperName(playerid), playerid);
    foreach(sendto){
      if(pData[playerid][gang] == pData[sendto][gang]) SendClientMessage(sendto, gData[pData[playerid][gang]][gColor] * 256, buf);
    }
  }
  return 1;
}

gangs_SetPlayerAttachedObject(playerid){
  if (!pData[playerid][loggedIn]) return;

}

gangs_ShowGangsList(playerid){
  new
    szGangList[1024],
    szGangName[8],
    szGangDesc[32],
    szGangColor[7],
    szGangID[4],
    i = 0,
    buffer[1024];

  format(buffer, sizeof buffer, "SELECT tag,color,name,id FROM %s", gmData[DB_gangs]);
  mysql_query(buffer);
  mysql_store_result();
  while(mysql_retrieve_row())
  {
    mysql_get_field("tag", szGangName);
    mysql_get_field("color", szGangColor);
    mysql_get_field("name", szGangDesc);
    mysql_get_field("id", szGangID);

    format(buffer, sizeof buffer, "{%s}%s\t{ffffff}%s\n", szGangColor, szGangName, szGangDesc);
    strcat(szGangList, buffer);

    pTemp[playerid][statsGangListID][i++] = StringToInt(szGangID);
  }

  strdel(szGangList, strlen(szGangList) - 1, strlen(szGangList));

  mysql_free_result();

  return  ShowPlayerDialog(playerid, DIALOG_STATS_GANG_LIST, DIALOG_STYLE_LIST, __("Statystyki > gangi"), szGangList, __("Wybierz"), __("Wroc"));
}

gangs_saveGangData(bool:force=false){
  new buf[128];
  for(new i=1;i<(MAX_GANGS+1);i++)
  if (force || random(2)==1) {
    format(buf, sizeof buf, "UPDATE fs_gangs SET respect=%d,spawnpoint='%.2f,%.2f,%.2f,%.2f' WHERE id=%d AND active=1", gData[i][respect],
            gData[i][gSpawnPoint][0], gData[i][gSpawnPoint][1], gData[i][gSpawnPoint][2], gData[i][gSpawnPoint][3],i);
        mysql_query(buf);

  }
  return;
}

OutputGangChat(playerid, szMsg[])
{
  new
   buffer[200];

  format(buffer, sizeof buffer, "GANG %s> %s: %s", gData[pData[playerid][gang]][tag], GetPlayerProperName(playerid), szMsg);

  foreach(i)
  {
    if(pData[i][loggedIn] && (pData[i][gang] == pData[playerid][gang] || ((pData[i][adminLevel] == LEVEL_ADMINHIDDEN || pData[i][adminLevel] == LEVEL_ADMIN3) && gmTemp[showGang])))
    {
      SendClientMessage(i, gData[pData[playerid][gang]][gColor] * 256, buffer);
    }
  }
  OutputLog(LOG_OCHAT, buffer);
}

OutputGangChatId(playerid, id, szMsg[])
{
  new
   buffer[200];

  format(buffer, sizeof buffer, "GANG %s> %s: %s", gData[id][tag], GetPlayerProperName(playerid), szMsg);

  foreach(i)
  {
    if(pData[i][loggedIn] && (pData[i][gang] == id || ((pData[i][adminLevel] == LEVEL_ADMINHIDDEN || pData[i][adminLevel] == LEVEL_ADMIN3) && gmTemp[showGang])))
    {
      SendClientMessage(i, gData[id][gColor] * 256, buffer);
    }
  }
  OutputLog(LOG_OCHAT, buffer);
}

OutputAllyChat(playerid, szMsg[])
{
  new
   buffer[200];

  format(buffer, sizeof buffer, "SOJUSZ> {%06x}%s{909090}: %s", gData[pData[playerid][gang]][gColor], GetPlayerNick(playerid), szMsg);

  foreach(i)
    if(pData[i][loggedIn] && ((pData[i][gang]!=NO_GANG && (pData[i][gang] == pData[playerid][gang] || gangi_sojusze[pData[playerid][gang]][pData[i][gang]])) || ((pData[i][adminLevel] == LEVEL_ADMINHIDDEN || pData[i][adminLevel] == LEVEL_ADMIN3) && gmTemp[showGang])))
      SendClientMessage(i, -1, buffer);

  OutputLog(LOG_OCHAT, buffer);
}

OutputLeaderGangChat(playerid, szMsg[])
{
  new
   buffer[200];

  format(buffer, sizeof buffer, __("LIDERZY> {%06x}%s{ffffff}: %s"), gData[pData[playerid][gang]][gColor], GetPlayerProperName(playerid), szMsg);

  foreach(i)
  {
    if(pData[i][loggedIn] && ((pData[i][gang]!=NO_GANG && (pData[i][gangRank]==GANG_RANK_OWNER || pData[i][gangRank]==GANG_RANK_VICEOWNER || pData[i][gangRank]==GANG_RANK_LEADER)) || ((pData[i][adminLevel] == LEVEL_ADMINHIDDEN || pData[i][adminLevel] == LEVEL_ADMIN3) && gmTemp[showGang])))
    {
      SendClientMessage(i, -1, buffer);
      PlaySound(i, 1002); //1082
    }
  }
  format(buffer, sizeof buffer, "[LGANG] %s: %s", GetPlayerNick(playerid), szMsg);
  OutputLog(LOG_OCHAT, buffer);
}

gangs_MSG(gangid, szMsg[])
{
  if (gangid==NO_GANG) return;
  new
   buffer[200];

  format(buffer, sizeof buffer, " (%s){ffffff} %s", gData[gangid][tag], szMsg);

  foreach(i)
    if((pData[i][gang] == gangid && pData[i][loggedIn]) || ((pData[i][adminLevel] == LEVEL_ADMINHIDDEN || pData[i][adminLevel] == LEVEL_ADMIN3) && gmTemp[showGang]))
      SendClientMessage(i, gData[gangid][gColor] * 256, buffer);

  format(buffer, sizeof buffer, "[GANG %s] %s", gData[gangid][tag], szMsg);
  OutputLog(LOG_OCHAT, buffer);
  return;
}

gangs_isPlayerGangOwnerOrLeader(playerid) {
  if (!pData[playerid][loggedIn]) return false;
  if (pData[playerid][gang]==NO_GANG) return false;
  new buf[128], bool:res=false;
  format(buf, sizeof buf, "select 1 from fs_players_in_gangs where rank in ('owner','leader','viceowner') AND id_player=%d;", pData[playerid][accountID]);
  mysql_query(buf);
  mysql_store_result();
  if (mysql_num_rows() && mysql_fetch_int()==1) res=true;
  mysql_free_result();
  return res;
}

gangs_RemovePlayerFromGang(playerid){
  if (!IsPlayerConnected(playerid))
    return printf("DEBUG gangs_RemovePlayerFromGang, ale gracz nie jest polaczony!");
  if (!pData[playerid][loggedIn])
    return printf("DEBUG gangs_RemovePlayerFromGang, ale gracz nie jest zalogowany!");
  new gangid=pData[playerid][gang];
  if (gangid==NO_GANG) return printf("DEBUG gangs_RemovePlayerFromGang, ale gracz nie jest w gangu!");

  new buf[255];
  // kasujemy dom!!! - jesli jest gangowy
  new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
  if(hid>=0 && FSDomy[hid][ed_restrict_gang]!=NO_GANG && FSDomy[hid][ed_restrict_gang]==pData[playerid][gang]) {
    format(buf,sizeof buf,"UPDATE fs_houses set paidTo='0000-00-00', ownerid=0 WHERE ownerid=%d AND id=%d AND restrict_gang=%d", pData[playerid][accountID], hid, pData[playerid][gang]);
    mysql_query(buf);
    pTemp[playerid][e_houseid]=-1;
    domy_Reload();
  }

  format(buf,sizeof buf,"DELETE FROM fs_players_in_gangs WHERE id_gang=%d AND id_player=%d", pData[playerid][gang], pData[playerid][accountID]);
  mysql_query(buf);
  pData[playerid][gang]=NO_GANG;
  SetPlayerProperColor(playerid,true);

  if (SkinKobiecy(GetPlayerSkin(playerid)))
    format(buf,sizeof buf,__("%s (%d) opuscila gang %s"), GetPlayerProperName(playerid), playerid, gData[gangid][name]);
  else
    format(buf,sizeof buf,__("%s (%d) opuscil gang %s"), GetPlayerProperName(playerid), playerid, gData[gangid][name]);

  gangs_MSG(gangid, buf);
  Msg(playerid, COLOR_INFO2, buf);

  UpdatePlayerFactionName(playerid);

  return 1;
}

gangs_SetGangSpawn(playerid){
  if (pData[playerid][gang]==NO_GANG)
    return Msg(playerid, COLOR_ERROR, __("Nie nalezysz do zadnego gangu!"));
  if(!TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if(GetPlayerVirtualWorld(playerid)>0)
    return Msg(playerid, COLOR_ERROR, __("Nie mozesz tutaj ustawic spawnu gangu!"));

  if (GetPlayerInterior(playerid)!=0)
    return Msg(playerid, COLOR_ERROR, __("Spawn gangu moze byc zapisany tylko na zewnatrz"));

  if (IsPlayerInFullDMArea(playerid))
    return Msg(playerid,COLOR_ERROR,"Nie mozna zapisac pozycji w {b}strefie smierci{/b}.");

  new gangid=pData[playerid][gang];
  switch(GetPlayerState(playerid)) {
    case PLAYER_STATE_ONFOOT: {
      GetPlayerPos(playerid, gData[gangid][gSpawnPoint][0], gData[gangid][gSpawnPoint][1], gData[gangid][gSpawnPoint][2]);
      GetPlayerFacingAngle(playerid, gData[gangid][gSpawnPoint][3]);
    }
    case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER: {
      new vid=GetPlayerVehicleID(playerid);
      GetVehiclePos(vid, gData[gangid][gSpawnPoint][0], gData[gangid][gSpawnPoint][1], gData[gangid][gSpawnPoint][2]);
      GetVehicleZAngle(vid, gData[gangid][gSpawnPoint][3]);
    }
    default:
      return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  }
  new buf[127];
  format(buf, sizeof buf, "%s ustanowil%s nowy spawn gangu.", GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
  gangs_MSG(gangid, buf);
  return 1;
}

gangs_TeleportPlayerToGangSpawn(playerid,bool:force=false){
  if (pData[playerid][gang]==NO_GANG)
    return Msg(playerid, COLOR_ERROR, __("Nie nalezysz do zadnego gangu!"));
  if(!force && !TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  new gangid=pData[playerid][gang];

  Teleport(T_BOTH, playerid, gData[gangid][gSpawnPoint][0]+random(2)-1, gData[gangid][gSpawnPoint][1]+random(2)-1, gData[gangid][gSpawnPoint][2]+random(2)-1, gData[gangid][gSpawnPoint][3]+random(10)-5, 0, VW_DEFAULT, true);
  GameTextForPlayer(playerid, __("~y~spawn gangu"), 1000, 1);
  return 1;
}

gangs_TeleportToOtherGangSpawn(playerid,gangid,bool:force=false){
  if (pData[playerid][gang]==NO_GANG)
    return Msg(playerid, COLOR_ERROR, __("Nie nalezysz do zadnego gangu!"));
  if(!gangi_sojusze[pData[playerid][gang]][gangid])
      return Msg(playerid, COLOR_ERROR, __("Wystapil blad z sojuszami! Kod bledu: #GANG1_OR_GANG2_NOT_ON_ALLIANCE"));
  if(!force && !TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, gData[gangid][gSpawnPoint][0]+random(2)-1, gData[gangid][gSpawnPoint][1]+random(2)-1, gData[gangid][gSpawnPoint][2]+random(2)-1, gData[gangid][gSpawnPoint][3]+random(10)-5, 0, VW_DEFAULT, true);
  GameTextForPlayer(playerid, __("~y~Spawn sojusznika"), 1000, 1);
  return 1;
}

gangs_TeleportPlayerToBaseSpawn(playerid,bool:force=false){
  if (pData[playerid][gang]==NO_GANG)
    return Msg(playerid, COLOR_ERROR, __("Nie nalezysz do zadnego gangu!"));
  if(!force && !TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  new gangid=pData[playerid][gang];

  Teleport(T_BOTH, playerid, gData[gangid][gBaseSpawn][0], gData[gangid][gBaseSpawn][1], gData[gangid][gBaseSpawn][2], gData[gangid][gBaseSpawn][3], 0, VW_DEFAULT, true);
  GameTextForPlayer(playerid, __("~y~wyspa gangu"), 1000, 1);
  return 1;
}

gangs_TeleportToOtherBaseSpawn(playerid,gangid,bool:force=false){
  if (pData[playerid][gang]==NO_GANG)
    return Msg(playerid, COLOR_ERROR, __("Nie nalezysz do zadnego gangu!"));
  if(!gangi_sojusze[pData[playerid][gang]][gangid])
      return Msg(playerid, COLOR_ERROR, __("Wystapil blad z sojuszami! Kod bledu: #GANG1_OR_GANG2_NOT_ON_ALLIANCE"));
  if(!force && !TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, gData[gangid][gBaseSpawn][0], gData[gangid][gBaseSpawn][1], gData[gangid][gBaseSpawn][2], gData[gangid][gBaseSpawn][3], 0, VW_DEFAULT, true);
  GameTextForPlayer(playerid, __("~y~wyspa sojusznika"), 1000, 1);
  return 1;
}

gangs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
#pragma unused listitem
  switch(dialogid){
    case DIALOG_GANG_MAINMENU: {
      if(!response) return 1;
      switch(inputtext[0]){
        case 'C': {
            if(gmData[artefactOwner] == playerid) // prevent player from changing color & alpha if artefact owner!
            {
              SetPlayerColor(playerid, 0xC6E0FA * 256+32);
              pData[playerid][currentColor] = 0xC6E0FA;
              Msg(playerid,COLOR_ERROR,"Posiadasz artefakt i nie mo¿esz zmieniæ swojego koloru!");
            } else {
               pData[playerid][currentColor]=gData[pData[playerid][gang]][gColor];
               SetPlayerColor(playerid, pData[playerid][currentColor] * 256 + pTemp[playerid][playerColorAlpha]);
               Msg(playerid,COLOR_ERROR,"Twój kolor zosta³ ustawiony na taki sam jak ma gang.");
            }
        }
        case 'W': {
          if(inputtext[1]==' ') return gangs_TeleportPlayerToBaseSpawn(playerid);
          else{
              new sojusznikow=0;
            for(new i=1;i<MAX_GANGS;i++)
            {
              if (gangi_sojusze[pData[playerid][gang]][i]) {
                  sojusznikow++;
                if(sojusznikow==strval(inputtext[1])) return gangs_TeleportToOtherBaseSpawn(playerid,i,false);
              }
            }
            }
        }
        case 'U': return gangs_SetGangSpawn(playerid);
        case 'S': {
          if(inputtext[1]==' ') return gangs_TeleportPlayerToGangSpawn(playerid);
          else{
              new sojusznikow=0;
            for(new i=1;i<MAX_GANGS;i++){
              if (gangi_sojusze[pData[playerid][gang]][i]) {
                  sojusznikow++;
                if(sojusznikow==strval(inputtext[1])) return gangs_TeleportToOtherGangSpawn(playerid,i,false);
              }
            }
          }
        }
        case 'E': {
          if (pData[playerid][loggedIn]) {
            new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
            if(hid>=0 && FSDomy[hid][ed_restrict_gang]!=NO_GANG && FSDomy[hid][ed_restrict_gang]==pData[playerid][gang])
              return ShowPlayerDialog(playerid, DIALOG_GANG_CONFIRMLEAVING, DIALOG_STYLE_MSGBOX, "Gang > opusc", "Czy na pewno chcesz opuscic ten gang?", __("Wybierz"), __("Wroc"));
          }

          return ShowPlayerDialog(playerid, DIALOG_GANG_CONFIRMLEAVING, DIALOG_STYLE_MSGBOX, "Gang > Opusc", "Czy na pewno chcesz opuscic ten gang?", __("Wybierz"), __("Wroc"));
        }
        case 'L': return gangs_ShowGangsList(playerid);
      }
    }
    case DIALOG_GANG_CONFIRMLEAVING: {
      if(!response) return _gang(playerid);
      gangs_RemovePlayerFromGang(playerid);
    }


  }
  return 1;
}

gangs_ShowGangDetails(playerid, gangid){
  new buffer[2048],buf2[512];
  new czlonkow, czlonkow_24h, czlonkow_7d,tmpresult[32];

  format(buffer, sizeof buffer, "{%06x}%s \t\t {A9C4E4}tag: {FFFFFF}%s\n\n%s\n\n{A9C4E4}Zalozyciel: {FFFFFF}%s\n{A9C4E4}Wspó³zalozyciel: {FFFFFF}%s\n{A9C4E4}Data rejestracji: {FFFFFF}%s\n\n{A9C4E4}Liderzy:\t {FFFFFF}",
        gData[gangid][gColor], gData[gangid][name], gData[gangid][tag],
        gData[gangid][www],
        GetGangOwner(gangid),
        GetGangViceOwner(gangid),
        GetGangData(gangid, "datetime_created")
      );
  format(buf2, sizeof buf2, "select p.nick from fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player AND pig.rank='leader' where pig.id_gang=%d ORDER BY nick ASC;", gangid);
  mysql_query(buf2);
  mysql_store_result();
  while (mysql_fetch_row_format(tmpresult))
      if(strlen(tmpresult)>0) {
        strcat(buffer, " ");
        strcat(buffer, tmpresult);
      }

  mysql_free_result();

  new sojusznikow=0;
  strcat(buffer,"\n{A9C4E4}Sojusznicy:\t {FFFFFF}");
  for(new i=1;i<MAX_GANGS;i++)
    if (gangi_sojusze[gangid][i]) {
      if (sojusznikow++>0)
        strcat(buffer, ", ");
      strcat(buffer, gData[i][tag]);
    }
  if (sojusznikow==0)
      strcat(buffer, "brak");

  format(buf2, sizeof buf2, "select count(pig.id_player) czlonkow from fs_players_in_gangs pig where pig.id_gang=%d", gangid);
  sscanf(SQLGetResult(buf2), "D(0)", czlonkow);

  format(buf2, sizeof buf2, "select count(pig.id_player) czlonkow from fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player AND DATEDIFF(NOW(),p.datetime_last)<=1 where pig.id_gang=%d", gangid);

  sscanf(SQLGetResult(buf2), "D(0)", czlonkow_24h);

  format(buf2, sizeof buf2, "select count(pig.id_player) czlonkow from fs_players_in_gangs pig JOIN fs_players p ON p.id=pig.id_player AND DATEDIFF(NOW(),p.datetime_last)<=7 where pig.id_gang=%d", gangid);
  sscanf(SQLGetResult(buf2), "D(0)", czlonkow_7d);

  format(buf2, sizeof buf2, "\n\nRespekt:\t %d\nCzlonkow:\t %d (online w ciagu 24h: %d, w ciagu 7d: %d)",
        gData[gangid][respect],
        czlonkow, czlonkow_24h, czlonkow_7d
    );
  strcat(buffer, buf2);

  new
   szTitle[32];

  format(szTitle, sizeof szTitle, TXT(playerid, 239), gData[gangid][tag]);

  return  ShowPlayerDialog(playerid, DIALOG_STATS_GANG_INFO, DIALOG_STYLE_MSGBOX, szTitle, buffer, __("Wyjdz"), __("Wroc"));
}

gangs_LoadGangData(){
  mysql_query("DELETE FROM fs_players_in_gangs WHERE id_gang NOT IN (SELECT id FROM fs_gangs);");
  mysql_query("DELETE FROM fs_gangs_alliance WHERE g1 NOT IN (SELECT id FROM fs_gangs) OR g2 NOT IN (SELECT id FROM fs_gangs);");

  new buffer[512];
  new wyspa[63];
  new Float:gangWyspa[4];
  for(new i = 1; i < MAX_GANGS; i++)
  {
    for(new i2 = 1; i2 < MAX_GANGS; i2++)
      gangi_sojusze[i][i2]=false;

    copy("",gData[i][name]);
    copy("",gData[i][tag]);
    gData[i][gColor]=0xFFFFFF;
    gData[i][respect]=0;
  }
  format(buffer,sizeof buffer,"SELECT id,name,tag,color,respect,spawnpoint,basespawn,www,IFNULL(wyspa_sphere,'-') FROM fs_gangs WHERE active=1 AND id>0 AND id<%d", MAX_GANGS);

  mysql_query(buffer);
  mysql_store_result();
  while(mysql_fetch_row_format(buffer,"|")) {
    new id=0; // tak ktos juz wymyslil przede mna, 0 nie jest w tej tablicy uzywane
    // najpierw odnajdzmy id tego gangu!
    if (sscanf(buffer,"p<|>d", id) || id<=0 || id>MAX_GANGS)
      continue;

    new tmp_basespawn[65];
    new tmp_spawnpoint[65];
    new tmp_color[7];

    sscanf(buffer,"p<|>ds[50]s[10]s[10]ds[70]s[70]s[100]s[63]",
        id, gData[id][name], gData[id][tag], tmp_color, gData[id][respect], tmp_spawnpoint,tmp_basespawn, gData[id][www],wyspa);

    gData[id][gColor] = HexToInt(tmp_color);

    sscanf(tmp_spawnpoint,"p<,>ffff",gData[id][gSpawnPoint][0], gData[id][gSpawnPoint][1], gData[id][gSpawnPoint][2], gData[id][gSpawnPoint][3]);

    gangWyspa[0]=FLOAT_NAN;
    gangWyspa[1]=FLOAT_NAN;
    gangWyspa[2]=FLOAT_NAN;
    gangWyspa[3]=FLOAT_NAN;
    sscanf(wyspa,"p<,>ffff", gangWyspa[0],gangWyspa[1],gangWyspa[2],gangWyspa[3]);
    sscanf(tmp_basespawn,"p<,>ffff", gData[id][gBaseSpawn][0],gData[id][gBaseSpawn][1],gData[id][gBaseSpawn][2],gData[id][gBaseSpawn][3]);

    DestroyDynamicArea(gData[id][wyspaId]);

    gData[id][wyspaId]=CreateDynamicSphere(gangWyspa[0],gangWyspa[1],gangWyspa[2],gangWyspa[3],-1,0,-1);
  }
  mysql_free_result();

  mysql_query("SELECT g1,g2 FROM fs_gangs_alliance");
  mysql_store_result();
  new g1,g2;
  while (mysql_fetch_row_format(buffer,"|")) {
    if (sscanf(buffer,"p<|>dd", g1,g2) || g1<1  || g2<1 || g1>MAX_GANGS || g2>MAX_GANGS)
      continue;
    gangi_sojusze[g1][g2]=true;
    gangi_sojusze[g2][g1]=true;
  }
  mysql_free_result();
}

gang_OPEnterDynamicArea(playerid,areaid)
{
  new gangTmp=0;
  new buf128[128];
  for(new i;i<MAX_GANGS;i++){
    if(areaid==gData[i][wyspaId]) {
      gangTmp=i;
    }
  }

  if(gangTmp==0) return 0;

  if(pData[playerid][gang]==gangTmp) GameTextForPlayer(playerid, "~g~przebywasz na wyspie swojego gangu", 5000, 3);

  if(pData[playerid][gang]!=gangTmp && !gangi_sojusze[pData[playerid][gang]][gangTmp]) {
      format(buf128,128,"~r~to jest wyspa gangu~n~~h~ %s",gData[gangTmp][name]);
    GameTextForPlayer(playerid, buf128, 5000, 3);
  }

  if(pData[playerid][gang]!=gangTmp && gangi_sojusze[pData[playerid][gang]][gangTmp]) {
      format(buf128,128,"~g~to jest wyspa gangu~n~~h~ %s ~n~~n~~g~masz sojusz z tym gangiem",gData[gangTmp][name]);
    GameTextForPlayer(playerid, buf128, 5000, 3);
  }
  return 1;
}

// commands

CMD:g(playerid, params[])
{
  if(!pData[playerid][loggedIn] || pData[playerid][gang] == NO_GANG) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 135)); // U¿ywaj: /g <tekst>

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);
  OutputGangChat(playerid, params);
  return 1;
}

CMD:gwrite(playerid, params[])
{
    if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;

  new msg[127],gidx;
  if (sscanf(params,"ds[127]",gidx,msg) || gidx==0) return Msg(playerid, COLOR_ERROR,"Uzywaj: /gwrtite <id gangu> <tekst>"); // U¿ywaj: /g <tekst

  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(msg);
  OutputGangChatId(playerid, gidx, msg);
  return 1;
}

CMD:s(playerid, params[])
{
  if(!pData[playerid][loggedIn] || pData[playerid][gang] == NO_GANG) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzyj: /s <tekst>"); // U¿ywaj: /g <tekst>

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);
  OutputAllyChat(playerid, params);
  return 1;
}

CMD:ustawbazegangu(playerid,params[]){
  if (! GANGMGMT ) return 0;
  new gidx;
  sscanf(params,"d",gidx);
  if (gidx<1 || gidx>MAX_GANGS) return Msg(playerid,COLOR_ERROR,"Niewlasciwe ID gangu");
  if (strlen(gData[gidx][tag])<=0) return Msg(playerid,COLOR_ERROR,"Nie odnaleziono podanego ID gangu");
  if (GetPlayerInterior(playerid)>0 || GetPlayerVirtualWorld(playerid)>0) return Msg(playerid,COLOR_ERROR,"Baza/spawn moze byc tylko na zewnatrz.");
  new Float:P[4];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  GetPlayerFacingAngle(playerid,P[3]);
  format(gstr,sizeof gstr,"UPDATE fs_gangs SET basespawn='%.1f,%.1f,%.1f,%.1f' WHERE id=%d LIMIT 1", P[0],P[1],P[2],P[3], gidx);
  mysql_query(gstr);
  gData[gidx][gBaseSpawn][0]=P[0];
  gData[gidx][gBaseSpawn][1]=P[1];
  gData[gidx][gBaseSpawn][2]=P[2];
  gData[gidx][gBaseSpawn][3]=P[3];
  Msg(playerid,COLOR_INFO,"Spawn gangu ustawiony");
  return 1;
}

CMD:gangprzyjmij(playerid,params[]){
  if (!gangs_isPlayerGangOwnerOrLeader(playerid))
    return Msg(playerid, COLOR_ERROR, __("Funkcja tylko dla liderow/wlascicieli gangow"));

  new userid,tplayer[24];
  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    userid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid))  return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/gangprzyjmij [id/nick]");

  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,__("Nie ma takiego gracza!"));
  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Gracz nie jest zarejestrowany!"));
  if (pData[userid][gang]==pData[playerid][gang])
    return Msg(playerid,COLOR_ERROR,__("Gracz jest juz czlonkiem Twojego gangu"));
  if (pData[userid][gang]!=NO_GANG)
    return Msg(playerid,COLOR_ERROR,__("Gracz nalezy juz do innego gangu"));
    if (pData[userid][respect]<2000)
    return Msg(playerid,COLOR_ERROR,__("Gracz nie ma wymaganej ilosci {b}2000 respektu{/b}"));

  pData[userid][gang]=pData[playerid][gang];
  SetPlayerProperColor(userid,true);
  new buf[128];
  format(buf,sizeof buf,"INSERT INTO fs_players_in_gangs SET id_gang=%d,id_player=%d,rank='member'", pData[playerid][gang], pData[userid][accountID]);
  mysql_query(buf);
  format(buf, sizeof buf, __("_~n~_~n~_~n~~w~Witaj w~n~~b~%s"), gData[pData[playerid][gang]][name]);
  GameTextForPlayer(userid,buf,5000,6);
  PlayRandomMusic(userid,5000);

  if (SkinKobiecy(GetPlayerSkin(userid)))
    format(buf,sizeof buf,__("%s (%d) dolaczyla do gangu %s"), GetPlayerProperName(userid), userid, gData[pData[playerid][gang]][name]);
  else
    format(buf,sizeof buf,__("%s (%d) dolaczyl do gangu %s"), GetPlayerProperName(userid), userid, gData[pData[playerid][gang]][name]);

  gangs_MSG(pData[playerid][gang], buf);

  UpdatePlayerFactionName(userid);

  return 1;
}

CMD:gangwyrzuc(playerid,params[]){
  if (!gangs_isPlayerGangOwnerOrLeader(playerid))
    return Msg(playerid, COLOR_ERROR, __("Funkcja tylko dla liderow/wlascicieli gangow"));

  new userid,tplayer[24];
  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    userid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) {
      return 1;
    }
  } else
    return Msg(playerid,COLOR_ERROR,"/gangwyrzuc [id/nick]");

  if(playerid==userid)
      return Msg(playerid,COLOR_ERROR,__("Nie mozesz wyrzucic samego siebie!"));
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,__("Nie ma takiego gracza!"));
  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Gracz nie jest zarejestrowany!"));
  if (pData[userid][gang]==NO_GANG)
    return Msg(playerid,COLOR_ERROR,__("Gracz nie nalezy do zadnego klanu"));
  if (pData[userid][gang]!=pData[playerid][gang])
    return Msg(playerid,COLOR_ERROR,__("Gracz nie jest czlonkiem!"));
  if (pData[userid][gangRank]==GANG_RANK_OWNER)
    return Msg(playerid,COLOR_ERROR,__("Nie mozna wyrzucic zalozyciela!"));
    if (pData[userid][gangRank]==GANG_RANK_VICEOWNER)
    return Msg(playerid,COLOR_ERROR,__("Nie mozna wyrzucic wspolzalozyciela!"));

  gangs_RemovePlayerFromGang(userid);
  return 1;
}

CMD:reloadgangs(playerid){
  if (!IsPlayerAdmin(playerid)) return 0;
  SendClientMessageToAll(-1,"Przeladowywanie danych gangow...");
  gangs_saveGangData(true);
  gangs_LoadGangData();
  return 1;
}

CMD:gang(playerid){
  if (pData[playerid][gang]==NO_GANG)
    return gangs_ShowGangsList(playerid);

  new lista[1024];

    strcat(lista,"{000000}C {ffffff}Ustaw sobie kolor gangu\n");
  strcat(lista,"{000000}W {ffffff}Wyspa gangu\n");
  strcat(lista,"{000000}S {d0d0d0}Spawn gangu\n");
                new sojusznikow=0;
        for(new i=1;i<MAX_GANGS;i++)
        if (gangi_sojusze[pData[playerid][gang]][i]) {
          sojusznikow++;
          format(lista,sizeof lista, "%s{000000}W%d {d0d0d0}Wyspa sojusznika {%06x}%s\n", lista, sojusznikow,gData[i][gColor],gData[i][tag]);
        }
          sojusznikow=0;
        for(new i=1;i<MAX_GANGS;i++)
        if (gangi_sojusze[pData[playerid][gang]][i]) {
          sojusznikow++;
          format(lista,sizeof lista, "%s{000000}S%d {d0d0d0}Spawn sojusznika {%06x}%s\n", lista, sojusznikow,gData[i][gColor],gData[i][tag]);
        }

  if (pData[playerid][gangRank]==GANG_RANK_LEADER || pData[playerid][gangRank]==GANG_RANK_OWNER || pData[playerid][gangRank]==GANG_RANK_VICEOWNER) {
    strcat(lista,"{000000}U {ffffff}Ustaw spawn\n");
  }
  if (pData[playerid][gangRank]!=GANG_RANK_OWNER && pData[playerid][gangRank]!=GANG_RANK_VICEOWNER) {
    strcat(lista,"{000000}E {ffffff}Opusc ");
    format(lista,sizeof lista, "%s{%06x}%s\n", lista, gData[pData[playerid][gang]][gColor], gData[pData[playerid][gang]][tag]);
  }

  strcat(lista,"{000000}L {ffffff}Lista gangow na serwerze");
  return ShowPlayerDialog(playerid, DIALOG_GANG_MAINMENU, DIALOG_STYLE_LIST, "Gang", lista, __("Wybierz"), __("Wroc"));
}
// EOF
