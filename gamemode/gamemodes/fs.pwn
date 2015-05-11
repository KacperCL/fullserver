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

// SA-MP includes
#include <samp/a_samp>
#include <samp/a_http>

// some pre-defines
#define DYNAMIC_HP
#define SPECTATE_KILLER // czy po smierci gracz ma specowac zabojce
#define SNIPER_HEADSHOOT
#define CJHIT_MODEL // moze powodowac lagi

// 3rd party includes
#include <3rdparty/y_timers>
#include <3rdparty/sscanf2>
#include <3rdparty/streamer>
#include <3rdparty/md5>
#include <3rdparty/dc_cmd>
#include <3rdparty/progress>
#include <3rdparty/audio>
#include <3rdparty/a_mysql>
#include <3rdparty/mapandreas>
#include <3rdparty/regex>
#include <3rdparty/profiler>

// main gamemode includes
#include <fullserver/version>
#include <fullserver/fs_header>
#include <fullserver/money>
#include <fullserver/atms>
#include <fullserver/utility_functions>
#include <fullserver/factions>
#include <fullserver/logging>
#include <fullserver/audio_functions>
#include <fullserver/domy>
#include <fullserver/gangzones>
//#include <fullserver/mo/rakieta>
#include <fullserver/mo/balony>
#include <fullserver/paczki>
#include <fullserver/objects>
#include <fullserver/gangs>
#include <fullserver/scripting_functions>
#include <fullserver/regexp>  // wyrazenia regularne
#include <fullserver/spawns>
#include <fullserver/areny>
#include <fullserver/poczta>
#include <fullserver/timers>
#include <fullserver/vehicles>
#include <fullserver/solo>
#include <fullserver/artefact>
#include <fullserver/textdraws>
#include <fullserver/score>
#include <fullserver/prezenty>
#include <fullserver/gz_wars>
#include <fullserver/jail>
#include <fullserver/warsztat>
#include <fullserver/exports>
#include <fullserver/quiz>
// attraction includes
#include <fullserver/attraction_derby>
#include <fullserver/attraction_race>
#include <fullserver/attraction_drifting>
#include <fullserver/attraction_ctf>
#include <fullserver/attraction_hay>
#include <fullserver/attraction_wg>
#include <fullserver/attraction_chowany>
#include <fullserver/attraction_strzelnica>
#include <fullserver/attraction_gungame>
// commands includes
#include <fullserver/commands_admin>
#include <fullserver/commands_rcon>
#include <fullserver/commands_gm>
#include <fullserver/commands_other>
#include <fullserver/commands_eng>
#include <fullserver/commands_anim>
#include <fullserver/commands_vip>
#include <fullserver/commands_zaloz>

main()
{
}

public OnVehicleMod(playerid,vehicleid,componentid)
{
  if(GetPlayerInterior(playerid) == 0)
  {
    RemovePlayerFromVehicle(playerid);
    DestroyVehicle(vehicleid);
    pTemp[playerid][identStep]++;
    return 0;
  }
  return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
  return 1; // consider adding some checks here? we can return 0 to prevent server from updating internal vehicle pool informations.
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
   return 1;
}

public OnActorStreamIn(actorid, forplayerid)
{
   return 1;
}

public OnActorStreamOut(actorid, forplayerid)
{
   return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
  return 1;
}

public OnGameModeInit()
{
  new
   buffer[200];

  gmTemp[startTime] = GetTickCount();

  SendRconCommand("unbanip *.*.*.*");

  CreateConfigDBIfNotExists();
  LoadConfig();

  // rot13 on rcon_password!
  new rconPass[128];
  GetServerVarAsString("rcon_password", rconPass, sizeof rconPass);
  rot13(rconPass);
  format(rconPass, sizeof rconPass, "rcon_password %s", rconPass);
  SendRconCommand(rconPass);

  Streamer_TickRate(50); //domyslnie jest zawsze 50

  if(!strlen(gmData[DB_hostname]) || !strlen(gmData[DB_username]) || !strlen(gmData[DB_database]))
  {
    printf(" Nie udalo sie wczytac konfiguracji bazy danych. Wylaczanie gamemode");
    fread(File:EXIT, gstr); // crash server asap!
  }
  printf(" Inicjacja i laczenie z baza MySQL...");

  mysql_debug(0);
  new rConn = mysql_connect(gmData[DB_hostname], gmData[DB_username], gmData[DB_database], gmData[DB_password]);
  if(!rConn || !mysql_ping(rConn))
  {
    printf(" Nie udalo sie polaczyc z baza danych");
    fread(File:EXIT, gstr); // crash server asap!
    return 0;
  }
  hMySQL = rConn;

  printf( "\n\n");
  printf( "d88888b db    db db      db      .d8888. d88888b d8888b. db    db d88888b d8888b. ");
  printf( "88'     88    88 88      88      88'  YP 88'     88  `8D 88    88 88'     88  `8D ");
  printf( "88ooo   88    88 88      88      `8bo.   88ooooo 88oobY' Y8    8P 88ooooo 88oobY' ");
  printf( "88~~~   88    88 88      88        `Y8b. 88~~~~~ 88`8b   `8b  d8' 88~~~~~ 88`8b   ");
  printf( "88      88b  d88 88booo. 88booo. db   8D 88.     88 `88.  `8bd8'  88.     88 `88. ");
  printf( "YP      ~Y8888P' Y88888P Y88888P `8888Y' Y88888P 88   YD    YP    Y88888P 88   YD\n\n");
  printf( "        FullServer XyzzyDM v%s, %s\n\n", GMVERSION, GMCOMPILED);
  format(buffer,sizeof buffer,"   Fullserver XyzzyDM v%s", GMVERSION);

  printf(__(" Uruchamianie FullServer DM:"));
  printf(__(" Ladowanie ustawien i elementow glownych ..."));

  SetGameModeText("Polski/PL FullServer DM");
  SendRconCommand("mapname × Full Andreas ×");
  SendRconCommand("language Polish");

  UsePlayerPedAnims();
  DisableInteriorEnterExits();
  EnableStuntBonusForAll(false);
  ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
  ManualVehicleEngineAndLights();

  gmTemp[lastHour] = -1;
  gmTemp[lastDay] = -1;
  gmData[artefactOwner] = INVALID_PLAYER_ID;
  gmTemp[changed_time] = -1;

  gmTemp[forceQuiz]=false;
  gmTemp[isBombs] = true;
  gmTemp[showTroll]=true;
  gmTemp[showGang]=true;
  
  new failSafe = mysql_query("SHOW TABLES;"); // failsafe!!!11111111 DO NOT DELETE111!!!!!!!!
  if(!failSafe || !mysql_ping(hMySQL))
  {
    printf(" Nie udalo sie polaczyc z baza danych");
    fread(File:EXIT, gstr); // crash server asap!
    return 0;
  }
  mysql_store_result();
  mysql_free_result();

  LoadCensoredWords();

  printf(" Ladowanie elementow wizualnych ...");

  CreateTextDraws();

  printf(" Ladowanie jezykow ...");
  LoadLanguages();

  printf(" Ladowanie klas graczy ...");

  AddPlayerClass(292, -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0); // gracze zarejestrowani zobacza zamiast niego swoj wlasny skin

  for(new skinid = 0; skinid <= 311; skinid++)
  {
    switch(skinid)
    {
      case 74:
        continue;
      default:
        if (!SkinKobiecy(skinid)) AddPlayerClass(skinid, -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0);
    }
  }

  for(new skinid = 312; skinid >0; skinid--)
  {
    if (SkinKobiecy(skinid)) AddPlayerClass(skinid, -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0);
  }

  printf(" Inicjalizowanie GangZones...");
  GZ_Init();
  gzw_LoadGangZones();

  printf(" Ladowanie obiektow ...");

  CreateObjects();
  CreateATMs();

  // stolik w wybieralce sf
  CreateObject(1433,-1962.03, 817.68,110.0, 0.0, 0.0, 0.0, 300.0);
  
  printf(" Ladowanie pojazdow ...");

  // statyczne
  vehicles_LoadStatic(0); // pad

  for(new i = 0; i < sizeof rodzaje; i++) {
    format(txt_rodzaje,sizeof txt_rodzaje,"%s\r\n%s", txt_rodzaje, rodzaje[i]);
  }

  printf(" Pobieranie danych gangow ...");
  gangs_LoadGangData();

  printf(" Pobieranie danych domow");
  domy_Reload();

  printf(" Ladowanie pickup-ow, checkpointow, teleporting pickups, returning pickups, i co tam jeszcze XJL wymyslil ...");

  CreatePickups();
  CreateArtefact();
  obiekty_odswiezTeleCheckpoints();
  obiekty_odswiezReturnPickups();
  obiekty_odswiezTelePickups();
  obiekty_odswiezMapIcons();
  obiekty_odswiezMiscPickups();
  zaladujPaczki();

  gmTemp[snoopPM]=0;
  gmTemp[showJoins]=1;

  printf(" Wybor paczki dzwiekowej...");
    Audio_SetPack("fullserver", true);

  printf(" Inicjalizowanie MapAndreas...");
  MapAndreas_Init(MAP_ANDREAS_MODE_FULL);

  printf(" Przygotowywaine atrakcji...");
  Drifting_Init();  // tworzy textdrawy

  printf(" Aktualizacja danych i przygotowywanie do uruchomienia ...");

  gmTemp[popularArena]=3;

  telpos[tpX]=FLOAT_NAN;
  evtp[tpX]=FLOAT_NAN;

  printf(" Pobieranie nazw pojazdow z bazy danych");
  UpdateVehicleNames();

  printf(" Odswiezanie konfiguracji");
  RefreshConfiguration();

  new seth, setm, sets;
  gettime(seth, setm, sets);
  SetWorldTime(seth);

  mysql_query("UPDATE fs_players SET active_server=0 WHERE active_server=" #SERVER_NUM);

  printf(" Uruchamianie petli glownej");
  SetTimer("UpdateTimer", UPDATE_TIMER_TIME, true);

  TimeSync();

  gmData[join_count]=StringToInt(GetServerStat("join_count"));
  gmData[kill_count]=StringToInt(GetServerStat("kill_count"));
  gmData[death_count]=StringToInt(GetServerStat("death_count"));

  printf(" FullServer DM zaladowany pomyslnie (%0.3f sekund)\r\n", float(GetTickCount() - gmTemp[startTime]) / 1000);
  
  Profiler_Start();

  return 1;
}

public OnGameModeExit()
{
  Profiler_Stop();
  Profiler_Dump();

  printf(" Wylaczanie gamemode. Zapisywanie danych graczy i statystyk serwera");
  foreach(playerid){
    if(pData[playerid][loggedIn]){
      UpdatePlayerAccountData(playerid,true);
    }
    Kick(playerid);
  }
  SendRconCommand("unbanip *.*.*.*");
  gangs_saveGangData(true);
  SetServerStatInt("death_count", gmData[death_count]);
  SetServerStatInt("kill_count", gmData[kill_count]);
  SetServerStatInt("join_count", gmData[join_count]);
  printf("Dane graczy i statystyki serwera zapisane");
  //mysql_close(hMySQL);

  return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
  if(IsPlayerNPC(playerid)) return 1;
  // if (pData[playerid][logonDialog]) return 0;
  SyncPlayerGameTime(playerid);

  ShowElement(playerid, TDE_CLOCK, false);
  ShowElement(playerid, TDE_DATETIME, false);
  ShowElement(playerid, TDE_STATS, false);
  ShowElement(playerid, TDE_ATTRACTIONBOX, false);
  ShowElement(playerid, TDE_WYBIERALKA, true);

  if (pTemp[playerid][firstClassSelection]) {
    if (pData[playerid][lastUsedSkin]!=-1 || pData[playerid][accountID]>0)
      Msg(playerid,COLOR_INFO2,__("Wcisnij {b}shift lub enter{/b}, aby wybrac swoj ostatni skin, lub wybierz nowy."), false);
    else
      Msg(playerid,COLOR_INFO,__("Wybierz skin, ktorego chcesz uzywac w grze."));
    Msg(playerid,COLOR_INFO2,__("Przewijaj w lewo aby przegladac skiny kobiece, w prawo aby przegladac skiny meskie."), false);
    Msg(playerid,COLOR_INFO,__("Jesli znasz ID skina, mozesz wpisac {b}/skin id{/b}, aby wybrac go od razu"), false);
    pTemp[playerid][firstClassSelection]=false;
  }
  if (classid==0) {
    if (pData[playerid][lastUsedSkin]!=-1)
      SetPlayerSkin(playerid, pData[playerid][lastUsedSkin]);
    GameTextForPlayer(playerid,__("~n~~n~~n~~n~~n~<< kobiety  I  mezczyzni >>"),2500,5);
  } else {
    new buf[44];
    format(buf,sizeof buf,__("~n~~n~~n~~n~~n~Skin ~y~%d"), GetPlayerSkin(playerid));
    GameTextForPlayer(playerid,buf,3000,5);
  }

  pData[playerid][classSelecting] = true;

  GivePlayerWeapon(playerid,27,1);  // combat shotgun
  SetPlayerArmedWeapon(playerid,27);

  SetPlayerPos(playerid, 371.3095, -1797.1166, 24.9386);  // plaza
  SetPlayerFacingAngle(playerid, 180.0);
  SetPlayerInterior(playerid, 0);
  SetPlayerDrunkLevel(playerid, 2);

  new Float:pvector[3] = {371.3275, -1800.0743, 25.5306};
  pvector[0] += 7.0 * floatsin(float(random(60) - 30), degrees);
  pvector[1] -= float(random(150)) / 150.0;
  pvector[2] += 4.0 * floatsin(float(random(30) - 15), degrees);

  GivePlayerWeapon(playerid,27,1);  // combat shotgun
  SetPlayerArmedWeapon(playerid,27);

  SetPlayerCameraPos(playerid, pvector[0], pvector[1], pvector[2]);
  SetPlayerCameraLookAt(playerid, 371.3275, -1797.0743, 25.2306,CAMERA_MOVE);

  if (Kobieta(playerid)) switch(random(5)) {
    case 0: ApplyAnimation(playerid,"DANCING", "DAN_Down_A", 4.0,  1, 1, 1, 1, 1, 0); // tance
    case 1: ApplyAnimation(playerid, "DANCING", "dance_loop", 4.1, 1, 1, 1, 1, 1, 0);
    case 2: ApplyAnimation(playerid, "DANCING", "DAN_Right_A", 4.1, 1, 1, 1, 1, 1, 0);
    case 3: ApplyAnimation(playerid, "DANCING", "DAN_Up_A", 4.1,  1, 1, 1, 1, 1, 0);
        case 4: ApplyAnimation(playerid, "DANCING", "dnce_M_a", 4.1,  1, 1, 1, 1, 1, 0);
  } else switch(random(3)) {
    case 0: ApplyAnimation(playerid, "SHOTGUN", "shotgun_crouchfire", 4.1, 0, 1, 1, 1, 1, 1); // strzelanie
    case 1: ApplyAnimation(playerid, "SHOTGUN", "shotgun_fire", 4.1, 0, 1, 1, 1, 1, 1);
    case 2: ApplyAnimation(playerid, "SHOTGUN", "shotgun_fire_poor", 4.1, 0, 1, 1, 1, 1, 1);
  }

  PlaySound(playerid, 1132);
  GivePlayerWeapon(playerid,27,1);  // combat shotgun
  SetPlayerArmedWeapon(playerid,27);


  return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    if(IsPlayerNPC(playerid)) return 1;
  if(pData[playerid][logonDialog])
    return 0;

  pData[playerid][classSelecting] = false;

  SetPlayerProperColor(playerid,true);
  TogglePlayerSpectating(playerid, false);
  SetPlayerDrunkLevel(playerid, 0);

  pData[playerid][lastUsedSkin] = GetPlayerSkin(playerid);
  ShowElement(playerid, TDE_WYBIERALKA, false);
  OnPlayerFirstSpawn(playerid);

  return 1;
}

public OnPlayerConnect(playerid)
{
  if(IsPlayerNPC(playerid)) return 1;
  if (playerid>=(MAX_SERVER_PLAYERS-10)) {
      Msg(playerid,COLOR_ERROR,"Programowy limit graczy osiagniety, kod bledu: #AMX_DEFINE_SERVER_FULL");
    KickPlayer(playerid);
    return 0; // nie przekazujemy informacji do skryptow itp
  }

  new serial[50],IP[16],version[10],PlayerName[32];
  gpci(playerid, serial, sizeof serial);
  GetPlayerVersion(playerid,version,sizeof version);
    GetPlayerIp(playerid,IP,sizeof(IP));
    GetPlayerName(playerid,PlayerName,sizeof(PlayerName));

    if (strcmp(version,"0.3.7",true)!=0 && strcmp(version,"0.3.7-R1-2",true)!=0 && strcmp(version,"0.3.7-R2",true)!=0) {
        printf("Zla wersja (%s) klienta SA-MP z IP %s - ZBANOWANY",version, IP);
        BanEx(playerid,"Zla wersja klienta SA-MP");
        return 0;
    }

    new time = gettime();
    new joins;
    for(new i=0;i<10;i++){
        if(LastTenJoinTimes[i] + 10 >= time){
            if(!strcmp(LastTenJoinIps[i],IP,false) && LastTenJoinIps[i][0] != '\0'){
                joins++;
            }
        }
    }
    joins += 1;
    if(joins >= 3){
        // 3 polaczenia z tego samego IP w ciagu 10 sekund
        printf("Prawdopodobny flooder na graczy z IP %s - ZBANOWANY",IP);
    BanEx(playerid,"Prawdopodobny flooder na graczy <zbyt duzo polaczen w ciagu 10 sekund>");
    return 0;
    }

    new index;
    new ttime = time;
    for(new i=0;i<10;i++){
        if(LastTenJoinTimes[i] <= ttime){
            ttime = LastTenJoinTimes[i];
            index = i;
        }
    }
    LastTenJoinTimes[index] = time;
    LastTenJoinIps[index] = IP;

    if(pTemp[playerid][uglyRejoinHack]){ // rejoin na to samo ID bez wysylania onPlayerDisconnect (a.k.a m0d_rejoin by bartekdvd)
        printf("Prawdopodobny m0d_rejoin hack z IP %s - ZBANOWANY",IP);
    BanEx(playerid,"Prawdopodobny m0d_rejoin hack");
    pTemp[playerid][uglyRejoinHack]=false;
    return 0;
  }

  if (strlen(PlayerName)<3 || strlen(PlayerName)>23){
        printf("Nieprawidlowa dlugosc nicku z IP %s - ZBANOWANY",IP);
        BanEx(playerid,"Nieprawidlowa dlugosc nicku");
        return 0;
  }

  // logujemy wszystkie seriale
  printf("SL %s %d %s %s %s", PlayerName, playerid, serial, IP, version);

  if (gmTemp[highestPlayerID]<playerid) gmTemp[highestPlayerID]=playerid; // this should be faster then calling to GetPlayerPoolSize

  pTemp[playerid][uglyRejoinHack]=true;
  
  ////////////////////// restore after crash! /////////////////
  if(!pData[playerid][isCountedForRestore])
  {
    format(pData[playerid][lastNick],32,"%s",PlayerName);
    format(pData[playerid][lastIp],32,"%s",IP);
    format(pData[playerid][lastSerial],48,"%s",serial);
  }
  
  if(pData[playerid][isCountedForRestore]) {
    printf(" INFO: player %d found for restore! tick: %d, disconnectSess: %d, disconnectCHECK: %d",playerid,GetTickCount(),pData[playerid][disconnectSession],(pData[playerid][disconnectSession] + 300000));
  }
  
  pData[playerid][restoredFromLastSession] = false;
  if(pData[playerid][isCountedForRestore] && pData[playerid][disconnectSession] + 300000 >= GetTickCount())
  {
    if(strcmp(pData[playerid][lastNick],PlayerName,false)==0 && strcmp(pData[playerid][lastIp],IP,false)==0 && strcmp(pData[playerid][lastSerial],serial,false)==0){
      pData[playerid][restoredFromLastSession] = true;
    }
  }
  /////////////////////// end restore ///////////////////////////
  if(!pData[playerid][restoredFromLastSession]) pData[playerid][session] = GetTickCount();

  SendClientMessage(playerid,-1," ");
  SendClientMessage(playerid,0xffffffff, "FullServer GM{adc3e7}, ver. " #GMVERSION " " #GMCOMPILED);

  CreatePlayerTextDraws(playerid);

  new buffer[128];

  pData[playerid][spawnData]=5;
  pTemp[playerid][mute]=0;
  pData[playerid][accountID]=0;
  pData[playerid][pmoney]=5000;   // domyslna kasa dla nowych graczy
  pData[playerid][loggedIn] = false;
  pTemp[playerid][loginAttemps] = 0;
  pTemp[playerid][shamalId] = 0;
  pTemp[playerid][fakeKillWarn] = false;
  if(!pData[playerid][restoredFromLastSession]) pTemp[playerid][fRsp] = true;
  if(!pData[playerid][restoredFromLastSession]) pTemp[playerid][gongAll] = false;
  pData[playerid][vipEnabled] = false;
  pData[playerid][vipDaysLeft]=-1;
  pTemp[playerid][watchDmg]=false;
  pTemp[playerid][lastDmg]=0;
  pTemp[playerid][aCTFexit]=false;
  pTemp[playerid][aGGexit]=false;
  pTemp[playerid][lastHealUsed]=GetTickCount();
  format(pData[playerid][vipToDate], 12, "0000-00-00");
  pTemp[playerid][hPaymentType]=0;
  pTemp[playerid][smsType]=0;

  pTemp[playerid][skinBeforeChange]=-1;
  pTemp[playerid][lastSessionSaveTick] = GetTickCount();
  pTemp[playerid][noSkill] = false;
  //pTemp[playerid][audioHandleId] = -1;
  
  pTemp[playerid][lastDeathHeadshottedBy] = INVALID_PLAYER_ID;

  if(!pData[playerid][restoredFromLastSession]) pTemp[playerid][doubleMode]=false;
  if(!pData[playerid][restoredFromLastSession]) pTemp[playerid][godMode]=false;
  pTemp[playerid][lastKillTickCount]=0;
  pTemp[playerid][fastKillWarn]=false;
  if(!pData[playerid][restoredFromLastSession]) pTemp[playerid][rampy]=false;

  pTemp[playerid][identStep]=0;

  pTemp[playerid][isFS] = false;
  if(strfind(PlayerName, "[FS]", true) != -1 || strfind(PlayerName, ".fs", true) != -1 || strfind(PlayerName, "fs.", true) != -1
  || strfind(PlayerName, "FS_", true) != -1  || strfind(PlayerName, "_FS", true) != -1){
      pTemp[playerid][isFS] = true;
  }
    format(pTemp[playerid][properName],MAX_PLAYER_NAME,"%s", PlayerName);

  pData[playerid][gang] = NO_GANG;
  pData[playerid][gangRank] = GANG_RANK_NONE;
  pData[playerid][allowedLevel]=0;
  pData[playerid][adminLevel]=0;
  pData[playerid][suspended]=false;
  pData[playerid][spectating] = INVALID_PLAYER_ID;
  pTemp[playerid][firstSelectingScreen] = true;
  pData[playerid][destroyMyVehicle] = false;
  pData[playerid][currentColor] = gmData[color_normalUser];
  pTemp[playerid][lastMsgTick] = 0;
  pTemp[playerid][spamCount] = 0;
  pData[playerid][kills] = 0;
  pData[playerid][teamkills] = 0;
  pData[playerid][deaths] = 0;
  pData[playerid][suicides] = 0;
  pData[playerid][averagePing] = 0;
  pTemp[playerid][pingChecks] = 0;
  pTemp[playerid][pingSum] = 0;
  pTemp[playerid][pingWarningCount] = 0;
  pData[playerid][respect]=0;
  pData[playerid][skill] = 0;
  pData[playerid][gamep] = 0;
  pTemp[playerid][specPosReturn] = false;
  pData[playerid][pAttraction] = A_NONE;
  pData[playerid][aChowany] = false;
  pData[playerid][aHAY] = false;
  pData[playerid][aDerby] = false;
  pData[playerid][aRace] = false;
  pData[playerid][aDrift] = false;
  pData[playerid][aWG] = false;
  pData[playerid][aStrzelnica] = false;
  pData[playerid][statsShowed] = false;
  pData[playerid][lastUsedSkin] = -1;
  pTemp[playerid][performingAnim] = false;
  pTemp[playerid][hasVoted]=false;
  pTemp[playerid][warningReceived]=0;
  pTemp[playerid][firstClassSelection]=true;
  pTemp[playerid][onArena]=ARENA_NONE;
  pTemp[playerid][pickupDelay]=0;
  if(!pData[playerid][restoredFromLastSession]) pTemp[playerid][bonusHours] = 0;
  pTemp[playerid][miniGame]=MINIGAME_NONE;
  pTemp[playerid][lastPlayerKilled]=INVALID_PLAYER_ID;
  pTemp[playerid][killStreak]=0;
  pTemp[playerid][e_houseid]=-1;
  pTemp[playerid][drunkLevel]=0;
  pTemp[playerid][drunkLevelL]=0;
  pTemp[playerid][playerColorAlpha]=PLAYER_COLOR_ALPHA;
  pTemp[playerid][tmpAlpha]=PLAYER_COLOR_ALPHA;
  pTemp[playerid][TPInv]=0;
  pTemp[playerid][disableWeaponCheck]=false;

  pTemp[playerid][wStrefieNODM]=false;
  pTemp[playerid][wStrefieFULLDM]=false;

  pTemp[playerid][curPos]=1;
  pTemp[playerid][lastPos]=-1;

  pTemp[playerid][cenzura]=false;
  pTemp[playerid][protkill]=false;
  pTemp[playerid][protping]=false;

  pTemp[playerid][troll]=false;
  pTemp[playerid][desync]=false;
  pTemp[playerid][antygl]=0;
  pTemp[playerid][lockedPos]=false;
  pTemp[playerid][weaponsAllowed]=true;
  pTemp[playerid][cheater]=false;

  pTemp[playerid][weaponSkill_pistol]=0;
  pTemp[playerid][weaponSkill_silenced]=0;
  pTemp[playerid][weaponSkill_sawnoff]=0;
  pTemp[playerid][weaponSkill_uzi]=0;
  pTemp[playerid][audio_vehicle]=-1;
  pTemp[playerid][vehicleSpecialLastUsed]=GetTickCount();

  pTemp[playerid][bannedPlayersCnt]=0;
  pTemp[playerid][bannedPlayersRS]=0;
  pTemp[playerid][faction]=FACTION_NONE;
  pTemp[playerid][FactionName]=CreateDynamic3DTextLabel(" ", 0xffffffff, 0, 0, 1, 30, playerid, INVALID_VEHICLE_ID, 1, 0);
  pTemp[playerid][fakeAFK]=false;
  pTemp[playerid][ept_fpswarns]=0;
  pTemp[playerid][seesNametags]=true;

  for(new i=0;i<ARENA_MAX;i++)
    pTemp[playerid][arenaScore][i]=0;

  privpos[playerid][tpX]=FLOAT_NAN;
  privpos2[playerid][tpX]=FLOAT_NAN;
  pTemp[playerid][isFreezed]=false;
  pTemp[playerid][playPlayall]=true;
  pTemp[playerid][bombId]=-1;
  pTemp[playerid][bombPos][0]=-1;
  pTemp[playerid][bombPos][1]=-1;
  pTemp[playerid][bombPos][2]=-1;

  pTemp[playerid][vipHeal]=-1;
  pTemp[playerid][vipArmor]=-1;

  SetPlayerColor(playerid,0x606060FF);
  SetPlayerTeam(playerid, playerid+1);
  if (!LoadBasicPlayerData(playerid)) return 0;

  SetTimerEx("checkBan",1000,false,"d",playerid);

  if(IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]<LEVEL_ADMIN3) // zalogowany na rcona bez rcona w bazie danych!
  {
    format(buffer,sizeof buffer,__("Nieautoryzowane logowanie na admina RCON przez %s (%d)! Wykopany."), GetPlayerNick(playerid), playerid);
    KickPlayer(playerid);
    MSGToAdmins(COLOR_ERROR, buffer, false);
    OutputLog(LOG_SYSTEM, buffer);
    return 0;

  }
  else if (IsPlayerAdmin(playerid) && pData[playerid][allowedLevel]>=LEVEL_ADMIN3)
    pTemp[playerid][isRcon] = true;


  ShowElement(playerid, TDE_WIDE, true);
  ShowElement(playerid, TDE_FULLSERVERLOGO, true);

  if (gmTemp[debugMode]){
    TextDrawShowForPlayer(playerid,gTextDraw[30]);
  }

  for(new i = TD_STARS_START; i <= TD_STARS_END; i++)
  {
    TextDrawHideForPlayer(playerid, gTextDraw[i]);
  }

  ShowPlayerHudElement(playerid, HUD_HP, false);

  if (gmTemp[showJoins]==2 || (gmTemp[showJoins]==1 && gmTemp[pAbsAdminCount]>0))
  foreach(i)
  {
    if(playerid == i) continue;

    new
     szPlayerName[24];

    GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

    if(IsAdmin(i))
    {
      format(buffer, sizeof buffer, SkinKobiecy(pData[playerid][lastUsedSkin]) ? ("{b}%s (%i){/b} weszla na serwer (IP: %s)") : ("{b}%s (%i){/b} wszedl na serwer (IP: %s)"), szPlayerName, playerid, GetPlayerIP(playerid));
      Msg(i, COLOR_JOININFO, buffer, ((gmTemp[pPlayerCount]<5 && pData[playerid][allowedLevel]<=0) ? true : false) );
    }
    else if (gmTemp[showJoins]==2)
    {
      format(buffer, sizeof buffer, (SkinKobiecy(pData[playerid][lastUsedSkin])) ? ("{b}%s (%i){/b} weszla na serwer.") : ("{b}%s (%i){/b} wszedl na serwer."), szPlayerName, playerid);
      Msg(i, COLOR_JOININFO, buffer, ((gmTemp[pPlayerCount]<5 && pData[playerid][allowedLevel]<=0) ? true : false) );
    }
  }

  new
   pOnline = GetPlayerCount();

  if(pOnline > StringToInt(GetServerStat("most_online")))
  {
    SetServerStatInt("most_online", pOnline);
    SetServerStatString("most_online_date", "NOW()", true);

    format(buffer, sizeof buffer, __("Nowy rekord graczy na serwerze!~n~~n~~r~~h~%i!"), pOnline);
    ShowAnnouncement(buffer);
  }

  gmData[join_count]++;

  if(PlayerAccountExists(playerid))
  {
    pData[playerid][logonDialog] = true;
    ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, __("Witaj na FullServerze!"), __("Zaloguj sie uzywajac swojego hasla podanego podczas rejestracji.\n\nPlease login using password provided when creating this account."), __("Zaloguj"), __("Wyjdz"));
    pData[playerid][classSelecting] = false;
  }
  else
  {
    pData[playerid][logonDialog] = false;
    pData[playerid][classSelecting] = false;
    
    ShowPlayerDialog(playerid, DIALOG_HELP_RULES_GUEST, DIALOG_STYLE_MSGBOX, TXT(playerid, 80), GetDialogContent(DIALOG_HELP_RULES), __("Akceptuj"), __("Wyjdz"));
  }


  new
   szPlayerName[24],
   str[160];

  GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

  format(str,sizeof str," ~r~%s ~w~(~l~%d~w~) ",szPlayerName,playerid);
  PlayerTextDrawSetString(playerid,pTextDraw[PTD_STAT][playerid], str);

  Msg(playerid,COLOR_INFO,"Je¿eli chcesz dowiedzieæ siê co jest nowego na serwerze, wpisz {b}/nowosci{/b}");
  
  if(pData[playerid][restoredFromLastSession]) Msg(playerid, COLOR_ERROR, "Wygl¹da na to, ¿e mia³eœ crasha. Twoje podstawowe statystyki zosta³y przywrócone z poprzedniej sesji.");

  PlaySound(playerid, 1183);  // standardowa muza fs

  pTemp[playerid][p3d_status] = CreateDynamic3DTextLabel("", COLOR_3DTEXT_HITMAN, 0.0, 0.0, 0.7, 50.0, playerid, INVALID_VEHICLE_ID, 1);

  pTemp[playerid][accepts][eac_solo]=ACCEPTS_ALL;
  pTemp[playerid][accepts][eac_pm]=ACCEPTS_ALL;

  if (gmTemp[protAll])
    Msg(playerid, COLOR_ERROR,__("Uwaga! Obecnie wszyscy gracze na serwerze posiadaja imunitet!"));

  UpdatePlayerNickTD(playerid);

  copy("",pData[playerid][pDzaloz]);
  for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
    {
        if(IsPlayerAttachedObjectSlotUsed(playerid, i))
        {
            RemovePlayerAttachedObject(playerid, i);
        }
    }

  TogglePlayerClock(playerid,0);
  SyncPlayerGameTime(playerid);

  return 1;
}

