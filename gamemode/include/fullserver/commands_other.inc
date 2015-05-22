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
 
CMD:kolorauto(playerid){
    if (!IsPlayerInAnyVehicle(playerid))
        return Msg(playerid,COLOR_ERROR,__("Musisz byc w pojeŸdzie"));

    if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
        return Msg(playerid,COLOR_ERROR,__("Nie mozesz uzyc {b}/tune{/b} bedac pasazerem"));


    if (GetPlayerMoney(playerid)<500)
        return Msg(playerid,COLOR_ERROR,__("Nie stac Cie na to! Automatyczne malowanie kosztuje {b}500${/b}"));
    GivePlayerMoney(playerid, -500);
    new vid=GetPlayerVehicleID(playerid);

    tVehicles[vid][vo_color][0]=random(50)-1;
    tVehicles[vid][vo_color][1]=random(50)-1;
    ChangeVehicleColor(vid, tVehicles[vid][vo_color][0], tVehicles[vid][vo_color][1]);
    PlaySound(playerid, 1134);
    return 1;
}

CMD:furia(playerid)
{
  if(!checkDoubleMode(playerid)) Msg(playerid,COLOR_INFO3,"[INFORMACJA] Tryb furii nie jest obecnie aktywny");
  return 1;
}

CMD:cpanel(playerid) // thx to pr0f from PBS for this command
{
    MUST_BE_SPAWNED(playerid);

  if (pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid,COLOR_ERROR,TXT(playerid,454)); // nie mozesz tego teraz zrobic

    if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return Msg(playerid, COLOR_ERROR, "Aby uzywac tej komendy musisz byc kierowca pojazdu");

  new vid=GetPlayerVehicleID(playerid),sstr[256];
  tVehicles[vid][vo_occupied]=true;

  if (vid>0 && vid!=INVALID_VEHICLE_ID) {
      static const K[][] = {"{FF0000}OFF","{24FF0A}ON","{FF0000}CLOSED","{24FF0A}OPENED"};
    new engine, lights, alarm, doors, bonnet, boot, objective;

    GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);

    if(engine == VEHICLE_PARAMS_UNSET) engine = VEHICLE_PARAMS_ON;
    if(lights == VEHICLE_PARAMS_UNSET) lights = VEHICLE_PARAMS_ON;
    if(alarm == VEHICLE_PARAMS_UNSET) alarm = VEHICLE_PARAMS_OFF;
    if(doors == VEHICLE_PARAMS_UNSET) doors = DOOR_OPENED;
    if(bonnet == VEHICLE_PARAMS_UNSET) bonnet = VEHICLE_PARAMS_OFF;
    if(boot == VEHICLE_PARAMS_UNSET) boot = VEHICLE_PARAMS_OFF;

    SetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);

    tVehicleUsed[vid]=true;
    tVehicles[vid][vo_used]=true;

    format(sstr,sizeof sstr,"Opcja\tStatus\nSilnik:\t%s\nŒwiat³a:\t%s\nAlarm:\t%s\nDrzwi:\t%s\nMaska:\t%s\nBaga¿nik:\t%s",K[engine],K[lights],K[alarm],K[3-doors],K[bonnet+2],K[boot+2]);
    ShowPlayerDialog(playerid,DIALOG_VEH_PANEL,DIALOG_STYLE_TABLIST_HEADERS,"Zarz¹dzanie pojazdem",sstr,"Prze³¹cz","Zamknij");
  }
  else return Msg(playerid,COLOR_INFO,TXT(playerid,451)); // musisz byc w pojezdzie aby to zrobic
  return 1;
}

CMD:nametags(playerid)
{
    if (pData[playerid][pAttraction]==A_CHOWANY) return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
    if (pTemp[playerid][seesNametags]) {
        for(new i = 0; i < MAX_PLAYERS; i++)
            ShowPlayerNameTagForPlayer(playerid, i, false);
        Msg(playerid, COLOR_INFO,__("Wylaczono pokazywanie nametagow"));
        pTemp[playerid][seesNametags]=false;
    } else {
        for(new i = 0; i < MAX_PLAYERS; i++)
            ShowPlayerNameTagForPlayer(playerid, i, true);
        Msg(playerid, COLOR_INFO,__("Wlaczono pokazywanie nametagow"));
        pTemp[playerid][seesNametags]=true;
    }
    return 1;
}

CMD:bankomat(playerid) // prawie deprecated, bo samo sie wlacza po wejsciu w CP przy bankomacie.
{
    MUST_BE_SPAWNED(playerid);
    if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 110)); // Komenda dostêpna tylko dla zarejestrowanych u¿ytkowników.
    if(!IsCloseToATM(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 109)); // Operacje bankowe mo¿na wykonywaæ tylko przy bankomacie.

    ShowPlayerDialog(playerid, DIALOG_BANK_MAIN, DIALOG_STYLE_LIST, "Bank", GetDialogList(playerid, DIALOG_BANK_MAIN), __("Wybierz"), __("Wyjdz"));

    return 1;
}

CMD:gp(playerid)
{
    if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, "Nie masz jescze konta na serwerze! Aby skorzystac ze tej funkcji serwera utworz je! {b}/rej <haslo>{/b}");
    ShowPlayerDialog(playerid, DIALOG_EXCHANGE_GP, DIALOG_STYLE_LIST, "Centrum wymiany GamePoints", "Co to sa Game Points\n\t\nDoladuj GamePoints SMSem\nDoladuj GamePoints kasa z gry\n\t\nOplac swoje konto VIP\nOplac domek\nWykup szybsza zmiane nicku\nTryb furii\nTryb pasywny", "Wybierz", "Anuluj");
  return 1;
}

CMD:raudio(playerid){
  Msg(playerid,COLOR_INFO,"Streaming audio zosta³ {b}zresetowany{/b} i {b}zatrzymany{/b}");
  Audio_TransferPack(playerid);
  return 1;
}

CMD:spawn(playerid){
  new dialog[386];
  format(dialog,sizeof dialog,"Los Santos\nLas Venturas\nSan Fierro\nFull Andreas%s%s",isPlayerHaveHouse(playerid)?("\nDomek"):(""),isPlayerInGang(playerid)?("\nWyspa gangu\nSpawn gangu"):(""));
  ShowPlayerDialog(playerid, DIALOG_CHANGE_SPAWN, DIALOG_STYLE_LIST, "Wybierz spawn", dialog, "Wybierz", "Anuluj");
  return 1;
}

CMD:bs(playerid){
    MUST_BE_SPAWNED(playerid);
  if (pData[playerid][pAttraction]!=A_NONE || IsPlayerInAnyVehicle(playerid) || GetPlayerInterior(playerid)>0 || GetPlayerVirtualWorld(playerid)>0 || pTemp[playerid][godMode]) return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
  if(pData[playerid][respect]<40000) return Msg(playerid,COLOR_ERROR,"Korzystanie z broni specjalnych wymaga minimum 40k respektu!");
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  new dialog[128];
  if(pData[playerid][respect]>=75000) format(dialog,sizeof dialog,"Granaty\nKoktajle Molotova\nBrak");
    else format(dialog,sizeof dialog,"Koktajle Molotova\nBrak");
  ShowPlayerDialog(playerid, DIALOG_CHANGE_WEAPON, DIALOG_STYLE_LIST, "Wybierz bron specjalna", dialog, "Wybierz", "Anuluj");
  return 1;
}

CMD:odleglosc(playerid,params[]){
  new user;
  if (sscanf(params,"u",user))
    return Msg(playerid,COLOR_ERROR,"/odleglosc <id gracza>");
    if (user==playerid)
    return Msg(playerid,COLOR_ERROR,"/odleglosc <id gracza>");
    if (!IsPlayerConnected(user) || !IsPlayerSpawned(user))
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest polaczony/zespawnowany");
  new Float:FP[3];
  GetPlayerPos(user,FP[0],FP[1],FP[2]);
  new Float:dist=GetPlayerDistanceFromPoint(playerid,FP[0],FP[1],FP[2]);
  new buf[128];
  format(buf, sizeof buf,"Odleglosc do gracza {b}%s{/b}: {b}%.1f{/b}m", GetPlayerNick(user),dist);
  return Msg(playerid,COLOR_INFO, buf);
}

CMD:wyrzuc(playerid,params[]){
    if(isnull(params)) return Msg(playerid, COLOR_ERROR, "U¿ywaj: /wyrzuc <ID> (Gracz musi byc pasazerem w Twoim pojezdzie!)");
  if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return Msg(playerid, COLOR_ERROR, "Aby uzywac tej komendy musisz byc kierowca pojazdu");
    new gracz = strval(params);
  if(!IsNumeric(params) || !IsPlayerConnected(playerid) || GetPlayerVehicleID(playerid)!=GetPlayerVehicleID(gracz)) return Msg(playerid, COLOR_ERROR, "Taki gracz nie istnieje lub nie jest pasazerem w Twoim pojezdzie");
  RemovePlayerFromVehicle(gracz);
  _rsp(gracz,"");
  return 1;
}

CMD:rcolor(playerid)
{
    MUST_BE_SPAWNED(playerid);
  if (pData[playerid][pAttraction]!=A_NONE || gmData[artefactOwner]==playerid) return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
    SetPlayerProperColor(playerid,true);
  Msg(playerid,COLOR_INFO,"Twoj kolor zostal zrestartowany");
  return 1;
}

CMD:dajbronie(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if (pData[playerid][pAttraction]!=A_NONE || IsPlayerInAnyVehicle(playerid) || GetPlayerInterior(playerid)>0 || GetPlayerVirtualWorld(playerid)>0 || pTemp[playerid][godMode]) return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
  ResetPlayerWeapons(playerid);
    GivePlayerNormalWeapons(playerid,false);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL, pTemp[playerid][weaponSkill_pistol]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED, pTemp[playerid][weaponSkill_silenced]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN, pTemp[playerid][weaponSkill_sawnoff]);
  SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI, pTemp[playerid][weaponSkill_uzi]);

  Msg(playerid,COLOR_INFO,"Przywrociles sobie zwykle bronie");
  return 1;
}

CMD:uptime(playerid){
  new tt=floatround(float(GetTickCount()-gmTemp[startTime])/1000);
  format(gstr, sizeof gstr, "Serwer wlaczony od: {b}%d{/b} %s {b}%d{/b} %s",
    tt/3600, dli(tt/3600,"godziny","godzin","godzin"),
    tt%3600/60, dli(tt%3600/60,"minuty","minut","minut"));
  Msg(playerid,COLOR_INFO,gstr);
  return 1;
}

CMD:osiagniecia(playerid){
    if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, "Nie masz jescze konta na serwerze! Aby skorzystac ze tej funkcji serwera utworz je! {b}/rej <haslo>{/b}");
  new buffer[256],nazwao[36],score[8],dlg_buffer[1500];
  format(buffer,sizeof buffer,"SELECT a.name,pa.score FROM fs_players_achievements pa JOIN fs_achievements a ON a.shortname=pa.shortname WHERE pa.playerid=%d",pData[playerid][accountID]);
  mysql_query(buffer);
  mysql_store_result();
  while(mysql_retrieve_row()){
        mysql_get_field("name", nazwao);
        mysql_get_field("score", score);

        format(buffer,sizeof buffer,"Nazwa\tPoziom\n{FFFF00}%36s\t{FF0000}%8s\n",nazwao,score);
        strcat(dlg_buffer, buffer);
  }
  strdel(dlg_buffer, strlen(dlg_buffer) - 1, strlen(dlg_buffer));
  mysql_free_result();

  if(strlen(dlg_buffer)>1)
    ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_TABLIST_HEADERS, "Osiagniecia", dlg_buffer, "Zamknij", "");
  else
      Msg(playerid,COLOR_ERROR,"Nie masz jescze zadnych osiagniec, kod b³êdu: #MYSQL_NO_ACH_OR");
  return 1;
}

CMD:artefakt(playerid)
{
  if(gmData[artefactOwner]!=INVALID_PLAYER_ID && IsPlayerConnected(gmData[artefactOwner])){

    format(gstr,sizeof gstr,"Artefakt jest w posiadaniu gracza {b}%s{/b}",GetPlayerNick(gmData[artefactOwner]),gmData[artefactOwner]);
    Msg(playerid,COLOR_INFO,gstr);

    if(GetPlayerInterior(gmData[artefactOwner])>0) return Msg(playerid,COLOR_INFO2,"Gracz jest w pomieszczeniu, kod b³êdu: #TARGET_PLAYERID_AT_OTHER_INT_CRC");

    if (GetPlayerInterior(playerid)==GetPlayerInterior(gmData[artefactOwner]) && GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(gmData[artefactOwner])) {
      new Float:FP[3];
      GetPlayerPos(gmData[artefactOwner],FP[0],FP[1],FP[2]);
      new Float:dist=GetPlayerDistanceFromPoint(playerid,FP[0],FP[1],FP[2]);

      format(gstr, sizeof gstr,"Odleglosc do gracza: {b}%.1f{/b}m", dist);
      Msg(playerid,COLOR_INFO2, gstr);
    }else return Msg(playerid,COLOR_INFO2,"Nie mo¿na pobraæ odleg³oœci od gracza, kod b³êdu: #TARGET_PLAYERID_AT_OTHER_VW_CRC");


  }else{
      format(gstr,sizeof gstr,"Artefakt jest gdzieœ na mapie. {b}Znajdz go, zanim zrobia to inni!{/b}");
    Msg(playerid,COLOR_INFO,gstr);
    new Float:FP[3];
    GetPlayerPos(playerid,FP[0],FP[1],FP[2]);
    new Float:dist=GetDistanceBetweenPoints3D(FP[0],FP[1],FP[2],gmTemp[artePos][0],gmTemp[artePos][1],gmTemp[artePos][2]);
    format(gstr, sizeof gstr,"Odleglosc: {b}%.1f{/b}m", dist);
    Msg(playerid,COLOR_INFO2, gstr);
  }

  return 1;
}

CMD:pomoc(playerid)
{
  ShowPlayerDialog(playerid, DIALOG_HELP_MAIN, DIALOG_STYLE_LIST, TXT(playerid, 97), GetDialogList(playerid, DIALOG_HELP_MAIN), __("Wybierz"), __("Wyjdz"));

  return 1;
}

CMD:nowynick(playerid, params[])
{
  new
   buffer[128];

  if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 22)); // Musisz byæ zarejestrowany, aby u¿yæ tej komendy.

  format(buffer, sizeof buffer, "SELECT TIMESTAMPDIFF(MINUTE, NOW(), next_nick_change) FROM %s WHERE id = %i AND next_nick_change > NOW() LIMIT 1",
    gmData[DB_players],
    pData[playerid][accountID]
  );

  mysql_query(buffer);
  mysql_store_result();

  if(mysql_num_rows())
  {
    new
     nextChangeTime,
     tempPeriod;

    mysql_fetch_row_format(buffer);
    nextChangeTime = StringToInt(buffer) * 60;

    GetOptimalTimeUnit(nextChangeTime, tempPeriod);

    format(buffer, sizeof(buffer), TXT(playerid, 23), nextChangeTime, GetPeriodName(playerid, tempPeriod, nextChangeTime));
    return Msg(playerid, COLOR_ERROR, buffer); // Nastêpna zmiana nicku bêdzie mo¿liwa po xxx minutach/godzinach.
  }

  mysql_free_result();

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 16)); // U¿ywaj: /nowynick <nowy nick>
  if(!IsNickCorrect(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 17)); // Podany nick zawiera niedozwolone znaki lub nie mieœci siê w przedziale 3-20 znaków.
  if(AccountExists(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 333)); // Podany nick jest ju¿ zarezerwowany przez innego gracza, musisz wybraæ inny.

  pTemp[playerid][loginAttemps] = 0;
  copy(params, pTemp[playerid][newNick]);

  ShowPlayerDialog(playerid, DIALOG_NICK_CHANGE, DIALOG_STYLE_PASSWORD, TXT(playerid, 55), TXT(playerid, 56), TXT(playerid, 57), TXT(playerid, 58));

  return 1;
}

CMD:zmienhaslo(playerid, params[])
{
  if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 22)); // Musisz byæ zarejestrowany, aby u¿yæ tej komendy.

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 352)); // U¿ywaj: /zmienhaslo <nowe has³o>
  if(!IsPasswordCorrect(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 3)); // Has³o mo¿e sk³adaæ siê tylko z liter i cyfr, oraz musi mieæ od 5 do 30 znaków.

  pTemp[playerid][loginAttemps] = 0;
  copy(params, pTemp[playerid][newPassword]);

  ShowPlayerDialog(playerid, DIALOG_PASSWORD_CHANGE, DIALOG_STYLE_PASSWORD, TXT(playerid, 349), TXT(playerid, 350), TXT(playerid, 57), TXT(playerid, 58));

  return 1;
}

CMD:zmienemail(playerid, params[])
{
  if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 22)); // Musisz byæ zarejestrowany, aby u¿yæ tej komendy.

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "U¿ywaj: /zmienemail <nowy email>");
  if(!IsValidEmail(params)) return Msg(playerid, COLOR_ERROR, "Proszê podaæ poprawny adres email w formie u¿ytkownik@host.domena"); // Has³o mo¿e sk³adaæ siê tylko z liter i cyfr, oraz musi mieæ od 5 do 30 znaków.
  if(EmailExists(params)) return Msg(playerid, COLOR_ERROR, "Na ten adres email jest juz zarejestrowane inne konto. Je¿eli zapomnia³eœ swojego has³a u¿yj mo¿liwoœci przypominania has³a na forum!");

  copy(params, pTemp[playerid][newEmail]);

    pTemp[playerid][loginAttemps]=0;
  ShowPlayerDialog(playerid, DIALOG_EMAIL_CHANGE, DIALOG_STYLE_PASSWORD, "Aktualizacja adresu email", TXT(playerid, 350), TXT(playerid, 57), TXT(playerid, 58));

  return 1;
}

CMD:mojskin(playerid)
{
  if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 22)); // Musisz byæ zarejestrowany, aby u¿yæ tej komendy.
  if(!pData[playerid][classSelecting]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 100)); // Ta komenda jest dostêpna tylko w wybiera³ce skinów.
  if(!pData[playerid][lastUsedSkin]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 101)); // Bêdziesz móg³ u¿yæ tej komendy po ponownym wejœciu na serwer.

  SetSpawnInfo(playerid, (playerid+1), pData[playerid][lastUsedSkin], -2876.87, 2807.71, 252.25, 45.0, 0, 0, 0, 0, 0, 0);
  SpawnPlayer(playerid);
  OnPlayerRequestSpawn(playerid);

  return 1;
}

