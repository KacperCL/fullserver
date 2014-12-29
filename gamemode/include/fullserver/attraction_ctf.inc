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

#define CTF_FINISH_POINT 3

new Float:aCTFWBCube[6],
    Float:aCTFWBSphere[4],
  Float:aCTFFlagaa[3],
  CTFPozaStrefa[MAX_QUEUE_CTF]={10,...},
  aCTFWBmode,
  aCTFPoints_green,
  aCTFPoints_blue,
  bool:flagaPoza,
  Float:aCTFSpawnFlagi[2][3];

forward CTF_Countdown();

CTF_Start(){
  new
   aPlayers[MAX_QUEUE_CTF],
   aMaxPlayers = 0,
   buffer[255],
   _skill[2],
   _teamsize[2],
   wb_sphere[80],
   flaga_spawn[80],
   flaga_team[2][80],
   wb_cube[80];

  foreach(playerid)
  {
    if(pData[playerid][aCTF])
    {
      if(AttractionAllowed(playerid) && !pTemp[playerid][godMode])
      {
        _teamsize[aMaxPlayers%2]++;
        aPlayers[aMaxPlayers++] = playerid;
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
      }
      else
      {
        pData[playerid][aCTF] = false;
        Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
      }
    }
  }

  if(aMaxPlayers < MIN_QUEUE_CTF)
  {
    foreach(playerid)
    {
      Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}CTF{/b}, zapisy ponowione.");
      pData[playerid][aCTF] = false;
    }

    aData[A_CTF][aState] = A_STATE_OFF;
    return 0;
  }

  gmTemp[aCTFBar]=CreateProgressBar(453.00, 356.00, 166.50, 4.50, 16777215, aMaxPlayers*100);

  // losowanie areny
  if (gmTemp[aCTFForceArena]>0) {
      format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz'),IFNULL(flaga_spawn,'-'),flaga_team0,flaga_team1 from fs_ctf_arena where id=%d LIMIT 1;",gmTemp[aCTFForceArena]);
    gmTemp[aCTFForceArena]=0;
  } else
      format(buffer,sizeof buffer,"select id,descr,interior,IFNULL(wb_sphere,'-'),IFNULL(wb_cube,'-'),(wb_mode='oraz'),IFNULL(flaga_spawn,'-'),flaga_team0,flaga_team1 from fs_ctf_arena where minplayers<=%d AND %d<=maxplayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
    mysql_query(buffer);
    mysql_store_result();
    if (!mysql_num_rows()) {    // nie znaleziono odpowiedniej areny
        foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/CTF{/b} nie wystartowalo - nie znaleziono odpowiedniej areny");

    CTF_Finish();
    mysql_free_result();
        return 0;
    }

    mysql_fetch_row_format(buffer,"|");
  mysql_free_result();

    if (sscanf(buffer,"p<|>ds[63]ds[63]s[63]ds[63]s[63]s[63]",gmTemp[aCTFArena],gmTemp[aCTFDescr],gmTemp[aCTFInterior], wb_sphere,wb_cube,aCTFWBmode,flaga_spawn,flaga_team[A_CTF_TEAM_GREEN],flaga_team[A_CTF_TEAM_BLUE])) {
        foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/CTF{/b} nie wystartowalo - nie znaleziono odpowiedniej areny");
    CTF_Finish();
        return 0;
  }
  aCTFWBSphere[0]=FLOAT_NAN;
  sscanf(wb_sphere,"p<,>ffff", aCTFWBSphere[0],aCTFWBSphere[1],aCTFWBSphere[2],aCTFWBSphere[3]);

  aCTFWBCube[0]=FLOAT_NAN;
  sscanf(wb_cube,"p<,>ffffff", aCTFWBCube[0],aCTFWBCube[1],aCTFWBCube[2],aCTFWBCube[3],aCTFWBCube[4],aCTFWBCube[5]);

    aCTFFlagaa[0]=FLOAT_NAN;
  sscanf(flaga_spawn,"p<,>fff", aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2]);

  DestroyObject(gmTemp[aCTFFlagaObj]);
  gmTemp[aCTFFlagaObj] = CreateObject(11245,aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2],0, 288.53997802734, 328.23999023438);
  // MoveObject(gmTemp[aCTFFlagaObj],aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2],999);
  gmTemp[aCTFFlaga]=INVALID_PLAYER_ID;

  aCTFSpawnFlagi[A_CTF_TEAM_BLUE][0]=FLOAT_NAN;
  sscanf(flaga_team[A_CTF_TEAM_BLUE],"p<,>fff", aCTFSpawnFlagi[A_CTF_TEAM_BLUE][0],aCTFSpawnFlagi[A_CTF_TEAM_BLUE][1],aCTFSpawnFlagi[A_CTF_TEAM_BLUE][2]);

  aCTFSpawnFlagi[A_CTF_TEAM_GREEN][0]=FLOAT_NAN;
  sscanf(flaga_team[A_CTF_TEAM_GREEN],"p<,>fff", aCTFSpawnFlagi[A_CTF_TEAM_GREEN][0],aCTFSpawnFlagi[A_CTF_TEAM_GREEN][1],aCTFSpawnFlagi[A_CTF_TEAM_GREEN][2]);

    TextDrawSetString(gTextDraw[TD_ACTFINFO],"~g~-   ~w~vs   ~b~-");
    TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~y~Odliczanie");

  format(buffer,sizeof buffer,"(SELECT team,X,Y,Z,A FROM fs_ctf_arena_sp WHERE aid=%d AND team=0 ORDER BY RAND() LIMIT %d) UNION (SELECT team,X,Y,Z,A FROM fs_ctf_arena_sp WHERE aid=%d AND team=1 ORDER BY rand() LIMIT %d)", gmTemp[aCTFArena], _teamsize[0], gmTemp[aCTFArena], _teamsize[1]);
  mysql_query(buffer);
  mysql_store_result();

    aCTFPoints_green=0;
    aCTFPoints_blue=0;

  new p=0;
  while (mysql_fetch_row_format(buffer,"|") && p<aMaxPlayers) {
    new team,Float:PX,Float:PY,Float:PZ,Float:PA;

    CTFPozaStrefa[p]=10;
    sscanf(buffer,"p<|>dffff",team, PX, PY, PZ, PA);

    pTemp[aPlayers[p]][aCTFTeam]=team;
    pTemp[aPlayers[p]][aCTFDead]=false;
    _skill[team]+=pData[aPlayers[p]][skill];

    SetPlayerHealth(aPlayers[p],99.0);
    SetPlayerArmour(aPlayers[p],0.0);
    Streamer_UpdateEx(aPlayers[p],PX,PY,PZ+2.4);

    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_PISTOL_SILENCED, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
    SetPlayerSkillLevel(aPlayers[p],WEAPONSKILL_MICRO_UZI, 0);

    TogglePlayerControllable(aPlayers[p],false);
    ResetPlayerWeapons(aPlayers[p]);
    SetPlayerPos(aPlayers[p],PX,PY,PZ+2.4);
    Streamer_Update(aPlayers[p]);
    SetPlayerFacingAngle(aPlayers[p],PA);
    SetPlayerTeam(aPlayers[p], team);
    SetPlayerVirtualWorld(aPlayers[p], VW_CTF);
    SetPlayerInterior(aPlayers[p], gmTemp[aCTFInterior]);

    pData[aPlayers[p]][pAttraction] = A_CTF;
    pData[aPlayers[p]][aCTF] = false;

        TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_ACTFINFO]);
        TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_ACTFSTATETITLE]);
        TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_ACTFSTATETEXT]);
    TextDrawShowForPlayer(aPlayers[p],gTextDraw[TD_ACTFINFOPOINTS]);
    ShowProgressBarForPlayer(aPlayers[p],gmTemp[aCTFBar]);

    TogglePlayerControllable(aPlayers[p], false);
    GivePlayerWeapon(aPlayers[p],24,1000);
    GivePlayerWeapon(aPlayers[p],25,200);
    GivePlayerWeapon(aPlayers[p],34,1000);

    DisablePlayerRaceCheckpoint(aPlayers[p]);
    DisablePlayerCheckpoint(aPlayers[p]);

        pTemp[aPlayers[p]][skinBeforeChange] = GetPlayerSkin(aPlayers[p]);
    if (team==A_CTF_TEAM_GREEN){
      SetPlayerColor(aPlayers[p],0x00FF00FF);
      SetPlayerSkin(aPlayers[p],106);
    }else{
      SetPlayerColor(aPlayers[p],0x0000FFFF);
      SetPlayerSkin(aPlayers[p],104);
    }
    p++;
  }

  mysql_free_result();

  gmTemp[aCTFPlayers] = aPlayers;
  gmTemp[aCTFMaxPlayers] = aMaxPlayers;
  gmTemp[aCTFCount] = 3;
  gmTemp[timerCTFCountdown] = SetTimer("CTF_Countdown", 1000, true);
  gmTemp[aCTFStartTick] = GetTickCount();
  CTF_Update();
  return 1;
}

