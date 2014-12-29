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

forward vip_vbronCB(playerid,bron);

CMD:vsay(playerid, params[])
{
  if(!pData[playerid][vipEnabled]) return Msg(playerid, COLOR_ERROR, __("Musisz posiadac konto VIP aby uzywac tej komendy."));
  if(isPlayerMuted(playerid))
    return Msg(playerid,COLOR_ERROR,"Jesteœ wyciszony - nie mo¿esz pisaæ na czacie.");
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, __("Uzyj: {b}/vsay <tekst>{/b}")); // U¿ywaj: /vsay <tekst>
  if (gmTemp[chatDisabled]) return Msg(playerid, COLOR_ERROR, "Niestety, czat jest obecnie wylaczony przez administratora");

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  new
   buffer[160];

  format(buffer, sizeof buffer, "{FCC000}VIP {%06x}%s {FCC000}%s", gmData[color_vipSay], GetPlayerProperName(playerid), params);

  if (pTemp[playerid][troll]) {
    Msg(playerid, COLOR_VIPSAY, buffer);

    if(gmTemp[showTroll]){
      format(buffer, sizeof buffer, "TROLL: %s", buffer);
      MSGToAdmins(COLOR_ADMIN, buffer, false, LEVEL_ADMIN2);
    }

    format(buffer, sizeof buffer, "[VIPSAY-TROLL] %s %s", GetPlayerNick(playerid), params);
    OutputLog(LOG_OCHAT, buffer);
    return 1;
  }

  foreach(i){
    if(pData[i][logonDialog] || pData[i][classSelecting]) continue;
    Msg(i, COLOR_VIPSAY, buffer);
  }

  format(buffer, sizeof buffer, "[VIPSAY] %s %s", GetPlayerNick(playerid), params);
  OutputLog(LOG_OCHAT, buffer);

  return 1;
}

CMD:v(playerid, params[])
{
  if(!pData[playerid][vipEnabled]) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, __("Uzyj: {b}/v <tekst>{/b}")); // U¿ywaj: /v <tekst>

  if(isPlayerMuted(playerid))
    return Msg(playerid,COLOR_ERROR,"Jesteœ wyciszony - nie mo¿esz pisaæ na czacie.");
  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  OutputVipChat(playerid, params);

  return 1;
}

CMD:varmor(playerid,params[])
{
  if(!pData[playerid][vipEnabled]) return Msg(playerid, COLOR_ERROR, __("Musisz posiadac konto VIP aby uzywac tej komendy."));
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  new tplayer[MAX_PLAYER_NAME],uid,buf[128];
  if (!sscanf(params,"s[24]",tplayer)){
    if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  } else
    uid=playerid;

  if(!IsPlayerSpawned(uid) || !AttractionAllowed(uid)) {
    format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
    return Msg(playerid,COLOR_ERROR,buf);
  }
  if(uid==gmData[artefactOwner]) {
      format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
    return Msg(playerid,COLOR_ERROR,buf);
  }

  if (uid==playerid) {
      SetPlayerArmour(playerid,100.0);
    ShowPlayerArmourIcon(uid,playerid);

    Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Uzupelnilas sobie kamizelke."):("Uzupelniles sobie kamizelke."));
    return 1;
  }

  format(buf,sizeof buf,"Wyslal%ss propozycje uzupe³nienia kamizeki do gracza %s (%d)",Kobieta(playerid)?("a"):("e"),GetPlayerProperName(uid),uid);
  Msg(playerid,COLOR_VIP,buf);

  format(buf,sizeof buf,"VIP %s (%d) chce uzupe³niæ Ci kamizlkê! Wpisz {b}/vatak{/b} aby potwierdzic.",GetPlayerProperName(playerid),playerid);
  Msg(uid,COLOR_VIP,buf);

  pTemp[uid][vipArmor]=playerid;
  return 1;
}