CMD:skin(playerid, params[])
{
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 102)); // U¿ywaj: /skin <id skina>
  if(pData[playerid][pAttraction]!=A_NONE || GetPlayerState(playerid)!=PLAYER_STATE_ONFOOT) return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
  if(!IsNumeric(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 104)); // Wartoœæ tej komendy musi byæ numeryczna.

  new
   skinid = strval(params),
   buffer[128];

  if(!IsValidSkin(skinid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 103)); // Nieprawid³owy identyfikator skina.
  SetPlayerSkin(playerid,skinid);
  pData[playerid][lastUsedSkin]=skinid;

  if (pData[playerid][classSelecting])
    OnPlayerRequestClass(playerid,skinid);

  format(buffer, sizeof buffer, TXT(playerid, 105), skinid);
  Msg(playerid, COLOR_INFO, buffer);

  SetPlayerArmedWeapon(playerid,0);

  return 1;
}

CMD:pm(playerid, params[])
{
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 136)); // U¿ywaj: /pw <gracz> <tekst>
  if (isPlayerMuted(playerid))
    return Msg(playerid,COLOR_ERROR,__("Jesteœ wyciszony - nie mo¿esz pisaæ"));
  new
   szPlayerName[25],
   szMessage[111],
   buffer[256],
   targetplayerid;
  if (strlen(params)>110)
    return Msg(playerid,COLOR_ERROR,__("Wprowadzony tekst jest zbyt dlugi!"));
  sscanf(params, "s[24]s[96]", szPlayerName, szMessage);

  targetplayerid = FindPlayer(szPlayerName);

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

  if(playerid == targetplayerid) return Msg(playerid, COLOR_ERROR, TXT(playerid, 137)); // Wysy³anie prywatnych wiadomoœci do siebie jest co najmniej idiotyczne.

  if (pTemp[targetplayerid][accepts][eac_pm]==ACCEPTS_NONE && pData[playerid][adminLevel]!=LEVEL_ADMIN3)  {
    format(buffer,sizeof buffer,__("{b}%s{/b} nie przyjmuje zadnych wiadomosci prywatnych."), GetPlayerProperName(targetplayerid));
    return Msg(playerid,COLOR_INFO2,buffer);
  }

  if (isPlayerMuted(targetplayerid))  {
    format(buffer,sizeof buffer,__("{b}%s{/b} jest wyciszony i nie mo¿e Ci odpisaæ."), GetPlayerProperName(targetplayerid));
    Msg(playerid,COLOR_INFO2,buffer);
  }

  if (pData[targetplayerid][pAttraction]>0)  {
    format(buffer,sizeof buffer,__("{b}%s{/b} jest na atrakcji i mo¿liwe, ¿e nie bêdzie móg³ Ci odpisaæ."), GetPlayerProperName(targetplayerid));
    Msg(playerid,COLOR_INFO2,buffer);
  }

  if(pData[targetplayerid][jail] - (GetTickCount() / 1000) > 0 && pData[targetplayerid][jail] != 0){
    format(buffer,sizeof buffer,__("{b}%s{/b} jest w wiêzieniu i nie mo¿e Ci odpisaæ."), GetPlayerProperName(targetplayerid));
    Msg(playerid,COLOR_INFO2,buffer);
  }

  if(FilterText(playerid, szMessage) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  format(buffer, sizeof buffer, ">>> {b}%s(%d){/b} %s", GetPlayerProperName(targetplayerid), targetplayerid, szMessage);
  Msg(playerid, COLOR_PRIVATEMSG, buffer, false, false); // (PW do xxx) xxx

  new bool:ad_detected=false;
  if ((pTemp[playerid][troll] || pTemp[targetplayerid][troll])) {
    // nic z trollem nie robimy
  } else if (ContainsIP(szMessage)) {
    ad_detected=true;
    format(buffer, sizeof buffer, __("Ninjabanowana proba reklamy przez {b}%s (%d){/b}."), GetPlayerNick(playerid), playerid);
    MSGToAdmins(COLOR_ADMIN, buffer, false, LEVEL_ADMIN2);
  } else if (!pTemp[targetplayerid][fakeAFK] && pTemp[targetplayerid][staleTime]<=30) {
    format(buffer, sizeof buffer, "<<< {b}%s(%d){/b} %s", GetPlayerProperName(playerid), playerid, szMessage);
    Msg(targetplayerid, COLOR_PRIVATEMSG, buffer, false, false); // (PW od xxx (xxx)) xxx
  } else {
      format(buffer, sizeof buffer, "<<< {b}%s(%d){/b} %s", GetPlayerProperName(playerid), playerid, szMessage);
    Msg(targetplayerid, COLOR_PRIVATEMSG, buffer, false, false); // (PW od xxx (xxx)) xxx
  }

  if ((pTemp[targetplayerid][troll] || pTemp[playerid][troll]))
    format(buffer, sizeof buffer, "{b}TROLL{/b} %s -> %s: %s", GetPlayerNick(playerid), GetPlayerNick(targetplayerid), szMessage);
  else
    format(buffer, sizeof buffer, "%s -> %s: %s", GetPlayerNick(playerid), GetPlayerNick(targetplayerid), szMessage);
  OutputLog(LOG_PM, buffer);


  if ((pTemp[targetplayerid][troll] || pTemp[playerid][troll]) && gmTemp[pAbsAdminCount]>0 && gmTemp[showTroll]) {
    foreach(i)
      if(pData[i][adminLevel]>=LEVEL_ADMIN2 && i!=playerid && i!=targetplayerid)
        Msg(i,COLOR_INFO3, buffer, false);
  } else if((!pTemp[targetplayerid][troll] && !pTemp[playerid][troll]) &&
      (gmTemp[snoopPM]==1 && gmTemp[pAbsAdminCount]>0 &&  pData[playerid][respect]<1000 )
      ||
      (gmTemp[snoopPM]==2 && gmTemp[pAbsAdminCount]>0)
      || ad_detected
    ) {

    foreach(i)
      if(pData[i][adminLevel]>=LEVEL_ADMIN2 && i!=playerid && i!=targetplayerid)
        Msg(i,COLOR_INFO3, buffer, false);
  }


  PlaySound(playerid, 1150);

  if (!ad_detected && !pTemp[targetplayerid][troll] && !pTemp[playerid][troll]) {
    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, "~y~priv msg", 2000, 1);
  }

  if (pTemp[targetplayerid][fakeAFK] || pTemp[targetplayerid][staleTime]>=30) {
    format(buffer, sizeof buffer,__("{b}%s{/b} jest AFK."), GetPlayerProperName(targetplayerid));
    Msg(playerid, COLOR_INFO2, buffer, false);
  }
  if (isPlayerMuted(targetplayerid)){
    format(buffer, sizeof buffer,"{b}%s{/b} jest obecnie wyciszon%s, nie bedzie w stanie Ci odpisac.", GetPlayerProperName(targetplayerid), Kobieta(targetplayerid)?("a"):("y"));
    Msg(playerid, COLOR_INFO2, buffer, false);
  }

  return 1;
}

CMD:toadmin(playerid, params[])
{
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, __("Uzywaj: {b}/toadmin <tresc>{/b}"));

  new
   buffer[160];
  if (strlen(params)>100)
    return Msg(playerid,COLOR_ERROR,__("Wprowadzony tekst jest zbyt dlugi!"));



  format(buffer, sizeof buffer, __("DO ADMINOW >>> {b}%s{/b}"), params);
  Msg(playerid, COLOR_ADMIN, buffer, false, false);

  format(buffer, sizeof buffer, "TOADMIN: {b}%s(%d){/b} >>> {b}%s{/b}", GetPlayerProperName(playerid), playerid, params);

  if (gmTemp[pAdminCount]>0)
    MSGToAdmins(COLOR_ADMIN, buffer, false, LEVEL_ADMIN1);
  else
    Msg(playerid,COLOR_ERROR, __("Obecnie brak jest zalogowanych adminow..."));

  OutputLog(LOG_PM, buffer);

  return 1;
}

CMD:togm(playerid, params[])
{
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, __("Uzywaj: {b}/togm <tresc>{/b}")); // U¿ywaj: /pw <gracz> <tekst>

  if (isPlayerMuted(playerid))
    return Msg(playerid,COLOR_ERROR,__("Jesteœ wyciszony - nie mo¿esz pisaæ."));

  new
   buffer[160];
  if (strlen(params)>100)
    return Msg(playerid,COLOR_ERROR,__("Wprowadzony tekst jest zbyt dlugi!"));

  format(buffer, sizeof buffer, __("DO GMOW >>> {b}%s{/b}"), params);
  Msg(playerid, COLOR_GM, buffer, false, false);

  format(buffer, sizeof buffer, "TOGM: {b}%s(%d){/b} >>> {b}%s{/b}", GetPlayerProperName(playerid), playerid, params);

  if (gmTemp[pGMCount]>0)
    MSGToAdmins(COLOR_GM, buffer, false, LEVEL_GM, LEVEL_ADMINHIDDEN);
  else if (gmTemp[pAdminCount]>0)
    Msg(playerid,COLOR_ERROR, __("Obecnie brak jest zalogowanych moderatorow, sprobuj {b}/toadmin{/b}."));
  else
    Msg(playerid,COLOR_ERROR, __("Obecnie brak jest zalogowanych moderatorow..."));

  OutputLog(LOG_PM, buffer);

  return 1;
}

CMD:l(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsPlayerSpawned(playerid) && !IsAdmin(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 24));
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 144)); // U¿ywaj: /l <tekst>

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0, int,vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);
  format(buffer, sizeof buffer, "%d %s: %s", playerid, GetPlayerProperName(playerid), params);

  UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xaaaaaaff,params);
  pTemp[playerid][lcTicks]=7;

  foreach(i)
  if ((IsPlayerStreamedIn(playerid, i) || pData[playerid][spectating]!=INVALID_PLAYER_ID) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw && i!=playerid)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      if(!pTemp[playerid][troll]) SendClientMessage(i, gmData[color_chatIC]*256, buffer);
      if (IsPlayerStreamedIn(i, playerid) || IsAdmin(playerid)) pCount++; // sprawdzamy w druga strone, aby uniemozliwic graczowi sprawdzanie czy admin go specuje poprzez wpisanie /l
    }
  }

  if(pCount)
  {
    SendClientMessage(playerid, gmData[color_chatIC], buffer);

    format(buffer, sizeof buffer, "[LOCAL] %s(%d): %s", GetPlayerNick(playerid), playerid, params);
    OutputLog(LOG_OCHAT, buffer);
  }
  else
    Msg(playerid, COLOR_ERROR, TXT(playerid, 145));

  return 1;
}

CMD:zw(playerid)
{
  if (gmTemp[chatDisabled])
    return Msg(playerid, COLOR_ERROR, __("Czat jest wylaczony."));

  new buffer[128];
  format(buffer, sizeof buffer, __("%s {%s}zaraz wraca."), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));
  if(!pTemp[playerid][troll]) SendClientMessageToAll(gmData[color_chatME_HL] * 256, buffer);

  format(buffer, sizeof buffer, "[CHAT] %s(%d) ZW", GetPlayerNick(playerid), playerid);
  OutputLog(LOG_OCHAT, buffer);

  return 1;
}

CMD:jj(playerid)
{
  if (gmTemp[chatDisabled])
    return Msg(playerid, COLOR_ERROR, __("Czat jest wylaczony."));
  new buffer[128];
  if (random(2)==1)
    format(buffer, sizeof buffer, __("%s {%s}juz jest!"), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));
  else {
    if (SkinKobiecy(GetPlayerSkin(playerid)))
      format(buffer, sizeof buffer, __("%s {%s}wrocila."), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));
    else
      format(buffer, sizeof buffer, __("%s {%s}wrocil."), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));
  }

  if(!pTemp[playerid][troll]) SendClientMessageToAll(gmData[color_chatME_HL] * 256, buffer);

  format(buffer, sizeof buffer, "[CHAT] %s(%d) JJ", GetPlayerNick(playerid), playerid);
  OutputLog(LOG_OCHAT, buffer);

  return 1;
}

CMD:siema(playerid)
{
  if (gmTemp[chatDisabled])
    return Msg(playerid, COLOR_ERROR, __("Krzyczysz {b}HEJ{/b}, ale nikt Cie nie slyszy."));
  new buffer[128];
  if (random(2)==1)
    format(buffer, sizeof buffer, __("%s {%s}mowi siema!"), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));
  else
    format(buffer, sizeof buffer, __("%s {%s}mowi hej!"), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));

  if(!pTemp[playerid][troll]) SendClientMessageToAll(gmData[color_chatME_HL] * 256, buffer);

  format(buffer, sizeof buffer, __("[CHAT] %s(%d) /siema"), GetPlayerNick(playerid), playerid);
  OutputLog(LOG_OCHAT, buffer);

  return 1;
}

CMD:nara(playerid)
{
  if (gmTemp[chatDisabled])
    return Msg(playerid, COLOR_ERROR, __("Krzyczysz {b}NARA{/b}, ale nikt Cie nie slyszy."));
  new buffer[128];
  if (random(2)==1)
    format(buffer, sizeof buffer, __("%s {%s}mowi nara!"), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));
  else
    format(buffer, sizeof buffer, __("%s {%s}mowi narazie!"), GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]));

  if(!pTemp[playerid][troll]) SendClientMessageToAll(gmData[color_chatME_HL] * 256, buffer);

  format(buffer, sizeof buffer, "[CHAT] %s(%d) /nara", GetPlayerNick(playerid), playerid);
  OutputLog(LOG_OCHAT, buffer);

  return 1;
}

CMD:staty(playerid)
{
  ShowPlayerDialog(playerid, DIALOG_STATS_MAIN, DIALOG_STYLE_LIST, TXT(playerid, 154), GetDialogList(playerid, DIALOG_STATS_MAIN), __("Wybierz"), __("Wyjdz"));
  return 1;
}

CMD:top(playerid)
{
  ShowPlayerDialog(playerid, DIALOG_TOP_MAIN, DIALOG_STYLE_LIST, TXT(playerid, 157), GetDialogList(playerid, DIALOG_TOP_MAIN), __("Wybierz"), __("Wyjdz"));
  return 1;
}

CMD:lock(playerid)
{
  MUST_BE_SPAWNED(playerid);

  new
   pVehicleID = GetPlayerVehicleID(playerid),
   pVehicleModel = GetVehicleModel(pVehicleID);
  if ((pVehicleID==0 || pVehicleID==INVALID_VEHICLE_ID) && IsGM(playerid))
    pVehicleID=GetNearestVehicleToPlayer(playerid,4);
  if (pVehicleID==0 || pVehicleID==INVALID_VEHICLE_ID)
    return Msg(playerid, COLOR_ERROR, TXT(playerid, 224));
  if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER && !IsGM(playerid))
    return Msg(playerid,COLOR_ERROR,"Komenda dostepna tylko dla kierowcow");
  if(pVehicleModel==537 || pVehicleModel==538)
        return Msg(playerid,COLOR_ERROR,"Komenda niedostepna dla tego pojazdu");

  if(vehicleDoorState[pVehicleID] == DOOR_CLOSED) return Msg(playerid, COLOR_ERROR, TXT(playerid, 227));

  new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(pVehicleID, engine, lights, alarm, doors, bonnet, boot, objective);
  SetVehicleParamsEx(pVehicleID, engine, lights, alarm, 1, bonnet, boot, objective);

  SetVehicleParamsForPlayer(pVehicleID, playerid, 0, DOOR_OPENED);

  vehicleDoorState[pVehicleID] = DOOR_CLOSED;
  vehicleDoorOwner[pVehicleID] = playerid;

  Msg(playerid, COLOR_INFO, TXT(playerid, 225));
  if (GetPlayerVehicleID(playerid)==pVehicleID) {
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid],0xcc000030);
    if(pData[playerid][hudSetting][HUD_VEHICLEBOX]) ShowElement(playerid, TDE_VEHICLEBOX, true);
  }

  return 1;
}

CMD:unlock(playerid)
{
  MUST_BE_SPAWNED(playerid);

  new
   pVehicleID = GetPlayerVehicleID(playerid),
   pVehicleModel = GetVehicleModel(pVehicleID);
  if (pVehicleID==0 || pVehicleID==INVALID_VEHICLE_ID)
    pVehicleID=GetNearestVehicleToPlayer(playerid,4);
  if (pVehicleID==0  || pVehicleID==INVALID_VEHICLE_ID)
    return Msg(playerid, COLOR_ERROR, __("Wsiadz, lub stan obok pojazdu ktory chcesz otworzyc."));
  if (vehicleDoorOwner[pVehicleID]!=playerid && !IsGM(playerid))
    return Msg(playerid, COLOR_ERROR, __("Nie masz klucza do tego pojazdu."));
  if (pVehicleID==0 || pVehicleID==INVALID_VEHICLE_ID)
    return Msg(playerid, COLOR_ERROR, TXT(playerid, 224));
  if(pVehicleModel==537 || pVehicleModel==538)
        return Msg(playerid,COLOR_ERROR,"Komenda niedostepna dla tego pojazdu");

  if(vehicleDoorState[pVehicleID] == DOOR_OPENED) return Msg(playerid, COLOR_ERROR, TXT(playerid, 228));

  new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(pVehicleID, engine, lights, alarm, doors, bonnet, boot, objective);
  SetVehicleParamsEx(pVehicleID, engine, lights, alarm, 0, bonnet, boot, objective);

  SetVehicleParamsForPlayer(pVehicleID, playerid, 0, DOOR_OPENED);

  vehicleDoorState[pVehicleID] = DOOR_OPENED;
  vehicleDoorOwner[pVehicleID] = playerid;


  Msg(playerid, COLOR_INFO, TXT(playerid, 226));
  if (GetPlayerVehicleID(playerid)==pVehicleID) {
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid],3145776);
    if(pData[playerid][hudSetting][HUD_VEHICLEBOX]) ShowElement(playerid, TDE_VEHICLEBOX, true);
  }

  return 1;
}

CMD:chowany(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pData[playerid][aChowany]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 252));
  if(GetAttractionPlayerCount(A_CHOWANY) >= MAX_QUEUE_CHOWANY) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aChowany] = true;
  Msg(playerid, COLOR_INFO, TXT(playerid, 260));

  return 1;
}

CMD:derby(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pData[playerid][aDerby]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 254));
  if(GetAttractionPlayerCount(A_DERBY) >= MAX_QUEUE_DERBY) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aDerby] = true;
  Msg(playerid, COLOR_INFO, TXT(playerid, 262));

  return 1;
}

CMD:race(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pData[playerid][aRace]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 256));
  if(GetAttractionPlayerCount(A_RACE) >= MAX_QUEUE_RACE) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aRace] = true;
  Msg(playerid, COLOR_INFO, TXT(playerid, 264));

  return 1;
}