CTF_Finish(){
  if (_:gmTemp[aCTFBar]!=-1) {
    HideProgressBarForAll(gmTemp[aCTFBar]);
    DestroyProgressBar(gmTemp[aCTFBar]);
  }
  TextDrawHideForAll(gTextDraw[TD_ACTFINFO]);
  TextDrawHideForAll(gTextDraw[TD_ACTFINFOPOINTS]);
  TextDrawHideForAll(gTextDraw[TD_ACTFSTATETITLE]);
    TextDrawHideForAll(gTextDraw[TD_ACTFSTATETEXT]);
  aData[A_CTF][aState] = A_STATE_OFF;

  for(new i = 0; i < gmTemp[aCTFMaxPlayers]; i++) {
    if (gmTemp[aCTFPlayers][i]==INVALID_PLAYER_ID) continue;
    if (!IsPlayerConnected(gmTemp[aCTFPlayers][i])) {
      gmTemp[aCTFPlayers][i]=INVALID_PLAYER_ID;
      continue;
    }

    pData[gmTemp[aCTFPlayers][i]][pAttraction] = A_NONE;
    SetPlayerTeam(gmTemp[aCTFPlayers][i], gmTemp[aCTFPlayers][i]+1);
    if (GetPlayerState(gmTemp[aCTFPlayers][i])!=PLAYER_STATE_WASTED && !pTemp[gmTemp[aCTFPlayers][i]][aCTFexit] && GetPlayerVirtualWorld(gmTemp[aCTFPlayers][i])==VW_CTF) {
      SetPlayerHealth(gmTemp[aCTFPlayers][i],100.0);
      SetPlayerSkin(gmTemp[aCTFPlayers][i],pTemp[gmTemp[aCTFPlayers][i]][skinBeforeChange]);
      SetSpawnInfo(gmTemp[aCTFPlayers][i],gmTemp[aCTFPlayers][i],pTemp[gmTemp[aCTFPlayers][i]][skinBeforeChange],0.0, 0.0, 0.0,0.0,0,0,0,0,0,0);
      SpawnPlayer(gmTemp[aCTFPlayers][i]);
      SetPlayerSkin(gmTemp[aCTFPlayers][i],pTemp[gmTemp[aCTFPlayers][i]][skinBeforeChange]);
    }
    DisablePlayerRaceCheckpoint(i);
    DisablePlayerCheckpoint(i);
    gmTemp[aCTFPlayers][i]=INVALID_PLAYER_ID;
  }
  DestroyObject(gmTemp[aCTFFlagaObj]);
  aData[A_CTF][aState] = A_STATE_OFF;
  gmTemp[aCTFMaxPlayers]=0;
  return 1;
}

