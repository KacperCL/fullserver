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

enum e_soloinvitation {
  bool:esi_sent,
  esi_targetplayerid,
  esi_arenaid,
  esi_weapon1,
  esi_weapon2,
  esi_when,
  esi_start
}

new soloinv[MAX_PLAYERS][e_soloinvitation];

CMD:solo(playerid,params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz tego zrobic!");
  new targetid;
  // for sscanf
  new target[24], areaid, weap1, weap2;
  if (isnull(params))
    return Msg(playerid,COLOR_ERROR,"/solo {b}<id/nick>{/b}  - lub - /solo {b}<id/nick>{/b} {b}<arena>{/b} {b}<bron 1>{/b} {b}<bron 2>{/b}");

  if (sscanf(params,"s[24]ddd",target, areaid, weap1, weap2))
  { // only ID (show dialog for selecting)
    if (!FindUIDByNameOrID(params,targetid,playerid))
      return 1;

    if (targetid==playerid) return Msg(playerid, COLOR_INFO3, "Fap, fap, fap... {b}Przegrales{/b}");

    if(!pTemp[targetid][accepts][eac_solo])
      return Msg(playerid, COLOR_ERROR, "Wskazany gracz nie akceptuje zaproszen do SOLO.");

    if(!IsPlayerConnected(targetid)) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie jest polaczona");
    if(!TeleportAllowed(targetid)) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie moze teraz stoczyc pojedynku");

    if (soloinv[targetid][esi_targetplayerid]==playerid && soloinv[targetid][esi_sent] ) {  // dwustronnie zaakceptowany pojedynek
      soloinv[playerid][esi_targetplayerid]=targetid;
      solo_startSolo(playerid, targetid, soloinv[targetid][esi_arenaid]);
      return 1;
    }
    // wybor areny

    soloinv[playerid][esi_targetplayerid]=targetid;
    soloinv[playerid][esi_sent]=false;

    return solo_wybierzArene(playerid);
  }
  else
  { // we got all parameters so skip dialog and send invite now!
    if (!FindUIDByNameOrID(target,targetid,playerid))
      return 1;
    
    if (targetid==playerid) return Msg(playerid, COLOR_INFO3, "Fap, fap, fap... {b}Przegrales{/b}");

    if(!pTemp[targetid][accepts][eac_solo])
      return Msg(playerid, COLOR_ERROR, "Wskazany gracz nie akceptuje zaproszen do SOLO.");

    if(!IsPlayerConnected(targetid)) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie jest polaczona");
    if(!TeleportAllowed(targetid)) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie moze teraz stoczyc pojedynku");

    if (soloinv[targetid][esi_targetplayerid]==playerid && soloinv[targetid][esi_sent] ) {  // dwustronnie zaakceptowany pojedynek
      soloinv[playerid][esi_targetplayerid]=targetid;
      solo_startSolo(playerid, targetid, soloinv[targetid][esi_arenaid]);
      return 1;
    }
    
    if(weap1 != 0 && weap1 != 2 && weap1 != 4 && weap1 != 9 && weap1 != 14 && weap1 != 16 && weap1 != 18 && weap1 != 22 && weap1 != 23 && weap1 != 24 && weap1 != 25 && weap1 != 26 && weap1 != 27 && weap1 != 28 && weap1 != 29 && weap1 != 30 && weap1 != 31 && weap1 != 32 && weap1 != 33 && weap1 != 34 && weap1 != 42 && weap1 != 43)
      return Msg(playerid, COLOR_ERROR,"Podano nieprawidłowe lub niedozwolone ID broni (numer 1)");
    if((weap2 != 0 && weap2 != 2 && weap2 != 4 && weap2 != 9 && weap2 != 14 && weap2 != 16 && weap2 != 18 && weap2 != 22 && weap2 != 23 && weap2 != 24 && weap2 != 25 && weap2 != 26 && weap2 != 27 && weap2 != 28 && weap2 != 29 && weap2 != 30 && weap2 != 31 && weap2 != 32 && weap2 != 33 && weap2 != 34 && weap2 != 42 && weap2 != 43) || weap2 == weap1)
      return Msg(playerid, COLOR_ERROR,"Podano nieprawidłowe, niedozwolone lub identyczne co pierwsze ID broni (numer 2)");

    soloinv[playerid][esi_targetplayerid]=targetid;
    soloinv[playerid][esi_arenaid]=areaid;
    soloinv[playerid][esi_weapon1]=weap1;
    soloinv[playerid][esi_weapon2]=weap2;
    soloinv[playerid][esi_sent]=true;
    
    new buf[255];
    format(buf,sizeof buf,"{b}%s{/b} zaprasza Cie do pojedynku: arena {b}%s{/b}, bron: {b}%s, %s{/b}.", GetPlayerProperName(playerid),
      DATA_arenysolo[soloinv[playerid][esi_arenaid]][eas_nazwa],  weaponName[soloinv[playerid][esi_weapon1]], weaponName[soloinv[playerid][esi_weapon2]]
    );

    Msg(soloinv[playerid][esi_targetplayerid],COLOR_INFO,buf);
    format(buf,sizeof buf,"Aby zaakceptowac, wpisz {b}/solo %d{/b}", playerid);
    Msg(soloinv[playerid][esi_targetplayerid],COLOR_INFO,buf ,false);

    Msg(playerid,COLOR_INFO,"Wyslano zaproszenie do pojedynku.");
  }
  return 1;
}