CMD:drifting(playerid)
{
  if(pData[playerid][averagePing]>150)
    return Msg(playerid,COLOR_ERROR,__("Niestety! Twoj sredni ping przekracza {b}150ms{/b} - nie mozesz zapisac sie na ta atrakcje!"));


  MUST_BE_SPAWNED(playerid);
  if(pData[playerid][aDrift]) return Msg(playerid, COLOR_ERROR, __("Jestes juz w kolejce na {b}/drifting{/b}."));
  if(GetAttractionPlayerCount(A_DRIFT) >= MAX_QUEUE_DRIFT) return Msg(playerid, COLOR_ERROR, __("Kolejka jest pelna."));

  pData[playerid][aDrift] = true;
  Msg(playerid, COLOR_INFO, __("Zapisales/as sie do kolejki na {b}/drifting{/b}."));

  return 1;
}

CMD:ctf(playerid)
{
    MUST_BE_SPAWNED(playerid);
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if(pData[playerid][aCTF]) return Msg(playerid, COLOR_ERROR, "Jestes juz zapisany do kolejki na {b}/CTF{/b}");
  if(GetAttractionPlayerCount(A_CTF) >= MAX_QUEUE_CTF) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aCTF] = true;
  Msg(playerid, COLOR_INFO, "Zapisales sie do kolejki na {b}/CTF{/b}");

  return 1;
}

CMD:gg(playerid)
{
    MUST_BE_SPAWNED(playerid);
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if(pData[playerid][aGG]) return Msg(playerid, COLOR_ERROR, "Jestes juz zapisany do kolejki na {b}/GG{/b}");
  if(GetAttractionPlayerCount(A_GG) >= MAX_QUEUE_GG) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aGG] = true;
  Msg(playerid, COLOR_INFO, "Zapisales sie do kolejki na {b}/GG{/b}");

  return 1;
}

CMD:wg(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if(pData[playerid][aWG]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 258));
  if(GetAttractionPlayerCount(A_WG) >= MAX_QUEUE_WG) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aWG] = true;
  Msg(playerid, COLOR_INFO, TXT(playerid, 266));

  return 1;
}

CMD:strzelnica(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if(pData[playerid][aStrzelnica]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 259));
  if(GetAttractionPlayerCount(A_STRZELNICA) >= MAX_QUEUE_STRZELNICA) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aStrzelnica] = true;
  Msg(playerid, COLOR_INFO, TXT(playerid, 267));

  return 1;
}

CMD:hitman(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 290)); // U¿ywaj: /zlecenie <gracz> <nagroda: kwota>

  new
   szPlayerName[24],
   targetplayerid,
   amount,
   buffer[128],
   pomoney;

  pomoney=GetPlayerMoney(playerid);

  sscanf(params, "s[24]i", szPlayerName, amount);

  if (amount>pomoney)
    return Msg(playerid, COLOR_ERROR, __("Nie masz tyle gotowki!"));

  targetplayerid = FindPlayer(szPlayerName);

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

  if(targetplayerid == playerid) return Msg(playerid,COLOR_INFO,"Nie sadzisz, ze wyznaczanie nagrody na samego siebie jest dziwne? ;>");

  if(amount < 1 || amount > 5000000) return Msg(playerid, COLOR_ERROR, "Wartoœæ pieniêdzy musi siê mieœciæ w przedziale 1 - 5000000"); // Wartoœæ pieniêdzy musi siê mieœciæ w przedziale 1 - 5000000.


  new
   currentPrize = pData[targetplayerid][hitman];

  if (currentPrize > 80000000)
    return Msg(playerid, COLOR_INFO, __("Za tego gracza wystawiona juz jest {b}maksymalna{/b} nagroda!"));

  GivePlayerMoney(playerid, -amount);


  if (currentPrize>0) {
    format(buffer, sizeof buffer, __("Stawka za zabojstwo gracza {b}%s{/b} zwiekszona przez {b}%s{/b} z %d do %d$!"), GetPlayerProperName(targetplayerid), GetPlayerProperName(playerid), currentPrize, currentPrize+amount);
    foreach(i)
      Msg(i, COLOR_INFO3, buffer, amount>500000?true:false ); // Stawka za zabójstwo gracza "xxx" zwiêkszona przez "%s" z $%i do $%i!
  } else {
    format(buffer, sizeof buffer, TXT(targetplayerid, 292), GetPlayerNick(targetplayerid), amount, GetPlayerProperName(playerid));
    foreach(i)
      Msg(i, COLOR_INFO3, buffer, amount>500000?true:false ); // Nowa nagroda za zabójstwo gracza "xxx" - $xxx (wystawione przez "%s").
  }

  IncreasePlayerHitman(targetplayerid, amount);
  return 1;
}

CMD:givecash(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 295)); // U¿ywaj: /dajkase <gracz> <kwota>

  new
   szPlayerName[24],
   targetplayerid,
   amount,
   buffer[128];

  sscanf(params, "s[24]i", szPlayerName, amount);

  targetplayerid = FindPlayer(szPlayerName);

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

  if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));
  if (amount<=0) return Msg(playerid, COLOR_ERROR, __("Takie kwoty musisz ustalac z [FS]$KOMORNIKIEM$ :-)"));
  if(amount > GetPlayerMoney(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 124)); // Podana iloœæ pieniêdzy przekracza wartoœæ Twojego portfela.

  if(amount>10000){
      return Msg(playerid, COLOR_INFO, "Nie mo¿esz przes³aæ tak du¿ej kwoty (przekracza limit {b}$10000{/b}). Skorzystaj z przelewu w {b}/bank{/b}u.");
  }

  GivePlayerMoney(playerid, -amount);
  GivePlayerMoney(targetplayerid, amount, true);

  if (pData[playerid][hitman]>0 && pTemp[targetplayerid][faction]==FACTION_POLICE) {  // danie kasy policjantowi -> splata zadluzenia
    Msg(playerid,COLOR_INFO,__("Splaciles/as swoje mandaty/grzywne."));
    pData[playerid][hitman]-=amount;
    if (pData[playerid][hitman]<0) pData[playerid][hitman]=0;
    UpdatePlayerFactionName(playerid);
  }

  format(buffer, sizeof buffer, TXT(playerid, 297), GetPlayerNick(targetplayerid), amount);
  Msg(playerid, COLOR_INFO, buffer); // Przekaza³eœ graczowi "xxx" $xxx.

  format(buffer, sizeof buffer, TXT(targetplayerid, 298), GetPlayerNick(playerid), amount);
  Msg(targetplayerid, COLOR_INFO, buffer); // Gracz "%s" przekaza³ Ci $%i.

  return 1;
}

CMD:dajgp(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "U¿ywaj: /dajgp <gracz> <ilosc>"); // U¿ywaj: /dajkase <gracz> <kwota>

  new
   szPlayerName[24],
   targetplayerid,
   amount,
   buffer[128];

  sscanf(params, "s[24]i", szPlayerName, amount);

  targetplayerid = FindPlayer(szPlayerName);

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

  if(pData[playerid][respect]<2000) return Msg(playerid, COLOR_ERROR, "Musisz posiadac minimum 2k (2000) respektu aby wykonywac przelewy GP");
  if (amount<=0) return Msg(playerid, COLOR_ERROR, __("Takie kwoty musisz ustalac z [FS]$KOMORNIKIEM$ :-)"));
  if(amount > pData[playerid][gamep]) return Msg(playerid, COLOR_ERROR, "Podana ilosc GamePoints przekracza zawartosc twojego konta"); // Podana iloœæ pieniêdzy przekracza wartoœæ Twojego portfela.

  if(amount>80){
      return Msg(playerid, COLOR_INFO, "Maksymalna wartosc przelewu to {b}80{/b} GamePoints.");
  }

  GivePlayerGamePoints(playerid, -amount, "przelew");
  GivePlayerGamePoints(targetplayerid, amount, "przelew");

  format(buffer, sizeof buffer, "Przekaza³eœ graczowi {b}%s{/b} GamePoints w iloœci {b}%d{/b}", GetPlayerNick(targetplayerid), amount);
  Msg(playerid, COLOR_INFO, buffer); // Przekaza³eœ graczowi "xxx" $xxx.

  format(buffer, sizeof buffer, "Gracz {b}%s{/b} przekaza³ Ci {b}%d{/b} GamePoints", GetPlayerNick(playerid), amount);
  Msg(targetplayerid, COLOR_INFO, buffer); // Gracz "%s" przekaza³ Ci $%i.

  return 1;
}

CMD:hud(playerid)
{
  ShowPlayerDialog(playerid, DIALOG_HUD, DIALOG_STYLE_LIST, TXT(playerid, 329), GetDialogList(playerid, DIALOG_HUD), TXT(playerid, 330), __("Wyjdz"));

  return 1;
}

CMD:kill(playerid, params[])
{
  if (IsAdmin(playerid))
    return _killplayer(playerid,params);

  MUST_BE_SPAWNED(playerid);

    if (pData[playerid][skill]>0) pData[playerid][skill]--;
  SetPlayerHealth(playerid, 0.0);
  return 1;
}

CMD:raport(playerid, params[])
{
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 402)); // U¿ywaj: /raport <gracz> <powód>

  new
   szPlayerName[24],
   targetplayerid,
   buffer[128],
   szReason[128];

  sscanf(params, "s[24]s[128]", szPlayerName, szReason);
  targetplayerid = FindPlayer(szPlayerName);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), szPlayerName);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if (GetPlayerCurrentSession(targetplayerid)<=5)
    return Msg(playerid, COLOR_ERROR, __("Ten gracz sie dopiero polaczyl."));


  if(pTemp[playerid][reportedTick][targetplayerid] != 0 && GetTickCount() - pTemp[playerid][reportedTick][targetplayerid] < 60000) return Msg(playerid, COLOR_ERROR, TXT(playerid, 407)); // Niedawno raportowa³eœ tego gracza, musisz odczekaæ trochê czasu.

  if(!strlen(szReason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 402)); // U¿ywaj: /raport <gracz> <powód>
  if(strlen(szReason) > 50) return Msg(playerid, COLOR_ERROR, TXT(playerid, 405)); // D³ugoœæ raportu nie mo¿e przekraczaæ 50 znaków.

  AddPlayerToReportList(targetplayerid, szReason, playerid);

  pTemp[playerid][reportedTick][targetplayerid] = GetTickCount();

  format(buffer, sizeof buffer, TXT(playerid, 404), GetPlayerNick(targetplayerid), szReason);
  Msg(playerid, COLOR_INFO, buffer);

  foreach(i)
    if(IsAdmin(i, LEVEL_ADMIN1))
    {
      format(buffer, sizeof buffer, __("[RAPORT] {ffffff}Nowy raport!"));
      SendClientMessage(i, 0xff0000ff, buffer);
      GameTextForPlayer(i, __("~n~~n~~n~~n~~r~Raport!"), 1000, 5);
      PlaySound(i, 1147);
    }

  return 1;
}

CMD:admins(playerid)
{
  for(new l = LEVEL_ADMIN3; l >= LEVEL_GM; l--)
  {
    new
     szMsg[128],
     buffer[32],
     counter = 0;

    format(szMsg, sizeof szMsg, TXT(playerid, 416 + l));

    foreach(i)
    {
      if(pData[i][adminLevel] == l)
      {
        format(buffer, sizeof buffer, " %s (%i),", GetPlayerNick(i), i);
        strcat(szMsg, buffer);

        counter++;
      }

      if(strlen(szMsg) >= 90)
      {
        Msg(playerid, COLOR_INFO, szMsg, false);
        format(szMsg, sizeof szMsg, "  ");
      }
    }

    if(counter)
    {
      strdel(szMsg, strlen(szMsg) - 1, strlen(szMsg));
      Msg(playerid, COLOR_INFO, szMsg, false);
    }
    else
    {
      format(buffer, sizeof buffer, "%s %s", TXT(playerid, 416 + l), TXT(playerid, 421));
      Msg(playerid, COLOR_INFO, buffer, false);
    }
  }

  PlaySound(playerid, 1150);

  return 1;
}

CMD:vips(playerid)
{
  new
   szMsg[128],
   buffer[32],
   counter = 0;

  format(szMsg, sizeof szMsg, TXT(playerid, 422));

  foreach(i)
  {
    if(pData[i][vipEnabled])
    {
      format(buffer, sizeof buffer, " %s (%i),", GetPlayerNick(i), i);
      strcat(szMsg, buffer);

      counter++;
    }

    if(strlen(szMsg) >= 90)
    {
      Msg(playerid, COLOR_INFO, szMsg, false);
      format(szMsg, sizeof szMsg, "  ");
    }
  }

  if(counter)
  {
    strdel(szMsg, strlen(szMsg) - 1, strlen(szMsg));
    Msg(playerid, COLOR_INFO, szMsg, false);
  }
  else
  {
    format(buffer, sizeof buffer, "%s %s", TXT(playerid, 422), TXT(playerid, 421));
    Msg(playerid, COLOR_INFO, buffer, false);
  }

  PlaySound(playerid, 1150);

  return 1;
}

CMD:credits(playerid)
{
  OnDialogResponse(playerid, DIALOG_HELP_MAIN, BUTTON_NEXT, f_h_AUTHOR, "");
  return 1;
}

CMD:vipinfo(playerid)
{
  OnDialogResponse(playerid, DIALOG_HELP_MAIN, BUTTON_NEXT, f_h_VIP, "");

  return 1;
}

CMD:respekt(playerid)
{
  OnDialogResponse(playerid, DIALOG_HELP_MAIN, BUTTON_NEXT, f_h_RESPECT, "");
  return 1;
}

CMD:cmd(playerid)
{
  OnDialogResponse(playerid, DIALOG_HELP_MAIN, BUTTON_NEXT, f_h_CMD, "");
  return 1;
}

CMD:teleporty(playerid)
{
  OnDialogResponse(playerid, DIALOG_HELP_CMD, BUTTON_NEXT, f_h__TELEPORTS, "");
  return 1;
}

CMD:rsp(playerid,params[]){
  if (pData[playerid][classSelecting])
    return Msg(playerid,COLOR_INFO2,__("Nie mozesz teraz z tego skorzystac."));

  if (pTemp[playerid][dead])
    return Msg(playerid,COLOR_ERROR,Kobieta(playerid)?(__("Jestes martwa!")):(__("Jestes martwy!")));

  if (pTemp[playerid][AFK])
    Msg(playerid,COLOR_ERROR,__("Jestes AFK! Prawdopodobnie jestes zdesynchronizowany i musisz polaczyc sie ponownie z serwerem!"));

  if (pTemp[playerid][isFreezed])
    return Msg(playerid,COLOR_ERROR,__("Jestes zamrozony przez Administratora/GM'a!"));

  new Float:hp;
  GetPlayerHealth(playerid, hp);
  if (IsPlayerInAnyVehicle(playerid)) {
    SetPlayerHealth(playerid,hp);
    Msg(playerid,COLOR_INFO2,__("Wysiadz z pojazdu zanim uzyjesz {b}/rsp{/b}"));
    return 1;
  }

  new skin,team,Float:ar;
  pTemp[playerid][specPosReturn]=true;
  GetPlayerPos(playerid, pTemp[playerid][specPosition][0], pTemp[playerid][specPosition][1], pTemp[playerid][specPosition][2]);
  GetPlayerFacingAngle(playerid, pTemp[playerid][specPosition][3]);

  ClearAnimations(playerid);

  pTemp[playerid][specInterior] = GetPlayerInterior(playerid);
  skin = GetPlayerSkin(playerid);
  team = GetPlayerTeam(playerid);
  pTemp[playerid][specVirtualWorld] = GetPlayerVirtualWorld(playerid);

  new bronie[12][2];
  for (new i=0;i<sizeof bronie;i++)
    GetPlayerWeaponData(playerid, i+1, bronie[i][0], bronie[i][1]);

  PlayerPlaySound(playerid, 1083,0,0,0);

  pTemp[playerid][useRsp]=true;

  GetPlayerArmour(playerid, ar);
  SetSpawnInfo(playerid,team,skin,pTemp[playerid][specPosition][0], pTemp[playerid][specPosition][1], pTemp[playerid][specPosition][2]-0.5,pTemp[playerid][specPosition][3],0,0,0,0,0,0);
  SpawnPlayer(playerid);

  SetPlayerHealth(playerid,hp);
  SetPlayerArmour(playerid,ar);

  SetPlayerVirtualWorld(playerid, pTemp[playerid][specVirtualWorld]);

  if (pData[playerid][gang]!=NO_GANG)
    gangs_SetPlayerAttachedObject(playerid);

  if (gmData[artefactOwner] == playerid)
    artefact_SetPlayerHolding(playerid);

  for (new i=0;i<sizeof bronie;i++)
    GivePlayerWeapon(playerid, bronie[i][0], bronie[i][1]);

  SetPlayerArmedWeapon(playerid,bronie[0][0]);

  if(isnull(params)) Msg(playerid,COLOR_INFO,"Wykonales szybki respawn i resynchronizacjê postaci");

  return 1;

}

CMD:tak(playerid){
  if (gmTemp[voteTimeleft]<=0) {
    Msg(playerid,COLOR_ERROR,TXT(playerid,458));  // obecnie nie trwa zadne glosowanie
  } else if (pTemp[playerid][hasVoted]) {
    Msg(playerid,COLOR_ERROR,TXT(playerid,459));  // Oddales/as juz swoj glos
  } else {
    gmTemp[voteCount][0]++;
    pTemp[playerid][hasVoted]=true;
    Msg(playerid,COLOR_INFO,"Glosujesz na {b}TAK{/b}");
  }

  return 1;
}

CMD:nie(playerid){
  if (gmTemp[voteTimeleft]<=0) {
    Msg(playerid,COLOR_ERROR,TXT(playerid,458));  // obecnie nie trwa zadne glosowanie
  } else if (pTemp[playerid][hasVoted]) {
    Msg(playerid,COLOR_ERROR,TXT(playerid,459));  // Oddales/as juz swoj glos
  } else {
    gmTemp[voteCount][1]++;
    pTemp[playerid][hasVoted]=true;
    Msg(playerid,COLOR_INFO,"Glosujesz na {b}NIE{/b}");
  }
  return 1;
}

CMD:100hp(playerid){
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
    if (GetTickCount()-pTemp[playerid][lastHealUsed]<HEAL_REPAIR_INTERVAL) {
      format(gstr,sizeof gstr,"Musisz odczekac {b}%ds{/b} przed kolejnym uzyciem /100hp", (HEAL_REPAIR_INTERVAL-(GetTickCount()-pTemp[playerid][lastHealUsed]))/1000 );
      return Msg(playerid,COLOR_ERROR,gstr);
  }
  if (pData[playerid][pAttraction]!=A_NONE || gmData[artefactOwner]==playerid) return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
  if (GetPlayerMoney(playerid) >= 800) {
    GivePlayerMoney(playerid, -800);
    ApplyAnimation(playerid,"FOOD","EAT_Burger",4.1,0,0,0,0,0);
    SetTimerEx("PlayerSelfHeal", 15000, false, "i" ,playerid);
    Msg(playerid,COLOR_INFO, "Zostaniesz uleczony/-a za 15 sekund.");
    pTemp[playerid][lastHealUsed]=GetTickCount();
  } else
    return Msg(playerid,COLOR_ERROR,__("Uleczenie sie kosztuje {b}800${/b}. Nie stac Cie!"));

  return 1;
}