public CTF_Countdown()
{
  new
   buffer[64],team;

  if(gmTemp[aCTFCount] <= 0)
  {
    format(buffer, sizeof buffer, "~g~GO!");
    KillTimer(gmTemp[timerCTFCountdown]);
    TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~y~Flaga na spawnie");
  }
  else
  {
    format(buffer, sizeof buffer, "~r~%i", gmTemp[aCTFCount]);
  }

  for(new playerid = 0; playerid < gmTemp[aCTFMaxPlayers]; playerid++) {
      if(!IsPlayerConnected(gmTemp[aCTFPlayers][playerid])) continue;
    GameTextForPlayer(gmTemp[aCTFPlayers][playerid], buffer, 1000, 6);
    if(gmTemp[aCTFCount] == 0)
    {
      TogglePlayerControllable(gmTemp[aCTFPlayers][playerid], 1);
      PlaySound(gmTemp[aCTFPlayers][playerid], 1057);

      team=pTemp[gmTemp[aCTFPlayers][playerid]][aCTFTeam];
      SetPlayerCheckpoint(gmTemp[aCTFPlayers][playerid], aCTFSpawnFlagi[team][0], aCTFSpawnFlagi[team][1], aCTFSpawnFlagi[team][2], 4.0);
      SetPlayerRaceCheckpoint(gmTemp[aCTFPlayers][playerid],2,aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2],0.0,0.0,0.0,4);
    } else
      PlaySound(gmTemp[aCTFPlayers][playerid], 1056);

  }

  gmTemp[aCTFCount]--;
}

