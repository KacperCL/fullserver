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

#define arena_checklimit(%0) if (Areny[%0][ea_limit]>0 && Areny[%0][ea_cnt]>=Areny[%0][ea_limit]) return Msg(playerid, COLOR_INFO2, "Nie mozesz dolaczyc, na tej arenie jest juz maksymalna ilosc graczy")

enum e_arena {
  ea_cmd[16],
  ea_limit,
  ea_cnt,
  ea_minfps
}

new Areny[ARENA_MAX][e_arena]={
    {""},
  {"/ARENASOLO"},
  {"/PUEBLO"},
  {"/SNIPER"},
  {"/MINIGUN"},
  {"/PRO",0,0},
  {"/ONEDE",0,0},
  {"/RIFLE"},
  {"/RPG"},
  {"/SO",10},
  {"/ONEDE2",0,0},
  {"/ONEGUN",20,0},
  {"/DM"},
  {"/CHAINSAW"},
  {"/CS",30,0}
};

Timer:areny_recount[30000](){ // 30sek
  for (new i=0;i<sizeof Areny;i++)
    Areny[i][ea_cnt]=0;

  foreach(i)
    if (pTemp[i][onArena]>0 && pTemp[i][onArena]<ARENA_MAX)
      Areny[pTemp[i][onArena]][ea_cnt]++;

  for (new i=0;i<sizeof Areny;i++)
   if (strlen(Areny[i][ea_cmd])>0) {
    if (gmTemp[popularArena]!=i && Areny[gmTemp[popularArena]][ea_cnt]<Areny[i][ea_cnt])
      gmTemp[popularArena]=i;
  }
}

CMD:arena(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid) || pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  areny_recount();

  new lista[1024];
  format(lista, sizeof lista, "Arena\tGracze\n");
  for (new i=0;i<sizeof Areny;i++)
   if (strlen(Areny[i][ea_cmd])>0) {
    if (Areny[i][ea_limit]<=0)
      format(lista, sizeof lista, "%s%s{%06x}%16s\t{ffffff}%d/-", lista, strlen(lista)>0?("\n"):(""), Areny[i][ea_cnt]>0?0x74c1f9:0x3481B9, Areny[i][ea_cmd], Areny[i][ea_cnt]);
    else
      format(lista, sizeof lista, "%s%s{%06x}%16s\t{ffffff}%d/%d", lista, strlen(lista)>0?("\n"):(""), Areny[i][ea_cnt]>0?0x74c1f9:0x3481B9, Areny[i][ea_cmd], Areny[i][ea_cnt], Areny[i][ea_limit]);

   }

  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_TABLIST_HEADERS,"Wybierz arene", lista, "OK", "Anuluj");
  return 1;
}
CMD:areny(playerid)
  return _arena(playerid);

CMD:arenaexit(playerid){
  MUST_BE_SPAWNED(playerid);
  if (pTemp[playerid][onArena]==ARENA_NONE)
    return Msg(playerid,COLOR_ERROR,__("Nie jestes na zadnej arenie!"));
  if (Areny[pTemp[playerid][onArena]][ea_cnt]>0) Areny[pTemp[playerid][onArena]][ea_cnt]--;
  pData[playerid][pAttraction] = A_NONE;
  pTemp[playerid][onArena]=ARENA_NONE;
  pData[playerid][hudSetting][HUD_FPS]=false;
  Msg(playerid, COLOR_INFO, __("Opusciles arene"));
  SetPlayerHealth(playerid,100.0);
  SpawnPlayer(playerid);
  return 1;
}
CMD:arenexit(playerid)
  return _arenaexit(playerid);

CMD:ae(playerid)
    return _arenaexit(playerid);