CMD:raceexit(playerid){
  if (pData[playerid][pAttraction]==A_RACE) {
    race_RemovePlayer(playerid);
    Msg(playerid,COLOR_INFO,__("Zostales wypisany/-a z /race"));
  } else if (pData[playerid][aRace]) {
    pData[playerid][aRace]=false; // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /race"));
  }
  return 1;
}

CMD:strzelexit(playerid){
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (pData[playerid][pAttraction]==A_STRZELNICA) {
    pData[playerid][aStrzelnica]=false; // zapisy
    if (IsPlayerSpawned(playerid)) {
        SpawnPlayer(playerid);
        SetPlayerHealth(playerid,0.0);
    }
    Msg(playerid,COLOR_INFO,__("Opusciles strzelnice."));
    DisablePlayerSounds(playerid);

  } else if (pData[playerid][aStrzelnica]) {
    pData[playerid][aStrzelnica]=false; // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /strzelnica"));
  }
  return 1;
}

CMD:driftexit(playerid){
  if (pData[playerid][pAttraction]==A_DRIFT) {
    drift_RemovePlayer(playerid);
    Msg(playerid,COLOR_INFO,__("Zostales wypisany/-a z /drifting"));
  } else if (pData[playerid][aDrift]) {
    pData[playerid][aDrift]=false;  // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /drifting"));
  }
  return 1;
}

CMD:derbyexit(playerid){
  if (pData[playerid][pAttraction]==A_DERBY) {
    Derby_RemovePlayer(playerid);
    pData[playerid][pAttraction] = A_NONE;
    pData[playerid][aDerby]=false;  // zapisy

    Msg(playerid,COLOR_INFO,__("Zostales wypisany/-a z /derby"));
  } else if (pData[playerid][aDerby]) {
    pData[playerid][aDerby]=false;  // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /derby"));
  }
  return 1;
}

CMD:chexit(playerid){
  if (pData[playerid][pAttraction]==A_CHOWANY) {
    pData[playerid][pAttraction] = A_NONE;
    pData[playerid][aChowany]=false;  // zapisy
    if (!pTemp[playerid][aCHDead]) {
      SpawnPlayer(playerid);
      Msg(playerid,COLOR_INFO,__("Opusciles chowanego."));
    }
  } else if (pData[playerid][aChowany]) {
    pData[playerid][aChowany]=false;  // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /chowany"));
  }
  return 1;
}

CMD:wgexit(playerid){
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (pData[playerid][pAttraction]==A_WG) {
    pData[playerid][pAttraction] = A_NONE;
    pData[playerid][aWG]=false; // zapisy
    TextDrawHideForPlayer(playerid,gTextDraw[TD_AWGINFO]);
    TextDrawHideForPlayer(playerid,gTextDraw[TD_AWGINFOHP]);
    HideProgressBarForPlayer(playerid,gmTemp[aWGPBar]);
    if (!pTemp[playerid][aWGDead]) {
        SetPlayerSkin(playerid,pTemp[playerid][skinBeforeChange]);
        SetSpawnInfo(playerid,playerid,pTemp[playerid][skinBeforeChange],0.0, 0.0, 0.0,0.0,0,0,0,0,0,0);
      SpawnPlayer(playerid);
      Msg(playerid,COLOR_INFO,__("Opusciles WG."));
      GivePlayerScore(playerid,-1,"Opuszczenie wg");
    }
  } else if (pData[playerid][aWG]) {
    pData[playerid][aWG]=false; // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /wg"));
  }
  return 1;
}

CMD:ggexit(playerid){
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (pData[playerid][pAttraction]==A_GG) {
    pData[playerid][pAttraction] = A_NONE;
    pData[playerid][aGG]=false;  // zapisy
    pTemp[playerid][aGGexit]=true;
    Msg(playerid,COLOR_INFO,__("Opusciles GG."));
    GivePlayerScore(playerid,-1,"Opuszczenie gg");
    SetPlayerTeam(playerid, playerid+1);
    PlayerTextDrawHide(playerid,pTextDraw[PTD_AGGINFO][playerid]);
    SetPlayerHealth(playerid, 100.0);
    SpawnPlayer(playerid);
  } else if (pData[playerid][aGG]) {
    pData[playerid][aGG]=false;  // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /GG"));
  }
  return 1;
}

CMD:ctfexit(playerid){
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (pData[playerid][pAttraction]==A_CTF) {
    pData[playerid][pAttraction] = A_NONE;
    pData[playerid][aCTF]=false;  // zapisy
    SetPlayerSkin(playerid,pTemp[playerid][skinBeforeChange]);
    SetSpawnInfo(playerid,playerid,pTemp[playerid][skinBeforeChange],0.0, 0.0, 0.0,0.0,0,0,0,0,0,0);
    SpawnPlayer(playerid);
    pTemp[playerid][aCTFexit]=true;
    Msg(playerid,COLOR_INFO,__("Opusciles CTF."));
    GivePlayerScore(playerid,-1,"Opuszczenie ctf");
    TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFINFOPOINTS]);
    TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFINFO]);
    TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFSTATETITLE]);
    TextDrawHideForPlayer(playerid,gTextDraw[TD_ACTFSTATETEXT]);
    HideProgressBarForPlayer(playerid,gmTemp[aCTFBar]);
    if(gmTemp[aCTFFlaga]==playerid) CTF_RespawnFlagi();
  } else if (pData[playerid][aCTF]) {
    pData[playerid][aCTF]=false;  // zapisy
    Msg(playerid,COLOR_INFO,__("Zrezygnowales/as z zapisow na /ctf"));
  }
  return 1;
}

CMD:bronie(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid) || pTemp[playerid][godMode]) return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz kupic broni!"));
  return ShowPlayerDialog(playerid, DIALOG_WEAPON_QUICKBUY, DIALOG_STYLE_TABLIST_HEADERS, __("Zakup broni"), GetDialogList(playerid, DIALOG_WEAPON_QUICKBUY), __("Wybierz"), __("Wyjdz"));
}

CMD:spadochron(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz kupic spadochronu"));
  if (GetPlayerMoney(playerid)<250 && !IsAdmin(playerid))
    return Msg(playerid, COLOR_ERROR, __("Nie masz wystarczajacej ilosc gotowki ($250)!"));
  if (!IsAdmin(playerid))
    SetPlayerMoney(playerid, GetPlayerMoney(playerid)-250);
  GivePlayerWeapon(playerid,46,1);
  return 1;
}

CMD:aparat(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz kupic aparatu"));
  if (GetPlayerMoney(playerid)<250 && !IsAdmin(playerid))
    return Msg(playerid, COLOR_ERROR, __("Nie masz wystarczajacej ilosc gotowki ($250)!"));
  if (!IsAdmin(playerid))
    SetPlayerMoney(playerid, GetPlayerMoney(playerid)-250);
  GivePlayerWeapon(playerid,43,200);
  return 1;
}

CMD:tune(playerid){
  if (!IsPlayerInAnyVehicle(playerid))
    return Msg(playerid,COLOR_ERROR,__("Musisz byc w pojezdzie"));

  if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz uzyc {b}/tune{/b} bedac pasazerem"));

    if (pData[playerid][pAttraction]!=A_NONE &&  pData[playerid][pAttraction]!=A_ARENA)
        return Msg(playerid,COLOR_ERROR,__("Nie mozesz tego teraz zrobic!"));


  if (GetPlayerMoney(playerid)<1000)
    return Msg(playerid,COLOR_ERROR,__("Nie stac Cie na to! Automatyczny tuning kosztuje {b}1000${/b}"));

  if (TuneCar(GetPlayerVehicleID(playerid))) {
    GivePlayerMoney(playerid, -1000);
      PlaySound(playerid, 1134);
    Msg(playerid, COLOR_INFO, __("Twoj pojazd zostal ztuningowany!"), false);
  } else
    Msg(playerid, COLOR_ERROR, __("Tego pojazdu nie da sie stuningowac!"));

  return 1;
}

CMD:rcmd(playerid) {
  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_TABLIST_HEADERS,__("Komendy za respekt:"),"Opcja\tKoszt\tOpis\n{FFFFFF}/RARMOR\t{ff3030}10\t{ffffff}kamizelka\n{DDDDFF}/RINVISIBLE\t{ff3030}35\t{FFFFFF}Niewidzialnosc na radarze\n{FFAAAA}/RDUALSO\t{ff3030}30\t{ffffff}dwa sawnoffy\n{FFAAAA}/RDUALUZI\t{ff3030}30\t{ffffff}dwa polautomaty\n{FFAAAA}/RDUALPISTOLS\t{ff3030}30\t{ffffff}dwa pistolety","OK","Anuluj");
  return 1;
}

CMD:rarmor(playerid){ // 10
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Musisz byc zarejestrowanym graczem, aby korzystac z /rcmd. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac."));
  if (pData[playerid][respect]<10)
    return Msg(playerid,COLOR_ERROR,__("Masz za malo respektu! Wymagane {b}10{/b}!"));
  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz kupic armora!"));

  GivePlayerScore(playerid,-10, "/rarmor");
  SetPlayerArmour(playerid,100.0);

  Msg(playerid,COLOR_INFO,__("Kupiles {b}armor{/b} za {b}10 punktow{/b} respektu."));
  return 1;
}

CMD:visible(playerid){
  pTemp[playerid][playerColorAlpha]=PLAYER_COLOR_ALPHA;
  SetPlayerProperColor(playerid);

  return Msg(playerid,COLOR_INFO,"Jestes widzialny na radarze!");
}

CMD:rinvisible(playerid){ // 35
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Musisz byc zarejestrowanym graczem, aby korzystac z /rcmd. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac."));
  if (pData[playerid][respect]<35)
    return Msg(playerid,COLOR_ERROR,__("Masz za malo respektu! Wymagane {b}35{/b}!"));
  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  GivePlayerScore(playerid,-35, "/rinvisible");
  pTemp[playerid][playerColorAlpha]=0;
  pTemp[playerid][tmpAlpha]=0;
  SetPlayerProperColor(playerid);

  Msg(playerid,COLOR_INFO,__("Kupiles {b}niewidzialnosc{/b} za {b}35 punktow{/b} respektu."));
  return 1;
}

CMD:rdualso(playerid){  // 30
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Musisz byc zarejestrowanym graczem, aby korzystac z /rcmd. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac."));
  if (pData[playerid][respect]<30)
    return Msg(playerid,COLOR_ERROR,__("Masz za malo respektu! Wymagane {b}30{/b}!"));

  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  GivePlayerScore(playerid,-30, "/rdualso");
  SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 999);
  pTemp[playerid][weaponSkill_sawnoff]=999;

  GivePlayerWeapon(playerid, 26, 2000);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  Msg(playerid,COLOR_INFO,__("Kupiles {b}mozliwosc strzelania oburacz z dualso{/b} za {b}30 punktow{/b} respektu."));
  return 1;
}

CMD:rdualuzi(playerid){ // 30
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Musisz byc zarejestrowanym graczem, aby korzystac z /rcmd. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac."));
  if (pData[playerid][respect]<30)
    return Msg(playerid,COLOR_ERROR,__("Masz za malo respektu! Wymagane {b}30{/b}!"));

  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  GivePlayerScore(playerid,-30, "/rdualuzi");
  SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 999);
  pTemp[playerid][weaponSkill_uzi]=999;

  GivePlayerWeapon(playerid,32, 2000);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  Msg(playerid,COLOR_INFO,__("Kupiles {b}mozliwosc strzelania oburacz z UZI{/b} za {b}30 punktow{/b} respektu."));
  return 1;
}


CMD:rdualpistols(playerid){ // 30
  MUST_BE_SPAWNED(playerid);
  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,__("Musisz byc zarejestrowanym graczem, aby korzystac z /rcmd. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac."));
  if (pData[playerid][respect]<30)
    return Msg(playerid,COLOR_ERROR,__("Masz za malo respektu! Wymagane {b}30{/b}!"));

  if(!TeleportAllowed(playerid) || !AttractionAllowed(playerid) || pTemp[playerid][godMode])
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz teraz tego zrobic."));

  GivePlayerScore(playerid,-30, "/rdualpistols");
  SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL_SILENCED, 999);
  SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 999);
  pTemp[playerid][weaponSkill_silenced]=999;
  pTemp[playerid][weaponSkill_pistol]=999;

  GivePlayerWeapon(playerid,22+random(1),1000);

    new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);

  Msg(playerid,COLOR_INFO,__("Kupiles {b}mozliwosc strzelania oburacz z pistoletow{/b} za {b}30 punktow{/b} respektu."));
  return 1;
}

CMD:id(playerid,params[]){
  if (isnull(params))
    return Msg(playerid,COLOR_ERROR,__("Uzyj: {b}/id <id/fragment nicku>{/b}"));
  new buffer[128];
  new targetplayerid = FindPlayer(params);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    new lista[1024];
    new znaleziono=0;
    foreach(i)
      if(FindString(GetPlayerNick(i), params))  // bylo GetPlayerProperName
        if (znaleziono++<10)
          format(lista,sizeof lista,"%s%s{ffffff}%d\t{%06x}%s", lista, strlen(lista)>0?("\n"):(""), i, pData[i][currentColor],GetPlayerNick(i));
    if (znaleziono>=10)
      format(lista,sizeof lista,"%s\n{ffffff}+%d %s", lista, (znaleziono-9), dli(znaleziono-9, "gracz","graczy","graczy"));
    ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_LIST, "Znalezieni gracze:", lista, "Zamknij","");

    return 1;
  }
  new buf[128];
  if(pTemp[targetplayerid][fakeAFK] || pTemp[targetplayerid][staleTime]>=60) format(buf, sizeof buf, __("Znaleziono: {b}%s{/b}, ID: {b}%d{/b}, AFK od {b}%d{/b}m"), GetPlayerNick(targetplayerid), targetplayerid, pTemp[targetplayerid][staleTime]/60);
  else format(buf, sizeof buf, __("Znaleziono: {b}%s{/b}, ID: {b}%d{/b}"), GetPlayerNick(targetplayerid), targetplayerid);
  return Msg(playerid,COLOR_INFO, buf);
}

CMD:pmoff(playerid){
  pTemp[playerid][accepts][eac_pm]=ACCEPTS_NONE;
  return Msg(playerid,COLOR_INFO,__("Nie akceptujesz {b}zadnych /pm{/b}."));
}

CMD:pmon(playerid){
  pTemp[playerid][accepts][eac_pm]=ACCEPTS_ALL;
  return Msg(playerid,COLOR_INFO,__("Przyjmujesz {b}wszystkie /pm{/b}."));
}

CMD:tunemenu(playerid){
    if (pData[playerid][pAttraction]!=A_NONE &&  pData[playerid][pAttraction]!=A_ARENA)
        return Msg(playerid,COLOR_ERROR,__("Nie mozesz tego teraz zrobic!"));

  return vehicles_TunemenuShow(playerid);
}

CMD:fps(playerid){
  pData[playerid][hudSetting][HUD_FPS]=!pData[playerid][hudSetting][HUD_FPS];
  ShowPlayerHudElement(playerid,HUD_FPS,pData[playerid][hudSetting][HUD_FPS]);
  return 1;
}

CMD:skok(playerid,params[]){
  MUST_BE_SPAWNED(playerid);
  if (!TeleportAllowed(playerid) || GetPlayerVirtualWorld(playerid)==69)
    return Msg(playerid,COLOR_ERROR,__("Nie mozesz tego teraz zrobic."));

  if (GetPlayerInterior(playerid)>0)
    return Msg(playerid,COLOR_ERROR,__("Komendy {b}/skok{/b} mozna uzywac tylko na zewnatrz!"));

    if (GetPlayerVirtualWorld(playerid)==VW_JAIL)
    return Msg(playerid,COLOR_ERROR,__("Komendy {b}/skok{/b} mozna uzywac tylko na zewnatrz!"));

  new wysokosc;
  if (sscanf(params,"d",wysokosc))
    return Msg(playerid, COLOR_ERROR, __("Uzyj: {b}/skok <wysokosc:200-1500>{/b}"));

  if (wysokosc<200 || wysokosc>1500)
    return Msg(playerid, COLOR_ERROR, __("Podaj wartosc z zakresu {b}200-1500{/b}."));

  new Float:pP[3];
  GivePlayerWeapon(playerid,46,1);
  GetPlayerPos(playerid,pP[0],pP[1],pP[2]);
  SetPlayerPos(playerid,pP[0]+random(10)-5,pP[1]+random(10)-5,pP[2]+wysokosc+random(50)-25);
  return 1;
}

CMD:odlicz(playerid){
  new buf[128];
  MUST_BE_SPAWNED(playerid);
  if (GetTickCount()-gmTemp[odliczLast]<30000)
    return Msg(playerid,COLOR_ERROR,"Musi minac przynajmniej {b}30 sekund{/b} od ostatniego odliczania.");
  gmTemp[odliczLast]=GetTickCount();

  Odliczanie(5,0,0,playerid);
  if (SkinKobiecy(GetPlayerSkin(playerid)))
    format(buf, sizeof buf, "{b}%s (%d){/b} uruchomila odliczanie.", GetPlayerProperName(playerid), playerid);
  else
    format(buf, sizeof buf, "{b}%s (%d){/b} uruchomil odliczanie.", GetPlayerProperName(playerid), playerid);

  foreach(i)
    if (pData[i][pAttraction]==A_NONE)
      Msg(i,COLOR_INFO, buf, false);

  return 1;
}

CMD:st(playerid){
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  new buf[128],Float:hp,Float:ar;
  GetPlayerHealth(playerid,hp);
  GetPlayerArmour(playerid,ar);
  format(buf, sizeof buf, "{b}%s{/b}({b}%d{/b}) - HP: {b}%03.1f{/b}, AR: {b}%03.1f{/b}.", GetPlayerNick(playerid), playerid, hp, ar);
  Msg(playerid,COLOR_INFO,buf,false);
  if (pTemp[playerid][killStreak]>1) {
    format(buf, sizeof buf, "Zabi%s {b}%d %s{/b} od ostatniego spawnu.",
    (SkinKobiecy(GetPlayerSkin(playerid)))?("las"):("les"), pTemp[playerid][killStreak], dli(pTemp[playerid][killStreak], "przeciwnika", "przeciwnikow", "przeciwnikow"));
    Msg(playerid,COLOR_INFO,buf,false);
  }
  return 1;

}