CTF_Update(){
  new _cnt[2],buf[128];
  new Float:ccx, Float:ccy, Float:ccz;
  GetObjectPos(gmTemp[aCTFFlagaObj], ccx, ccy, ccz);
  if(/*(ccx==-1 || ccy==-1 || ccz==-1) && */(IsValidObject(gmTemp[aCTFFlagaObj]) && gmTemp[aCTFFlaga]==INVALID_PLAYER_ID) && gmTemp[aCTFCount] == 0){
      if(flagaPoza){
          CTF_RespawnFlagi();
    }else flagaPoza=true;

  }

  for(new playerid = 0; playerid < gmTemp[aCTFMaxPlayers]; playerid++)
  if (!IsPlayerConnected(gmTemp[aCTFPlayers][playerid])) {
      gmTemp[aCTFPlayers][playerid]=INVALID_PLAYER_ID;
      continue;
  } else if (GetPlayerVirtualWorld(gmTemp[aCTFPlayers][playerid])==VW_CTF)
  {
    if (pTemp[gmTemp[aCTFPlayers][playerid]][staleTime]>5) {
      Msg(gmTemp[aCTFPlayers][playerid],COLOR_ERROR,"Zostajesz wyrzucony z {b}CTF{/b} za bycie {b}AFK{/b}");
        if(gmTemp[aCTFFlaga]==playerid) CTF_RespawnFlagi();
      SetPlayerHealth(gmTemp[aCTFPlayers][playerid],0);
      pData[gmTemp[aCTFPlayers][playerid]][pAttraction]=A_NONE;
      gmTemp[aCTFPlayers][playerid]=INVALID_PLAYER_ID;
      pTemp[playerid][aCTFexit]=true;
      continue;
    }
    new bool:pozaS=false, bool:pozaC=false;

    if (aCTFWBSphere[0]==aCTFWBSphere[0] ) {
      if (GetPlayerDistanceFromPoint(gmTemp[aCTFPlayers][playerid], aCTFWBSphere[0], aCTFWBSphere[1], aCTFWBSphere[2])> aCTFWBSphere[3]) {
        pozaS=true;
      }
    } else if (!aCTFWBmode) pozaS=true;

    if (aCTFWBCube[0]==aCTFWBCube[0]) {
      if (!IsPlayerInCube(gmTemp[aCTFPlayers][playerid],
          aCTFWBCube[3], aCTFWBCube[4], aCTFWBCube[5],
          aCTFWBCube[0], aCTFWBCube[1], aCTFWBCube[2] )) {
        pozaC=true;

      }
    } else if (!aCTFWBmode) pozaC=true;

    if (aCTFWBCube[0]==aCTFWBCube[0] || aCTFWBSphere[0]==aCTFWBSphere[0])
    if (gmTemp[aCTFCount]<=0 &&
        (
          (aCTFWBmode && (pozaS || pozaC))        // oraz!
          ||
          (!aCTFWBmode && pozaS && pozaC)       // lub
        ))
       {
      if(CTFPozaStrefa[playerid]>0) {
        format(buf, sizeof buf,"~r~Wracaj na arene!~n~~w~ %d s", CTFPozaStrefa[playerid]);
        CTFPozaStrefa[playerid]--;
        GameTextForPlayer(gmTemp[aCTFPlayers][playerid], buf,1000,3);
        FlashScreen(gmTemp[aCTFPlayers][playerid]);
        PlaySound(gmTemp[aCTFPlayers][playerid], 1135);


      } else {
        Msg(gmTemp[aCTFPlayers][playerid], COLOR_ERROR, "Znajdowales sie poza arena zbyt dlugo.");
        SetPlayerHealth(gmTemp[aCTFPlayers][playerid],0);
      }

    } else if (CTFPozaStrefa[playerid]<10)
      CTFPozaStrefa[playerid]++;

        _cnt[pTemp[gmTemp[aCTFPlayers][playerid]][aCTFTeam]]++;
  }
  static blue_tmp;
  static green_tmp;

  blue_tmp=aCTFPoints_blue;
  green_tmp=aCTFPoints_green;

  blue_tmp+=1;  green_tmp+=1; // zabezpieczenie przeciwko dzieleniu przez 0
  SetProgressBarMaxValue(gmTemp[aCTFBar],blue_tmp+green_tmp);
  SetProgressBarValue(gmTemp[aCTFBar],green_tmp);
  SetProgressBarColor(gmTemp[aCTFBar], 0x00000000+ (floatround( 255* green_tmp / (green_tmp+blue_tmp))<<16) + (floatround( 255* blue_tmp / (green_tmp+blue_tmp))<<8) + 0xff);


  format(buf,sizeof buf,"~g~%d   ~w~vs   ~b~%d", _cnt[0], _cnt[1]);
  TextDrawSetString(gTextDraw[TD_ACTFINFO], buf);
  format(buf,sizeof buf,"~g~%d PKT               ~b~%d PKT~n~~n~~w~%s_(%d)", aCTFPoints_green,aCTFPoints_blue,gmTemp[aCTFDescr],gmTemp[aCTFArena]);
  TextDrawSetString(gTextDraw[TD_ACTFINFOPOINTS], buf);

  for(new playerid = 0; playerid < gmTemp[aCTFMaxPlayers]; playerid++)
    if (gmTemp[aCTFPlayers][playerid]!=INVALID_PLAYER_ID && pData[gmTemp[aCTFPlayers][playerid]][pAttraction]==A_CTF)
      UpdateProgressBar(gmTemp[aCTFBar],  gmTemp[aCTFPlayers][playerid]);

  // Team BLUE wins
  if(aCTFPoints_blue==CTF_FINISH_POINT)
  {
    foreach(playerid)
    {
      if(pData[playerid][pAttraction] == A_CTF && pTemp[playerid][aCTFTeam] == A_CTF_TEAM_BLUE)
      {
        Msg(playerid, COLOR_INFO3, "Twoja druzyna wygrywa {b}/CTF{/b}!"); // Twoja dru¿yna ctf!
        GivePlayerScore(playerid,(5+floatround(gmTemp[aCTFMaxPlayers]/2))*((pTemp[playerid][aCTFDead]) ? 2 : 1),"CTF");
      }
      else
        Msg(playerid, -1, "Niebiescy wygrywaja {ffff00}/CTF{ffffff}"); // Niebiescy wygrywaj¹ ctf!

    }
    CTF_Finish();
  }

  // Team GREEN wins
  else if(aCTFPoints_green==CTF_FINISH_POINT)
  {
    foreach(playerid)
    {
      if(pData[playerid][pAttraction] == A_CTF && pTemp[playerid][aCTFTeam] == A_CTF_TEAM_GREEN)
      {
        Msg(playerid, COLOR_INFO3, "Twoja druzyna wygrywa {b}/CTF{/b}!"); // Twoja dru¿yna ctf!
        GivePlayerScore(playerid,(5+floatround(gmTemp[aCTFMaxPlayers]/2))*((pTemp[playerid][aCTFDead]) ? 2 : 1),"CTF");
      }
      else
        Msg(playerid, -1, "Zieloni wygrywaja {ffff00}/CTF{ffffff}"); // Zieloni wygrywaj¹ ctf!
    }
    CTF_Finish();
  }

  // None wins
  else if(_cnt[A_CTF_TEAM_BLUE] < 1 || _cnt[A_CTF_TEAM_GREEN] < 1)
  {
    foreach(playerid)
      Msg(playerid, COLOR_INFO3, "{b}/CTF{/b} zostalo przerwane z powodu braku graczy"); // CTF zosta³o przerwane z powodu braku graczy.
    CTF_Finish();
  }

  return;
}