OnPlayerLogin(playerid)
{
  new
   buffer[160];

  pData[playerid][accountID] = GetAccountID(GetPlayerNick(playerid));

  format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(SECOND, NOW(), date_end) pozostalo,reason FROM %s \
    WHERE player_banned = %i \
    AND date_end > NOW() \
    AND date_created < NOW() \
    LIMIT 1",
  gmData[DB_bans], pData[playerid][accountID]);

  mysql_query(buffer);
  mysql_store_result();

  if(mysql_num_rows())
  {
    new
     banExpireTime,
     tempPeriod,
     banReason[100];

    mysql_fetch_row_format(buffer,"|");// aqq
    sscanf(buffer,"p<|>dS(brak powodu)[99]", banExpireTime, banReason);

    GetOptimalTimeUnit(banExpireTime, tempPeriod);
    format(buffer, sizeof buffer, TXT(playerid, 20), banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
    Msg(playerid, COLOR_INFO, buffer); // Twoje konto jest zbanowane na xxx minut/godzin.
    format(buffer, sizeof buffer, __("Powod: {b}%s{/b}"), banReason);
    Msg(playerid, COLOR_INFO, buffer); // Twoje konto jest zbanowane na xxx minut/godzin.

    Msg(playerid, COLOR_INFO2, __("Jesli uwazasz ze ban byl niesluszny, badz tez pragniesz byc odbanowany przed uplywem podanego czasu"));
    Msg(playerid, COLOR_INFO2, __("zloz {b}podanie o odbanowanie{/b} na stronie {b}WWW.FULLSERVER.EU{/b}."));

    format(buffer, sizeof buffer, __("Gracz {b}%s{/b} nie dolaczyl z powodu aktywnego bana na konto {b}%s:+%d %s"), GetPlayerNick(playerid), banReason, banExpireTime, GetPeriodName(playerid, tempPeriod, banExpireTime));
    MSGToAdmins(COLOR_INFO2, buffer, false, LEVEL_ADMIN1);

    KickPlayer(playerid);
    mysql_free_result();
    return 0;
  }

  mysql_free_result();

  if(pData[playerid][isLocked]){
      SendClientMessage(playerid,-1," ");
    SendClientMessage(playerid,0xff0000ff,"Twoje konto zostalo zablokowane przez administratora. Jezeli to pomylka, napisz na WWW.FULLSERVER.EU");
    SendClientMessage(playerid,0xff0000ff,"Your account has been closed. Contact us at WWW.FULLSERVER.EU if you think that this is mistake.");
    SendClientMessage(playerid,-1," ");
    KickPlayer(playerid);
    return 0;
  }

  FetchPlayerAccountData(playerid);

  avt_getPlayerAchievements(playerid);  // pobiera wszystkie osiagniecia gracza i jego rankingi

  if(IsPlayerAdmin(playerid) && pData[playerid][adminLevel]<LEVEL_ADMIN3) // logowanie na rcona bez rcona w bazie danych!
  {
    format(buffer,sizeof buffer,__("Nieautoryzowane logowanie na admina RCON przez %s (%d)! Wykopany."), GetPlayerNick(playerid), playerid);
    KickPlayer(playerid);
    foreach(i)
      if(IsAdmin(i, LEVEL_ADMIN1))
        Msg(i, COLOR_ERROR, buffer);
    OutputLog(LOG_SYSTEM, buffer);
    return 0;
  }
  else if(IsPlayerAdmin(playerid) && pData[playerid][allowedLevel] == LEVEL_ADMIN3)
  {
    pData[playerid][adminLevel] = LEVEL_ADMIN3;
  }

  SetPlayerScore(playerid, pData[playerid][respect]);

  LogConnection(playerid);

  SetPlayerProperColor(playerid,true);


  pData[playerid][loggedIn] = true;
  pData[playerid][logonDialog] = false;

  new
   szName[24];

  GetPlayerName(playerid, szName, sizeof szName);

  if(pData[playerid][hitman] > 0)
  {
    IncreasePlayerHitman(playerid, 0);
  }
  // wczytujemy informacje o domu
  if (pData[playerid][accountID]>0)
    pTemp[playerid][e_houseid]=domy_findHouseByOwnerID(pData[playerid][accountID]);
  else
    pTemp[playerid][e_houseid]=-1;

  if (pTemp[playerid][e_houseid]>=0)
    domy_OnHouseOwnerLogin(playerid);

  if (pData[playerid][vipEnabled]) {
    if (gmTemp[pPlayerCount]<150) {
      format(buffer, sizeof buffer, "{A0A0A0}({FACB00}VIP{A0A0A0}) Do gry dolacza {FACB00}%s{A0A0A0}.", GetPlayerProperName(playerid));
      SendClientMessageToAll(0xA0A0A0FF, buffer);
    }
    _vpozostalo(playerid);
    UpdatePlayerNickTD(playerid);
  }
  checkDoubleMode(playerid);

  if (pData[playerid][gang]!=NO_GANG)
    gangs_OnPlayerLogin(playerid);

  if(pData[playerid][accountID]==702) {
    format(buffer,sizeof buffer,__("*** %s zalogowal sie jako maper."), GetPlayerNick(playerid));
    SendClientMessageToAll(0x636dfb, buffer);
    foreach(i)
      PlaySound(i, 1133);
  }
  // aqq

  if(!IsValidEmail(GetPlayerAccountData(playerid, "email"))) return ShowPlayerDialog(playerid, DIALOG_EMAIL_FORCE_CHANGE, DIALOG_STYLE_INPUT, "Podaj adres email", "Z powodu dodania mo¿liwoœci utworzenia konta na forum bazuj¹c na koncie z serwera wymagamy podania adresu email podczas rejestracji.\nUzupe³nij swoje konto o wymagane dane", "Dalej", TXT(playerid, 58));

  return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
  if(IsPlayerNPC(playerid)) return 1;
  
  gmTemp[highestPlayerID] = GetPlayerPoolSize();
    
  DestroyDynamic3DTextLabel(pTemp[playerid][FactionName]);
  pTemp[playerid][FactionName]=Text3D:-1;
  DestroyDynamic3DTextLabel(pTemp[playerid][p3d_status]);
  pTemp[playerid][p3d_status]=Text3D:-1;

  DestroyPickup(pTemp[playerid][bombId]); //bomby
  if(pTemp[playerid][bombTimer]!=-1) KillTimer(pTemp[playerid][bombTimer]);
  pTemp[playerid][bombTimer]=-1;

  if(IsPlayerInAnyVehicle(playerid)) tVehicles[GetPlayerVehicleID(playerid)][vo_driver]=INVALID_PLAYER_ID;

  //////////////// restore after crash! ///////////////
  pData[playerid][disconnectSession] = GetTickCount();
  pData[playerid][isCountedForRestore] = false;
  if(reason == 0 || pTemp[playerid][lostedByAdmin]) {
    pData[playerid][isCountedForRestore] = true;
    printf(" INFO: player %d counted for restore, tick count: %d, lost status: %d",playerid,GetTickCount(),pTemp[playerid][lostedByAdmin]);
  }
  //////////////// end restore after crash! ///////////////
  
  pData[playerid][pAttraction] = A_NONE;
  pData[playerid][aChowany] = false;
  pData[playerid][aHAY] = false;
  pData[playerid][aDerby] = false;
  pData[playerid][aRace] = false;
  pData[playerid][aDrift] = false;
  pData[playerid][aWG] = false;
  pData[playerid][aStrzelnica] = false;
  pData[playerid][aCTF] = false;
  pData[playerid][aGG] = false;

  HideProgressBarForPlayer(playerid,gmTemp[aCTFBar]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFINFOPOINTS]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFINFO]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFSTATETITLE]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFSTATETEXT]);
  PlayerTextDrawHide(playerid,pTextDraw[PTD_AGGINFO][playerid]);
  if(gmTemp[aCTFFlaga]==playerid) CTF_RespawnFlagi();

  TextDrawHideForPlayer(playerid,gTextDraw[TD_AWGINFO]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_AWGINFOHP]);
  HideProgressBarForPlayer(playerid,gmTemp[aWGPBar]);

  TextDrawHideForPlayer(playerid,gTextDraw[30]);

  if (gmTemp[aRacePlayersR][playerid]!=INVALID_PLAYER_ID) {
    gmTemp[aRacePlayers][gmTemp[aRacePlayersR][playerid]]=INVALID_PLAYER_ID;
    gmTemp[aRacePlayersR][playerid] = INVALID_PLAYER_ID;
  }

  if (aData[A_WG][aState] != A_STATE_OFF)
    for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++)
          if (gmTemp[aWGPlayers][i]==playerid) {
        gmTemp[aWGPlayers][i]=INVALID_PLAYER_ID;
        break;
      }
    if (aData[A_CTF][aState] != A_STATE_OFF)
    for(new i = 0; i < gmTemp[aCTFMaxPlayers]; i++)
          if (gmTemp[aCTFPlayers][i]==playerid) {
        gmTemp[aCTFPlayers][i]=INVALID_PLAYER_ID;
        break;
      }
      if (aData[A_GG][aState] != A_STATE_OFF)
    for(new i = 0; i < gmTemp[aGGMaxPlayers]; i++)
          if (gmTemp[aGGPlayers][i]==playerid) {
        gmTemp[aGGPlayers][i]=INVALID_PLAYER_ID;
        break;
      }

  if (aData[A_CHOWANY][aState] != A_STATE_OFF)
    for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
          if (gmTemp[aChowanyPlayers][i]==playerid) {
        gmTemp[aChowanyPlayers][i]=INVALID_PLAYER_ID;
        break;
      }

  avt_zeroPlayerAchievements(playerid); // zerujemy zapisy o osiagnieciach, na wypadek gdyby na ten id wrocil ktos niezarejestrowany (i te nie zostana pobrane).
    poczta_wyczyscTorbe(playerid);

  if(pData[playerid][reported]) RemovePlayerFromReportList(playerid);

  if(pTemp[playerid][e_houseid]>=0)
    domy_OnHouseOwnerDisconnects(playerid);

  for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
  {
    if(playerid != gmTemp[aStrzelnicaPlayers][i]) continue;

    gmTemp[aStrzelnicaPlayers][i] = INVALID_PLAYER_ID;
  }

  if(pData[playerid][loggedIn])
  {
    UpdatePlayerAccountData(playerid,true,true);  // 3 arg - wylogowany
    pTemp[playerid][lastSessionSaveTick] = GetTickCount();
  }

  if(gmData[artefactOwner] == playerid){
    DropArtefact(playerid);
  }

  if(pData[playerid][loggedIn]){

    if(pData[playerid][jail] - (GetTickCount() / 1000) > 0 && pData[playerid][jail] != 0)
    {
          format(gstr,sizeof gstr,"UPDATE fs_players SET jail=%d WHERE id=%d LIMIT 1",pData[playerid][jail] - (GetTickCount() / 1000), pData[playerid][accountID]);
      mysql_query(gstr);
    }
    else
    {
      format(gstr,sizeof gstr,"UPDATE fs_players SET jail=0 WHERE id=%d LIMIT 1", pData[playerid][accountID]);
      mysql_query(gstr);
    }

    format(gstr,sizeof gstr,"UPDATE fs_players SET spawnData=%d WHERE id=%d LIMIT 1", pData[playerid][spawnData],pData[playerid][accountID]);
    mysql_query(gstr);

    format(gstr,sizeof gstr,"UPDATE fs_players SET hudData='%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d' WHERE id=%d LIMIT 1",
      pData[playerid][hudSetting][HUD_STATUSBAR],pData[playerid][hudSetting][HUD_ATTRACTIONBOX],pData[playerid][hudSetting][HUD_ANNOUNCEMENTS],
      pData[playerid][hudSetting][HUD_VOTING],pData[playerid][hudSetting][HUD_SERVERLOGO],pData[playerid][hudSetting][HUD_DATE],pData[playerid][hudSetting][HUD_CLOCK],
      pData[playerid][hudSetting][HUD_STARS],pData[playerid][hudSetting][HUD_VEHICLEBOX],pData[playerid][hudSetting][HUD_HP],pData[playerid][hudSetting][HUD_CJHIT],pData[playerid][accountID]);
    mysql_query(gstr);
  }

  if (gmTemp[showJoins]>0)
  foreach(i)
  {
    if(pData[i][spectating] == playerid)
    {
      StopSpectating(i);
      GivePlayerNormalWeapons(i);
    }
    if(i == playerid || reason == LEAVE_REASON_KICKBAN) continue;
  }

  if (gmTemp[showJoins]>0) {
    new
     playedSeconds = GetPlayerCurrentSession(playerid),
     tempPeriod;
    GetOptimalTimeUnit(playedSeconds, tempPeriod);

    format(gstr, sizeof gstr,
        (SkinKobiecy(pData[playerid][lastUsedSkin]))?("%s (%i) opuœci³a serwer (gra³a %i %s)%s"):("%s (%i) opuœci³ serwer (gra³ %i %s)%s"),
        GetPlayerProperName(playerid), playerid, playedSeconds, GetPeriodName(playerid, tempPeriod, playedSeconds), (reason == LEAVE_REASON_TIMEOUT) ? (" (crash/timeout)") : (""));
    foreach(i) {
      if (gmTemp[showJoins]>0 && IsAdmin(i))
        Msg(i, COLOR_LEAVEINFO, gstr, ((gmTemp[pPlayerCount]<=10) ? true : false));
      else if (gmTemp[showJoins]==2)
        Msg(i, COLOR_LEAVEINFO, gstr, ((gmTemp[pPlayerCount]<=10) ? true : false));
    }
  }

  pData[playerid][loggedIn]=false;
  pData[playerid][vipEnabled]=false;
  pData[playerid][adminLevel]=0;
  pData[playerid][allowedLevel]=0;
  if(!pData[playerid][isCountedForRestore]) pData[playerid][session]=0;
  pData[playerid][accountID]=0;
  pTemp[playerid][curPos]=1;
  pTemp[playerid][lastPos]=-1;
  
  pTemp[playerid][lostedByAdmin] = false;
  pTemp[playerid][cenzura]=false;
  pTemp[playerid][protkill]=false;
  pTemp[playerid][protping]=false;
  pTemp[playerid][troll]=false;
  pTemp[playerid][lockedPos]=false;
  pTemp[playerid][weaponsAllowed]=true;

  pTemp[playerid][weaponSkill_pistol]=0;
  pTemp[playerid][weaponSkill_silenced]=0;
  pTemp[playerid][weaponSkill_sawnoff]=0;
  pTemp[playerid][weaponSkill_uzi]=0;
  pTemp[playerid][faction]=FACTION_NONE;
  UpdatePlayerStatsTD(playerid);

  ShowPlayerHudElement(playerid, HUD_STATUSBAR, false);
  ShowPlayerHudElement(playerid, HUD_ATTRACTIONBOX, false);
  ShowPlayerHudElement(playerid, HUD_ANNOUNCEMENTS, false);
  ShowPlayerHudElement(playerid, HUD_VOTING, false);
  ShowPlayerHudElement(playerid, HUD_SERVERLOGO, false);
  ShowPlayerHudElement(playerid, HUD_DATE, false);
  ShowPlayerHudElement(playerid, HUD_CLOCK, false);
  ShowPlayerHudElement(playerid, HUD_STARS, false);
  ShowPlayerHudElement(playerid, HUD_VEHICLEBOX, false);
  ShowPlayerHudElement(playerid, HUD_FPS, false);
  ShowPlayerHudElement(playerid, HUD_HP, false);

  pTemp[playerid][isRcon] = false;
  pTemp[playerid][uglyRejoinHack]=false; // na koncu dla zachowania pewnosci ze onPlayerDisconnect sie wykonalo i zachowana jest spojnosc danych!

  if (IsValidDynamicObject(oKratki[playerid]))
  DestroyDynamicObject(oKratki[playerid]);

  return 1;
}

public OnPlayerSpawn(playerid)
{
    if(IsPlayerNPC(playerid)) return 1;
    if(GetPVarInt(playerid,"Zaladowano")>0) {
    SetPVarInt(playerid,"Zaladowano",0);
  }

  if(GetPVarInt(playerid,"jetmax")>0) {
    SetPVarInt(playerid,"jetmax",0);
  }

#if defined CJHIT_MODEL
  if(pData[playerid][hudSetting][HUD_CJHIT] && GetPlayerState(playerid)!=PLAYER_STATE_SPECTATING) {
    PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT][playerid], GetPlayerSkin(playerid));
    PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT2][playerid], GetPlayerSkin(playerid));
    ShowPlayerHudElement(playerid, HUD_CJHIT, true);
  }