CMD:regulamin(playerid)
  return ShowPlayerDialog(playerid, DIALOG_HELP_RULES, DIALOG_STYLE_MSGBOX, TXT(playerid, 80), GetDialogContent(DIALOG_HELP_RULES), __("Wyjdz"), __("Wroc"));

CMD:stylwalki(playerid){
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  return ShowPlayerDialog(playerid, DIALOG_STYLWALKI, DIALOG_STYLE_LIST, "Wybierz swoj styl walki!", "Normalny \n Karate \n Boxer \n Gangster \n Styl pijanej malpy", "Wybieram", "Anuluj");
}

CMD:rozbroj(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
    ResetPlayerWeapons(playerid);
  if (Kobieta(playerid))
    Msg(playerid, COLOR_INFO, "Zostalas rozbrojona.");
  else
    Msg(playerid, COLOR_INFO, "Zostales rozbrojony.");
  return 1;
}

CMD:audio(playerid){
  if (pTemp[playerid][playPlayall]) {
    Msg(playerid, COLOR_INFO,__("{b}Wylaczono{/b} odtwarzanie dzwiekow serwera"));
    pTemp[playerid][playPlayall]=false;
  } else {
    Msg(playerid, COLOR_INFO,__("{b}Wlaczono{/b} odtwarzanie dzwiekow serwera"));
    pTemp[playerid][playPlayall]=true;
  }
  Audio_TransferPack(playerid);
  return 1;
}

CMD:podloz(playerid)
{
  if(!gmTemp[isBombs]) return Msg(playerid,COLOR_ERROR,"Mozliwosc podkladania bomb jest wylaczona!");
  if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if(GetPlayerScore(playerid) < 1000 && !pData[playerid][vipEnabled] && !IsGM(playerid))
    return Msg(playerid,COLOR_ERROR,"Musisz miec przynajmiej 1000 respektu aby podlozyc bobme!");

  if(!IsPlayerInFullDMArea(playerid)) return Msg(playerid,COLOR_ERROR,"Bomby mozna podkladac tylko w strefie smierci!");
  if(pTemp[playerid][bombId]!=-1) return Msg(playerid,COLOR_ERROR,"Podlozyles juz jedna bombe");

  GetPlayerPos(playerid,pTemp[playerid][bombPos][0], pTemp[playerid][bombPos][1], pTemp[playerid][bombPos][2]);
  pTemp[playerid][bombId]=CreatePickup(1252,1,pTemp[playerid][bombPos][0], pTemp[playerid][bombPos][1], pTemp[playerid][bombPos][2]);
  Msg(playerid,COLOR_INFO,"Podlozyles bombe! Aby ja zdetonowac uzyj /zdetonuj");
  pTemp[playerid][bombTimer]=SetTimerEx("clearPlayerBombs",420000,false,"d",playerid);
  return 1;
}

CMD:zdetonuj(playerid)
{
    if(!gmTemp[isBombs]) return Msg(playerid,COLOR_ERROR,"Mozliwosc podkladania bomb jest wylaczona!");
    if(pTemp[playerid][godMode]) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

    if(pTemp[playerid][bombId]==-1) return Msg(playerid,COLOR_ERROR,"Nie podlozyles jescze zadnej bomby");
    if(!IsPlayerInFullDMArea(playerid)) return Msg(playerid,COLOR_ERROR,"Bomby mozna detonowac tylko w strefie smierci!");

  Msg(playerid,COLOR_INFO,"Zdetonowales bombe!");
  CreateExplosion(pTemp[playerid][bombPos][0], pTemp[playerid][bombPos][1], pTemp[playerid][bombPos][2], 6, 20.0);
  DestroyPickup(pTemp[playerid][bombId]);
  pTemp[playerid][bombPos][0]=-1;
  pTemp[playerid][bombPos][1]=-1;
  pTemp[playerid][bombPos][2]=-1;
  pTemp[playerid][bombId]=-1;
  return 1;
}

CMD:net(playerid){
  new netstat[1023],Float:ploss,dup;
  Msg(playerid,COLOR_INFO2,"Twoje statystyki polaczenia z serwerem",false);
  format(netstat,sizeof netstat,"IP: %s. PING aktualny: {b}%d{/b}ms, sredni: {b}%d{/b}ms", GetPlayerIP(playerid), GetPlayerPing(playerid), pData[playerid][averagePing]);
  Msg(playerid,COLOR_INFO,netstat,false);


  GetPlayerNetworkStats(playerid,netstat,sizeof netstat);

  if (sscanf(netstat,"p<%>'Packetloss: 'f'Duplicate acks received: 'p<\n>d",ploss,dup))
    return Msg(playerid,COLOR_ERROR,"Nie udalo sie pobrac statystyk");



  format(netstat,sizeof netstat,"Utrata pakietow: {b}%.1f%%{/b}, duplikaty: {b}%d{/b}.",ploss,dup);
  Msg(playerid,COLOR_INFO,netstat,false);

  return 1;
}

CMD:rampy(playerid){
  if(pTemp[playerid][rampy]){
      pTemp[playerid][rampy]=false;
      Msg(playerid,COLOR_INFO,"{b}Wylaczyles{/b} rampy");
  }else{
      pTemp[playerid][rampy]=true;
      Msg(playerid,COLOR_INFO,"{b}Wlaczyles{/b} rampy aktywowane pod {b}LCTRL{/b}");
  }
  return 1;
}

CMD:frsp(playerid){
  if(pTemp[playerid][fRsp]){
      pTemp[playerid][fRsp]=false;
      Msg(playerid,COLOR_INFO,"{b}Wylaczyles{/b} szybki respawn");
  }else{
      pTemp[playerid][fRsp]=true;
      Msg(playerid,COLOR_INFO,"{b}Wlaczyles{/b} szybki respawn");
  }
  return 1;
}

CMD:rejestracja(playerid){
  if(pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 0)); // Jeste? ju¿ zarejestrowany.
  if(PlayerAccountExists(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 1)); // Twój nick jest ju¿ zarejestrowany.
  ShowPlayerDialog(playerid, DIALOG_HELP_RULES_REGISTER, DIALOG_STYLE_MSGBOX, TXT(playerid, 80), GetDialogContent(DIALOG_HELP_RULES), __("Akceptuj"), __("Wyjdz"));
  return 1;
}

CMD:gong(playerid){
  if(pTemp[playerid][gongAll]){
    Msg(playerid,COLOR_INFO2,"Gong obrazenia aktywny {b}tylko na arenach{/b}");
    pTemp[playerid][gongAll] = false;
  }else{
    Msg(playerid,COLOR_INFO2,"Gong obrazenia aktywny {b}na calej mapie{/b}");
    pTemp[playerid][gongAll] = true;
  }
  return 1;
}

CMD:nowosci(playerid) { //staramy sie zachowywac limit 30 nowosci
    new s[3850];
    strcat(s,"{FFFFFF}- Usunieto problem z denerwujacym napisem 'Poszukiwani przestepcy' widocznym przez sciany na /ONEDE - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Dodano dzwieki do KillStreak i HeadShota (zgloszenie #000329), wymaga AudioPlugina - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Komenda /audio wylacza streaming z AudioPlugina (alias: /setplay) - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Komenda /raudio przeladowywuje Audio Plugina w razie problemow - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Dodano gong na calej mapie pod komenda /gong - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Poprawiono obsluge domkow ktore mozna zakupic wylacznie poprzez GamePoints - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Dodano 10 pojazdow ktore beda mialy neony (zawsze losowe po restarcie serwera) - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Usunieto blad z zablokowanymi komendami /cmd /komendy /teleporty (zgloszenie #000341) - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Niezniszczalny pojazd w trybie pasywnym (zgloszenie #000144) - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Zwiekszone nagrody rozdawane przez artefakt (zgloszenie #000338) - {A30B0B}23.03.2014\n");
    strcat(s,"{FFFFFF}- Usuniete denerwujace krzesla na /ONEDE i /ONEGUN - {A30B0B}24.03.2014\n");
    strcat(s,"{FFFFFF}- Usunieto problem z samo-kupujacymi sie biznesami - {A30B0B}24.03.2014\n");
    strcat(s,"{FFFFFF}- Poprawione resetowanie statystyk gangowych i rekordow co miesiac - {A30B0B}24.03.2014\n");
    strcat(s,"{FFFFFF}- Dodano unikalny system zapisu czasu gry i biznesow przy crashu (dane przechowywane przez 2 minuty) - {A30B0B}24.03.2014\n");
    strcat(s,"{FFFFFF}- Dodano komende /driftingexit jako alias do /driftexit - {A30B0B}25.03.2014\n");
    strcat(s,"{FFFFFF}- Dodano nowa atrakcje /GUNGAME (max 15 osob) - {A30B0B}25.03.2014\n");
    strcat(s,"{FFFFFF}- Zwiekszono limit czasu dla zachowania informacji po crashu do 5 minut - {A30B0B}25.03.2014\n");
    strcat(s,"{FFFFFF}- Kasa za godzinny bonus gry zalezy teraz od ilosci przegranych godzin - {A30B0B}25.03.2014\n");
    strcat(s,"{FFFFFF}- Mozliwosc podania bezposrednio id areny i broni w komendzie /solo (zgloszenie #000300) - {A30B0B}26.03.2014\n");
    strcat(s,"{FFFFFF}- Usunieto blad z usuwaniem kastetu po /rsp (zgloszenie #000375) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Tanszy tryb furii dla VIPow (zgloszenie #000376) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Quiz dziala w godzinach 15 do 16, od 17 do 18 i od 20 do 21 (zgloszenie #000374) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Naprawiony blad z flaga na /CTF (zgloszenie #000364) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Poprawiony blad z wiezieniem i /kill na trybie pasywnym (zgloszenie #000365) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Automatyczny respawn po wylaczeniu trybu pasywnego (zgloszenie #000366) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Mozna zapisac holownik pod domem (zgloszenie #000350) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Kuloodoporne czo³gi z wiêkszym HP i niesmiertelnym graczem w srodku (zgloszenie #000105) - {A30B0B}01.04.2014\n");
    strcat(s,"{FFFFFF}- Poprawiony wybuch przy /NOS w motorach (zgloszenie #000379) - {A30B0B}04.04.2014\n");
    strcat(s,"{FFFFFF}- Dodano bonus na /RACE i /DRIFTING za brak zniszczen (zgloszenie #000378) - {A30B0B}04.04.2014\n");
    strcat(s,"{FFFFFF}- Dodano komende /FRAKCJA do sprawdzania listy osob w danej frakcji (zgloszenie #000395) - {A30B0B}08.04.2014\n");
  ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "{FFFFFF}Ostatnie {FFFF00}30{FFFFFF} nowosci {FFFF00}FullServer'a{FFFFFF}", s, "Zamknij", "");
  return 1;
}

// teleports

CMD:tir1(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  Teleport(T_BOTH, playerid, 667.6860+random(5)-2.5, 1247.6492+random(5)-2.5, 11.4534, 204.1839, 0, VW_DEFAULT);

  return 1;
}

CMD:marynarka(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_marynarka);
    Teleport(T_PLAYER, playerid, spawns_marynarka[r][0]+random(3)-1.5, spawns_marynarka[r][1]+random(3)-1.5, spawns_marynarka[r][2], spawns_marynarka[r][3]+random(10)-5,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      new r=random(sizeof spawns_marynarka);
      Teleport(T_BOTH_PV, playerid, spawns_marynarka[r][0]+random(200)-100, spawns_marynarka[r][1]+random(200)-100, spawns_marynarka[r][2]+100+random(100), FLOAT_NAN,0, VW_DEFAULT);
    }else{
        new r=random(sizeof spawns_marynarka);
      Teleport(T_PLAYER, playerid, spawns_marynarka[r][0]+random(3)-1.5, spawns_marynarka[r][1]+random(3)-1.5, spawns_marynarka[r][2], spawns_marynarka[r][3]+random(10)-5,0, VW_DEFAULT);
    }
  }
  GameTextForPlayer(playerid, "~r~marynarka ~h~wojskowa",1000,1);

  return 1;
}