CMD:vatak(playerid)
{
  if(pTemp[playerid][vipArmor]==-1) return Msg(playerid,COLOR_ERROR,"Nie masz zadnych propozycji uleczenia");
  if(pData[playerid][pAttraction]!=A_NONE || pTemp[playerid][godMode]) return Msg(playerid,COLOR_ERROR,"Nie mozesz tego teraz zrobic");

    SetPlayerArmour(playerid,100.0);
  ShowPlayerArmourIcon(playerid,playerid);

  new buf[128];

    format(buf, sizeof buf, "VIP {b}%s{/b}(%d) uzupe³ni³%s Twoja kamizlke", GetPlayerProperName(pTemp[playerid][vipArmor]), pTemp[playerid][vipArmor], SkinKobiecy(GetPlayerSkin(pTemp[playerid][vipArmor])) ? ("a") : (""));
  Msg(playerid,COLOR_VIP,buf);
  GameTextForPlayer(playerid,"~g~AR~w~+", 250,6);
  format(buf, sizeof buf, "Uzupelnil%ss kamizlkê graczowi {b}%s{/b}.", SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : ("e"), GetPlayerProperName(playerid));
  Msg(pTemp[playerid][vipArmor],COLOR_INFO, buf, false);

  pTemp[playerid][vipArmor]=-1;

  return 1;
}

CMD:vheal(playerid,params[])
{

  if(!pData[playerid][vipEnabled]) return Msg(playerid, COLOR_ERROR, __("Musisz posiadac konto VIP aby uzywac tej komendy."));
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  new tplayer[MAX_PLAYER_NAME],uid,buf[128];
  if (!sscanf(params,"s[24]",tplayer)){
    if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  } else
    uid=playerid;

  if(!IsPlayerSpawned(uid) || !AttractionAllowed(uid)) {
    format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
    return Msg(playerid,COLOR_ERROR,buf);
  }
  if(uid==gmData[artefactOwner]) {
      format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
    return Msg(playerid,COLOR_ERROR,buf);
  }

  if (uid==playerid) {
      SetPlayerHealth(uid,100.0);
    ShowPlayerHealIcon(uid,playerid);

    if (random(2)==1)
      Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Uleczylas sie."):("Uleczyles sie."));
    else
      Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Uzupelnilas sobie punkty HP."):("Uzupelniles sobie punkty HP."));
    return 1;
  }

  format(buf,sizeof buf,"Wyslal%ss propozycje uleczenia do gracza %s (%d)",Kobieta(playerid)?("a"):("e"),GetPlayerProperName(uid),uid);
  Msg(playerid,COLOR_VIP,buf);

  format(buf,sizeof buf,"VIP %s (%d) chce Cie uleczyc! Wpisz {b}/vhtak{/b} aby potwierdzic.",GetPlayerProperName(playerid),playerid);
  Msg(uid,COLOR_VIP,buf);

  pTemp[uid][vipHeal]=playerid;
  return 1;
}

CMD:vhtak(playerid)
{
  if(pTemp[playerid][vipHeal]==-1) return Msg(playerid,COLOR_ERROR,"Nie masz zadnych propozycji uleczenia");
  if(pData[playerid][pAttraction]!=A_NONE || pTemp[playerid][godMode]) return Msg(playerid,COLOR_ERROR,"Nie mozesz tego teraz zrobic");

    SetPlayerHealth(playerid,100.0);
  ShowPlayerHealIcon(playerid,playerid);

  new buf[128];

    format(buf, sizeof buf, "VIP {b}%s{/b}(%d) uzupelnil%s Twoj poziom HP.", GetPlayerProperName(pTemp[playerid][vipHeal]), pTemp[playerid][vipHeal], SkinKobiecy(GetPlayerSkin(pTemp[playerid][vipHeal])) ? ("a") : (""));
  Msg(playerid,COLOR_VIP,buf);
  GameTextForPlayer(playerid,"~g~HP~w~+", 250,6);
  format(buf, sizeof buf, "Uleczyl%ss gracza {b}%s{/b}.", SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : ("e"), GetPlayerProperName(playerid));
  Msg(pTemp[playerid][vipHeal],COLOR_INFO, buf, false);

  pTemp[playerid][vipHeal]=-1;

  return 1;
}

