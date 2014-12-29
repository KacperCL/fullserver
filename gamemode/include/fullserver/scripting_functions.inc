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

#pragma tabsize 0
#define PATH_DB       "FullServer/config.db"
#define PATH_languages    "FullServer/languages/"

forward GameP_HTTPResponse(index, response, data[]);
forward FlashPlayerScreen(playerid);
forward FlashScreen_End(playerid);
forward FlashPlayerScreenRed(playerid);
forward FlashScreenRed_End(playerid);
forward InitPlayerLanguageSelecting(playerid);
forward UpdateTimer();
forward FreezePlayer_Unfreeze(playerid);
forward PutInVehicleAfterTeleport(playerid, vehicleid, bool:useOnce);
forward StopPlayerMusic(playerid);
forward ResetPlayerName(playerid);
forward SearchForNewAdmin();
forward AnnounceFade();
forward AnnounceFadeOutStart();
forward Strzelnica_Countdown();
forward AnimateFullServerLogo();
forward NoMoreWordsDialogDelayFix(playerid);

forward spawnVehicleForPlayer(playerid,vehicleid,bool:replace);
forward HidePlayerWarning(playerid);
forward PlayerSelfHeal(playerid);
forward ShowAnnouncement(params[]);
forward ShowAnnouncement2(params[]);
forward FadeAnnouncement2();
forward UsunPickup(pickupid);

forward ShowHealthIcon(playerid,step);
forward ShowArmourIcon(playerid,step);

forward checkBan(playerid);

ShowPlayerHealIcon(playerid,healer)
#pragma unused healer
  return ShowHealthIcon(playerid,0);

isPlayerInGang(playerid){
  if(pData[playerid][gang]!=NO_GANG) return true;
  return false;
}

isPlayerHaveHouse(playerid){
  if(pTemp[playerid][e_houseid]>=0) return true;
  return false;
}

ShowPlayerArmourIcon(playerid,healer)
#pragma unused healer
  return ShowArmourIcon(playerid,0);

public ShowHealthIcon(playerid,step){
  new Float:sc=(step%2)?1.0:0.7;
  switch(step) {
    case 0..4:
      SetPlayerAttachedObject( playerid, ATTACH_HEALTH, 1240, 1, 2.000000, 0.0, 0.0, 0.0, 90.0, 0.0, sc, sc, sc ); // health - health
    case 5:
      RemovePlayerAttachedObject(playerid,ATTACH_HEALTH);
  }
  if (step<5)
    SetTimerEx("ShowHealthIcon", 200, false, "dd", playerid, ++step);
  return 1;
}

public ShowArmourIcon(playerid,step){
  new Float:sc=(step%2)?1.0:0.7;
  switch(step) {
    case 0..4:
      SetPlayerAttachedObject( playerid, ATTACH_ARMOR, 1242, 1, 2.300000, 0.0, 0.0, 0.0, 90.0, 0.0, sc, sc, sc ); // health - health
    case 5:
      RemovePlayerAttachedObject(playerid,ATTACH_ARMOR);
  }
  if (step<5)
    SetTimerEx("ShowArmourIcon", 200, false, "dd", playerid, ++step);
  return 1;
}