CMD:vip(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if (!pData[playerid][vipEnabled] && pData[playerid][adminLevel]<1)
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz uprawnien {b}VIP{/b}. Aby je uzyskac, odwiedz {b}http://fullserver.eu/doladowania/{/b}");


  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid,-493.69+random(5)-2.5,2611.57+random(5)-2.5, 53.77,136.0,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Teleport(T_BOTH_PV, playerid,-493.69+random(30)-15, 2611.57+random(30)-15, 53.77+70+random(50), FLOAT_NAN, 0,VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac! Najblizszy teleport wodny: {b}/tama{/b}.");
    } else {
      Teleport(T_BOTH, playerid,-375.0+random(5)-2.5, 2588.19+random(5)-2.5, 40.35,82.45,0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~y~Baza ~h~VIP", 1000, 1);

  return 1;
}

CMD:dilerzy(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2010.00+random(5)-2.5, 1055.31+random(5)-2.5, 55.57, 242.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~r~Siedziba ~h~dilerow", 1000, 1);

  return 1;
}

CMD:tama(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, -912.1114+random(2)-1, 2005.2952+random(2)-1, 60.4851, 274.0, 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Teleport(T_BOTH_PV, playerid, -912.1114+random(50)-25, 2005.2952+random(50)-25, 60.4851+70+random(5), FLOAT_NAN, 0, VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      Teleport(T_BOTH_PV, playerid, -873.36+random(20)-10, 2116.19+random(20)-10, 40.12, FLOAT_NAN, 0, VW_DEFAULT);
    } else {
      Teleport(T_BOTH, playerid, -912.1114+random(2)-1, 2005.2952+random(2)-1, 60.4851, 274.0, 0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~Tama", 1000, 1);

  return 1;
}

CMD:tir(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz baze transportowa","{3481B9}/TIR1 \t{E3C759}pustynna LV\n{3481B9}/TIR2 \t{90ff90}lesna\n{3481B9}/TIRRC \t{ff6090}Red County\n{3481B9}/TIRLV \t{ff9060}Rockshore LV\n{3481B9}/TIROD \t{9060ff}Ocean Docks LS","OK","Anuluj");
  return 1;
}

new
  Float:spawns_fc_road[][4]={
    {-187.80,1137.61,19.31,177.50},
    {-20.93,1101.49,19.32,89.74},
    {-13.57,1047.24,19.32,358.99},
    {86.15,1200.99,18.27,89.73}
  },
  Float:spawns_fc_foot[][4]={
    {-182.92,1090.33,19.74,38.66},
    {-204.42,1090.78,19.74,318.13}
  };

CMD:fc(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_fc_foot);
    Teleport(T_PLAYER, playerid, spawns_fc_foot[r][0]+random(1)-0.5, spawns_fc_foot[r][1]+random(1)-0.5, spawns_fc_foot[r][2], spawns_fc_foot[r][3]+random(10)-5,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,"Pojazdy wojskowe nie moga sie tam teleportowac! Najblizszy teleport: {b}/wojsko{/b}");
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      new r=random(sizeof spawns_fc_foot);
      Teleport(T_BOTH_PV, playerid, spawns_fc_foot[r][0]+random(200)-100, spawns_fc_foot[r][1]+random(200)-100, spawns_fc_foot[r][2]+150+random(150), FLOAT_NAN,0, VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      Teleport(T_BOTH, playerid, -458.80+random(10)-5, 1136.96+random(10)-5, 0+random(1), 287+random(10)-5,0, VW_DEFAULT);
    } else {
      new r=random(sizeof spawns_fc_road);
      Teleport(T_BOTH, playerid, spawns_fc_road[r][0]+random(2)-1, spawns_fc_road[r][1]+random(2)-1, spawns_fc_road[r][2], spawns_fc_road[r][3]+random(5)-2.5,0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~Fort ~h~Carson",1000,1);
  return 1;
}

new
  Float:spawns_warsztatlv_road[][4]={
    {1107.21,1829.29,10.57,107.72},
    {1049.79,1830.32,10.59,234.02},
    {1031.00,1811.66,10.44,269.42},
    {1066.43,1836.77,10.59,182.25}
  },
  Float:spawns_warsztatlv_foot[][4]={
    {1076.78,1832.24,10.82,166.12},
    {1055.35,1831.49,10.82,197.76}
  };

CMD:warsztatlv(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_warsztatlv_foot);
    Teleport(T_PLAYER, playerid, spawns_warsztatlv_foot[r][0]+random(1)-0.5, spawns_warsztatlv_foot[r][1]+random(1)-0.5, spawns_warsztatlv_foot[r][2], spawns_warsztatlv_foot[r][3]+random(10)-5,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,"Pojazdy wojskowe nie moga sie tam teleportowac! Najblizszy teleport: {b}/wojsko{/b}");
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Msg(playerid,COLOR_INFO,"Zostajesz skierowany na pobliskie lotnisko.", false);
      return _lvlot(playerid);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    } else {
      new r=random(sizeof spawns_warsztatlv_road);
      Teleport(T_BOTH, playerid, spawns_warsztatlv_road[r][0]+random(2)-1, spawns_warsztatlv_road[r][1]+random(2)-1, spawns_warsztatlv_road[r][2], spawns_warsztatlv_road[r][3]+random(5)-2.5,0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~Warsztat ~h~LV",1000,1);
  return 1;
}

CMD:tir2(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, -470, -506+random(6), 25.51, 25+random(5)-2.5, 0,VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Msg(playerid,COLOR_INFO,"Zostajesz skierowany na pobliskie lotnisko.", false);
      return _sflot(playerid);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    } else {
      Teleport(T_BOTH, playerid, -491+random(3)-1.5, -501+random(3)-1.5, 25.6, random(360),0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~g~Lesna~h~ baza transportowa", 1000, 1);

  return 1;
}

CMD:tirlv(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid,2791+random(5)-2.5, 934.46+random(5)-2.5, 10.75, 260+random(50)-25, 0,VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Msg(playerid,COLOR_INFO,"Zostajesz skierowany na pobliskie lotnisko.", false);
      return _sflot(playerid);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    } else {
      Teleport(T_BOTH, playerid, 2818+random(6)-3, 954+random(6)-3, 10.75, random(360),0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~r~~h~Baza ~r~Rockshore ~h~LV", 1000, 1);

  return 1;
}

CMD:tirrc(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid,-232.81+random(5)-2.5, -255.93+random(5)-2.5, 1.42, 245+random(50)-25, 0,VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Msg(playerid,COLOR_INFO,"Zostajesz skierowany na pobliskie lotnisko.", false);
      return _sflot(playerid);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    } else {
      Teleport(T_BOTH, playerid, -122+random(6)-3, -374+random(6)-3, 1.42, random(360),0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~r~~h~Baza ~r~Red County", 1000, 1);

  return 1;
}

CMD:tirod(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid,2202+random(5)-2.5, -2297.90+random(5)-2.5, 14.76, 106+random(50)-25, 0,VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Msg(playerid,COLOR_INFO,"Zostajesz skierowany na pobliskie lotnisko.", false);
      return _sflot(playerid);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    } else {
      Teleport(T_BOTH, playerid, 2175+random(6)-3, -2303+random(6)-3, 13.54, random(360),0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~~h~Baza ~b~Ocean Docks ~b~~h~LS", 1000, 1);

  return 1;
}

CMD:ksw(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid))
    return Msg(playerid, COLOR_ERROR, "Najpierw wysiadz z pojazdu");

  Teleport(T_PLAYER, playerid, 1301.32, -809.33, 84.14, 6.37,0, VW_DEFAULT);

  GameTextForPlayer(playerid, "~r~KSW ~y~ZTP ~r~~h~WwK", 1000, 1);

  return 1;
}

CMD:ksw2(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (IsPlayerInAnyVehicle(playerid))
    return Msg(playerid, COLOR_ERROR, "Najpierw wysiadz z pojazdu");

  Teleport(T_PLAYER, playerid, -2267.46, -153.38, 35.32, 133.13,0, VW_DEFAULT);

  GameTextForPlayer(playerid, "~r~KSW~g~2", 1000, 1);

  return 1;
}

CMD:ksw3(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 692.32, 1471.89, 848.54, 133.13,6, VW_DEFAULT);

  GameTextForPlayer(playerid, "~r~KSW~g~3", 1000, 1);

  return 1;
}

CMD:stunt(playerid) {
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz teleport:","{FFD700}/SKATEPARK","OK","Anuluj");
  return 1;
}

CMD:stuntpark(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  Teleport(T_PLAYER, playerid,-3701.59+random(3)-1.5, -1299.35+random(3)-1.5, 18.14+5, 83+random(20)-10, 0,VW_DEFAULT);

  GameTextForPlayer(playerid, "~y~~h~Stunt~y~park", 1000, 1);

  return 1;
}

CMD:pd(playerid){
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz komisariat","{3481B9}/LSPD \t{ffffff}Los Santos \n{3481B9}/LVPD \t{ffffff}Las Venturas \n{3481B9}/SFPD \t{ffffff}San Fierro","OK","Anuluj");
  return 1;
}

CMD:lspd(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, 1517.7+random(5)-2.5, -1664.88+random(5)-2.5, 13.88, 237.90,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Teleport(T_BOTH_PV, playerid,1517.7+random(30)-15, -1664.88+random(30)-15, 13.79+40+random(50), FLOAT_NAN, 0,VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac! Najblizszy teleport wodny: {b}/ls{/b}.");
    } else {
      Teleport(T_BOTH, playerid, 1578.6+random(5)-2.5, -1613.91+random(5)-2.5, 13.10, 88.45,0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~Komisariat w ~h~Los Santos", 1000, 1);

  return 1;
}

CMD:lvpd(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, 2297.37+random(5)-2.5, 2422.18+random(5)-2.5, 10.82, 39.60,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Teleport(T_BOTH_PV, playerid,2299.7+random(30)-15, 2432.88+random(30)-15, 10.79+50+random(50), FLOAT_NAN, 0,VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    } else {
      Teleport(T_BOTH, playerid, 2287.0+random(5)-2.5, 2412.41+random(5)-2.5, 10.87,266.45,0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~Komisariat w ~h~Las Venturas", 1000, 1);

  return 1;
}

CMD:sfpd(playerid)
{
  MUST_BE_SPAWNED(playerid);

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid,-1605.66+random(5)-2.5,721.14+random(5)-2.5, 11.98,180.0,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Teleport(T_BOTH_PV, playerid,-1605.7+random(30)-15, 730.88+random(30)-15, 11.79+70+random(50), FLOAT_NAN, 0,VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac! Najblizszy teleport wodny: {b}/portsf{/b}.");
    } else {
      Teleport(T_BOTH, playerid,-1620.0+random(5)-2.5, 726.41+random(5)-2.5, 14.35,268.45,0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~b~Komisariat w ~h~San Fierro", 1000, 1);

  return 1;
}

CMD:kino(playerid)
{
    if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  GameTextForPlayer(playerid, __("~y~kino"),1000,1);

  Teleport(T_PLAYER, playerid, 1013.45, -1122.21, 23.89, 181.02, 0, VW_DEFAULT);

  return 1;
}

CMD:oboz(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  GameTextForPlayer(playerid, __("~y~oboz ~r~koncentracyjny"),1000,1);

  Teleport(T_PLAYER, playerid, 2681.35, -4517.77, 15.19, 181.02, 0, VW_DEFAULT);

  return 1;
}

CMD:ruiny(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  GameTextForPlayer(playerid, __("~y~ruiny ~r~/LV"),1000,1);

  Teleport(T_PLAYER, playerid, 2111.96, 977.42, 3011.67, 33.41, 0, VW_DEFAULT);

  return 1;
}

CMD:wiezienie2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 940.64, -2374.23, 14.08, 117.16, 0, VW_DEFAULT);

  return 1;
}

CMD:trackstadium(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1403.01, -250.44, 1043.53, 355.86, 7, VW_DEFAULT);

  return 1;
}

CMD:rcbatte(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1129.89, 1057.54, 1346.41, 274.52, 10, VW_DEFAULT);

  return 1;
}

CMD:vcstadium(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1401.06, 1265.36, 1039.86, 178.64, 16, VW_DEFAULT);

  return 1;
}

CMD:ddw(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1379.45, 915.07, 1039.70, 302.34, 15, VW_DEFAULT);

  return 1;
}

CMD:angelpine(playerid)
{
    if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2169.9177, -2445.2344, 30.4648, 143.7730, 0, VW_DEFAULT);

  return 1;
}

CMD:hotel(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 952.87, -918.09, 44.54, 357.98, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Hotel", 1000, 1);

  return 1;
}

CMD:bayside(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2443.47, 2241.94, 4.84, 134.68, 0, VW_DEFAULT);

  return 1;
}

new
    Float:spawns_wyspa_foot[][4]={
        {3500.67, 387.10, 3.05, 332.00},
        {3492.39, 396.01, 2.87, 321.00},
        {3516.76, 385.51, 5.54, 11.0}
    };

CMD:wyspa(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid)==PLAYER_STATE_PASSENGER) {
    new rs=random(sizeof spawns_wyspa_foot);
    Teleport(T_PLAYER, playerid, spawns_wyspa_foot[rs][0]+random(1)-0.5, spawns_wyspa_foot[rs][1]+random(1)-0.5, spawns_wyspa_foot[rs][2], spawns_wyspa_foot[rs][3], 0, VW_DEFAULT,true);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      new rs=random(sizeof spawns_wyspa_foot);
      Teleport(T_BOTH_PV,playerid,spawns_wyspa_foot[rs][0]+random(1)-0.5, spawns_wyspa_foot[rs][1]+random(1)-0.5, spawns_wyspa_foot[rs][2]+100+random(50), FLOAT_NAN ,0,VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      Teleport(T_BOTH_PV, playerid,   3500.00+((random(30)+10)*4), 500.00+((random(30)+10)*4), 0+random(5), FLOAT_NAN );
    else {
      // todo zakaz dla drogowych
      Teleport(T_BOTH, playerid,  3509.25+random(10)-5, 500.00+random(10)-5, 8.04, FLOAT_NAN, 0, VW_DEFAULT,true);
    }
  }
  GameTextForPlayer(playerid, __("~y~Bezludna ~h~wyspa"),1000,1);
  return 1;
}

new
    Float:spawns_wyspas_foot[][4]={
        {3424.88,-2023.00, 3.76, 101.00},
        {3354.02,-2028.59, 1.66, 274.54},
        {3299.02,-1985.61, 4.71, 180.0}
    };

CMD:wyspas(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid)==PLAYER_STATE_PASSENGER) {
    new rs=random(sizeof spawns_wyspas_foot);
    Teleport(T_PLAYER, playerid, spawns_wyspas_foot[rs][0], spawns_wyspas_foot[rs][1], spawns_wyspas_foot[rs][2], spawns_wyspas_foot[rs][3], 0, VW_DEFAULT,true);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      new rs=random(sizeof spawns_wyspas_foot);
      Teleport(T_BOTH_PV, playerid, spawns_wyspas_foot[rs][0]+random(10), spawns_wyspas_foot[rs][1]+random(10), spawns_wyspas_foot[rs][2]+100+random(50), FLOAT_NAN, 0, VW_DEFAULT,true);

    } else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      Teleport(T_BOTH, playerid,  3265.15+random(5)-2.5,-2051.33+random(5)-2.5, 0, FLOAT_NAN );
    else {
      return Msg(playerid,COLOR_ERROR,__("Nie mozesz sie tam przeniesc {b}tym{/b} pojazdem ;-)"));
    }
  }
  GameTextForPlayer(playerid, __("~y~Wyspa ~h~skarbow"),1000,1);
  return 1;
}

CMD:osiedle1(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1431.2541, 2590.4101, 10.6718, 100.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Osiedle 1", 1000, 1);

  return 1;
}

CMD:osiedle2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1602.2232, 2733.3798, 10.6718, 270.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Osiedle 2", 1000, 1);

  return 1;
}

CMD:osiedle3(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1993.9541, 2743.2902, 10.6718, 270.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Osiedle 3", 1000, 1);

  return 1;
}

CMD:osiedle4(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1946.4294, 939.1718, 10.3921, 123.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Osiedle 4", 1000, 1);

  return 1;
}

CMD:osiedle5(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 2149.3955, 715.5645, 10.8304, 260.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Osiedle 5", 1000, 1);

  return 1;
}

CMD:g1(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 2264.2097, 1398.7369, 42.5924, 261.3, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Parking 1", 1000, 1);

  return 1;
}

CMD:g2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 2008.1485, 1732.1975, 18.9339, 60.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Parking 2", 1000, 1);

  return 1;
}

CMD:g3(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 2074.0437, 2416.875, 49.5233, 235.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Parking 3", 1000, 1);

  return 1;
}

CMD:g4(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 1700.6284, 1194.107, 34.7891, 0.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Parking 4", 1000, 1);

  return 1;
}

CMD:miasteczko(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -393.5246, 2280.4821, 40.7083, 190.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Miasteczko", 1000, 1);

  return 1;
}

CMD:impra(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  ResetPlayerWeapons(playerid);
  Teleport(T_PLAYER, playerid, 489.0614, -14.679, 1000.6796, 0.0, 17, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Dyskoteka", 1000, 1);

  return 1;
}

CMD:tunelv(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 2387.0808, 1016.9998, 10.5459, 0.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Tuning LV", 1000, 1);

  return 1;
}

CMD:molo(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid))
    Teleport(T_PLAYER, playerid, 2311+random(10)-5, 578+random(13)-7.5, 7.78, 180.0+random(15)-7.5, 0, VW_DEFAULT);
  else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);
    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      Teleport(T_BOTH_PV, playerid, 2290.3881+random(3)-1.5, 611.6699+random(3)-1.5, 10.5461, FLOAT_NAN, 0, VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      Teleport(T_BOTH, playerid, 2270+random(10)-5, 497+random(10)-5, 0+random(1), 318+random(10)-5,0, VW_DEFAULT);
    } else
      Teleport(T_BOTH, playerid, 2283.3881+random(7), 610+random(10), 10.82, 180.0+random(15)-7.5, 0, VW_DEFAULT);


  }
  GameTextForPlayer(playerid, "~y~Molo", 1000, 1);

  return 1;
}

CMD:zadupie(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -1383.3281, -1507.3011, 102.2328, 90.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Zadupie", 1000, 1);

  return 1;
}

CMD:sflot(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -1538.8636, -422.9143, 5.8516, 314.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Lotnisko SF", 1000, 1);

  return 1;
}

CMD:salon(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -1987.7372, 288.7828, 34.568, 260.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Salon", 1000, 1);

  return 1;
}

new
  Float:spawns_gs_foot[][4]={
    {2479.47, -1685.48, 13.50, 328.24},
    {2504.87, -1683.79, 13.54, 33.10},
    {2512.97, -1670.41, 13.49, 87.62},
    {2504.83, -1653.91, 13.59, 144.65},
    {2478.22, -1650.79, 13.46, 197.29}
  };

CMD:gs(playerid){
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    new rs=random(sizeof spawns_gs_foot);
    Teleport(T_PLAYER, playerid, spawns_gs_foot[rs][0]+random(1)-0.5, spawns_gs_foot[rs][1]+random(1)-0.5, spawns_gs_foot[rs][2], spawns_gs_foot[rs][3], 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,"Pojazdy wojskowe nie moga sie tam teleportowac!");
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      return Msg(playerid,COLOR_ERROR,"Pojazdy powietrzne nie moga sie tam teleportowac!");
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      return Msg(playerid,COLOR_ERROR,"Pojazdy wodne nie moga sie tam teleportowac!");
    else {
      Teleport(T_BOTH, playerid,  2490.84+random(40)-20, -1670.00+random(40)-20, 13.04, FLOAT_NAN );


    }
  }
  GameTextForPlayer(playerid, "~g~Grove ~h~Street",1000,1);
  return 1;
}

new
  Float:spawns_kosciol_foot[][4]={
    {-2710.37,-303.99,7.18,125.0},
    {-2740.84,-331.23,7.18,-35.0},
    {-2746.83,-320.16,7.18,-90.0},
    {-2725.07,-300.04,7.18,-190.0}
  },
  Float:spawns_kosciol_road[][4]={
    {-2741.90, -281.89, 6.77, 180.56},
    {-2744.36, -282.38, 6.77, 181.63},
    {-2749.29, -281.67, 6.77, 180.0},
    {-2754.42, -282.10, 6.77, 180.0},
    {-2759.11, -281.42, 6.77, 180.0},
    {-2763.82, -282.07, 6.78, 180.0},
    {-2766.16, -311.09, 6.77, 3.90}

  };

CMD:kosciol(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    new rs=random(sizeof spawns_kosciol_foot);
    Teleport(T_PLAYER, playerid, spawns_kosciol_foot[rs][0]+random(1)-0.5, spawns_kosciol_foot[rs][1]+random(1)-0.5, spawns_kosciol_foot[rs][2], spawns_kosciol_foot[rs][3], 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      return Msg(playerid,COLOR_ERROR,"Pojazdy powietrzne nie moga sie tam teleportowac!");
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      Teleport(T_BOTH, playerid,  -2710.28+random(3)-1.5, -468.35+random(3)-1.5,  3.0,355.16+random(5)-2.5);
    else {
      new rs=random(sizeof spawns_kosciol_road);
      Teleport(T_BOTH, playerid, spawns_kosciol_road[rs][0], spawns_kosciol_road[rs][1], spawns_kosciol_road[rs][2], spawns_kosciol_road[rs][3], 0, VW_DEFAULT);

    }
  }
  GameTextForPlayer(playerid, "~y~Kosciol", 1000, 1);
  return 1;
}

new
  Float:spawns_kosciol2_foot[][4]={
    {2480.6,938.66,11.02,192.05},
    {2460.1,936.9,11.03,248.76},
    {2460.18,913.37,11.02,287.93},
    {2461.04,906.47,11.03,301.40}
  },
  Float:spawns_kosciol2_road[][4]={
    {2468.89,914.59,10.37,301.40},
    {2428.01,944.59,10.38,239.40},
    {2478.72,977.41,10.38,176.85}
  };

CMD:kosciol2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    new rs=random(sizeof spawns_kosciol2_foot);
    Teleport(T_PLAYER, playerid, spawns_kosciol2_foot[rs][0]+random(1)-0.5, spawns_kosciol2_foot[rs][1]+random(1)-0.5, spawns_kosciol2_foot[rs][2], spawns_kosciol2_foot[rs][3], 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      return Msg(playerid,COLOR_ERROR,"Pojazdy powietrzne nie moga sie tam teleportowac!");
    else {
      new rs=random(sizeof spawns_kosciol2_road);
      Teleport(T_BOTH, playerid, spawns_kosciol2_road[rs][0], spawns_kosciol2_road[rs][1], spawns_kosciol2_road[rs][2], spawns_kosciol2_road[rs][3], 0, VW_DEFAULT);

    }
  }
  GameTextForPlayer(playerid, "~y~Parafia Las Venturas", 1000, 1);
  return 1;
}

CMD:tunesf(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2694.8189, 216.2326, 4.3564, 90.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Tuning SF", 1000, 1);

  return 1;
}

CMD:chiliad(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2321.1321, -1634.269, 483.8788, 0.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Gora Chiliad", 1000, 1);

  return 1;
}

CMD:zakochani(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -2647.1644, 1408.9735, 906.2733, 77.0, 3, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Night Club", 1000, 1);

  return 1;
}

CMD:plazasf(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2896.8655, 144.7969, 4.9552, 0.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Plaza SF", 1000, 1);

  return 1;
}

CMD:wiezowiec(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 1545.9459, -1353.565, 329.6513, 180.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Wiezowiec", 1000, 1);

  return 1;
}

CMD:skatepark(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 1874.03, -1386.2403, 13.7217, 250.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Skate Park", 1000, 1);

  return 1;
}

CMD:tunels(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 2645.1042, -2005.1769, 13.5594, 180.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Tuning LS", 1000, 1);

  return 1;
}

CMD:lslot(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 1953.5203, -2290.1131, 13.5468, 125.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Lotnisko LS", 1000, 1);

  return 1;
}

CMD:plaza(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 330.1647, -1798.5217, 4.7, 190.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Plaza LS", 1000, 1);

  return 1;
}

CMD:dd(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1405.4444, 946.1091, 1030.0839, 0.0, 15, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Destruction Derby", 1000, 1);

  return 1;
}

CMD:db(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1433.8196, -653.962, 1051.561, 0.0, 4, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Dirt Bike", 1000, 1);

  return 1;
}

CMD:kss(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1475.9512, 1640.5053, 1052.5312, 200.0, 14, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Kickstart Stadium", 1000, 1);

  return 1;
}

CMD:vc(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -1396.3194, 86.3535, 1032.4809, 50.0, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Vice Stadium", 1000, 1);

  return 1;
}

CMD:statek(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 2001.6911, 1544.4111, 13.5859, 0.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Statek", 1000, 1);

  return 1;
}


CMD:4smoki(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 2023.6054, 1008.2421, 10.3641, 270.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Kasyno 4 smokow", 1000, 1);

  return 1;
}

CMD:burdel(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 2016.3636,1105.3380,10.8203,40.1436, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Burdelik", 1000, 1);

  return 1;
}

CMD:skocznia(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 1573.97,-2943.96,293.06, 90.00, 0, VW_DEFAULT, true);

  return 1;
}