CMD:vnapraw(playerid,params[])
{

  if(!pData[playerid][vipEnabled]) return Msg(playerid, COLOR_ERROR, __("Musisz posiadac konto VIP aby uzywac tej komendy."));
  new tplayer[MAX_PLAYER_NAME],uid,buf[128];
  if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  } else
    uid=playerid;

  if(!IsPlayerSpawned(uid) || !AttractionAllowed(uid)) {
    format(buf,sizeof buf, "{b}%s{/b} jest obecnie nieosiagaln%s.", GetPlayerProperName(uid), SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("y") );
    return Msg(playerid,COLOR_ERROR,buf);
  }
  if (!IsPlayerInAnyVehicle(uid)) {
    format(buf,sizeof buf, "{b}%s{/b} nie jest obecnie w zadnym pojezdzie.", GetPlayerProperName(uid));
    return Msg(playerid,COLOR_ERROR, buf);
  }

  new vid=GetPlayerVehicleID(uid);

  if (GetTickCount()-tVehicles[vid][vo_lastRepair]<VEHICLE_REPAIR_INTERVAL) {
    format(gstr,sizeof gstr,"Musisz odczekac {b}%ds{/b} przed kolejnym uzyciem /vnapraw", (VEHICLE_REPAIR_INTERVAL-(GetTickCount()-tVehicles[vid][vo_lastRepair]))/1000 );
    return Msg(playerid,COLOR_ERROR,gstr);
  }

  RepairVehicle(vid);
  SetVehicleHealth(vid, VEHICLE_DEFAULT_HP);
  tVehicles[vid][vo_lastRepair]=GetTickCount();

  if (uid==playerid)
    return  Msg(playerid,COLOR_VIP,Kobieta(playerid)?("Naprawilas swoj pojazd."):("Naprawiles swoj pojazd."));

  format(buf, sizeof buf, "VIP {b}%s{/b}(%d) naprawil%s Ci pojazd", GetPlayerProperName(playerid), playerid, SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : (""));
  Msg(uid,COLOR_VIP,buf);
  GameTextForPlayer(uid,"~y~VRP~w~+", 250,6);
  format(buf, sizeof buf, "Naprawil%ss pojazd gracza {b}%s{/b}.", SkinKobiecy(GetPlayerSkin(uid)) ? ("a") : ("e"), GetPlayerProperName(uid));
  Msg(playerid,COLOR_INFO, buf, false);
  return 1;
}

CMD:vpomoc(playerid)
{
  OnDialogResponse(playerid, DIALOG_HELP_CMD, BUTTON_NEXT, f_h__VIP, "");
  return 1;
}

CMD:vtt(playerid,params[]){
  if(!pData[playerid][vipEnabled] && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, __("Musisz posiadac konto VIP aby uzywac tej komendy."));
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz tego zrobic!");

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/vtt <id/nick>{/b}");

  new
   targetplayerid = FindPlayer(params),
   buffer[128];

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if(targetplayerid == playerid) return 1;
  new tpstate=GetPlayerState(targetplayerid);
  if (tpstate==PLAYER_STATE_NONE || tpstate==PLAYER_STATE_WASTED || tpstate==PLAYER_STATE_SPECTATING)
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest obecnie zespawnowany");

  pTemp[targetplayerid][TPInv]=playerid+1;

  format(buffer, sizeof buffer, "%s(%d) chce sie przeniesc {b}do Ciebie{/b}. Wpisz {b}/vtak{/b} aby wyrazic zgode.", GetPlayerProperName(playerid), playerid);
  Msg(targetplayerid, COLOR_VIPSAY, buffer);

    if (!TeleportAllowed(targetplayerid)) return Msg(playerid,COLOR_INFO2, "Nie mozesz sie teraz teleportowac do tego gracza, zostalo do niego wyslane zaproszenie z ktorego bedzie mogl skorzystac pozniej", false);
  else Msg(playerid, COLOR_INFO2, "Wyslano zaproszenie do teleportacji.");

  GameTextForPlayer(targetplayerid,"~g~TP", 500,6);

  return 1;
}