#endif

    ShowElement(playerid, TDE_VEHICLEBOX, false);

  if (poczta_wyczyscTorbe(playerid))
    Msg(playerid,COLOR_ERROR,"Zostales okradziony i straciles torbe z listami!");

  if (pTemp[playerid][onArena]==ARENA_SOLO && !pTemp[playerid][useRsp]) {
    solo_OnPlayerDeath(playerid,soloinv[playerid][esi_targetplayerid],54);
  }

  if(pTemp[playerid][useRsp]){
      if (pData[playerid][gang]!=NO_GANG)
      gangs_SetPlayerAttachedObject(playerid);

    if (gmData[artefactOwner] == playerid){
      artefact_SetPlayerHolding(playerid);
    }

        if(gmTemp[aCTFFlaga]==playerid && aData[A_CTF][aState] == A_STATE_ON && pData[playerid][pAttraction]==A_CTF){
          DestroyObject(gmTemp[aCTFFlagaObj]);
      gmTemp[aCTFFlagaObj] = CreateObject(11245,-1,-1,-1,0, 288.53997802734, 328.23999023438);
      MoveObject(gmTemp[aCTFFlagaObj],-1,-1,-1,999);
        AttachObjectToPlayer(gmTemp[aCTFFlagaObj], playerid, 1.5, 0.5, 0.0, 0.0, 1.5, 2);
    }
  }
  pTemp[playerid][dead]=false;
  pTemp[playerid][useRsp]=false;

  if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && strlen(pData[playerid][pDzaloz])>0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID])
    _zaloz(playerid,pData[playerid][pDzaloz]);

  if(pTemp[playerid][specPosReturn])
  {
    Teleport(T_PLAYER, playerid, pTemp[playerid][specPosition][0], pTemp[playerid][specPosition][1], pTemp[playerid][specPosition][2], pTemp[playerid][specPosition][3], pTemp[playerid][specInterior], pTemp[playerid][specVirtualWorld]);
    SetCameraBehindPlayer(playerid);
    pTemp[playerid][specPosReturn] = false;
    return 1;
  }

  if(pData[playerid][hudSetting][HUD_HP]) ShowPlayerHudElement(playerid, HUD_HP, true);

  if (pTemp[playerid][onArena]>0)
    return arena_SpawnPlayer(playerid);
  if(pData[playerid][pAttraction]==A_CTF) {
      CTF_ReSpawn(playerid);
    return 1;
  }
  if(pData[playerid][pAttraction]==A_GG) {
      GG_ReSpawn(playerid);
    return 1;
  }

    pData[playerid][hudSetting][HUD_FPS]=false;
    ShowPlayerHudElement(playerid, HUD_FPS, false);

  gzw_ShowGangZonesForPlayer(playerid);

  // full dm round
  for(new i=80; i>=0; i--) {
    GangZoneShowForPlayer(playerid, gmTemp[lastZoneIdFullDM]-i, 0x840B0B20);
  }
  // full dm normal
  for(new i=2; i>=0; i--) {
    GangZoneShowForPlayer(playerid, gmTemp[firstZoneIdFullDM]-i, 0x840B0B90); // last normal!
  }
  // no-dm round
  for(new i=50; i>=0; i--) { // ilosc * 10!
    GangZoneShowForPlayer(playerid, gmTemp[lastZoneIdNoDM]-i, 0x22BAFB20);
  }
  // no-dm normal
  for(new i=5; i>=0; i--) {
    GangZoneShowForPlayer(playerid, gmTemp[firstZoneIdNoDM]-i, 0x22BAFB90); // last normal!
  }

  if(pTemp[playerid][skinBeforeChange]!=-1){
    SetPlayerSkin(playerid,pTemp[playerid][skinBeforeChange]);
    SetSpawnInfo(playerid,playerid,pTemp[playerid][skinBeforeChange],0.0, 0.0, 0.0,0.0,0,0,0,0,0,0);
    SpawnPlayer(playerid);
    SetPlayerSkin(playerid,pTemp[playerid][skinBeforeChange]);
        pTemp[playerid][skinBeforeChange]=-1;
  }

  new
   randArg;

  switch(pData[playerid][spawnData])
  {
      case 0://CITY_LS:
    {
      randArg = random(sizeof DATA_spawns_LS);
      Teleport(T_PLAYER, playerid, DATA_spawns_LS[randArg][X], DATA_spawns_LS[randArg][Y], DATA_spawns_LS[randArg][Z], DATA_spawns_LS[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
    }
    case 1://CITY_SF:
    {
      randArg = random(sizeof DATA_spawns_SF);
      Teleport(T_PLAYER, playerid, DATA_spawns_SF[randArg][X], DATA_spawns_SF[randArg][Y], DATA_spawns_SF[randArg][Z], DATA_spawns_SF[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
    }
    case 2://CITY_LV:
    {
      randArg = random(sizeof DATA_spawns_LV);
      Teleport(T_PLAYER, playerid, DATA_spawns_LV[randArg][X], DATA_spawns_LV[randArg][Y], DATA_spawns_LV[randArg][Z], DATA_spawns_LV[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
    }
    case 3: //gang
    {
            if(pData[playerid][gang]!=NO_GANG) gangs_TeleportPlayerToBaseSpawn(playerid,true);
            else {
            switch (random(3))
        {
          case 0://CITY_LS:
          {
            randArg = random(sizeof DATA_spawns_LS);
            Teleport(T_PLAYER, playerid, DATA_spawns_LS[randArg][X], DATA_spawns_LS[randArg][Y], DATA_spawns_LS[randArg][Z], DATA_spawns_LS[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
          case 1://CITY_SF:
          {
            randArg = random(sizeof DATA_spawns_SF);
            Teleport(T_PLAYER, playerid, DATA_spawns_SF[randArg][X], DATA_spawns_SF[randArg][Y], DATA_spawns_SF[randArg][Z], DATA_spawns_SF[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
          case 2://CITY_LV:
          {
            randArg = random(sizeof DATA_spawns_LV);
            Teleport(T_PLAYER, playerid, DATA_spawns_LV[randArg][X], DATA_spawns_LV[randArg][Y], DATA_spawns_LV[randArg][Z], DATA_spawns_LV[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
        }
      }
    }
    case 4: //dom
    {
        if(pTemp[playerid][e_houseid]>=0) domy_tpto(playerid);
        else {
            switch (random(3))
        {
          case 0://CITY_LS:
          {
            randArg = random(sizeof DATA_spawns_LS);
            Teleport(T_PLAYER, playerid, DATA_spawns_LS[randArg][X], DATA_spawns_LS[randArg][Y], DATA_spawns_LS[randArg][Z], DATA_spawns_LS[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
          case 1://CITY_SF:
          {
            randArg = random(sizeof DATA_spawns_SF);
            Teleport(T_PLAYER, playerid, DATA_spawns_SF[randArg][X], DATA_spawns_SF[randArg][Y], DATA_spawns_SF[randArg][Z], DATA_spawns_SF[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
          case 2://CITY_LV:
          {
            randArg = random(sizeof DATA_spawns_LV);
            Teleport(T_PLAYER, playerid, DATA_spawns_LV[randArg][X], DATA_spawns_LV[randArg][Y], DATA_spawns_LV[randArg][Z], DATA_spawns_LV[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
        }
      }

    }
    case 5: //randomowy spawn w miescie
    {
        switch (random(3))
      {
        case 0://CITY_LS:
        {
          randArg = random(sizeof DATA_spawns_LS);
          Teleport(T_PLAYER, playerid, DATA_spawns_LS[randArg][X], DATA_spawns_LS[randArg][Y], DATA_spawns_LS[randArg][Z], DATA_spawns_LS[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
        }
        case 1://CITY_SF:
        {
          randArg = random(sizeof DATA_spawns_SF);
          Teleport(T_PLAYER, playerid, DATA_spawns_SF[randArg][X], DATA_spawns_SF[randArg][Y], DATA_spawns_SF[randArg][Z], DATA_spawns_SF[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
        }
        case 2://CITY_LV:
        {
          randArg = random(sizeof DATA_spawns_LV);
          Teleport(T_PLAYER, playerid, DATA_spawns_LV[randArg][X], DATA_spawns_LV[randArg][Y], DATA_spawns_LV[randArg][Z], DATA_spawns_LV[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
        }
      }
    }
    case 6: //gang spawn
    {
            if(pData[playerid][gang]!=NO_GANG) gangs_TeleportPlayerToGangSpawn(playerid,true);
            else {
            switch (random(3))
        {
          case 0://CITY_LS:
          {
            randArg = random(sizeof DATA_spawns_LS);
            Teleport(T_PLAYER, playerid, DATA_spawns_LS[randArg][X], DATA_spawns_LS[randArg][Y], DATA_spawns_LS[randArg][Z], DATA_spawns_LS[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
          case 1://CITY_SF:
          {
            randArg = random(sizeof DATA_spawns_SF);
            Teleport(T_PLAYER, playerid, DATA_spawns_SF[randArg][X], DATA_spawns_SF[randArg][Y], DATA_spawns_SF[randArg][Z], DATA_spawns_SF[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
          case 2://CITY_LV:
          {
            randArg = random(sizeof DATA_spawns_LV);
            Teleport(T_PLAYER, playerid, DATA_spawns_LV[randArg][X], DATA_spawns_LV[randArg][Y], DATA_spawns_LV[randArg][Z], DATA_spawns_LV[randArg][angle], 0, pTemp[playerid][cheater]?VW_TROLLE:VW_DEFAULT);
          }
        }
      }
    }
  }
  SetCameraBehindPlayer(playerid);

  SetPlayerProperColor(playerid);
  // synchronizujemy zegarek z czasem w grze
  SyncPlayerGameTime(playerid);
  SyncPlayerWeather(playerid);

  if (pTemp[playerid][cheater])
    SetPlayerVirtualWorld(playerid,VW_TROLLE);

  GivePlayerNormalWeapons(playerid);
  SetPlayerHealth(playerid,100.0);
  
  SetPlayerTeam(playerid, playerid+1);

  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, pTemp[playerid][weaponSkill_pistol]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, pTemp[playerid][weaponSkill_silenced]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, pTemp[playerid][weaponSkill_sawnoff]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, pTemp[playerid][weaponSkill_uzi]);

  if (pData[playerid][gang]!=NO_GANG && pData[playerid][loggedIn])
    gangs_SetPlayerAttachedObject(playerid);

  if (gmData[artefactOwner] == playerid){
    artefact_SetPlayerHolding(playerid);
  }
  if (pTemp[playerid][faction]!=FACTION_NONE)
    factions_SetPlayerAttObject(playerid);

  if (GetTickCount()%3==0) Msg(playerid,COLOR_INFO3,"Mo¿esz zmieniæ swój spawn wpisuj¹c komendê {b}/spawn{/b}");
  return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    if(IsPlayerNPC(playerid)) return 1;
  if(pData[playerid][pAttraction]==A_CTF) {
      new Float:cx,Float:cy,Float:cz;
      GetPlayerPos(playerid,cx,cy,cz);
      SetTimerEx("CTF_UpuscFlage",400,false,"dfff",playerid,cx,cy,cz);
  }
    new
   buffer[128];

  pTemp[playerid][shamalId]=0;
  
  if(pTemp[playerid][godMode]) killerid=INVALID_PLAYER_ID;

  if((playerid==killerid) || (killerid!=INVALID_PLAYER_ID && !IsPlayerConnected(killerid))) {
    if(pTemp[playerid][fakeKillWarn]){
      new szPlayerName[24];
        GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);
        printf("Prawdopodobny flooder fake killami - %s (%d) - ZBANOWANY",szPlayerName,playerid);
      BanEx(playerid,"Prawdopodobny flooder fake killami");
    }else{
        pTemp[playerid][fakeKillWarn]=true;
    }
    return 0;
  }

  if(GetTickCount()-pTemp[playerid][lastKillTickCount] < 2000 && killerid!=INVALID_PLAYER_ID && pTemp[playerid][lastKillTickCount]!=0) {
    if(pTemp[playerid][fastKillWarn]){
      new szPlayerName[24];
        GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);
        printf("Prawdopodobny flooder fake killami - %s (%d) - ZBANOWANY",szPlayerName,playerid);
      BanEx(playerid,"Prawdopodobny flooder fake killami");
    }else{
        pTemp[playerid][fastKillWarn]=true;
    }
    return 0;
  }

  if(GetTickCount()-pTemp[playerid][lastKillTickCount] < 45000 && pTemp[playerid][lastKillTickCount]!=0 && (pTemp[playerid][fakeKillWarn] || pTemp[playerid][fastKillWarn])) {
      pTemp[playerid][fakeKillWarn]=false;
      pTemp[playerid][fastKillWarn]=false;
  }

  pTemp[playerid][lastKillTickCount]=GetTickCount();

  if (pTemp[playerid][protkill]) {
    Msg(playerid,COLOR_INFO2, "Tracisz immunitet.");
    GameTextForPlayer(playerid, "~g~tracisz immunitet", 4000, 1);
    pTemp[playerid][protkill]=false;
  }

#if defined SPECTATE_KILLER
  if (killerid!=INVALID_PLAYER_ID && reason!=34) {  // ze snajperki nie pokazujemy
    DeathSpec(playerid,killerid);
  }
#endif

  pTemp[playerid][dead]=true;
  pTemp[playerid][isFreezed]=false;

  if(gmData[artefactOwner] == playerid) {
    DropArtefact(playerid);
  }
  
  new additionSkill = 0;
  
  ////////// killstreak addition processing //////////////
  if(killerid!=INVALID_PLAYER_ID){
    pTemp[killerid][killStreak]++;
    additionSkill = pTemp[killerid][killStreak]/5; // 5 = 1, 10 = 2 etc.
  }
 
  /////////// headshot addition processing ///////////////
  if(killerid!=INVALID_PLAYER_ID){
    if(pTemp[playerid][lastDeathHeadshottedBy] == killerid) additionSkill++;
    pTemp[playerid][lastDeathHeadshottedBy] = INVALID_PLAYER_ID; // reset it!
  }
  
  ////////////// skill and records processing /////////////////
  pTemp[playerid][killStreak]=0;
  if (pData[playerid][pAttraction] == A_NONE || pData[playerid][pAttraction] == A_ARENA || pData[playerid][pAttraction] == A_WG ) {
    pData[playerid][deaths]++;
    if (killerid!=INVALID_PLAYER_ID) {
      if (pTemp[killerid][lastPlayerKilled]!=playerid/* && GetPlayerState(killerid)!=PLAYER_STATE_DRIVER*/) {
        if(!pTemp[killerid][noSkill] && !pTemp[playerid][noSkill]) {
            if(pTemp[killerid][doubleMode]) {
              pData[killerid][skill]=(pData[killerid][skill]+(1+additionSkill)*2);
            }
            else {
              pData[killerid][skill]=(pData[killerid][skill]+1+additionSkill);
            }
        }
        if(!pTemp[playerid][noSkill] && !pTemp[killerid][noSkill]) {
          if (pData[playerid][skill]>0) pData[playerid][skill]--;
        }
        if (pData[killerid][loggedIn] && pData[killerid][kills]++%10==0)
          avt_record(killerid,e_kills,pData[killerid][kills],ART_UPDATE);
      }
      pTemp[killerid][lastPlayerKilled]=playerid;   // nie zliczamy podwojnych zabic
    }
  }
  
  ////////////////// gang processing ///////////////////
  if(killerid!=INVALID_PLAYER_ID && (pData[killerid][gang] != NO_GANG && pData[playerid][gang]!=NO_GANG) && (pData[playerid][pAttraction] == A_NONE || pData[playerid][pAttraction] == A_ARENA)) {
    if((pData[playerid][gang] == pData[killerid][gang] || gangi_sojusze[pData[playerid][gang]][pData[killerid][gang]]) && pTemp[playerid][onArena]!=ARENA_SOLO) // Team-kill
    {
        pData[killerid][teamkills]++;
        if (gData[pData[killerid][gang]][respect]>0)  {
          gData[pData[killerid][gang]][respect]--;

          if (GetPlayerInterior(killerid)==0 && GetPlayerVirtualWorld(killerid)==0 && pData[killerid][pAttraction] == A_NONE) {
            // wywolujemy funkcje odpowiedzialna za zarejestrowanie zabicia
            // sprawdzi ona czy zabicia dokonano na terenie gangzone i zapisze to w bazie (z buforowaniem)
            gzw_tk(killerid, pData[killerid][gang]);
          }
        }
    } else if (pData[playerid][gang] != pData[killerid][gang]) {                // Other Team-kill
        gData[pData[killerid][gang]][respect]++;
        if (gData[pData[playerid][gang]][respect]>0) gData[pData[playerid][gang]][respect]--;

        if (GetPlayerInterior(killerid)==0 && GetPlayerVirtualWorld(killerid)==0 && pData[killerid][pAttraction] == A_NONE) {
          // komentarz j.w.
          // funkcja przetwarza dwa gangi
          gzw_pk(killerid, pData[killerid][gang], pData[playerid][gang], playerid);
        }

    }
  }

  if (killerid!=INVALID_PLAYER_ID && pTemp[killerid][onArena]!=ARENA_NONE)
    pTemp[killerid][arenaScore][pTemp[killerid][onArena]]++;

  if(pData[playerid][pAttraction] == A_NONE)
  {

    if (killerid==playerid || killerid==INVALID_PLAYER_ID)
      pData[playerid][suicides]++;

    if(gmData[artefactOwner] == playerid)
      DropArtefact(playerid);
    new bool:dropmoney=true;

    //  --- Normal kill ---
    if(!gmTemp[protAll] && killerid!=INVALID_PLAYER_ID && (!pTemp[killerid][protkill] && pData[killerid][adminLevel]<LEVEL_GM))
    {
      if(IsPlayerInNoDMArea(playerid,true) || IsPlayerInNoDMArea(killerid,true)) {
        Msg(killerid, COLOR_INFO2, __("Zabijanie w strefie bez DM jest zakazane."));
        JailPlayer(killerid, 4);
        dropmoney=false;
      }else if(pTemp[playerid][godMode] && pData[killerid][adminLevel]<LEVEL_GM){
          Msg(killerid, COLOR_INFO2, __("Zabijanie w trybie pasywnym jest zakazane."));
        JailPlayer(killerid, 4);
        dropmoney=false;
      }else if ((!IsPlayerInFullDMArea(playerid) || !IsPlayerInFullDMArea(killerid)) && !IsFreeTime()) {
        if (GetPlayerState(killerid)==PLAYER_STATE_DRIVER) {
          dropmoney=false;
          switch(GetVehicleModel(GetPlayerVehicleID(killerid))){
            case 537,538, // pociagi
              460,511,512,513,519,553,593: { }    // samoloty oprocz andromedy, at-400, hydry, rustlera
            case 520,425,476: {
                Msg(killerid,COLOR_INFO2,"Zabijanie z uzyciem pojazdow wojskowych jest zakazane.");
              JailPlayer(killerid, 1);
            }
            case 417,563,548,497,488,487,469,447: {
                if(reason==50) {
                Msg(killerid,COLOR_INFO2,"Zabijanie z uzyciem smigla helikoptera jest zakazane.");
                JailPlayer(killerid, 1);
              }
            }
            default: {
              Msg(killerid,COLOR_INFO2,"Zabijanie przez potracenie pojazdem jest zakazane.");
              JailPlayer(killerid, 1);
            }
          }

        }
      }

    }

    new
     pMoney = GetPlayerMoney(playerid);

    if(dropmoney && pMoney >= 1000)
    {
      GivePlayerMoney(playerid, -(pMoney / 4));

      new
       pVector[e_Vectors];

      GetPlayerPos(playerid, pVector[X], pVector[Y], pVector[Z]);

      if(gmTemp[moneyPickupIdx] >= MAX_MONEY_PICKUPS)
        gmTemp[moneyPickupIdx] = 0;

      if(gMoneyPickup[gmTemp[moneyPickupIdx]][gmpTime] != 0)
      {
        if (IsValidDynamicPickup(gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID])) {
          DestroyDynamicPickup(gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID]);
          gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID]=-1;
        }

        if (IsValidDynamic3DTextLabel(gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText])) {
          DestroyDynamic3DTextLabel(gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText]);
          gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText]=Text3D:-1;
        }
        gMoneyPickup[gmTemp[moneyPickupIdx]][gmpTime]=0;
      }

      format(buffer, sizeof buffer, "$%i", pMoney / 4);

      gMoneyPickup[gmTemp[moneyPickupIdx]][gmpPickupID] = CreateDynamicPickup(1212, 1, pVector[X], pVector[Y], pVector[Z], GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid));
      gMoneyPickup[gmTemp[moneyPickupIdx]][gmpMoney] = pMoney / 4;
      gMoneyPickup[gmTemp[moneyPickupIdx]][gmpTime] = GetTickCount() / 1000;
      gMoneyPickup[gmTemp[moneyPickupIdx]][gmp3DText] = CreateDynamic3DTextLabel(buffer, 0x539C0090, pVector[X], pVector[Y], pVector[Z] + 0.3, 10.0, INVALID_PLAYER_ID,INVALID_VEHICLE_ID,0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid),-1, 10.0);

      gmTemp[moneyPickupIdx]++;
    }
  } else if (pData[playerid][pAttraction] == A_WG) {
    pTemp[playerid][aWGDead]=true;
    pData[playerid][pAttraction]=A_NONE;
    WG_Update();
  } else if (pData[playerid][pAttraction] == A_CHOWANY) {
    pTemp[playerid][aCHDead]=true;
    pData[playerid][pAttraction]=A_NONE;
    CH_Update();
  }


  if(killerid != INVALID_PLAYER_ID && pData[playerid][hitman] > 0)
  {
    foreach(i)
    {
      format(buffer, sizeof buffer, TXT(i, 294), GetPlayerNick(killerid), GetPlayerNick(playerid), pData[playerid][hitman]);
      Msg(i, COLOR_INFO3, buffer);
    }

    GivePlayerMoney(killerid, pData[playerid][hitman]);
    pData[playerid][hitman] = 0;

    if(pData[playerid][loggedIn])
    {
      SetPlayerAccountDataInt(playerid, "hitman_prize", 0);
    }

    PlaySound(killerid, 1057);
    IncreasePlayerHitman(playerid, 0);
  }

  gmData[death_count]++;

  if(killerid != INVALID_PLAYER_ID)
      gmData[kill_count]++;

  if(reason == 51)
  {
    reason = pTemp[killerid][lastWeaponUsed];
  }

  SendDeathMessage(killerid, playerid, reason);

  //  --- ATTRACTION STUFF ---

  if(pData[playerid][pAttraction] != A_NONE)
  {
    switch(pData[playerid][pAttraction])
    {
      case A_STRZELNICA: pData[playerid][pAttraction] = A_NONE;
      case A_DERBY: Derby_RemovePlayer(playerid,true);
      case A_WG: {
        pData[playerid][pAttraction] = A_NONE;
      }
      case A_CHOWANY:
      {
        if(pTemp[playerid][seesNametags]){
          for(new ii = 0; ii < MAX_PLAYERS; ii++)
              ShowPlayerNameTagForPlayer(playerid, ii, true);
        }
        if(killerid != INVALID_PLAYER_ID)
        {
          for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
          {
            if(gmTemp[aChowanyPlayers][i] == INVALID_PLAYER_ID) continue;

            format(buffer, sizeof buffer, TXT(i, 439), GetPlayerNick(killerid), GetPlayerNick(playerid));
            Msg(i, COLOR_INFO3, buffer);
          }
        }

        pData[playerid][pAttraction] = A_NONE;
        avt_record(playerid,e_chofan,1,ART_ADD);
        CH_Update();
      }
      case A_ARENA:
        if (arena_OnPlayerDeath(playerid,killerid,reason)==1) return 1;
      case A_CTF:
          if(CTF_onPlayerDeath(playerid)==1) return 1;
      case A_GG:{
        if(killerid!=INVALID_PLAYER_ID){
          if(GG_onPlayerDeath(playerid,killerid)==1) return 1;
        }
      }
    }
  }

  for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
  {
    if(playerid != gmTemp[aStrzelnicaPlayers][i]) continue;

    gmTemp[aStrzelnicaPlayers][i] = INVALID_PLAYER_ID;
  }

  for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++)
  {
    if(playerid != gmTemp[aWGPlayers][i]) continue;
    gmTemp[aWGPlayers][i] = INVALID_PLAYER_ID;
  }

  for(new i = 0; i < gmTemp[aChowanyMaxPlayers]; i++)
  {
    if(playerid != gmTemp[aChowanyPlayers][i]) continue;

    gmTemp[aChowanyPlayers][i] = INVALID_PLAYER_ID;

    if(pTemp[playerid][seesNametags]){
      for(new ii = 0; ii < MAX_PLAYERS; ii++)
              ShowPlayerNameTagForPlayer(playerid, ii, true);
        }
  }

  SetPlayerTeam(playerid, playerid+1);

  TextDrawHideForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_AREANODM]);

  TextDrawHideForPlayer(playerid,gTextDraw[TD_AWGINFO]);
  TextDrawHideForPlayer(playerid,gTextDraw[TD_AWGINFOHP]);
  HideProgressBarForPlayer(playerid,gmTemp[aWGPBar]);

  if(pData[playerid][hudSetting][HUD_HP]) ShowPlayerHudElement(playerid, HUD_HP, false);

#if defined CJHIT_MODEL
    if(pData[playerid][hudSetting][HUD_CJHIT]) {
    ShowPlayerHudElement(playerid, HUD_CJHIT, false);
  }
#endif

  pTemp[playerid][wStrefieNODM]=false;
  pTemp[playerid][wStrefieFULLDM]=false;

  // KASA
  new kasa=500 + floatround(pData[playerid][respect]/10);
  if (GetPlayerMoney(playerid)<kasa) {
    SetPlayerMoney(playerid,kasa);
  }

  return 1;
}

public OnVehicleSpawn(vehicleid)
{
  tVehicles[vehicleid][vo_used]=false;
  tVehicleUsed[vehicleid]=false;
  tVehicles[vehicleid][vo_occupied]=false;
  tVehicles[vehicleid][vo_driver]=INVALID_PLAYER_ID;
  tVehicles[vehicleid][vo_lastRepair]=GetTickCount()-VEHICLE_REPAIR_INTERVAL;

  if(tVehicles[vehicleid][vo_private]) {
      if (tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID) {
        Msg(tVehicles[vehicleid][vo_owningPlayerId],COLOR_INFO,__("Twoj pojazd wrocil pod Twoj dom."));
        // w tej funkcji nastapi zniszczenie aktualnego pojazdu i zespawnowanie go pod domem gracza
        domy_SpawnVehicle(tVehicles[vehicleid][vo_owningPlayerId]);
      } else {  // wlasciciel sie rozlaczyl a pojazd ulegl respawnowi
        tVehicles[vehicleid][vo_private]=false;
        FSDomy[tVehicles[vehicleid][vo_houseid]][ehv_id]=INVALID_VEHICLE_ID;
        tVehicles[vehicleid][vo_houseid]=-1;
        DestroyVehicle(vehicleid);
      }
      return 1;
  }
  if(vehicleid > staticVehicleCount) tVehicleSpawned[vehicleid]=true;

  tVehicles[vehicleid][vo_paintjob]=0;
  vehicleHasNitro[vehicleid]=false;
  tVehicles[vehicleid][vo_hasTurbo]=false;

  if (tVehicles[vehicleid][vo_static]) {
    vehicleDoorState[vehicleid] = DOOR_OPENED;
    SetVehicleParamsEx(vehicleid, 1, 0, 0, 0, 0, 0, 0);
  }

  SetVehicleHealth(vehicleid, VEHICLE_DEFAULT_HP);

  SetVehicleParamsEx(vehicleid, 1, 0, 0, 0, 0, 0, 0);

  if (!tVehicles[vehicleid][vo_licensePlateSet]) {
    new string[50];
    format(string,sizeof(string),"{000000}%c%c%c %i%i%i",(65+random(26)),(65+random(26)),(65+random(26)),random(10),random(10),random(10));
    SetVehicleNumberPlate(vehicleid,string);
  }
  return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
  tVehicles[vehicleid][vo_used]=false;
  tVehicles[vehicleid][vo_occupied]=false;
  tVehicles[vehicleid][vo_driver]=INVALID_PLAYER_ID;
  
  if(tVehicles[vehicleid][vo_private]) {
    if (tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID) {
      Msg(tVehicles[vehicleid][vo_owningPlayerId],COLOR_INFO,__("Twoj pojazd wrocil pod Twoj dom."));
      return domy_SpawnVehicle(tVehicles[vehicleid][vo_owningPlayerId]);
    } else {
      tVehicles[vehicleid][vo_private]=false;
      FSDomy[tVehicles[vehicleid][vo_houseid]][ehv_id]=INVALID_VEHICLE_ID;
      tVehicles[vehicleid][vo_houseid]=-1;
    }
    return 1;
  }

  vehicleHasNitro[vehicleid]=false;
  tVehicles[vehicleid][vo_hasTurbo]=false;
  vehicleDoorState[vehicleid] = DOOR_OPENED;
  vehicleDoorOwner[vehicleid] = INVALID_PLAYER_ID;
  tVehicles[vehicleid][vo_licensePlateSet]=false;

  if(GetVehicleModel(vehicleid) == 519)
    {
      foreach(i)
    {
      if(pTemp[i][shamalId] != 0)
          {
        SetPlayerHealth(i, 0);
        Msg(i,COLOR_ERROR,"Samolot rozbi³ siê i wraz z innymi pasa¿erami zgin¹³eœ w ogniach piekelnych :-)");
      }
    }
  }

  return 1;
}

public OnPlayerText(playerid, text[])
{
  if(pData[playerid][logonDialog] || pData[playerid][classSelecting]) {
    Msg(playerid,COLOR_ERROR,__("Najpierw musisz dolaczyc do gry."));
    return 0;
  }

  pTemp[playerid][lastPos]=-1;

  if(pData[playerid][jail]>0) {
    Msg(playerid, COLOR_ERROR, __("Niestety - siedzisz w paczce."));
    return 0;
  }

  if((strlen(GetPlayerNick(playerid)) + strlen(text) + 6) > 128)
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 399));
    return 0;
  }

  if (FilterText(playerid, text, true) == 0) return 0;
  if (gmTemp[lastHour]>6 && gmTemp[lastHour]<23)  // cenzura nieaktywna w nocy
    if(CensorText(text)) GivePlayerScore(playerid,-1,"cenzura");    // jak cenzura to dla wszystkich

  //  --- Game moderator chat ---

  if(text[0] == '$' && IsGM(playerid)) {
    OutputModeratorChat(playerid, text[1]);
    return 0;

  }

  //  --- Master & Normal admin chat ---

  if(text[0] == '@' && text[1]=='@' && IsAdmin(playerid, LEVEL_ADMIN3)){
    OutputAdmin3Chat(playerid, text[2]);
    return 0;
  } else if(text[0]=='@' && IsAdmin(playerid)){
    OutputAdminChat(playerid, text[1]);
    return 0;
  }

  //  --- Faction chat ---

  if (text[0]=='#') {
    if (pTemp[playerid][faction]!=FACTION_NONE)
      OutputFactionChat(playerid, text[1]);
    else
      Msg(playerid,COLOR_ERROR,__("Nie nalezysz do zadnej frakcji!"));
    return 0;
  }

  //  --- Gang chat ---
  if (text[0] == '!' && text[1] == '!' && pData[playerid][loggedIn] && pData[playerid][gang] != NO_GANG && (pData[playerid][gangRank]==GANG_RANK_OWNER || pData[playerid][gangRank]==GANG_RANK_LEADER || pData[playerid][gangRank]==GANG_RANK_VICEOWNER)) {
    OutputLeaderGangChat(playerid, text[2]);
    return 0;
  } else if(text[0] == '!') {
    if (pData[playerid][loggedIn] && pData[playerid][gang] != NO_GANG)
      OutputGangChat(playerid, text[1]);
    else
      Msg(playerid,COLOR_ERROR,__("Nie jestes w gangu!"));
    return 0;
  }

  new
   buffer[160];

  if(isPlayerMuted(playerid))
  {
      SendClientMessage(playerid,-1,"");
      if(pData[playerid][accountID]>0){
      format(buffer, sizeof buffer, "Jesteœ wyciszony na {b}%d{/b} %s", pTemp[playerid][mExpireTime], pTemp[playerid][mPeriod]);
      Msg(playerid, COLOR_INFO, buffer);
    }else{
      new period,muteTimeLeft = pTemp[playerid][mute] - (GetTickCount() / 1000);
      GetOptimalTimeUnit(muteTimeLeft, period);
      if(period == 'm') muteTimeLeft += 1;

        format(buffer, sizeof buffer, "Jesteœ wyciszony na {b}%d{/b} %s", muteTimeLeft, GetPeriodName(playerid, period, muteTimeLeft));
      Msg(playerid, COLOR_INFO, buffer);
    }
    format(buffer, sizeof buffer, __("Powod: {b}%s{/b}"), pTemp[playerid][mReason]);
    Msg(playerid, COLOR_INFO, buffer);
    Msg(playerid, COLOR_INFO2, __("Jesli uwazasz ze wyciszenie jest niesluszne, badz tez pragniesz byc odciszony przed uplywem podanego czasu"));
    Msg(playerid, COLOR_INFO2, __("zloz {b}podanie o odciszenie{/b} na stronie {b}WWW.FULLSERVER.EU{/b}."));
    SendClientMessage(playerid,-1,"");
    return 0;
  }

  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(text);

  if (gmTemp[chatLastMSGSender]!=playerid) {
    gmTemp[chatLastMSGSender]=playerid;
    gmTemp[chatMSGCount]=(gmTemp[chatMSGCount]+1)%2;
  }

  if (gmTemp[chatMSGCount]==0)
    format(buffer, sizeof buffer, "%i {%06x}%s: {FFFFFF}%s", playerid, pData[playerid][currentColor], GetPlayerNick(playerid), text);
  else
    format(buffer, sizeof buffer, "%i {%06x}%s: {EAEAEA}%s", playerid, pData[playerid][currentColor], GetPlayerNick(playerid), text);

  if (gmTemp[chatDisabled] && pData[playerid][adminLevel]<LEVEL_ADMIN2) {
    Msg(playerid, COLOR_ERROR, __("Czat jest obecnie wylaczony."));
    return 0;
  }

  if (ContainsIP(text)) {
    SendClientMessage(playerid, pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF, buffer);
    MSGToAdmins(COLOR_ADMIN, buffer, true, LEVEL_ADMIN2);
    format(buffer, sizeof buffer, __("Ninjabanowana proba reklamy przez {b}%s (%d){/b}."), GetPlayerNick(playerid), playerid);
    MSGToAdmins(COLOR_ADMIN, buffer, false, LEVEL_ADMIN2);
    return 0;
  }

  if(NinjaBan(text)){
      printf("Wykryto ninjabanowana probe trollowania przez %s: %s",GetPlayerNick(playerid),text);
      SendClientMessage(playerid, pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF, buffer);
      return 0;
  }

  if (pTemp[playerid][troll]) {
    SendClientMessage(playerid, pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF, buffer);

    if(gmTemp[showTroll]){
      format(buffer, sizeof buffer, "TROLL:%s", buffer);
      MSGToAdmins(COLOR_ADMIN, buffer, false, LEVEL_ADMIN2);
    }
    return 0;
  }

  sprawdzOdpowiedz(playerid,text);

  foreach(player){
    if(pData[playerid][logonDialog] || pData[playerid][classSelecting]) continue;
    SendClientMessage(player,pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF,buffer);
  }
  // SendClientMessageToAll(pData[playerid][adminLevel]==LEVEL_ADMIN3 ? 0xA01010FF : 0x606060FF, buffer);

  return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) // wykonywane nawet jak gracz zrezygnuje z akcji
{
    if(IsPlayerNPC(playerid)) return 1;
  if (tVehicles[vehicleid][vo_private] && tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID && tVehicles[vehicleid][vo_owningPlayerId]!=playerid) {
    new buf[128];
    format(buf,sizeof buf,"To jest prywatny pojazd {b}%s{/b}(%d).", GetPlayerProperName(tVehicles[vehicleid][vo_owningPlayerId]), tVehicles[vehicleid][vo_owningPlayerId]);
    Msg(playerid,COLOR_INFO,buf,false);
    if (!ispassenger || !tVehicles[vehicleid][vo_occupied]) {
      RemovePlayerFromVehicle(playerid);
      return 0;
    }
  }

  tVehicles[vehicleid][vo_used]=true;

  tVehicles[vehicleid][vo_occupied]=true;

  tVehicleUsed[vehicleid]=true;

  if(!IsFreeTime()){
    if(!ispassenger) SetPlayerArmedWeapon(playerid,0);
  }
  if(ispassenger && tVehicles[pData[playerid][lastVehicle]][vo_driver]==INVALID_PLAYER_ID) SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed]) SetPlayerArmedWeapon(playerid,0);
  if(IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  if (GetVehicleModel(vehicleid)==594)  // doniczka
    SetPlayerArmedWeapon(playerid,0);

    if (ispassenger && GetVehicleModel(vehicleid) == 519){
    pTemp[playerid][shamalId]=vehicleid;
    SetPlayerVirtualWorld(playerid,vehicleid);
    SetPlayerInterior(playerid, 1);
    SetPlayerPos(playerid, 1891.4706,-8156.8750,2.4471);
    SetPlayerFacingAngle(playerid, 0);
    SetCameraBehindPlayer(playerid);
    FreezePlayer(playerid,750);
    Streamer_Update(playerid);
  }

  return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if(IsPlayerNPC(playerid)) return 1;
    if(GetPVarInt(playerid,"Zaladowano")>0) {
    SetPVarInt(playerid,"Zaladowano",0);
    Msg(playerid,COLOR_ERROR,"Zostales okradziony i straciles paczki z samolotu!");
  }

  if (pData[playerid][pAttraction]==A_DERBY) {
    RespawnVehicle(vehicleid);
    return Derby_RemovePlayer(playerid);
  } else if (pData[playerid][pAttraction]==A_RACE) {
    RespawnVehicle(vehicleid);
    return race_RemovePlayer(playerid);
  } else if (pData[playerid][pAttraction]==A_DRIFT) {
    RespawnVehicle(vehicleid);
    return drift_RemovePlayer(playerid);
  }

  tVehicles[vehicleid][vo_used]=true;
  if (GetPlayerVehicleSeat(playerid)==0) {  // wysiadl kierowca
    tVehicles[vehicleid][vo_occupied]=false;
    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    if (lights) lights=0;
    SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
  }

  ShowElement(playerid, TDE_VEHICLEBOX,false);

  tVehicleUsed[vehicleid]=true;
  
  return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(IsPlayerNPC(playerid)) return 1;
  SyncPlayerGameTime(playerid);

  // Enter vehicle
  if((newstate == PLAYER_STATE_DRIVER && oldstate == PLAYER_STATE_ONFOOT) || (newstate == PLAYER_STATE_PASSENGER && oldstate == PLAYER_STATE_ONFOOT))
  {

    pData[playerid][lastVehicle]=GetPlayerVehicleID(playerid);
    if (newstate==PLAYER_STATE_DRIVER) {
      if (tVehicles[pData[playerid][lastVehicle]][vo_private] &&
            tVehicles[pData[playerid][lastVehicle]][vo_owningPlayerId]!=INVALID_PLAYER_ID &&
            tVehicles[pData[playerid][lastVehicle]][vo_owningPlayerId]!=playerid) {
              RemovePlayerFromVehicle(playerid);
              return 0;
      }
      if(!IsFreeTime()) SetPlayerArmedWeapon(playerid,0);
      if(!pTemp[playerid][weaponsAllowed]) SetPlayerArmedWeapon(playerid,0);
      if(IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);
      tVehicles[pData[playerid][lastVehicle]][vo_driver]=playerid;
    }else if (newstate==PLAYER_STATE_PASSENGER) {
      if (tVehicles[pData[playerid][lastVehicle]][vo_driver]==INVALID_PLAYER_ID || pTemp[tVehicles[pData[playerid][lastVehicle]][vo_driver]][godMode] || IsPlayerInNoDMArea(playerid))
        SetPlayerArmedWeapon(playerid,0); // ustawiamy na piesci
    }
    if(!pTemp[playerid][weaponsAllowed]) SetPlayerArmedWeapon(playerid,0);


    if (vehicleDoorState[pData[playerid][lastVehicle]] == DOOR_CLOSED)
      PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid],0xcc000030);
    else
      PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid],3145776);

    if(pData[playerid][hudSetting][HUD_VEHICLEBOX]) ShowElement(playerid, TDE_VEHICLEBOX, true);


    foreach(i)
    {
      if(pData[i][spectating] == playerid)
      {
        PlayerSpectateVehicle(i, pData[playerid][lastVehicle]);
      }
    }
    if (newstate==PLAYER_STATE_DRIVER) {
      new engine, lights, alarm, doors, bonnet, boot, objective;
      GetVehicleParamsEx(pData[playerid][lastVehicle], engine, lights, alarm, doors, bonnet, boot, objective);
      SetVehicleParamsEx(pData[playerid][lastVehicle], engine, 1, alarm, doors, bonnet, boot, objective);
    }
  }

  // Leave vehicle
  else if((newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER) || (newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_PASSENGER))
  {
    if (oldstate==PLAYER_STATE_DRIVER)
      tVehicles[pData[playerid][lastVehicle]][vo_driver]=INVALID_PLAYER_ID;


    ShowElement(playerid, TDE_VEHICLEBOX, false);
    if(pData[playerid][destroyMyVehicle] && pData[playerid][lastVehicle]!=INVALID_VEHICLE_ID)
    {
      DestroyVehicle(pData[playerid][lastVehicle]);
      pData[playerid][destroyMyVehicle] = false;
    }

    foreach(i)
    {
      if(pData[i][spectating] == playerid)
      {
        PlayerSpectatePlayer(i, playerid);
      }
    }
      if(GetPVarInt(playerid,"Zaladowano")>0) {
      SetPVarInt(playerid,"Zaladowano",0);
      Msg(playerid,COLOR_ERROR,"Zostales okradziony i straciles paczki z samolotu!");
    }

    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(pData[playerid][lastVehicle], engine, lights, alarm, doors, bonnet, boot, objective);
    if (lights) lights=0;
    SetVehicleParamsEx(pData[playerid][lastVehicle], engine, 0, alarm, doors, bonnet, boot, objective);
  }
  // dzwieki w pojazdach
  if (newstate==PLAYER_STATE_DRIVER || oldstate==PLAYER_STATE_DRIVER)
    Audio_OnPlayerStateChange(playerid,newstate,oldstate);

  if(oldstate==PLAYER_STATE_SPECTATING && newstate!=PLAYER_STATE_WASTED && newstate!=PLAYER_STATE_NONE)
  {
#if defined CJHIT_MODEL
    if(pData[playerid][hudSetting][HUD_CJHIT]) {
      PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT][playerid], GetPlayerSkin(playerid));
      ShowPlayerHudElement(playerid, HUD_CJHIT, true);
    }
#endif
  }

  if(newstate==PLAYER_STATE_SPECTATING)
  {
#if defined CJHIT_MODEL
    if(pData[playerid][hudSetting][HUD_CJHIT]) {
      ShowPlayerHudElement(playerid, HUD_CJHIT, false);
    }
#endif
  }
  return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    if(aData[A_CTF][aState] == A_STATE_ON && pData[playerid][pAttraction] == A_CTF){
      CTF_OPEFlagaRCP(playerid);
  }
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
  if(aData[A_CTF][aState] == A_STATE_ON && pData[playerid][pAttraction] == A_CTF){
      CTF_OPETeamCP(playerid);
  }
  return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
  new
   buffer[128];

  if(pickupid == gPickup[artefact] && gmData[artefactOwner] == INVALID_PLAYER_ID)
  {
    artefact_OnPlayerPickup(playerid);
    return 1;
  }


  if (prezenty_OPPickUpDynamicPickup(playerid,pickupid))  return 1;
  if (obiekty_OPPickUpDynamicPickup(playerid,pickupid)) return 1;
  if (OPEDP_Paczki(playerid,pickupid)) return 1;

  for(new i = 0; i < MAX_MONEY_PICKUPS; i++)
  {
    if(gMoneyPickup[i][gmpTime]>0 && gMoneyPickup[i][gmpPickupID] == pickupid)
    {
      if (IsValidDynamicPickup(gMoneyPickup[i][gmpPickupID])) {
        DestroyDynamicPickup(gMoneyPickup[i][gmpPickupID]);
        gMoneyPickup[i][gmpPickupID]=-1;
      }
      if (IsValidDynamic3DTextLabel(gMoneyPickup[i][gmp3DText])) {
        DestroyDynamic3DTextLabel(gMoneyPickup[i][gmp3DText]);
        gMoneyPickup[i][gmp3DText]=Text3D:-1;
      }

      gMoneyPickup[i][gmpTime] = 0;

      GivePlayerMoney(playerid, gMoneyPickup[i][gmpMoney]);

      format(buffer, sizeof buffer, TXT(playerid, 4), gMoneyPickup[i][gmpMoney]);
      Msg(playerid, COLOR_INFO, buffer);

      return 1;
    }
  }

  return 0;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
  tVehicles[vehicleid][vo_paintjob]=paintjobid;
  return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
  tVehicles[vehicleid][vo_color][0]=color1;
  tVehicles[vehicleid][vo_color][1]=color2;
  return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid){
  if(newinteriorid>0){
    SetPlayerWeather(playerid,0);
    SetPlayerTime(playerid,12,0);
  }else
    SetPlayerWeather(playerid,gmTemp[currentWeather]);
  SyncPlayerGameTime(playerid);

  if (gmTemp[pGMCount]+gmTemp[pAdminCount]>0)
    foreach(i)
      if(pData[i][spectating]==playerid)
        StartSpectating(i,playerid);

  return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
  new pstate=GetPlayerState(playerid);

  if (pstate==PLAYER_STATE_SPECTATING) {
    if (KEY_PRESSED(KEY_CROUCH) && (pData[playerid][spectating]!=INVALID_PLAYER_ID) && (pData[playerid][adminLevel]>=LEVEL_ADMIN1)) {
      _info(playerid,""); return 0;
    } else if (KEY_PRESSED(KEY_FIRE) && (pData[playerid][spectating]!=INVALID_PLAYER_ID) && (pData[playerid][adminLevel]>=LEVEL_ADMIN1)) {
      FindNextPlayerToSpectate(playerid, false);
      return 0;
    } else if (KEY_PRESSED(KEY_HANDBRAKE) && (pData[playerid][spectating]!=INVALID_PLAYER_ID) && (pData[playerid][adminLevel]>=LEVEL_ADMIN1)) {
      FindNextPlayerToSpectate(playerid, true);
      return 0;
    }
  } else if (pstate == PLAYER_STATE_ONFOOT) {
    // antygl na wybranej arenie
    if(KEY_PRESSED(KEY_CROUCH) && pTemp[playerid][onArena]==ARENA_ONEDE2 && GetTickCount()-pTemp[playerid][lastFireKeypress]<600 && GetPlayerWeapon(playerid)==24) 
    {
      //format(gstr,sizeof gstr,"%d", GetPlayerAnimationIndex(playerid));
      //SendClientMessage(playerid,-1,gstr);
      SetPlayerVelocity(playerid, 0.0,0.0,0.5);
      if(!pTemp[playerid][isFreezed]) FreezePlayer(playerid,200);
      SetPlayerArmedWeapon(playerid,0);
      /*if((pTemp[playerid][antygl]++)==MAX_IDENT_STEP) {
        Msg(playerid,COLOR_ERROR,"Ignorowanie regulaminu nie bêdzie tutaj tolerowane. Zostajesz wyrzucony");
        Kick(playerid);
      }*/
      Msg(playerid,COLOR_ERROR,"Na tej arenie nie wolno wykonywaæ GL/TGL!");
      return 0;
    }
    if(KEY_PRESSED(KEY_FIRE))
      {
          if((pTemp[playerid][wStrefieNODM] || pTemp[playerid][protping] || (aData[A_CHOWANY][aState] == A_STATE_ON && pData[playerid][aChowany] && pTemp[playerid][aChowanySide] == A_CHOWANY_HIDDING && pData[playerid][pAttraction] == A_CHOWANY) || !pTemp[playerid][weaponsAllowed]) &&
      !pTemp[playerid][protkill] &&
      (pData[playerid][adminLevel]<LEVEL_GM || pTemp[playerid][protping] || !pTemp[playerid][weaponsAllowed]) && pstate!=0 && pstate!=9 && pstate!=8 && pstate!=7 && GetPlayerWeapon(playerid) != 43 && GetPlayerWeapon(playerid) != 46 && !IsPlayerSwimming(playerid)){
            SetPlayerVelocity(playerid, 0.0,0.0,0.5);
          SetPlayerArmedWeapon(playerid,0);
          if(!pTemp[playerid][isFreezed]) FreezePlayer(playerid,200);
          return 0;
      }

      pTemp[playerid][lastFireKeypress]=GetTickCount();
      pTemp[playerid][staleTime]=0;
          pTemp[playerid][lastWeaponUsed] = GetPlayerWeapon(playerid);
    }
    if(pTemp[playerid][performingAnim]) { // resetujemy odtwarzana animacje
          pTemp[playerid][performingAnim]=false;
          ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0, 0);
          ClearAnimations(playerid, 0);
    }
    //KEY_HANDBRAKE == key_aim
    if(HOLDING_KEYS(KEY_HANDBRAKE | KEY_JUMP  ) && (GetPlayerWeapon(playerid)==0 || GetPlayerWeapon(playerid)==1) && pTemp[playerid][fRsp]) {
        if(GetTickCount()-pTemp[playerid][lastResyncByKey]>3000) {
        _rsp(playerid,"");
        pTemp[playerid][lastResyncByKey]=GetTickCount();
      }
    }
  } else if (pstate==PLAYER_STATE_PASSENGER) {
    // nothing!
  } else if (pstate==PLAYER_STATE_DRIVER) {
    new vid=GetPlayerVehicleID(playerid);
    if(vid>0 && vid!=INVALID_VEHICLE_ID) {
      if(KEY_PRESSED(KEY_SUBMISSION)) {
        if(pTemp[playerid][cenzura]) return 0;
        if (pData[playerid][pAttraction]==A_RACE) {
          race_Napraw(playerid);
          return 0;
        } else if (pData[playerid][pAttraction]==A_DRIFT) {
          drift_Napraw(playerid);
          return 0;
        } else
        {
            new vf=GetVehicleFlags(vid);
            if (vf&VF_MILITARY==VF_MILITARY) return 0;
          if (vf&VF_AIRPLANE==VF_AIRPLANE) return 0;
          if (vf&VF_AIRBORNE==VF_AIRBORNE) return 0;
          if (vf&VF_NATATORIAL==VF_NATATORIAL) return 0;
          if(!pData[playerid][vipEnabled]) _napraw(playerid);
          else _vnapraw(playerid,"");
          _flip(playerid);
        }
      } else if (KEY_PRESSED(KEY_FIRE)) {     // turbo

        if(pData[playerid][pAttraction]==A_DERBY && GetVehicleModel(vid)==564) { // rc tiger
          vehicles_RCTiger_shoot(playerid);
          return 0;
        }

        if (pData[playerid][pAttraction]==A_NONE && tVehicles[vid][vo_hasTurbo]) {
          vehicles_EngageTurbo(playerid,vid,0);
          return 0;
        }

      } else if (KEY_RELEASED(KEY_FIRE)) {  // nitro
        if (vehicleHasNitro[vid]) {
          AddVehicleComponent(vid, 1010);
        }
      } else  if (KEY_PRESSED(KEY_ANALOG_DOWN)) {
        if (vid>0 && GetVehicleModel(vid)==525) { // towtruck
          TowVehicle(vid);
        }
      } else if(KEY_PRESSED(KEY_ACTION)) {
          if(!pTemp[playerid][rampy]) return 0;
          if(pData[playerid][pAttraction]!=A_NONE) return 0;
          if(IsPlayerInNoDMArea(playerid)) return 0;
          new vf=GetVehicleFlags(vid);
          if (vf&VF_MILITARY==VF_MILITARY) return 0;
        if (vf&VF_AIRPLANE==VF_AIRPLANE) return 0;
        if (vf&VF_AIRBORNE==VF_AIRBORNE) return 0;
        if (vf&VF_NATATORIAL==VF_NATATORIAL) return 0;
          KillTimer(rampTimer);
          RemoveRamp(playerid);
          new Float:x,Float:y,Float:z,Float:anglee;
        GetPlayerPos(playerid, x, y, z);
          anglee = GetXYInFrontOfPlayer(playerid, x, y, GetOptimumRampDistance(playerid));
        z += 0.5;
          pTemp[playerid][rampId]=CreatePlayerObject(playerid, 1632, x, y, z - 0.5, 0.0, 0.0, anglee);
          pTemp[playerid][rampTimer]=SetTimerEx("RemoveRamp", 2000, 0, "d", playerid);
          return 0;
      }
    }
  }


    if (newkeys == 16 && pTemp[playerid][shamalId] > 0)
  {
    new Float:sX,Float:sY,Float:sZ;
    GetVehiclePos(pTemp[playerid][shamalId], sX, sY, sZ);
    SetPlayerPos(playerid, sX+4, sY, sZ);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid,0);
    pTemp[playerid][shamalId]=0;
  }
  return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
  if(success)
  {
    SetTimer("SearchForNewAdmin", 50, false);
  }
  else
  {
        static RconLoginTimes;
    static Times;

    if((GetTickCount()-RconLoginTimes) < 1000){
      Times++;
      if(Times > 2){
        Times = RconLoginTimes = 0;
        foreach(hackerid){

          if(!strcmp(ip,GetPlayerIP(hackerid),true)){
            SendClientMessage(hackerid, 0xFFFFFFFF, "SERVER: Wrong Password. Bye!");
            BanEx(hackerid,"RCON CRASH ATTEMPT");
          }
        }
      }
    }else{
      Times = RconLoginTimes = 0;
    }

    RconLoginTimes = GetTickCount();
  }

  new
   buffer[128];

  format(buffer, sizeof buffer, "[Logowanie na RCON] IP: %s - haslo: %s - zalogowano: %s", ip, password, success ? ("tak") : ("nie"));
  OutputLog(LOG_SYSTEM, buffer);

  return 1;
}

public OnPlayerUpdate(playerid)
{
  pTemp[playerid][staleTime]=0;
  
  if(IsPlayerNPC(playerid)) return 1;
  if(pTemp[playerid][desync]) return 0;
  
  if (GetTickCount()%2==0) return 1;    // raz na 2 razy kontynuujemy

  static pState,wepid,bbuf[128];
  pState = GetPlayerState(playerid);

  wepid=GetPlayerWeapon(playerid);
  if (wepid!=pTemp[playerid][lastWeaponHolded]) {
     if (wepid>1 && wepid!=41 &&
      (pTemp[playerid][wStrefieNODM] || pTemp[playerid][protping] || !pTemp[playerid][weaponsAllowed]) &&
      !pTemp[playerid][protkill] &&
      (pData[playerid][adminLevel]<LEVEL_GM || pTemp[playerid][protping] || !pTemp[playerid][weaponsAllowed]) && pState!=0 && pState!=9 && pState!=8 && pState!=7 && GetPlayerWeapon(playerid) != 43 && GetPlayerWeapon(playerid) != 46) {
        if(GetPlayerWeapon(playerid) != 0) {
          SetPlayerArmedWeapon(playerid,0);
          return 0;
        }
    }

    OnPlayerChangeWeapon(playerid,pTemp[playerid][lastWeaponHolded], wepid);
    pTemp[playerid][lastWeaponHolded]=wepid;
  }

  if(GetTickCount()%2==0 || !IsPlayerSpawned(playerid) || GetPlayerState(playerid)==PLAYER_STATE_WASTED || pData[playerid][classSelecting]) return 1;

  if(pData[playerid][hudSetting][HUD_HP]) {
    static Float:opupHealth, Float:opupArmour;
    GetPlayerHealth(playerid,opupHealth);
    GetPlayerArmour(playerid,opupArmour);
    if(floatcmp(opupArmour, 0.0)!=0) {
        if(floatcmp(opupHealth, 100.0)==1 && floatcmp(opupArmour, 100.0)!=1) format(bbuf,28,"%.0f~n~~n~~n~~n~~r~~h~GOD",opupArmour);
        else if(floatcmp(opupArmour, 100.0)==1 && floatcmp(opupHealth, 100.0)!=1) format(bbuf,28,"GOD~n~~n~~n~~n~~r~~h~%.0f",opupHealth);
        else if(floatcmp(opupArmour, 100.0)==1 && floatcmp(opupHealth, 100.0)==1) format(bbuf,28,"GOD~n~~n~~n~~n~~r~~h~GOD");
        else format(bbuf,28,"%.0f~n~~n~~n~~n~~r~~h~%.0f",opupArmour,opupHealth);
      PlayerTextDrawShow(playerid, pTextDraw[PTD_ARICON][playerid] );
    }
    else {
      if(floatcmp(opupHealth, 100.0)==1) format(bbuf,28,"_~n~~n~~n~~n~~r~~h~GOD");
        else format(bbuf,28,"_~n~~n~~n~~n~~r~~h~%.0f",opupHealth);
      PlayerTextDrawHide(playerid, pTextDraw[PTD_ARICON][playerid] );
    }
    PlayerTextDrawSetString(playerid,pTextDraw[PTD_HPTEXT][playerid], bbuf);
  }

  if(pState == PLAYER_STATE_DRIVER || pState == PLAYER_STATE_PASSENGER){
    RefreshPlayerVehicleInfo(playerid);
  }

  if(GetTickCount()%3==0) return 1;

  if(pTemp[playerid][godMode] && pData[playerid][pAttraction]==A_NONE){
    SetPlayerHealth(playerid,99999.0);
    SetPlayerArmour(playerid,99999.0);
    ResetPlayerWeapons(playerid);
  }

  if((aData[A_CHOWANY][aState] == A_STATE_ON && pData[playerid][aChowany] && pTemp[playerid][aChowanySide] == A_CHOWANY_SEARCHING && pData[playerid][pAttraction] == A_CHOWANY)){
      SetPlayerHealth(playerid,9999.0);
  }

  if(IsPlayerInAnyVehicle(playerid))
  {
     if(pTemp[playerid][godMode] && pData[playerid][pAttraction]==A_NONE){
      RepairVehicle(GetPlayerVehicleID(playerid));
      SetVehicleHealth(GetPlayerVehicleID(playerid),VEHICLE_DEFAULT_HP);
    }
    if(tVehicles[pData[playerid][lastVehicle]][vo_driver]==INVALID_PLAYER_ID || pTemp[tVehicles[pData[playerid][lastVehicle]][vo_driver]][godMode] || IsPlayerInNoDMArea(playerid)){
      SetPlayerArmedWeapon(playerid,0);
      return 0;
    }
    if(!IsFreeTime() && pState == PLAYER_STATE_DRIVER){
      SetPlayerArmedWeapon(playerid,0);
      return 0;
    }
  }
  if(pTemp[playerid][identStep]>MAX_IDENT_STEP){
    BanEx(playerid, "BAD PLAYER");
    return 0;
  }
  return 1;
}

public OnPlayerStreamIn(playerid, forplayerid){
  if (!pTemp[forplayerid][seesNametags])
    ShowPlayerNameTagForPlayer(forplayerid, playerid, false);
  return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid){
  new doors,bool:objective;
  doors=vehicleDoorState[vehicleid];

  if ((forplayerid == vehicleDoorOwner[vehicleid]) ||  (tVehicles[vehicleid][vo_private] && tVehicles[vehicleid][vo_owningPlayerId]==forplayerid ))
    doors=DOOR_OPENED;

  if (IsPlayerInAnyVehicle(forplayerid)) {
    new vid=GetPlayerVehicleID(forplayerid);
    if (vid!=vehicleid && GetVehicleModel(vid)==525 && !IsTrailerAttachedToVehicle(vid))  // towtruck
      if ((!tVehicles[vehicleid][vo_occupied] && tVehicles[vehicleid][vo_used] && !tVehicles[vehicleid][vo_private]) ||
        (tVehicles[vehicleid][vo_static] && !tVehicles[vehicleid][vo_used] && !tVehicles[vehicleid][vo_occupied] &&
         random(10)==1) && GetVehicleFlags(vehicleid)&VF_TOWABLE==VF_TOWABLE)
        objective=true;
  }

  SetVehicleParamsForPlayer(vehicleid, forplayerid, objective, doors);
  return 1;
}

public GameP_HTTPResponse(index, response, data[]){
  if(response!=200) {
    switch(pTemp[index][smsType])
    {
      case 1: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7636{FFFFFF}\nKoszt wykonania uslugi to {FF0000}7,38 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Nie udalo nawiazac sie bezpiecznego polaczenia z serwerem weryfikacji kodow!","Wykonaj","Wroc");
      }
      case 2: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7936{FFFFFF}\nKoszt wykonania uslugi to {FF0000}11,07 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Nie udalo nawiazac sie bezpiecznego polaczenia z serwerem weryfikacji kodow!","Wykonaj","Wroc");
      }
      case 3: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}91955{FFFFFF}\nKoszt wykonania uslugi to {FF0000}23,37 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Nie udalo nawiazac sie bezpiecznego polaczenia z serwerem weryfikacji kodow!","Wykonaj","Wroc");
      }
      case 4: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}92555{FFFFFF}\nKoszt wykonania uslugi to {FF0000}30,75 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Nie udalo nawiazac sie bezpiecznego polaczenia z serwerem weryfikacji kodow!","Wykonaj","Wroc");
      }
    }
    return 1;
  }

  if (data[0]=='1'){
    switch(pTemp[index][smsType])
    {
        case 1: {
        Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}69{/b} punktów GP!");
        GivePlayerGamePoints(index,69,"zakup za SMS");
      }
      case 2: {
                Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}161{/b} punktów GP!");
                GivePlayerGamePoints(index,161,"zakup za SMS");
      }
      case 3: {
                Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}345{/b} punktów GP!");
                GivePlayerGamePoints(index,345,"zakup za SMS");
      }
      case 4: {
                Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}690{/b} punktów GP!");
                GivePlayerGamePoints(index,690,"zakup za SMS");
      }
      case 5: {
        Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}1035{/b} punktów GP!");
        GivePlayerGamePoints(index,1035,"zakup za SMS");
      }
      case 6: {
                Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}1380{/b} punktów GP!");
                GivePlayerGamePoints(index,1380,"zakup za SMS");
      }
      case 7: {
                Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}2070{/b} punktów GP!");
                GivePlayerGamePoints(index,2070,"zakup za SMS");
      }
      case 8: {
                Msg(index,COLOR_INFO,"Gratulacje! Prawid³owo zakupiles na swoje konto {b}2760{/b} punktów GP!");
                GivePlayerGamePoints(index,2760,"zakup za SMS");
      }
    }
    pTemp[index][smsType]=0;
  } else {
    switch(pTemp[index][smsType])
    {
      case 1: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7136{FFFFFF}\nKoszt wykonania uslugi to {FF0000}1,23 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 2: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7255{FFFFFF}\nKoszt wykonania uslugi to {FF0000}2,46 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 3: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7455{FFFFFF}\nKoszt wykonania uslugi to {FF0000}4,92 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 4: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7636{FFFFFF}\nKoszt wykonania uslugi to {FF0000}7,83 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 5: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7936{FFFFFF}\nKoszt wykonania uslugi to {FF0000}11,07 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 6: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}91455{FFFFFF}\nKoszt wykonania uslugi to {FF0000}17,22 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 7: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}91955{FFFFFF}\nKoszt wykonania uslugi to {FF0000}23,37 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
      case 8: {
        ShowPlayerDialog(index,DIALOG_ADD_GP_STEP2,DIALOG_STYLE_INPUT,"Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa","{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}92555{FFFFFF}\nKoszt wykonania uslugi to {FF0000}30,75 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania\n\n{FF0000}BLAD! Podany kod SMS wyglada nie nieprawidlowy. Czy na pewno dobrze go przepisales?","Wykonaj","Wroc");
      }
    }
  }
  return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
  new
   buffer[512];

  switch(dialogid)
  {
      case DIALOG_VEH_PANEL:
      {
          if (!response) return 0;

      if (pData[playerid][pAttraction]!=A_NONE)
        return Msg(playerid,COLOR_ERROR,TXT(playerid,454)); // nie mozesz tego teraz zrobic

            if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return Msg(playerid, COLOR_ERROR, "Aby uzywac tej komendy musisz byc kierowca pojazdu");

      new vid=GetPlayerVehicleID(playerid);
      tVehicles[vid][vo_occupied]=true;

      if (vid>0 && vid!=INVALID_VEHICLE_ID) {
              new engine, lights, alarm, doors, bonnet, boot, objective;

        GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);

        switch(listitem){
            case 0: engine=!engine;
            case 1: lights=!lights;
            case 2: {
                if(alarm) alarm=0;
                else alarm=1;
          }
            case 3: {
                if(!doors) _lock(playerid);
                else _unlock(playerid);
          }
            case 4: bonnet=!bonnet;
            case 5: boot=!boot;
        }

        // ugly fix to prevent changing doors state
        new engine_tmp, lights_tmp, alarm_tmp, doors_tmp, bonnet_tmp, boot_tmp, objective_tmp;
        GetVehicleParamsEx(vid, engine_tmp, lights_tmp, alarm_tmp, doors_tmp, bonnet_tmp, boot_tmp, objective_tmp);

        SetVehicleParamsEx(vid, engine, lights, alarm, doors_tmp, bonnet, boot, objective);
        tVehicleUsed[vid]=true;
        tVehicles[vid][vo_used]=true;
      }
      else return Msg(playerid,COLOR_INFO,TXT(playerid,451));
      return 1;
      }
    case DIALOG_EXCHANGE_GP:
    {
      if (!response) return 0;

      switch(listitem)
      {
        case 0: ShowPlayerDialog(playerid, DIALOG_HELP_GPOINTS, DIALOG_STYLE_MSGBOX, "Panel pomocy > GamePoints", GetDialogContent(DIALOG_HELP_GPOINTS), __("Wyjdz"), __("Wroc"));
                case 2: ShowPlayerDialog(playerid, DIALOG_ADD_GP, DIALOG_STYLE_LIST, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wybierz ilosc punktow", "{FF0000}69{FFFFFF} punktow\n{FF0000}161{FFFFFF} punktow\n{FF0000}345{FFFFFF} punktow\n{FF0000}690{FFFFFF} punktow\n{FF0000}1035{FFFFFF} punktow\n{FF0000}1380{FFFFFF} punktow\n{FF0000}2070{FFFFFF} punktow\n{FF0000}2760{FFFFFF} punktow\n", __("Dalej"), __("Wroc"));
                case 3: ShowPlayerDialog(playerid, DIALOG_ADD_GP_MONEY, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints kasa z banku", "{FFFFFF}Wpisz ilosc GamePoints jaka chcesz wykupic. Minimum to 3, a maksimum to 15.\nKoszt jednego GamePoint to $200000.", __("Doladuj"), __("Wroc"));
        case 5: ShowPlayerDialog(playerid, DIALOG_VIP_FOR_GP, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Oplac konto VIP", "{FFFFFF}Wpisz ilosc dni na jaka chcesz zakupic VIPa. Minimum to 15, a maksimum to 120.\nKoszt jednego dnia VIPa to 23 GP.", __("Wykonaj"), __("Wroc"));
                case 6: ShowPlayerDialog(playerid, DIALOG_HOUSE_FOR_GP_HELP, DIALOG_STYLE_MSGBOX, "Centrum wymiany GamePoints > Oplac domek", "{FFFFFF}Aby zakupic domek za punkty GP musisz podejsc do niego, wybrac funkcje oplacania\ni przy etapie wyboru platnosci wybrac \"GamePoints\" zamiast \"Respet\"", __("Zamknij"), __(""));
        case 7: ShowPlayerDialog(playerid, DIALOG_NICK_FOR_GP, DIALOG_STYLE_MSGBOX, "Centrum wymiany GamePoints > Szybsza zmiana nicku", "{FFFFFF}Prosze potwierdzic zakup tej uslugi. Koszt wykonania to 10 GP.", __("Wykonaj"), __("Wroc"));
                case 8: ShowPlayerDialog(playerid, DIALOG_DOUBLE_FOR_GP, DIALOG_STYLE_MSGBOX, "Centrum wymiany GamePoints > Tryb furii", "{FFFFFF}W trybie furii otrzymujesz 2 razy wiecej respektu, skilla i GP (wylacznie za godzine gry) za kazda wykonana akcje.\nKoszt aktywacji to 100 GP (80 GP dla VIPa) za uruchomienie uslugi na najblizsze 4 godziny bez wzgledu na aktywnosc w grze.", __("Wykonaj"), __("Wroc"));
                case 9: {
                  if(!pTemp[playerid][godMode]){
                    ShowPlayerDialog(playerid, DIALOG_GOD_FOR_GP, DIALOG_STYLE_MSGBOX, "Centrum wymiany GamePoints > Tryb pasywny", "{FFFFFF}W trybie pasywnym nie mozesz uczestniczyc w interakcjach DM, a inni nie moga zadac Ci obrazen. Koszt aktywacji to 5 GP za uruchomienie uslugi do konca sesji gry.\n{FF0000}W przypadku checi wylaczenia wybierz te opcje ponownie z menu /GP. Kolejne uruchomienie po wylaczeniu bedzie pelnoplatne!{FFFFFF}", __("Wykonaj"), __("Wroc"));
                  }else{
                    ShowPlayerDialog(playerid, DIALOG_GOD_FOR_GP, DIALOG_STYLE_MSGBOX, "Centrum wymiany GamePoints > Tryb pasywny", "{FFFFFF}Chcesz wylaczyc funkcje trybu pasywnego. Je¿eli jesteœ pewny kliknij 'Wylacz'. Pamiêtaj, ¿e kolejne uruchomienie po wylaczeniu bedzie pelnoplatne!{FFFFFF}", __("Wylacz"), __("Wroc"));
                  }
                }
      }
      return 1;
    }
    case DIALOG_ADD_GP_MONEY:
    {
      if (!response) return _gp(playerid);

      if(strval(inputtext)>15 || strval(inputtext)<3) return ShowPlayerDialog(playerid, DIALOG_ADD_GP_MONEY, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints kasa z banku", "{FF0000}Blad! Dostosuj sie do limitow ilosciowych.{FFFFFF}\n\nWpisz ilosc GamePoints jaka chcesz wykupic. Minimum to 3, a maksimum to 15.\nKoszt jednego GamePoint to $200000.", __("Doladuj"), __("Wroc"));
      if(StringToInt(GetPlayerAccountData(playerid, "bank_money"))<strval(inputtext)*MONEY_GP_COST) return ShowPlayerDialog(playerid, DIALOG_ADD_GP_MONEY, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints kasa z banku", "{FF0000}Blad! Nie masz takiej ilosci pieniedzy w banku.{FFFFFF}\n\nWpisz ilosc GamePoints jaka chcesz wykupic. Minimum to 15, a maksimum to 15.\nKoszt jednego GamePoint to $200000.", __("Doladuj"), __("Wroc"));
      printf("ZAKUP GAME POINTS ZA KASE W GRZE, ACCOUNT: %d, ILOSC GP: %d, KOSZT: %d",pData[playerid][accountID],strval(inputtext),strval(inputtext)*MONEY_GP_COST);
      GivePlayerGamePoints(playerid,strval(inputtext),"zakup GP za kase w grze");
      MinusPlayerAccountData(playerid, "bank_money", strval(inputtext)*MONEY_GP_COST);
      return 1;
    }
    case DIALOG_ADD_GP:
    {
        pTemp[playerid][smsType]=0;

        if (!response) return _gp(playerid);

        switch(listitem)
      {
          case 0: {
          pTemp[playerid][smsType]=1;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7136{FFFFFF}\nKoszt wykonania uslugi to {FF0000}1,23 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 1: {
            pTemp[playerid][smsType]=2;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7255{FFFFFF}\nKoszt wykonania uslugi to {FF0000}2,46 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 2: {
            pTemp[playerid][smsType]=3;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7455{FFFFFF}\nKoszt wykonania uslugi to {FF0000}4,92 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 3: {
            pTemp[playerid][smsType]=4;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7636{FFFFFF}\nKoszt wykonania uslugi to {FF0000}7,38 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 4: {
          pTemp[playerid][smsType]=5;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}7936{FFFFFF}\nKoszt wykonania uslugi to {FF0000}11,07 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 5: {
            pTemp[playerid][smsType]=6;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}91455{FFFFFF}\nKoszt wykonania uslugi to {FF0000}17,22 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 6: {
            pTemp[playerid][smsType]=7;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}91955{FFFFFF}\nKoszt wykonania uslugi to {FF0000}23,37 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }
        case 7: {
            pTemp[playerid][smsType]=8;
          ShowPlayerDialog(playerid, DIALOG_ADD_GP_STEP2, DIALOG_STYLE_INPUT, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wyslij SMSa", "{FFFFFF}Wyslij wiadomosc SMS o tresci {FF0000}KOD.FSEXTRA {FFFFFF}na numer {FF0000}92555{FFFFFF}\nKoszt wykonania uslugi to {FF0000}30,75 brutto{FFFFFF}\nRegulamin, reklamacje i informacje dostepne na stronie {FF0000}http://fullserver.eu/doladowania", __("Wykonaj"), __("Wroc"));
        }

      }
      return 1;

    }
    case DIALOG_ADD_GP_STEP2:
    {
        if (!response) return ShowPlayerDialog(playerid, DIALOG_ADD_GP, DIALOG_STYLE_LIST, "Centrum wymiany GamePoints > Doladuj GamePoints SMSem > Wybierz ilosc punktow", "{FF0000}69{FFFFFF} punktow\n{FF0000}161{FFFFFF} punktow\n{FF0000}345{FFFFFF} punktow\n{FF0000}690{FFFFFF} punktow\n{FF0000}1035{FFFFFF} punktow\n{FF0000}1380{FFFFFF} punktow\n{FF0000}2070{FFFFFF} punktow\n{FF0000}2760{FFFFFF} punktow\n", __("Dalej"), __("Wroc"));

        new gbuf[128];
        switch(pTemp[playerid][smsType]) // http://fsutil.i32.pl/checkin/payment/?id=32&numer=7636&kod=h83m3c47&gamep=690
      {
        case 1: {
            format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=7136&kod=%s&gamep=69",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 69
        }
        case 2: {
            format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=7255&kod=%s&gamep=161",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 161
        }
        case 3: {
            format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=7455&kod=%s&gamep=345",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 345
        }
        case 4: {
          format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=7636&kod=%s&gamep=690",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 690
        }
        case 5: {
            format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=7936&kod=%s&gamep=1035",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 1035
        }
        case 6: {
            format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=91455&kod=%s&gamep=1380",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 1380
        }
        case 7: {
            format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=91955&kod=%s&gamep=2070",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 2070
        }
        case 8: {
          format(gbuf,sizeof gbuf,"fsutil.i32.pl/checkin/payment/?id=%d&numer=92555&kod=%s&gamep=2760",GetPlayerAccountID(playerid),inputtext);
          HTTP(playerid, HTTP_GET, gbuf, "", "GameP_HTTPResponse"); // 2760
        }
        default: Msg(playerid,COLOR_ERROR,"Wystapil nieoczekiwany problem. Skontaktuj sie z administratorem. Kod bledu #NO_PACKAGE_SELECTED");
      }
      return 1;
    }
    case DIALOG_VIP_FOR_GP:
    {
      if (!response) return _gp(playerid);

      if(strval(inputtext)>120 || strval(inputtext)<15) return ShowPlayerDialog(playerid, DIALOG_VIP_FOR_GP, DIALOG_STYLE_INPUT, "Centrum wymiany Game Points > Doladuj konto VIP", "{FF0000}Blad! Dostosuj sie do limitow ilosciowych.{FFFFFF}\n\nWpisz ilosc dni na jaka chcesz zakupic VIPa. Minimum to 15, a maksimum to 120.\nKoszt jednego dnia VIPa to 23 GP.", __("Wykonaj"), __("Wroc"));
      if(pData[playerid][gamep]<strval(inputtext)*VIP_GP_COST) return ShowPlayerDialog(playerid, DIALOG_VIP_FOR_GP, DIALOG_STYLE_INPUT, "Centrum wymiany Game Points > Doladuj konto VIP", "{FF0000}Blad! Nie masz wystarczajacej ilosci GP. Moze sproboj podac mniejsza ilosc dni? Mo¿esz te¿ zakupiæ GamePoints za SMSa!{FFFFFF}\n\nWpisz ilosc dni na jaka chcesz zakupic VIPa. Minimum to 15, a maksimum to 120.\nKoszt jednego dnia VIPa to 23 GP.", __("Wykonaj"), __("Wroc"));
      printf("ZAKUP VIPA ZA GAME POINTS, ACCOUNT: %d, ILOSC DNI: %d, KOSZT: %d",pData[playerid][accountID],strval(inputtext),strval(inputtext)*VIP_GP_COST);
      GivePlayerGamePoints(playerid,-strval(inputtext)*VIP_GP_COST,"zakup vipa");

      pData[playerid][vipEnabled]=true;
      pData[playerid][vipDaysLeft]=pData[playerid][vipDaysLeft]+strval(inputtext);
      new mysqlf[128];
      format(mysqlf,128,"UPDATE fs_players SET vip=IF(vip>NOW(),vip + INTERVAL %d DAY,NOW()+INTERVAL %d DAY) WHERE id=%d LIMIT 1",strval(inputtext),strval(inputtext),pData[playerid][accountID]);
      mysql_query(mysqlf);

      format(mysqlf,128,"SELECT vip FROM fs_players WHERE id=%d LIMIT 1",pData[playerid][accountID]);
      mysql_query(mysqlf);
      mysql_store_result();
      if(mysql_num_rows())
      {
        mysql_fetch_row(pData[playerid][vipToDate]);
      }
      mysql_free_result();
      _vpozostalo(playerid);
      return 1;
    }
    case DIALOG_NICK_FOR_GP:
    {
        if (!response) return _gp(playerid);

        if(pData[playerid][gamep]<10) return Msg(playerid,COLOR_ERROR,"Nie staæ Cie na zakup tej us³ugi. Koszt to {b}10{/b} Game Points");

      format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(MINUTE, NOW(), next_nick_change) FROM %s WHERE id = %i AND next_nick_change > NOW() LIMIT 1",
        gmData[DB_players],
        pData[playerid][accountID]
      );

      mysql_query(buffer);
      mysql_store_result();

      if(mysql_num_rows())
      {
        format(gstr, sizeof gstr, "UPDATE fs_players SET next_nick_change=NOW() WHERE id=%d LIMIT 1", pData[playerid][accountID]);
        mysql_query(gstr);
        printf("ZAKUP ZMIANY NICKU ZA GAME POINTS, ACCOUNT: %d",pData[playerid][accountID]);
        GivePlayerGamePoints(playerid,-10,"zakup uslugi szybkiej zmiany nicku");
        Msg(playerid,COLOR_INFO,"Zakup us³ugi powiód³ siê. Mo¿esz teraz zmieniæ nick :-)");
      }
      else Msg(playerid,COLOR_ERROR,"Mo¿esz zmieniæ nick ju¿ teraz i nie potrzebujesz u¿ywaæ tego!");
            mysql_free_result();
      return 1;
    }
    case DIALOG_DOUBLE_FOR_GP:
    {
        if (!response) return _gp(playerid);

        if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Nie mo¿esz teraz w³¹czyæ tej funkcji"));

        if(pData[playerid][vipEnabled]) {
          if(pData[playerid][gamep]<(DOUBLE_GP_COST-20)) {
            return Msg(playerid,COLOR_ERROR,"Nie staæ Cie na zakup tej us³ugi. Koszt to {b}80{/b} Game Points");
          }
        }else {
          if(pData[playerid][gamep]<DOUBLE_GP_COST) {
            return Msg(playerid,COLOR_ERROR,"Nie staæ Cie na zakup tej us³ugi. Koszt to {b}100{/b} Game Points");
          }
        }

      if(!pTemp[playerid][doubleMode])
      {
        pTemp[playerid][doubleMode]=true;
        format(gstr, sizeof gstr, "UPDATE `fs_players` SET `doubleMode` = NOW() + INTERVAL 4 HOUR WHERE `id` = %d LIMIT 1", pData[playerid][accountID]);
        mysql_query(gstr);
        printf("ZAKUP TRYBU FURII ZA GAME POINTS, ACCOUNT: %d",pData[playerid][accountID]);
        if(pData[playerid][vipEnabled]) GivePlayerGamePoints(playerid,-(DOUBLE_GP_COST-20),"zakup uslugi trybu furii (VIP)");
        else GivePlayerGamePoints(playerid,-DOUBLE_GP_COST,"zakup uslugi trybu furii");
        checkDoubleMode(playerid);
      }
      else checkDoubleMode(playerid);
      return 1;
    }
    case DIALOG_GOD_FOR_GP:
    {
        if (!response) return _gp(playerid);

        if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid)) return Msg(playerid,COLOR_ERROR,"Nie mo¿esz teraz u¿yæ tej funkcji");

        if(pData[playerid][gamep]<GOD_GP_COST && !pTemp[playerid][godMode]) return Msg(playerid,COLOR_ERROR,"Nie staæ Cie na zakup tej us³ugi. Koszt to {b}5{/b} Game Points");

      if(!pTemp[playerid][godMode])
      {
        pTemp[playerid][godMode]=true;
        pTemp[playerid][weaponsAllowed]=false;
        pTemp[playerid][protkill]=false;
        GivePlayerNormalWeapons(playerid);
        ResetPlayerWeapons(playerid);
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
        printf("ZAKUP TRYBU PASYWNEGO ZA GAME POINTS, ACCOUNT: %d",pData[playerid][accountID]);
        GivePlayerGamePoints(playerid,-GOD_GP_COST,"zakup uslugi trybu pasywnego");
        Msg(playerid,COLOR_INFO,"Tryb pasywny zosta³ {b}aktywowany{/b}. U¿yj tej opcji ponownie aby wy³¹czyæ!");
      }
      else {
          pTemp[playerid][godMode]=false;
          pTemp[playerid][weaponsAllowed]=true;
          GivePlayerNormalWeapons(playerid);
          SetPlayerHealth(playerid,100.0);
          SpawnPlayer(playerid);
          Msg(playerid,COLOR_INFO,"Tryb pasywny zosta³ {b}wy³¹czony{/b}. Kolejne uruchomienie bedzie pe³nop³atne.");
      }
      UpdatePlayerFactionName(playerid);
      return 1;
    }
    case DIALOG_HOUSE_FOR_GP_HELP:
    {
      return 1;
    }
    case DIALOG_CHANGE_WEAPON:
      {
          if(pData[playerid][respect]<40000) return Msg(playerid,COLOR_ERROR,"Korzystanie z broni specjalnych wymaga minimum 40k respektu!");
        if (!response) return 0;
      switch(listitem)
      {
          case 0:
          {
              if(pData[playerid][respect]>=75000){
                Msg(playerid,COLOR_INFO3,"Zmieniono bron specjalna na {b}Granaty{/b}");
                pData[playerid][selectedWeap]=1;
          }else{
              Msg(playerid,COLOR_INFO3,"Zmieniono bron specjalna na {b}Koktajle Molotova{/b}");
                pData[playerid][selectedWeap]=2;
          }
        }
        case 1:
          {
              if(pData[playerid][respect]>=40000){
                Msg(playerid,COLOR_INFO3,"Zmieniono bron specjalna na {b}Koktajle Molotova{/b}");
                if(pData[playerid][respect]>=75000) pData[playerid][selectedWeap]=2;
                else pData[playerid][selectedWeap]=1; // we want to show 75k player grenades as default!
          }else{
              Msg(playerid,COLOR_INFO3,"{b}Wy³¹czono{/b} bron specjalna");
                pData[playerid][selectedWeap]=0;
          }
        }
        case 2:
          {
            Msg(playerid,COLOR_INFO3,"{b}Wy³¹czono{/b} bron specjalna");
              pData[playerid][selectedWeap]=0;
        }
      }
      if(pData[playerid][accountID]>0){
        format(gstr,sizeof gstr,"UPDATE fs_players SET selectedWeap=%d WHERE id=%d LIMIT 1", pData[playerid][selectedWeap],pData[playerid][accountID]);
        mysql_query(gstr);
      }
      ResetPlayerWeapons(playerid);
      GivePlayerNormalWeapons(playerid,false);
      return 1;
    }
    case DIALOG_CHANGE_SPAWN:
      {
        if (!response) return 0;
      switch(listitem)
      {
          case 0: //ls
          {
              Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Los Santos{/b}");
              pData[playerid][spawnData]=0;
        }
        case 1: //lv
          {
               Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Las Venturas{/b}");
              pData[playerid][spawnData]=2;
        }
        case 2: //sf
          {
               Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}San Fierro{/b}");
                    pData[playerid][spawnData]=1;
        }
        case 3: //cale full andreas
          {
               Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Full Andreas{/b}");
              pData[playerid][spawnData]=5;
        }
        case 4: //wyspa gangu lub domek
          {
              if(pTemp[playerid][e_houseid]<0){
                  Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Wyspa gangu{/b}");
                      pData[playerid][spawnData]=3;
                      return 1;
          }
            Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Domek{/b}");
              pData[playerid][spawnData]=4;
        }
        case 5: //wyspa gangu lub spawn gangu
          {

            if(pTemp[playerid][e_houseid]<0){
                Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Spawn gangu{/b}");
                      pData[playerid][spawnData]=6;
                      return 1;
          }

              Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Wyspa gangu{/b}");
                    pData[playerid][spawnData]=3;
        }
        case 6: //spawn gangu
          {
               Msg(playerid,COLOR_INFO3,"Zmieniono spawn postaci na {b}Spawn gangu{/b}");
                    pData[playerid][spawnData]=6;
        }
      }
      if(pData[playerid][accountID]>0){
        format(gstr,sizeof gstr,"UPDATE fs_players SET spawnData=%d WHERE id=%d LIMIT 1", pData[playerid][spawnData],pData[playerid][accountID]);
        mysql_query(gstr);
      }
      return 1;
    }
    case DIALOG_INFO:
      return 1;
    case DIALOG_GANG..(DIALOG_GANG+9):
      return gangs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    case DIALOG_TUNEMENU_MAIN..(DIALOG_TUNEMENU_MAIN+9):
      return vehicles_TunemenuResponse(playerid, dialogid, response, listitem, inputtext);
    case DIALOG_WARSZTAT..(DIALOG_WARSZTAT+9):
      return warsztat_DialogResponse(playerid, dialogid, response, listitem, inputtext);
    case DIALOG_DOMY..(DIALOG_DOMY+10):
      return domy_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    case DLG_POCZTA..(DLG_POCZTA+10):
      return poczta_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    case DIALOG_POJAZDY..(DIALOG_POJAZDY_RODZAJE+sizeof(pojazdy)):
      return vehicles_PojazdyResponse(playerid, dialogid, response, listitem, inputtext);
    case DIALOG_ARENASOLO_SELECT:
    {
      if (!response) return 1;
      else return solo_wybranaArena(playerid,listitem);
    }
    case DIALOG_CMDSEL:
    {
      if (!response || inputtext[0]!='/') return 1;
      new idx=1;
      new cmd[32];

      format(cmd,sizeof cmd,"_%s", strtok(inputtext, idx));
      for (new i=0;i<strlen(cmd);i++)
        cmd[i]=tolower(cmd[i]);

      CallLocalFunction(cmd,"d",playerid);
      return 1;
    }
    case DIALOG_WEAPON_QUICKBUY:
    {
      if (!response) return 1;
      else return PlayerQuickbuyWeapon(playerid,listitem);
    }
    case DIALOG_CONFIRM:
    {
      if (!response) return Msg(playerid,COLOR_ERROR,Kobieta(playerid)?("Zrezygnowalas"):("Zrezygnowales"));
      CallLocalFunction(pTemp[playerid][dialogCB], "d", playerid);
      return 1;
    }
    case DIALOG_WEAPON_SELECT:
    {
      if(!response) return 1;
      new wepid,wname[24];
      if (!response || sscanf(inputtext,"ds[24]",wepid,wname)) CallLocalFunction(pTemp[playerid][dialogCB], "dd", playerid,-1);

      CallLocalFunction(pTemp[playerid][dialogCB], "dd", playerid,wepid);
      return 1;
    }
    
    case DIALOG_HELP_RULES_REGISTER:
    {
      if(response == BUTTON_OK)
      {
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, __("Rejestracja konta na FullServerze!"), __("Twoje konto nie jest zarejestrowane! Podaj haslo ktore bedzie uzywane do logowania na to konto.\n\nThis account isn't registred. Please enter new password to create account with this nickname."), __("Dalej"), __("Porzuæ"));
      }
    }
    
    case DIALOG_HELP_RULES_GUEST:
    {
      if(response != BUTTON_OK)
      {
        HidePlayerDialog(playerid);
        FlashScreen(playerid,true); // zaslaniamy ekran (domyslnie bylo widac postac, gdzies zespawnowana)
        KickPlayer(playerid);
      }
    }
    
    case DIALOG_REGISTER:
    {
      if(response == BUTTON_OK)
      {
        if(pData[playerid][loggedIn]) { Msg(playerid, COLOR_ERROR, "Jesteœ ju¿ zarejestrowany || You're already logged in."); return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, __("Rejestracja konta na FullServerze!"), __("Twoje konto nie jest zarejestrowane! Podaj haslo ktore bedzie uzywane do logowania na to konto.\n\nThis account isn't registred. Please enter new password to create account with this nickname."), __("Dalej"), __("Porzuæ")); }
        if(PlayerAccountExists(playerid)) { 
          Msg(playerid, COLOR_ERROR, "Twój nick jest ju¿ zarejestrowany || Your nickname is already registred"); 
          return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, __("Rejestracja konta na FullServerze!"), __("Twoje konto nie jest zarejestrowane! Podaj haslo ktore bedzie uzywane do logowania na to konto.\n\nThis account isn't registred. Please enter new password to create account with this nickname."), __("Dalej"), __("Porzuæ")); 
         }
  
        if(!IsPasswordCorrect(inputtext)) { 
          Msg(playerid, COLOR_ERROR, "Has³o mo¿e sk³adaæ siê tylko z liter, cyfr i niektorych znakow specjalnych. Musi mieæ od 5 do 30 znaków."); 
          Msg(playerid, COLOR_ERROR, "Password may contain only letters, numbers and some of special characters. Password must be 5-30 characters."); 
          return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, __("Rejestracja konta na FullServerze!"), __("Twoje konto nie jest zarejestrowane! Podaj haslo ktore bedzie uzywane do logowania na to konto.\n\nThis account isn't registred. Please enter new password to create account with this nickname."), __("Dalej"), __("Porzuæ")); 
         }
        
        format(pTemp[playerid][tmpRegisterPass],56,"%s",inputtext);
        ShowPlayerDialog(playerid, DIALOG_REGISTER_STEP2, DIALOG_STYLE_INPUT, __("Rejestracja konta na FullServerze!"), __("Swietnie! Teraz podaj nam adres email ktory bedzie uzyty jezeli zapomnisz swojego hasla\n\nPlease enter email addres to restore your password when forgotten."), __("Rejestruj"), __("Porzuæ"));
        
      }
    }
    
    case DIALOG_REGISTER_STEP2:
    {
      if(response == BUTTON_OK)
      {
        if(!IsValidEmail(inputtext)) { Msg(playerid, COLOR_ERROR, "Proszê podaæ adres email w formie u¿ytkownik@host.domena || Please provide email in (user@host.domain)"); return ShowPlayerDialog(playerid, DIALOG_REGISTER_STEP2, DIALOG_STYLE_INPUT, __("Rejestracja konta na FullServerze!"), __("Swietnie! Teraz podaj nam adres email ktory bedzie uzyty jezeli zapomnisz swojego hasla\n\nPlease enter email addres to restore your password when forgotten."), __("Rejestruj"), __("Porzuæ")); }
        if(EmailExists(inputtext)) { Msg(playerid, COLOR_ERROR, "Na ten adres email jest juz zarejestrowane inne konto || This email address is already in use"); return ShowPlayerDialog(playerid, DIALOG_REGISTER_STEP2, DIALOG_STYLE_INPUT, __("Rejestracja konta na FullServerze!"), __("Swietnie! Teraz podaj nam adres email ktory bedzie uzyty jezeli zapomnisz swojego hasla\n\nPlease enter email addres to restore your password when forgotten."), __("Rejestruj"), __("Porzuæ")); }
        
        new params[128];
        format(params,128,"%s %s",pTemp[playerid][tmpRegisterPass],inputtext);
        if(!funcRejestracja(playerid,params)){
          HidePlayerDialog(playerid);
          FlashScreen(playerid,true); // zaslaniamy ekran (domyslnie bylo widac postac, gdzies zespawnowana)
          KickPlayer(playerid);
          return 0;
        }
        
        OnPlayerLogin(playerid);
        if(gmTemp[debugMode]) printf(" * DEBUG MODE: REGISTER AT LOGIN %s USING PASS %s",GetPlayerNick(playerid),pTemp[playerid][tmpRegisterPass]);
      }
    }

    case DIALOG_LOGIN:
    {
      if(response == BUTTON_OK)
      {
        if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
        {
          format(gstr, sizeof gstr, "%d blednych prob zalogowan na konto {b}%s{/b} z adresu IP {b}%s{/b}.", MAX_LOGIN_ATTEMPS, GetPlayerNick(playerid), GetPlayerIP(playerid));
          MSGToAdmins(COLOR_ERROR, gstr, false, LEVEL_ADMIN2);
          Msg(playerid, COLOR_ERROR, TXT(playerid, 10)); // Zbyt wiele prób logowañ, zosta³eœ wyrzucony z serwera.
          KickPlayer(playerid);
          return 1;
        }
        new escaped_nick[MAX_PLAYER_NAME+16];
        mysql_real_escape_string(GetPlayerNick(playerid),escaped_nick);
        format(buffer, sizeof buffer, "SELECT password FROM %s WHERE nick = '%s'", gmData[DB_players], escaped_nick);
        mysql_query(buffer);

        mysql_store_result();
        mysql_fetch_row(buffer);
        mysql_free_result();

        if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), buffer, true) != 0 && strlen(buffer))
        {
          pTemp[playerid][loginAttemps]++;
          ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, __("Witaj na FullServerze!"), __("Zaloguj sie uzywajac swojego hasla podanego podczas rejestracji.\n\nPlease login using password provided when creating this account."), __("Zaloguj"), __("Wyjdz"));

          Msg(playerid, COLOR_ERROR, __("Nieprawidlowe haslo.")); // Nieprawid³owe has³o.
          if (pTemp[playerid][loginAttemps]%3==0)
            Msg(playerid, COLOR_ERROR, __("Masz problem z zalogowaniem? Nie pamietasz hasla? Sproboj zreseowac haslo na {b}http://fullserver.eu/haslo{/b}"));
          return 1;
        }

        OnPlayerLogin(playerid);
        Msg(playerid, COLOR_INFO, TXT(playerid, 9)); // Zalogowano pomyœlnie, mi³ej gry!
        if(gmTemp[debugMode]) printf(" * DEBUG MODE: ACCESS AT LOGIN %s USING PASS %s",GetPlayerNick(playerid),inputtext);

      }
      else
      {
        HidePlayerDialog(playerid);
        FlashScreen(playerid,true); // zaslaniamy ekran (domyslnie bylo widac postac, gdzies zespawnowana)
        KickPlayer(playerid);
      }
    }

    case DIALOG_CONFIG_MAIN:
    {
      if(!IsAdmin(playerid)) {
      new buf[128];

      MSGToAdmins(COLOR_ERROR, "Prawdopodobny hacking attempt - proba obejscia autoryzacji na komende /config", true, LEVEL_ADMIN3);
      printf("Prawdopodobny hacking attempt - proba obejscia autoryzacji na komende /config");
      format(buf, sizeof buf, "Zwrocenie danych bezposrednio formularza, nick %s id %d", GetPlayerNick(playerid), playerid);
      MSGToAdmins(COLOR_ERROR, buf,true, LEVEL_ADMIN3);
      printf("%s", buf);
      return 1;
      }
      if(response == BUTTON_QUIT) return 1;
      switch(listitem)
      {
        case f_c_PCOLORS: ShowPlayerDialog(playerid, DIALOG_CONFIG_PCOLORS, DIALOG_STYLE_LIST, TXT(playerid, 95), GetDialogList(playerid, DIALOG_CONFIG_PCOLORS), __("Wybierz"), __("Wroc"));
        case f_c_CCOLORS: ShowPlayerDialog(playerid, DIALOG_CONFIG_CCOLORS, DIALOG_STYLE_LIST, TXT(playerid, 96), GetDialogList(playerid, DIALOG_CONFIG_CCOLORS), __("Wybierz"), __("Wroc"));
        case f_c_MAXPING: ShowPlayerDialog(playerid, DIALOG_CONFIG_MAXPING, DIALOG_STYLE_INPUT, TXT(playerid, 334), TXT(playerid, 79), __("Zapisz"), __("Wroc"));
        case f_c_ASETTINGS: ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS, DIALOG_STYLE_LIST, TXT(playerid, 337), GetDialogList(playerid, DIALOG_CONFIG_ASETTINGS), __("Zapisz"), __("Wroc"));
        case f_c_CENSORSHIP: ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP, DIALOG_STYLE_LIST, TXT(playerid, 386), GetDialogList(playerid, DIALOG_CONFIG_CENSORSHIP), __("Zapisz"), __("Wroc"));

        case f_c_CHATCOLORS:
        {
          if(gmData[chatColors])
          {
            gmData[chatColors] = false;
            Msg(playerid, COLOR_INFO, TXT(playerid, 401));
          }
          else
          {
            gmData[chatColors] = true;
            Msg(playerid, COLOR_INFO, TXT(playerid, 400));
          }

          SetConfigValueInt("chatcolors", BoolToInt(gmData[chatColors]));

          _config(playerid);
        }
      }
    }

    case DIALOG_CONFIG_PCOLORS:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) return _config(playerid);

      c_f_PlayerColors(playerid, listitem);
    }

    case DIALOG_CONFIG_CCOLORS:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) return _config(playerid);

      c_f_ChatColors(playerid, listitem);
    }

    case DIALOG_CONFIG_MAXPING:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_BACK) return _config(playerid);

      if(!IsNumeric(inputtext)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 122));

      new
       _maxPing = strval(inputtext);

      if(_maxPing < 50 || _maxPing > 9999) return Msg(playerid, COLOR_ERROR, TXT(playerid, 335));

      gmData[maxPing] = _maxPing;

      SetConfigValueInt("max_ping", _maxPing);

      format(buffer, sizeof buffer, TXT(playerid, 336), _maxPing);
      Msg(playerid, COLOR_INFO, buffer);

      _config(playerid);
    }

    case DIALOG_CONFIG_ASETTINGS:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) return _config(playerid);

      pTemp[playerid][tmpConfigAttraction] = listitem;

      format(buffer, sizeof buffer, TXT(playerid, 338), aData[listitem][aName]);
      ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_LIST, DIALOG_STYLE_LIST, buffer, GetDialogList(playerid, DIALOG_CONFIG_ASETTINGS_LIST), __("Wybierz"), __("Wroc"));
    }

    case DIALOG_CONFIG_CENSORSHIP:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_BACK) return _config(playerid);

      switch(listitem)
      {
        case f_c_c_ENABLEDISABLE:
        {
          if(gmData[censorship]) gmData[censorship] = false;
          else gmData[censorship] = true;

          SetConfigValueInt("censorship", BoolToInt(gmData[censorship]));

          if(gmData[censorship]) Msg(playerid, COLOR_INFO, TXT(playerid, 390));

          else Msg(playerid, COLOR_INFO, TXT(playerid, 391));

          OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
        }

        case f_c_c_LIST:
        {
          new
           szList[1024],
           File:hFile;

          hFile = fopen("FullServer/cenzura.ini", io_read);

          while(fread(hFile, buffer))
          {
            strcat(szList, buffer);
            strcat(szList, "\n");
          }

          fclose(hFile);

          strdel(szList, strlen(szList) - 1, strlen(szList));

          if(!strlen(szList))
          {
            strcat(szList, TXT(playerid, 392));
          }

          ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP_LIST, DIALOG_STYLE_LIST, TXT(playerid, 387), szList, __("Wroc"), "");
        }

        case f_c_c_ADDWORD:
        {
          ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP_ADD, DIALOG_STYLE_INPUT, TXT(playerid, 388), TXT(playerid, 393), __("Zapisz"), __("Wroc"));
        }

        case f_c_c_DELETEWORD:
        {
          new
           szList[1024],
           File:hFile;

          hFile = fopen("FullServer/cenzura.ini", io_read);

          while(fread(hFile, buffer))
          {
            strcat(szList, buffer);
            strcat(szList, "\n");
          }

          fclose(hFile);

          strdel(szList, strlen(szList) - 1, strlen(szList));

          if(!strlen(szList))
          {
            Msg(playerid, COLOR_ERROR, TXT(playerid, 394));
            SetTimerEx("NoMoreWordsDialogDelayFix", 50, false, "i", playerid);
          }
          else
          {
            ShowPlayerDialog(playerid, DIALOG_CONFIG_CENSORSHIP_DELETE, DIALOG_STYLE_LIST, TXT(playerid, 389), szList, __("Zapisz"), __("Wroc"));
          }
        }
      }
    }

    case DIALOG_CONFIG_CENSORSHIP_LIST:
    {
      if(!IsAdmin(playerid)) return 1;
      OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
    }

    case DIALOG_CONFIG_CENSORSHIP_ADD:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
      }

      if(!IsCorrectWordForCensorship(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 395));
        OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_ADDWORD, " ");

        return 1;
      }

      new
       File:hFile;

      if(IsWordCensored(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 398));
        OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_ADDWORD, " ");

        return 1;
      }

      hFile = fopen("FullServer/cenzura.ini", io_append);

      format(buffer, sizeof buffer, "%s\n", inputtext);
      fwrite(hFile, buffer);

      fclose(hFile);

      format(buffer, sizeof buffer, TXT(playerid, 396), inputtext);
      Msg(playerid, COLOR_INFO, buffer);
      OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_ADDWORD, " ");

      LoadCensoredWords();
    }

    case DIALOG_CONFIG_CENSORSHIP_DELETE:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_CENSORSHIP, " ");
      }

      if(!IsCorrectWordForCensorship(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 395));
        OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_DELETEWORD, " ");

        return 1;
      }

      new
       File:hFile,
       File:hFileTmp,
       __count = 0;

      hFile = fopen("FullServer/cenzura.ini", io_read);
      hFileTmp = fopen("FullServer/cenzura.tmp", io_write);

      while(fread(hFile, buffer))
      {
        if(__count++ != listitem)
        {
          fwrite(hFileTmp, buffer);
        }
      }

      fclose(hFileTmp);
      fclose(hFile);

      hFile = fopen("FullServer/cenzura.ini", io_write);
      hFileTmp = fopen("FullServer/cenzura.tmp", io_read);

      while(fread(hFileTmp, buffer))
      {
        fwrite(hFile, buffer);
      }

      fclose(hFileTmp);
      fclose(hFile);
      fremove("FullServer/cenzura.tmp");

      format(buffer, sizeof buffer, TXT(playerid, 347), inputtext);
      Msg(playerid, COLOR_INFO, buffer);
      OnDialogResponse(playerid, DIALOG_CONFIG_CENSORSHIP, 1, f_c_c_DELETEWORD, " ");

      LoadCensoredWords();
    }

    case DIALOG_CONFIG_ASETTINGS_LIST:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, f_c_ASETTINGS, " ");
      }

      switch(listitem)
      {
        case f_c_a_NAME:
        {
          format(buffer, sizeof buffer, TXT(playerid, 339), aData[listitem][aName]);
          ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_NAME, DIALOG_STYLE_INPUT, buffer, TXT(playerid, 79), __("Zapisz"), __("Wroc"));
        }

        case f_c_a_QUEUE:
        {
          format(buffer, sizeof buffer, TXT(playerid, 340), aData[listitem][aName]);
          ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_QUEUE, DIALOG_STYLE_INPUT, buffer, TXT(playerid, 79), __("Zapisz"), __("Wroc"));
        }

        case f_c_a_TIME:
        {
          format(buffer, sizeof buffer, TXT(playerid, 341), aData[listitem][aName]);
          ShowPlayerDialog(playerid, DIALOG_CONFIG_ASETTINGS_TIME, DIALOG_STYLE_INPUT, buffer, TXT(playerid, 79), __("Zapisz"), __("Wroc"));
        }
      }
    }

    case DIALOG_CONFIG_ASETTINGS_NAME:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
      }

      if(strlen(inputtext) < 1 || strlen(inputtext) > 14)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 342)); // D³ugoœæ nazwy musi siê mieœciæ w przedziale 1 - 14.
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_NAME, " ");
      }

      if(!CheckTildes(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_NAME, " ");
      }

      if(SpaceCheck(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 348)); // Nazwa atrakcji nie mo¿e zawieraæ spacji.
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_NAME, " ");
      }

      copy(inputtext, aData[pTemp[playerid][tmpConfigAttraction]][aName]);
      SaveAttractionData(pTemp[playerid][tmpConfigAttraction]);

      format(buffer, sizeof buffer, TXT(playerid, 343), inputtext);
      Msg(playerid, COLOR_INFO, buffer); // Nazwa atrakcji zosta³a zmieniona na XXX.
      OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
    }

    case DIALOG_CONFIG_ASETTINGS_QUEUE:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
      }

      if(!IsNumeric(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 122)); // Podana wartoœæ musi byæ numeryczna.
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_QUEUE, " ");
      }

      new
       _queue = strval(inputtext);

      if(_queue < GetAttractionMinimumQueueValue(pTemp[playerid][tmpConfigAttraction]) || _queue > GetAttractionMaximumQueueValue(pTemp[playerid][tmpConfigAttraction]))
      {
        format(buffer, sizeof buffer, TXT(playerid, 344), GetAttractionMinimumQueueValue(pTemp[playerid][tmpConfigAttraction]), GetAttractionMaximumQueueValue(pTemp[playerid][tmpConfigAttraction]));
        Msg(playerid, COLOR_ERROR, buffer); // Wielkoœæ kolejki nie mo¿e byæ mniejsza od xxx i wiêksza ni¿ xxx.
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_QUEUE, " ");
      }

      aData[pTemp[playerid][tmpConfigAttraction]][aStartPlayers] = _queue;
      SaveAttractionData(pTemp[playerid][tmpConfigAttraction]);

      format(buffer, sizeof buffer, TXT(playerid, 345), aData[pTemp[playerid][tmpConfigAttraction]][aName], _queue);
      Msg(playerid, COLOR_INFO, buffer); // Wielkoœæ kolejki XXX zosta³a zmieniona na XXX.
      OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
    }

    case DIALOG_CONFIG_ASETTINGS_TIME:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
      }

      if(!IsNumeric(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 122)); // Podana wartoœæ musi byæ numeryczna.
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_TIME, " ");
      }

      new
       _time = strval(inputtext);

      if(_time < 1 || _time > 120)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 397)); // Wartoœæ czasu musi siê mieœciæ w przedziale 1 - 120 (sekund).
        return OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS_LIST, 1, f_c_a_TIME, " ");
      }

      aData[pTemp[playerid][tmpConfigAttraction]][aStartingTime] = _time;
      SaveAttractionData(pTemp[playerid][tmpConfigAttraction]);

      format(buffer, sizeof buffer, TXT(playerid, 346), aData[pTemp[playerid][tmpConfigAttraction]][aName], _time);
      Msg(playerid, COLOR_INFO, buffer); // Czas startu XXX zosta³ zmieniony na XXX.
      OnDialogResponse(playerid, DIALOG_CONFIG_ASETTINGS, 1, pTemp[playerid][tmpConfigAttraction], " ");
    }

    case DIALOG_CONFIG_PCOLORS_SET:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 0, " ");
      }

      if(!IsHex(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 14)); // Nieprawid³owa wartoœæ, wprowadŸ kod koloru w systemie szesnastkowym RGB, np. 44FF2C.
        c_f_PlayerColors(playerid, pTemp[playerid][lastDialog]);
        return 1;
      }

      switch(pTemp[playerid][lastDialogItem])
      {
        case 0: gmData[color_normalUser] = HexToInt(inputtext);
        case 1: gmData[color_fsUser] = HexToInt(inputtext);
        case 2: gmData[color_vipUser] = HexToInt(inputtext);
        case 3: gmData[color_gmUser] = HexToInt(inputtext);
        case 4: gmData[color_lvl1User] = HexToInt(inputtext);
        case 5: gmData[color_lvl2User] = HexToInt(inputtext);
        case 6: gmData[color_adminUser] = HexToInt(inputtext);
      }

      //SaveConfig();
      foreach(i) SetPlayerProperColor(i);

      format(buffer, sizeof buffer, TXT(playerid, 15), inputtext, inputtext);
      Msg(playerid, COLOR_INFO, buffer); // Kolor zosta³ zmieniony na %s.
      OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 0, " ");
    }

    case DIALOG_CONFIG_CCOLORS_SET:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 1, " ");
      }

      if(!IsHex(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 14)); // Nieprawid³owa wartoœæ, wprowadŸ kod koloru w systemie szesnastkowym RGB, np. 44FF2C.
        c_f_ChatColors(playerid, pTemp[playerid][lastDialog]);
        return 1;
      }

      switch(pTemp[playerid][lastDialogItem])
      {
        // Normal
        case 0: gmData[color_chatInfo] = HexToInt(inputtext);
        case 1: gmData[color_chatInfo2] = HexToInt(inputtext);
        case 2: gmData[color_chatInfo3] = HexToInt(inputtext);
        case 3: gmData[color_chatError] = HexToInt(inputtext);
        case 4: gmData[color_joinInfo] = HexToInt(inputtext);
        case 5: gmData[color_leaveInfo] = HexToInt(inputtext);
        case 6: gmData[color_chatGM] = HexToInt(inputtext);
        case 7: gmData[color_chatAdmin] = HexToInt(inputtext);
        case 8: gmData[color_chatAdmin3] = HexToInt(inputtext);
        case 9: gmData[color_chatPM] = HexToInt(inputtext);
        case 10: gmData[color_chatIC] = HexToInt(inputtext);
        case 11: gmData[color_chatME] = HexToInt(inputtext);
        case 12: gmData[color_vipSay] = HexToInt(inputtext);
        case 13: gmData[color_chatVip] = HexToInt(inputtext);

        // Highlight
        case 14: gmData[color_chatInfo_HL] = HexToInt(inputtext);
        case 15: gmData[color_chatInfo2_HL] = HexToInt(inputtext);
        case 16: gmData[color_chatInfo3_HL] = HexToInt(inputtext);
        case 17: gmData[color_chatError_HL] = HexToInt(inputtext);
        case 18: gmData[color_joinInfo_HL] = HexToInt(inputtext);
        case 19: gmData[color_leaveInfo_HL] = HexToInt(inputtext);
        case 20: gmData[color_chatGM_HL] = HexToInt(inputtext);
        case 21: gmData[color_chatAdmin_HL] = HexToInt(inputtext);
        case 22: gmData[color_chatAdmin3_HL] = HexToInt(inputtext);
        case 23: gmData[color_chatPM_HL] = HexToInt(inputtext);
        case 24: gmData[color_chatIC_HL] = HexToInt(inputtext);
        case 25: gmData[color_chatME_HL] = HexToInt(inputtext);
        case 26: gmData[color_vipSay_HL] = HexToInt(inputtext);
        case 27: gmData[color_chatVip_HL] = HexToInt(inputtext);
      }

      format(buffer, sizeof buffer, TXT(playerid, 15), inputtext, inputtext);
      Msg(playerid, COLOR_INFO, buffer); // Kolor zosta³ zmieniony na %s.
      OnDialogResponse(playerid, DIALOG_CONFIG_MAIN, 1, 1, " ");
    }

    case DIALOG_NICK_CHANGE:
    {
      if(response == BUTTON_QUIT) return 1;

      if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 59)); // Zbyt wiele prób potwierdzenia has³a, zosta³eœ wyrzucony z serwera.
        KickPlayer(playerid);

        return 1;
      }

      if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), GetPlayerAccountData(playerid, "password"), true) != 0)
      {
        pTemp[playerid][loginAttemps]++;
        ShowPlayerDialog(playerid, DIALOG_NICK_CHANGE, DIALOG_STYLE_PASSWORD, TXT(playerid, 55), TXT(playerid, 56), TXT(playerid, 57), TXT(playerid, 58));
        return Msg(playerid, COLOR_ERROR, TXT(playerid, 5)); // Nieprawid³owe has³o.
      }

      new
       oldNick[24];

      copy(GetPlayerNick(playerid), oldNick);

      strcat(pTemp[playerid][newNick], pTemp[playerid][newNick]);
      SetPlayerName(playerid, pTemp[playerid][newNick]);

      SetPlayerAccountDataString(playerid, "nick", pTemp[playerid][newNick]);
      SetPlayerAccountDataString(playerid, "password", MD5_Hash(PassHash(pTemp[playerid][newNick], inputtext)));
      SetPlayerAccountDataString(playerid, "next_nick_change", "NOW() + INTERVAL 1 DAY", true);

      format(buffer, sizeof buffer, TXT(playerid, 18), pTemp[playerid][newNick]);
      Msg(playerid, COLOR_INFO, buffer); // Twój nick zosta³ zmieniony na "xxx". Nastêpna zmiana bêdzie mo¿liwa po 24 godzinach.

      foreach(i)
      {
        if(i != playerid)
        {
          format(buffer, sizeof buffer, TXT(i, 19), oldNick, pTemp[playerid][newNick]);
          Msg(i, COLOR_INFO, buffer); // "xxx" zmieni³ swój nick na "xxx".
        }
      }
      format(buffer,sizeof buffer, "Zmiana nicku, accountid: %d, playerid:%d starynick:%s nowynick:%s ip:%s", pData[playerid][accountID], playerid, oldNick, pTemp[playerid][newNick], GetPlayerIP(playerid));
      OutputLog(LOG_PLAYERS,buffer);

      pTemp[playerid][isFS] = false;
      if(strfind(pTemp[playerid][newNick], "[FS]", true) != -1 || strfind(pTemp[playerid][newNick], ".fs", true) != -1 || strfind(pTemp[playerid][newNick], "fs.", true) != -1
      || strfind(pTemp[playerid][newNick], "FS_", true) != -1  || strfind(pTemp[playerid][newNick], "_FS", true) != -1){
          pTemp[playerid][isFS] = true;
          if (pData[playerid][pAttraction]==A_NONE && gmData[artefactOwner]!=playerid) SetPlayerProperColor(playerid,true);
      }
      format(pTemp[playerid][properName],MAX_PLAYER_NAME,"%s", pTemp[playerid][newNick]);

      UpdatePlayerNickTD(playerid);

      return 1;

    }

    case DIALOG_PASSWORD_CHANGE:
    {
      if(response == BUTTON_QUIT) return 1;

      if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 59)); // Zbyt wiele prób potwierdzenia has³a, zosta³eœ wyrzucony z serwera.
        KickPlayer(playerid);

        return 1;
      }

      if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), GetPlayerAccountData(playerid, "password"), true) != 0)
      {
        pTemp[playerid][loginAttemps]++;
        ShowPlayerDialog(playerid, DIALOG_PASSWORD_CHANGE, DIALOG_STYLE_INPUT, TXT(playerid, 349), TXT(playerid, 350), TXT(playerid, 57), TXT(playerid, 58));
        return Msg(playerid, COLOR_ERROR, TXT(playerid, 5)); // Nieprawid³owe has³o.
      }

      SetPlayerAccountDataString(playerid, "password", MD5_Hash(PassHash(GetPlayerNick(playerid), pTemp[playerid][newPassword])));

      format(buffer, sizeof buffer, TXT(playerid, 351), pTemp[playerid][newPassword]);
      Msg(playerid, COLOR_INFO, buffer); // Twoje has³o zosta³o zmienione na "xxx". Dobrze je zapamiêtaj!
      format(buffer,sizeof buffer, "Zmiana hasla, accountid: %d, playerid:%d loginattemps:%d ip:%s", pData[playerid][accountID], playerid, pTemp[playerid][loginAttemps], GetPlayerIP(playerid));
      OutputLog(LOG_PLAYERS,buffer);
      return 1;
    }

    case DIALOG_EMAIL_FORCE_CHANGE:
    {
      if(response == BUTTON_QUIT) KickPlayer(playerid);
      if(!IsValidEmail(inputtext)) {
        Msg(playerid, COLOR_ERROR, "Proszê podaæ poprawny adres email w formie u¿ytkownik@host.domena"); // Has³o mo¿e sk³adaæ siê tylko z liter i cyfr, oraz musi mieæ od 5 do 30 znaków.
        return ShowPlayerDialog(playerid, DIALOG_EMAIL_FORCE_CHANGE, DIALOG_STYLE_INPUT, "Podaj adres email", "Z powodu dodania mo¿liwoœci utworzenia konta na forum bazuj¹c na koncie z serwera wymagamy podania adresu email w procesie rejestracji na serwerze. Uzupe³nij swoje konto o wymagane dane", "Dalej", TXT(playerid, 58));
      }
      copy(inputtext, pTemp[playerid][newEmail]);

        pTemp[playerid][loginAttemps]=0;

      SetPlayerAccountDataString(playerid, "email", pTemp[playerid][newEmail]);

      format(buffer, sizeof buffer, "Twój email zosta³ zmieniony na {b}%s{/b}", pTemp[playerid][newEmail]);
      Msg(playerid, COLOR_INFO, buffer);
      format(buffer,sizeof buffer, "Zmiana emailu, accountid: %d, playerid:%d loginattemps:%d ip:%s", pData[playerid][accountID], playerid, pTemp[playerid][loginAttemps], GetPlayerIP(playerid));
      OutputLog(LOG_PLAYERS,buffer);
      return 1;
    }
    case DIALOG_EMAIL_CHANGE:
    {
      if(response == BUTTON_QUIT) return 1;

      if(pTemp[playerid][loginAttemps] >= MAX_LOGIN_ATTEMPS)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 59)); // Zbyt wiele prób potwierdzenia has³a, zosta³eœ wyrzucony z serwera.
        KickPlayer(playerid);

        return 1;
      }

      if(strcmp(MD5_Hash(PassHash(GetPlayerNick(playerid), inputtext)), GetPlayerAccountData(playerid, "password"), true) != 0)
      {
        pTemp[playerid][loginAttemps]++;
        ShowPlayerDialog(playerid, DIALOG_PASSWORD_CHANGE, DIALOG_STYLE_INPUT, TXT(playerid, 349), TXT(playerid, 350), TXT(playerid, 57), TXT(playerid, 58));
        return Msg(playerid, COLOR_ERROR, TXT(playerid, 5)); // Nieprawid³owe has³o.
      }

        SetPlayerAccountDataString(playerid, "email", pTemp[playerid][newEmail]);

      format(buffer, sizeof buffer, "Twój email zosta³ zmieniony na {b}%s{/b}", pTemp[playerid][newEmail]);
      Msg(playerid, COLOR_INFO, buffer);
      format(buffer,sizeof buffer, "Zmiana emailu, accountid: %d, playerid:%d loginattemps:%d ip:%s", pData[playerid][accountID], playerid, pTemp[playerid][loginAttemps], GetPlayerIP(playerid));
      OutputLog(LOG_PLAYERS,buffer);
      return 1;
    }

    case DIALOG_BAN_CONFIRM:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_QUIT) return 1;

      new
       targetplayerid =   pTemp[playerid][tmpTargetPlayerID],
       banTime =      pTemp[playerid][tmpBanTime],
       period =       pTemp[playerid][tmpPeriod],
       reason[128], escaped_reason[140],
       banAccountID,
       szBanAccountName[24];

      copy(pTemp[playerid][tmpReason], reason);
      mysql_real_escape_string(reason,escaped_reason);

      if(targetplayerid == -1)
      {
        banAccountID = GetAccountID(pTemp[playerid][tmpBanAccountName]);
        copy(pTemp[playerid][tmpBanAccountName], szBanAccountName);
      }
      else
      {
        banAccountID = pData[targetplayerid][accountID];
        copy(GetPlayerProperName(targetplayerid), szBanAccountName);
      }


      format(buffer, sizeof buffer, "INSERT INTO %s (player_banned, player_given, date_created, date_end, reason) VALUES (%i, %i, NOW(), NOW() + INTERVAL %i %s, '%s')",
        gmData[DB_bans],
        banAccountID,
        pData[playerid][accountID],
        banTime,
        GetMySQLNameOfPeriod(period),
        escaped_reason
      );

      mysql_query(buffer);

      if(targetplayerid != -1)
      {
          SetPlayerAccountDataString(targetplayerid, "ban_count", "ban_count + 1", true);
      }
      else
      {
        format(buffer, sizeof buffer, "UPDATE %s SET ban_count = ban_count + 1 WHERE id = %i", gmData[DB_players], banAccountID);
        mysql_query(buffer);
      }

      SetServerStatString("ban_count", "value + 1", true);

      format(buffer, sizeof buffer, "{ff0000}Gracz {ffffff}%s{ff0000} zostal zbanowany na {ff9090}%i %s.", szBanAccountName, banTime, GetPeriodName(playerid, period, banTime));
      foreach(i)
          Msg(i,COLOR_ERROR, buffer);
      format(buffer, sizeof buffer, "{ff0000}Powod: {ffffff}%s", reason);
      foreach(i)
          Msg(i,COLOR_ERROR, buffer);

      if (pData[playerid][adminLevel]!=LEVEL_ADMINHIDDEN) {
        format(buffer, sizeof buffer, "{ff0000}Zbanowal%s {ffffff}%s", SkinKobiecy(GetPlayerSkin(playerid))?("a"):(""), GetPlayerNick(playerid));
        foreach(i)
          Msg(i,COLOR_ERROR, buffer);
      }


      if(targetplayerid != -1)
      {
          GivePlayerScore(targetplayerid,-100,"ban");
          GameTextForPlayer(targetplayerid, "~r~ZROB SCREEN ~w~- ~r~~h~F8", 10000, 3);
        SendClientMessage(targetplayerid,-1, " ");
        Msg(targetplayerid,COLOR_INFO,"Jesli uwazasz ze ten ban jest niesluszny, badz tez bedziesz staral sie o wczesniejsze jego zdjecie", false);
        Msg(targetplayerid,COLOR_INFO,"to koniecznie zrob {b}screenshot{/b} wciskajac teraz klawisz {b}F8{/b}, nastepnie odwiedz nasze forum", false);
        Msg(targetplayerid,COLOR_INFO,"pod adresem {b}WWW.FULLSERVER.EU{/b} i zloz tam podanie o odbanowanie.");
        if (Audio_IsClientConnected(targetplayerid))
          Audio_Play(targetplayerid,AUDIOID_BAN, false, false, true);
        KickPlayer(targetplayerid);
      }
    }

    case DIALOG_BANIP_CONFIRM:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_QUIT) return 1;

      new
       banTime =      pTemp[playerid][tmpBanTime],
       period =       pTemp[playerid][tmpPeriod],
       reason[128],
       szIP[16];

      copy(pTemp[playerid][tmpReason], reason);
      copy(pTemp[playerid][tmpBanIP], szIP);

      for(new i = 0; i < strlen(szIP); i++)
      {
        if(szIP[i] == '*') szIP[i] = '%';
      }
      new escaped_reason[128];
      mysql_real_escape_string(reason,escaped_reason);
      format(buffer, sizeof buffer, "INSERT INTO %s (ip, player_given, date_created, date_end, reason) VALUES ('%s', %i, NOW(), NOW() + INTERVAL %i %s, '%s')",
        gmData[DB_ipbans],
        szIP,
        pData[playerid][accountID],
        banTime,
        GetMySQLNameOfPeriod(period),
        escaped_reason
      );

      mysql_query(buffer);

      SetServerStatString("ban_count", "value + 1", true);

      format(buffer, sizeof buffer, TXT(playerid, 189), pTemp[playerid][tmpBanIP], banTime, GetPeriodName(playerid, period, banTime), reason);
      Msg(playerid, COLOR_INFO, buffer); // Adres IP "xxx" zosta³ zbanowany na xxx xxx. Powód: xxx
    }

    case DIALOG_UNBAN_CONFIRM:
    {
      if(!IsAdmin(playerid)) return 1;

      if(response == BUTTON_QUIT) return 1;

      format(buffer, sizeof buffer, "DELETE FROM %s WHERE player_banned = %i",
        gmData[DB_bans],
        GetAccountID(pTemp[playerid][tmpBanAccountName])
      );

      mysql_query(buffer);

      format(buffer, sizeof buffer, TXT(playerid, 184), pTemp[playerid][tmpBanAccountName]);
      Msg(playerid, COLOR_INFO, buffer); // Konto "xxx" zosta³o odbanowane.
    }

    case DIALOG_UNBANIP_CONFIRM:
    {
      if(!IsAdmin(playerid)) return 1;
      if(response == BUTTON_QUIT) return 1;

      new
       szIP[16];

      copy(pTemp[playerid][tmpBanIP], szIP);

      for(new i = 0; i < strlen(szIP); i++)
      {
        if(szIP[i] == '*') szIP[i] = '%';
      }

      format(buffer, sizeof buffer, "DELETE FROM %s WHERE ip = '%s'",
        gmData[DB_ipbans],
        szIP
      );

      mysql_query(buffer);

      format(buffer, sizeof buffer, TXT(playerid, 195), pTemp[playerid][tmpBanIP]);
      Msg(playerid, COLOR_INFO, buffer); // Adres IP "xxx" zosta³ usuniêty z listy zbanowanych.
    }

    case DIALOG_HELP_MAIN:
    {
      if(response == BUTTON_QUIT) return 1;

      switch(listitem)
          {
              case f_h_RULES:   ShowPlayerDialog(playerid, DIALOG_HELP_RULES, DIALOG_STYLE_MSGBOX, TXT(playerid, 80), GetDialogContent(DIALOG_HELP_RULES), __("Wyjdz"), __("Wroc"));
        case f_h_RESPECT: ShowPlayerDialog(playerid, DIALOG_HELP_RESPECT, DIALOG_STYLE_MSGBOX, TXT(playerid, 81), GetDialogContent(DIALOG_HELP_RESPECT), __("Wyjdz"), __("Wroc"));
        case f_h_GP:      ShowPlayerDialog(playerid, DIALOG_HELP_GPOINTS, DIALOG_STYLE_MSGBOX, "Panel pomocy > Game Points", GetDialogContent(DIALOG_HELP_GPOINTS), __("Wyjdz"), __("Wroc"));
        case f_h_STARS:   ShowPlayerDialog(playerid, DIALOG_HELP_STARS, DIALOG_STYLE_MSGBOX, TXT(playerid, 82), GetDialogContent(DIALOG_HELP_STARS), __("Wyjdz"), __("Wroc"));
        case f_h_VIP:   ShowPlayerDialog(playerid, DIALOG_HELP_VIP, DIALOG_STYLE_MSGBOX, TXT(playerid, 83), GetDialogContent(DIALOG_HELP_VIP), __("Wyjdz"), __("Wroc"));
        case f_h_AUTHOR:  ShowPlayerDialog(playerid, DIALOG_HELP_AUTHOR, DIALOG_STYLE_MSGBOX, TXT(playerid, 84), GetDialogContent(DIALOG_HELP_AUTHOR), __("Wyjdz"), __("Wroc"));
        case f_h_CMD:   ShowPlayerDialog(playerid, DIALOG_HELP_CMD, DIALOG_STYLE_LIST, TXT(playerid, 85), GetDialogList(playerid, DIALOG_HELP_CMD), __("Wybierz"), __("Wroc"));
      }
    }

    case DIALOG_HELP_RULES..DIALOG_HELP_AUTHOR,DIALOG_HELP_GPOINTS:
    {
      if(response == 1) return 1;
      else _pomoc(playerid);
    }

    case DIALOG_HELP_CMD:
    {
      if(response == BUTTON_BACK) return _pomoc(playerid);

      switch(listitem)
      {
        case f_h__GENERAL:    ShowPlayerDialog(playerid, DIALOG_HELP__GENERAL, DIALOG_STYLE_MSGBOX, TXT(playerid, 86), GetDialogContent(DIALOG_HELP__GENERAL), __("Wyjdz"), __("Wroc"));
        case f_h__ATRACTIONS: ShowPlayerDialog(playerid, DIALOG_HELP__ATRACTIONS, DIALOG_STYLE_MSGBOX, TXT(playerid, 87), GetDialogContent(DIALOG_HELP__ATRACTIONS), __("Wyjdz"), __("Wroc"));
        case f_h__ACCOUNT:    ShowPlayerDialog(playerid, DIALOG_HELP__ACCOUNT, DIALOG_STYLE_MSGBOX, TXT(playerid, 88), GetDialogContent(DIALOG_HELP__ACCOUNT), __("Wyjdz"), __("Wroc"));
        case f_h__HOUSES:   ShowPlayerDialog(playerid, DIALOG_HELP__HOUSES, DIALOG_STYLE_MSGBOX, TXT(playerid, 89), GetDialogContent(DIALOG_HELP__HOUSES), __("Wyjdz"), __("Wroc"));
        case f_h__RESPECT:    return _rcmd(playerid);// ShowPlayerDialog(playerid, DIALOG_HELP__RESPECT, DIALOG_STYLE_MSGBOX, TXT(playerid, 90), GetDialogContent(DIALOG_HELP__RESPECT), __("Wyjdz"), __("Wroc"));
        case f_h__TELEPORTS:    ShowPlayerDialog(playerid, DIALOG_HELP__TELEPORTS, DIALOG_STYLE_MSGBOX, TXT(playerid, 91), GetDialogContent(DIALOG_HELP__TELEPORTS), __("Wyjdz"), __("Wroc"));
        case f_h__ANIMATIONS: return _anims(playerid);//ShowPlayerDialog(playerid, DIALOG_HELP__ANIMATIONS, DIALOG_STYLE_MSGBOX, TXT(playerid, 92), GetDialogContent(DIALOG_HELP__ANIMATIONS), __("Wyjdz"), __("Wroc"));
        case f_h__VIP:      ShowPlayerDialog(playerid, DIALOG_HELP__VIP, DIALOG_STYLE_MSGBOX, TXT(playerid, 93), GetDialogContent(DIALOG_HELP__VIP), __("Wyjdz"), __("Wroc"));
      }
    }

    case DIALOG_HELP__GENERAL..DIALOG_HELP__ADMIN:
    {
      if(response == 1) return 1;
      else {
        OnDialogResponse(playerid, DIALOG_HELP_MAIN, BUTTON_NEXT, f_h_CMD, "");
      }
    }

    case DIALOG_BANK_MAIN:
    {
      if(response == BUTTON_QUIT) return 1;

      switch(listitem)
          {
              case f_b_BALANCE:
        {
          format(buffer, sizeof buffer, TXT(playerid, 117), GetPlayerAccountData(playerid, "bank_money"));
          ShowPlayerDialog(playerid, DIALOG_BANK_BALANCE, DIALOG_STYLE_MSGBOX, TXT(playerid, 111), buffer, __("Wyjdz"), __("Wroc"));
        }
        case f_b_DEPOSIT: ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INPUT, DIALOG_STYLE_INPUT, TXT(playerid, 112), TXT(playerid, 118), TXT(playerid, 119), __("Wroc"));
        case f_b_DEPOSIT_ALL:
        {
            if(GetPlayerMoney(playerid)>2100000000)
          {
            Msg(playerid,COLOR_ERROR,"Nie mozesz wykonywaæ operacji bankowych na kowatch wiêkszych ni¿ {b}$2100000000{/b}");
            return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
          }
            if((StringToInt(GetPlayerAccountData(playerid, "bank_money")) + GetPlayerMoney(playerid))>2100000000)
          {
              Msg(playerid,COLOR_ERROR,"Nie mozesz trzymaæ w banku wiêcej ni¿ $2100000000");
            return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
          }
            format(buffer, sizeof buffer, TXT(playerid, 125), GetPlayerMoney(playerid));
            ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 112), buffer, __("Wyjdz"), __("Wroc"));

            AddPlayerAccountData(playerid, "bank_money", GetPlayerMoney(playerid));
          GivePlayerMoney(playerid, -GetPlayerMoney(playerid));
          }
        case f_b_WITHDRAW: ShowPlayerDialog(playerid, DIALOG_BANK_WITHDRAW_INPUT, DIALOG_STYLE_INPUT, TXT(playerid, 113), TXT(playerid, 120), TXT(playerid, 119), __("Wroc"));
        case f_b_TRANSFER: ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_STEP1, DIALOG_STYLE_INPUT, TXT(playerid, 115), TXT(playerid, 121), TXT(playerid, 119), __("Wroc"));
      }
    }

    case DIALOG_BANK_BALANCE:
    {
      if(response == 1) return 1;
      else _bankomat(playerid);
    }

    case DIALOG_BANK_DEPOSIT_INPUT:
    {
      if(response == BUTTON_BACK) return _bankomat(playerid);

      if(!IsNumeric(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
      }

      new
       amount = StringToInt(inputtext);

      if(amount < 0)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 123));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
      }

      if(amount > GetPlayerMoney(playerid))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 124));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
      }

        if(amount>2100000000)
      {
        Msg(playerid,COLOR_ERROR,"Nie mozesz wykonywaæ operacji bankowych na kowatch wiêkszych ni¿ {b}$2100000000{/b}");
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
      }
      if((StringToInt(GetPlayerAccountData(playerid, "bank_money")) + amount)>2100000000)
      {
          Msg(playerid,COLOR_ERROR,"Nie mozesz trzymaæ w banku wiêcej ni¿ $2100000000");
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_DEPOSIT, "");
      }

      AddPlayerAccountData(playerid, "bank_money", amount);
      GivePlayerMoney(playerid, -amount);

      format(buffer, sizeof buffer, TXT(playerid, 125), amount);
      ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 112), buffer, __("Wyjdz"), __("Wroc"));
    }

    case DIALOG_BANK_DEPOSIT_INFO:
    {
      if(response == 1) return 1;
      else _bankomat(playerid);
    }

    case DIALOG_BANK_WITHDRAW_INPUT:
    {
      if(response == BUTTON_BACK) return _bankomat(playerid);

      if(!IsNumeric(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
      }

      new
       amount = StringToInt(inputtext);

      if(amount < 0)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 123));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
      }

      new
       balance = StringToInt(GetPlayerAccountData(playerid, "bank_money"));

      if(amount > balance)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 126));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
      }

      if(amount>2100000000)
      {
        Msg(playerid,COLOR_ERROR,"Nie mozesz wykonywaæ operacji bankowych na kowatch wiêkszych ni¿ {b}$2100000000{/b}");
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
      }

      if(GetPlayerMoney(playerid)+amount>800000)
      {
                Msg(playerid,COLOR_ERROR,"Nie mozesz przechowywaæ w portfelu wiêcej ni¿ {b}$800000{/b}");
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_WITHDRAW, "");
      }

      MinusPlayerAccountData(playerid, "bank_money", amount);
      GivePlayerMoney(playerid, amount, true);

      format(buffer, sizeof buffer, TXT(playerid, 127), amount);
      ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 112), buffer, __("Wyjdz"), __("Wroc"));
    }

    case DIALOG_BANK_WITHDRAW_INFO:
    {
      if(response == 1) return 1;
      else _bankomat(playerid);
    }

    case DIALOG_BANK_TRANSFER_STEP1:
    {
      if(response == BUTTON_BACK) return _bankomat(playerid);

      if(strlen(inputtext) < 3 || strlen(inputtext) > 24 || !IsNickCorrect(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 128));
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_TRANSFER, "");
      }

      if(!AccountExists(inputtext))
      {
        format(buffer, sizeof buffer, TXT(playerid, 129), inputtext);
        Msg(playerid, COLOR_ERROR, buffer);
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_TRANSFER, "");
      }

      copy(inputtext, pTemp[playerid][bankTransferAName]);
      pTemp[playerid][bankTransferAID] = GetAccountID(inputtext);

      ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_STEP2, DIALOG_STYLE_INPUT, TXT(playerid, 116), TXT(playerid, 130), TXT(playerid, 119), __("Wroc"));
    }

    case DIALOG_BANK_TRANSFER_STEP2:
    {
      if(response == BUTTON_BACK) {
        return OnDialogResponse(playerid, DIALOG_BANK_MAIN, BUTTON_NEXT, f_b_TRANSFER, "");
      }

      if(!IsNumeric(inputtext))
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 122));
        return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
      }

      new
       amount = StringToInt(inputtext);

      if(amount < 0)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 123));
        return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
      }

      new
       balance = StringToInt(GetPlayerAccountData(playerid, "bank_money"));

      if(amount > balance)
      {
        Msg(playerid, COLOR_ERROR, TXT(playerid, 126));
        return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
      }

      if(amount>2100000000)
      {
        Msg(playerid,COLOR_ERROR,"Nie mozesz wykonywaæ operacji bankowych na kowatch wiêkszych ni¿ {b}$2100000000{/b}");
        return OnDialogResponse(playerid, DIALOG_BANK_TRANSFER_STEP1, BUTTON_NEXT, 0, pTemp[playerid][bankTransferAName]);
      }

      pTemp[playerid][bankTransferAmount] = amount;

      format(buffer, sizeof buffer, TXT(playerid, 131), amount, pTemp[playerid][bankTransferAName]);
      ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_STEP3, DIALOG_STYLE_MSGBOX, TXT(playerid, 114), buffer, __("Tak"), __("Nie"));
    }

    case DIALOG_BANK_TRANSFER_STEP3:
    {
      if(response == BUTTON_NO) return _bankomat(playerid);

        AddPlayerAccountData(pTemp[playerid][bankTransferAID], "bank_money", pTemp[playerid][bankTransferAmount],false,true);
      MinusPlayerAccountData(playerid, "bank_money", pTemp[playerid][bankTransferAmount],false,false);

      format(buffer, sizeof buffer, TXT(playerid, 132), pTemp[playerid][bankTransferAmount], pTemp[playerid][bankTransferAName]);
      ShowPlayerDialog(playerid, DIALOG_BANK_TRANSFER_INFO, DIALOG_STYLE_MSGBOX, TXT(playerid, 114), buffer, __("Wyjdz"), __("Wroc"));

      format(buffer, sizeof buffer, "Gracz %s przelal Ci na konto bankowe kwote w wysokosci $%d", GetPlayerNick(playerid), pTemp[playerid][bankTransferAmount]);
      poczta_powiadomGracza(pTemp[playerid][bankTransferAID],buffer);

      new targetplayerid = FindPlayer(pTemp[playerid][bankTransferAName]);
      if(targetplayerid > 0 && IsPlayerConnected(targetplayerid)) Msg(targetplayerid, COLOR_INFO, buffer);
    }
    case DIALOG_BANK_TRANSFER_INFO:
    {
      if(response == 1) return 1;
      else _bankomat(playerid);
    }

    case DIALOG_KICK_CONFIRM:
    {
      if(!IsAdmin(playerid) && !IsGM(playerid)) return 1;
      if(response == BUTTON_QUIT) return 1;

      new
       targetplayerid = pTemp[playerid][tmpTargetPlayerID],
       reason[128];

      SetPlayerAccountDataString(targetplayerid, "kick_count", "kick_count + 1", true);
      SetServerStatString("kick_count", "value + 1", true);

      copy(pTemp[playerid][tmpReason], reason);

      return KickPlayerWithReason(targetplayerid, playerid, reason);
    }

    case DIALOG_STATS_MAIN:
    {
      if(response == BUTTON_QUIT) return 1;

      switch(listitem)
          {
        case f_s_PLAYERSTATS:
        {
          new
           timePlayed = StringToInt(GetPlayerAccountData(playerid, "session")) + ((GetTickCount() - pTemp[playerid][lastSessionSaveTick]) / 1000),
           period;

          GetOptimalTimeUnit(timePlayed, period);

          format(buffer, sizeof buffer, GetDialogContent(DIALOG_STATS_PLAYER),
           GetPlayerAccountData(playerid, "datetime_registered"),
           pData[playerid][respect],
           pData[playerid][skill],
           pData[playerid][gamep],
           pData[playerid][kills],
           pData[playerid][teamkills],
           pData[playerid][deaths],
           pData[playerid][suicides],
           GetPlayerAccountData(playerid, "kick_count"),
           GetPlayerAccountData(playerid, "ban_count"),
           pData[playerid][averagePing],
           timePlayed,
           GetPeriodName2(playerid, period, timePlayed)
          );

          ShowPlayerDialog(playerid, DIALOG_STATS_PLAYER, DIALOG_STYLE_MSGBOX, TXT(playerid, 155), buffer, __("Wyjdz"), __("Wroc"));
        }

        case f_s_SERVERSTATS:
        {
          new
           registeredPlayers;

          format(buffer, sizeof buffer, "SELECT count(id) FROM %s", gmData[DB_players]);
          mysql_query(buffer);
          mysql_store_result();
          if (mysql_fetch_row(buffer))
            registeredPlayers = StringToInt(buffer);
          mysql_free_result();

          format(buffer, sizeof buffer, GetDialogContent(DIALOG_STATS_SERVER),
           GetServerStat("most_online"),
           GetServerStat("most_online_date"),
           registeredPlayers,
           gmData[join_count],
           gmData[kill_count],
           gmData[death_count],
           GetServerStat("kick_count"),
           GetServerStat("ban_count")
          );

          ShowPlayerDialog(playerid, DIALOG_STATS_SERVER, DIALOG_STYLE_MSGBOX, TXT(playerid, 156), buffer, __("Wyjdz"), __("Wroc"));
        }

        case f_s_GANGSTATS:
        {
          gangs_ShowGangsList(playerid);
        }
      }
    }
    case DIALOG_STYLWALKI: {
      if (!response) return 1;
      switch(listitem)
      {
          case 0:
          {
          SetPlayerFightingStyle(playerid,4);
          Msg(playerid, COLOR_INFO, "Wybra³eœ(aœ) {b}normalny{/b} styl walki.");
        }
        case 1:
        {
          SetPlayerFightingStyle(playerid,6);
          Msg(playerid, COLOR_INFO, "Wybra³eœ(aœ) styl walki {b}karate{/b}.");
        }
        case 2:
        {
          SetPlayerFightingStyle(playerid,5);
          Msg(playerid, COLOR_INFO, "Wybra³eœ(aœ) styl walki {b}boxera{/b}.");
        }
        case 3:
        {
          SetPlayerFightingStyle(playerid,7);
          Msg(playerid, COLOR_INFO, "Wybra³eœ(aœ) styl walki {b}gangstera{/b}.");
        }
        case 4:
        {
          if (random(2)==1)
            SetPlayerFightingStyle(playerid,15);
          else
            SetPlayerFightingStyle(playerid,26);
          Msg(playerid, COLOR_INFO, "Wybra³eœ(aœ) styl walki {b}pijanej malpy{/b}.");
        }
      }
      return 1;
    }

    case DIALOG_STATS_GANG_LIST:
    {
      if(response == BUTTON_BACK) return _staty(playerid);
      return gangs_ShowGangDetails(playerid,pTemp[playerid][statsGangListID][listitem]);
    }

    case DIALOG_STATS_PLAYER, DIALOG_STATS_SERVER:
    {
      if(response == 1) return 1;
      else _staty(playerid);
    }

    case DIALOG_STATS_GANG_INFO:
    {
      if(response == BUTTON_BACK) OnDialogResponse(playerid, DIALOG_STATS_MAIN, BUTTON_NEXT, f_s_GANGSTATS, "");
    }

    case DIALOG_TOP_MAIN:
    {
      if(response == BUTTON_QUIT) return 1;

      switch(listitem)
          {
        case f_t_RESPECT:
        {
          new
           szTopList[512],
           szNick[24],
           szRespect[12],
           i = 1;

          format(buffer, sizeof buffer, "SELECT nick, respect FROM %s ORDER BY `respect` DESC LIMIT 10", gmData[DB_players]);
          mysql_query(buffer);
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("nick", szNick);
            mysql_get_field("respect", szRespect);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %6s {EFE58D}%s", i, szRespect, szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_RESPECT, DIALOG_STYLE_MSGBOX, TXT(playerid, 158), szTopList, __("Wyjdz"), __("Wroc"));
        }

        case f_t_SKILL:
        {
          new
           szTopList[512],
           szNick[24],
           szSkill[12],
           i = 1;

          format(buffer, sizeof buffer, "SELECT nick, skill FROM %s ORDER BY skill DESC LIMIT 10", gmData[DB_players]);
          mysql_query(buffer);
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("nick", szNick);
            mysql_get_field("skill", szSkill);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %6s {EFE58D}%s", i, szSkill, szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_RESPECT, DIALOG_STYLE_MSGBOX, "Top lista > Skill", szTopList, __("Wyjdz"), __("Wroc"));
        }

        case f_t_FRAGS:
        {
          new
           szTopList[512],
           szNick[24],
           szKills[12],
           i = 1;

          format(buffer, sizeof buffer, "SELECT nick, kill_count FROM %s ORDER BY `kill_count` DESC LIMIT 10", gmData[DB_players]);
          mysql_query(buffer);
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("nick", szNick);
            mysql_get_field("kill_count", szKills);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %6s {EFE58D}%s", i, szKills, szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_FRAGS, DIALOG_STYLE_MSGBOX, TXT(playerid, 159), szTopList, __("Wyjdz"), __("Wroc"));
        }

        case f_t_MONEY:
        {
          new
           szTopList[512],
           szNick[24],
           szMoney[32],
           i = 1;

          format(buffer, sizeof buffer, "SELECT nick, `bank_money` AS `kasa` FROM %s ORDER BY `bank_money`+`wallet_money` DESC LIMIT 10", gmData[DB_players]);
          mysql_query(buffer);
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("nick", szNick);
            mysql_get_field("kasa", szMoney);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %s {EFE58D}%s", i, szMoney, szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_MONEY, DIALOG_STYLE_MSGBOX, TXT(playerid, 199), szTopList, __("Wyjdz"), __("Wroc"));
        }

        case f_t_PLAYEDTIME:
        {
          new
           szTopList[512],
           szNick[24],
           _session,
           _period,
           i = 1;

          format(buffer, sizeof buffer, "SELECT nick, session FROM %s ORDER BY `session` DESC LIMIT 10", gmData[DB_players]);
          mysql_query(buffer);
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("nick", szNick);
            mysql_get_field("session", buffer);
            _session = StringToInt(buffer);

            GetOptimalTimeUnit(_session, _period);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %i %s {EFE58D}%s", i, _session, GetPeriodName2(playerid, _period, _session),  szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_TIMEPLAYED, DIALOG_STYLE_MSGBOX, TXT(playerid, 160), szTopList, __("Wyjdz"), __("Wroc"));
        }

        case f_t_GANGS:
        {
          new
           szTopList[512],
           szGangName[8],
           szGangRespect[12],
           i = 1;

          format(buffer, sizeof buffer, "SELECT tag, respect FROM %s ORDER BY respect DESC LIMIT 10", gmData[DB_gangs]);
          mysql_query(buffer);
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("tag", szGangName);
            mysql_get_field("respect", szGangRespect);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %5s {EFE58D}%s", i, szGangRespect, szGangName);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_GANGS, DIALOG_STYLE_MSGBOX, __("Top lista > Gangi"), szTopList, __("Wyjdz"), __("Wroc"));
        }
        case f_t_LISTY:
        {
          new
           szTopList[512],
           szNick[24],
           szListy[12],
           i = 1;

          mysql_query("SELECT pa.score,p.nick FROM fs_players_achievements pa JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='LISTONOSZ' ORDER BY pa.score DESC limit 10");
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("score", szListy);
            mysql_get_field("nick", szNick);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %d {EFE58D}%s", i, StringToInt(szListy), szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_MONEY, DIALOG_STYLE_MSGBOX, __("Top lista > Listonosze"), szTopList, __("Wyjdz"), __("Wroc"));
        }
        case f_t_WALIZKI:
        {
          new
           szTopList[512],
           szNick[24],
           szListy[12],
           i = 1;

          mysql_query("SELECT pa.score,p.nick FROM fs_players_achievements pa JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='WALIZKI' ORDER BY pa.score DESC limit 10");
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("score", szListy);
            mysql_get_field("nick", szNick);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %d {EFE58D}%s", i, StringToInt(szListy), szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_MONEY, DIALOG_STYLE_MSGBOX, __("Top lista > Poszukiwacze bonusów"), szTopList, __("Wyjdz"), __("Wroc"));
        }
        case f_t_QUIZ:
        {
          new
           szTopList[512],
           szNick[24],
           szListy[12],
           i = 1;

          mysql_query("SELECT pa.score,p.nick FROM fs_players_achievements pa JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='QUIZ' ORDER BY pa.score DESC limit 10");
          mysql_store_result();

          while(mysql_retrieve_row())
          {
            mysql_get_field("score", szListy);
            mysql_get_field("nick", szNick);

            format(buffer, sizeof buffer, "{A9C4E4}%2i. %d {EFE58D}%s", i, StringToInt(szListy), szNick);
            strcat(szTopList, buffer);

            if(i < 10) strcat(szTopList, "\n");
            i++;
          }

          mysql_free_result();

          ShowPlayerDialog(playerid, DIALOG_TOP_MONEY, DIALOG_STYLE_MSGBOX, __("Top lista > Rozwiazane QUIZy"), szTopList, __("Wyjdz"), __("Wroc"));
        }
      }
    }

    case DIALOG_TOP_RESPECT..DIALOG_TOP_TIMEPLAYED, DIALOG_TOP_MONEY, DIALOG_TOP_GANGS:
    {
      if(response == 1) return 1;
      else _top(playerid);
    }

    case DIALOG_HUD:
    {
      if(response == BUTTON_QUIT) return 1;
      if (listitem==0) {
        for(new i=0; i<MAX_HUD_ELEMENTS; i++) {
          pData[playerid][hudSetting][i] = !pData[playerid][hudSetting][i];
          ShowPlayerHudElement(playerid, i, pData[playerid][hudSetting][i]);

        }
        Msg(playerid,COLOR_INFO,__("Wszystkie ustawienia HUD przelaczone."));
      } else {
        listitem--;

        pData[playerid][hudSetting][listitem] = !pData[playerid][hudSetting][listitem];
        ShowPlayerHudElement(playerid, listitem, pData[playerid][hudSetting][listitem]);

        if(pData[playerid][hudSetting][listitem])
          format(buffer, sizeof buffer, __("Element {b}%s{/b} zostal wlaczony"), fNames_hud[listitem]);
        else
          format(buffer, sizeof buffer, __("Element {b}%s{/b} zostal wylaczony"), fNames_hud[listitem]);
        Msg(playerid, COLOR_INFO, buffer);
      }

      if(pData[playerid][accountID]>0){
        format(gstr,sizeof gstr,"UPDATE fs_players SET hudData='%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d' WHERE id=%d LIMIT 1",
          pData[playerid][hudSetting][HUD_STATUSBAR],pData[playerid][hudSetting][HUD_ATTRACTIONBOX],pData[playerid][hudSetting][HUD_ANNOUNCEMENTS],
          pData[playerid][hudSetting][HUD_VOTING],pData[playerid][hudSetting][HUD_SERVERLOGO],pData[playerid][hudSetting][HUD_DATE],pData[playerid][hudSetting][HUD_CLOCK],
          pData[playerid][hudSetting][HUD_STARS],pData[playerid][hudSetting][HUD_VEHICLEBOX],pData[playerid][hudSetting][HUD_HP],pData[playerid][hudSetting][HUD_CJHIT],pData[playerid][accountID]);
        mysql_query(gstr);
      }

      return _hud(playerid);
    }

    case DIALOG_REPORTS:
    {
      if(response == BUTTON_QUIT) return 1;

      new
       reportIndex = pTemp[playerid][reportSync][listitem];

      pTemp[playerid][reportTmpPlayerId] = gReports[reportIndex][rPlayerId];
      format(pTemp[playerid][reportTmpReason],128,"%s",gReports[reportIndex][rReason]);

      format(buffer, sizeof buffer, __("Gracz: %s\nID: %i\nPowod: %s\nZlozone przez: %s"), gReports[reportIndex][rPlayerName], gReports[reportIndex][rPlayerId], gReports[reportIndex][rReason], gReports[reportIndex][rReportingPlayerName]);
      ShowPlayerDialog(playerid, DIALOG_REPORTS_MANAGE, DIALOG_STYLE_MSGBOX, __("Raporty"), buffer, __("Spec"), __("Usun"));
    }

    case DIALOG_REPORTS_MANAGE:
    {
      if(!IsPlayerConnected(pTemp[playerid][reportTmpPlayerId]))
        return Msg(playerid, COLOR_ERROR, __("Gracz nie jest juz polaczony."));

      if(response == 1) // Spec
      {
        if(!SpecAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Nie mozesz w tym momencie obserwowac graczy")); // Nie mo¿esz w tym momencie obserwowaæ graczy.

        StartSpectating(playerid, pTemp[playerid][reportTmpPlayerId]);

        format(buffer, sizeof buffer, __("Obserwujesz gracza {b}%s (%i){/b} z powodu {b}%s{/b}"), GetPlayerNick(pTemp[playerid][reportTmpPlayerId]), pTemp[playerid][reportTmpPlayerId], pTemp[playerid][reportTmpReason]);
        Msg(playerid, COLOR_INFO, buffer);
      }
      else // Usuñ
      {
        format(buffer, sizeof buffer, __("Gracz {b}%s{/b} zostal usuniety z listy raportow."), GetPlayerNick(pTemp[playerid][reportTmpPlayerId]));
        Msg(playerid, COLOR_INFO, buffer);
      }

      RemovePlayerFromReportList(pTemp[playerid][reportTmpPlayerId]);
    }
  }

  return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
  if (pData[playerid][logonDialog])
    return 0;

  return ShowPlayerInfo(playerid,clickedplayerid);
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
  pTemp[playerid][lastPos]=-1;

  if(pData[playerid][jail]>0 && pData[playerid][adminLevel]<LEVEL_GM && pData[playerid][allowedLevel]<LEVEL_GM && strfind(cmdtext,"raport",true)!=0) {
    Msg(playerid, COLOR_ERROR, __("Niestety - siedzisz w paczce."));
    return 0;
  }
  if(pTemp[playerid][cenzura] && pData[playerid][adminLevel]<4){
    Msg(playerid, COLOR_ERROR, __("{b}Jestes ocenzurowany/-a{/b}! Nie mozesz wpisywac komend."));
    return 0;
  }
  if(pData[playerid][logonDialog] || (pData[playerid][classSelecting] && strfind(cmdtext, "skin", true) == -1)) {
    Msg(playerid,COLOR_ERROR,__("Najpierw musisz dolaczyc do gry."));
    return 0;
  }

  return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
  if(pData[playerid][adminLevel]<LEVEL_ADMIN2 && (GetTickCount() - pTemp[playerid][lastMsgTick] < 1000))
  {
    pTemp[playerid][spamCount]++;

    if(pTemp[playerid][spamCount] >= 10)
    {
      Msg(playerid, COLOR_ERROR, __("Spamowanie nie bedzie tolerowane."));
      KickPlayer(playerid);

      return 1;
    }

    if(pTemp[playerid][spamCount] >= 2)
    {
      Msg(playerid, COLOR_ERROR, __("Nie spamuj! Kolejne ostrzezenia moga skonczyc sie wyrzuceniem z serwera."));
      pTemp[playerid][lastMsgTick] = GetTickCount();

      return 1;
    }
  }
  else
  {
    pTemp[playerid][spamCount] = 0;
  }

  pTemp[playerid][lastMsgTick] = GetTickCount();

  if(success)
  {
    format(gstr, sizeof gstr, "%i %s", playerid, cmdtext);
    OutputLog(LOG_COMMANDS, gstr);

    return 1;
  }
  else
  {
    Msg(playerid, COLOR_ERROR, __("Nieznana komenda, uzyj /komendy aby zobaczyc pelna liste komend."));
    return 1;
  }
}