CMD:solooff(playerid)
  return Msg(playerid,COLOR_INFO,"Uzyj komendy {b}/soloacc{/b} aby zarzadzac zezwoleniami na zaproszenia do /solo");

CMD:soloacc(playerid){
  if (pTemp[playerid][accepts][eac_solo]==ACCEPTS_NONE) {
    pTemp[playerid][accepts][eac_solo]=ACCEPTS_ALL;
    Msg(playerid,COLOR_INFO,"{b}Akceptujesz{/b} zaproszenia do pojedynkow solo.");
  } else {
    pTemp[playerid][accepts][eac_solo]=ACCEPTS_NONE;
    Msg(playerid,COLOR_INFO,"{b}Nie akceptujesz{/b} zaproszen do pojedynkow solo.");
  }

  return 1;
}

solo_wybierzArene(playerid)
  return ShowPlayerDialog(playerid, DIALOG_ARENASOLO_SELECT, DIALOG_STYLE_LIST, "Wybierz arene", GetDialogList(playerid, DIALOG_ARENASOLO_SELECT), __("Wybierz"), __("Wyjdz"));

solo_wybranaArena(playerid,arenaid){
  // mamy wybrana arene
  soloinv[playerid][esi_arenaid]=arenaid;
  soloinv[playerid][esi_sent]=true;

  // wybieramy bron1
  return ShowWeaponSelectMenu(playerid, "Wybierz pierwsza bron na solo", "solo_wybranaBron1");

}
forward solo_wybranaBron1(playerid,bron1);
public solo_wybranaBron1(playerid,bron1){
  if (bron1<0)
    return solo_wybierzArene(playerid);
  if (bron1==46) // spadochron
    bron1=0;        // piesci

  soloinv[playerid][esi_weapon1]=bron1;
  return ShowWeaponSelectMenu(playerid, "Wybierz druga bron na solo", "solo_wybranaBron2", weaponSlots[bron1]);
}
forward solo_wybranaBron2(playerid,bron2);
public solo_wybranaBron2(playerid,bron2){
  if (bron2==46) // spadochron
    bron2=0;        // piesci
  soloinv[playerid][esi_weapon2]=bron2;
  // wysylamy zaproszenie do gracza

  if(!TeleportAllowed(soloinv[playerid][esi_targetplayerid])) return Msg(playerid, COLOR_ERROR, "Wskazana osoba nie moze teraz stoczyc pojedynku");

  new buf[255];
  format(buf,sizeof buf,"{b}%s{/b} zaprasza Cie do pojedynku: arena {b}%s{/b}, bron: {b}%s, %s{/b}.", GetPlayerProperName(playerid),
        DATA_arenysolo[soloinv[playerid][esi_arenaid]][eas_nazwa],  weaponName[soloinv[playerid][esi_weapon1]], weaponName[soloinv[playerid][esi_weapon2]]
        );

  Msg(soloinv[playerid][esi_targetplayerid],COLOR_INFO,buf);
  format(buf,sizeof buf,"Aby zaakceptowac, wpisz {b}/solo %d{/b}", playerid);
  Msg(soloinv[playerid][esi_targetplayerid],COLOR_INFO,buf ,false);

  Msg(playerid,COLOR_INFO,"Wyslano zaproszenie do pojedynku.");

  return 1;
}