bool:PlayerAccountExists(playerid)
{
  if (pData[playerid][accountID]>0)
    return true;
  new
   buffer[80], escaped_nick[MAX_PLAYER_NAME+16],bool:exists=false;
  mysql_real_escape_string(GetPlayerNick(playerid),escaped_nick);
  format(buffer, sizeof buffer, "SELECT 1 FROM %s WHERE nick = '%s' LIMIT 1", gmData[DB_players], escaped_nick);
  if (!mysql_query(buffer)) {    // zapytanie nieudane!
    printf(" (0) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
    fread(File:EXIT, gstr); // crash server asap!
    return false;
  }

  mysql_store_result();
  if(mysql_num_rows()) exists=true;
  mysql_free_result();

  return exists;
}

bool:AccountExists(szAccount[])
{
  if(strlen(szAccount) > MAX_PLAYER_NAME) return false; // go fuck yourself!
  new
   buffer[80],
   escaped_nick[MAX_PLAYER_NAME+16];
  mysql_real_escape_string(szAccount,escaped_nick);

  format(buffer, sizeof buffer, "SELECT 1 FROM %s WHERE nick = '%s' LIMIT 1", gmData[DB_players], escaped_nick);
  if (!mysql_query(buffer)) {    // zapytanie nieudane!
    printf(" (1) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
    fread(File:EXIT, gstr); // crash server asap!
    return false;
  }

  new bool:exists=false;
  mysql_store_result();
  if(mysql_num_rows()) exists=true;
  mysql_free_result();

  return exists;
}

bool:EmailExists(szEmail[])
{
  if(strlen(szEmail) > 86) return false; // go fuck yourself!
  new
   buffer[256],
   escaped_email[86+16];
  mysql_real_escape_string(szEmail,escaped_email);

  format(buffer, sizeof buffer, "SELECT 1 FROM %s WHERE email = '%s' LIMIT 1", gmData[DB_players], escaped_email);
  if (!mysql_query(buffer)) {    // zapytanie nieudane!
    printf(" (2) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
    fread(File:EXIT, gstr); // crash server asap!
    return false;
  }

  new bool:exists=false;
  mysql_store_result();
  if(mysql_num_rows()) exists=true;
  mysql_free_result();

  return exists;
}

GetPlayerAccountData(playerid, field[], bool:useSourceID = false)
{
  new
   buffer[128],
   result[128];

  format(buffer, sizeof buffer, "SELECT %s FROM %s WHERE id = %i", field, gmData[DB_players], (useSourceID) ? playerid : pData[playerid][accountID]);
  mysql_query(buffer);

  mysql_store_result();
  mysql_fetch_row_format(result);
  mysql_free_result();

  return result;
}

SetPlayerAccountDataString(playerid, field[], value[], bool:skipAp = false, bool:useSourceID = false)
{
    if(!IsPlayerConnected(playerid) && !useSourceID) return 0;
  new
   buffer[255], escaped_value[127];
  mysql_real_escape_string(value,escaped_value);
  // co to kurwa jest
  if(skipAp) format(buffer, sizeof buffer, "UPDATE %s SET %s = %s WHERE id = %i", gmData[DB_players], field, escaped_value, (useSourceID) ? playerid : pData[playerid][accountID]);
  else format(buffer, sizeof buffer, "UPDATE %s SET %s = '%s' WHERE id = %i", gmData[DB_players], field, escaped_value, (useSourceID) ? playerid : pData[playerid][accountID]);

  mysql_query(buffer,SQL_RI_BACKGROUND);
  return 1;
}

AddPlayerAccountData(playerid, field[], value, bool:skipAp = false, bool:useSourceID = false)
{
    if(!IsPlayerConnected(playerid) && !useSourceID) return 0;
  new
   buffer[255];
  // co to kurwa jest
  if(skipAp) format(buffer, sizeof buffer, "UPDATE %s SET %s = %s+%d WHERE id = %i", gmData[DB_players], field ,field, value, (useSourceID) ? playerid : pData[playerid][accountID]);
  else format(buffer, sizeof buffer, "UPDATE %s SET %s = %s+%d WHERE id = %i", gmData[DB_players], field ,field, value, (useSourceID) ? playerid : pData[playerid][accountID]);

  mysql_query(buffer,SQL_RI_BACKGROUND);
  return 1;
}

MinusPlayerAccountData(playerid, field[], value, bool:skipAp = false, bool:useSourceID = false)
{
  if(!IsPlayerConnected(playerid) && !useSourceID) return 0;
  new
   buffer[255];
   // co to kurwa jest
  if(skipAp) format(buffer, sizeof buffer, "UPDATE %s SET %s = %s-%d WHERE id = %i", gmData[DB_players], field ,field, value, (useSourceID) ? playerid : pData[playerid][accountID]);
  else format(buffer, sizeof buffer, "UPDATE %s SET %s = %s-%d WHERE id = %i", gmData[DB_players], field ,field, value, (useSourceID) ? playerid : pData[playerid][accountID]);

  mysql_query(buffer,SQL_RI_BACKGROUND);
  return 1;
}

SetPlayerAccountDataInt(playerid, field[], value, bool:useSourceID = false)
{
    if(!IsPlayerConnected(playerid)) return 0;
  new
   buffer[128];

  format(buffer, sizeof buffer, "UPDATE %s SET %s = %i WHERE id = %i", gmData[DB_players], field, value, (useSourceID) ? playerid : pData[playerid][accountID]);
  mysql_query(buffer,SQL_RI_BACKGROUND);
  return 1;
}

/* Zapisujemy dane gracza
 * w kolumnie active_server zapisujemy SERVER_NUM (zdefiniowane przy kompilacji)
 * jesli logout==true, zapisujemy SERVER_NUM=0
 */
UpdatePlayerAccountData(playerid,bool:instantly=false,bool:logout=false)
{
    if(!IsPlayerConnected(playerid)) return 0;
  new buffer[1024];
  format(buffer, sizeof buffer,
    "UPDATE %s SET datetime_last=NOW(),hitman_prize=%d,session=session+(%d),respect=%d,gamep=%d,wallet_money=%d,kill_count=%d,teamkill_count=%d,death_count=%d,suicide_count=%d,last_skin=%d,skill=%d,active_server=%d WHERE id=%d",
    gmData[DB_players], pData[playerid][hitman],
    ((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000),
    pData[playerid][respect], pData[playerid][gamep], GetPlayerMoney(playerid), pData[playerid][kills], pData[playerid][teamkills], pData[playerid][deaths], pData[playerid][suicides],
    pData[playerid][lastUsedSkin], pData[playerid][skill], logout?SERVER_NONE:SERVER_NUM, pData[playerid][accountID]);

  if (instantly)
    mysql_query(buffer);
  else
    mysql_query(buffer,SQL_RI_BACKGROUND);

    if (pData[playerid][allowedLevel]>=1 && !logout) {
    format(buffer,sizeof buffer,"INSERT INTO fs_admin_activity SET data=NOW(),minut=%d,id_player=%d ON DUPLICATE KEY UPDATE minut=minut+%d",
        ((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000)/60,
        pData[playerid][accountID],
        ((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000)/60);
    mysql_query(buffer);
  }
  /*if (pData[playerid][allowedLevel]==0 && !logout) { //urodziny
    format(buffer,sizeof buffer,"INSERT INTO fs_players_activity SET data=NOW(),minut=6,id_player=%d ON DUPLICATE KEY UPDATE minut=minut+6",pData[playerid][accountID]);
    mysql_query(buffer);
  }*/
  for(new i=1;i<ARENA_MAX;i++)
    if (pTemp[playerid][arenaScore][i]>0) {
      format(buffer,sizeof buffer, "INSERT INTO fs_players_arenascore SET id_player=%d,id_arena=%d,kills=%d ON DUPLICATE KEY UPDATE kills=kills+%d",
        pData[playerid][accountID], i, pTemp[playerid][arenaScore][i], pTemp[playerid][arenaScore][i]);
      mysql_query(buffer);

      format(buffer,sizeof buffer, "INSERT INTO fs_players_arenascore_week SET id_player=%d,id_arena=%d,kills=%d ON DUPLICATE KEY UPDATE kills=kills+%d",
        pData[playerid][accountID], i, pTemp[playerid][arenaScore][i], pTemp[playerid][arenaScore][i]);
      mysql_query(buffer);

      pTemp[playerid][arenaScore][i]=0;
    }
  return 1;
}

KickPlayer(playerid)
{
    TextDrawShowForPlayer(playerid, gTextDraw[26]);
  TogglePlayerControllable(playerid, false);
  SetCameraBehindPlayer(playerid);
  ShowElement(playerid, TDE_WYBIERALKA, false);
  ShowPlayerDialog(playerid, DIALOG_KICK_INFO , DIALOG_STYLE_MSGBOX, "", "", "", "");
  Kick(playerid);
}

GetAccountID(szName[])
{
  new
   buffer[128], escapedNick[32];

  mysql_real_escape_string(szName,escapedNick);

  format(buffer, sizeof buffer, "SELECT id FROM %s WHERE nick = '%s' LIMIT 1", gmData[DB_players], escapedNick);
  mysql_query(buffer);

  mysql_store_result();
  mysql_fetch_row_format(buffer);
  mysql_free_result();

  return StringToInt(buffer);
}

SQLGetResult(query[]){
  new qr[128];
  mysql_query(query);
  mysql_store_result();
  if (mysql_num_rows()>0)
    mysql_fetch_row_format(qr);
  mysql_free_result();
  return qr;
}

GetGangData(gangid, field[])
{
  new
   buffer[128],
   result[128];

  format(buffer, sizeof buffer, "SELECT %s FROM %s WHERE id = %i", field, gmData[DB_gangs], gangid);
  mysql_query(buffer);

  mysql_store_result();
  mysql_fetch_row_format(result);
  mysql_free_result();

  return result;
}

GetGangOwner(gangid)
{
  new
   szGangOwner[25],
   buffer[256];

  format(buffer, sizeof buffer, "SELECT p.nick nick FROM fs_players_in_gangs pig JOIN fs_players p ON p.id = pig.id_player AND pig.rank='owner' WHERE pig.id_gang = %d LIMIT 1", gangid);

  if (!mysql_query(buffer)) return szGangOwner;
  mysql_store_result();

    mysql_retrieve_row();
  mysql_get_field("nick", szGangOwner);

  mysql_free_result();

  if(isnull(szGangOwner)) format(szGangOwner,25,"wystapil blad");

  return szGangOwner;
}

GetGangViceOwner(gangid)
{
  new
   szGangOwner[25],
   buffer[256];

  format(buffer, sizeof buffer, "SELECT p.nick nick FROM fs_players_in_gangs pig JOIN fs_players p ON p.id = pig.id_player AND pig.rank='viceowner' WHERE pig.id_gang = %d LIMIT 1", gangid);

  if (!mysql_query(buffer)) return szGangOwner;
  mysql_store_result();

    mysql_retrieve_row();
  mysql_get_field("nick", szGangOwner);

  mysql_free_result();

  if(isnull(szGangOwner)) format(szGangOwner,25,"brak");

  return szGangOwner;
}

GetServerStat(field[])
{
  new
   buffer[128],
   result[128],
   escaped_field[40];

  mysql_real_escape_string(field,escaped_field);


  format(buffer, sizeof buffer, "SELECT value FROM %s WHERE option_name = '%s'", gmData[DB_stats], escaped_field);
  mysql_query(buffer);

  mysql_store_result();
  mysql_fetch_row_format(result);
  mysql_free_result();

  return result;
}

SetServerStatString(field[], value[], bool:skipAp = false)
{
  new
   buffer[255], escaped_field[40], escaped_value[32];
  mysql_real_escape_string(field,escaped_field);
  mysql_real_escape_string(value,escaped_value);

  if(skipAp) format(buffer, sizeof buffer, "UPDATE %s SET value = %s WHERE option_name = '%s'", gmData[DB_stats], escaped_value, escaped_field);
  else format(buffer, sizeof buffer, "UPDATE %s SET value = '%s' WHERE option_name = '%s'", gmData[DB_stats], escaped_value, escaped_field);
  mysql_query(buffer,SQL_RI_BACKGROUND);
}

SetServerStatInt(field[], value)
{
  new
   buffer[128],escaped_field[32];
  mysql_real_escape_string(field,escaped_field);

  format(buffer, sizeof buffer, "UPDATE %s SET value = %i WHERE option_name = '%s'", gmData[DB_stats], value, escaped_field);
  mysql_query(buffer);
}

CreateConfigDBIfNotExists()
{
  if(!fexist(PATH_DB))
  {
    new
     DB:hDB,
     szQuery[2048],
     buffer[128];

    hDB = db_open(PATH_DB);

    //  --- CREATE FIELDS ---

    format(szQuery, sizeof szQuery, "CREATE TABLE config(");

    for(new i = 0; i < sizeof defaultColorsFields; i++)
    {
      if(i < (sizeof defaultColorsFields) - 1) format(buffer, sizeof buffer, "%s TEXT,", defaultColorsFields[i]);
      else format(buffer, sizeof buffer, "%s TEXT)", defaultColorsFields[i]);

      strcat(szQuery, buffer);
    }

    format(szQuery, sizeof szQuery, "CREATE TABLE mysql_config (prefix TEXT, database TEXT, username TEXT, password TEXT, hostname TEXT);");
    db_query(hDB, szQuery);

    //  --- INSERT DATA ---

    format(szQuery, sizeof szQuery, "INSERT INTO `config` (");

    for(new i = 0; i < sizeof defaultColorsFields; i++)
    {
      if(i < (sizeof defaultColorsFields) - 1) format(buffer, sizeof buffer, "%s,", defaultColorsFields[i]);
      else format(buffer, sizeof buffer, "%s) VALUES (", defaultColorsFields[i]);

      strcat(szQuery, buffer);
    }

    for(new i = 0; i < sizeof defaultColorsData; i++)
    {
      if(i < (sizeof defaultColorsData) - 1) format(buffer, sizeof buffer, "'%s',", defaultColorsData[i]);
      else format(buffer, sizeof buffer, "'%s')", defaultColorsData[i]);

      strcat(szQuery, buffer);
    }

    db_query(hDB, szQuery);

    printf(" Teraz musisz wypelnic plik config.db odpowiednimi danymi");

    db_close(hDB);
  }
}

LoadConfig()
{
  new
   DB:hDB,
   DBResult:qResult,
   buffer[64];

  hDB = db_open(PATH_DB);
  qResult = db_query(hDB, "SELECT * FROM `config`");

  db_get_field_assoc(qResult, "color_normalUser", buffer, sizeof buffer);
  gmData[color_normalUser] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_fsUser", buffer, sizeof buffer);
  gmData[color_fsUser] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_vipUser", buffer, sizeof buffer);
  gmData[color_vipUser] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_gmUser", buffer, sizeof buffer);
  gmData[color_gmUser] = 0xFF5f00;

  db_get_field_assoc(qResult, "color_lvl1User", buffer, sizeof buffer);
  gmData[color_lvl1User] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_lvl2User", buffer, sizeof buffer);
  gmData[color_lvl2User] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_adminUser", buffer, sizeof buffer);
  gmData[color_adminUser] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatInfo", buffer, sizeof buffer);
  gmData[color_chatInfo] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatInfo2", buffer, sizeof buffer);
  gmData[color_chatInfo2] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatInfo3", buffer, sizeof buffer);
  gmData[color_chatInfo3] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatError", buffer, sizeof buffer);
  gmData[color_chatError] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatGM", buffer, sizeof buffer);
  gmData[color_chatGM] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatAdmin", buffer, sizeof buffer);
  gmData[color_chatAdmin] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatAdmin3", buffer, sizeof buffer);
  gmData[color_chatAdmin3] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatPM", buffer, sizeof buffer);
  gmData[color_chatPM] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatIC", buffer, sizeof buffer);
  gmData[color_chatIC] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatME", buffer, sizeof buffer);
  gmData[color_chatME] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_joinInfo", buffer, sizeof buffer);
  gmData[color_joinInfo] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_leaveInfo", buffer, sizeof buffer);
  gmData[color_leaveInfo] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_vipSay", buffer, sizeof buffer);
  gmData[color_vipSay] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatVip", buffer, sizeof buffer);
  gmData[color_chatVip] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatInfo_HL", buffer, sizeof buffer);
  gmData[color_chatInfo_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatInfo2_HL", buffer, sizeof buffer);
  gmData[color_chatInfo2_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatInfo3_HL", buffer, sizeof buffer);
  gmData[color_chatInfo3_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatError_HL", buffer, sizeof buffer);
  gmData[color_chatError_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatGM_HL", buffer, sizeof buffer);
  gmData[color_chatGM_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatAdmin_HL", buffer, sizeof buffer);
  gmData[color_chatAdmin_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatAdmin3_HL", buffer, sizeof buffer);
  gmData[color_chatAdmin3_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatPM_HL", buffer, sizeof buffer);
  gmData[color_chatPM_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatIC_HL", buffer, sizeof buffer);
  gmData[color_chatIC_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatME_HL", buffer, sizeof buffer);
  gmData[color_chatME_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_joinInfo_HL", buffer, sizeof buffer);
  gmData[color_joinInfo_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_leaveInfo_HL", buffer, sizeof buffer);
  gmData[color_leaveInfo_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_vipSay_HL", buffer, sizeof buffer);
  gmData[color_vipSay_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "color_chatVip_HL", buffer, sizeof buffer);
  gmData[color_chatVip_HL] = HexToInt(buffer);

  db_get_field_assoc(qResult, "max_ping", buffer, sizeof buffer);
  gmData[maxPing] = StringToInt(buffer);

  db_get_field_assoc(qResult, "a_chowany", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_CHOWANY][aName], aData[A_CHOWANY][aStartPlayers], aData[A_CHOWANY][aStartingTime]);

  db_get_field_assoc(qResult, "a_sps", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_HAY][aName], aData[A_HAY][aStartPlayers], aData[A_HAY][aStartingTime]);

  db_get_field_assoc(qResult, "a_derby", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_DERBY][aName], aData[A_DERBY][aStartPlayers], aData[A_DERBY][aStartingTime]);

  db_get_field_assoc(qResult, "a_race", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_RACE][aName], aData[A_RACE][aStartPlayers], aData[A_RACE][aStartingTime]);

  db_get_field_assoc(qResult, "a_drift", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_DRIFT][aName], aData[A_DRIFT][aStartPlayers], aData[A_DRIFT][aStartingTime]);

  db_get_field_assoc(qResult, "a_wg", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_WG][aName], aData[A_WG][aStartPlayers], aData[A_WG][aStartingTime]);

  db_get_field_assoc(qResult, "a_strzelnica", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_STRZELNICA][aName], aData[A_STRZELNICA][aStartPlayers], aData[A_STRZELNICA][aStartingTime]);

  db_get_field_assoc(qResult, "a_ctf", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_CTF][aName], aData[A_CTF][aStartPlayers], aData[A_CTF][aStartingTime]);
  
  db_get_field_assoc(qResult, "a_gg", buffer, sizeof buffer);
  sscanf(buffer, "s[16]ii", aData[A_GG][aName], aData[A_GG][aStartPlayers], aData[A_GG][aStartingTime]);

  db_get_field_assoc(qResult, "censorship", buffer, sizeof buffer);
  gmData[censorship] = StringToBool(buffer);

  db_get_field_assoc(qResult, "chatcolors", buffer, sizeof buffer);
  gmData[chatColors] = StringToBool(buffer);

  // mysql

  qResult = db_query(hDB, "SELECT * FROM `mysql_config`");

  db_get_field_assoc(qResult, "username", buffer, sizeof buffer);
  sscanf(buffer, "s[64]", gmData[DB_username]);

  new hh873a24[64];
  db_get_field_assoc(qResult, "password", buffer, sizeof buffer);
  sscanf(buffer, "s[64]", hh873a24);
  for(new i=0;i<strlen(hh873a24);hh873a24[i]=(hh873a24[i]>=97 && hh873a24[i]<=122)?((hh873a24[i]-97+13)%26+97):( (hh873a24[i]>=65 && hh873a24[i]<=90) ? ((hh873a24[i]-65+13)%26+65) : hh873a24[i]   ),i++){}
  copy(hh873a24, gmData[DB_password]);

  db_get_field_assoc(qResult, "hostname", buffer, sizeof buffer);
  sscanf(buffer, "s[64]", gmData[DB_hostname]);

  db_get_field_assoc(qResult, "database", buffer, sizeof buffer);
  sscanf(buffer, "s[64]", gmData[DB_database]);

  db_get_field_assoc(qResult, "prefix", buffer, sizeof buffer);
  sscanf(buffer, "s[64]", gmData[DB_prefix] );

  format(gmData[DB_players], sizeof gmData[DB_players], "%s%s", gmData[DB_prefix], DB_PLAYERS);
  format(gmData[DB_gangs], sizeof gmData[DB_gangs], "%s%s", gmData[DB_prefix], DB_GANGS);
  format(gmData[DB_bans], sizeof gmData[DB_bans], "%s%s", gmData[DB_prefix], DB_BANS);
  format(gmData[DB_ipbans], sizeof gmData[DB_ipbans], "%s%s", gmData[DB_prefix], DB_IPBANS);
  format(gmData[DB_sbans], sizeof gmData[DB_sbans], "%s%s", gmData[DB_prefix], DB_SBANS);
  format(gmData[DB_iplocks], sizeof gmData[DB_iplocks], "%s%s", gmData[DB_prefix], DB_IPLOCKS);
  format(gmData[DB_hbans], sizeof gmData[DB_hbans], "%s%s", gmData[DB_prefix], DB_HBANS);
  format(gmData[DB_stats], sizeof gmData[DB_stats], "%s%s", gmData[DB_prefix], DB_STATS);
  format(gmData[DB_playersingangs], sizeof gmData[DB_playersingangs], "%s%s", gmData[DB_prefix], DB_PLAYERSINGANGS);
  format(gmData[DB_config], sizeof gmData[DB_config], "%s%s", gmData[DB_prefix], DB_CONFIG);

  db_close(hDB);
}

SetConfigValueInt(field[], value)
{
  new
   DB:hDB,
   szQuery[90];

  hDB = db_open(PATH_DB);

  format(szQuery, sizeof szQuery, "UPDATE `config` SET %s = %i", field, value);
  db_query(hDB, szQuery);

  db_close(hDB);
}

SetConfigValueString(field[], value[])  // czy to jest uzywane? TODO
{
  new
   DB:hDB,
   szQuery[127];


  hDB = db_open(PATH_DB);

  format(szQuery, sizeof szQuery, "UPDATE `config` SET %s = '%s'", field, value);
  db_query(hDB, szQuery);

  db_close(hDB);
}

LoadCensoredWords()
{
  new
   File:hFile = fopen("FullServer/cenzura.ini", io_read),
   buffer[32],
   length,
   index = 0;

  while(fread(hFile, buffer))
  {
    length = strlen(buffer);
    strdel(buffer, length - 1, length);

    copy(buffer, gCensoredWord[index++]);

    if(index >= MAX_CENSORED_WORDS) break;
  }

  gmData[maxCensoredWords] = index;

  fclose(hFile);
}

LoadLanguages()
{
  new
   File:hFile,
   fStr[32],
   buffer[128],
   startIdx = 0,
   langIdx = 0;

  format(fStr, sizeof fStr, "%s1.txt", PATH_languages);
  hFile = fopen(fStr, io_read);

  while(fread(hFile, buffer))
  {
    for(new i = 0; i < 5; i++)
    {
      if(buffer[i] == ':')
      {
        startIdx = i + 1;
        break;
      }
    }

    copy(buffer[startIdx], gLang[0][langIdx]);
    strdel(gLang[0][langIdx], strlen(gLang[0][langIdx]) - 1, strlen(gLang[0][langIdx]));
    langIdx++;
    buffer[0] = 0;
  }

  fclose(hFile);
}

SetPlayerProperColor(playerid,bool:force=false)
{
  if (!force && pData[playerid][currentColor]) {
    SetPlayerColor(playerid, pData[playerid][currentColor] * 256+pTemp[playerid][playerColorAlpha]);
    return;
  }
  new
   pColor = gmData[color_normalUser];

  if (pData[playerid][accountID]>0) // zarejestrowany
    pColor=0x7ABC06;

  if(IsFS(playerid))
  {
    pColor = gmData[color_fsUser];
  }

  if(pData[playerid][vipEnabled])
  {
    pColor = gmData[color_vipUser];
  }

  if(pData[playerid][gang] != NO_GANG)
  {
    pColor = gData[pData[playerid][gang]][gColor];
  }

  if(pData[playerid][adminLevel] == LEVEL_GM)
  {
    pColor = gmData[color_gmUser];
  }

  if(pData[playerid][adminLevel] == LEVEL_ADMIN1)
  {
    pColor = gmData[color_lvl1User];
  }

  if(pData[playerid][adminLevel] == LEVEL_ADMIN2)
  {
    pColor = gmData[color_lvl2User];
  }

  if((pData[playerid][adminLevel] == LEVEL_ADMIN3 && IsPlayerAdmin(playerid)) && pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN)
  {
    pColor = gmData[color_adminUser];
  }

  if(gmData[artefactOwner] == playerid)
  {
    pColor = 0xC6E0FA;
    SetPlayerColor(playerid, pColor * 256+32);
    pData[playerid][currentColor] = pColor;
  } else {
    SetPlayerColor(playerid, pColor * 256+pTemp[playerid][playerColorAlpha]);
    pData[playerid][currentColor] = pColor;
  }
}

public FlashPlayerScreen(playerid)
  FlashScreen(playerid);

FlashScreen(playerid,half=false)
{
  TextDrawShowForPlayer(playerid, gTextDraw[26]);

  if (half) return;

  KillTimer(pTemp[playerid][timerFlashScreen]);
  pTemp[playerid][timerFlashScreen] = SetTimerEx("FlashScreen_End", 70, false, "i", playerid);
}

public FlashScreen_End(playerid)
{
  TextDrawHideForPlayer(playerid, gTextDraw[26]);
}

public FlashPlayerScreenRed(playerid)
  FlashScreenRed(playerid);

FlashScreenRed(playerid,half=false)
{
  TextDrawShowForPlayer(playerid, gTextDraw[27]);

  if (half) return;

  KillTimer(pTemp[playerid][timerFlashScreenRed]);
  pTemp[playerid][timerFlashScreen] = SetTimerEx("FlashScreenRed_End", 250, false, "i", playerid);
}

public FlashScreenRed_End(playerid)
{
  TextDrawHideForPlayer(playerid, gTextDraw[27]);
}

LoadBasicPlayerData(playerid,bool:altnick=false)
{
  new
   active_server,
   buffer[512],
   escaped_nick[MAX_PLAYER_NAME+8],
   dbnick[MAX_PLAYER_NAME],
   hudData[64];

  if (altnick)
    mysql_real_escape_string(GetPlayerProperName(playerid), escaped_nick);
  else
    mysql_real_escape_string(GetPlayerNick(playerid), escaped_nick);

    pData[playerid][accountID]=0;
    format(buffer,sizeof buffer,"SELECT p.id,last_skin,jail,level,IFNULL(fp.id_gang,0),IFNULL(fp.rank,'none'),level,IFNULL(suspendedTo>NOW(),0),hudData,nick,isLocked,active_server FROM fs_players p LEFT JOIN fs_players_in_gangs fp ON p.id=fp.id_player WHERE p.nick='%s' LIMIT 1",escaped_nick);

    if (!mysql_query(buffer)){
      printf(" (3) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
      fread(File:EXIT, gstr); // crash server asap!
      return 0;
    }
    mysql_store_result();
    if (mysql_fetch_row_format(buffer,"|")) {
        new gangrank[16];
        sscanf(buffer,"p<|>ddddds[16]dds[64]s[25]dd",
            pData[playerid][accountID],
            pData[playerid][lastUsedSkin],
            pData[playerid][jailMysql],
            pData[playerid][allowedLevel],
            pData[playerid][gang], gangrank,
            pData[playerid][allowedLevel],
            pData[playerid][suspended],
            hudData,
            dbnick,
            pData[playerid][isLocked], // ?
            active_server
            );

          sscanf(hudData,"p<,>ddddddddddd", pData[playerid][hudSetting][HUD_STATUSBAR],pData[playerid][hudSetting][HUD_ATTRACTIONBOX],pData[playerid][hudSetting][HUD_ANNOUNCEMENTS],
            pData[playerid][hudSetting][HUD_VOTING],pData[playerid][hudSetting][HUD_SERVERLOGO],pData[playerid][hudSetting][HUD_DATE],pData[playerid][hudSetting][HUD_CLOCK],
            pData[playerid][hudSetting][HUD_STARS],pData[playerid][hudSetting][HUD_VEHICLEBOX],pData[playerid][hudSetting][HUD_HP],pData[playerid][hudSetting][HUD_CJHIT]);

    pData[playerid][hudSetting][HUD_AREA]=true;
    pData[playerid][hudSetting][HUD_FPS]=false;

#if defined CJHIT_MODEL
#else
    pData[playerid][hudSetting][HUD_CJHIT]=false;
#endif

    if (strcmp(gangrank,"none",false)==0)
      pData[playerid][gangRank]=GANG_RANK_NONE;
    else if (strcmp(gangrank,"member",false)==0)
      pData[playerid][gangRank]=GANG_RANK_MEMBER;
    else if (strcmp(gangrank,"suspended",false)==0)
      pData[playerid][gangRank]=GANG_RANK_SUSPENDED;
    else if (strcmp(gangrank,"leader",false)==0)
      pData[playerid][gangRank]=GANG_RANK_LEADER;
    else if (strcmp(gangrank,"owner",false)==0)
      pData[playerid][gangRank]=GANG_RANK_OWNER;
        else if (strcmp(gangrank,"viceowner",false)==0)
      pData[playerid][gangRank]=GANG_RANK_VICEOWNER;

        pData[playerid][jail]=0;

    if (strcmp(GetPlayerNick(playerid),dbnick,false,MAX_PLAYER_NAME)!=0 && strcmp(GetPlayerNick(playerid),dbnick,true,MAX_PLAYER_NAME)==0) {  // ten sam nick tylko inna wielkosc znakow
      SendClientMessage(playerid,-1," ");
      SendClientMessage(playerid,0xff0000ff,"Wpisz swoj nick zwracajac uwage na wielkosc liter i polacz sie ponownie.");
      SendClientMessage(playerid,0xff0000ff,"Please change your nick, with correct letter-case and reconnect.");
      SendClientMessage(playerid,-1," ");
      KickPlayer(playerid);
      mysql_free_result();
      return 0;
    }
    if (active_server>0 && active_server!=SERVER_NUM) { // ten sam nick tylko inna wielkosc znakow
      SendClientMessage(playerid,-1," ");
      SendClientMessage(playerid,0xff0000ff,"Jestes polaczony z innym naszym serwerem. Mozesz byc polaczony tylko do jednego w danej chwili!");
      SendClientMessage(playerid,0xff0000ff,"You're connected to our other server. You can be connected to only one at a time.");
      SendClientMessage(playerid,-1," ");
      KickPlayer(playerid);
      mysql_free_result();
      return 0;
    }
  } else {
    for(new i=0; i<MAX_HUD_ELEMENTS-1;i++)  // bez ostatniego elementu - FPS
      pData[playerid][hudSetting][i]=true;

    pData[playerid][jail]=0;
    pData[playerid][allowedLevel]=0;
  }

  mysql_free_result();

  if(pData[playerid][language]>=MAX_LANGUAGES)  // w pewnej czesci rozwoju zdecydowalem sie wylaczyc j. angielski
      pData[playerid][language]=0;      // to resetuje kazdy jezyk powyzej indeksu na polski
  return 1;
}

public UpdateTimer()
{
  if (gmTemp[updateTimerRunning]) {   // nie wyrobilismy sie w sekunde z updatetimerem! olaboga!
    printf("UpdateTimer nie wyrobil sie w petli ze swoimi zadaniami! OLABOGA!");
    if (random(3)!=1)
      return;
  }

  gmTemp[updateTimerRunning]=true;
  new
   tick = GetTickCount(),
   sec;

  sec = tick / 1000;
  if (gmTemp[voteTimeleft]>0 && (sec-gmTemp[interval_voteCheck])>=1) {
    new str[1024];

    if (--gmTemp[voteTimeleft]==0) {
      if (gmTemp[voteCount][0]==gmTemp[voteCount][1]) // remis
        format(str,sizeof str,"%s ~n~ ~n~~y~REMIS!",gmTemp[voteQuestion]);
      else if (gmTemp[voteCount][0]>gmTemp[voteCount][1]) // tak
        format(str,sizeof str,__("%s ~n~ ~n~~y~TAK!    ~w~Glosow: %d"),gmTemp[voteQuestion], gmTemp[voteCount][0]);
      else
        format(str,sizeof str,__("%s ~n~ ~n~~y~NIE!    ~w~Glosow: %d"),gmTemp[voteQuestion], gmTemp[voteCount][1]);

      TextDrawSetString(gTextDraw[TD_VOTING],str);
      gmTemp[voteTimeleft]=-1;

      format(str,sizeof str,"GLOSOWANIE ZAKONCZONE, autor: id %d, pytanie: %s, TAK: %d, NIE: %d",gmTemp[voteAuthor],gmTemp[voteQuestion],gmTemp[voteCount][0],gmTemp[voteCount][1]);
      OutputLog(LOG_CHAT,str);
    } else {
      format(str,sizeof str,__("%s %s~n~~y~%2d       ~g~%d   ~w~/TAK   ~r~%d   ~w~/NIE"),
          gmTemp[voteQuestion], (strlen(gmTemp[voteQuestion])<35) ? ("~n~ ") : (" "),
          gmTemp[voteTimeleft],gmTemp[voteCount][0],gmTemp[voteCount][1]);
      TextDrawSetString(gTextDraw[TD_VOTING],str);
    }

    gmTemp[interval_voteCheck]=sec;
  }
  if (gmTemp[voteTimeleft]==-1 && (sec-gmTemp[interval_voteCheck])>=5) {
    TextDrawHideForAll(gTextDraw[TD_VOTING]);
    gmTemp[interval_voteCheck]=sec;
    gmTemp[voteTimeleft]=0;
  }

  //  --- ATTRACTION UPDATE: CHOWANY ---
  if(aData[A_CHOWANY][aState] == A_STATE_ON && ((sec - gmTemp[interval_aChowanyUpdate]) >= 2))
  {
    new
      bool:aBreak=false;
    if(tick - gmTemp[aChowanyStartTick] >= 480000) // 8 minut
    {
      aBreak=true;

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("Kilka osób zosta³o na chowanym po 5 minutach i ka¿dy z nich otrzymuje {b}4 punkty{/b} respektu!"));
    }
    if (!aBreak)
      CH_Update();
    else
      CH_Finish(true);

    gmTemp[interval_aChowanyUpdate] = sec;
  }

    //  --- ATTRACTION UPDATE: HAY ---
  if(aData[A_HAY][aState] == A_STATE_ON && ((sec - gmTemp[interval_aHAYUpdate]) >= 2))
  {
    new
      bool:aBreak=false;
    if(tick - gmTemp[aHAYStartTick] >= 300000) // 5 minut
    {
      aBreak=true;

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}HAY{/b} zostalo przerwany z powodu limitu czasu na ukonczenie."));
    }
    if (!aBreak)
      HAY_Update();
    else
      HAY_Finish();

    gmTemp[interval_aHAYUpdate] = sec;
  }

  //  --- ATTRACTION UPDATE: DERBY ---
  if(aData[A_DERBY][aState] == A_STATE_ON && (sec - gmTemp[interval_aDerbyUpdate]) >= 3) {
    if(tick - gmTemp[aDerbyStartTick] >= 480000) // 8 minut
    {
            aData[A_DERBY][aState] = A_STATE_OFF;
      Derby_Cleanup();
      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}Derby{/b} zostaly przerwane z powodu limitu czasu na ukonczenie."));
    } else {
      Derby_Recount();
    }
    gmTemp[interval_aDerbyUpdate] = sec;
  }

  //  --- ATTRACTION UPDATE: RACE ---
  if (aData[A_RACE][aState] == A_STATE_ON && (sec - gmTemp[interval_aRaceUpdate]) >= 3){
    if(tick - gmTemp[aRaceStartTick] >= 480000) // 8 minut
    {
            aData[A_RACE][aState] = A_STATE_OFF;
      Race_Cleanup();
      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}Race{/b} zostalo przerwane z powodu limitu czasu na ukonczenie."));
    } else {
      Race_Recount();
    }
    gmTemp[interval_aRaceUpdate]=sec;
  }

  //  --- ATTRACTION UPDATE: DRIFT ---
  if (aData[A_DRIFT][aState] == A_STATE_ON && (sec - gmTemp[interval_aDriftUpdate]) >= 3){
    if(tick - gmTemp[aDriftStartTick] >= 300000) // 8 minut
    {
            aData[A_DRIFT][aState] = A_STATE_OFF;
      Drift_Cleanup();
      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}Drifting{/b} zostalo przerwane z powodu limitu czasu na ukonczenie."));
    } else {
      Drift_Recount();
    }
    gmTemp[interval_aDriftUpdate]=sec;
  }

  //  --- ATTRACTION UPDATE: WG ---
  if(aData[A_WG][aState] == A_STATE_ON && (sec - gmTemp[interval_aWGUpdate]) >= 3)
  {
    new
     bool:aBreak = false;

    if(tick - gmTemp[aWGStartTick] >= 480000) // 8 minut
    {
      aBreak = true;

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}Wojna gangow{/b} zostala przerwana z powodu limitu czasu na ukonczenie.")); // WG zosta³o przerwane z powodu limitu czasu na ukoñczenie.
    }

    if(!aBreak)
      WG_Update();

    if(aBreak)
      WG_Finish();

    gmTemp[interval_aWGUpdate] = sec;
  }

    //  --- ATTRACTION UPDATE: CTF ---
  if(aData[A_CTF][aState] == A_STATE_ON && (sec - gmTemp[interval_aCTFUpdate]) >= 3)
  {
    new
     bool:aBreak = false;

    if(tick - gmTemp[aCTFStartTick] >= 480000) // 8 minut
    {
      aBreak = true;

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}CTF{/b} zostalo przerwane z powodu limitu czasu na ukonczenie.")); // CTF zosta³o przerwane z powodu limitu czasu na ukoñczenie.
    }

    if(!aBreak)
      CTF_Update();

    if(aBreak)
      CTF_Finish();

    gmTemp[interval_aCTFUpdate] = sec;
  }
  
    //  --- ATTRACTION UPDATE: GG ---
  if(aData[A_GG][aState] == A_STATE_ON && (sec - gmTemp[interval_aGGUpdate]) >= 3)
  {
    new
     bool:aBreak = false;

    if(tick - gmTemp[aGGStartTick] >= 1920000) // 32 minut
    {
      aBreak = true;

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}GunGame{/b} zostalo przerwane z powodu limitu czasu na ukonczenie.")); // GG zosta³o przerwane z powodu limitu czasu na ukoñczenie.
    }

    if(!aBreak)
      GG_Update();

    if(aBreak)
      GG_Finish();

    gmTemp[interval_aGGUpdate] = sec;
  }

  //  --- ATTRACTION UPDATE: STRZELNICA ---
  if(aData[A_STRZELNICA][aState] == A_STATE_ON && (sec - gmTemp[interval_aStrzelnicaUpdate]) >= 5)
  {
    strzelnica_AttractionUpdate(tick);
    gmTemp[interval_aStrzelnicaUpdate] = sec;
  }

  //  --- STARS UPDATE + RESPECT ADDITION ---
  if((sec - gmTemp[interval_gangs]) >= (360+gmTemp[pPlayerCount])) {
    gangs_saveGangData();
    gmTemp[interval_gangs]=sec;
  }

  //  --- STATS UPDATE ---
  if((sec - gmTemp[interval_statsUpdate]) >=
      gmTemp[pPlayerCount]>100 ? 10 : ( gmTemp[pPlayerCount]>50 ? 7 : 5 )
      )
  {

    new
     __admins = 0,
     _absadmins = 0,
     __vips = 0,
     _gms = 0,
     _players = 0;

    foreach(playerid)
    {
        _players++;

        if(pData[playerid][loggedIn] && pData[playerid][vipEnabled]) __vips++;
        switch(pData[playerid][adminLevel]) {
          case LEVEL_ADMINHIDDEN:
            _absadmins++;
          case LEVEL_ADMIN1,LEVEL_ADMIN2,LEVEL_ADMIN3: {
            __admins++;
            _absadmins++;
          }
          case LEVEL_GM:
            _gms++;
        }

        if(pTemp[playerid][staleTime]<3 && pData[playerid][hudSetting][HUD_STATUSBAR])
          UpdatePlayerStatsTD(playerid);

    }
    gmTemp[pPlayerCount]=_players;
    gmTemp[pAdminCount]=__admins;
    gmTemp[pAbsAdminCount]=_absadmins;
    gmTemp[pGMCount]=_gms;
    gmTemp[pVipCount]=__vips;
    gmTemp[interval_statsUpdate] = sec;
  }

  //  --- ARTEFACT ---
  if((sec - gmTemp[interval_artefact]) >= 300)  // 5 minut
  {
    domy_Reload();
    obiekty_odswiezMiscPickups();
    gmTemp[interval_artefact] = sec;
  }

  if (gmTemp[performBenchmark]>0) {
    new buf[48];
    format(buf,sizeof buf, "~g~ %d~w~ms at ~g~%d ~w~ticks", GetTickCount()-tick,GetServerTickRate());
    GameTextForAll(buf,1250,3);
    gmTemp[performBenchmark]--;
  }
  gmTemp[updateTimerRunning]=false;
}

SavePlayerData(playerid)
{
  if(pData[playerid][loggedIn])
  {
    UpdatePlayerAccountData(playerid);
    pTemp[playerid][lastSessionSaveTick] = GetTickCount();
  }
}

GetDialogList(playerid, dialogid)
{
  new
   szList[1024];

  switch(dialogid)
  {
    case DIALOG_ARENASOLO_SELECT:
    {
      for(new i=0;i<sizeof DATA_arenysolo;i++)
      {
        format(szList,sizeof szList,"%s%s%s", szList, ((i>0)?("\n"):("")), DATA_arenysolo[i][eas_nazwa]);
      }
      return szList;
    }
    case DIALOG_WEAPON_QUICKBUY:
    {
      for(new i=0;i<sizeof quickbuyWeapons;i++)
      if (quickbuyWeapons[i][ewd_quickBuy])
        format(szList,sizeof szList,"%s%s{FCE206}%d$\t{ffffff}%s", szList, ((i>0)?("\n"):("")), quickbuyWeapons[i][ewd_baseCost],quickbuyWeapons[i][ewd_PLname]);
      return szList;
    }

    case DIALOG_CONFIG_ASETTINGS:
    {
      for(new i = 0; i < MAX_ATTRACTIONS; i++)
      {
        strcat(szList, aData[i][aName]);

        if(i < MAX_ATTRACTIONS)
          strcat(szList, "\n");
      }
    }
    case DIALOG_CONFIG_MAIN:
    {
      for(new i = 0; i < sizeof fNames_config; i++)
      {
        new
         buffer[64];

        switch(i)
        {
          case 2:
          {
            format(buffer, sizeof buffer, "%s [%i]", fNames_config[i], gmData[maxPing]);
            strcat(szList, buffer);
          }

          case 5:
          {
            format(buffer, sizeof buffer, "%s [%s]", fNames_config[i], gmData[chatColors] ? ("ON") : ("OFF"));
            strcat(szList, buffer);
          }

          default: strcat(szList, fNames_config[i]);
        }

        if(i < sizeof fNames_config)
          strcat(szList, "\n");
      }
    }

    case DIALOG_CONFIG_PCOLORS:
    {
      for(new i = 0; i < sizeof fNames_config_pColors; i++)
      {
        new
         buffer[64];

        switch(i)
        {
          case 0: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_normalUser]), fNames_config_pColors[i]);
          case 1: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_fsUser]), fNames_config_pColors[i]);
          case 2: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipUser]), fNames_config_pColors[i]);
          case 3: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_gmUser]), fNames_config_pColors[i]);
          case 4: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_lvl1User]), fNames_config_pColors[i]);
          case 5: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_lvl2User]), fNames_config_pColors[i]);
          case 6: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_adminUser]), fNames_config_pColors[i]);
        }

        strcat(szList, buffer);

        if(i < sizeof fNames_config_pColors)
          strcat(szList, "\n");
      }
    }

    case DIALOG_CONFIG_CCOLORS:
    {
      for(new i = 0; i < sizeof fNames_config_cColors; i++)
      {
        new
         buffer[64];

        switch(i)
        {
          case 0: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo]), fNames_config_cColors[i]);
          case 1: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo2]), fNames_config_cColors[i]);
          case 2: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo3]), fNames_config_cColors[i]);
          case 3: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatError]), fNames_config_cColors[i]);
          case 4: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_joinInfo]), fNames_config_cColors[i]);
          case 5: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_leaveInfo]), fNames_config_cColors[i]);
          case 6: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatGM]), fNames_config_cColors[i]);
          case 7: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin]), fNames_config_cColors[i]);
          case 8: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin3]), fNames_config_cColors[i]);
          case 9: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatPM]), fNames_config_cColors[i]);
          case 10: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatIC]), fNames_config_cColors[i]);
          case 11: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME]), fNames_config_cColors[i]);
          case 12: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipSay]), fNames_config_cColors[i]);
          case 13: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatVip]), fNames_config_cColors[i]);
          case 14: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo_HL]), fNames_config_cColors[i]);
          case 15: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo2_HL]), fNames_config_cColors[i]);
          case 16: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatInfo3_HL]), fNames_config_cColors[i]);
          case 17: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatError_HL]), fNames_config_cColors[i]);
          case 18: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_joinInfo_HL]), fNames_config_cColors[i]);
          case 19: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_leaveInfo_HL]), fNames_config_cColors[i]);
          case 20: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatGM_HL]), fNames_config_cColors[i]);
          case 21: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin_HL]), fNames_config_cColors[i]);
          case 22: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatAdmin3_HL]), fNames_config_cColors[i]);
          case 23: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatPM_HL]), fNames_config_cColors[i]);
          case 24: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatIC_HL]), fNames_config_cColors[i]);
          case 25: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME_HL]), fNames_config_cColors[i]);
          case 26: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatME_HL]), fNames_config_cColors[i]);
          case 27: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_vipSay_HL]), fNames_config_cColors[i]);
          case 28: format(buffer, sizeof buffer, "{%s}%s", IntToHex(gmData[color_chatVip_HL]), fNames_config_cColors[i]);
        }

        strcat(szList, buffer);

        if(i < sizeof fNames_config_cColors)
          strcat(szList, "\n");
      }
    }

    case DIALOG_HELP_MAIN:
    {
      for(new i = 0; i < sizeof fNames_help; i++)
      {
        strcat(szList, fNames_help[i]);

        if(i < sizeof fNames_help)
          strcat(szList, "\n");
      }
    }

    case DIALOG_HELP_CMD:
    {
      for(new i = 0; i < sizeof fNames_help_cmd; i++)
      {
        strcat(szList, fNames_help_cmd[i]);

        if(i < sizeof fNames_help_cmd)
          strcat(szList, "\n");
      }
    }

    case DIALOG_BANK_MAIN:
    {
      for(new i = 0; i < sizeof fNames_bank; i++)
      {
        strcat(szList, fNames_bank[i]);

        if(i < sizeof fNames_bank)
          strcat(szList, "\n");
      }
    }

    case DIALOG_STATS_MAIN:
    {
      for(new i = 0; i < sizeof fNames_stats; i++)
      {
        strcat(szList, fNames_stats[i]);

        if(i < sizeof fNames_stats)
          strcat(szList, "\n");
      }
    }

    case DIALOG_TOP_MAIN:
    {
      for(new i = 0; i < sizeof fNames_top; i++)
      {
        strcat(szList, fNames_top[i]);

        if(i < sizeof fNames_top)
          strcat(szList, "\n");
      }
    }

    case DIALOG_HUD:
    {
      new
       buffer[128];
      strcat(szList,"{FFD700}Przelacz wszystkie\n");

      for(new i = 0; i < sizeof fNames_hud; i++)
      {
        if(pData[playerid][hudSetting][i]) format(buffer, sizeof buffer, "{7DC158}[ON]  %s", fNames_hud[i]);
        else format(buffer, sizeof buffer, "{C15E58}[OFF] %s", fNames_hud[i]);
        strcat(szList, buffer);

        if(i < sizeof fNames_hud)
          strcat(szList, "\n");
      }
    }

    case DIALOG_CONFIG_ASETTINGS_LIST:
    {
      for(new i = 0; i < sizeof fNames_config_attraction; i++)
      {
        strcat(szList, fNames_config_attraction[i]);

        if(i < sizeof fNames_config_attraction)
          strcat(szList, "\n");
      }
    }

    case DIALOG_CONFIG_CENSORSHIP:
    {
      for(new i = 0; i < sizeof fNames_censorship_off; i++)
      {
        strcat(szList, (gmData[censorship]) ? fNames_censorship_on[i] : fNames_censorship_off[i]);

        if(i < sizeof fNames_censorship_off)
          strcat(szList, "\n");
      }
    }
  }


  return szList;
}