CTF_OPEFlagaRCP(playerid){
  DestroyObject(gmTemp[aCTFFlagaObj]);
  gmTemp[aCTFFlagaObj] = CreateObject(11245,-100,-100,-100,0, 288.53997802734, 328.23999023438);
  // MoveObject(gmTemp[aCTFFlagaObj],-1,-1,-1,999);
    AttachObjectToPlayer(gmTemp[aCTFFlagaObj], playerid, 1.5, 0.5, 0.0, 0.0, 1.5, 2);
    gmTemp[aCTFFlaga]=playerid;
    SetPlayerHealth(playerid,10.0);
    if(pTemp[playerid][aCTFTeam] == A_CTF_TEAM_BLUE) {
        TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~b~Flaga w rekach niebieskich");
        foreach(i){
            if(pData[i][pAttraction] == A_CTF){
            DisablePlayerRaceCheckpoint(i);
          }
     }
  }else{
      TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~g~Flaga w rekach zielonych");
      foreach(i){
          if(pData[i][pAttraction] == A_CTF){
        DisablePlayerRaceCheckpoint(i);
      }
    }
  }
  return 1;
}

CTF_OPETeamCP(playerid){
    if(gmTemp[aCTFFlaga]!=playerid) return Msg(playerid,COLOR_ERROR,"Wroc tu z flaga ;-)");
      // setTimer ( respawnFlagi )
      gmTemp[aCTFFlaga]=INVALID_PLAYER_ID;
    DestroyObject(gmTemp[aCTFFlagaObj]);
    SetPlayerHealth(playerid,99.0);
      if(pTemp[playerid][aCTFTeam]==A_CTF_TEAM_BLUE){
      aCTFPoints_blue=aCTFPoints_blue+1;
      if(aCTFPoints_blue>=CTF_FINISH_POINT){
          TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~b~Punkt dla niebieskich");
      }else{
            SetTimer("CTF_RespawnFlagi",4000,0); // this is our anserw to ghost flags on -1,-1,-1? (look up)
            TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~b~Punkt dla niebieskich~n~~y~Respawn flagi za 4 sek");
      }
    }else{
      aCTFPoints_green=aCTFPoints_green+1;
        if(aCTFPoints_green>=CTF_FINISH_POINT){
            TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~g~Zieloni wygrali");
      }else{
          SetTimer("CTF_RespawnFlagi",4000,0); // this is our anserw to ghost flags on -1,-1,-1? (look up)
          TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~g~Punkt dla zielonych~n~~y~Respawn flagi za 4 sek");
      }
    }
  return 1;
}