solo_startSolo(playerid,targetid,arenaid) {
  new buf[128];

  if(!TeleportAllowed(playerid) || !TeleportAllowed(targetid)) {
    Msg(playerid, COLOR_ERROR, "Solo nie moglo sie rozpoczac.");
    Msg(targetid, COLOR_ERROR, "Solo nie moglo sie rozpoczac.");
    return 1;
  }

  soloinv[playerid][esi_weapon1]=soloinv[targetid][esi_weapon1];
  soloinv[playerid][esi_weapon2]=soloinv[targetid][esi_weapon2];
  soloinv[playerid][esi_arenaid]=soloinv[targetid][esi_arenaid];

  soloinv[targetid][esi_sent]=false;

  soloinv[playerid][esi_start]=GetTickCount();
  soloinv[targetid][esi_start]=soloinv[playerid][esi_start];

  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, 999);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, 999);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, 999);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, 999);

  SetPlayerSkillLevel(targetid,WEAPONSKILL_PISTOL, 999);
  SetPlayerSkillLevel(targetid,WEAPONSKILL_PISTOL_SILENCED, 999);
  SetPlayerSkillLevel(targetid,WEAPONSKILL_SAWNOFF_SHOTGUN, 999);
  SetPlayerSkillLevel(targetid,WEAPONSKILL_MICRO_UZI, 999);

  TogglePlayerClock(playerid,0);
  SetPlayerWeather(playerid,0);
  SetPlayerTime(playerid,12,0);
  TogglePlayerClock(targetid,0);
  SetPlayerWeather(targetid,0);
  SetPlayerTime(targetid,12,0);

  Teleport(T_PLAYER, playerid,
      DATA_arenysolo[arenaid][eas_spawn1][0], DATA_arenysolo[arenaid][eas_spawn1][1],DATA_arenysolo[arenaid][eas_spawn1][2], DATA_arenysolo[arenaid][eas_spawn1][3],
      DATA_arenysolo[arenaid][eas_interior], VW_UNIQUE_ARENASOLO+playerid+1, true);

  Teleport(T_PLAYER, targetid,
      DATA_arenysolo[arenaid][eas_spawn2][0], DATA_arenysolo[arenaid][eas_spawn2][1],DATA_arenysolo[arenaid][eas_spawn2][2], DATA_arenysolo[arenaid][eas_spawn2][3],
      DATA_arenysolo[arenaid][eas_interior], VW_UNIQUE_ARENASOLO+playerid+1, true);

  pData[playerid][pAttraction] = A_ARENA;
    pTemp[playerid][onArena]=ARENA_SOLO;

  pData[targetid][pAttraction] = A_ARENA;
    pTemp[targetid][onArena]=ARENA_SOLO;


  if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, false);
  if(pData[playerid][hudSetting][HUD_CLOCK]) ShowElement(playerid, TDE_CLOCK, false);
  if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, false);
  if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, false);


  if(pData[targetid][hudSetting][HUD_DATE]) ShowElement(targetid, TDE_DATETIME, false);
  if(pData[targetid][hudSetting][HUD_CLOCK]) ShowElement(targetid, TDE_CLOCK, false);
  if(pData[targetid][hudSetting][HUD_STATUSBAR]) ShowElement(targetid, TDE_STATS, false);
  if(pData[targetid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(targetid, TDE_ATTRACTIONBOX, false);

    SetPlayerHealth(playerid,99.0);
  SetPlayerArmour(playerid,DATA_arenysolo[arenaid][eas_ar]);
  ResetPlayerWeapons(playerid);

  new ammo1=500, ammo2=500;
  if (soloinv[targetid][esi_weapon1]==0)
    ammo1=0;
  else if (soloinv[targetid][esi_weapon1]<=15 || soloinv[targetid][esi_weapon1]==46)
    ammo1=1;

  if (soloinv[targetid][esi_weapon2]==0)
    ammo2=0;
  else if (soloinv[targetid][esi_weapon2]<=15 || soloinv[targetid][esi_weapon2]==46)
    ammo2=1;

  if (ammo1>0)
    GivePlayerWeapon(playerid,soloinv[targetid][esi_weapon1],ammo1);
  if (ammo2>0)
    GivePlayerWeapon(playerid,soloinv[targetid][esi_weapon2],ammo2);

  SetPlayerHealth(targetid,99.0);
  SetPlayerArmour(targetid,DATA_arenysolo[arenaid][eas_ar]);
  ResetPlayerWeapons(targetid);

  if (ammo1>0)
    GivePlayerWeapon(targetid,soloinv[targetid][esi_weapon1],ammo1);
  if (ammo2>0)
    GivePlayerWeapon(targetid,soloinv[targetid][esi_weapon2],ammo2);

  format(buf, sizeof buf, "Arena solo - %s, {3030ff}%s{ffffff} vs {3030ff}%s{ffffff}. Bronie: {306090}%s, %s",
      DATA_arenysolo[soloinv[targetid][esi_arenaid]][eas_nazwa], GetPlayerProperName(playerid), GetPlayerProperName(targetid),
      weaponName[soloinv[targetid][esi_weapon1]], weaponName[soloinv[targetid][esi_weapon2]]);

  if (gmTemp[pPlayerCount]<=10)
    SendClientMessageToAll(0xff0000ff, buf);
  else
    solo_MSG(0xff0000ff, buf);

  return 1;
}

solo_OnPlayerDeath(playerid,killerid,reason){
  #pragma unused reason

  new buf[255], Float:hp, Float: ar;
  GetPlayerHealth(killerid,hp);
  GetPlayerArmour(killerid,ar);
  new roznica=floatround((GetTickCount()-soloinv[playerid][esi_start])/1000,floatround_floor);

  TogglePlayerClock(playerid,0);
  if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
  if(pData[playerid][hudSetting][HUD_CLOCK]) ShowElement(playerid, TDE_CLOCK, true);
  if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
  if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
  SyncPlayerGameTime(playerid);


  if (killerid!=INVALID_PLAYER_ID) {
    TogglePlayerClock(killerid,0);
    if(pData[killerid][hudSetting][HUD_DATE]) ShowElement(killerid, TDE_DATETIME, true);
    if(pData[killerid][hudSetting][HUD_CLOCK]) ShowElement(killerid, TDE_CLOCK, true);
    if(pData[killerid][hudSetting][HUD_STATUSBAR]) ShowElement(killerid, TDE_STATS, true);
    if(pData[killerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(killerid, TDE_ATTRACTIONBOX, true);
    SyncPlayerGameTime(killerid);

    format(buf, sizeof buf, "%s {eeeeee}wygrywa solo z {6060d0}%s{eeeeee}, arena %s.",  GetPlayerProperName(killerid), GetPlayerProperName(playerid),
        DATA_arenysolo[soloinv[killerid][esi_arenaid]][eas_nazwa]
        );

    if (gmTemp[pPlayerCount]<=10)
      SendClientMessageToAll(0x6060d0ff, buf);
    else
      solo_MSG(0x6060d0ff, buf);

    format(buf, sizeof buf, "Bronie: %s, %s, zwyciezcy zostalo %03.0fHP, %03.0fAR, czas: {ffb648}%02dm%02ds{eeeeee}.",// zwyciezcy zostalo {ffb648}%fHP{eeeeee}.",
      weaponName[soloinv[killerid][esi_weapon1]], weaponName[soloinv[killerid][esi_weapon2]],
      hp, ar,
      floatround(roznica/60, floatround_floor), roznica%60);

    if (gmTemp[pPlayerCount]<=10)
      SendClientMessageToAll(0x6060d0ff,buf);
    else
      solo_MSG(0x6060d0ff,buf);

    pData[killerid][pAttraction] = A_NONE;
      pTemp[killerid][onArena]=ARENA_NONE;
    SetPlayerHealth(killerid,100);
    OnPlayerSpawn(killerid);

    }
  pData[playerid][pAttraction] = A_NONE;
    pTemp[playerid][onArena]=ARENA_NONE;

    SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, pTemp[playerid][weaponSkill_pistol]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, pTemp[playerid][weaponSkill_silenced]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, pTemp[playerid][weaponSkill_sawnoff]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, pTemp[playerid][weaponSkill_uzi]);

  SetPlayerSkillLevel(killerid,WEAPONSKILL_PISTOL, pTemp[playerid][weaponSkill_pistol]);
  SetPlayerSkillLevel(killerid,WEAPONSKILL_PISTOL_SILENCED, pTemp[playerid][weaponSkill_silenced]);
  SetPlayerSkillLevel(killerid,WEAPONSKILL_SAWNOFF_SHOTGUN, pTemp[playerid][weaponSkill_sawnoff]);
  SetPlayerSkillLevel(killerid,WEAPONSKILL_MICRO_UZI, pTemp[playerid][weaponSkill_uzi]);

  if (killerid>=0 && killerid!=INVALID_PLAYER_ID && IsPlayerConnected(killerid)) {
    format(buf, sizeof buf, "INSERT INTO fs_solo_matches SET arenaid=%d,weapon1=%d,weapon2=%d,playerid=%d,killerid=%d,fightlen=%d,hp=%.0f,ar=%.0f,ts=NOW();",
      DATA_arenysolo[soloinv[playerid][esi_arenaid]][eas_code], soloinv[playerid][esi_weapon1], soloinv[playerid][esi_weapon2],
      pData[playerid][accountID], pData[killerid][accountID], roznica, hp, ar);
    mysql_query(buf);
  }
  return 1;
}

solo_MSG(color, txt[]){
  foreach(i)
    if ((pData[i][pAttraction]==A_ARENA && pTemp[i][onArena]==ARENA_SOLO) || (GetPlayerVirtualWorld(i)==VW_UNIQUE_ARENASOLO))
      SendClientMessage(i, color, txt);
  return 1;
}
// EOF