GetDialogContent(dialogid)
{
  new
   szContent[1024];

  switch(dialogid) {
    case DIALOG_HELP_RULES:
    {
      for(new i = 0; i < sizeof szHelpRules; i++)
      {
        strcat(szContent, szHelpRules[i]);

        if(i < sizeof szHelpRules)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP_RESPECT:
    {
      for(new i = 0; i < sizeof szHelpRespect; i++)
      {
        strcat(szContent, szHelpRespect[i]);

        if(i < sizeof szHelpRespect)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP_GPOINTS:
    {
      for(new i = 0; i < sizeof szHelpGPoints; i++)
      {
        strcat(szContent, szHelpGPoints[i]);

        if(i < sizeof szHelpGPoints)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP_STARS:
    {
      for(new i = 0; i < sizeof szHelpStars; i++)
      {
        strcat(szContent, szHelpStars[i]);

        if(i < sizeof szHelpStars)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP_VIP:
    {
      for(new i = 0; i < sizeof szHelpVIP; i++)
      {
        strcat(szContent, szHelpVIP[i]);

        if(i < sizeof szHelpVIP)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP_AUTHOR:
    {
      for(new i = 0; i < sizeof szHelpAuthor; i++)
      {
        strcat(szContent, szHelpAuthor[i]);

        if(i < sizeof szHelpAuthor)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__GENERAL:
    {
      for(new i = 0; i < sizeof szHelpCmdGeneral; i++)
      {
        strcat(szContent, szHelpCmdGeneral[i]);

        if(i < sizeof szHelpCmdGeneral)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__ATRACTIONS:
    {
      for(new i = 0; i < sizeof szHelpCmdAtractions; i++)
      {
        strcat(szContent, szHelpCmdAtractions[i]);

        if(i < sizeof szHelpCmdAtractions)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__ACCOUNT:
    {
      for(new i = 0; i < sizeof szHelpCmdAccount; i++)
      {
        strcat(szContent, szHelpCmdAccount[i]);

        if(i < sizeof szHelpCmdAccount)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__HOUSES:
    {
      for(new i = 0; i < sizeof szHelpCmdHouses; i++)
      {
        strcat(szContent, szHelpCmdHouses[i]);

        if(i < sizeof szHelpCmdHouses)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__TELEPORTS:
    {
      for(new i = 0; i < sizeof szHelpCmdTeleports; i++)
      {
        strcat(szContent, szHelpCmdTeleports[i]);

        if(i < sizeof szHelpCmdTeleports)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__ANIMATIONS:
    {
      for(new i = 0; i < sizeof szHelpCmdAnimations; i++)
      {
        strcat(szContent, szHelpCmdAnimations[i]);

        if(i < sizeof szHelpCmdAnimations)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__VIP:
    {
      for(new i = 0; i < sizeof szHelpCmdVIP; i++)
      {
        strcat(szContent, szHelpCmdVIP[i]);

        if(i < sizeof szHelpCmdVIP)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_HELP__ADMIN:
    {
      for(new i = 0; i < sizeof szHelpCmdAdmin; i++)
      {
        strcat(szContent, szHelpCmdAdmin[i]);

        if(i < sizeof szHelpCmdAdmin)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_STATS_PLAYER:
    {
      for(new i = 0; i < sizeof szStatsPlayer; i++)
      {
        strcat(szContent, szStatsPlayer[i]);

        if(i < sizeof szStatsPlayer)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_STATS_SERVER:
    {
      for(new i = 0; i < sizeof szStatsServer; i++)
      {
        strcat(szContent, szStatsServer[i]);

        if(i < sizeof szStatsServer)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_TOP_RESPECT:
    {
      for(new i = 0; i < sizeof szTopRespect; i++)
      {
        strcat(szContent, szTopRespect[i]);

        if(i < sizeof szTopRespect)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_TOP_FRAGS:
    {
      for(new i = 0; i < sizeof szTopFrags; i++)
      {
        strcat(szContent, szTopFrags[i]);

        if(i < sizeof szTopFrags)
          strcat(szContent, "\n");
      }
    }

    case DIALOG_TOP_TIMEPLAYED:
    {
      for(new i = 0; i < sizeof szTopPlayedTime; i++)
      {
        strcat(szContent, szTopPlayedTime[i]);

        if(i < sizeof szTopPlayedTime)
          strcat(szContent, "\n");
      }
    }
  }
  return szContent;
}

c_f_PlayerColors(playerid, listitem)
{
  ShowPlayerDialog(playerid, DIALOG_CONFIG_PCOLORS_SET, DIALOG_STYLE_INPUT, fNames_config_pColors[listitem], TXT(playerid, 79), __("Zapisz"), __("Wroc"));

  pTemp[playerid][lastDialog] = DIALOG_CONFIG_PCOLORS_SET;
  pTemp[playerid][lastDialogItem] = listitem;
}

c_f_ChatColors(playerid, listitem)
{
  ShowPlayerDialog(playerid, DIALOG_CONFIG_CCOLORS_SET, DIALOG_STYLE_INPUT, fNames_config_cColors[listitem], TXT(playerid, 79), __("Zapisz"), __("Wroc"));

  pTemp[playerid][lastDialog] = DIALOG_CONFIG_CCOLORS_SET;
  pTemp[playerid][lastDialogItem] = listitem;
}

FindNextPlayerToSpectate(playerid, bool:direction){
  if (pData[playerid][spectating]==INVALID_PLAYER_ID) return;
  new foundTarget=INVALID_PLAYER_ID;
  if (direction) {  // do przodu
    for (new i=pData[playerid][spectating]+1; i<=gmTemp[highestPlayerID] && foundTarget==INVALID_PLAYER_ID; i++)
      if (IsPlayerConnected(i) && IsPlayerSpawned(i) && (pData[i][adminLevel]<=pData[playerid][adminLevel] || pData[i][adminLevel]==LEVEL_ADMINHIDDEN))
        foundTarget=i;
    if (foundTarget==INVALID_PLAYER_ID) // jeszcze nie znaleziono, lecimy od 0
    for (new i=0; i<=pData[playerid][spectating] && foundTarget==INVALID_PLAYER_ID; i++)
      if (IsPlayerConnected(i) && IsPlayerSpawned(i) && (pData[i][adminLevel]<=pData[playerid][adminLevel] || pData[i][adminLevel]==LEVEL_ADMINHIDDEN))
        foundTarget=i;
  } else {
    for (new i=pData[playerid][spectating]-1; i>=0 && foundTarget==INVALID_PLAYER_ID; i--)
      if (IsPlayerConnected(i) && IsPlayerSpawned(i) && (pData[i][adminLevel]<=pData[playerid][adminLevel] || pData[i][adminLevel]==LEVEL_ADMINHIDDEN))
        foundTarget=i;
    if (foundTarget==INVALID_PLAYER_ID) // jeszcze nie znaleziono, lecimy od konca
    for (new i=gmTemp[highestPlayerID]; i<=pData[playerid][spectating] && foundTarget==INVALID_PLAYER_ID; i--)
      if (IsPlayerConnected(i) && IsPlayerSpawned(i) && (pData[i][adminLevel]<=pData[playerid][adminLevel] || pData[i][adminLevel]==LEVEL_ADMINHIDDEN))
        foundTarget=i;
  }
  if (foundTarget==INVALID_PLAYER_ID) // nie znaleziono, przerywamy specowanie
    StopSpectating(playerid);
  else
    StartSpectating(playerid, foundTarget);
}

StartSpectating(playerid, targetplayerid)
{
  if(pData[playerid][pAttraction] != A_NONE)
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 286));
    return;
  }

  if (pData[playerid][spectating]==INVALID_PLAYER_ID) {
    GetPlayerPosition(playerid, pTemp[playerid][specPosition][0], pTemp[playerid][specPosition][1], pTemp[playerid][specPosition][2], pTemp[playerid][specPosition][3]);
    pTemp[playerid][specInterior] = GetPlayerInterior(playerid);
    pTemp[playerid][specVirtualWorld] = GetPlayerVirtualWorld(playerid);
  }
  pData[playerid][spectating] = targetplayerid;
  TogglePlayerSpectating(playerid, true);
  SetPlayerInterior(playerid,GetPlayerInterior(targetplayerid));
  SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(targetplayerid));

  if(pTemp[targetplayerid][cheater])
    Msg(playerid,COLOR_INFO2,"Gracz jest odizolowany od reszty graczy (/drop)");

  if(IsPlayerInAnyVehicle(targetplayerid)) {
    new vid=GetPlayerVehicleID(targetplayerid);
    PlayerSpectateVehicle(playerid, vid);
  } else
    PlayerSpectatePlayer(playerid, targetplayerid);

  new Float:HP,Float:AR;
  GetPlayerArmour(targetplayerid, AR);
  GetPlayerHealth(targetplayerid, HP);
  if (pData[targetplayerid][pAttraction]!=A_NONE && pData[targetplayerid][pAttraction]<A_ARENA)
    if(!pTemp[targetplayerid][godMode]) format(gstr, sizeof gstr, "%s (%d), HP: %3.0f AR: %3.0f, atrakcja: %s", GetPlayerNick(targetplayerid), targetplayerid, HP, AR, aData[pData[targetplayerid][pAttraction]][aName]);
    else format(gstr, sizeof gstr, "%s (%d), HP: TRYB PASYWNY, atrakcja: %s", GetPlayerNick(targetplayerid), targetplayerid, aData[pData[targetplayerid][pAttraction]][aName]);
  else{
      if(!pTemp[targetplayerid][godMode]) format(gstr, sizeof gstr, "%s (%d), HP: %3.0f AR: %3.0f", GetPlayerNick(targetplayerid), targetplayerid, HP, AR);
      else format(gstr, sizeof gstr, "%s (%d), TRYB PASYWNY", GetPlayerNick(targetplayerid), targetplayerid);
  }

    SCM(playerid, -1, " ");
  SCM(playerid, -1, gstr);
  SCM(playerid, -1, " ");

  format(gstr, sizeof gstr, "%d", targetplayerid);

  _cwep(playerid, gstr);
}

StopSpectating(playerid)
{
  pData[playerid][spectating] = INVALID_PLAYER_ID;
  pTemp[playerid][specPosReturn] = true;

  TogglePlayerSpectating(playerid, false);
}

FreezePlayer(playerid, ms)
{
  TogglePlayerControllable(playerid, false);

  KillTimer(pTemp[playerid][timerFreezePlayer]);
  pTemp[playerid][timerFreezePlayer] = SetTimerEx("FreezePlayer_Unfreeze", ms, false, "i", playerid);
}

public FreezePlayer_Unfreeze(playerid)
{
  TogglePlayerControllable(playerid, true);
}

public PutInVehicleAfterTeleport(playerid, vehicleid, bool:useOnce)
{
  new
   Float:pVector[e_Vectors];

  GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z], pVector[A]);

  new buffer[128];
  format(buffer, 128, "%f %f %f %f", pVector[X], pVector[Y], pVector[Z], pVector[A]);
  Msg(playerid, 0xFFFFFFFF, buffer);

  pData[playerid][lastVehicle] = CreateVehicle(vehicleid, pVector[X], pVector[Y], pVector[Z], pVector[A], random(15), random(15), 99999);
  if(useOnce) pData[playerid][destroyMyVehicle] = true;

  PutPlayerInVehicle(playerid, pData[playerid][lastVehicle], 0);
  TogglePlayerControllable(playerid, true);
}

PlayRandomMusic(playerid, time = 0)
{
  new musicSounds[] = {
   1062,
   1068,
   1076,
   1097,
   1183,
   1185,
   1187
  };

  new
   randomIdx = random(sizeof musicSounds);

  PlaySound(playerid, musicSounds[randomIdx]);

  if(time)
  {
    KillTimer(pTemp[playerid][timerStopPlayerMusic]);
    pTemp[playerid][timerStopPlayerMusic] = SetTimerEx("StopPlayerMusic", time, false, "i", playerid);
  }
}

DisablePlayerSounds(playerid)
{
  PlaySound(playerid, 1188);
}

public StopPlayerMusic(playerid)
{
  DisablePlayerSounds(playerid);
}

OutputVipChat(playerid, szMsg[])
{
  new
   buffer[200];

  format(buffer, sizeof buffer, "VIP> %s: {e0e0ff}%s", GetPlayerProperName(playerid), szMsg);

  foreach(i)
    if(pData[i][vipEnabled] || IsGM(i))
    {
      Msg(i, COLOR_VIP, buffer, true, false);
      PlaySound(i, 1027);
    }

  format(buffer, sizeof buffer, "VIP> %s: %s", GetPlayerNick(playerid), szMsg);
  OutputLog(LOG_OCHAT, buffer);
}

OutputModeratorChat(playerid, szMsg[])
{
  new
   buffer[200];

  if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
    format(buffer, sizeof buffer, " {b}(GM) (Zdalny admin):{/b} %s", szMsg);
  else
    format(buffer, sizeof buffer, " {b}(GM) %s:{/b} %s", GetPlayerProperName(playerid), szMsg);

  foreach(i)
  {
    if(IsGM(i))
    {
      Msg(i, COLOR_GM, buffer, true, false);
      PlaySound(i, 1027);
    }
  }

  format(buffer, sizeof buffer, "GM> %s: %s", GetPlayerNick(playerid), szMsg);
  OutputLog(LOG_OCHAT, buffer);
}

OutputAdminChat(playerid, szMsg[])
{
  new
   buffer[200];

  if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
    format(buffer, sizeof buffer, "{b}A> (zdalny):{/b} %s", szMsg);
  else
    format(buffer, sizeof buffer, "{b}A> %s:{/b} %s", GetPlayerProperName(playerid), szMsg);

  foreach(i)
  {
    if(IsAdmin(i))
    {
      Msg(i, COLOR_ADMIN, buffer, true, false);
      PlaySound(i, 1027);
    }
  }

  format(buffer, sizeof buffer, "A> %s: %s", GetPlayerNick(playerid), szMsg);
  OutputLog(LOG_OCHAT, buffer);
}

OutputAdmin3Chat(playerid, szMsg[])
{
  new
   buffer[200];

  format(buffer, sizeof buffer, "AA> %s: %s", GetPlayerProperName(playerid), szMsg);

  foreach(i)
  {
    if(IsAdmin(i, LEVEL_ADMIN3))
    {
      Msg(i, COLOR_ADMIN3, buffer, true, false);
      PlaySound(i, 1145);
    }
  }

  format(buffer, sizeof buffer, "AA> %s: %s", GetPlayerNick(playerid), szMsg);
  OutputLog(LOG_OCHAT, buffer);
}

public ResetPlayerName(playerid)
{
  SetPlayerName(playerid, pTemp[playerid][tmpPlayerName]);
}

stock RandomizeWeather()
{
  gmTemp[currentWeather]=DATA_weathers[random(sizeof DATA_weathers)];
  foreach(i)
    if (GetPlayerInterior(i)==0)
      SetPlayerWeather(i,gmTemp[currentWeather]);
  gmTemp[lastWeatherChangeTime]=GetTickCount();
}

public SearchForNewAdmin()
{
  foreach(playerid)
  {
    if(!pTemp[playerid][isRcon] && IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]>=LEVEL_ADMIN3)
    {
      pTemp[playerid][isRcon] = true;
      OnPlayerRCONLogin(playerid);

      break;
    } else if (!pTemp[playerid][isRcon] && IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]<LEVEL_ADMIN3) {
      new buffer[255];
      format(buffer,sizeof buffer,"Nieautoryzowane logowanie na admina RCON %d (dobre haslo!) przez %s (%d)! Poziom %d/%d.", pTemp[playerid][isRcon], GetPlayerNick(playerid), playerid, pData[playerid][adminLevel], pData[playerid][allowedLevel]);
      KickPlayer(playerid);
      foreach(i)
        if(IsAdmin(i, LEVEL_ADMIN1))
          Msg(i, COLOR_ERROR, buffer);
      OutputLog(LOG_SYSTEM, buffer);
    }
  }
}

public AnnounceFade()
{
  if(gmTemp[ann_effect_mode] == ANN_MODE_FADE_IN)     gmTemp[ann_effect_alpha] += 10;
  else if(gmTemp[ann_effect_mode] == ANN_MODE_FADE_OUT) gmTemp[ann_effect_alpha] -= 10;

  TextDrawColor(gTextDraw[TD_ANNOUNCE], SetAlpha(0xFFFFD0FF, gmTemp[ann_effect_alpha]));
  TextDrawBackgroundColor(gTextDraw[TD_ANNOUNCE], SetAlpha(0x000000FF, gmTemp[ann_effect_alpha] / 2));

  foreach(playerid)
    if (pData[playerid][hudSetting][HUD_ANNOUNCEMENTS])
      TextDrawShowForPlayer(playerid,gTextDraw[TD_ANNOUNCE]);

  if(gmTemp[ann_effect_alpha] >= 250)
  {
    KillTimer(gmTemp[timerAnnounce]);
    gmTemp[timerAnnounceEnd] = SetTimer("AnnounceFadeOutStart", 6000, false);
    gmTemp[ann_effect_mode] = ANN_MODE_ON;
  }

  if(gmTemp[ann_effect_alpha] <= 0)
  {
    KillTimer(gmTemp[timerAnnounce]);
    TextDrawHideForAll(gTextDraw[TD_ANNOUNCE]);
    gmTemp[ann_effect_mode] = ANN_MODE_OFF;

    if(gmTemp[ann_switchToNew])
    {
      gmTemp[ann_effect_alpha] = 1;
      gmTemp[ann_effect_mode] = ANN_MODE_FADE_IN;

      gmTemp[timerAnnounce] = SetTimer("AnnounceFade", 70, true);
      TextDrawSetString(gTextDraw[TD_ANNOUNCE], gmTemp[ann_switchToNewText]);

      gmTemp[ann_switchToNew] = false;
    }
  }
}

public AnnounceFadeOutStart()
{
  gmTemp[ann_effect_mode] = ANN_MODE_FADE_OUT;

  gmTemp[timerAnnounce] = SetTimer("AnnounceFade", 70, true);
}

RefreshPlayerVehicleInfo(playerid)
{
  new
   vehicleid = GetPlayerVehicleID(playerid),
   Float:vHealth,
   vmodel;

    if (vehicleid==0 || vehicleid==INVALID_VEHICLE_ID) return;

  vmodel=GetVehicleModel(vehicleid);
  if (vmodel==0) return;
  GetVehicleHealth(vehicleid, vHealth);

  vHealth = (vHealth - 250.0) / 7.5; // zeskalowane dla DVHP 1000
  if (vHealth>100) vHealth=100;

  new hpstr[16];
  if (vHealth>=95)
    hpstr="~g~]]]]]";
  else if (vHealth>=85)
    hpstr="~g~]]]]~l~]";
  else if (vHealth>=75)
    hpstr="~y~~h~]]]~l~]]";
  else if (vHealth>=50)
    hpstr="~y~]]]~l~]]";
  else if (vHealth>=30)
    hpstr="~y~]]~l~]]]";
  else if (vHealth>=10)
    hpstr="~r~]~l~]]]]";
  else
    hpstr="~l~]]]]]";

  format(gstr, sizeof gstr, "~r~~h~%s~n~~b~~h~~h~~h~%i ~w~km/h    %s",
    vehicleName[vmodel - 400],
    GetVehicleSpeed(vehicleid),hpstr);

  PlayerTextDrawSetString(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], gstr);

  if(gmData[artefactOwner] == playerid)
  {
    new vf=GetVehicleFlags(vehicleid);

    if (vf&VF_MILITARY==VF_MILITARY || vf&VF_AIRBORNE==VF_AIRBORNE || vf&VF_NATATORIAL==VF_NATATORIAL){
      Msg(playerid,COLOR_ERROR,"Niestety, artefakt postanowil Cie opuscic...");
      DropArtefact(gmData[artefactOwner]);
    }
  }
}

FilterText(playerid, text[], bool:antiSpam = false)
{
  //  --- Space filter ---
  new
   tmpPos = 0,
   bigCaseLetters;

  while(tmpPos < strlen(text))
  {
    if(text[tmpPos] >= 'A' && text[tmpPos] <= 'Z') bigCaseLetters++;
    else if(text[tmpPos] == ' ')
    {
      if(text[tmpPos + 1] == ' ')
      {
        strdel(text, tmpPos, tmpPos + 1);

        continue;
      }
    }
    tmpPos++;
  }

  //  --- Caps lock filter ---
  new
   length = strlen(text);

  if(length>5 && ((float(length) / float(bigCaseLetters)) <= 3.3 && bigCaseLetters > 0))
    for(new i = 0; i < length; i++)
      text[i] = DownCase(text[i]);

  //  --- Anti spam ---
  if(antiSpam && pData[playerid][adminLevel]<LEVEL_ADMIN3)
  {
    if(GetTickCount() - pTemp[playerid][lastMsgTick] < (pData[playerid][respect]<1000?(pData[playerid][respect]<333?3000:2000):1000))
    {
      pTemp[playerid][spamCount]++;

      if(pTemp[playerid][spamCount] >= (pData[playerid][respect]<1000?(pData[playerid][respect]<333?5:9):12) )
      {
        format(gstr, sizeof gstr, __("Gracz %s dostal%s kicka z powodu: spamowanie."), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
        if (gmTemp[pPlayerCount]<20 || gmTemp[showJoins]==2)
          SendClientMessageToAll(0xA0A0A0FF, gstr);
        else
          MSGToAdmins(COLOR_INFO2, gstr, false, LEVEL_ADMIN2);

        Msg(playerid, COLOR_ERROR, TXT(playerid, 141));
        KickPlayer(playerid);
        return 0;
      }

      if(pTemp[playerid][spamCount] >= 2)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 140));
        pTemp[playerid][lastMsgTick] = GetTickCount();

        return 0;
      }
    }
    else
    {
      pTemp[playerid][spamCount] = 0;
    }

    pTemp[playerid][lastMsgTick] = GetTickCount();
  }
  return 1;
}

ShowPlayerHudElement(playerid, element, bool:show)
{
  switch(element)
  {
    case HUD_STATUSBAR:
    {
      ShowElement(playerid, TDE_STATS, show);
    }

    case HUD_ATTRACTIONBOX:
    {
      ShowElement(playerid, TDE_ATTRACTIONBOX, show);
    }

    case HUD_SERVERLOGO:
    {
      ShowElement(playerid, TDE_FULLSERVERLOGO, show);
    }

    case HUD_AREA: {  // zalaczane i wylaczane automatycznie
      if (!show) {
        TextDrawHideForPlayer(playerid,gTextDraw[TD_AREANODM]);
        TextDrawHideForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
      } else if (IsPlayerInFullDMArea(playerid))
        TextDrawShowForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
      else if (IsPlayerInNoDMArea(playerid))
        TextDrawShowForPlayer(playerid,gTextDraw[TD_AREANODM]);
    }

    case HUD_VOTING: {  // glosowania
      if (!show)
          TextDrawHideForPlayer(playerid,gTextDraw[TD_VOTING]);
    }

    case HUD_DATE:
    {
      ShowElement(playerid, TDE_DATETIME, show);
    }
    
    case HUD_CLOCK:
    {
      ShowElement(playerid, TDE_CLOCK, show);
    }

    case HUD_STARS:
    {
      if(show)
      {
        RefreshPlayerStars(playerid);
      }
      else
      {
        for(new i = TD_STARS_START; i <= TD_STARS_END; i++)
        {
          TextDrawHideForPlayer(playerid, gTextDraw[i]);
        }
      }
    }

    case HUD_FPS:
    {
      if(show) {
        pTemp[playerid][ept_fps]=30;  // srednia bedzie wyliczona co krok poprzez dodanie biezacej wartosci do poprzedniej oraz podzielenie wyniku przez 2 wiec szybko spadnie/wzrosnie do wlasciwej ilosci
        PlayerTextDrawShow(playerid,pTextDraw[PTD_FPS][playerid]);
      } else
        PlayerTextDrawHide(playerid,pTextDraw[PTD_FPS][playerid]);
    }

    case HUD_HP:
    {
        if(show){
          PlayerTextDrawShow(playerid, pTextDraw[PTD_HPTEXT][playerid] );
        PlayerTextDrawShow(playerid, pTextDraw[PTD_ARICON][playerid] );
        PlayerTextDrawShow(playerid, pTextDraw[PTD_HPICON][playerid] );
      } else {
          PlayerTextDrawHide(playerid, pTextDraw[PTD_HPTEXT][playerid] );
        PlayerTextDrawHide(playerid, pTextDraw[PTD_ARICON][playerid] );
        PlayerTextDrawHide(playerid, pTextDraw[PTD_HPICON][playerid] );
      }
    }
    
#if defined CJHIT_MODEL
    case HUD_CJHIT:
    {
      if(show) {
        PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT2][playerid]);
        PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT][playerid], GetPlayerSkin(playerid));
        PlayerTextDrawShow(playerid,pTextDraw[PTD_CJHIT][playerid]);
        PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], "_");
        PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT_TXT][playerid]);
        PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_HP][playerid], "_");
        PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT_HP][playerid]);
      } else
        PlayerTextDrawHide(playerid,pTextDraw[PTD_CJHIT][playerid]);
        PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT2][playerid]);
        PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], "_");
        PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT_TXT][playerid]);
        PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_HP][playerid], "_");
        PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT_HP][playerid]);
    }
#endif
  }
}

SaveAttractionData(attractionID)
{
  new
   buffer[64];

  format(buffer, sizeof buffer, "%s %i %i", aData[attractionID][aName], aData[attractionID][aStartPlayers], aData[attractionID][aStartingTime]);

  switch(attractionID)
  {
    case A_CHOWANY:   SetConfigValueString("a_chowany", buffer);
    case A_HAY:     SetConfigValueString("a_sps", buffer);
    case A_DERBY:     SetConfigValueString("a_derby", buffer);
    case A_RACE:    SetConfigValueString("a_race", buffer);
    case A_DRIFT:     SetConfigValueString("a_drift", buffer);
    case A_WG:      SetConfigValueString("a_wg", buffer);
    case A_STRZELNICA:  SetConfigValueString("a_strzelnica", buffer);
    case A_CTF:     SetConfigValueString("a_ctf", buffer);
    case A_GG:     SetConfigValueString("a_gg", buffer);
  }
}

public AnimateFullServerLogo()
{
  switch(++gmTemp[fsaIndex])
  {
    case 0:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "~r~w~w~ww.FullServer.eu");
    case 1:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "~r~~h~w~r~w~w~w.FullServer.eu");
    case 2:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "~r~~h~~h~w~r~~h~w~r~w~w~.FullServer.eu");
    case 3:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "~r~~h~~h~~h~w~r~~h~~h~w~r~~h~w~r~.~w~FullServer.eu");
    case 4:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "~r~~h~~h~~h~~h~w~r~~h~~h~~h~w~r~~h~~h~w~r~~h~.~r~F~w~ullServer.eu");
    case 5:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "~r~~h~~h~~h~~h~~h~w~r~~h~~h~~h~~h~w~r~~h~~h~~h~w~r~~h~~h~.~r~~h~F~r~u~w~llServer.eu");
    case 6:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "w~r~~h~~h~~h~~h~~h~w~r~~h~~h~~h~~h~w~r~~h~~h~~h~.~r~~h~~h~F~r~~h~u~r~l~w~lServer.eu");
    case 7:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "ww~r~~h~~h~~h~~h~~h~w~r~~h~~h~~h~~h~.~r~~h~~h~~h~F~r~~h~~h~u~r~~h~l~r~l~w~Server.eu");
    case 8:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www~r~~h~~h~~h~~h~~h~.~r~~h~~h~~h~~h~F~r~~h~~h~~h~u~r~~h~~h~l~r~~h~l~r~S~w~erver.eu");
    case 9:   TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.~r~~h~~h~~h~~h~~h~F~r~~h~~h~~h~~h~u~r~~h~~h~~h~l~r~~h~~h~l~r~~h~S~r~e~w~rver.eu");
    case 10:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.F~r~~h~~h~~h~~h~~h~u~r~~h~~h~~h~~h~l~r~~h~~h~~h~l~r~~h~~h~S~r~~h~e~r~r~w~ver.eu");
    case 11:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.Fu~r~~h~~h~~h~~h~~h~l~r~~h~~h~~h~~h~l~r~~h~~h~~h~S~r~~h~~h~e~r~~h~r~r~v~w~er.eu");
    case 12:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.Ful~r~~h~~h~~h~~h~~h~l~r~~h~~h~~h~~h~S~r~~h~~h~~h~e~r~~h~~h~r~r~~h~v~r~e~w~r.eu");
    case 13:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.Full~r~~h~~h~~h~~h~~h~S~r~~h~~h~~h~~h~e~r~~h~~h~~h~r~r~~h~~h~v~r~~h~e~r~r~w~.eu");
    case 14:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullS~r~~h~~h~~h~~h~~h~e~r~~h~~h~~h~~h~r~r~~h~~h~~h~v~r~~h~~h~e~r~~h~r~r~.~w~eu");
    case 15:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullSe~r~~h~~h~~h~~h~~h~r~r~~h~~h~~h~~h~v~r~~h~~h~~h~e~r~~h~~h~r~r~~h~.~r~e~w~u");
    case 16:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullSer~r~~h~~h~~h~~h~~h~v~r~~h~~h~~h~~h~e~r~~h~~h~~h~r~r~~h~~h~.~r~~h~e~r~u");
    case 17:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullServ~r~~h~~h~~h~~h~~h~e~r~~h~~h~~h~~h~r~r~~h~~h~~h~.~r~~h~~h~e~r~~h~u");
    case 18:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullServe~r~~h~~h~~h~~h~~h~r~r~~h~~h~~h~~h~.~r~~h~~h~~h~e~r~~h~~h~u");
    case 19:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullServer~r~~h~~h~~h~~h~~h~.~r~~h~~h~~h~~h~e~r~~h~~h~~h~u");
    case 20:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullServer.~r~~h~~h~~h~~h~~h~e~r~~h~~h~~h~~h~u");
    case 21:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullServer.e~r~~h~~h~~h~~h~~h~u");
    case 22:  TextDrawSetString(gTextDraw[TD_FULLSERVERWEBSITE], "www.FullServer.eu");
    default:    KillTimer(gmTemp[fsaTimer]);
  }
}

