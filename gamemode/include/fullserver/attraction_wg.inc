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

new Float:aWGWBCube[6],
    Float:aWGWBSphere[4],
  WGPozaStrefa[MAX_QUEUE_WG]={10,...},
  aWGWBmode;

forward WG_Countdown();

WG_Start(){
  new
   aPlayers[MAX_QUEUE_WG],
   aMaxPlayers = 0,
   buffer[255],
   _skill[2],
   _teamsize[2],
   wb_sphere[80],
   wb_cube[80];

  foreach(playerid)
  {
    if(pData[playerid][aWG])
    {
      if(AttractionAllowed(playerid) && !pTemp[playerid][godMode])
      {
        _teamsize[aMaxPlayers%2]++;
        aPlayers[aMaxPlayers++] = playerid;
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
      }
      else
      {
        pData[playerid][aWG] = false;
        Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
      }
    }
  }
  //  TODO kod do wyrownywania druzyn

  if(aMaxPlayers < MIN_QUEUE_WG)
  {
    foreach(playerid)
    {
      Msg(playerid, COLOR_INFO3, TXT(playerid, 275));
      pData[playerid][aWG] = false;
    }

    aData[A_WG][aState] = A_STATE_OFF;
    return 0;
  }

  gmTemp[aWGPBar]=CreateProgressBar(453.00, 356.00, 166.50, 4.50, 16777215, aMaxPlayers*100);

  // losowanie areny
  if (gmTemp[aWGForceArena]>0) {
      format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz') from fs_wg_arena where id=%d LIMIT 1;",gmTemp[aWGForceArena]);
    gmTemp[aWGForceArena]=0;
  } else
      format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz') from fs_wg_arena where minplayers<=%d AND %d<=maxplayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
    mysql_query(buffer);
    mysql_store_result();
    if (!mysql_num_rows()) {    // nie znaleziono odpowiedniej areny
        foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/WG{/b} nie wystartowalo - nie znaleziono odpowiedniej areny");

    WG_Finish();
    mysql_free_result();
        return 0;
    }

    mysql_fetch_row_format(buffer,"|");
  mysql_free_result();

    if (sscanf(buffer,"p<|>ds[63]ds[63]s[63]d",gmTemp[aWGArena],gmTemp[aWGDescr],gmTemp[aWGInterior], wb_sphere,wb_cube,aWGWBmode)) {
        foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/WG{/b} nie wystartowalo - nie znaleziono odpowiedniej areny");
    WG_Finish();
        return 0;
  }
  aWGWBSphere[0]=FLOAT_NAN;
  sscanf(wb_sphere,"p<,>ffff", aWGWBSphere[0],aWGWBSphere[1],aWGWBSphere[2],aWGWBSphere[3]);

  aWGWBCube[0]=FLOAT_NAN;
  sscanf(wb_cube,"p<,>ffffff", aWGWBCube[0],aWGWBCube[1],aWGWBCube[2],aWGWBCube[3],aWGWBCube[4],aWGWBCube[5]);

  TextDrawSetString(gTextDraw[TD_AWGINFO],"~g~-   ~w~vs   ~b~-");

  format(buffer,sizeof buffer,"(SELECT team,X,Y,Z,A FROM fs_wg_arena_sp WHERE aid=%d AND team=0 ORDER BY RAND() LIMIT %d) UNION (SELECT team,X,Y,Z,A FROM fs_wg_arena_sp WHERE aid=%d AND team=1 ORDER BY rand() LIMIT %d)", gmTemp[aWGArena], _teamsize[0], gmTemp[aWGArena], _teamsize[1]);
  mysql_query(buffer);
  mysql_store_result();

  new p=0;
  while (mysql_fetch_row_format(buffer,"|") && p<aMaxPlayers) {
    new team,Float:PX,Float:PY,Float:PZ,Float:PA;
    WGPozaStrefa[p]=10;
    sscanf(buffer,"p<|>dffff",team, PX, PY, PZ, PA);

    pTemp[aPlayers[p]][aWGTeam]=team;
    pTemp[aPlayers[p]][aWGDead]=false;
    _skill[team]+=pData[aPlayers[p]][skill];

    SetPlayerHealth(aPlayers[p],99.0);
    SetPlayerArmour(aPlayers[p],0.0);
    Streamer_UpdateEx(aPlayers[p],PX,PY,PZ);

    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL_SILENCED, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_MICRO_UZI, 0);

    pData[aPlayers[p]][pAttraction] = A_WG;
    pData[aPlayers[p]][aWG] = false;

    TogglePlayerControllable(aPlayers[p],false);
    SetPlayerVirtualWorld(aPlayers[p], VW_WG);
    SetPlayerInterior(aPlayers[p], gmTemp[aWGInterior]);
    ResetPlayerWeapons(aPlayers[p]);
    SetPlayerPos(aPlayers[p],PX,PY,PZ);
    Streamer_Update(aPlayers[p]);
    SetPlayerFacingAngle(aPlayers[p],PA);
    SetPlayerTeam(aPlayers[p], team);

    TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_AWGINFO]);
    ShowProgressBarForPlayer(aPlayers[p],gmTemp[aWGPBar]);
    TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_AWGINFOHP]);

    TogglePlayerControllable(aPlayers[p], false);
    ResetPlayerWeapons(aPlayers[p]);
    GivePlayerWeapon(aPlayers[p],24,1000);
    GivePlayerWeapon(aPlayers[p],25,200);
    GivePlayerWeapon(aPlayers[p],34,1000);

        pTemp[aPlayers[p]][skinBeforeChange] = GetPlayerSkin(aPlayers[p]);
    if (team==A_WG_TEAM_GREEN) {
      SetPlayerColor(aPlayers[p],0x00FF00FF);
      SetPlayerSkin(aPlayers[p],106);
    }else{
      SetPlayerColor(aPlayers[p],0x0000FFFF);
      SetPlayerSkin(aPlayers[p],104);
    }
    p++;
  }

  mysql_free_result();

  gmTemp[aWGPlayers] = aPlayers;
  gmTemp[aWGMaxPlayers] = aMaxPlayers;
  gmTemp[aWGCount] = 3;
  gmTemp[timerWGCountdown] = SetTimer("WG_Countdown", 1000, true);
  gmTemp[aWGStartTick] = GetTickCount();
  WG_Update();
  return 1;
}

