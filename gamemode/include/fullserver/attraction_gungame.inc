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

#define GUN_FINISH_LEVEL 11

forward GG_Countdown();

new GGbronieNames[GUN_FINISH_LEVEL][48] = {
  "",
  "Silencer",
  "Shotgun",
  "DE",
  "MP5",
  "Sniper",
  "Sawnoff",
  "M4",
  "Rifle",
  "Combat",
  "ZESTAW"
};

new Float:DATA_gunGameSpawn[][e_DATA_spawns] = {
  {-1476.8704,1643.3887,1052.5313,142.6595},
  {-1459.6731,1620.2274,1052.5313,31.1352},
  {-1451.2478,1617.5807,1052.5313,29.2551},
  {-1443.9457,1618.3226,1052.5313,46.4885},
  {-1428.4747,1596.9474,1052.5313,42.0784},
  {-1431.0546,1581.1462,1055.7191,38.0051},
  {-1408.1814,1585.4918,1055.8151,357.2948},
  {-1380.4928,1557.3630,1052.5313,28.2918},
  {-1351.7594,1574.6837,1052.5313,17.6384},
  {-1357.5240,1615.3812,1052.5313,95.9724},
  {-1369.2920,1629.5251,1052.5313,64.9521},
  {-1357.3447,1640.0983,1052.5313,165.1964},
  {-1393.8710,1642.6294,1052.5313,100.9624},
  {-1418.5022,1626.2388,1052.5313,109.1092},
  {-1427.5372,1618.5833,1052.5313,79.9689},
  {-1449.1749,1582.3981,1052.7500,106.2892},
  {-1486.4229,1569.4764,1052.5313,41.4286},
  {-1487.8739,1592.4121,1055.3237,332.4946},
  {-1486.8428,1642.8660,1060.6719,355.9949},
  {-1429.8484,1614.3538,1054.9276,2.5750}
};

GG_Start(){
  new
   aPlayers[MAX_QUEUE_GG],
   aMaxPlayers = 0;

  foreach(playerid)
  {
    if(pData[playerid][aGG])
    {
      if(AttractionAllowed(playerid) && !pTemp[playerid][godMode])
      {
        aPlayers[aMaxPlayers++] = playerid;
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
      }
      else
      {
        pData[playerid][aGG] = false;
        Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
      }
    }
  }

  if(aMaxPlayers < MIN_QUEUE_GG)
  {
    foreach(playerid)
    {
      Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}GunGame{/b}, zapisy ponowione.");
      pData[playerid][aGG] = false;
    }

    aData[A_GG][aState] = A_STATE_OFF;
    return 0;
  }

  new p=0;
  while (p<aMaxPlayers) {
    pTemp[aPlayers[p]][aGGDead]=false;
    pTemp[aPlayers[p]][aGGexit] = false;
    
    SetPlayerHealth(aPlayers[p],99.0);
    SetPlayerArmour(aPlayers[p],0.0);
    
    
    PlayerTextDrawSetString(aPlayers[p],pTextDraw[PTD_AGGINFO][aPlayers[p]],"~b~Killi: ~r~0~n~~b~Level: ~r~1~n~~b~Bron: ~r~Silencer");

    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL_SILENCED, 999);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_MICRO_UZI, 0);

    TogglePlayerControllable(aPlayers[p],false);
    ResetPlayerWeapons(aPlayers[p]);
    new randomPos = random(sizeof(DATA_gunGameSpawn));
    SetPlayerPos(aPlayers[p],DATA_gunGameSpawn[randomPos][X], DATA_gunGameSpawn[randomPos][Y], DATA_gunGameSpawn[randomPos][Z]);
    Streamer_Update(aPlayers[p]);
    SetPlayerFacingAngle(aPlayers[p],DATA_gunGameSpawn[randomPos][angle]);
    SetPlayerVirtualWorld(aPlayers[p], VW_GG);
    SetPlayerInterior(aPlayers[p], 14);

    pData[aPlayers[p]][pAttraction] = A_GG;
    pData[aPlayers[p]][aGG] = false;

    PlayerTextDrawShow(aPlayers[p],pTextDraw[PTD_AGGINFO][aPlayers[p]]);

    TogglePlayerControllable(aPlayers[p], false);
    
    pTemp[aPlayers[p]][GGprevKillCount] = 0;
    pTemp[aPlayers[p]][GGlevelCount] = 1;
    pTemp[aPlayers[p]][GGactKillCount] = 0;
    
    p++;
  }

  gmTemp[aGGPlayers] = aPlayers;
  gmTemp[aGGMaxPlayers] = aMaxPlayers;
  gmTemp[aGGCount] = 3;
  gmTemp[timerGGCountdown] = SetTimer("GG_Countdown", 1000, true);
  gmTemp[aGGStartTick] = GetTickCount();
  GG_Update();
  return 1;
}