public NoMoreWordsDialogDelayFix(playerid)
{
  ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP, DIALOG_STYLE_LIST, TXT(playerid, 386), GetDialogList(playerid, DIALOG_CONFIG_CENSORSHIP), __("Zapisz"), __("Wroc"));
}

bool:IsWordCensored(szWord[])
{
  new
   File:hFile = fopen("FullServer/cenzura.ini", io_read),
   buffer[32],
   length;

  while(fread(hFile, buffer))
  {
    length = strlen(buffer);
    strdel(buffer, length - 1, length);

    if(strcmp(buffer, szWord, true) == 0)
    {
      return true;
    }
  }

  fclose(hFile);

  return false;
}

CensorText(text[])
{
  if(!gmData[censorship]) return 0;

  new
   buffer[32],
   szSource[128],
   _start,
   _end,
   _find=0;

  copy(text, szSource);

  for(new i = 0; i < gmData[maxCensoredWords]; i++)
  {
    while(FindString(szSource, gCensoredWord[i], _start, _end))
    {
        if(_start!=0 && (szSource[_start-1])!=' ') break;

      copy(gCensoredWord[i], buffer);

      for(new g = 1; g < strlen(buffer) - 1; g++)
      {
        buffer[g] = '*';
      }

      strdel(text, _start, _end);
      strins(text, buffer, _start, 128);

      strdel(szSource, _start, _end);
      strins(szSource, buffer, _start, 128);

      _find++;
    }
  }

  if(_find>0) {
    return 1;
  }else {
    return 0;
  }
}