WG_Finish(){
  if (_:gmTemp[aWGPBar]!=-1) {
    HideProgressBarForAll(gmTemp[aWGPBar]);
    DestroyProgressBar(gmTemp[aWGPBar]);
  }
  TextDrawHideForAll(gTextDraw[TD_AWGINFO]);
  TextDrawHideForAll(gTextDraw[TD_AWGINFOHP]);
  aData[A_WG][aState] = A_STATE_OFF;

  for(new i = 0; i < gmTemp[aWGMaxPlayers]; i++) {
    if (gmTemp[aWGPlayers][i]==INVALID_PLAYER_ID) continue;
    if (!IsPlayerConnected(gmTemp[aWGPlayers][i]) || pData[gmTemp[aWGPlayers][i]][pAttraction]!=A_WG) {
      gmTemp[aWGPlayers][i]=INVALID_PLAYER_ID;
      continue;
    }

    pData[gmTemp[aWGPlayers][i]][pAttraction] = A_NONE;
    SetPlayerTeam(gmTemp[aWGPlayers][i], gmTemp[aWGPlayers][i]+1);
    if (GetPlayerState(gmTemp[aWGPlayers][i])!=PLAYER_STATE_WASTED && !pTemp[gmTemp[aWGPlayers][i]][aWGDead]) {
      SetPlayerHealth(gmTemp[aWGPlayers][i],100.0);
      SetPlayerSkin(gmTemp[aWGPlayers][i],pTemp[gmTemp[aWGPlayers][i]][skinBeforeChange]);
      SetSpawnInfo(gmTemp[aWGPlayers][i],gmTemp[aWGPlayers][i],pTemp[gmTemp[aWGPlayers][i]][skinBeforeChange],0.0, 0.0, 0.0,0.0,0,0,0,0,0,0);
      SpawnPlayer(gmTemp[aWGPlayers][i]);
    }
    gmTemp[aWGPlayers][i]=INVALID_PLAYER_ID;
  }
  aData[A_WG][aState] = A_STATE_OFF;
  gmTemp[aWGMaxPlayers]=0;
  return 1;
}