forward arena_CheckLimits(playerid,force);
public arena_CheckLimits(playerid,force){

    if(pTemp[playerid][godMode]) {
    Msg(playerid, COLOR_ERROR, __("Ta akcja jest teraz niedostepna."));
    pData[playerid][pAttraction]=A_NONE;
    pTemp[playerid][onArena]=ARENA_NONE;
    SpawnPlayer(playerid);
    return false;
  }

  if (Areny[pTemp[playerid][onArena]][ea_minfps]>0 && pTemp[playerid][ept_fps]<Areny[pTemp[playerid][onArena]][ea_minfps]) { // well... shit! we don't need it in 0.3z and its totally fucked up!
    if((pTemp[playerid][ept_fps]>1 && pTemp[playerid][ept_fps]<120) && !force){
      format(gstr,sizeof gstr,__("Twoja ilosc FPS ({b}%d{/b}) jest nizsza od wymaganej na tej arenie ({b}%d{/b}) - zostajesz usuniêty z tej areny"),pTemp[playerid][ept_fps],Areny[pTemp[playerid][onArena]][ea_minfps]);
      Msg(playerid,COLOR_INFO2, gstr);
      pData[playerid][pAttraction]=A_NONE;
      pTemp[playerid][onArena]=ARENA_NONE;
      SpawnPlayer(playerid);
      return false;
    }
  }

  switch(pTemp[playerid][onArena]){
    case ARENA_PRODM,ARENA_ONEDE,ARENA_CS:  {
      if(pData[playerid][averagePing]>100) {
        Msg(playerid,COLOR_ERROR,__("Niestety! Twoj sredni ping przekracza {b}100ms{/b} - nie mozesz dolaczyc do tej areny!"));
        pData[playerid][pAttraction]=A_NONE;
        pTemp[playerid][onArena]=ARENA_NONE;
        return false;
      }
    }
  }

    if (pTemp[playerid][cheater]) {
    pData[playerid][pAttraction]=A_NONE;
    pTemp[playerid][onArena]=ARENA_NONE;
    return false;
  }

  return true;
}