FilterTextWithColors(text[])
{
  new
   pos = -1;

  while((pos = strfind(text, "[", false, pos + 1)) != -1)
  {
    new
     c = pos + 1,
     n = 0,
     ch;

    while((ch = text[c]) && n != 6)
    {
      if(!('a' <= ch <= 'f' || 'A' <= ch <= 'F' || '0' <= ch <= '9')) break;

      ++c;
      ++n;
    }

    if(n == 6 && ch == ']')
    {
      text[pos] = '{';
      text[c] = '}';
    }
  }
}

AddPlayerToReportList(playerid, reason[], reporting_playerid)
{
  if(pData[playerid][reported]) return;

  for(new i = 0; i < MAX_REPORTS; i++)
  {
    if(!gReports[i][rOccupiedIndex])
    {
      gReports[i][rPlayerId] = playerid;
      copy(GetPlayerNick(playerid), gReports[i][rPlayerName]);
      copy(reason, gReports[i][rReason]);
      gReports[i][rOccupiedIndex] = true;
      copy(GetPlayerNick(reporting_playerid), gReports[i][rReportingPlayerName]);

      pData[playerid][reported] = true;
      return;
    }
  }
}

RemovePlayerFromReportList(playerid)
{
  for(new i = 0; i < MAX_REPORTS; i++)
  {
    if(gReports[i][rOccupiedIndex] && gReports[i][rPlayerId] == playerid)
    {
      gReports[i][rOccupiedIndex] = false;
      pData[gReports[i][rPlayerId]][reported] = false;

      break;
    }
  }
}