OnPlayerRCONLogin(playerid)
{
  new
   buffer[128];

  if (!pData[playerid][loggedIn] || pData[playerid][allowedLevel]<LEVEL_ADMIN3) {
    format(buffer,sizeof buffer,__("Nieautoryzowane logowanie na admina RCON przez %s (%d)! Poziom %d/%d Wykopany."), GetPlayerNick(playerid), playerid, pData[playerid][adminLevel], pData[playerid][allowedLevel]);
    KickPlayer(playerid);
    foreach(i)
      if(IsAdmin(i, LEVEL_ADMIN1))
        Msg(i, COLOR_ERROR, buffer);
    OutputLog(LOG_SYSTEM, buffer);
  } else {
    pData[playerid][adminLevel]=pData[playerid][allowedLevel];
    if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN) {
      format(buffer, sizeof buffer, __("{b}%s{/b} zalogowal(a) sie na admina (poziom %i)."), GetPlayerNick(playerid), pData[playerid][adminLevel] - 1);
      foreach(i) {
        Msg(i, COLOR_INFO2, buffer, false);
        PlaySound(i, 1133);
      }
    }
  }

  SetPlayerProperColor(playerid,true);
  UpdatePlayerNickTD(playerid);
}

OnPlayerFirstSpawn(playerid)
{
    RemoveBuildings(playerid);
  DisablePlayerSounds(playerid);
  SetPlayerVirtualWorld(playerid,0);
  PlaySound(playerid, 1058);

  ShowElement(playerid, TDE_WIDE, false);
  ShowElement(playerid, TDE_WELCOMEBOX, false);

  if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowPlayerHudElement(playerid, HUD_STATUSBAR, true);
  if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowPlayerHudElement(playerid, HUD_ATTRACTIONBOX, true);
  if(pData[playerid][hudSetting][HUD_ANNOUNCEMENTS]) ShowPlayerHudElement(playerid, HUD_ANNOUNCEMENTS, true);
  if(pData[playerid][hudSetting][HUD_VOTING]) ShowPlayerHudElement(playerid, HUD_VOTING, true);
  if(pData[playerid][hudSetting][HUD_SERVERLOGO]) ShowPlayerHudElement(playerid, HUD_SERVERLOGO, true);
  if(pData[playerid][hudSetting][HUD_DATE]) ShowPlayerHudElement(playerid, HUD_DATE, true);
  if(pData[playerid][hudSetting][HUD_CLOCK]) ShowPlayerHudElement(playerid, HUD_CLOCK, true);
  if(pData[playerid][hudSetting][HUD_STARS]) ShowPlayerHudElement(playerid, HUD_STARS, true);
  if(pData[playerid][hudSetting][HUD_HP]) ShowPlayerHudElement(playerid, HUD_HP, true);

  if(pData[playerid][jailMysql] > 0)
    SetTimerEx("checkJail",2000,false,"d",playerid);

  // KASA
  new kasa=500 + floatround(pData[playerid][respect]/10);
  if (GetPlayerMoney(playerid)<kasa) {
    SetPlayerMoney(playerid,kasa);
  }

  Audio_OdtworzPowitanie(playerid);

  return 1;

}