arena_SpawnPlayer(playerid){
  if (Areny[pTemp[playerid][onArena]][ea_minfps]>0 && !pData[playerid][hudSetting][HUD_FPS]) {
    pData[playerid][hudSetting][HUD_FPS]=true;
    ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);
  }
  ResetPlayerWeapons(playerid);
  if (!arena_CheckLimits(playerid,1)) return SpawnPlayer(playerid);

  switch(pTemp[playerid][onArena]) {
    case ARENA_SOLO: {
      return _arenasolo(playerid);
    }
    case ARENA_PUEBLO: {
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerInterior(playerid, 0);
      SetPlayerArmour(playerid,50.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+32);
      GivePlayerWeapon(playerid, 4, 1);   // noz
      GivePlayerWeapon(playerid, 27, 1000);   // combat shotgun

      new r=random(sizeof spawns_arena_pueblo);
      SetPlayerPos(playerid, spawns_arena_pueblo[r][X], spawns_arena_pueblo[r][Y], spawns_arena_pueblo[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_pueblo[r][angle]);

      SetCameraBehindPlayer(playerid);
    }
    case ARENA_PRODM: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerInterior(playerid, 0);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+96);
      GivePlayerWeapon(playerid, 4, 1);
      GivePlayerWeapon(playerid, 25,  3500);  // shotgun
      GivePlayerWeapon(playerid, 24,  3500);  // deagle
      GivePlayerWeapon(playerid, 34,  3500);  // sniper
      new r=random(sizeof spawns_arena_prodm);
      SetPlayerPos(playerid, spawns_arena_prodm[r][X], spawns_arena_prodm[r][Y], spawns_arena_prodm[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_prodm[r][angle]);

      SetCameraBehindPlayer(playerid);

    }
    case ARENA_ONEDE: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,20.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerInterior(playerid, 3);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
      GivePlayerWeapon(playerid, 24,  3500);  // deagle
      new r=random(sizeof spawns_ARENA_ONEDE);
      SetPlayerPos(playerid, spawns_ARENA_ONEDE[r][X], spawns_ARENA_ONEDE[r][Y], spawns_ARENA_ONEDE[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_ARENA_ONEDE[r][angle]);

      SetCameraBehindPlayer(playerid);
    }
    case ARENA_ONEDE2: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,20.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA2);
      SetPlayerInterior(playerid, 3);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
      GivePlayerWeapon(playerid, 24,  3500);  // deagle
      new r=random(sizeof spawns_ARENA_ONEDE);
      SetPlayerPos(playerid, spawns_ARENA_ONEDE[r][X], spawns_ARENA_ONEDE[r][Y], spawns_ARENA_ONEDE[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_ARENA_ONEDE[r][angle]);

      SetCameraBehindPlayer(playerid);
    }
    case ARENA_MINIGUN: {
      new r=random(sizeof spawns_minigun_foot);
      SetPlayerPos(playerid, spawns_minigun_foot[r][0], spawns_minigun_foot[r][1], spawns_minigun_foot[r][2]);
      SetPlayerFacingAngle(playerid, spawns_minigun_foot[r][3]);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
      SetPlayerVirtualWorld(playerid,VW_ARENA2);
      SetPlayerInterior(playerid, 0);
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      GivePlayerWeapon(playerid,38,9999);
    }
    case ARENA_RPG: {
      new r=random(sizeof spawns_rpg_foot);
      SetPlayerPos(playerid, spawns_rpg_foot[r][0], spawns_rpg_foot[r][1], spawns_rpg_foot[r][2]);
      SetPlayerFacingAngle(playerid, spawns_rpg_foot[r][3]+random(5)-2.5);
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA3);
      SetPlayerInterior(playerid, 0);
      GivePlayerWeapon(playerid,35,300);
    }

    case ARENA_RIFLE: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,5.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA3);
      SetPlayerInterior(playerid, 0);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
      GivePlayerWeapon(playerid, 33,  3500);  // 34 strzelba
      new r=random(sizeof spawns_arena_rifle);
      SetPlayerPos(playerid, spawns_arena_rifle[r][X], spawns_arena_rifle[r][Y], spawns_arena_rifle[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_rifle[r][angle]);

      SetCameraBehindPlayer(playerid);
    }

    case ARENA_ONEGUN: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,4.0);
      SetPlayerInterior(playerid, 10);
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
      GivePlayerWeapon(playerid, 25,  1000);  // 25 shotgun
      new r=random(sizeof spawns_arena_onegun);
      SetPlayerPos(playerid, spawns_arena_onegun[r][X], spawns_arena_onegun[r][Y], spawns_arena_onegun[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_onegun[r][angle]);

      SetCameraBehindPlayer(playerid);
    }

    case ARENA_SNIPER: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,20.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerInterior(playerid, 0);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+10);
      GivePlayerWeapon(playerid, 34,  3500);  // sniper
      new r=random(sizeof spawns_arena_sniper);
      SetPlayerPos(playerid, spawns_arena_sniper[r][X], spawns_arena_sniper[r][Y], spawns_arena_sniper[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_sniper[r][angle]);

      SetCameraBehindPlayer(playerid);
    }

    case ARENA_SO: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA2);
      SetPlayerInterior(playerid,9);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+64);
      GivePlayerWeapon(playerid, 26,  3000);  // sawnoff
      SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 999);
      new r=random(sizeof spawns_arena_so);
      SetPlayerPos(playerid, spawns_arena_so[r][X], spawns_arena_so[r][Y], spawns_arena_so[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_so[r][angle]);

      SetCameraBehindPlayer(playerid);
    }
    case ARENA_DM: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerInterior(playerid,0);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+25);
      GivePlayerWeapon(playerid, 30,  60);  // ak47
      new r=random(sizeof spawns_arena_dm);
      SetPlayerPos(playerid, spawns_arena_dm[r][X], spawns_arena_dm[r][Y], spawns_arena_dm[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_dm[r][angle]);
      SetCameraBehindPlayer(playerid);
        GivePlayerWeapon(playerid, 30,  60);  // ak47
    }

    case ARENA_CHAINSAW: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerVirtualWorld(playerid,VW_ARENA);
      SetPlayerInterior(playerid,0);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+36);
      new r=random(sizeof spawns_arena_chain);
      SetPlayerPos(playerid, spawns_arena_chain[r][X], spawns_arena_chain[r][Y], spawns_arena_chain[r][Z]);
      SetPlayerFacingAngle(playerid, spawns_arena_chain[r][angle]);
      SetCameraBehindPlayer(playerid);
      GivePlayerWeapon(playerid, 9, 1); // pila
    }

    case ARENA_CS: {
      SetPlayerArmour(playerid,0.0);
      SetPlayerHealth(playerid,99.0);
      SetPlayerVirtualWorld(playerid,146);
      SetPlayerInterior(playerid,14);
      SetPlayerColor(playerid,pData[playerid][currentColor]*256+46);
      new r=random(sizeof spawns_arena_cs);
      FreezePlayer(playerid,750);
      SetPlayerPos(playerid, spawns_arena_cs[r][X], spawns_arena_cs[r][Y], spawns_arena_cs[r][Z]+2);
      SetPlayerFacingAngle(playerid, spawns_arena_cs[r][angle]);
      Streamer_Update(playerid);
      SetCameraBehindPlayer(playerid);
      GivePlayerWeapon(playerid, 4, 1);   // noz
      GivePlayerWeapon(playerid, 34,  3500);  // sniper
      GivePlayerWeapon(playerid, 24,  3500);  // deagle
      GivePlayerWeapon(playerid, 30, 3500);// m4
      GivePlayerWeapon(playerid, 29,  3000);  // mp5
      GivePlayerWeapon(playerid, 25,  1000);  // 25 shotgun
    }
  }
  Msg(playerid,COLOR_INFO3,"Aby wyjsæ z areny wpisz {b}/ae{/b}");
  SetTimerEx("arena_CheckLimits",2000, false,"dd",playerid,0);
  return 1;
}