public HidePlayerWarning(playerid)
{
  if (IsPlayerConnected(playerid))
    TextDrawHideForPlayer(playerid, gTextDraw[TD_WARNING]);
  return 1;

}

UpdatePlayerStatsTD(playerid)
{
  new pPing=GetPlayerPing(playerid);
  static str[160];

  new pSessionH = GetPlayerCurrentSession(playerid) / 60;

  format(str,sizeof str,"%02dh%02dm    ~w~%dms      ", pSessionH/60, pSessionH%60, pPing);

  format(str,sizeof str,"%23s%d", str, pData[playerid][respect]);
  format(str,sizeof str,"%33s%d", str, pData[playerid][skill]);
  format(str,sizeof str,"%43s%d", str, pData[playerid][gamep]);

  PlayerTextDrawSetString(playerid,pTextDraw[PTD_STAT][playerid], str);
}

UpdatePlayerNickTD(playerid)
{
  if (IsAdmin(playerid,LEVEL_ADMIN1))
    format(gstr,sizeof gstr,"~r~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),  playerid);
  else if (IsGM(playerid))
    format(gstr,sizeof gstr,"~r~~h~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid), playerid);
  else if (pData[playerid][vipEnabled])
    format(gstr,sizeof gstr,"~y~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),  playerid);
  else
    format(gstr,sizeof gstr,"~w~%s ~w~(~y~%d~w~)",GetPlayerNick(playerid),  playerid);

  PlayerTextDrawSetString(playerid,pTextDraw[PTD_NICK][playerid], gstr);

  UpdatePlayerFactionName(playerid);
}

public PlayerSelfHeal(playerid){
  if (pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid,COLOR_ERROR,"Nie mozesz sie teraz uleczyc!");
  SetPlayerHealth(playerid,100.0);
  ShowPlayerHealIcon(playerid,playerid);
  if (Kobieta(playerid))
    Msg(playerid,COLOR_INFO,"Zostalas uleczona");
  else
    Msg(playerid,COLOR_INFO,"Zostales uleczony");
  return 1;
}

ShowPlayerInfo(playerid,targetid){
  new buffer[2048];
  if(!IsPlayerConnected(targetid))
    return Msg(playerid,COLOR_ERROR,"Nie znaleziono gracza!");
  format(buffer,sizeof buffer,"{d0d0d0}Nick: {ffffff}%s \n",GetPlayerNick(targetid));
  if (pData[targetid][loggedIn]) {
    switch(pData[targetid][adminLevel]) {
      case LEVEL_ADMIN3:
        strcat(buffer,__(" {ff0000} Administrator RCON{ffffff} \n"));
      case LEVEL_ADMIN2:
         strcat(buffer,__(" {ff0000} Administrator{ffffff} \n"));
      case LEVEL_ADMIN1:
         strcat(buffer,__(" {ff0000} Administrator rekrut{ffffff} \n"));
      case LEVEL_GM:
         strcat(buffer,__(" {FF6600} Moderator{ffffff} \n"));
    }
    if (pData[targetid][vipEnabled])
      strcat(buffer, __("{FFD700} Gracz V.I.P{ffffff} \n"));

    if (pData[targetid][accountID]>0)  {
      format(buffer, sizeof buffer,__("%s\n{d0d0d0}Respekt: {ffffff}%d\n{d0d0d0}Skill: {ffffff}%d\n{d0d0d0}GamePoints: {ffffff}%d\n"), buffer, pData[targetid][respect], pData[targetid][skill], pData[targetid][gamep]);
    } else
      strcat(buffer, __("Gracz niezarejestrowany\n"));
  }
  if (pData[targetid][gang]!=NO_GANG) {
    strcat(buffer," \n{ffffff}");
    switch (pData[targetid][gangRank]) {
      case GANG_RANK_MEMBER:
        strcat(buffer,__(" Czlonek "));
      case GANG_RANK_OWNER:
        strcat(buffer,__(" Zalozyciel "));
            case GANG_RANK_VICEOWNER:
        strcat(buffer,__(" Wspolzalozyciel "));
      case GANG_RANK_LEADER:
        strcat(buffer,__(" Lider "));
      case GANG_RANK_SUSPENDED:
        strcat(buffer,__(" Zawieszony czlonek "));
    }
    format(buffer, sizeof buffer,"%s {%06x} %s \n \n", buffer, gData[pData[targetid][gang]][gColor], gData[pData[targetid][gang]][name]);

  }
  format(buffer,sizeof buffer,__("%s{d0d0d0}Ping sredni/biezacy:  {ffffff}%dms/%dms\n"),buffer,pData[targetid][averagePing],GetPlayerPing(targetid));
  format(buffer,sizeof buffer,__("%s{d0d0d0}Plugin audio: %s \n"),buffer, ((Audio_IsClientConnected(targetid))?(__("{30ff30} TAK")):(__("{ff3030} NIE"))));

  ShowPlayerDialog(playerid,DIALOG_PLAYER_INFO,DIALOG_STYLE_MSGBOX,__("Informacje o graczu"),buffer,"OK","");

  return 1;
}

public ShowAnnouncement(params[]){
  if(isnull(params) || !CheckTildes(params)) return 0;
  unpliterki(params);

  KillTimer(gmTemp[timerAnnounce]);
  KillTimer(gmTemp[timerAnnounceEnd]);

  if(gmTemp[ann_effect_mode] == ANN_MODE_ON || gmTemp[ann_effect_mode] == ANN_MODE_FADE_IN || gmTemp[ann_effect_mode] == ANN_MODE_FADE_OUT)
  {
    AnnounceFadeOutStart();
    gmTemp[ann_switchToNew] = true;
    copy(params, gmTemp[ann_switchToNewText]);
  }
  else
  {
    gmTemp[ann_effect_alpha] = 1;
    gmTemp[ann_effect_mode] = ANN_MODE_FADE_IN;

    gmTemp[timerAnnounce] = SetTimer("AnnounceFade", 70, true);
    TextDrawSetString(gTextDraw[TD_ANNOUNCE], params);

  }
  return 1;
}

public ShowAnnouncement2(params[]){
  if(isnull(params) || !CheckTildes(params)) return 0;
  if (gmTemp[ann2_step]>0) {  // juz jest jakis TD wyswietlany
    KillTimer(gmTemp[timerAnnounce2]);
  }
  unpliterki(params);

  gmTemp[ann2_step]=1;
  TextDrawSetString(gTextDraw[TD_ANN2], params);
  gmTemp[timerAnnounce2]=SetTimer("FadeAnnouncement2",150,false);

  return 1;
}

public FadeAnnouncement2(){
  if (gmTemp[ann2_step]==8) {
    gmTemp[ann2_step]=0; TextDrawHideForAll(gTextDraw[TD_ANN2]); return;
  }
  if (gmTemp[ann2_step]<=4)
    TextDrawColor(gTextDraw[TD_ANN2],0xffffff00+(63*gmTemp[ann2_step]));
  else
    TextDrawColor(gTextDraw[TD_ANN2],0xffffff00+(63*(8-gmTemp[ann2_step])));
  foreach(playerid)
    if (pData[playerid][hudSetting][HUD_ANNOUNCEMENTS] && pData[playerid][pAttraction]!=A_RACE)
      TextDrawShowForPlayer(playerid,gTextDraw[TD_ANN2]);

  gmTemp[ann2_step]++;
  gmTemp[timerAnnounce2]=SetTimer("FadeAnnouncement2",150+((gmTemp[ann2_step]==5)?10000:0),false);
  return;
}

KickPlayerWithReason(targetplayerid,playerid,reason[]){
  new buffer[127];
  new buffer2[127];

  new bool:kobietat=Kobieta(targetplayerid);
  new bool:in1line;
  if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN) {
    format(buffer, sizeof buffer, "{b}%s{/b} zostal%s wyrzucon%s z serwera: {b}%s{/b}",
        GetPlayerNick(targetplayerid), kobietat?("a"):(""), kobietat?("a"):("y"), reason);
  } else {
    format(buffer, sizeof buffer, "{b}%s{/b} zostal%s wyrzucon%s z serwera przez admin%s {b}%s{/b}",
        GetPlayerNick(targetplayerid), kobietat?("a"):(""), kobietat?("a"):("y"), Kobieta(playerid)?("ke"):("a"), GetPlayerProperName(playerid));
  }

  if (strlen(buffer)+24+strlen(reason)<127) {
    strcat(buffer,": {b}");
    strcat(buffer,reason);
    in1line=true;
  } else {
    format(buffer2, sizeof buffer2, __("Powod: {b}%s"), reason);
    in1line=false;
  }

  foreach(i){
    Msg(i, COLOR_INFO2, buffer);
    if (!in1line)
      Msg(i, COLOR_INFO2, buffer2);
  }

  if (Audio_IsClientConnected(targetplayerid))
    Audio_Play(targetplayerid,AUDIOID_KICK, false, false, true);

    GivePlayerScore(targetplayerid,-50,"kick");

  KickPlayer(targetplayerid);
  return 1;
}

LogConnection(playerid){
  new buffer[255],szIP[16], escaped_ip[20],serial[50];
  GetPlayerIp(playerid, szIP, sizeof szIP);
  mysql_real_escape_string(szIP,escaped_ip);
  gpci(playerid, serial, sizeof serial);

  format(buffer,sizeof buffer,"UPDATE fs_players SET datetime_last=NOW(),login_count=login_count+1,ip_last='%s',serial_last='%s',active_server=%d WHERE id=%d",escaped_ip,serial,SERVER_NUM,pData[playerid][accountID]);
  mysql_query(buffer,SQL_RI_BACKGROUND);
  return 1;
}

FetchPlayerAccountData(playerid){
  new buffer[512];
  format(buffer,sizeof buffer,"SELECT spawnData,selectedWeap,wallet_money,IFNULL(DATEDIFF(vip,NOW()),'-1'),IFNULL(vip,'0000-00-00'),level,respect,gamep,kill_count,teamkill_count,death_count,suicide_count,skill,hitman_prize,IFNULL(zaloz,'') zaloz FROM fs_players WHERE id=%d LIMIT 1", pData[playerid][accountID]);
  mysql_query(buffer);
  mysql_store_result();
  if(mysql_num_rows()>0) {
    mysql_fetch_row_format(buffer,"|");
    sscanf(buffer,"p<|>dddds[11]ddddddddds[32]",pData[playerid][spawnData],pData[playerid][selectedWeap],pData[playerid][pmoney],pData[playerid][vipDaysLeft],pData[playerid][vipToDate],
          pData[playerid][allowedLevel],pData[playerid][respect],pData[playerid][gamep],
          pData[playerid][kills],pData[playerid][teamkills],pData[playerid][deaths],pData[playerid][suicides],
          pData[playerid][skill],pData[playerid][hitman], pData[playerid][pDzaloz]);
    SetPlayerMoney(playerid,pData[playerid][pmoney]);
    pData[playerid][vipEnabled]=(pData[playerid][vipDaysLeft]>=0) ? (true): (false);
  }
  mysql_free_result();
  return;
}

PlayerQuickbuyWeapon(playerid,listitem){
  new buf[127];
  new nlistitem=0;

  for (new i=0;(i<sizeof quickbuyWeapons && nlistitem<listitem);i++)
    if (quickbuyWeapons[i][ewd_quickBuy]) nlistitem++;
  listitem=nlistitem+1;

  if (!(quickbuyWeapons[listitem][ewd_id]>0))
    return Msg(playerid, COLOR_ERROR, __("Niestety, ta bron jest obecnie niedostepna"));
  if (pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid, COLOR_ERROR, __("Nie mozesz obecnie kupowac broni!"));
  if (GetPlayerMoney(playerid)<quickbuyWeapons[listitem][ewd_baseCost])
    return Msg(playerid, COLOR_ERROR, __("Nie masz wystarczajacej ilosc gotowki!"));
  SetPlayerMoney(playerid, GetPlayerMoney(playerid)-quickbuyWeapons[listitem][ewd_baseCost]);
  format(buf,sizeof buf,__("Kupiles/-as {b}%s{/b} za {b}%d{/b}$."), quickbuyWeapons[listitem][ewd_PLname], quickbuyWeapons[listitem][ewd_baseCost]);
  Msg(playerid,COLOR_INFO, buf);

  if (quickbuyWeapons[listitem][ewd_id]<=15 || quickbuyWeapons[listitem][ewd_id]==49) { // bronie biale/spadochron
    GivePlayerWeapon(playerid,quickbuyWeapons[listitem][ewd_id],1);
    SetPlayerAmmo(playerid,quickbuyWeapons[listitem][ewd_id],1);
  }
    else
  GivePlayerWeapon(playerid,quickbuyWeapons[listitem][ewd_id],quickbuyWeapons[listitem][ewd_baseAmmo]);

  new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  return 1;
}

public UsunPickup(pickupid){
  if (IsValidDynamicPickup(pickupid))
    DestroyDynamicPickup(pickupid);
  return;
}

RefreshConfiguration(){
  if (!mysql_query("SELECT option_name,value FROM fs_" #DB_CONFIG))
  {
    printf(" (4) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
    fread(File:EXIT, gstr); // crash server asap!
    return 0;
  }
  new buffer[1023],option_name[33],value[512];
  mysql_store_result();
  while(mysql_fetch_row_format(buffer,"|")) {
    if (sscanf(buffer,"p<|>s[32]s[511]",option_name,value)) continue;
    if (strcmp(option_name,"admin_password")==0) {
      copy(value, gmTemp[AdminPasswordHash]);
    }
  }
  mysql_free_result();
  return 1;
}

ShowWeaponSelectMenu(playerid, title[], callback[32], restricted_slot=7, restricted_wepid=16){
  new listitems[1024], buffer[128];
  for(new i=0; i<sizeof quickbuyWeapons; i++) {
    if (restricted_slot>=0 && quickbuyWeapons[i][ewd_slot]==restricted_slot) continue;
    if (restricted_wepid>=0 && quickbuyWeapons[i][ewd_id]==restricted_wepid) continue;
    if(strfind(callback, "solo_wybranaBron", true) != -1 && (quickbuyWeapons[i][ewd_id]==46 || quickbuyWeapons[i][ewd_slot]==7)) continue;
    format(buffer, sizeof buffer, "{000000}%02d {ffffff}%s", quickbuyWeapons[i][ewd_id], quickbuyWeapons[i][ewd_PLname]);
    if (i>0) strcat(listitems, "\n");
    strcat(listitems, buffer);
  }
  copy(callback, pTemp[playerid][dialogCB]);
  ShowPlayerDialog(playerid,DIALOG_WEAPON_SELECT,DIALOG_STYLE_LIST,title,listitems,"Wybierz", "Anuluj");
  return 1;
}

OnPlayerChangeWeapon(playerid,oldwepid, wepid){
  #pragma unused oldwepid
  if (!IsPlayerSpawned(playerid)) return;
  if (pTemp[playerid][disableWeaponCheck]) return;
  if (IsPlayerNPC(playerid)) return;

  new buf[128];

  if (oldwepid==22)
    RemovePlayerAttachedObject(playerid,3);

  if (wepid==22) {  // pistolet 9mm
    SetPlayerAttachedObject( playerid, 3, 18643, 6, 0.156490, 0.027780, 0.103976, 335.000000, 350.000000, 0.000000, 0.600000, 0.600000, 0.6 ); // LaserPointer1 - laser
  }

  if (pData[playerid][adminLevel]==LEVEL_ADMIN3) return;

  new ammo,wep;
  GetPlayerWeaponData(playerid,0,wep,ammo);
  new ammo_knife,wep_knife;
  GetPlayerWeaponData(playerid,1,wep,ammo);

  if(wepid==44 || wepid==45) {  // gogle
    format(buf, sizeof buf, __("WYKRYTY WH: {b}%s{/b}(%d) ma gogle/noktowizor."), GetPlayerNick(playerid), playerid);
    MSGToAdmins(COLOR_INFO2, buf, true);
    SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,wepid);
  } else if(wep==0 && (ammo==1000 || ammo==10000)) {  //aim
    format(buf, sizeof buf, __("WYKRYTY AIM/WH: {b}%s{/b}(%d) ma zbyt duzo piesci."), GetPlayerNick(playerid), playerid);
    MSGToAdmins(COLOR_INFO2, buf, true);
  } else if(wep_knife==4 && ammo_knife==7) {  //aim
    format(buf, sizeof buf, __("WYKRYTY AIM/WH: {b}%s{/b}(%d) ma zbyt duzo nozy."), GetPlayerNick(playerid), playerid);
    MSGToAdmins(COLOR_INFO2, buf, true);
  } else if (wepid==36) { // hs rocket launcher
    format(buf, sizeof buf, __("WYKRYTY WH: {b}%s{/b}(%d) ma RPG z naprowadzaniem (niedostepne w grze!)."), GetPlayerNick(playerid), playerid);
    MSGToAdmins(COLOR_INFO2, buf, true);
    SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,wepid);
  } else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==16 && pData[playerid][respect]<75000 && pData[playerid][adminLevel]<LEVEL_ADMIN1 && pTemp[playerid][onArena]!=ARENA_DM && pTemp[playerid][onArena]!=ARENA_CS && !(IsPlayerInFullDMArea(playerid))) {  // granaty
    SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,wepid);
  } else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==18 && pData[playerid][respect]<40000 && pData[playerid][adminLevel]<LEVEL_ADMIN1 && pTemp[playerid][onArena]!=ARENA_DM && !(IsPlayerInFullDMArea(playerid))) {  // molotov
    SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,wepid);
  } else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==37 && GetPlayerInterior(playerid)!=11 && !(IsPlayerInFullDMArea(playerid))) { // flame thrower   oprocz int kanalow TODO dokladniejsze sprawdzenie
    SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,wepid);
  } else if (!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==35 && pData[playerid][adminLevel]<LEVEL_ADMIN1 && !(IsPlayerInFullDMArea(playerid)) && pTemp[playerid][onArena]!=ARENA_RPG) { // RPG
    SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,wepid);
  } else if(!pTemp[playerid][protkill] && !gmTemp[protAll] && wepid==38 && pData[playerid][adminLevel]<LEVEL_ADMIN1 && pTemp[playerid][onArena]!=ARENA_MINIGUN && !(IsPlayerInFullDMArea(playerid)) && pTemp[playerid][aChowanySide] != A_CHOWANY_SEARCHING) {    // minigun
        SetPlayerArmedWeapon(playerid,0);
    RemovePlayerWeapon(playerid,38);
  }
}