public OnPlayerEnterDynamicArea(playerid,areaid){
    if (prezenty_OPEnterDynamicArea(playerid,areaid)) return 1;
  if (obiekty_OPEnterDynamicArea(playerid,areaid)) return 1;
  if (gang_OPEnterDynamicArea(playerid,areaid)) return 1;
    return 0;
}

public OnPlayerLeaveDynamicArea(playerid,areaid){
    if (prezenty_OPLeaveDynamicArea(playerid,areaid)) return 1;
  if (obiekty_OPLeaveDynamicArea(playerid,areaid)) return 1;
    return 0;
}

public OnPlayerEnterDynamicCP(playerid,checkpointid){
  if (obiekty_OPEnterDynamicCP(playerid,checkpointid)) return 1;
  if (domy_OPEnterDynamicCP(playerid,checkpointid)) return 1;
  if (poczta_OPEDCP(playerid,checkpointid)) return 1;
  return 0;
}
public OnPlayerLeaveDynamicCP(playerid,checkpointid){
  if (obiekty_OPLeaveDynamicCP(playerid,checkpointid)) return 1;
  return 0;
}

public OnPlayerEnterDynamicRaceCP(playerid, checkpointid){
  if (aData[A_DRIFT][aState] == A_STATE_ON && drift_OPEDRCP(playerid, checkpointid)) return 1;
  if (aData[A_RACE][aState] == A_STATE_ON && race_OPEDRCP(playerid, checkpointid)) return 1;
  return 0;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
  if (IsGM(playerid) && GetPlayerInterior(playerid)==0 && GetPlayerVirtualWorld(playerid)==0){
      new Float:MAz;
    MapAndreas_FindZ_For2DCoord(fX,fY,MAz);
    SetPlayerPos(playerid,fX,fY,MAz+1.5);
  }
  return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart)
{
  return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
  static Float:hlpl;
  
  if(issuerid!=INVALID_PLAYER_ID) 
  {
    GetPlayerHealth(playerid,hlpl);
    
    if(pTemp[issuerid][godMode] || pTemp[playerid][godMode]) return 0;
    if((pTemp[issuerid][wStrefieNODM] || pTemp[playerid][wStrefieNODM]) && pData[playerid][adminLevel]<LEVEL_GM) return 0;
    if(pTemp[playerid][staleTime]>3) return 0;
    if((GetPlayerTeam(playerid) == GetPlayerTeam(issuerid)) && (GetPlayerTeam(playerid) != 255 && GetPlayerTeam(issuerid) != 255)) return 0;
    if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 432 && pData[playerid][pAttraction]==A_NONE) return 0;
    
    if(issuerid!=playerid && hlpl>0 && IsPlayerSpawned(playerid) && IsPlayerSpawned(issuerid) && !pTemp[playerid][godMode]){
      if(pData[issuerid][pAttraction]!=A_NONE || pTemp[issuerid][gongAll]) PlayerPlaySound(issuerid,17802,0, 0, 0);
    }

#if defined DYNAMIC_HP
    if(pTemp[playerid][lcTicks]>0){
      format(gstr,sizeof(gstr),"-%.1f HP",amount+pTemp[playerid][lastDmg]);
      pTemp[playerid][lastDmg]=amount+pTemp[playerid][lastDmg];
    }else{
        format(gstr,sizeof(gstr),"-%.1f HP",amount);
        pTemp[playerid][lastDmg]=amount;
    }
    UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xFF0000FF,gstr);
    pTemp[playerid][lcTicks]=3;