GG_Finish(){
  aData[A_GG][aState] = A_STATE_OFF;

  for(new i = 0; i < gmTemp[aGGMaxPlayers]; i++) {
    if (gmTemp[aGGPlayers][i]==INVALID_PLAYER_ID) continue;
    if (!IsPlayerConnected(gmTemp[aGGPlayers][i])) {
      gmTemp[aGGPlayers][i]=INVALID_PLAYER_ID;
      continue;
    }

    pData[gmTemp[aGGPlayers][i]][pAttraction] = A_NONE;
    PlayerTextDrawHide(gmTemp[aGGPlayers][i],pTextDraw[PTD_AGGINFO][gmTemp[aGGPlayers][i]]);
    SetPlayerTeam(gmTemp[aGGPlayers][i], gmTemp[aGGPlayers][i]+1);
    if(GetPlayerState(gmTemp[aGGPlayers][i])!=PLAYER_STATE_WASTED && !pTemp[gmTemp[aGGPlayers][i]][aGGexit] && GetPlayerVirtualWorld(gmTemp[aGGPlayers][i])==VW_GG) {
      SetPlayerHealth(gmTemp[aGGPlayers][i],100.0);
      SpawnPlayer(gmTemp[aGGPlayers][i]);
    }
    gmTemp[aGGPlayers][i]=INVALID_PLAYER_ID;
  }
  
  aData[A_GG][aState] = A_STATE_OFF;
  gmTemp[aGGMaxPlayers]=0;
  return 1;
}

public GG_Countdown()
{
  new
   buffer[64];

  if(gmTemp[aGGCount] <= 0)
  {
    format(buffer, sizeof buffer, "~g~GO!");
    KillTimer(gmTemp[timerGGCountdown]);
  }
  else
  {
    format(buffer, sizeof buffer, "~r~%i", gmTemp[aGGCount]);
  }

  for(new playerid = 0; playerid < gmTemp[aGGMaxPlayers]; playerid++) {
    if(!IsPlayerConnected(gmTemp[aGGPlayers][playerid])) continue;
    GameTextForPlayer(gmTemp[aGGPlayers][playerid], buffer, 1000, 6);
    if(gmTemp[aGGCount] == 0)
    {
      TogglePlayerControllable(gmTemp[aGGPlayers][playerid], 1);
      PlaySound(gmTemp[aGGPlayers][playerid], 1057);
      GG_PlayerWeapons(playerid);
    } else
      PlaySound(gmTemp[aGGPlayers][playerid], 1056);
  }
  gmTemp[aGGCount]--;
}