MSGToAdmins(msgcolor, msg[], bool:msgsound=false, minLevel=LEVEL_ADMIN2, maxLevel=LEVEL_ADMINHIDDEN)
{
  if (gmTemp[pAdminCount]==0) return;

  foreach(i)
    if (pData[i][adminLevel]>=minLevel && pData[i][adminLevel]<=maxLevel)
      Msg(i,msgcolor,msg, msgsound);
  return;
}

forward explode_After(playerid,userid,step);
public explode_After(playerid,userid,step){
  new Float:AR, Float:HP;
  GetPlayerHealth(userid,HP);
  GetPlayerArmour(userid,AR);
  format(gstr, sizeof gstr, "%03.1f HP / %03.1f AR", HP, AR);
  GameTextForPlayer(playerid, gstr, 350, 5);
  if (step>0){
      step--;
    SetTimerEx("explode_After", 500, false, "ddd", playerid, userid,step);
  }
  return 1;
}

IncreasePlayerHitman(playerid,amount){
  if (amount<0) return;
  pData[playerid][hitman]+=amount;
  UpdatePlayerFactionName(playerid);
  return;
}

UpdatePlayerFactionName(playerid){
  if (!IsPlayerConnected(playerid)) return;
    // ranga
  if (pData[playerid][adminLevel]==LEVEL_ADMIN3)
    copy("{ee0000}RCON", gstr);
  else if (pData[playerid][adminLevel]>=LEVEL_ADMIN1 && pData[playerid][adminLevel]<=LEVEL_ADMIN2)
    copy("{ff0000}ADM", gstr);
  else if (pData[playerid][adminLevel]==LEVEL_GM)
    copy("{FD5E00}GM", gstr);
  else if (pData[playerid][vipEnabled])
    copy("{FFFF00}VIP", gstr);
  else
    copy("", gstr);
  // gang
  if (pData[playerid][gang]!=NO_GANG) {
    if (strlen(gstr)>0) strcat(gstr," {ffffff}| ");
    format(gstr, sizeof gstr, "%s{%06x}%s", gstr, gData[pData[playerid][gang]][gColor], gData[pData[playerid][gang]][name]);
  }
  // hitman
  if (pData[playerid][hitman]>0 || pTemp[playerid][faction]!=FACTION_NONE) {
    if (strlen(gstr)>0) strcat(gstr,"\n");
    if (pTemp[playerid][faction]!=FACTION_NONE)
      format(gstr, sizeof gstr, "%s{%06x}%s%s", gstr, Factions[pTemp[playerid][faction]][ef_color], Factions[pTemp[playerid][faction]][ef_name], pData[playerid][hitman]>0?(" {ffffff}| "):(""));
    if (pData[playerid][hitman]>0)
      format(gstr, sizeof gstr, "%s{b02020}%d$", gstr, pData[playerid][hitman]);
  }

  if(pTemp[playerid][godMode]){
      if (strlen(gstr)>0) strcat(gstr,"\n");
    format(gstr, sizeof gstr, "%s{01a05f}%sTryb pasywny", gstr);
  }
  UpdateDynamic3DTextLabelText(pTemp[playerid][FactionName], 0xffffffff, gstr);

}