forward CTF_UpuscFlage(playerid,Float:x,Float:y,Float:z);
public CTF_UpuscFlage(playerid,Float:x,Float:y,Float:z)
{
  if(aData[A_CTF][aState] != A_STATE_ON) return 0;
  DestroyObject(gmTemp[aCTFFlagaObj]);
  gmTemp[aCTFFlagaObj] = CreateObject(11245,x,y,z,0, 288.53997802734, 328.23999023438);
  // MoveObject(gmTemp[aCTFFlagaObj],x,y,z,999);
  gmTemp[aCTFFlaga]=INVALID_PLAYER_ID;
  TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~y~Flaga na ziemi");
  flagaPoza=false; // ???
  foreach(i){
    if(pData[i][pAttraction] == A_CTF){
      SetPlayerRaceCheckpoint(i,2,x,y,z,0.0,0.0,0.0,4);
    }
  }
  return 1;
}

forward CTF_RespawnFlagi();
public CTF_RespawnFlagi(){
  if(aData[A_CTF][aState] != A_STATE_ON) return 0;
    DestroyObject(gmTemp[aCTFFlagaObj]);
  gmTemp[aCTFFlagaObj] = CreateObject(11245,aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2],0, 288.53997802734, 328.23999023438);
  // MoveObject(gmTemp[aCTFFlagaObj],aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2],999);
  gmTemp[aCTFFlaga]=INVALID_PLAYER_ID;
  flagaPoza=false;
  TextDrawSetString(gTextDraw[TD_ACTFSTATETEXT],"~y~Flaga na spawnie");
  foreach(i){
    if(pData[i][pAttraction] == A_CTF){
      SetPlayerRaceCheckpoint(i,2,aCTFFlagaa[0],aCTFFlagaa[1],aCTFFlagaa[2],0.0,0.0,0.0,4);
    }
  }
  return 1;
}