#endif

#if defined CJHIT_MODEL
    if(pData[playerid][hudSetting][HUD_CJHIT]){
        PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], boneName[bodypart]);
        PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT_TXT][playerid]);

        if(pTemp[playerid][hitTicks]==0){
          PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT2][playerid]);
      }
      PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_HP][playerid], gstr);
      PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT_HP][playerid]);
      pTemp[playerid][hitTicks]=2;
    }
#endif

#if defined SNIPER_HEADSHOOT
    if(bodypart==BODY_PART_HEAD && (weaponid==34 || weaponid==33)){
      OnPlayerDeath(playerid, issuerid, weaponid);
      GameTextForPlayer(issuerid,"~w~>> ~r~HEADSHOTTA ~w~<<",2000,5);
      GameTextForPlayer(playerid,"~w~>> ~r~HEADSHOTTED ~w~<<",2000,5);
      FlashScreenRed(playerid);
      Audio_Play(issuerid,AUDIOID_HEAD);
      SpawnPlayer(playerid);
      
      pTemp[playerid][lastDeathHeadshottedBy] = issuerid;

      new Float:Ppb[3];
      GetPlayerPos(playerid, Ppb[0], Ppb[1], Ppb[2]);
      pTemp[playerid][bloodId][0] = CreateObject(18668, Ppb[0], Ppb[1], Ppb[2], 0, 0, 0, 25.0);
      pTemp[playerid][bloodId][1] = CreateObject(18668, Ppb[0]+2, Ppb[1], Ppb[2], 0, 0, 0, 25.0);
      SetTimerEx("removePlayerBloodObj",900,false,"i",playerid);
      return 1;
    }