GivePlayerNormalWeapons(playerid,bool:armor=true){
  if(armor){
    if (pData[playerid][respect]>=25 && pData[playerid][respect]<1500)
      SetPlayerArmour(playerid,33.0);
    if (pData[playerid][respect]>=1500 && pData[playerid][respect]<5000)
      SetPlayerArmour(playerid,50.0);
    else if (pData[playerid][respect]>=5000 && pData[playerid][respect]<15000)
      SetPlayerArmour(playerid,75.0);
    else if (pData[playerid][respect]>=15000)
      SetPlayerArmour(playerid,100.0);

    if(pTemp[playerid][godMode] && pData[playerid][pAttraction]==A_NONE){
        SetPlayerHealth(playerid,99999.0);
        SetPlayerArmour(playerid,99999.0);
        return 1;
    }
  }

  new skin=GetPlayerSkin(playerid);
  if (pData[playerid][respect]<10000) {
    if (SkinKobiecy(skin)) {
      GivePlayerWeapon(playerid, 14,1); // kwiaty 10
      SetPlayerAmmo(playerid, 14, 1);
    } else {
      GivePlayerWeapon(playerid, 1, 1); // kastet 0
      SetPlayerAmmo(playerid, 1, 1);
    }
  } else {
    GivePlayerWeapon(playerid,4,1);     // noz, slot 1
    SetPlayerAmmo(playerid, 4, 1);
  }

  if (pTemp[playerid][weaponSkill_pistol]>500)
    GivePlayerWeapon(playerid, 22+random(1), 1000); // desert eagle
  else
    GivePlayerWeapon(playerid, 24, 1000); // desert eagle

  if (pTemp[playerid][weaponSkill_uzi]>500)
    GivePlayerWeapon(playerid, 32, 1000);   // uzi
  else
    GivePlayerWeapon(playerid, 29, 1000);   // mp5

  if (pTemp[playerid][weaponSkill_sawnoff]>500)
    GivePlayerWeapon(playerid, 26, 200);  // shotgun
  else if (pData[playerid][respect]>=40000)
    GivePlayerWeapon(playerid, 27, 1000); // combat shotgun
  else if (pData[playerid][respect]>=1000)
    GivePlayerWeapon(playerid, 25, 200);  // shotgun

  if (pData[playerid][respect]>=2000) GivePlayerWeapon(playerid, 31,  3000);  // m4
  if (pData[playerid][respect]>=7000) GivePlayerWeapon(playerid, 34,  300); // sniperka

  if (pData[playerid][respect]>=75000){ // dajemy graczowi wybor (2 molotov, 1 granat, 0 brak)!
      if(pData[playerid][selectedWeap]==2){
      GivePlayerWeapon(playerid, 18,  15);  // molotov
    }else if(pData[playerid][selectedWeap]==1){
        GivePlayerWeapon(playerid, 16,  15);  // granat
    }
  }else if (pData[playerid][respect]>=40000){
      if(pData[playerid][selectedWeap]>0){ // ignorujemy granaty!
      GivePlayerWeapon(playerid, 18,  15);  // molotov
    }
  }
  return 1;
}

isPlayerMuted(playerid){
  new buffer[256];
  if(pData[playerid][accountID]>0){
    format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) pozostalo,reason FROM fs_mutes \
      WHERE player_muted = %i \
      AND date_end > NOW() \
      AND date_created < NOW() \
      LIMIT 1",pData[playerid][accountID]
    );

    mysql_query(buffer);
    mysql_store_result();

    if(mysql_num_rows())
    {
        new tmpExpireTime,tmpMuteReason[100],tempPeriod;
      mysql_fetch_row_format(buffer,"|");// aqq
      sscanf(buffer,"p<|>dS(brak powodu)[99]", tmpExpireTime, tmpMuteReason);

      GetOptimalTimeUnit(tmpExpireTime, tempPeriod);
      pTemp[playerid][mExpireTime]=tmpExpireTime;
      format(pTemp[playerid][mReason], 100, "%s", tmpMuteReason);
      format(pTemp[playerid][mPeriod], 10, "%s", GetPeriodName(playerid, tempPeriod, tmpExpireTime));
      mysql_free_result();
      return true;
    }
    mysql_free_result();
  }else{
    if(pTemp[playerid][mute] - (GetTickCount() / 1000) > 0 && pTemp[playerid][mute] != 0) return true;
  }
  return false;
}

checkDoubleMode(playerid){
  new buffer[256];
  if(pData[playerid][accountID]>0){
    format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(SECOND, NOW(), doubleMode) pozostalo FROM fs_players \
      WHERE id = %i \
      AND doubleMode > NOW() \
      LIMIT 1",pData[playerid][accountID]
    );

    mysql_query(buffer);
    mysql_store_result();

    if(mysql_num_rows())
    {
        new tmpExpireTime,tempPeriod,tttmp[128];
      mysql_fetch_row_format(buffer,"|");// aqq
      sscanf(buffer,"p<|>d", tmpExpireTime);

      GetOptimalTimeUnit(tmpExpireTime, tempPeriod);
      format(tttmp,sizeof tttmp,"[INFORMACJA] Tryb furii konczy sie za {b}%d %s{/b}",tmpExpireTime,GetPeriodName(playerid, tempPeriod, tmpExpireTime));
      Msg(playerid,COLOR_INFO3,tttmp);
      mysql_free_result();
      pTemp[playerid][doubleMode]=true;
      return true;
    }
    mysql_free_result();
  }else{
    if(pTemp[playerid][doubleMode]) {
            Msg(playerid,COLOR_INFO3,"[INFORMACJA] Tryb furii jest aktywny do konca sesji gry!");
      return true;
    }
  }
  return false;
}

public checkBan(playerid){
    new
   buffer[256];

    new PlayerName[32];
    GetPlayerName(playerid,PlayerName,sizeof(PlayerName));

  new serial[50];
  gpci(playerid, serial, sizeof serial);

  new IP[32];
    GetPlayerIp(playerid,IP,sizeof(IP));

    format(buffer, sizeof buffer, "SELECT ip_class, nick_exc, exception FROM %s \
    WHERE '%s'=serial \
    LIMIT 1",
  gmData[DB_sbans], serial);

  if (!mysql_query(buffer)){
    printf(" (5) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
    fread(File:EXIT, gstr); // crash server asap!
    return 0;
  }else {
    mysql_store_result();

    if(mysql_num_rows())
    {
      mysql_fetch_row_format(buffer,"|");
        new ip_class[16],nick_format[512],exception,nick_exc[15][32];
        sscanf(buffer,"p<|>s[16]s[512]d",ip_class,nick_format,exception);
        sscanf(nick_format,"p<;>s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]s[32]",
        nick_exc[0],nick_exc[1],nick_exc[2],nick_exc[3],nick_exc[4],nick_exc[5],nick_exc[6],nick_exc[7],nick_exc[8],nick_exc[9],nick_exc[10],nick_exc[11],nick_exc[12],nick_exc[13],nick_exc[14]);

            if(strfind(IP, ip_class, true) != -1 && strfind(PlayerName, nick_exc[0], true) == -1
      && strfind(PlayerName, nick_exc[1], true) == -1 && strfind(PlayerName, nick_exc[2], true) == -1
      && strfind(PlayerName, nick_exc[3], true) == -1 && strfind(PlayerName, nick_exc[4], true) == -1
      && strfind(PlayerName, nick_exc[5], true) == -1 && strfind(PlayerName, nick_exc[6], true) == -1
      && strfind(PlayerName, nick_exc[7], true) == -1 && strfind(PlayerName, nick_exc[8], true) == -1
      && strfind(PlayerName, nick_exc[9], true) == -1 && strfind(PlayerName, nick_exc[10], true) == -1
      && strfind(PlayerName, nick_exc[11], true) == -1 && strfind(PlayerName, nick_exc[12], true) == -1
      && strfind(PlayerName, nick_exc[13], true) == -1 && strfind(PlayerName, nick_exc[14], true) == -1) {

        SCM(playerid,-1,"");
        format(buffer, sizeof buffer, __("Twoj SERIAL jest zbanowany!"));
        Msg(playerid, 0x900A00FF, buffer);

        Msg(playerid, COLOR_INFO2, __("Jezeli uwazasz, ze to pomylka, zglos to do administratora lub napisz prosbe o odbanowanie na {b}FullServer.eu{/b}"));

        format(buffer, sizeof buffer, __("Gracz {b}%s{/b} nie dolaczyl z powodu aktywnego bana na SERIAL"), GetPlayerNick(playerid));
        MSGToAdmins(COLOR_INFO2, buffer, false, LEVEL_ADMIN1);

        format(buffer, sizeof buffer, __("Gracz %s [IP: %s] nie dolaczyl z powodu aktywnego bana na SERIAL"), GetPlayerNick(playerid), IP);
        printf(buffer);

        if(exception==0) AC_BanEx(playerid,"Ban na SERIAL");
        else Kick(playerid);
        mysql_free_result();
        return 1;
      }
    }
    mysql_free_result();
  }

  format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) FROM %s \
    WHERE '%s' LIKE ip \
    AND date_end > NOW() \
    AND date_created < NOW() \
    LIMIT 1",
  gmData[DB_ipbans], GetPlayerIP(playerid));

  if (!mysql_query(buffer))
  {
    printf(" (6) MYSQL CONNECTION CHECKING RETURNED 0! SERVER IS GOING DOWN NOW TO PREVENT SYSTEM DAMAGE!");
    fread(File:EXIT, gstr); // crash server asap!
    return 0;
  }
  else {
    mysql_store_result();

    if(mysql_num_rows())
    {
      new
       tempPeriod,banExpireTime;

      mysql_fetch_row_format(buffer);
      banExpireTime = StringToInt(buffer);

      GetOptimalTimeUnit(banExpireTime, tempPeriod);

            SCM(playerid,-1,"");
      format(buffer, sizeof buffer, __("Twoj adres IP jest zbanowany na %d %s."), banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
      Msg(playerid, 0x900A00FF, buffer); // Twój adres IP jest zbanowany na xxx minut/godzin.

      Msg(playerid, COLOR_INFO2, __("Jezeli uwazasz, ze to pomylka, zglos to do administratora lub napisz prosbe o odbanowanie na {b}FullServer.eu{/b}"));

      format(buffer, sizeof buffer, __("Gracz {b}%s{/b} nie dolaczyl z powodu aktywnego bana na IP {b}+%d %s"), GetPlayerNick(playerid), banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
      MSGToAdmins(COLOR_INFO2, buffer, false, LEVEL_ADMIN1);

      format(buffer, sizeof buffer, __("Gracz %s [IP: %s] nie dolaczyl z powodu aktywnego bana na IP {b}+%d %s"), GetPlayerNick(playerid), IP, banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
      printf(buffer);

      KickPlayer(playerid);
      mysql_free_result();
      return 1;
    }
    mysql_free_result();
  }

  format(buffer, sizeof buffer, "SELECT ip_class FROM %s",gmData[DB_iplocks]);

  mysql_query(buffer);
  mysql_store_result();
  new pip[16];
  while (mysql_fetch_row_format(pip,"|")){
    if(pData[playerid][accountID]<=0 && strfind(IP, pip, true) == 0){
        SCM(playerid,-1,"");
      Msg(playerid,COLOR_ERROR,"Z powodów bezpieczeñstwa tworzenie nowych kont z Twojego adresu IP zosta³o zablokowane! Utwórz konto z innego adresu IP.");
      printf("PLAYER %d USING IP %s IS LOCKED DUE TO %s BAN",playerid,IP,pip);
      KickPlayer(playerid);
      mysql_free_result();
      return 1;
    }
  }
  mysql_free_result();

  return 1;
}

Float:GetOptimumRampDistance(playerid) // poprawic aby uwzglednialo predkosc
{
  new ping = GetPlayerPing(playerid), Float:dist;
  dist = floatpower(ping, 0.25);
  dist = dist*4.0;
  dist = dist+5.0;
  return dist+20;
}

Float:GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance)
{
  new Float:a;
  GetPlayerPos(playerid, x, y, a);
  if (IsPlayerInAnyVehicle(playerid)) GetVehicleZAngle(GetPlayerVehicleID(playerid), a);
  else GetPlayerFacingAngle(playerid, a);
  x += (distance * floatsin(-a, degrees));
  y += (distance * floatcos(-a, degrees));
  return a;
}

forward RemoveRamp(playerid);
public RemoveRamp(playerid)
{
  if (pTemp[playerid][rampId] != -1) {
      DestroyPlayerObject(playerid,pTemp[playerid][rampId]);
      pTemp[playerid][rampId] = -1;
  }
}

stock funcRejestracja(playerid, params[])
{
  if(pData[playerid][loggedIn]) { Msg(playerid, COLOR_ERROR, TXT(playerid, 0)); return 0; }
  if(PlayerAccountExists(playerid)) { Msg(playerid, COLOR_ERROR, TXT(playerid, 1)); return 0; }

  new
    buffer[512], //350 to za malo po dodaniu emaila!
    szIP[16],
    szDT[18],
    date[3],
    time[3],
    escaped_nick[MAX_PLAYER_NAME+8],
    mail[56],
    pass[56],
    escaped_mail[56+8];

  if (sscanf(params,"s[32]s[56]",pass,mail)) { Msg(playerid, COLOR_ERROR, TXT(playerid, 2)); return 0; }
  if(!IsPasswordCorrect(pass)) { Msg(playerid, COLOR_ERROR, TXT(playerid, 3)); return 0; }
  if(!IsValidEmail(mail)) { Msg(playerid, COLOR_ERROR, "Prosz?poda?poprawny adres email w formie u¿ytkownik@host.domena"); return 0; }
  if(EmailExists(mail)) { Msg(playerid, COLOR_ERROR, "Na ten adres email jest juz zarejestrowane inne konto. Je¿eli zapomnia³e?swojego has³a u¿yj mo¿liwoœci przypominania has³a na forum!"); return 0; }

  mysql_real_escape_string(GetPlayerNick(playerid),escaped_nick);
  mysql_real_escape_string(mail,escaped_mail);
  GetPlayerIp(playerid, szIP, sizeof szIP);
  getdate(date[2], date[1], date[0]);
  gettime(time[0], time[1], time[2]);
  format(szDT, sizeof szDT, "%i %i %i %i %i", date[2], date[1], date[0], time[0], time[1]);
  new serial[50];
  gpci(playerid, serial, sizeof serial);
  
  GivePlayerMoney(playerid,ON_START_MONEY);
  GivePlayerScore(playerid,25,"Rejestracja w grze");

  format(buffer, sizeof buffer, "INSERT INTO %s ( nick, password, email, ip_registered, ip_last, serial_registered, serial_last, datetime_registered, datetime_last, respect, skill, gamep, bank_money, wallet_money, active_server, spawnData ) VALUES\
  ( '%s', '%s', '%s', '%s', '%s', '%s', '%s', NOW(), NOW(), %i, %i, 15, %i, %i, %d, 5 )",
    gmData[DB_players],
    escaped_nick,             // Nick
    MD5_Hash(PassHash(GetPlayerNick(playerid), pass)),  // Password
    escaped_mail,
    szIP,                         // IP
    szIP,                         // IP
    serial,
    serial,
    pData[playerid][respect],
    pData[playerid][skill],
    ON_START_MONEY,
    GetPlayerMoney(playerid),      // Money to wallet
    SERVER_NUM  // zalezny od LANG_PL/LANG_EN ustawionego przy kompilacji!
  );

  mysql_query(buffer);
  ShowPlayerAchievement(playerid,__("Zarejestrowany gracz!"));
  OnPlayerLogin(playerid);

  format(buffer, sizeof buffer, "Konto {b}%s{/b} z adresem email {b}%s{/b} utworzone. Twoje haslo: {b}%s{/b}", GetPlayerProperName(playerid), mail, pass);
  Msg(playerid, COLOR_INFO, buffer); // Konto o nazwie "%s" zosta³o utworzone, zapamiêtaj swoje has³o: %s
  if (Kobieta(playerid))
    Msg(playerid, COLOR_INFO, __("Zostalas automatycznie zalogowana, milej gry!"));
  else
    Msg(playerid, COLOR_INFO, __("Zostales automatycznie zalogowany, milej gry!"));

    new PlayerName[32];
    GetPlayerName(playerid,PlayerName,sizeof(PlayerName));

  pTemp[playerid][isFS] = false;
  if(strfind(PlayerName, "[FS]", true) != -1 || strfind(PlayerName, ".fs", true) != -1 || strfind(PlayerName, "fs.", true) != -1
  || strfind(PlayerName, "FS_", true) != -1  || strfind(PlayerName, "_FS", true) != -1){
      pTemp[playerid][isFS] = true;
      if (pData[playerid][pAttraction]==A_NONE && gmData[artefactOwner]!=playerid) SetPlayerProperColor(playerid,true);
  }
    format(pTemp[playerid][properName],MAX_PLAYER_NAME,"%s", PlayerName);

  return 1;
}
// EOF