public WG_Countdown()
{
  new
   buffer[64];

  if(gmTemp[aWGCount] <= 0)
  {
    format(buffer, sizeof buffer, "~g~GO!");
    KillTimer(gmTemp[timerWGCountdown]);
  }
  else
  {
    format(buffer, sizeof buffer, "~r~%i", gmTemp[aWGCount]);
  }

  for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++) {
      if(!IsPlayerConnected(gmTemp[aWGPlayers][playerid])) continue;
    GameTextForPlayer(gmTemp[aWGPlayers][playerid], buffer, 1000, 6);
    if(gmTemp[aWGCount] == 0)
    {
      TogglePlayerControllable(gmTemp[aWGPlayers][playerid], 1);
      PlaySound(gmTemp[aWGPlayers][playerid], 1057);
    } else
      PlaySound(gmTemp[aWGPlayers][playerid], 1056);


  }
  gmTemp[aWGCount]--;
}

WG_Update(){
  new _hp[2],Float:tmphp,_cnt[2],buf[128];
  for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++)
  if (!IsPlayerConnected(gmTemp[aWGPlayers][playerid])) {
      gmTemp[aWGPlayers][playerid]=INVALID_PLAYER_ID;
      continue;
  } else if (!pTemp[gmTemp[aWGPlayers][playerid]][aWGDead] && GetPlayerVirtualWorld(gmTemp[aWGPlayers][playerid])==VW_WG)
  {
    if (pTemp[gmTemp[aWGPlayers][playerid]][staleTime]>5 && !IsPlayerAiming(playerid)) {
      Msg(gmTemp[aWGPlayers][playerid],COLOR_ERROR,"Zostajesz wyrzucony z {b}WG{/b} za bycie {b}AFK{/b}");
      SetPlayerHealth(gmTemp[aWGPlayers][playerid],0);
      pData[gmTemp[aWGPlayers][playerid]][pAttraction]=A_NONE;
      gmTemp[aWGPlayers][playerid]=INVALID_PLAYER_ID;
      continue;
    }
    new bool:pozaS=false, bool:pozaC=false;

    if (aWGWBSphere[0]==aWGWBSphere[0] ) {
      if (GetPlayerDistanceFromPoint(gmTemp[aWGPlayers][playerid], aWGWBSphere[0], aWGWBSphere[1], aWGWBSphere[2])> aWGWBSphere[3]) {
        pozaS=true;
      }
    } else if (!aWGWBmode) pozaS=true;

    if (aWGWBCube[0]==aWGWBCube[0]) {
      if (!IsPlayerInCube(gmTemp[aWGPlayers][playerid],
          aWGWBCube[3], aWGWBCube[4], aWGWBCube[5],
          aWGWBCube[0], aWGWBCube[1], aWGWBCube[2] )) {
        pozaC=true;

      }
    } else if (!aWGWBmode) pozaC=true;

    if (aWGWBCube[0]==aWGWBCube[0] || aWGWBSphere[0]==aWGWBSphere[0])
    if (gmTemp[aWGCount]<=0 &&
        (
          (aWGWBmode && (pozaS || pozaC))       // oraz!
          ||
          (!aWGWBmode && pozaS && pozaC)        // lub
        ))
       {
      if(WGPozaStrefa[playerid]>0) {
        format(buf, sizeof buf,"~r~Wracaj na arene!~n~~w~ %d s", WGPozaStrefa[playerid]);
        WGPozaStrefa[playerid]--;
        GameTextForPlayer(gmTemp[aWGPlayers][playerid], buf,1000,3);
        FlashScreen(gmTemp[aWGPlayers][playerid]);
        PlaySound(gmTemp[aWGPlayers][playerid], 1135);


      } else {
        Msg(gmTemp[aWGPlayers][playerid], COLOR_ERROR, "Znajdowales sie poza arena zbyt dlugo.");
        SetPlayerHealth(gmTemp[aWGPlayers][playerid],0);
      }

    } else if (WGPozaStrefa[playerid]<10)
      WGPozaStrefa[playerid]++;


    GetPlayerHealth(gmTemp[aWGPlayers][playerid], tmphp);
    _hp[pTemp[gmTemp[aWGPlayers][playerid]][aWGTeam]]+=floatround(tmphp);
    _cnt[pTemp[gmTemp[aWGPlayers][playerid]][aWGTeam]]++;
  }
  _hp[0]+=1;  _hp[1]+=1;  // zabezpieczenie przeciwko dzieleniu przez 0
  SetProgressBarMaxValue(gmTemp[aWGPBar],_hp[0]+_hp[1]);
  SetProgressBarValue(gmTemp[aWGPBar],_hp[0]);
  SetProgressBarColor(gmTemp[aWGPBar], 0x00000000+ (floatround( 255* _hp[0] / (_hp[0]+_hp[1]))<<16) + (floatround( 255* _hp[1] / (_hp[0]+_hp[1]))<<8) + 0xff);


  format(buf,sizeof buf,"~g~%d   ~w~vs   ~b~%d", _cnt[0], _cnt[1]);
  TextDrawSetString(gTextDraw[TD_AWGINFO], buf);
  format(buf,sizeof buf,"~g~%dHP               ~r~%dHP~n~~n~~w~%s_(%d)", _hp[0],_hp[1],gmTemp[aWGDescr],gmTemp[aWGArena]);
  TextDrawSetString(gTextDraw[TD_AWGINFOHP], buf);

  for(new playerid = 0; playerid < gmTemp[aWGMaxPlayers]; playerid++)
    if (gmTemp[aWGPlayers][playerid]!=INVALID_PLAYER_ID && pData[gmTemp[aWGPlayers][playerid]][pAttraction]==A_WG)
      UpdateProgressBar(gmTemp[aWGPBar],  gmTemp[aWGPlayers][playerid]);

  // Team BLUE wins
  if(_cnt[A_WG_TEAM_GREEN] < 1 && _cnt[A_WG_TEAM_BLUE] >= 1)
  {
    foreach(playerid)
    {
      if(pData[playerid][pAttraction] == A_WG && pTemp[playerid][aWGTeam] == A_WG_TEAM_BLUE)
      {
        Msg(playerid, COLOR_INFO3, TXT(playerid, 433)); // Twoja dru¿yna wygrywa wojnê gangów! Respekt +5
        GivePlayerScore(playerid,(5+floatround(gmTemp[aWGMaxPlayers]/2))*((pTemp[playerid][aWGDead]) ? 2 : 1),"WG");
      }
      else
        Msg(playerid, COLOR_INFO3, TXT(playerid, 432)); // Niebiescy wygrywaj¹ wojnê gangów!

    }
    WG_Finish();
  }

  // Team GREEN wins
  else if(_cnt[A_WG_TEAM_BLUE] < 1 && _cnt[A_WG_TEAM_GREEN] >= 1)
  {
    foreach(playerid)
    {
      if(pData[playerid][pAttraction] == A_WG && pTemp[playerid][aWGTeam] == A_WG_TEAM_GREEN)
      {
        Msg(playerid, COLOR_INFO3, TXT(playerid, 433)); // Twoja dru¿yna wygrywa wojnê gangów
        GivePlayerScore(playerid,(5+floatround(gmTemp[aWGMaxPlayers]/2))*((pTemp[playerid][aWGDead]) ? 2 : 1),"WG");
      }
      else
        Msg(playerid, COLOR_INFO3, TXT(playerid, 431)); // Zieloni wygrywaj¹ wojnê gangów!
    }
    WG_Finish();
  }

  // Noone wins
  else if(_cnt[A_WG_TEAM_BLUE] < 1 && _cnt[A_WG_TEAM_GREEN] < 1)
  {
    foreach(playerid)
      Msg(playerid, COLOR_INFO3, TXT(playerid, 429)); // WG zosta³o przerwane z powodu braku graczy.
    WG_Finish();
  }

  return;
}
// EOF