arena_join(playerid,arena) {
  pData[playerid][pAttraction] = A_ARENA;
  pTemp[playerid][onArena]=arena;
  Areny[arena][ea_cnt]++;

  pTemp[playerid][ept_fpswarns]=0;

  if (strlen(Areny[arena][ea_cmd])>0  && gmTemp[popularArena]!=arena && Areny[gmTemp[popularArena]][ea_cnt]<Areny[arena][ea_cnt])
    gmTemp[popularArena]=arena;
}

arena_OnPlayerDeath(playerid,killerid,reason){
  switch(pTemp[playerid][onArena]) {
    case ARENA_SOLO:  solo_OnPlayerDeath(playerid,killerid,reason);
    case ARENA_PRODM,ARENA_MINIGUN,ARENA_SO,ARENA_PUEBLO,ARENA_CS: {
        SetPlayerHealth(killerid, 99.0);
    }
  }
  if(killerid!=INVALID_PLAYER_ID && reason==0){
      switch(pTemp[killerid][onArena]) {
        case ARENA_ONEDE,ARENA_RIFLE,ARENA_ONEDE2,ARENA_ONEGUN: {
        Msg(killerid, COLOR_INFO2, __("Zabijanie na tej arenie z u¿yciem piêsci jest zakazane!"));
        JailPlayer(killerid, 2);
        return 1;
      }
    }
  }
  if (!arena_CheckLimits(playerid,1)) return SpawnPlayer(playerid);
  
  //////////////// killstreak processing /////////////////
  if(killerid==INVALID_PLAYER_ID) return 0;
  
  switch(pTemp[killerid][killStreak])
  {
    case 2: {
      GameTextForPlayer(killerid,"~w~>> ~r~DOUBLE KILL ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_DOUBLE);
    }
    case 3: {
      GameTextForPlayer(killerid,"~w~>> ~r~TRIPLE KILL ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_TRIPLE);
    }
    case 4: {
      GameTextForPlayer(killerid,"~w~>> ~r~EXCELLENT ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_EXCELLENT);
    }
    case 5: {
      GameTextForPlayer(killerid,"~w~>> ~r~MULTI KILL ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_MULTTI);
    }
    case 6: {
      GameTextForPlayer(killerid,"~w~>> ~r~KILL SPREE ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_SPREEE);
    }
    case 7: {
      GameTextForPlayer(killerid,"~w~>> ~r~ULTRA KILL ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_ULTRA);
    }
    case 8: {
      GameTextForPlayer(killerid,"~w~>> ~r~MONSTER KILL ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_MONSTER);
    }
    case 9: {
      GameTextForPlayer(killerid,"~w~>> ~r~TERMINATION ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_TERMINATION);
    }
    case 10: {
      GameTextForPlayer(killerid,"~w~>> ~r~DOMINATING ~w~<<",1000,5);
      Audio_Play(killerid,AUDIOID_DOMINATING);
    }
  }
  return 0;
}

CMD:pueblo(playerid){
    MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_PUEBLO);
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_PUEBLO);

  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~PUEBLO",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:chainsaw(playerid){
    MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_CHAINSAW);
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_CHAINSAW);

  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~CHAINSAW",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:pro(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_PRODM);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_PRODM);

  pData[playerid][hudSetting][HUD_FPS]=true;
  ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);

  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~PRO DM",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:onede(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_ONEDE);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_ONEDE);

  pData[playerid][hudSetting][HUD_FPS]=true;
  ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);

  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~ONE DE",1000,1);
  Msg(playerid,COLOR_INFO2,__("Arena {b}ONEDE{/b} - desert eagle, 1 hit = 1 kill, max. ping 100ms"));
  SpawnPlayer(playerid);
  return 1;
}

CMD:onede2(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_ONEDE2);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_ONEDE2);

  pData[playerid][hudSetting][HUD_FPS]=true;
  ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);

  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~ONE DE ~h~2",1000,1);
  Msg(playerid,COLOR_INFO2,__("Arena {b}ONEDE2{/b} - desert eagle, 1 hit = 1 kill, bez GL"));
  SpawnPlayer(playerid);
  return 1;
}