GG_Update(){
  new _cnt,buf[128];
  
  for(new playerid = 0; playerid < gmTemp[aGGMaxPlayers]; playerid++)
  {
    if (!IsPlayerConnected(gmTemp[aGGPlayers][playerid])) 
    {
      gmTemp[aGGPlayers][playerid]=INVALID_PLAYER_ID;
      continue;
    } 
    else if (GetPlayerVirtualWorld(gmTemp[aGGPlayers][playerid])==VW_GG)
    {
      if (pTemp[gmTemp[aGGPlayers][playerid]][staleTime]>5) 
      {
        Msg(gmTemp[aGGPlayers][playerid],COLOR_ERROR,"Zostajesz wyrzucony z {b}/GUNGAME{/b} za bycie {b}AFK{/b}");
        SetPlayerHealth(gmTemp[aGGPlayers][playerid],0);
        pData[gmTemp[aGGPlayers][playerid]][pAttraction]=A_NONE;
        pTemp[gmTemp[aGGPlayers][playerid]][aGGexit]=true;
        gmTemp[aGGPlayers][playerid]=INVALID_PLAYER_ID;
        continue;
      }
      _cnt++;
    }
  }
  
  for(new playerid = 0; playerid < gmTemp[aGGMaxPlayers]; playerid++)
  {
    if (gmTemp[aGGPlayers][playerid]!=INVALID_PLAYER_ID && pData[gmTemp[aGGPlayers][playerid]][pAttraction]==A_GG)
    {
      if(pTemp[gmTemp[aGGPlayers][playerid]][GGlevelCount] >= GUN_FINISH_LEVEL)
      {
        format(buf,128,"Gracz {ffff00}%s{ffffff} wygrywa GunGame z iloscia killow {ffff00}%d{ffffff} i otrzymuje {ffff00}10{ffffff} respa",GetPlayerNick(gmTemp[aGGPlayers][playerid]),pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount]);
        GivePlayerScore(gmTemp[aGGPlayers][playerid],10,"Wygranie GG");
        foreach(playeri)
        {
          Msg(playeri, -1, buf);
        }
        GG_Finish();
        return;
      }
      else
      {
        if(pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount] >= (pTemp[gmTemp[aGGPlayers][playerid]][GGprevKillCount]+(_cnt/2))) // well... shit! we can't -1 here because minimun is 2 and 2/2 = 1
        {
          pTemp[gmTemp[aGGPlayers][playerid]][GGlevelCount]++;
          pTemp[gmTemp[aGGPlayers][playerid]][GGprevKillCount] = pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount];
          if(pTemp[gmTemp[aGGPlayers][playerid]][GGlevelCount] >= GUN_FINISH_LEVEL)
          {
            format(buf,128,"Gracz {ffff00}%s{ffffff} wygrywa GunGame z iloscia killow {ffff00}%d{ffffff} i otrzymuje {ffff00}10{ffffff} respa",GetPlayerNick(gmTemp[aGGPlayers][playerid]),pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount]);
            GivePlayerScore(gmTemp[aGGPlayers][playerid],10,"Wygranie GG");
            foreach(playeri)
            {
              Msg(playeri, -1, buf);
            }
            GG_Finish();
            return;
          }
          ResetPlayerWeapons(gmTemp[aGGPlayers][playerid]);
          GG_PlayerWeapons(playerid);
          format(buf,128,"~b~Killi: ~r~%d~n~~b~Level: ~r~%d~n~~b~Bron: ~r~%s",pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount],pTemp[gmTemp[aGGPlayers][playerid]][GGlevelCount],GGbronieNames[pTemp[gmTemp[aGGPlayers][playerid]][GGlevelCount]]);
          PlayerTextDrawSetString(gmTemp[aGGPlayers][playerid],pTextDraw[PTD_AGGINFO][gmTemp[aGGPlayers][playerid]],buf);
        }
        else if(pTemp[gmTemp[aGGPlayers][playerid]][GGprevKillCount] > pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount]) { // some kind of internal error, idk where root of this problem is.
          format(ebuf,sizeof ebuf,"[GUNGAME BUG #1] prevKillCount (%d) > actKillCount (%d) == FALSE!!!!!!!!!!!!! force reset prev = act",pTemp[gmTemp[aGGPlayers][playerid]][GGprevKillCount],pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount]);
          OutputErrorLog("GG1",ebuf);
          pTemp[gmTemp[aGGPlayers][playerid]][GGprevKillCount] = pTemp[gmTemp[aGGPlayers][playerid]][GGactKillCount];
        }
      }
    }
  }
  
  if(_cnt < 2)
  {
    foreach(playerid)
      Msg(playerid, COLOR_INFO3, "{b}/GUNGAME{/b} zostalo przerwane z powodu braku graczy"); // GG zosta³o przerwane z powodu braku graczy.
    GG_Finish();
  }
  return;
}