CMD:vth(playerid,params[]){
  if(!pData[playerid][vipEnabled] && !IsGM(playerid)) return Msg(playerid, COLOR_ERROR, __("Musisz posiadac konto VIP aby uzywac tej komendy."));
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz tego zrobic!");

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/vtt <id/nick>{/b}");

  new
   targetplayerid = FindPlayer(params),
   buffer[128];

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if(targetplayerid == playerid) return 1;
  new tpstate=GetPlayerState(targetplayerid);
  if (tpstate==PLAYER_STATE_NONE || tpstate==PLAYER_STATE_WASTED || tpstate==PLAYER_STATE_SPECTATING)
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest obecnie zespawnowany");


  pTemp[targetplayerid][TPInv]=-1 * (playerid+1);

  format(buffer, sizeof buffer, "(VIP) %s(%d) chce Cie przeniesc {b}do siebie{/b}. Wpisz {b}/vtak{/b} aby wyrazic zgode.", GetPlayerProperName(playerid), playerid);
  Msg(targetplayerid, COLOR_VIPSAY, buffer);

  if (!TeleportAllowed(targetplayerid)) return Msg(playerid,COLOR_INFO2, "Wskazany gracz nie moze sie teraz teleportowac, zostalo do niego wyslane zaproszenie z ktorego bedzie mogl skorzystac pozniej", false);
  else Msg(playerid, COLOR_INFO2, "Wyslano zaproszenie do teleportacji.", false);

  GameTextForPlayer(targetplayerid,"~g~TP", 500,6);

  return 1;

}

CMD:vtak(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid))
    return Msg(playerid, COLOR_ERROR, "Nie mozesz teraz sie nigdzie przeniesc.");
  new buf[128];

  if (pTemp[playerid][TPInv]==0)
    return Msg(playerid,COLOR_ERROR,"Nie masz zadnych aktywnych zaproszen do teleportacji.");
  new uid,bool:ktt;
  uid=pTemp[playerid][TPInv];
  if (uid<0)
    uid*=-1;
  else
    ktt=true;

  uid--;

  if (!IsPlayerConnected(uid))
    return Msg(playerid,COLOR_ERROR,"Nie masz zadnych aktywnych zaproszen do teleportacji.");
  if (!TeleportAllowed(uid))
    return Msg(playerid,COLOR_ERROR,"Niestety, nie mozesz sie obecnie przeniesc do tego gracza.");
  format(buf, sizeof buf, "{b}%s(%d){/b} zgodzil%s sie na teleportacje", GetPlayerProperName(playerid), playerid, SkinKobiecy(GetPlayerSkin(playerid)) ? ("a") : (""));
  Msg(uid, COLOR_INFO, buf);

  new Float:pP[4];
  if (ktt) {
    GetPlayerPos(playerid,pP[0], pP[1], pP[2]);
    SetPlayerPos(uid, pP[0], pP[1], pP[2]);
    SetPlayerInterior(uid, GetPlayerInterior(playerid));
    SetPlayerVirtualWorld(uid, GetPlayerVirtualWorld(playerid));
  } else {
    GetPlayerPos(uid,pP[0], pP[1], pP[2]);
    SetPlayerPos(playerid, pP[0], pP[1], pP[2]);
    SetPlayerInterior(playerid, GetPlayerInterior(uid));
    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(uid));
  }
  pTemp[playerid][TPInv]=0;
  return 1;
}

CMD:vpozostalo(playerid){
  new buffer[127];
  if (!pData[playerid][vipEnabled])
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");
  format(buffer, sizeof buffer, "Twoje konto {b}VIP{/b} jest aktywne do {b}%s{/b} (jeszcze przez {b}%d %s{/b})", pData[playerid][vipToDate],
      pData[playerid][vipDaysLeft], dli(pData[playerid][vipDaysLeft], "dzien", "dni", "dni"));
  return Msg(playerid,COLOR_VIP, buffer);
}