CTF_onPlayerDeath(playerid)
{
    pTemp[playerid][aCTFDead]=true;
  return 1;
}

CTF_ReSpawn(playerid)
{
  new buffer[128],Float:cx,Float:cy,Float:cz,Float:ca;
  if(gmTemp[aCTFFlaga]==playerid){
    GetPlayerPos(playerid,cx,cy,cz);
    SetTimerEx("CTF_UpuscFlage",400,0,"dfff",playerid,cx,cy,cz); // ghost flag here!?
  }
  format(buffer,sizeof buffer,"SELECT X,Y,Z,A FROM fs_ctf_arena_sp WHERE aid=%d AND team=%d ORDER BY rand() LIMIT 1", gmTemp[aCTFArena], pTemp[playerid][aCTFTeam]);
  mysql_query(buffer);
  mysql_store_result();

  mysql_fetch_row_format(buffer,"|");

    sscanf(buffer,"p<|>ffff",cx,cy,cz,ca);
    Streamer_UpdateEx(playerid,cx,cy,cz+2.4);
    SetPlayerFacingAngle(playerid,ca);
    SetPlayerInterior(playerid, gmTemp[aCTFInterior]);
    SetPlayerArmour(playerid,0.0);
    SetPlayerHealth(playerid,99.0);
    SetPlayerPos(playerid,cx,cy,cz+2.4);
    SetPlayerVirtualWorld(playerid,VW_CTF);
    Streamer_Update(playerid);
    SetCameraBehindPlayer(playerid);

  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, 0);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, 0);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, 0);
  ResetPlayerWeapons(playerid);

  GivePlayerWeapon(playerid,24,1000);
  GivePlayerWeapon(playerid,25,200);
  GivePlayerWeapon(playerid,34,1000);

  if (pTemp[playerid][aCTFTeam]==A_CTF_TEAM_GREEN){
    SetPlayerColor(playerid,0x00FF00FF);
    SetPlayerSkin(playerid,106);
  }else{
    SetPlayerColor(playerid,0x0000FFFF);
    SetPlayerSkin(playerid,104);
  }

  mysql_free_result();
  return 1;
}
// EOF