GG_PlayerWeapons(playeroff)
{
  switch(pTemp[gmTemp[aGGPlayers][playeroff]][GGlevelCount]){
    case 1: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],23,1000);
    case 2: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],25,1000);
    case 3: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],24,1000);
    case 4: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],29,1000);
    case 5: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],34,1000);
    case 6: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],26,1000);
    case 7: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],31,1000);
    case 8: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],33,1000);
    case 9: GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],27,1000);
    case 10: {
      GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],25,1000);
      GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],24,1000);
      GivePlayerWeapon(gmTemp[aGGPlayers][playeroff],34,1000);
    }
    default: {
      format(ebuf,sizeof ebuf,"level (%d) less or greaster then 0-10!!!!!!!!! force reset it to 1",pTemp[gmTemp[aGGPlayers][playeroff]][GGlevelCount]);
      OutputErrorLog("GG2",ebuf);
      pTemp[gmTemp[aGGPlayers][playeroff]][GGlevelCount] = 1;
      GG_PlayerWeapons(playeroff);
    }
  }
}

GG_PlayerWeaponsNONOFF(playerid)
{
  switch(pTemp[playerid][GGlevelCount]){
    case 1: GivePlayerWeapon(playerid,23,1000);
    case 2: GivePlayerWeapon(playerid,25,1000);
    case 3: GivePlayerWeapon(playerid,24,1000);
    case 4: GivePlayerWeapon(playerid,29,1000);
    case 5: GivePlayerWeapon(playerid,34,1000);
    case 6: GivePlayerWeapon(playerid,26,1000);
    case 7: GivePlayerWeapon(playerid,31,1000);
    case 8: GivePlayerWeapon(playerid,33,1000);
    case 9: GivePlayerWeapon(playerid,27,1000);
    case 10: {
      GivePlayerWeapon(playerid,25,1000);
      GivePlayerWeapon(playerid,24,1000);
      GivePlayerWeapon(playerid,34,1000);
    }
    default: {
      format(ebuf,sizeof ebuf,"level (%d) less or greaster then 0-10!!!!!!!!! force reset it to 1",pTemp[playerid][GGlevelCount]);
      OutputErrorLog("GG2",ebuf);
      pTemp[playerid][GGlevelCount] = 1;
      GG_PlayerWeaponsNONOFF(playerid);
    }
  }
}

GG_onPlayerDeath(playerid,killerid)
{
  if(killerid == INVALID_PLAYER_ID){
    OutputErrorLog("GG3","killerid == INVALID_PLAYER_ID!!!!!!!! ingore this iteration");
    return 1;
  }
  if(!IsPlayerConnected(killerid)){
    OutputErrorLog("GG4","IsPlayerConnected(killerid)) == FALSE!!!!!!!! ingore this iteration");
    return 1;
  }
  pTemp[playerid][aGGDead]=true;
  pTemp[killerid][GGactKillCount]++;
  
  new buf[128];
  format(buf,128,"~b~Killi: ~r~%d~n~~b~Level: ~r~%d~n~~b~Bron: ~r~%s",pTemp[killerid][GGactKillCount],pTemp[killerid][GGlevelCount],GGbronieNames[pTemp[killerid][GGlevelCount]]);
  PlayerTextDrawSetString(killerid,pTextDraw[PTD_AGGINFO][killerid],buf);
  
  return 1;
}

GG_ReSpawn(playerid)
{
  if(playerid == INVALID_PLAYER_ID){
    OutputErrorLog("GG5","playerid == INVALID_PLAYER_ID!!!!!!!! ingore this iteration");
    return 1;
  }
  if(!IsPlayerConnected(playerid)){
    OutputErrorLog("GG6","IsPlayerConnected(playerid)) == FALSE!!!!!!!! ingore this iteration");
    return 1;
  }
  
  SetPlayerInterior(playerid, 14);
  SetPlayerArmour(playerid,0.0);
  SetPlayerHealth(playerid,99.0);
  new randomPos = random(sizeof(DATA_gunGameSpawn));
  SetPlayerPos(playerid,DATA_gunGameSpawn[randomPos][X], DATA_gunGameSpawn[randomPos][Y], DATA_gunGameSpawn[randomPos][Z]);
  Streamer_Update(playerid);
  SetPlayerFacingAngle(playerid,DATA_gunGameSpawn[randomPos][angle]);
  SetPlayerVirtualWorld(playerid,VW_GG);
  SetCameraBehindPlayer(playerid);

  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, 0);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, 999);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, 0);
  ResetPlayerWeapons(playerid);
  
  GG_PlayerWeaponsNONOFF(playerid);

  return 1;
}
// EOF