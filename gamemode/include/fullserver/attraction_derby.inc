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

new aDerbyVehicles[MAX_QUEUE_DERBY]={INVALID_VEHICLE_ID,...};
new aDerbyVehiclesPos[MAX_QUEUE_DERBY][e_Vectors];
new derbyEnd=false;

Derby_Start()
{
  new
   aPlayers[MAX_QUEUE_DERBY],
   aMaxPlayers = 0, nitro=0;

  derbyEnd=false;

  for(new i=0; i<MAX_QUEUE_DERBY;i++)
    aPlayers[i]=INVALID_PLAYER_ID;

  foreach(playerid)
  {
    if(pData[playerid][aDerby])
    {
      if(AttractionAllowed(playerid))
      {
        aPlayers[aMaxPlayers++] = playerid;
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
      }
      else
      {
        pData[playerid][aDerby] = false;
        Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
      }
    }
  }

  if (aMaxPlayers<MIN_QUEUE_DERBY) {
    if (aMaxPlayers==0){
      foreach(playerid)
            Msg(playerid,COLOR_ERROR,"{b}/Derby{/b} nie wystartowaly, nikt z zapisanych nie mogl dolaczyc");
    }else{
      foreach(playerid)
            Msg(playerid,COLOR_ERROR,"{b}/Derby{/b} nie wystartowaly, nie udalo sie skompletowac minimalnej ilosci graczy.");
    }
    aData[A_DERBY][aState] = A_STATE_OFF;
    Derby_Cleanup();
    return 0;
  }


  // losujemy arene
  // select id,vehicle,interior from fs_derby_arena where minPlayers<=3<=maxPlayers ORDER by rand() LIMIT 1;
  new buffer[256]; //przy 26 graczach ucinalo 1 na koncu zapytania!
  if (gmTemp[aDerbyForceArena]>0) { // wybieramy wymuszona
    format(buffer,sizeof buffer,"select id,minZ,vehicle,interior,nitro,descr from fs_derby_arena where id=%d LIMIT 1;",gmTemp[aDerbyForceArena]);
    gmTemp[aDerbyForceArena]=0;
  } else // losujemy
    format(buffer,sizeof buffer,"select id,minZ,vehicle,interior,nitro,descr from fs_derby_arena where minPlayers<=%d AND %d<=maxPlayers ORDER by rand() LIMIT 1;",aMaxPlayers,aMaxPlayers);
  mysql_query(buffer);
  mysql_store_result();
  if (!mysql_num_rows()) {  // nie znaleziono odpowiedniej areny
    foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/Derby{/b} nie wystartowaly - nie znaleziono odpowiedniej areny");

    for(new i = 0; i < aMaxPlayers; i++)  {
      pData[aPlayers[i]][pAttraction] = A_DERBY;
      pData[aPlayers[i]][aDerby] = false;
    }
    aData[A_DERBY][aState] = A_STATE_OFF;
    Derby_Cleanup();
    mysql_free_result();
    return 1;
  }

  mysql_fetch_row_format(buffer," ");

  if (sscanf(buffer,"dddddS(Derby)[32]",gmTemp[aDerbyArena], gmTemp[aDerbyMinZ], gmTemp[aDerbyVehicle],gmTemp[aDerbyInterior],nitro, gmTemp[aDerbyArenaDescr])) {
        foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/Derby{/b} nie wystartowaly - nie znaleziono odpowiedniej areny");

    for(new i = 0; i < aMaxPlayers; i++)  {
      pData[aPlayers[i]][pAttraction] = A_NONE;//A_DERBY;
      pData[aPlayers[i]][aDerby] = false;
    }
    aData[A_DERBY][aState] = A_STATE_OFF;
    Derby_Cleanup();
    mysql_free_result();
    return 1;
  }
  mysql_free_result();

  // pobieramy dane spawnow
  // select X,Y,Z,angle from fs_derby_arena_sp where aid=1 order by rand() limit 4
  format(buffer,sizeof buffer,"select X,Y,Z,angle from fs_derby_arena_sp where aid=%d order by rand() limit %d",gmTemp[aDerbyArena],aMaxPlayers);
  mysql_query(buffer);
  mysql_store_result();
  for (new i=0;i<mysql_num_rows();i++){
    mysql_fetch_row_format(buffer," ");
    new Float:P[4];
    if (!sscanf(buffer,"ffff",P[0],P[1],P[2],P[3])) {

      pData[aPlayers[i]][pAttraction] = A_DERBY;
      pData[aPlayers[i]][aDerby] = false;

      TogglePlayerControllable(aPlayers[i], false);
      SetPlayerArmour(aPlayers[i], 0.0);
      SetPlayerHealth(aPlayers[i], 100.0);
      ResetPlayerWeapons(aPlayers[i]);

      if(pData[aPlayers[i]][hudSetting][HUD_DATE]) ShowElement(aPlayers[i], TDE_DATETIME, false);
      if(pData[aPlayers[i]][hudSetting][HUD_STATUSBAR]) ShowElement(aPlayers[i], TDE_STATS, false);
      if(pData[aPlayers[i]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(aPlayers[i], TDE_ATTRACTIONBOX, false);

      Teleport(T_PLAYER, aPlayers[i], P[0], P[1], P[2]+4, P[3], gmTemp[aDerbyInterior], VW_DERBY);
      Streamer_Update(aPlayers[i]);
      // monster
      aDerbyVehicles[i]=CreateVehicle(gmTemp[aDerbyVehicle],P[0], P[1], P[2]+3, P[3],random(253),random(253),1);
      if(aDerbyVehicles[i] == INVALID_VEHICLE_ID) {
        foreach(playerid)
          Msg(playerid,COLOR_ERROR,"{b}/Derby{/b} nie wystartowaly - blad z tworzeniem pojazdu");
        
        for(new a = 0; a < aMaxPlayers; a++)  {
          pData[aPlayers[a]][pAttraction] = A_NONE;//A_DERBY;
          pData[aPlayers[a]][aDerby] = false;
        }
        aData[A_DERBY][aState] = A_STATE_OFF;
        Derby_Cleanup();
        mysql_free_result();
        return 1;
      }
      aDerbyVehiclesPos[i][X]=P[0];
      aDerbyVehiclesPos[i][Y]=P[1];
      aDerbyVehiclesPos[i][Z]=P[2];
      tVehicles[aDerbyVehicles[i]][vo_used]=true;
      tVehicles[aDerbyVehicles[i]][vo_occupied]=true;
      SetVehicleNumberPlate(aDerbyVehicles[i],"DERBY");
      if (nitro>0){
        AddVehicleComponent(aDerbyVehicles[i], 1010);
        vehicleHasNitro[aDerbyVehicles[i]]=true;
        tVehicles[aDerbyVehicles[i]][vo_used]=true;
      }

      SetVehicleVirtualWorld(aDerbyVehicles[i],VW_DERBY);
      LinkVehicleToInterior(aDerbyVehicles[i],gmTemp[aDerbyInterior]);
      if (gmTemp[aDerbyArena]==9) // ugly ale jebac to
        SetVehicleHealth(aDerbyVehicles[i],999999);

      TogglePlayerControllable(aPlayers[i], false);

//      PutPlayerInVehicle(aPlayers[i],aDerbyVehicles[i],0);
    } else {
      pData[aPlayers[i]][pAttraction] = A_NONE;
      pData[aPlayers[i]][aDerby] = false;
    }
  }

  mysql_free_result();

    gmTemp[aDerbyPlayers] = aPlayers;
    gmTemp[aDerbyMaxPlayers] = aMaxPlayers;
    gmTemp[aDerbyCount] = 5;
  KillTimer(gmTemp[timerDerbyCountdown]);
    gmTemp[timerDerbyCountdown] = SetTimer("Derby_Countdown", 700, true);
    gmTemp[aDerbyStartTick] = GetTickCount();
  format(buffer,sizeof buffer,"~w~Arena_~n~~y~~h~%s", gmTemp[aDerbyArenaDescr]);
  for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++)
    GameTextForPlayer(aPlayers[i], buffer, 6000, 1);

  return 1;
}

Derby_Cleanup(){
  KillTimer(gmTemp[timerDerbyCountdown]);

  for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++) {
    if (gmTemp[aDerbyPlayers][i]!=INVALID_PLAYER_ID && GetPlayerVirtualWorld( gmTemp[aDerbyPlayers][i] )==VW_DERBY) {
      if(pData[gmTemp[aDerbyPlayers][i]][hudSetting][HUD_DATE]) ShowElement(gmTemp[aDerbyPlayers][i], TDE_DATETIME, true);
      if(pData[gmTemp[aDerbyPlayers][i]][hudSetting][HUD_STATUSBAR]) ShowElement(gmTemp[aDerbyPlayers][i], TDE_STATS, true);
      if(pData[gmTemp[aDerbyPlayers][i]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(gmTemp[aDerbyPlayers][i], TDE_ATTRACTIONBOX, true);
    }
    gmTemp[aDerbyPlayers][i]=INVALID_PLAYER_ID;
  }
  return;
}

Derby_RemovePlayer(playerid,bool:force=false){
  if (playerid==INVALID_PLAYER_ID) return 1;
  if (force || pData[playerid][pAttraction]==A_DERBY) {
    if (IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
    pData[playerid][pAttraction] = A_NONE;
    pData[playerid][aDerby] = false;
        if (GetPlayerState(playerid)!=PLAYER_STATE_WASTED)
      SpawnPlayer(playerid);
    avt_record(playerid,e_derbyfan, 1, ART_ADD);
    if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
    if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
    if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
    for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++) {
      if(gmTemp[aDerbyPlayers][i]==playerid){
        DestroyVehicle(aDerbyVehicles[i]); // fix for ghost vehicles
        aDerbyVehicles[i] = INVALID_VEHICLE_ID;
        gmTemp[aDerbyPlayers][i]=INVALID_PLAYER_ID;
      }
    }
    Derby_Recount();
  }
  return 1;
}

Derby_Recount(){
  if (gmTemp[aDerbyCount]>0) return 1;

  new pozostalo,lastPlayer=INVALID_PLAYER_ID;

  new Float:POS[3];

  for (new i=0;i<gmTemp[aDerbyMaxPlayers];i++)
    if (gmTemp[aDerbyPlayers][i]==INVALID_PLAYER_ID) continue;
    else if (pData[gmTemp[aDerbyPlayers][i]][pAttraction]==A_DERBY && GetPlayerVirtualWorld(gmTemp[aDerbyPlayers][i])==VW_DERBY) {

      GetPlayerPos(gmTemp[aDerbyPlayers][i], POS[0],POS[1],POS[2]);
      if (POS[2]<gmTemp[aDerbyMinZ]) { // gracz jest ponizej MinZ
        Msg(gmTemp[aDerbyPlayers][i],COLOR_ERROR,"Wypadles z areny {b}/derby{/b}!");
        // Derby_RemovePlayer(gmTemp[aDerbyPlayers][i]);
        new Float:P[6];
          GetPlayerPos(gmTemp[aDerbyPlayers][i],P[0],P[1],P[2]);
        GetPlayerVelocity(gmTemp[aDerbyPlayers][i],P[3],P[4],P[5]);

          CreateExplosion(P[0],P[1],P[2], 2, 35.0);
          CreateExplosion(P[0]+random(2)-1,P[1]+random(2)-1,P[2]+random(2)-1, 2, 35.0);
          CreateExplosion(P[0]+random(2)-1,P[1]+random(2)-1,P[2]+random(2)-1, 2, 35.0);

        CreateExplosion(P[0]+P[3],P[1]+P[4],P[2]+P[5], 2, 35.0);
        CreateExplosion(P[0]+(P[3]*2),P[1]+(P[4]*2),P[2]+(P[5]*2), 2, 35.0);
        SetPlayerHealth(gmTemp[aDerbyPlayers][i],0);
      } else if (pTemp[gmTemp[aDerbyPlayers][i]][staleTime]>4) {
        Msg(gmTemp[aDerbyPlayers][i],COLOR_ERROR,"Zostales usuniety z {b}/derby{/b} za {b}AFK{/b}!");
        Derby_RemovePlayer(gmTemp[aDerbyPlayers][i]);
      } else if (GetPlayerVehicleID(gmTemp[aDerbyPlayers][i])==aDerbyVehicles[i]) {
        pozostalo++;
        lastPlayer=gmTemp[aDerbyPlayers][i];
      } else
        Derby_RemovePlayer(gmTemp[aDerbyPlayers][i]);
    }

  if (pozostalo<=1 && !derbyEnd) {
    aData[A_DERBY][aState] = A_STATE_OFF;

    if (lastPlayer!=INVALID_PLAYER_ID)  {
      pData[lastPlayer][pAttraction]=A_NONE;
      new vid=GetPlayerVehicleID(lastPlayer);
      RemovePlayerFromVehicle(lastPlayer);
      RespawnVehicle(vid);

      new Float:P[3];
      GetPlayerPos(lastPlayer,P[0],P[1],P[2]);
      SetPlayerPos(lastPlayer,P[0],P[1],P[2]);

      if (GetPlayerState(lastPlayer)!=PLAYER_STATE_WASTED)
        SpawnPlayer(lastPlayer);
      new str[127];
      format(str,sizeof(str),"Derby wygrywa {ffff00}%s{ffffff}. Otrzymuje {ffff00}%d{ffffff} respektu.",GetPlayerProperName(lastPlayer),5+gmTemp[aDerbyMaxPlayers]);
      GivePlayerScore(lastPlayer, 5+gmTemp[aDerbyMaxPlayers], "wygrane Derby");
      avt_record(lastPlayer,e_derbywin, 1, ART_ADD);
      avt_record(lastPlayer,e_derbyfan, 1, ART_ADD);

        foreach(playerid)
            Msg(playerid,-1,str);
    }
    Derby_Cleanup();
    derbyEnd=true;
  }

  return 1;
}

forward Derby_Countdown();
public Derby_Countdown()
{
  new
   buffer[64];

  if(gmTemp[aDerbyCount] <= 0)
  {
    format(buffer, sizeof buffer, "~g~GO!");
    KillTimer(gmTemp[timerDerbyCountdown]);
  }
  else
  {
    format(buffer, sizeof buffer, "~r~%i", gmTemp[aDerbyCount]);
  }

  for(new playerid = 0; playerid < gmTemp[aDerbyMaxPlayers]; playerid++)
  {
    GameTextForPlayer(gmTemp[aDerbyPlayers][playerid], buffer, 1000, 6);

    switch(gmTemp[aDerbyCount]){
      case 0: {
        PlaySound(gmTemp[aDerbyPlayers][playerid], 1057);
        TogglePlayerControllable(gmTemp[aDerbyPlayers][playerid], true);
        SetPlayerColor(playerid, pData[playerid][currentColor] * 256+PLAYER_COLOR_ALPHA); // force non-invisible color, will be changed back after respawn or first call to SetPlayerProperColor
      }
      case 1: {
        PlaySound(gmTemp[aDerbyPlayers][playerid], 1057);
        PutPlayerInVehicle(gmTemp[aDerbyPlayers][playerid],aDerbyVehicles[playerid],0);
      }
      case 5: {
        PlaySound(gmTemp[aDerbyPlayers][playerid], 1056);
      }
      default: {
        PlaySound(gmTemp[aDerbyPlayers][playerid], 1056);
        SetVehiclePos(aDerbyVehicles[playerid],aDerbyVehiclesPos[playerid][X],aDerbyVehiclesPos[playerid][Y],aDerbyVehiclesPos[playerid][Z]);
      }
    }
  }

  gmTemp[aDerbyCount]--;
}
// EOF