CMD:wh3(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1805.64, -1803.26, 52.56, 0.0, 0, 100);
  GameTextForPlayer(playerid, "~y~WH3", 1000, 1);

  return 1;
}

CMD:lvlot(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
    Teleport(T_PLAYER, playerid, 1686.3106+random(10)-5, 1609.5484+random(10)-5, 10.8203, 110.0, 0, VW_DEFAULT);
  else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRPLANE==VF_AIRPLANE)
      Teleport(T_BOTH_PV, playerid, 1495.0+random(20)-10,1648.00+random(20)-10,107.0+random(20-10), 0.0, 0, VW_DEFAULT);
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH, playerid, 1336.0+random(20)-10,1480.00+random(20)-10,40.0+random(20-10),-45.0, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
    else
      Teleport(T_BOTH, playerid, 1686.3106+random(10)-5, 1609.5484+random(10)-5, 10.8203, 110.0, 0, VW_DEFAULT);


  }
  GameTextForPlayer(playerid, "~y~Lotnisko LV", 1000, 1);

  return 1;
}

new
  Float:spawns_stlot_foot[][4]={
    {405.10, 2537.74, 16.54, 134.00},
    {430.08, 2534.51, 16.40, 122.00},
    {387.18, 2539.30, 16.53, 127.16}
  },
  Float:spawns_stlot_road[][4]={
    {472.36, 2485.48, 23.15, 0.0},
    {412.03, 2574.17, 15.91, 83.0},
    {389.50, 2579.31, 15.97, 182.05}
  };


CMD:stlot(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_stlot_foot);
    Teleport(T_PLAYER, playerid, spawns_stlot_foot[r][0]+random(2)-1, spawns_stlot_foot[r][1]+random(2)-1, spawns_stlot_foot[r][2], spawns_stlot_foot[r][3]+random(10)-5, 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,"Pojazdy wojskowe nie moga sie tam teleportowac! Najblizszy teleport wojskowy: {b}/wojsko{/b}.");
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid, 309.74+random(100)-50,2503+random(100)-50,16+100+random(75), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac! Najblizszy mozliwy teleport: {b}/tama{/b}.");
    else {
      new r=random(sizeof spawns_stlot_road);
      Teleport(T_BOTH, playerid, spawns_stlot_road[r][0]+random(1)-0.5, spawns_stlot_road[r][1]+random(1)-0.5, spawns_stlot_road[r][2], spawns_stlot_road[r][3]+random(2)-1, 0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, "~y~Stare ~h~lotnisko ", 1000, 1);

  return 1;
}

new
  Float:spawns_lv_foot[][4]={
    {2178.25, 997.89, 10.82, 67.00},
    {2100.56, 989.82, 10.82, 302.31},
    {2135.66, 1042.71, 10.81, 196.41}
  },
  Float:spawns_lv_road[][4]={
    {2162.48, 1009.77, 10.52, 89.66},
    {2141.83, 1009.89, 10.52, 89.66},
    {2132.12, 1028.96, 10.52, 271.27}
  };

CMD:lv(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    new rs=random(sizeof spawns_lv_foot);
    Teleport(T_PLAYER, playerid, spawns_lv_foot[rs][0]+random(2)-1, spawns_lv_foot[rs][1]+random(2)-1, spawns_lv_foot[rs][2], spawns_lv_foot[rs][3], 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid, 2132.12+random(100)-50,1028+random(100)-50,10+100+random(75), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      return Msg(playerid,COLOR_ERROR,"Pojazdy wodne nie moga sie tam teleportowac! Najblizszy teleport: {b}/molo{/b}.");
    else {
      new rs=random(sizeof spawns_lv_road);
      Teleport(T_BOTH, playerid, spawns_lv_road[rs][0], spawns_lv_road[rs][1], spawns_lv_road[rs][2], spawns_lv_road[rs][3], 0, VW_DEFAULT);
    }
  }

  GameTextForPlayer(playerid, "~y~Las ~h~Venturas", 1000, 1);
  return 1;
}

CMD:drift(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz teleport","{3481B9}/DRIFT1 \t{ffffff}Parking hotelowy LV\n{3481B9}/DRIFT2 \t{ffffff}baza transportowa na pustyni\n{3481B9}/DRIFT3 \t{ffffff}Wieza transmisyjna w San Fierro\n{3481B9}/DRIFT4 \t{ffffff}Whetstone\n{3481B9}/DRIFT5 \t{ffffff}Garaz w LV\n{3481B9}\t{ffffff}Kanion","OK","Anuluj");

  return 1;
}


CMD:drift1(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1962.7165, 1778.4757, 18.9337, 90.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Drift 1", 1000, 1);

  return 1;
}

CMD:drift2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -308.1578, 1527.1978, 74.92, 220.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Pustynna baza transportowa", 1000, 1);

  return 1;
}

CMD:drift3(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -2447.7688, -619.6808, 132.7393, 300.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Drift 3", 1000, 1);

  return 1;
}


CMD:drift4(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, -1260.3335, -1374.8666, 119.1694, 184.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Drift 4", 1000, 1);

  return 1;
}

CMD:drift5(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_BOTH, playerid, 2071.915, 2434.2551, 49.5233, 214.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Drift 5", 1000, 1);

  return 1;
}

CMD:lot(playerid){
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz lotnisko","{3481B9}/LVLOT \t{ffffff}Las Venturas \n{3481B9}/SFLOT \t{ffffff}San Fierro \n{3481B9}/LSLOT \t{ffffff}Los Santos\n{3481B9}/STLOT \t{ffffff}Stare lotnisko","OK","Anuluj");

  return 1;
}

CMD:ammo(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 302.2929, -143.1391, 1004.0625, 0.0, 7, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Ammunation", 1000, 1);

  return 1;
}

CMD:rcshop(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -2239.5711, 130.0223, 1035.414, 0.0, 6, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Sklep", 1000, 1);

  return 1;
}

CMD:cpn(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 666.233, -572.6985, 16.3358, 90.0, 0, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~CPN", 1000, 1);

  return 1;
}

CMD:calligula(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 2172.0036, 1620.7542, 999.9791, 215.0, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Kasyno Calligula", 1000, 1);

  return 1;
}

CMD:wooziebed(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -2158.72, 641.288, 1052.3817, 90.0, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Wooziebed", 1000, 1);

  return 1;
}

CMD:jaysdin(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 460.1, -88.4285, 999.5546, 90.0, 4, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Bar", 1000, 1);

  return 1;
}

CMD:woc(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 451.6644, -18.139, 1001.1328, 90.0, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Restauracja", 1000, 1);

  return 1;
}

CMD:tsdin(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 681.4749, -451.1511, -25.6173, 180.0, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Bar", 1000, 1);

  return 1;
}

CMD:wh(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1412.6398, -1.7876, 1000.9243, 90.0, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~WareHouse", 1000, 1);

  return 1;
}

CMD:wh2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 1302.5198, -1.7876, 1001.0283, 135.0, 18, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~WareHouse 2", 1000, 1);

  return 1;
}

CMD:resta(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 2324.4199, -1147.5401, 1050.71, 0.0, 12, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Restauracja", 1000, 1);

  return 1;
}

CMD:brothel(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 747.6088, 1438.713, 1102.9531, 0.0, 6, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Burdel", 1000, 1);

  return 1;
}

CMD:brothel2(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, 942.1719, -17.007, 1000.9296, 0.0, 3, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Burdel 2", 1000, 1);

  return 1;
}

CMD:libertycity(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  Teleport(T_PLAYER, playerid, -746.82, 508.10,1372.03, 159.10, 1, VW_DEFAULT);
  GameTextForPlayer(playerid, "~y~Liberty City", 1000, 1);

  return 1;
}

CMD:andromeda(playerid)
{
    if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
    ShowPlayerDialog(playerid,DIALOG_CMDSEL,DIALOG_STYLE_LIST,"Wybierz teleport:","{FFD700}/ANDROMEDALS\n{3481B9}/ANDROMEDALV\n{ffffff}/ANDROMEDASF","OK","Anuluj");
    return 1;
}

CMD:andromedalv(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  GivePlayerWeapon(playerid,46,1);
  Teleport(T_PLAYER, playerid, 2309.63, 1219.50, 1505.18, 2.0, 0, VW_DEFAULT,true);
  GameTextForPlayer(playerid, "~y~Andromeda LV", 1000, 1);

  return 1;
}

CMD:andromedals(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  GivePlayerWeapon(playerid,46,1);
  Teleport(T_PLAYER, playerid, 1109.49,-1481.31, 1504.87, 2.0, 0, VW_DEFAULT,true);
  GameTextForPlayer(playerid, "~y~Andromeda LS", 1000, 1);

  return 1;
}

CMD:andromedasf(playerid)
{
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  GivePlayerWeapon(playerid,46,1);
  Teleport(T_PLAYER, playerid,-1909.28, 819.75, 1505.12, 2.0, 0, VW_DEFAULT,true);
  GameTextForPlayer(playerid, "~y~Andromeda SF", 1000, 1);

  return 1;
}

CMD:savepos(playerid)
{
  if (!TeleportAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));

  new vw, int;
  vw=GetPlayerVirtualWorld(playerid);
  int=GetPlayerInterior(playerid);
  if (vw>0 || int>0)
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));
  if(IsPlayerInFullDMArea(playerid))
      return Msg(playerid,COLOR_ERROR,"Nie mozesz tworzyc teleportow w strefie smierci");

    format(telpos[autor],sizeof telpos[autor],"%s",GetPlayerNick(playerid));
  telpos[tpWorld]=vw;
  telpos[tpInterior]=int;
  telpos[tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;

  GetPlayerPos(playerid,telpos[tpX],telpos[tpY],telpos[tpZ]);
  return Msg(playerid,COLOR_INFO,TXT(playerid,453));
}

CMD:telpos(playerid)
{
    new vid=GetPlayerVehicleID(playerid);
  new vf=GetVehicleFlags(vid);
  if (!TeleportAllowed(playerid))
    Msg(playerid,COLOR_ERROR,"Nie mozesz sie tam teraz przeniesc!");
    else if (vf&VF_MILITARY==VF_MILITARY)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
  else if (vf&VF_AIRPLANE==VF_AIRPLANE)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
  else if (vf&VF_AIRBORNE==VF_AIRBORNE)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
  else if (vf&VF_NATATORIAL==VF_NATATORIAL)
    return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
  else if (telpos[tpX]==telpos[tpX] && telpos[tpX]!=-1)
    Teleport(( (telpos[tpWorld]>0  || telpos[tpInterior]>0) ?T_PLAYER:T_BOTH), playerid, telpos[tpX],telpos[tpY],telpos[tpZ], random(360), telpos[tpInterior], telpos[tpWorld], telpos[tpStreamed]);
  else
    Msg(playerid,COLOR_ERROR,"Nie ma zapisanego teleportu!");
  return 1;
}

CMD:evtp(playerid)
{
    new vid=GetPlayerVehicleID(playerid);
  new vf=GetVehicleFlags(vid);
  if (!TeleportAllowed(playerid))
    Msg(playerid,COLOR_ERROR,"Nie mozesz sie tam teraz przeniesc!");
  else if (vf&VF_MILITARY==VF_MILITARY)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
  else if (vf&VF_AIRPLANE==VF_AIRPLANE)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
  else if (vf&VF_AIRBORNE==VF_AIRBORNE)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
  else if (vf&VF_NATATORIAL==VF_NATATORIAL)
    return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
  else if (evtp[tpX]==evtp[tpX] && evtp[tpX]!=-1)
    Teleport( T_PLAYER, playerid, evtp[tpX],evtp[tpY],evtp[tpZ], random(360), evtp[tpInterior], evtp[tpWorld], evtp[tpStreamed]);
  else
    Msg(playerid,COLOR_ERROR,"Nie ma zapisanego teleportu!");
  return 1;
}

CMD:sp(playerid){
  if (!TeleportAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));
  if (IsPlayerInFullDMArea(playerid))
    return Msg(playerid,COLOR_ERROR,"Nie mozna zapisac pozycji w {b}strefie smierci{/b}.");

  new vw, int;
  vw=GetPlayerVirtualWorld(playerid);
  int=GetPlayerInterior(playerid);
  if (vw>0 || int>0)
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));

  privpos[playerid][tpWorld]=vw;
  privpos[playerid][tpInterior]=int;
  privpos[playerid][tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;
  if (IsPlayerInAnyVehicle(playerid))
    GetVehicleZAngle(GetPlayerVehicleID(playerid),privpos[playerid][tpAngle]);
  else
    GetPlayerFacingAngle(playerid,privpos[playerid][tpAngle]);

  GetPlayerPos(playerid,privpos[playerid][tpX],privpos[playerid][tpY],privpos[playerid][tpZ]);
  return Msg(playerid,COLOR_INFO,"Pozycja zapisana. Uzyj {b}/lp{/b} aby ja wczytac.");
}

CMD:lp(playerid)
{
    new vid=GetPlayerVehicleID(playerid);
  new vf=GetVehicleFlags(vid);
  if (!TeleportAllowed(playerid))
    Msg(playerid,COLOR_ERROR,"Nie mozesz sie tam teraz przeniesc!");
    else if (vf&VF_MILITARY==VF_MILITARY)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
  else if (vf&VF_AIRPLANE==VF_AIRPLANE)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
  else if (vf&VF_AIRBORNE==VF_AIRBORNE)
    return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
  else if (vf&VF_NATATORIAL==VF_NATATORIAL)
    return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
  else if (privpos[playerid][tpX]==privpos[playerid][tpX]) {
    if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
      Teleport( T_PLAYER, playerid, privpos[playerid][tpX],privpos[playerid][tpY],privpos[playerid][tpZ], privpos[playerid][tpAngle]+random(5)-2.5, privpos[playerid][tpInterior], privpos[playerid][tpWorld], privpos[playerid][tpStreamed]);
    else
      Teleport(( (privpos[playerid][tpWorld]>0  || privpos[playerid][tpInterior]>0) ?T_PLAYER:T_BOTH), playerid, privpos[playerid][tpX],privpos[playerid][tpY],privpos[playerid][tpZ], privpos[playerid][tpAngle]+random(5)-2.5, privpos[playerid][tpInterior], privpos[playerid][tpWorld], privpos[playerid][tpStreamed]);
  } else
    Msg(playerid,COLOR_ERROR,"Nie masz zapisanego prywatnego teleportu!");
  return 1;
}

CMD:sp2(playerid){
	if (!TeleportAllowed(playerid))
		return Msg(playerid,COLOR_ERROR,TXT(playerid,452));
	if (IsPlayerInFullDMArea(playerid))
		return Msg(playerid,COLOR_ERROR,"Nie mozna zapisac pozycji w {b}strefie smierci{/b}.");

	new vw, int;
	vw=GetPlayerVirtualWorld(playerid);
	int=GetPlayerInterior(playerid);
	if (vw>0 || int>0)
		return Msg(playerid,COLOR_ERROR,TXT(playerid,452));

	privpos2[playerid][tpWorld]=vw;
	privpos2[playerid][tpInterior]=int;
	privpos2[playerid][tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;
	if (IsPlayerInAnyVehicle(playerid))
		GetVehicleZAngle(GetPlayerVehicleID(playerid),privpos2[playerid][tpAngle]);
	else
		GetPlayerFacingAngle(playerid,privpos2[playerid][tpAngle]);

	GetPlayerPos(playerid,privpos2[playerid][tpX],privpos2[playerid][tpY],privpos2[playerid][tpZ]);
	return Msg(playerid,COLOR_INFO,"Pozycja zapisana. Uzyj {b}/lp2{/b} aby ja wczytac.");
}

CMD:lp2(playerid)
{
    new vid=GetPlayerVehicleID(playerid);
	new vf=GetVehicleFlags(vid);
	if (!TeleportAllowed(playerid))
		Msg(playerid,COLOR_ERROR,"Nie mozesz sie tam teraz przeniesc!");
    else if (vf&VF_MILITARY==VF_MILITARY)
		return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
	else if (vf&VF_AIRPLANE==VF_AIRPLANE)
		return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
	else if (vf&VF_AIRBORNE==VF_AIRBORNE)
		return Msg(playerid,COLOR_ERROR,__("Pojazdy latajace nie moga sie tam teleportowac!"));
	else if (vf&VF_NATATORIAL==VF_NATATORIAL)
		return Msg(playerid,COLOR_ERROR,"Pojazdy plywajace nie moga sie tam teleportowac!");
	else if (privpos2[playerid][tpX]==privpos2[playerid][tpX]) {
		if (GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
			Teleport( T_PLAYER, playerid, privpos2[playerid][tpX],privpos2[playerid][tpY],privpos2[playerid][tpZ], privpos2[playerid][tpAngle]+random(5)-2.5, privpos2[playerid][tpInterior], privpos2[playerid][tpWorld], privpos2[playerid][tpStreamed]);
		else 
			Teleport(( (privpos2[playerid][tpWorld]>0  || privpos2[playerid][tpInterior]>0) ?T_PLAYER:T_BOTH), playerid, privpos2[playerid][tpX],privpos2[playerid][tpY],privpos2[playerid][tpZ], privpos2[playerid][tpAngle]+random(5)-2.5, privpos2[playerid][tpInterior], privpos2[playerid][tpWorld], privpos2[playerid][tpStreamed]);
	} else
		Msg(playerid,COLOR_ERROR,"Nie masz zapisanego prywatnego teleportu!");
	return 1;
}

CMD:arenasolo(playerid){
  Teleport(T_PLAYER, playerid, -1139.87+random(8)-4, -1034.00+random(8)-4, 129.24, 310+random(10)-5, 9, VW_UNIQUE_ARENASOLO);
  return 1;
}

CMD:uboot(playerid){
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, -1856.76,  1423.66, 7.17, random(10)+10,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid, -1856.76+random(20)-10,   1423.66+random(20)-10, 107.17+random(10), FLOAT_NAN,0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      Teleport(T_BOTH_PV, playerid, -1891.76+random(20)-10,   1551.74+random(20)-10, 0, FLOAT_NAN,0, VW_DEFAULT);
    else
      Teleport(T_BOTH, playerid,  -1856.10+random(3)-1.5, 1394.14+random(3)-1.5, 7.18, random(10)+20, 0, VW_DEFAULT);

  }
  GameTextForPlayer(playerid, __("~b~Lodz podwodna"),1000,1);
  return 1;
}

CMD:pgr(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, 68.61+random(5)-2.5,   -194.11+random(5)-2.5, 1.60, 60+random(120), 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid, 68.61+random(100)-50,   -194.11+random(100)-50, 108.31+random(20), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wodne nie moga sie tam teleportowac!"));
    else
      Teleport(T_BOTH, playerid,  65+random(6)-3, -237+random(6)-3, 1.57, random(15), 0, VW_DEFAULT);
  }
  GameTextForPlayer(playerid, "~y~PGR", 1000, 1);
  return 1;
}