#endif

    if (pTemp[playerid][watchDmg]) {
      format(gstr, sizeof gstr,"[DMG] amount: %.2f, weapon: %2d (%s), %d, body: %d", amount, weaponid, weaponName[weaponid], issuerid, bodypart);
      MSGToAdmins(COLOR_ERROR, gstr, false, LEVEL_ADMIN3);
    }
  }
  return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
  if(hittype==BULLET_HIT_TYPE_PLAYER){
    if(pTemp[hitid][staleTime]>3) return 0;
  }
  if(hittype==BULLET_HIT_TYPE_VEHICLE){
    if(GetVehicleModel(hitid) == 432 && weaponid!=35 && pData[playerid][pAttraction]==A_NONE) return 0;
  }
  return 1;
}

/*public OnQueryFinish(query[], resultid, extraid, connectionHandle){
  printf(" >>> THREADED QUERY: %s",query);
  switch(resultid){
    case SQL_RI_BACKGROUND: {
      mysql_free_result();
      return 1;
    }
    case SQL_RI_DOMY_LISTA:
      return domy_OnMysqlQuery(resultid,extraid);
    case SQL_RI_VEHNAMES:
      return UpdateVehicleNames(resultid,extraid);
  }
  return 1;
}*/

public OnQueryError(errorid, error[], resultid, extraid, callback[], query[], connectionHandle)
{
	if(errorid == ER_DBACCESS_DENIED_ERROR || errorid == ER_ACCESS_DENIED_ERROR || errorid == ER_UNKNOWN_TABLE || errorid == ER_SYNTAX_ERROR || errorid == CR_SERVER_GONE_ERROR || errorid == CR_SERVER_LOST || errorid == CR_COMMAND_OUT_OF_SYNC || errorid == CR_SERVER_LOST_EXTENDED)
  {
    printf("  [MYSQL ERROR] Unrecoveable error occured: %s {%d}!", error, errorid);
    printf("  -> Executed query: %s",query);
    printf("  -> The server is going down NOW without saving data to prevent other damage!");
    fread(File:EXIT, gstr); // crash server asap!
	}
	return 1;
}
// EOF