CMD:vbron(playerid,params[]) {
  if (!pData[playerid][vipEnabled])
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");

  if (pData[playerid][pAttraction]!=A_NONE || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic!");
  new wepid;
    if (!sscanf(params,"d",wepid))
  {
    if (wepid<0 || wepid>46 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || (wepid>=35 && wepid<42) || wepid==44 || wepid==45)
      return Msg(playerid,COLOR_ERROR,"Bledne ID broni");

        new wi;
    for (new i=0; i<sizeof quickbuyWeapons;i++)
      if (quickbuyWeapons[i][ewd_id]==wepid) {  wi=i; break; }

    RemovePlayerWeapon(playerid,quickbuyWeapons[wi][ewd_id]);
        GivePlayerWeapon(playerid,quickbuyWeapons[wi][ewd_id],quickbuyWeapons[wi][ewd_baseAmmo]);

      new vid=GetPlayerVehicleID(playerid);
    if (GetVehicleModel(vid)==594)    // doniczka
      SetPlayerArmedWeapon(playerid,0);
    if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

    return 1;
  }

  return ShowWeaponSelectMenu(playerid, "Wybierz bron", "vip_vbronCB");
}

public vip_vbronCB(playerid,bron){
  if (pData[playerid][pAttraction]!=A_NONE || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz zakupic broni!");

  new wi;
  for (new i=0; i<sizeof quickbuyWeapons;i++)
    if (quickbuyWeapons[i][ewd_id]==bron) { wi=i; break; }

  GivePlayerWeapon(playerid,quickbuyWeapons[wi][ewd_id],quickbuyWeapons[wi][ewd_baseAmmo]);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  return 1;
}

CMD:vzestaw(playerid){
  if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");
  if (pData[playerid][pAttraction]!=A_NONE || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic!");



  GivePlayerWeapon(playerid, 24, 100);
  GivePlayerWeapon(playerid, 31, 100);
  GivePlayerWeapon(playerid, 34, 100);
  GivePlayerWeapon(playerid, 9, 1);
  SetPlayerAmmo(playerid,9,1);
  GivePlayerWeapon(playerid, 28, 100);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);


  return Msg(playerid,COLOR_VIP,"Otrzymujesz {b}zestaw broni VIPa{/b}");
}

CMD:vinvisible(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");
  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  pTemp[playerid][playerColorAlpha]=0;
  pTemp[playerid][tmpAlpha]=0;
  SetPlayerProperColor(playerid);

  Msg(playerid,COLOR_INFO,__("Dosta³eœ {b}niewidzialnosc{/b}."));
  return 1;
}

CMD:vdualso(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");

  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 999);
  pTemp[playerid][weaponSkill_sawnoff]=999;

  GivePlayerWeapon(playerid, 26, 2000);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  Msg(playerid,COLOR_INFO,__("Dosta³eœ {b}mozliwosc strzelania oburacz z dualso{/b}."));
  return 1;
}

CMD:vdualuzi(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");

  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 999);
  pTemp[playerid][weaponSkill_uzi]=999;

  GivePlayerWeapon(playerid,32, 2000);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  Msg(playerid,COLOR_INFO,__("Dosta³eœ {b}mozliwosc strzelania oburacz z UZI{/b}."));
  return 1;
}

CMD:vdualpistols(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");

  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL_SILENCED, 999);
  SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 999);
  pTemp[playerid][weaponSkill_silenced]=999;
  pTemp[playerid][weaponSkill_pistol]=999;

  GivePlayerWeapon(playerid,22+random(1),1000);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  Msg(playerid,COLOR_INFO,__("Dosta³eœ {b}mozliwosc strzelania oburacz z pistoletow{/b}."));
  return 1;
}

CMD:vtune(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][vipEnabled])
        return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");

  if (!IsPlayerInAnyVehicle(playerid))
    return Msg(playerid,COLOR_ERROR,__("Musisz byc w pojezdzie"));

  if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz uzyc {b}/tune{/b} bedac pasazerem"));

    if (pData[playerid][pAttraction]!=A_NONE &&  pData[playerid][pAttraction]!=A_ARENA)
        return Msg(playerid,COLOR_ERROR,__("Nie mozesz tego teraz zrobic!"));

  if (TuneCar(GetPlayerVehicleID(playerid))) {
      PlaySound(playerid, 1134);
    Msg(playerid, COLOR_INFO, __("Twoj pojazd zostal ztuningowany!"), false);
  } else
    Msg(playerid, COLOR_ERROR, __("Tego pojazdu nie da sie stuningowac!"));

  return 1;
}
// EOF