CMD:minigun(playerid)
{
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_MINIGUN);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_MINIGUN);
  GameTextForPlayer(playerid, "~b~ARENA ~h~MINIGUN",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:rpg(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_RPG);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_RPG);
  GameTextForPlayer(playerid, "~r~ARENA ~h~RPG", 1000, 1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:sniper(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_SNIPER);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_SNIPER);
  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~SNIPER",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:rifle(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_RIFLE);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_RIFLE);
  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~rifle",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:onegun(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_ONEGUN);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_ONEGUN);
  GameTextForPlayer(playerid,"~w~ARENA ~n~~r~One~h~gun",1000,1);
  SpawnPlayer(playerid);
  return 1;
}


CMD:so(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_SO);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_SO);
  GameTextForPlayer(playerid,"~w~ARENA ~n~~p~SAWN-OFF",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:dm(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_DM);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_DM);
  GameTextForPlayer(playerid,"~w~ARENA ~n~~p~DM",1000,1);
  SpawnPlayer(playerid);
  return 1;
}

CMD:cs(playerid){
  MUST_BE_SPAWNED(playerid);
  arena_checklimit(ARENA_DM);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid)) {
    RemovePlayerFromVehicle(playerid);
    new Float:PP[3];
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    SetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  }
  arena_join(playerid,ARENA_CS);
  GameTextForPlayer(playerid,"~w~ARENA ~n~~p~CS",1000,1);
  SpawnPlayer(playerid);
  return 1;
}
// EOF