new
  Float:spawns_ls_water[][4]={  // TODO dodac wiecej
    {726.91, -1646.38, 0.0, 0.0},
    {729.97, -1704.71, 0.0, 0.0}
  },

  Float:spawns_ls_foot[][3]={
    {1116.80, -1748.71, 13.57},
    {1117.18, -1732.24, 13.66},
    {1065.77, -1785.00, 13.64},
    {1074.89, -1785.00, 13.64},
    {1084.30, -1785.00, 13.64},
    {1093.97, -1785.00, 13.64},
    {1083.84, -1795.64, 13.66}
  },
  Float:spawns_ls_road[][4]={
    {1098.64, -1754.94, 13.07,  90.0},
    {1098.64, -1760.76, 13.07,  90.0},
    {1098.64, -1766.80, 13.07,  90.0},
    {1098.64, -1772.80, 13.07,  90.0},

    {1062.28, -1772.80, 13.12,  270.0},
    {1062.28, -1766.80, 13.12,  270.0},
    {1062.28, -1760.76, 13.12,  270.0},
    {1062.28, -1754.94, 13.12,  270.0},

    {1081.47, -1760.71, 13.10,  90.0},
    {1081.47, -1769.65, 13.10,  270.0}
  };

CMD:ls(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  Msg(playerid,COLOR_INFO2,__("Szukasz {b}Groove Street{/b}? Skorzystaj z teleportu {b}/gs{/b}"));

  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_ls_foot);
    Teleport(T_PLAYER, playerid, spawns_ls_foot[r][0]+random(3)-1.5,spawns_ls_foot[r][1]+random(3)-1.5, spawns_ls_foot[r][2], random(360), 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)

      Teleport(T_BOTH_PV, playerid,1071+random(100)-50,-1768+random(100)-50, 113.31+random(50), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      new r=random(sizeof spawns_ls_water);
      Teleport(T_BOTH,playerid, spawns_ls_water[r][0],spawns_ls_water[r][1], spawns_ls_water[r][2], spawns_ls_water[r][3], 0, VW_DEFAULT);
      return Msg(playerid,COLOR_INFO,__("Zacumowales przy najblizszym porcie."));
    } else {
      new r=random(sizeof spawns_ls_road);
      Teleport(T_BOTH,playerid, spawns_ls_road[r][0],spawns_ls_road[r][1], spawns_ls_road[r][2], spawns_ls_road[r][3], 0, VW_DEFAULT);
    }
  }

  GameTextForPlayer(playerid, "~y~Los Santos", 1000, 1);

  return 1;
}

new
  Float:spawns_sf_road[][4]={
    //{-1948.65,  898.77, 44.91,  180.0},
    {-1975.05,  893.27, 44.91,  90.0},
    {-1975.05,  886.27, 44.91,  90.0},
    {-1975.05,  879.74, 44.91,  90.0},
    {-1975.05,  872.74, 44.91,  90.0}
  };


CMD:sf(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, -2013.01,872+random(25), 45.44, 240+random(50), 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid,-1995+random(100)-50,883+random(100)-50, 145.31+random(50), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,__("Pojazdy plywajace nie moga sie tam teleportowac! Najbliszy teleport wodny: {b}/portsf{/b}"));
    } else {
      new r=random(sizeof spawns_sf_road);
      Teleport(T_BOTH,playerid, spawns_sf_road[r][0],spawns_sf_road[r][1], spawns_sf_road[r][2], spawns_sf_road[r][3], 0, VW_DEFAULT);
    }
  }

  GameTextForPlayer(playerid, "~y~San Fierro", 1000, 1);

  return 1;
}

new
  Float:spawns_sf2_road[][4]={
    {-2243.91,  562.31, 35.01, 270.0},
    {-2258.59,  548.87, 35.01, 178.0},
    {-2240.68,  510.63, 35.01,  90.0}
  };


CMD:sf2(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, -2270.03+random(5)-2.5,535.31+random(5)-2.5, 35.01, 270+random(10)-5, 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid,-2270+random(100)-50,535+random(100)-50, 205.31+random(50), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      return Msg(playerid,COLOR_ERROR,__("Pojazdy plywajace nie moga sie tam teleportowac! Najbliszy teleport wodny: {b}/portsf{/b}"));
    } else {
      new r=random(sizeof spawns_sf2_road);
      Teleport(T_BOTH,playerid, spawns_sf2_road[r][0],spawns_sf2_road[r][1], spawns_sf2_road[r][2], spawns_sf2_road[r][3], 0, VW_DEFAULT);
    }
  }

  GameTextForPlayer(playerid, "~y~San Fierro", 1000, 1);

  return 1;
}

new
  Float:spawns_bank_road[][4]={     // todo dodac wiecej
    {2296.87, -15.49, 25.90,  0.0},
    {2291.67,   7.57, 25.90,  180.0},
    {2227.60, -31.15, 25.90,  270.0}
  };

CMD:bank(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, 2313.00+random(1)-0.5,-4.5+random(2)-1, 26.74, 26.75+random(5)-2.5, 0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid,2130+random(100)-50,-15+random(100)-50, 150.31+random(50), FLOAT_NAN, 0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      Teleport(T_BOTH_PV, playerid,2100+random(15)-7,-143+random(15)-7, 0, FLOAT_NAN, 0, VW_DEFAULT);
      return Msg(playerid,COLOR_INFO,__("Zostales przeniesiony do najblizszego portu. Bank znajduje sie w centrum {b}Palomino Creek{/b}."));
    } else {
      new r=random(sizeof spawns_bank_road);
      Teleport(T_BOTH,playerid, spawns_bank_road[r][0],spawns_bank_road[r][1], spawns_bank_road[r][2], spawns_bank_road[r][3], 0, VW_DEFAULT);
    }
  }

  GameTextForPlayer(playerid, __("~y~Bank i poczta"), 1000, 1);

  return 1;
}

new
  Float:spawns_portsf_road[][4]={
    {-1641.56, 1286.67, 7.03, 253.0},
    {-1573.37, 1211.65, 7.19, 318.0}
  };

CMD:portsf(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    Teleport(T_PLAYER, playerid, -1598.10,  1248.59, 7.17, random(20)+255,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_MILITARY==VF_MILITARY)
      return Msg(playerid,COLOR_ERROR,__("Pojazdy wojskowe nie moga sie tam teleportowac!"));
    else if (vf&VF_AIRBORNE==VF_AIRBORNE)
      Teleport(T_BOTH_PV, playerid, -1598.10+random(20)-10,   1248.59+random(20)-10, 107.17+random(10), FLOAT_NAN,0, VW_DEFAULT);
    else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      Teleport(T_BOTH, playerid, -1469.53+random(20)-10,  1247.15+random(20)-10, 0, 90,0, VW_DEFAULT);
    else {
      new r=random(sizeof spawns_portsf_road);
      Teleport(T_BOTH, playerid,  spawns_portsf_road[r][0]+random(3)-1.5, spawns_portsf_road[r][1]+random(3)-1.5, spawns_portsf_road[r][2], spawns_portsf_road[r][3], 0, VW_DEFAULT);
    }

  }
  GameTextForPlayer(playerid, __("~b~Port wodny ~h~San Fierro"),1000,1);
  return 1;
}

new
  Float:spawns_wojsko_road[][4]={
    {338.69, 1726.29, 20.78,  43.48},
    {31.91, 1646.13,  17.22,  90.0},
    {119.19, 2065.49, 16.77,  240.49}
  },

  Float:spawns_wojsko_foot[][4]={
    {309.92,2047.52,17.64,179.38},
    {135.02,1929.05,19.21,236.22},
    {104.38,1728.53,17.64,287.52},
    {-2.41,1865.75,17.64,181.59}

  };

CMD:wojsko(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_wojsko_foot);
    Teleport(T_PLAYER, playerid, spawns_wojsko_foot[r][0]+random(3)-1.5, spawns_wojsko_foot[r][1]+random(3)-1.5, spawns_wojsko_foot[r][2], spawns_wojsko_foot[r][3]+random(10)-5,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      new r=random(sizeof spawns_wojsko_road);
      Teleport(T_BOTH_PV, playerid, spawns_wojsko_road[r][0]+random(200)-100, spawns_wojsko_road[r][1]+random(200)-100, spawns_wojsko_road[r][2]+100+random(100), FLOAT_NAN,0, VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL)
      return Msg(playerid, COLOR_ERROR, __("Pojazdy plywajace nie moga sie tam przenosic. Najblizszy teleport: {b}/tama{/b}."));
    else {
      new r=random(sizeof spawns_wojsko_road);
      Teleport(T_BOTH, playerid, spawns_wojsko_road[r][0]+random(2)-1, spawns_wojsko_road[r][1]+random(2)-1, spawns_wojsko_road[r][2], spawns_wojsko_road[r][3]+random(5)-2.5,0, VW_DEFAULT);
    }

  }
  Audio_PlayBGSound(playerid,AUDIOID_FXAFGANISTAN,false,false,true);
  GameTextForPlayer(playerid, "~r~Afganistan",1000,1);
  return 1;
}

new
  Float:spawns_wietnam_road[][4]={
    {-810.03,-2063.98,  25.22,348.68},
    {-860.26,-1985.42,18.29,341.62},
    {-503.62,-1851.59,16.44,112.62}
  },
  Float:spawns_wietnam_water[][4]={
    {-489.0,-1893.38, 5.03,81.68},
    {-756.97,-1969.08,  5.03,FLOAT_NAN}
  },

  Float:spawns_wietnam_foot[][4]={
    {-666.3363,-1839.6409,20.1756},
    {-483.9138,-1868.3649,10.2152},
    {-521.8719,-1916.6610,10.6034}
  };

CMD:wietnam(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));
  if (!IsPlayerInAnyVehicle(playerid)) {
    new r=random(sizeof spawns_wietnam_foot);
    Teleport(T_PLAYER, playerid, spawns_wietnam_foot[r][0]+random(1)-0.5, spawns_wietnam_foot[r][1]+random(1)-0.5, spawns_wietnam_foot[r][2], spawns_wietnam_foot[r][3]+random(10)-5,0, VW_DEFAULT);
  } else {
    new vid=GetPlayerVehicleID(playerid);
    new vf=GetVehicleFlags(vid);

    if (vf&VF_AIRBORNE==VF_AIRBORNE) {
      new r=random(sizeof spawns_wietnam_foot);
      Teleport(T_BOTH_PV, playerid, spawns_wietnam_foot[r][0]+random(200)-100, spawns_wietnam_foot[r][1]+random(200)-100, spawns_wietnam_foot[r][2]+150+random(150), FLOAT_NAN,0, VW_DEFAULT);
    } else if (vf&VF_NATATORIAL==VF_NATATORIAL) {
      new r=random(sizeof spawns_wietnam_water);
      Teleport(T_BOTH, playerid, spawns_wietnam_water[r][0]+random(10)-5, spawns_wietnam_water[r][1]+random(10)-5, spawns_wietnam_water[r][2]+random(2), spawns_wietnam_water[r][3]+random(10)-5,0, VW_DEFAULT);

    } else {
      new r=random(sizeof spawns_wietnam_road);
      Teleport(T_BOTH, playerid, spawns_wietnam_road[r][0]+random(2)-1, spawns_wietnam_road[r][1]+random(2)-1, spawns_wietnam_road[r][2], spawns_wietnam_road[r][3]+random(5)-2.5,0, VW_DEFAULT);
    }

  }
  Audio_PlayBGSound(playerid,AUDIOID_FXWIETNAM,false,false,true);
  GameTextForPlayer(playerid, __("~r~Wietnam"),1000,1);
  return 1;
}

// vehicles

CMD:nrg(playerid)
    return spawnVehicleForPlayer(playerid,522,true);

CMD:pcj(playerid)
    return spawnVehicleForPlayer(playerid,461,true);

CMD:fcr(playerid)
    return spawnVehicleForPlayer(playerid,521,true);

CMD:freeway(playerid) {
    return spawnVehicleForPlayer(playerid,463,true);
}

CMD:nos(playerid){
  if (pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid,COLOR_ERROR,TXT(playerid,454)); // nie mozesz tego teraz zrobic
  new vid=GetPlayerVehicleID(playerid);


  if (vid==0 || vid==INVALID_VEHICLE_ID)
    vid=GetNearestVehicleToPlayer(playerid,2.5,true);
  else
    tVehicles[vid][vo_occupied]=true;

  if (vid==0 || vid==INVALID_VEHICLE_ID)
    return Msg(playerid,COLOR_ERROR,"Musisz byc w pojezdzie, lub tuz obok niego, aby to zrobic"); // musisz byc w pojezdzie aby to zrobic


  new vf=GetVehicleFlags(vid);

  if (vf&VF_STREET!=VF_STREET)
    return Msg(playerid,COLOR_ERROR,"W tym pojezdzie nie mozna zainstalowac nitro...");

  if (vf&VF_BIKES==VF_BIKES) {
    tVehicleUsed[vid]=true;
    tVehicles[vid][vo_hasTurbo]=true;
    return Msg(playerid,COLOR_INFO,__("Wyposazyles swoj motor w TURBO. Wcisnij ALT aby uzyc."));  // wyposazyles swoj pojazd w nitro
  } else {
    AddVehicleComponent(vid, 1010);
    vehicleHasNitro[vid]=true;
    tVehicleUsed[vid]=true;
    tVehicles[vid][vo_used]=true;
    return Msg(playerid,COLOR_INFO,TXT(playerid,450));  // wyposazyles swoj pojazd w nitro
  }

}

CMD:nitro(playerid){
  return _nos(playerid);
}

CMD:napraw(playerid){
  if (pData[playerid][pAttraction]==A_RACE) {
    return race_Napraw(playerid);
  } else if (pData[playerid][pAttraction]==A_DRIFT) {
    return drift_Napraw(playerid);
  } else if (pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid,COLOR_ERROR,TXT(playerid,454)); // nie mozesz tego teraz zrobic
  new vid=GetPlayerVehicleID(playerid);

  if (vid==0 || vid==INVALID_VEHICLE_ID)
    vid=GetNearestVehicleToPlayer(playerid,2.0,true);
  else
    tVehicles[vid][vo_occupied]=true;

  if (vid>0 && vid!=INVALID_VEHICLE_ID) {
    new admin=IsAdmin(playerid, LEVEL_GM);
    new cost;
    new Float:vhp;
    GetVehicleHealth(vid,vhp);
    if (vhp>10000 && !IsGM(playerid)) // vehgod
      return Msg(playerid,COLOR_INFO,__("Pojazd nie wymaga naprawy"));

    if (vhp>VEHICLE_DEFAULT_HP) vhp=VEHICLE_DEFAULT_HP;

    cost=800+(VEHICLE_DEFAULT_HP-floatround(vhp));

    new money;
    money=GetPlayerMoney(playerid);
    if (money < cost && !admin)
      return Msg(playerid,COLOR_ERROR,TXT(playerid,456)); // nie stac Cie na to

    if (GetTickCount()-tVehicles[vid][vo_lastRepair]<VEHICLE_REPAIR_INTERVAL) {
      format(gstr,sizeof gstr,"Musisz odczekac {b}%ds{/b} przed kolejnym uzyciem /napraw", (VEHICLE_REPAIR_INTERVAL-(GetTickCount()-tVehicles[vid][vo_lastRepair]))/1000 );
      return Msg(playerid,COLOR_ERROR,gstr);
    }

    money-=cost;
    if (!admin) SetPlayerMoney(playerid,money);
    RepairVehicle(vid);
    SetVehicleHealth(vid,VEHICLE_DEFAULT_HP);

    tVehicles[vid][vo_lastRepair]=GetTickCount();

    tVehicleUsed[vid]=true;
    tVehicles[vid][vo_used]=true;
    return Msg(playerid,COLOR_INFO,TXT(playerid,457)); // pojazd nie wymaga naprawy
  } else
    return Msg(playerid,COLOR_INFO,TXT(playerid,451));  // musisz byc w pojezdzie aby to zrobic
}

CMD:flip(playerid){
  if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER)
      return Msg(playerid,COLOR_ERROR,"Tej komendy mo¿e u¿yæ tylko kierowca pojazdu!");
  if (pData[playerid][pAttraction]==A_RACE)
    return race_Napraw(playerid);
  else if (pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid,COLOR_ERROR,TXT(playerid,454)); // nie mozesz tego teraz zrobic
  new vid=GetPlayerVehicleID(playerid);

  tVehicles[vid][vo_occupied]=true;

  if (vid>0 && vid!=INVALID_VEHICLE_ID) {
    new Float:zangle;
    GetVehicleZAngle(vid,zangle);
    SetVehicleZAngle(vid,zangle);
    tVehicleUsed[vid]=true;
    tVehicles[vid][vo_used]=true;
    return 1;
  } else
    return Msg(playerid,COLOR_INFO,TXT(playerid,451));  // musisz byc w pojezdzie aby to zrobic
}

CMD:frakcja(playerid, params[])
{
    MUST_BE_SPAWNED(playerid);
    
    new
     frakcja,
     pcount = 0,
     lista[1024];

    if (!strcmp(params,"policja",true) && !isnull(params)) frakcja = FACTION_POLICE;
    else if (!strcmp(params,"wojsko",true) && !isnull(params)) frakcja = FACTION_ARMY;
    else if (!strcmp(params,"dilerzy",true) && !isnull(params)) frakcja = FACTION_DILERZY;
    else if (!strcmp(params,"mechanicy",true) && !isnull(params)) frakcja = FACTION_MECHANICY;
    else if (!strcmp(params,"kierowcy",true) && !isnull(params)) frakcja = FACTION_KIEROWCY;
    else if (!strcmp(params,"piloci",true) && !isnull(params)) frakcja = FACTION_PILOCI;
    else if (!strcmp(params,"maszynisci",true) && !isnull(params)) frakcja = FACTION_MASZYNISCI;
    else return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/frakcja <policja|wojsko|dilerzy|mechanicy|kierowcy|piloci|maszynisci>{/b}");
     
    foreach(i)
    {
      if(pTemp[i][faction] == frakcja)
      {
        if (pcount++<10)
          format(lista,sizeof lista,"%s%s{ffffff}%d\t{%06x}%s", lista, strlen(lista)>0?("\n"):(""), i, pData[i][currentColor],GetPlayerNick(i));
        if (pcount>=10)
          format(lista,sizeof lista,"%s\n{ffffff}+%d %s", lista, (pcount-9), dli(pcount-9, "gracz","graczy","graczy"));

        ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_LIST, "Znalezieni gracze:", lista, "Zamknij","");
        pcount++;
      }
    }
    if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

    return 1;
}
// EOF