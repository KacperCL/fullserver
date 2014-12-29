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
 
CMD:prsp(playerid, params[]){
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/prsp [id/nick]{/b}.",false);
  
    new
   targetplayerid,
   buffer[128];

  targetplayerid = FindPlayer(params);

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
  
  _rsp(targetplayerid,"secret");
  return 1;
}

CMD:telinfo(playerid){
    if(!IsAdmin(playerid)) return 0;
  new buf[128];
  format(buf,sizeof buf,"Obecne /telpos zosta≥o ustawione przez gracza {b}%s{/b}",telpos[autor]);
  Msg(playerid,COLOR_INFO2,buf);
  return 1;
}

CMD:desync(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Zdesynchronizowani na serwerze:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][desync]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          copy("", buffer);
        }
      }
    if (strlen(buffer)>0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"troll","trolle","trolli"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/desync [id/nick]{/b}. Ponowne uzycie komendy wylaczy ograniczenie.",false);
  }

  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

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
  if(!pTemp[targetplayerid][desync]) {
    pTemp[targetplayerid][desync]=true;
    Msg(playerid,COLOR_INFO, "Nalozyles ograniczenie na gracza");
    format(buffer,sizeof buffer,"{b}%s{/b} WLACZYL%s desynchronizacje {b}%s{/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("A"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer, true, LEVEL_ADMIN2);
  } else {
    pTemp[targetplayerid][desync]=false;
    Msg(playerid, COLOR_INFO,"Zdjales ograniczenie z gracza ");
    format(buffer,sizeof buffer,"{b}%s{/b} WYLACZYL%s desynchronizacje {b}%s{/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("A"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer, true, LEVEL_ADMIN2);
  }

  return 1;
}

CMD:raporty(playerid)
{
    if (!IsAdmin(playerid))
        return 0;
    new
     szList[3000],
     _players = 0,
     buffer[127];

    for(new i = 0; i < MAX_REPORTS; i++)
    {
        if(!gReports[i][rOccupiedIndex]) continue;

        format(buffer, sizeof buffer, "{%06x}%s{ffffff}(%i) - %s\n", pData[gReports[i][rPlayerId]][currentColor], gReports[i][rPlayerName], gReports[i][rPlayerId], gReports[i][rReason]);
        strcat(szList, buffer);

        pTemp[playerid][reportSync][_players++] = i;
    }

    if(_players)
    {
        if (pData[playerid][pAttraction]==A_ARENA)
            _arenaexit(playerid);

        new
         length = strlen(szList);

        strdel(szList, length - 1, length);
    }

    ShowPlayerDialog(playerid, DIALOG_REPORTS, DIALOG_STYLE_LIST, TXT(playerid, 406), (_players > 0) ? szList : TXT(playerid, 408), __("Wybierz"), __("Wyjdz"));

    return 1;
}

CMD:clearchat(playerid)
{
  if(!IsAdmin(playerid)) return 0;

  new
   buffer[128];

  for(new i = 1; i <= 100; i++)
    SendClientMessageToAll(-1, " ");

  foreach(i)
  {
    format(buffer, sizeof buffer, TXT(i, 384), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    Msg(i, COLOR_INFO2, buffer); // Czat zosta≥ wyczyszczony przez admina xxx.
  }

  return 1;
}

CMD:checkip(playerid, parms[]) { //z pm od mruczka v2
  if(!IsAdmin(playerid)) return 0;
  new ip[16],a,b,c,d,x,y,z,g,string[128],found;
  if(sscanf(parms,"p<.>ddD(-1)D(-1)",a,b,c,d)) return Msg(playerid,COLOR_ERROR,"Uøyj: /checkip <x.x.*.*>");
  foreach(i)
  {
    GetPlayerIp(i,ip,sizeof(ip));
    sscanf(ip,"p<.>dddd",x,y,z,g);
    if(a == x && b == y && c == z && d == g){
      format(string,sizeof(string),"Gracz o id %d ma identyczne ip: %d.%d.%d.%d",i,x,y,z,g);
      Msg(playerid,COLOR_INFO2,string);
      found++;
      continue;
    }else if(a == x && b == y && c == z){
      format(string,sizeof(string),"Gracz o id %d ma podobne ip: %d.%d.%d.%d",i,x,y,z,g);
      Msg(playerid,COLOR_INFO2,string);
      found++;
      continue;
    }else if(a == x && b == y){
      format(string,sizeof(string),"Gracz o id %d ma podobne ip: %d.%d.%d.%d",i,x,y,z,g);
      Msg(playerid,COLOR_INFO2,string);
      found++;
      continue;
    }
  }
  if(found>0) {
      format(string,sizeof string,"Znaleziono %d graczy pasujacych do wzorca",found);
      Msg(playerid,COLOR_INFO2,string);
  }else{
    Msg(playerid,COLOR_ERROR,"Nie znaleziono wzorca!");
  }
  return 1;
}

CMD:alogin(playerid,params[]){
  new buf[128];
  if (IsAdmin(playerid))
    return SetPlayerDrunkLevel(playerid,5000);

  if (pData[playerid][allowedLevel]<LEVEL_ADMIN1 || pData[playerid][allowedLevel]>=LEVEL_ADMIN3) {
    TextDrawShowForPlayer(playerid, gTextDraw[TD_WARNING]);
    SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");
    SendClientMessage(playerid,0xff0000ff,"Uwaga! {ffffff}Wszystkie proby nieautoryzowanego dostepu");
    SendClientMessage(playerid,0xffffffff,"sa rejestrowane i beda scigane z pelna surowoscia.");
    SendClientMessage(playerid,0xffffffff,"Powtarzajace sie przypadki sa zglaszane bezposrednio");
    SendClientMessage(playerid,0xffffffff,"na policje. {ff0000}Zastanow sie trzy razy, zanim zaczniesz.");
    SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");
    PlaySound(playerid,1053);
    SetTimerEx("HidePlayerWarning",10000,false,"d",playerid);

    if (pData[playerid][allowedLevel]<LEVEL_ADMIN1) { // nieudanych prob logowania hiddenow nie pokazujemy
      format(buf, sizeof buf, "Proba logowania na admin bez autoryzacji przez {b}%s{/b} (%d).", GetPlayerNick(playerid), playerid);
        MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN2);
      format(buf, sizeof buf, "{b}%s{/b} (%d) wprowadzil haslo: '{b}%s{/b}'", GetPlayerNick(playerid), playerid, params);
      MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN3);
    }
    pTemp[playerid][warningReceived]=GetTickCount();

    return 1;
  }

  if (isnull(params))
    return Msg(playerid,COLOR_INFO,"/alogin {b}<haslo>{/b}");
  else if (strlen(params)<6 || params[strlen(params)-1]!=0x20)
    return Msg(playerid,COLOR_ERROR,"Bledne haslo. Twoja proba zostala zarejestrowana, zastanow sie czy chcesz zgadywac dalej...");

  if (pData[playerid][suspended]) {
    format(gstr,sizeof gstr,"Jestes obecnie zawieszon%s w randze. Mozesz odwolac sie od zawieszenia w strefie na forum.", Kobieta(playerid)?("a"):("y"));
    Msg(playerid,COLOR_ERROR,gstr);
    format(gstr,sizeof gstr,"Proba zalogowania sie na admina przez zawieszonego gracza %s (%d).", GetPlayerProperName(playerid), playerid);
    MSGToAdmins(COLOR_ERROR, gstr, true, LEVEL_ADMIN1);
    return 1;
  }


  format(buf,sizeof buf,"%s%s",SALT_ALOGIN,params);

  if (pData[playerid][allowedLevel]>=LEVEL_ADMIN1 && strcmp(MD5_Hash(buf), gmTemp[AdminPasswordHash],true,32)==0) {
    pData[playerid][adminLevel]=pData[playerid][allowedLevel];
    SetPlayerProperColor(playerid,true);

    new
     szPlayerName[24];

    GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

    if (Kobieta(playerid))
      format(buf, sizeof buf, __("{b}%s{/b} zalogowala sie jako administratorka%s"), szPlayerName, pData[playerid][adminLevel]==LEVEL_ADMIN3?(" {b}RCON{/b}"):("."));
    else
      format(buf, sizeof buf, __("{b}%s{/b} zalogowal sie jako administrator%s"), szPlayerName, pData[playerid][adminLevel]==LEVEL_ADMIN3?(" {b}RCON{/b}"):("."));

    foreach(i)
    {
      Msg(i, (pData[playerid][adminLevel]==LEVEL_ADMIN3 ? COLOR_ADMIN3 : COLOR_ADMIN), buf, false);
      PlaySound(i, 1133);
    }


  } else {
    if (pData[playerid][allowedLevel]<=LEVEL_ADMIN3) {  // hiddenow nie logujemy
      format(buf, sizeof buf, __("Nieudana proba autoryzacji admina: {b}%s{/b} (%d)."), GetPlayerNick(playerid), playerid);
      MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN2);
      format(buf, sizeof buf, __("{b}%s{/b} (%d) wprowadzil haslo: '{b}%s{/b}'"), GetPlayerNick(playerid), playerid, params);
      MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN3);
    }
    pTemp[playerid][warningReceived]=GetTickCount();

    return Msg(playerid,COLOR_ERROR,__("Bledne haslo. Twoja proba zostala zarejestrowana, zastanow sie czy chcesz zgadywac dalej..."));
  }
  UpdatePlayerNickTD(playerid);
  return 1;
}

CMD:tpto(playerid,params[]){
    if(!IsAdmin(playerid) && NIEMAPERZY) return 0;
    if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

    new Float:P[3],int,vw;

    if (sscanf(params,"fffI(-1)I(-1)",P[0],P[1],P[2],int,vw))
  return Msg(playerid, COLOR_ERROR, "/tpto <X> <Y> <Z> [interior] [virtualworld]");

    SetPlayerPos(playerid,P[0],P[1],P[2]);
    if (int>-1)
  SetPlayerInterior(playerid,int);
    if (vw>-1)
  SetPlayerVirtualWorld(playerid,vw);

    SetCameraBehindPlayer(playerid);
    return 1;
}

CMD:setint(playerid,params[]){
    if(!IsAdmin(playerid) && NIEMAPERZY) return 0;
    new int,userid;
    if (sscanf(params,"dd",userid,int))
  return Msg(playerid,COLOR_ERROR,"/setip <id gracza> <interior>");
    else if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    else if (int<0)
  return Msg(playerid,0xffffffff,"Nieprawidlowy numer interioru (interiory ujemne nie wystepuja w GTA)!");
    SetPlayerInterior(userid,int);
    // todo powiadomienie dla gracza i admina
    return 1;
}

CMD:setvw(playerid,params[]){
    if(!IsAdmin(playerid) && NIEMAPERZY) return 0;
    new vw,userid;
    if (sscanf(params,"dd",userid,vw))
  return Msg(playerid,COLOR_ERROR,"/setworld <id gracza> <vw>");
    else if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    else if (vw<0 || vw>999)
  return Msg(playerid,0xffffffff,"Nieprawidlowy numer swiata! (ograniczenie do 999 dla bezpieczenstwa, rzeczywisty limit jest wyzszy)");
    SetPlayerVirtualWorld(userid,vw);
    // todo powiadomienie dla gracza i admina
    return 1;
}

CMD:setvhp(playerid,params[]){
    if(!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
    new hpv,userid;
    if (sscanf(params,"dd",userid,hpv))
  return Msg(playerid,COLOR_ERROR,"/setvhp <id gracza> <ile hp>");
    else if (!IsPlayerConnected(userid) || !IsPlayerInAnyVehicle(userid))
  return Msg(playerid,0xffffffff,"Gracz nie jest w pojezdzie lub nie jest online");
  new vid=GetPlayerVehicleID(userid);
  SetVehicleHealth(vid,hpv);
    // todo powiadomienie dla gracza i admina
    return 1;
}

CMD:sethp(playerid,params[]){
    if(!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
    new hpv,arv,userid;
    if (sscanf(params,"ddD(-1)",userid,hpv,arv))
  return Msg(playerid,COLOR_ERROR,"/sethp <id gracza> <ile hp> [ile ar]");
    else if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
  if (arv>-1) SetPlayerArmour(userid,arv);
  SetPlayerHealth(userid,hpv);
    // todo powiadomienie dla gracza i admina
    return 1;
}

CMD:explode(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
  new bool:force;
  if (params[strlen(params)-1]=='!') {
    force=true;
    params[strlen(params)-1]=0;
  }
    new userid;
    if (sscanf(params,"d",userid)) {
    if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
        userid=pData[playerid][spectating];
    else
          return Msg(playerid,COLOR_ERROR,"/explode [id gracza] [!]");
    }

    if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

  if (!force) {
    new pstate=GetPlayerState(userid);
    switch(pstate){
      case PLAYER_STATE_SPAWNED, PLAYER_STATE_ONFOOT:
        force=true;
      case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
        return Msg(playerid, COLOR_ERROR,"Najpierw wyjmij gracza z pojadu lub uzyj {b}/explode [id] !{/b} aby wymusic.");
      case PLAYER_STATE_SPECTATING:
        if (userid!=playerid)
          return Msg(playerid, COLOR_ERROR, "Wskazany graczy nie jest zespawnowany");
        else
          force=true;
      default:
        return Msg(playerid, COLOR_ERROR, "Wskazany gracz nie jest zespawnowany");
    }
  }

  if (force) {
    if (pTemp[userid][staleTime]>5)
      Msg(playerid, COLOR_INFO2, "Gracz jest AFK.", false);

    new Float:P[3],Float:HP,Float:AR;
    GetPlayerHealth(userid, HP);
    GetPlayerArmour(userid, AR);
    if (HP>100)
      Msg(playerid, COLOR_INFO2,"Gracz posiada goda (ze strony serwera).", false);
    GetPlayerPos(userid,P[0],P[1],P[2]);
    CreateExplosion(P[0],P[1],P[2], 2, 25.0);

    format(gstr, sizeof gstr, "%03.1f HP / %03.1f AR", HP, AR);
    GameTextForPlayer(playerid, gstr, 350, 5);

    SetTimerEx("explode_After", 500, false, "ddd", playerid, userid,2);

  } else {  // shouldn't happen
    Msg(playerid, COLOR_INFO2, "/explode nie wykonany. To sie nie powinno zdarzyc.");
  }
    return 1;
}

CMD:exp(playerid,params[])
  return _explode(playerid,params);

CMD:weather(playerid,params[]){
  if (!IsAdmin(playerid)) return 0;
  new wid;
  if (sscanf(params,"d",wid))
    return Msg(playerid,COLOR_ERROR,"/weather {b}<id pogody>{/b}");
  if (wid<0 || wid>45)
    return Msg(playerid,COLOR_ERROR,"Podany id pogody poza dopuszczalnym zakresem (0-45)!");
  gmTemp[currentWeather]=wid;
  SetWeather(gmTemp[currentWeather]);
  foreach(i)
      if(GetPlayerInterior(i)==0) SetPlayerWeather(i,gmTemp[currentWeather]);
  gmTemp[lastWeatherChangeTime]=GetTickCount()/1000;
  if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN)
    return 1;
  new str[80];
  format(str,sizeof str,"Pogoda zmieniona na {b}%d{/b} przez {b}%s{/b}.", wid, GetPlayerNick(playerid));
  MSGToAdmins(COLOR_INFO, str, false, LEVEL_GM);
  return 1;
}

CMD:god(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
    new userid,buf[128];
    if (sscanf(params,"d",userid))
  return Msg(playerid,COLOR_ERROR,"/god <id gracza>");
    if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    SetPlayerHealth(userid,9999999.0);
  if(gmData[artefactOwner] == userid) DropArtefact(userid);
  if(userid!=playerid) {

    format(buf,sizeof buf, "{b}%s (%d){/b} otrzymal%s goda od {b}%s{/b}", GetPlayerProperName(userid), userid, Kobieta(userid)?("a"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    MSGToAdmins(COLOR_INFO, buf, false);
    if (random(2)==1)
      format(buf,sizeof buf, "Administrator%s {b}%s{/b} uczynil Cie niesmierteln%s!", Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(userid)?("a"):("ym"));
    else
      format(buf,sizeof buf, "Otrzymal%s {b}niesmiertelnosc{/b} od admin%s {b}%s{/b}!", Kobieta(userid)?("as"):("es"), Kobieta(playerid)?("ki"):("a"), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    Msg(userid, COLOR_ADMIN, buf);
  }
    return 1;
}

CMD:godall(playerid,params[]){
    if (!IsAdmin(playerid)) return 0;
  new cnt,buf[128];
  foreach(i)
  if ((pData[i][pAttraction]==A_NONE || pData[i][pAttraction]==A_ARENA) && pTemp[i][onArena]!=ARENA_SOLO){
    if (random(2)==1)
      format(buf,sizeof buf, "Administrator%s {b}%s{/b} uczynil Cie niesmierteln%s!", Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(i)?("a"):("ym"));
    else
      format(buf,sizeof buf, "Otrzymal%s {b}niesmiertelnosc{/b} od admin%s {b}%s{/b}!", Kobieta(i)?("as"):("es"), Kobieta(playerid)?("ki"):("a"), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    Msg(i, COLOR_ADMIN, buf);

      SetPlayerHealth(i,9999999.0);
    cnt++;
  }
  format(buf,sizeof buf,"{b}%s{/b} dal/a niesmiertelnosc {b}%d %s{/b}", GetPlayerProperName(playerid), cnt, dli(cnt,"graczowi","graczom","graczom"));
  MSGToAdmins(COLOR_INFO, buf, false);
    return 1;
}

CMD:lost(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN1)) return 0;

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uøywaj: /lost <gracz>"); // Uøywaj: /kick <gracz>

  new
   szPlayerName[24],
   targetplayerid,
   buffer[128];

  sscanf(params, "s[24]", szPlayerName);
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

  if (targetplayerid!=playerid && pData[targetplayerid][loggedIn] && pData[targetplayerid][adminLevel]>pData[playerid][adminLevel] && pData[targetplayerid][adminLevel]<LEVEL_ADMINHIDDEN  && pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN) {
    format(buffer,sizeof buffer,"Admin {b}%s{/b}, poziom: {b}%d{/b}, probowal wyrzucic admina {b}%s{/b}, poziom: {b}%d{/b}!",
      GetPlayerNick(playerid), pData[playerid][adminLevel], GetPlayerNick(targetplayerid), pData[targetplayerid][adminLevel]);
    MSGToAdmins(COLOR_ADMIN, buffer, true, LEVEL_ADMIN2);

    Msg(playerid, COLOR_ERROR, "Nie mozesz zbanowac admina o wyzszym poziomie od swojego!");
    // banujemy sie automatycznie
      format(buffer, sizeof buffer,"%d do wyjasnienia w gronie administracji", playerid);
    _b(playerid, buffer);
    return 1;
  }
  
  pTemp[targetplayerid][lostedByAdmin] = true;

  Msg(targetplayerid,COLOR_ERROR,"WAØNA INFORMACJA: Najprawdopodobniej mia≥eú losta i administrator zdecydowa≥ siÍ usunπÊ CiÍ z serwera. Po≥πcz siÍ ponownie!");
  KickPlayer(targetplayerid);
  return 1;
}

CMD:kick(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN1)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 149)); // Uøywaj: /kick <gracz> <powÛd>

  new
   szPlayerName[24],
   targetplayerid,
   buffer[128],
   reason[85];

  sscanf(params, "s[24]s[85]", szPlayerName, reason);
  if(strlen(reason)>80)
    return Msg(playerid,COLOR_ERROR,"Powod kicka jest zbyt dlugi.");
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

  if (targetplayerid!=playerid && pData[targetplayerid][loggedIn] && pData[targetplayerid][adminLevel]>pData[playerid][adminLevel] && pData[targetplayerid][adminLevel]<LEVEL_ADMINHIDDEN  && pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN) {
    format(buffer,sizeof buffer,"Admin {b}%s{/b}, poziom: {b}%d{/b}, probowal wyrzucic admina {b}%s{/b}, poziom: {b}%d{/b}!",
      GetPlayerNick(playerid), pData[playerid][adminLevel], GetPlayerNick(targetplayerid), pData[targetplayerid][adminLevel]);
    MSGToAdmins(COLOR_ADMIN, buffer, true, LEVEL_ADMIN2);

    Msg(playerid, COLOR_ERROR, "Nie mozesz zbanowac admina o wyzszym poziomie od swojego!");
    // banujemy sie automatycznie
      format(buffer, sizeof buffer,"%d do wyjasnienia w gronie administracji", playerid);
    _b(playerid, buffer);
    return 1;
  }

  if(!strlen(reason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 149)); // Uøywaj: /kick <gracz> <powÛd>

  pTemp[playerid][tmpTargetPlayerID] = targetplayerid;
  copy(reason, pTemp[playerid][tmpReason]);

  format(buffer, sizeof buffer, TXT(playerid, 150), GetPlayerProperName(targetplayerid), reason);
  ShowPlayerDialog(playerid, DIALOG_KICK_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 151), buffer, TXT(playerid, 62), TXT(playerid, 63));
  return 1;
}

CMD:b(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN1)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/b <id/nick> [powod]{/b}");
  new
   tplayer[24], tplayer_escaped[32],
   uid,
   buf[512], reason[86], bool:accountBan, ipaddr[16];
  sscanf(params,"s[24]S(brak powodu)[85]", tplayer, buf);
  if (strlen(buf)>80)
    return Msg(playerid,COLOR_ERROR,"Powod bana jest zbyt dlugi");
  mysql_real_escape_string(buf,reason);

  if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;

  if (pTemp[uid][cheater] || pTemp[uid][desync] || pTemp[uid][troll])
    return Msg(playerid,COLOR_ERROR,"Gracz jest zdropowany/zdesynchronizowany/strollowany - nie trzeba go banowac.");


  if (uid!=playerid && pData[uid][loggedIn] && pData[uid][adminLevel]>=pData[playerid][adminLevel] && pData[uid][adminLevel]<LEVEL_ADMINHIDDEN  && pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN) {
    format(buf,sizeof buf,"Admin {b}%s{/b}, poziom: {b}%d{/b}, probowal zbanowac admina {b}%s{/b}, poziom: {b}%d{/b}!",
      GetPlayerNick(playerid), pData[playerid][adminLevel], GetPlayerNick(uid), pData[uid][adminLevel]);
    MSGToAdmins(COLOR_ADMIN, buf, true, LEVEL_ADMIN2);

    Msg(playerid, COLOR_ERROR, "Nie mozesz zbanowac admina o wyzszym poziomie od swojego!");
    // banujemy sie automatycznie
      format(buf, sizeof buf,"%d do wyjasnienia w gronie administracji", playerid);
    _b(playerid, buf);
    return 1;
  }

  if (pTemp[playerid][bannedPlayersCnt]++>(pData[playerid][adminLevel]==LEVEL_ADMIN1?10:25)  && playerid!=uid) {
    format(buf, sizeof buf,"%d do wyjasnienia w gronie administracji", playerid);
    _b(playerid, buf);

    format(buf,sizeof buf,"Admin {b}%s{/b}, zbanowal zbyt duzo graczy w jednej sesji i zostal automatycznie zablokowany!",
      GetPlayerNick(playerid));
    MSGToAdmins(COLOR_ADMIN, buf, true, LEVEL_ADMIN2);
    return 1;
  }

  if (pData[uid][loggedIn] && pData[uid][accountID]>0) {
    format(buf, sizeof buf, "INSERT INTO fs_bans SET player_banned=%d,player_given=%d,date_end=NOW()+INTERVAL 4 DAY,reason='%s'",
        pData[uid][accountID], pData[playerid][accountID], reason);
    mysql_query(buf);

    format(buf, sizeof buf, "UPDATE %s SET ban_count = ban_count + 1 WHERE id = %i", gmData[DB_players], pData[uid][accountID]);
    mysql_query(buf);
    accountBan=true;
  }

  mysql_real_escape_string(GetPlayerNick(uid),tplayer_escaped);

  // jesli ma konto, dajemy bana na 30 minut - zeby nie zmienil adresu i nie wlazl pod innym nickiem
  // jesli nie ma konta, ban na 7 dni
  GetPlayerIp(uid, ipaddr, 16);
  // todo poszukiwania innego gracza online z tym adresem

  if (accountBan)
    format(buf, sizeof buf, "INSERT INTO fs_ipbans SET ip='%s',player_given=%d,date_end=NOW()+INTERVAL 4 DAY,reason='%s',target_accountid=%d,target_nick='%s'",
          ipaddr, pData[playerid][accountID], reason, pData[uid][accountID],tplayer_escaped);
  else
    format(buf, sizeof buf, "INSERT INTO fs_ipbans SET ip='%s',player_given=%d,date_end=NOW()+INTERVAL 7 DAY,reason='%s',target_nick='%s'",
          ipaddr, pData[playerid][accountID], reason, tplayer_escaped);

  mysql_query(buf);

  format(buf, sizeof buf, "{ff0000}Gracz {ffffff}%s{ff0000} zostal zbanowany przez admin%s {ffffff}%s{ff0000}",
    GetPlayerNick(uid), SkinKobiecy(GetPlayerSkin(playerid)) ? ("ke") : ("a"),
    (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));

  foreach(i)
    Msg(i,COLOR_ERROR, buf);

  format(buf, sizeof buf, "{ff0000}Powod: {ffffff}%s{ff0000}", reason);

    foreach(i)
    Msg(i,COLOR_ERROR, buf);

    GivePlayerScore(uid,-100,"ban");

  GameTextForPlayer(uid, "~r~ZROB SCREEN ~w~- ~r~~h~F8", 10000, 3);
  SendClientMessage(uid,-1, " ");
  Msg(uid,COLOR_INFO,"Jesli uwazasz ze ten ban jest niesluszny, badz tez bedziesz staral sie o wczesniejsze jego zdjecie", false);
  Msg(uid,COLOR_INFO,"to koniecznie zrob {b}screenshot{/b} wciskajac teraz klawisz {b}F8{/b}, nastepnie odwiedz nasze forum", false);
  Msg(uid,COLOR_INFO,"pod adresem {b}WWW.FULLSERVER.EU{/b} i zloz tam podanie o odbanowanie.");
  if (Audio_IsClientConnected(uid))
    Audio_Play(uid,AUDIOID_BAN, false, false, true);
  KickPlayer(uid);
  return 1;
}

CMD:banip(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  if(isnull(params))
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 185), false); // Uøywaj: /banip <gracz/adres IP> <czas> <m/g/d/t/s/*> <powÛd>
    return Msg(playerid, COLOR_ERROR, TXT(playerid, 186)); // Podpowiedü: w pierwszym parametrze komendy wprowadü adres IP (zakres lub pe≥ny) albo nick/ID gracza bÍdπcego online.
  }

  new
   szIP[24],
   targetplayerid,
   banTime,
   period,
   buffer[256],
   reason[128];

  sscanf(params, "s[24]ics[128]", szIP, banTime, period, reason);

  if(!IsCorrectIP(szIP))
  {
    targetplayerid = FindPlayer(szIP);

    if(targetplayerid == ERROR_NOT_FOUND)
    {
      format(buffer, sizeof buffer, TXT(playerid, 190), szIP);
      Msg(playerid, COLOR_ERROR, buffer, false);
      return Msg(playerid, COLOR_ERROR, TXT(playerid, 186)); // Podpowiedü: w pierwszym parametrze komendy wprowadü adres IP (zakres lub pe≥ny) albo nick/ID gracza bÍdπcego online.
    }
    if(targetplayerid == ERROR_FOUND_MULTIPLE)
    {
      format(buffer, sizeof buffer, TXT(playerid, 28), szIP);
      return Msg(playerid, COLOR_ERROR, buffer);
    }

    GetPlayerIp(targetplayerid, szIP, sizeof szIP);
  }

  if((banTime < 1 || banTime > 3) && period == 's') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
  if((banTime < 1 || banTime > 29) && period == 'd') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
  if((banTime < 1 || banTime > 3) && period == 't') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    if((banTime < 1 || banTime > 59) && period == 'm') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    if((banTime < 1 || banTime > 23) && period == 'h') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");

  if(period != 'm' && period != 'g' && period != 'd' && period != 't' && period != 's')
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 29)); // Nieprawid≥owy ostatni parametr komendy.
    return Msg(playerid, COLOR_ERROR, TXT(playerid, 30)); // Podpowiedü: m - minuty, g - godziny, d - dni, t - tygodnie, s - miesiπce.
  }

  if(!strlen(reason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 185)); // Uøywaj: /banip <gracz/adres IP> <czas> <m/g/d/t/s/*> <powÛd>

  copy(szIP, pTemp[playerid][tmpBanIP]);
  pTemp[playerid][tmpBanTime] =   banTime;
  pTemp[playerid][tmpPeriod] =  period;
  copy(reason, pTemp[playerid][tmpReason]);

  format(buffer, sizeof buffer, TXT(playerid, 187), szIP, banTime, GetPeriodName(playerid, period, banTime), reason);
  ShowPlayerDialog(playerid, DIALOG_BANIP_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 188), buffer, TXT(playerid, 62), TXT(playerid, 63));

  return 1;
}

CMD:ban(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 26)); // Uøywaj: /ban <gracz/nazwa konta> <czas> <m/h/d/t/s/*> <powÛd>

  new
   szPlayerName[24],
   targetplayerid,
   banTime,
   period,
   buffer[256],
   reason[128],
   bool:accountBan = false;

  sscanf(params, "s[24]icS(brak powodu)[128]", szPlayerName, banTime, period, reason);
  targetplayerid = FindPlayer(szPlayerName);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    if(AccountExists(szPlayerName))
    {
      accountBan = true;
    }
    else
    {
      format(buffer, sizeof buffer, TXT(playerid, 180), szPlayerName);
      return Msg(playerid, COLOR_ERROR, buffer);
    }
  }
  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if (!accountBan && (pTemp[targetplayerid][cheater] || pTemp[targetplayerid][desync] || pTemp[targetplayerid][troll]))
    return Msg(playerid,COLOR_ERROR,"Gracz jest zdropowany/zdesynchronizowany/strollowany - nie trzeba go banowac.");

  if(!accountBan && !PlayerAccountExists(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 25));

  if (!accountBan && targetplayerid!=playerid && pData[targetplayerid][loggedIn] && pData[targetplayerid][adminLevel]>=pData[playerid][adminLevel] && pData[targetplayerid][adminLevel]<LEVEL_ADMINHIDDEN  && pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN) {
    format(buffer,sizeof buffer,"Admin {b}%s{/b}, poziom: {b}%d{/b}, probowal zbanowac admina {b}%s{/b}, poziom: {b}%d{/b}!",
      GetPlayerNick(playerid), pData[playerid][adminLevel], GetPlayerNick(targetplayerid), pData[targetplayerid][adminLevel]);
    MSGToAdmins(COLOR_ADMIN, buffer, true, LEVEL_ADMIN2);

    Msg(playerid, COLOR_ERROR, "Nie mozesz zbanowac admina o wyzszym poziomie od swojego!");
    // banujemy sie automatycznie
    format(buffer, sizeof buffer,"%d do wyjasnienia w gronie administracji", playerid);
    _b(playerid, buffer);
    return 1;
  }
  
  if (pTemp[playerid][bannedPlayersCnt]++>(pData[playerid][adminLevel]==LEVEL_ADMIN1?10:25)  && playerid!=targetplayerid) {
    format(buffer, sizeof buffer,"%d do wyjasnienia w gronie administracji", playerid);
    _b(playerid, buffer);

    format(buffer,sizeof buffer,"Admin {b}%s{/b}, zbanowal zbyt duzo graczy w jednej sesji i zostal automatycznie zablokowany!",
      GetPlayerNick(playerid));
    MSGToAdmins(COLOR_ADMIN, buffer, true, LEVEL_ADMIN2);
    return 1;
  }

  if((banTime < 1 || banTime > 3) && period == 's') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
  if((banTime < 1 || banTime > 29) && period == 'd') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
  if((banTime < 1 || banTime > 3) && period == 't') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    if((banTime < 1 || banTime > 59) && period == 'm') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    if((banTime < 1 || banTime > 23) && period == 'h') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");

  if(period != 'm' && period != 'h' && period != 'd' && period != 't' && period != 's')
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 29)); // Nieprawid≥owy ostatni parametr komendy.
    return Msg(playerid, COLOR_ERROR, "M - minuty, H - godziny, D - dni, T - tygodnie, S - miesiace");
  }

  if(!strlen(reason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 26)); // Uøywaj: /ban <gracz> <czas> <m/g/d/t/s> <powÛd>

  if(!accountBan)
  {
    pTemp[playerid][tmpTargetPlayerID] = targetplayerid;
  }
  else
  {
    pTemp[playerid][tmpTargetPlayerID] = -1;
    copy(szPlayerName, pTemp[playerid][tmpBanAccountName]);
  }
  pTemp[playerid][tmpBanTime] =       banTime;
  pTemp[playerid][tmpPeriod] =      period;
  copy(reason, pTemp[playerid][tmpReason]);

  format(buffer, sizeof buffer, TXT(playerid, 61), (accountBan) ? szPlayerName : GetPlayerProperName(targetplayerid), banTime, GetPeriodName(playerid, period, banTime), reason);
  ShowPlayerDialog(playerid, DIALOG_BAN_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 60), buffer, TXT(playerid, 62), TXT(playerid, 63));

  return 1;
}

CMD:spec(playerid, params[])
{
  if(!IsAdmin(playerid)) return 0;
  if(!SpecAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 411)); // Nie moøesz w tym momencie obserwowaÊ graczy.
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 106)); // Uøywaj: /spec <gracz>

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

  if(playerid == targetplayerid) return Msg(playerid, COLOR_ERROR, TXT(playerid, 64)); // Nie moøesz obserwowaÊ siebie.
  if(!IsPlayerSpawned(targetplayerid) && !IsPlayerNPC(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));

  StartSpectating(playerid, targetplayerid);

  format(buffer, sizeof buffer, TXT(playerid, 107), GetPlayerNick(targetplayerid), targetplayerid ); // Obserwujesz gracza "%s" (%i).
  Msg(playerid, COLOR_INFO, buffer);

  return 1;
}

CMD:specoff(playerid)
{
  if(!IsAdmin(playerid, LEVEL_ADMIN1)) return 0;
  if(pData[playerid][spectating] == INVALID_PLAYER_ID) return Msg(playerid, COLOR_ERROR, TXT(playerid, 65));

  StopSpectating(playerid);
  GivePlayerNormalWeapons(playerid);

  return 1;
}

CMD:ann(playerid, params[])
{
  if(!IsAdmin(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 177)); // Uøywaj: /ann <tekst>
  if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // IloúÊ tyld "~" musi byÊ parzysta.

  new buffer[512];
  format(buffer, sizeof buffer, "%s: %s", GetPlayerNick(playerid), params);
  OutputLog(LOG_ANN, buffer);
  ShowAnnouncement(params);
  if (strlen(params)>64) params[64]=0;
  format(buffer,sizeof buffer,"Ann: {b}%s%s{/b}, autor: {b}%s{/b}", params, strlen(params)==64?("..."):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  MSGToAdmins(COLOR_INFO, buffer, false, LEVEL_GM);
  return 1;
}

CMD:r(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "/r {b}<tekst>{/b}");
  new offset=0;
  while (offset<strlen(params)) {
    new line[90];
    strmid(line, params, offset, offset+70);
    format(line, sizeof line, "%s{ff1010}%s", ((offset>0) ? ("   ") : ">> "),line);
    SendClientMessageToAll(0xffffffff, line);
    offset+=70;
  }
  return 1;
}

CMD:ann3(playerid, params[])
{
  if(!IsAdmin(playerid)) return 0;
  new txt[80];
  if (sscanf(params,"s[80]",txt))
    return Msg(playerid, COLOR_ERROR, "/ann3 {b}<tekst>{/b}, maksymalnie 80 znakow"); // Uøywaj: /ann <tekst>
  if(!CheckTildes(txt)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // IloúÊ tyld "~" musi byÊ parzysta.
  unpliterki(params);

  new buffer[140];
  format(buffer, sizeof buffer, "%s: %s", GetPlayerNick(playerid), txt);
  OutputLog(LOG_ANN, buffer);
  format(buffer,sizeof buffer,"~n~%s",txt);
  foreach(player){
    if(pData[player][hudSetting][HUD_ANNOUNCEMENTS]){
      GameTextForPlayer(player,buffer,6000,5);
    }
  }

  if (strlen(txt)>64) txt[64]=0;
  format(buffer,sizeof buffer,"Ann: {b}%s%s{/b}, autor: {b}%s{/b}", txt, strlen(txt)==64?("..."):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  MSGToAdmins(COLOR_INFO, buffer, false, LEVEL_GM);


  return 1;
}

CMD:makesober(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;
  new userid;
  if (sscanf(params,"d",userid))
    return Msg(playerid,COLOR_ERROR,"/makesober {b}<id>{/b}");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest aktywny");
  SetPlayerDrunkLevel(userid,0);
  return Msg(playerid,COLOR_ERROR,"Gracz 'otrzezwiony'");
}

CMD:unwiezall(playerid){
  new buffer[128], cnt;
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  foreach(i)
    if (pData[i][jail]>0) {
        format(buffer,sizeof buffer,"%d", i);
        _unwiez(playerid,buffer);
        cnt++;
    }

  format(buffer,sizeof buffer,"Wypuszczono z wiezienia %d %s", cnt, dli(cnt,"gracza","graczy","graczy"));
  Msg(playerid,COLOR_INFO,buffer);
  return 1;

}

CMD:cenz(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze objeci cenzura:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][cenzura]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          buffer[0]=0;
        }
      }
    if (buffer[0]!=0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/cenz [id/nick]{/b}. Ponowne uzycie komendy wylaczy cenzure.",false);
  }

  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  else if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  } else if (targetplayerid==playerid)
    return Msg(playerid, COLOR_ERROR, "Nie mozesz ocenzurowac siebie.");

  if(pData[targetplayerid][adminLevel]>0 && pData[targetplayerid][adminLevel]!=LEVEL_ADMINHIDDEN && pData[targetplayerid][adminLevel]>pData[playerid][adminLevel]) {
    format(buffer,sizeof buffer,"Admin %s (%d) probowal Cie o(d)cenzurowac, ale niestety jest nizszego poziomu niz Ty.",GetPlayerProperName(playerid),playerid);
    Msg(targetplayerid,COLOR_ADMIN, buffer);
    return Msg(playerid, COLOR_ERROR, "Nie mozesz o(d)cenzurowac admina wyzszego poziomu!");
  }
  if(pTemp[targetplayerid][cenzura]) {
    new bool:kobieta=Kobieta(playerid),
      bool:kobietat=Kobieta(targetplayerid);
    pTemp[targetplayerid][cenzura]=false;
    format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odcenzurowal%s Cie - mozesz juz uzywac komend.",
      kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, kobietat?("~r~zostalas odcenzurowana"):("~r~zostales odcenzurowany!"), 4000, 1);
    format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s odcenzurowan%s przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
      kobietat?("a"):("y"), GetPlayerProperName(playerid));
    foreach(i)
      if (i!=targetplayerid)
        Msg(i,COLOR_ADMIN, buffer, false);
  } else {
    new bool:kobieta=Kobieta(playerid),
      bool:kobietat=Kobieta(targetplayerid);

    if (pData[targetplayerid][adminLevel]!=LEVEL_ADMINHIDDEN)
      pTemp[targetplayerid][cenzura]=true;

    format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} nalozyl%s na Ciebie cenzure - nie mozesz uzywac komend.",
      kobieta?("ka"):(""), pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN?(""):GetPlayerProperName(playerid), kobieta?("a"):(""));
    Msg(targetplayerid, COLOR_ADMIN, buffer, false);

    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, kobietat?("~r~zostalas ocenzurowana"):("~r~zostales ocenzurowany!"), 4000, 1);
    format(buffer,sizeof buffer,"{b}%s (%d){/b} zostal%s ocenzurowan%s przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), targetplayerid, kobietat?("a"):(""),
      kobietat?("a"):("y"), GetPlayerProperName(playerid));
    foreach(i)
      if (i!=targetplayerid)
        Msg(i,COLOR_ADMIN, buffer, false);

    if (pData[targetplayerid][adminLevel]==LEVEL_ADMINHIDDEN)
      Msg(targetplayerid, COLOR_ADMIN, "Z racji Twojego poziomu admina, cenzura nie zostala faktycznie wlaczona, ale nikt o tym nie wie.");

  }

  return 1;
}

CMD:drop(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze izolowani od reszty:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][cheater]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          buffer[0]=0;
        }
      }
    if (buffer[0]!=0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/drop [id/nick]{/b}. Ponowne uzycie komendy wylaczy dzialanie.",false);
  }

  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  else if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

    new bool:kobieta=Kobieta(playerid);
  if(pTemp[targetplayerid][cheater]) {
    pTemp[targetplayerid][cheater]=false;
    SetPlayerVirtualWorld(targetplayerid,0);
    Msg(playerid,COLOR_INFO,"Wylaczono izolacje gracza od reszty.");
    SetPlayerHealth(targetplayerid,0);
    format(buffer,sizeof buffer,"{b}%s{/b} WYLACZYL%s izolacje gracza {b}%s{/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("A"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer, true);
  } else {
    pTemp[targetplayerid][cheater]=true;
    pData[targetplayerid][pAttraction] = A_NONE;
    pTemp[targetplayerid][onArena]=ARENA_NONE;
    SetPlayerVirtualWorld(targetplayerid,VW_TROLLE);
    Msg(playerid,COLOR_INFO,"Wlaczono izolacje gracza od reszty.");
    SetPlayerHealth(targetplayerid,0);
    format(buffer,sizeof buffer,"{b}%s{/b} WLACZYL%s izolacje gracza {b}%s{/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("A"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer, true);
  }

  return 1;
}

CMD:protping(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze z protem na pinga:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][protping]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          buffer[0]=0;
        }
      }
    if (buffer[0]!=0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/protping [id/nick]{/b}. Ponowne uzycie komendy wylaczy prota.",false);
  }

  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

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
  if(pTemp[targetplayerid][protping]) {
    new bool:kobieta=Kobieta(playerid);
    pTemp[targetplayerid][protping]=false;
    pTemp[targetplayerid][weaponsAllowed]=true;
    format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odebral%s Ci zezwolenie na wyzszy ping.",
      kobieta?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));
    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    Msg(targetplayerid, COLOR_ADMIN, "Zdjeto z Ciebie ograniczenia posiadania broni.");

    PlaySound(targetplayerid, 1137);
  } else {
    new bool:kobieta=Kobieta(playerid);

    pTemp[targetplayerid][protping]=true;
    pTemp[targetplayerid][weaponsAllowed]=false;

    format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} dal%s Ci zezwolenie na wyzszy ping.",
      kobieta?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));

    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    Msg(targetplayerid, COLOR_ADMIN, "Pamietaj: z zezwoleniem na wyzszy ping nie mozesz uczestniczyc w DM.", false);
    PlaySound(targetplayerid, 1137);

  }
  Msg(playerid,COLOR_INFO,pTemp[targetplayerid][protping]?("Nadano pozwolenie na wyzszy ping"):("Odebrano pozwolenie na wyzszy ping"));

  return 1;
}

CMD:bezbroni(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze z zakazem posiadania broni:",false);
    new cnt;
    foreach(i)
      if (!pTemp[i][weaponsAllowed]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d) ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          buffer[0]=0;
        }
      }
    if (buffer[0]!=0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/bezbroni [id/nick]{/b}. Ponowne uzycie komendy wylaczy zakaz.",false);
  }

  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

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
  if(pTemp[targetplayerid][weaponsAllowed]) {
    new bool:kobieta=Kobieta(playerid);
    pTemp[targetplayerid][weaponsAllowed]=false;
    format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} nalozyl%s na Ciebie zakaz uzywania broni.",
      kobieta?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));
    Msg(targetplayerid, COLOR_ADMIN, buffer, false);

    PlaySound(targetplayerid, 1137);
  } else if (pTemp[targetplayerid][protping])
    return Msg(playerid, COLOR_ERROR, "Graczowi z /protping nie mozna zdjac zakazu uzywania broni.");
  else {
    new bool:kobieta=Kobieta(playerid);

    pTemp[targetplayerid][weaponsAllowed]=true;

    format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} zdj%s z Ciebie zakaz uzywania broni.",
      kobieta?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("ela"):("al"));

    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    PlaySound(targetplayerid, 1137);
  }
  Msg(playerid,COLOR_INFO,pTemp[targetplayerid][weaponsAllowed]?("Zdjeto zakaz uzywania broni."):("Nalozono zakaz uzywania broni."));

  return 1;
}

CMD:troll(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN2)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Trolle na serwerze:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][troll]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          copy("", buffer);
        }
      }
    if (strlen(buffer)>0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"troll","trolle","trolli"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/troll [id/nick]{/b}. Ponowne uzycie komendy wylaczy ograniczenie.",false);
  }

  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

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
  if(!pTemp[targetplayerid][troll]) {
    pTemp[targetplayerid][troll]=true;
    Msg(playerid,COLOR_INFO, "Nalozyles ograniczenie z gracza");
    format(buffer,sizeof buffer,"{b}%s{/b} NALOZYL%s ninja-mute na gracza {b}%s{/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("A"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer, true, LEVEL_ADMIN2);
  } else {
    pTemp[targetplayerid][troll]=false;
    Msg(playerid, COLOR_INFO,"Zadjales na gracza ograniczenie");
    format(buffer,sizeof buffer,"{b}%s{/b} ZDJ%sL%s ninja-mute z gracza {b}%s{/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid),Kobieta(playerid)?("E"):("A"), Kobieta(playerid)?("A"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer, true, LEVEL_ADMIN2);
  }

  return 1;
}

CMD:resetaudio(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
  Audio_SetPack("fullserver", true);
  return 1;
}

CMD:a(playerid, params[])
{
  if(!IsAdmin(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 133)); // Uøywaj: /a <tekst>

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  OutputAdminChat(playerid, params);

  return 1;
}

CMD:nearvehid(playerid){
  MUST_BE_SPAWNED(playerid);
  if(!IsAdmin(playerid)) return 0;

  new vid=GetNearestVehicleToPlayer(playerid);
  if (vid==INVALID_VEHICLE_ID)
    return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego pojazdu");

  new str[127];
  new Float:VP[3];
  GetVehiclePos(vid,VP[0],VP[1],VP[2]);
  format(str,sizeof str, "Najblizszy pojazd: %d, model: %d, polozenie %.1f %.1f %.1f", vid, GetVehicleModel(vid), VP[0],VP[1],VP[2]);
  return Msg(playerid,COLOR_INFO2,str);

}

CMD:startvote(playerid,params[]){
  return _glosowanie(playerid,params);
}

CMD:stopvote(playerid){
  return _glosowanie(playerid,"stop");
}

CMD:rrsp(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;
    new userid;
    if (sscanf(params,"d",userid))
    userid=playerid;
  else if (!IsPlayerConnected(userid))
    return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
  SpawnPlayer(userid);
  return 1;
}

CMD:warn(playerid,params[]){

  if(!IsAdmin(playerid)) return 0;

  new tplayer[24],userid,reason[127],bool:kickPlayer=false;

  if (!sscanf(params,"s[24]s[127]",tplayer,reason)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"Uzyj: /warn <id/nick> <powod>");

  if (GetPlayerCurrentSession(userid)<=5)
    return Msg(playerid, COLOR_ERROR, "Ten gracz dopiero sie polaczyl.");

  if (strlen(reason)>90)
    return Msg(playerid,COLOR_ERROR,"Powod warna jest zbyt dlugi");

  if (pTemp[userid][warningReceived]==0)  // pierwsze ostrzezenie
    pTemp[userid][warningReceived]=GetTickCount();
  else if (GetTickCount()-pTemp[userid][warningReceived]<15000)
    return Msg(playerid,0xff6060ff,"Ten gracz przed chwila otrzymal ostrzezenie.");
  else if (pTemp[userid][warningReceived]>0)
    kickPlayer=true;


  new buf[255];
  format(buf,sizeof buf,"{b}%s{/b} otrzymal/-a ostrzezenie od administratora!",GetPlayerProperName(userid));
  foreach(i)
    Msg(i,COLOR_ADMIN,buf);

    format(buf,sizeof buf,"PowÛd: {b}%s{/b}",reason);
  foreach(i)
    Msg(i,COLOR_ADMIN,buf);

  TextDrawShowForPlayer(userid, gTextDraw[TD_WARNING]);
  SendClientMessage(userid,-1," ");
  SendClientMessage(userid,0xff0000ff,"-------------------------------------------------");
  SendClientMessage(userid,0xff0000ff,"OTRZYMANO OSTRZEZENIE OD ADMINISTRATORA!");
  SendClientMessage(userid,0xffffffff,reason);
  SendClientMessage(userid,0xff0000ff,"-------------------------------------------------");
  SendClientMessage(userid,-1," ");

  unpliterki(reason);

  GivePlayerScore(userid,-2,"warn");

  format(buf,sizeof buf,"~n~_~n~~r~%s",reason);
  GameTextForPlayer(userid,buf,12000,5);

  if (kickPlayer && !(userid!=playerid && pData[userid][loggedIn] && pData[userid][adminLevel]>pData[playerid][adminLevel] && pData[userid][adminLevel]<LEVEL_ADMINHIDDEN  && pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN))
    return KickPlayerWithReason(userid,playerid,TXT(userid,465));   // user ignores warnings

  PlaySound(userid,1053);
  if (Audio_IsClientConnected(userid))
    Audio_Play(userid,AUDIOID_WARN_MALE,false,false,true);

  SetTimerEx("HidePlayerWarning",5000,false,"d",userid);
  return 1;
}

CMD:unwarn(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;

  new tplayer[24],userid;

  if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"Uzyj: /unwarn <id/nick>");

  if (pTemp[userid][warningReceived]==0)
    return Msg(playerid,COLOR_INFO,"Gracz nie ma zadnych ostrzezen");
  pTemp[userid][warningReceived]=0;
  Msg(userid,COLOR_INFO,TXT(userid,464));
  return Msg(playerid,COLOR_INFO,"Ostrzezenia zostaly usuniete");
}

CMD:raceend(playerid){
  if(!IsAdmin(playerid)) return 0;
  Race_Cleanup();
  aData[A_RACE][aState] = A_STATE_OFF;
  return 1;
}

CMD:driftend(playerid){
  if(!IsAdmin(playerid)) return 0;
  aData[A_DRIFT][aState] = A_STATE_OFF;
  Drift_Cleanup();
  aData[A_DRIFT][aState] = A_STATE_OFF;
  return 1;
}

CMD:wgend(playerid){
  if(!IsAdmin(playerid)) return 0;
  WG_Finish();
  aData[A_WG][aState] = A_STATE_OFF;
  return 1;
}

CMD:ggend(playerid){
  if(!IsAdmin(playerid)) return 0;
  GG_Finish();
  aData[A_GG][aState] = A_STATE_OFF;
  return 1;
}

CMD:ctfend(playerid){
  if(!IsAdmin(playerid)) return 0;
  CTF_Finish();
  aData[A_WG][aState] = A_STATE_OFF;
  return 1;
}

CMD:chend(playerid){
  if(!IsAdmin(playerid)) return 0;
  CH_Finish();
  return 1;
}

CMD:derbyend(playerid){
  if(!IsAdmin(playerid)) return 0;
  Derby_Cleanup();
  aData[A_DERBY][aState] = A_STATE_OFF;
  return 1;
}

CMD:spsend(playerid){
    if(!IsAdmin(playerid)) return 0;
  HAY_Finish();
  HAY_Cleanup();
  aData[A_HAY][aState] = A_STATE_OFF;
  return 1;
}

CMD:strzelend(playerid){
    if(!IsAdmin(playerid)) return 0;
  strzelnica_AttractionUpdate(0,true);
  aData[A_STRZELNICA][aState] = A_STATE_OFF;
  return 1;
}

CMD:info(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;

  new tplayer[24],uid;

  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    uid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/info [id/nick]");

  if (!IsPlayerConnected(uid))
    return Msg(playerid,COLOR_ERROR,"Wskazany gracz nie jest podlaczony.");

  SendClientMessage(playerid,0xffffffff,"");
  new buf[128],Float:hp,Float:ar;
  GetPlayerHealth(uid,hp);
  GetPlayerArmour(uid,ar);

  if(!pTemp[uid][godMode]) format(buf,sizeof buf,"NICK {b}%s{/b}, ID: {b}%d{/b} HP: {b}%03.1f{/b}, AR: {b}%03.1f{/b}", GetPlayerNick(uid), uid, hp,ar);
  else format(buf,sizeof buf,"NICK {b}%s{/b}, ID: {b}%d{/b} TRYB PASYWNY", GetPlayerNick(uid), uid);
  if (pTemp[uid][staleTime]>1) format(buf, sizeof buf, "%s, AFK od %ds", buf, pTemp[uid][staleTime]);
  Msg(playerid,COLOR_INFO,buf,false);

  new serial[50];
  gpci(uid, serial, sizeof serial);
  format(buf,sizeof buf,"SERIAL: {b}%s{/b}", serial);
  Msg(playerid, COLOR_INFO, buf, false);

  if (pData[uid][suspended])
    Msg(playerid,COLOR_INFO2,"Gracz zawieszony w randze.", false);

  if ((pData[uid][pAttraction] == A_WG || pData[uid][pAttraction] == A_CTF) && hp>99)
    Msg(playerid, COLOR_ERROR, "Gracz ma 100HP, chociaz max to 99. Prawdopodobny HH lub lost");
  if ((pData[uid][pAttraction] == A_WG || pData[uid][pAttraction] == A_CTF) && ar>0)
    Msg(playerid, COLOR_ERROR, "Gracz ma 100AR, chociaz powinien miec 0. Prawdopodobny AH lub lost");

  format(buf,sizeof buf,"IP: %s. PING aktualny: {b}%d{/b}ms, sredni: {b}%d{/b}ms", GetPlayerIP(uid), GetPlayerPing(uid), pData[uid][averagePing]);
  Msg(playerid,COLOR_INFO,buf,false);
  format(buf,sizeof buf,"%s.",(pData[uid][loggedIn]) ? ("ZALOGOWANY") : ("NIEZALOGOWANY") );
  Msg(playerid,COLOR_INFO,buf,false);
  format(buf,sizeof buf,"Resp: {b}%d{/b}, Skill: {b}%d{/b}, GP: {b}%d{/b}, Kasa: %d, Gang: %d, killStreak: {b}%d{/b}",
    pData[uid][respect], pData[uid][skill], pData[uid][gamep], GetPlayerMoney(uid), pData[uid][gang], pTemp[uid][killStreak] );
  Msg(playerid,COLOR_INFO,buf,false);

  new playerState = GetPlayerState(uid);
  switch (playerState){
    case PLAYER_STATE_NONE:
      format(buf,sizeof buf,"Stan: {b}NONE{/b}");
    case PLAYER_STATE_ONFOOT:
      format(buf,sizeof buf,"Stan: {b}PIESZY{/b}");
    case PLAYER_STATE_DRIVER:
      format(buf,sizeof buf,"Stan: {b}KIEROWCA{/b}");
    case PLAYER_STATE_PASSENGER:
      format(buf,sizeof buf,"Stan: {b}PASAZER{/b}");
    case 4:
      format(buf,sizeof buf,"Stan: {b}WYSIADA Z POJAZDU{/b}");
    case 5:
      format(buf,sizeof buf,"Stan: {b}WSIADA JAKO KIEROWCA{/b}");
    case 6:
      format(buf,sizeof buf,"Stan: {b}WSIADA JAKO PASAZER{/b}");
    case PLAYER_STATE_WASTED:
      format(buf,sizeof buf,"Stan: {b}MARTWY{/b}");
    case PLAYER_STATE_SPAWNED:
      format(buf,sizeof buf,"Stan: {b}ZESPAWNOWANY{/b}");
    case PLAYER_STATE_SPECTATING:
      format(buf,sizeof buf,"Stan: {b}SPEC{/b}");
  }
  new specact=GetPlayerSpecialAction(uid);
  new aniidx=GetPlayerAnimationIndex(uid);

  new animlib[32]="-";
  new animname[32]="-";
  if (aniidx)
    GetAnimationName(aniidx,animlib,32,animname,32);

  format(buf,sizeof buf,"%s, akcja: {b}%d{/b}, animacja {b}%s:%s (%d){/b}, A.FPS: %d",buf,specact, animlib,animname, aniidx, pTemp[uid][ept_fps]);

//  format(buf,sizeof buf,"A.FPS: %d", pData[playerid][hudSetting][HUD_FPS]?pTemp[uid][ept_fps]:-1);

  if (pData[uid][pAttraction]>=0 && pData[uid][pAttraction]<A_ARENA)  { // quick bugfix for array out of bounds
    format(buf,sizeof buf, "%s Atrakcja: %s", buf, aData[pData[uid][pAttraction]][aName]);
  }
  Msg(playerid,COLOR_INFO,buf, false);

  new cursess=GetPlayerCurrentSession(uid);
  format(buf,sizeof buf, "W grze od {b}%dm%02ds{/b}.", floatround(cursess/60, floatround_floor), cursess%60 );

  if (gmData[artefactOwner]==uid) {
    format(buf, sizeof buf, "%sNosi artefakt od {b}%ds{/b}", buf, ((GetTickCount() / 1000) - pTemp[gmData[artefactOwner]][artefactStartTime]));
  }

  Msg(playerid,COLOR_INFO,buf,false);

  format(buf,sizeof buf,"%d", uid); // lista broni
  _cwep(playerid,buf);

  if(IsAdmin(playerid,LEVEL_ADMIN3) && pData[uid][logonDialog])
    Msg(playerid,COLOR_INFO,"Gracz w trakcie logowania",false);

  if (IsPlayerInAnyVehicle(uid)){
    new vid=GetPlayerVehicleID(uid);
    format(buf, sizeof buf, "Pojazd ID %d.", vid);
    Msg(playerid,COLOR_INFO,buf,false);
  }
  return 1;
}

CMD:restartaudio(playerid,params[]){
    if(!IsAdmin(playerid)) return 0;
    new userid;
    if (sscanf(params,"d",userid))
        userid=playerid;
    else if (!IsPlayerConnected(userid))
        return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
  Audio_SetPack("fullserver", true);
  return 1;
}

CMD:givegun(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_GM)) return 0;
  new userid,wepid,ammo;
    if (sscanf(params,"ddd",userid,wepid,ammo))
    return Msg(playerid,COLOR_INFO,"/givegun {b}<id gracza> <id broni> <amunicja>{/b}");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
    return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
  if (wepid<=15 && ammo>1)
    ammo=1;
  if (ammo>10000 || ammo<0)
    ammo=10000;

  GivePlayerWeapon(userid, wepid, ammo);
  if (wepid<=15)
    SetPlayerAmmo(userid,wepid,1);

  return Msg(playerid,COLOR_INFO,"Done.");
}

CMD:gga(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;
  new wepid,ammo;
    if (sscanf(params,"dD(5000)",wepid,ammo))
    return Msg(playerid,COLOR_INFO,"/gga {b}<id broni> [amunicja]{/b}");

  if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
    return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
  if (wepid<=15 && ammo>1)
    ammo=1;
  if (ammo>50000 || ammo<0)
    ammo=50000;
  new cnt;
  new buffer[128];

  format(buffer,sizeof buffer,"Administrator%s %s rozdal%s wszystkim bron: {b}%s{/b}.", Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""), weaponName[wepid]);
  foreach(i)
  if (pData[i][pAttraction]==A_NONE && pTemp[i][weaponsAllowed])
  {
    GivePlayerWeapon(i, wepid, ammo);
    if (wepid<=15)
      SetPlayerAmmo(i,wepid,1);
    else if (ammo==50000)
      SetPlayerAmmo(i,wepid,ammo);
    Msg(i,COLOR_INFO,buffer,true);
    cnt++;
  }

  format(buffer,sizeof buffer,"Bron otrzymalo {b}%d %s{/b}.", cnt, dli(cnt,"gracz","graczy","graczy"));

  return Msg(playerid,COLOR_INFO,buffer);
}

CMD:gga2(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;
  new wepid,ammo,zasieg,
    pVector2[e_Vectors],
    int,vw;

    if (sscanf(params,"D(50)dD(5000)",zasieg,wepid,ammo))
    return Msg(playerid,COLOR_INFO,"/gga2 {b}<zasieg 1-100> <id broni> [amunicja]{/b}");
  if (zasieg>100 || zasieg<1)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-100{/b}");

  if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
    return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
  if (wepid<=15 && ammo>1)
    ammo=1;
  if (ammo>50000 || ammo<0)
    ammo=50000;
  new cnt;
  new buffer[128];

    int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  foreach(i)
  if (pData[i][pAttraction]==A_NONE && pTemp[i][weaponsAllowed])
  {
      if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
    {
      if(playerid != i && GetPlayerDistanceFromPoint(i, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
      {
        GivePlayerWeapon(i, wepid, ammo);
        if (wepid<=15)
          SetPlayerAmmo(i,wepid,1);
        else if (ammo==50000)
          SetPlayerAmmo(i,wepid,ammo);
        cnt++;
      }
    }
  }

  format(buffer,sizeof buffer,"Bron otrzymal%s {b}%d %s{/b}.", dli(cnt,"l","o","o"), cnt, dli(cnt,"gracz","graczy","graczy"));

  return Msg(playerid,COLOR_INFO,buffer);
}

CMD:vneon(playerid,params[])
{
  if (!IsAdmin(playerid)) return 0;
  new nnn;
  if (sscanf(params,"d",nnn))
    return Msg(playerid,COLOR_INFO2,"/vneon <id neonu (0-9)>");
  if(nnn > 9 || nnn < 0) return Msg(playerid,COLOR_INFO2,"/vneon <id neonu (0-9)>");
  
  new Float:vehX, Float:vehY, Float:vehZ;
  GetVehiclePos(neony[nnn][2], vehX, vehY, vehZ);
  SetPlayerPos(playerid, vehX, vehY, vehZ);
  
  return 1;
}

CMD:forcearena(playerid,params[]){
  if (!IsAdmin(playerid)) return 0;
  new atrakcja[6],arenaid;
  if (sscanf(params,"s[6]d",atrakcja,arenaid))
    return Msg(playerid,COLOR_INFO2,"/forcearena <derby/race/drift/wg/ch/chowany> <id>");
  if (strcmp(atrakcja,"derby")==0)
    gmTemp[aDerbyForceArena]=arenaid;
  else if (strcmp(atrakcja,"race")==0)
    gmTemp[aRaceForceCircuit]=arenaid;
  else if (strcmp(atrakcja,"drift")==0)
    gmTemp[aDriftForceCircuit]=arenaid;
  else if (strcmp(atrakcja,"wg")==0)
    gmTemp[aWGForceArena]=arenaid;
    else if (strcmp(atrakcja,"ctf")==0)
    gmTemp[aCTFForceArena]=arenaid;
  else if (strcmp(atrakcja,"ch")==0 || strcmp(atrakcja,"chowany")==0)
    gmTemp[aChowanyForceArena]=arenaid;
  else
    return Msg(playerid,COLOR_ERROR,"Nieprawidlowa nazwa atrakcji");
  Msg(playerid,COLOR_INFO,"Ustawione");
  return 1;
}

CMD:cwep(playerid,params[]){
  if (!IsAdmin(playerid)) return 0;
  new uid,tplayer[24];
  if (sscanf(params,"s[24]",tplayer))
    return Msg(playerid,COLOR_INFO2,"/cwep <id/nick>");

  if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;

  GetPlayerWeaponList(playerid,uid);
  return 1;
}

CMD:cgod(playerid){
  if (!IsAdmin(playerid)) return 0;
  new Float:AR, Float:HP,score;
  foreach(i)
   if (IsPlayerSpawned(i)) {
    GetPlayerHealth(i,HP);
    GetPlayerArmour(i,AR);
    score=0;
    new buf[127];
    if ((pData[i][loggedIn] && pTemp[i][killStreak]>10) || (!pData[i][loggedIn] && pTemp[i][killStreak]>5)) {
      score++;
      format(buf, sizeof buf, ", duzo zabic pod rzad");
    }
    if (HP>100 && !(pData[i][pAttraction]==A_CHOWANY && pTemp[i][aChowanySide] == A_CHOWANY_SEARCHING)) {
      score++;
      format(buf, sizeof buf, "%s, god otrzymany od admina/serwera", buf);
    }
    if (HP>99 &&
        ((pData[i][pAttraction]==A_CHOWANY && pTemp[i][aChowanySide] == A_CHOWANY_HIDDING) || (pData[i][pAttraction]==A_WG))) {
      score++;
      format(buf, sizeof buf, "%s, prawdopodobny HH", buf);
    }
    if (score>0) {
      format(gstr, sizeof gstr, "%s(%d) - HP: %3.0f, AR: %3.0f, zabic pod rzad: %d%s", GetPlayerNick(i), i, HP, AR, pTemp[i][killStreak], buf);
      SCM(playerid, -1, gstr);
    }
   }
  SCM(playerid, -1, "--");
  return 1;

}

CMD:killplayer(playerid, params[])
{
  if (!IsAdmin(playerid)) return 0;

  if (isnull(params)) {
    SetPlayerHealth(playerid,0.0);
    return Msg(playerid, COLOR_INFO, "Zabiles/-as sie", false);
  }

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

  if(!IsPlayerSpawned(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 379));

  SetPlayerHealth(targetplayerid, 0.0);

  format(buffer, sizeof buffer, TXT(playerid, 365), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer);

  if(targetplayerid == playerid) return 1;

  format(buffer, sizeof buffer, TXT(playerid, 366), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer);

  return 1;
}

/*CMD:playall(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
  new audioid[128];
  if (sscanf(params,"s[128]",audioid)){
    Msg(playerid, COLOR_ERROR, "/playall <adres>");
    return Msg(playerid, COLOR_ERROR, "Uzyj {b}'stop'{/b} jako adres, aby zatrzymac.");
  }

  new cnt=0;
  if(strfind(audioid, "stop", true) != -1){
    foreach (i){
        if(pTemp[i][audioHandleId]!=-1){
        if (Audio_IsClientConnected(i) && pTemp[i][playPlayall]==true){
          CallRemoteFunction("WylaczRadio","d",i);
          Audio_Stop(i,pTemp[i][audioHandleId]);
          pTemp[i][audioHandleId]=-1;
          cnt++;
        }
      }
    }
    new buf[128];
    format(buf,sizeof buf,"Zatrzymano u {b}%d{/b} graczy", cnt);
    Msg(playerid,COLOR_ERROR,buf);
    format(buf,sizeof buf,"Administrator%s {b}%s{/b} zatrzymal komunikat glosowy (/playall)",Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid));
    MSGToAdmins(COLOR_ERROR, buf,true,LEVEL_ADMIN2);
  }else{
    if (pData[playerid][adminLevel]<=LEVEL_ADMIN2 && GetTickCount()-gmTemp[playallLast]<(60*1000*10))
      return Msg(playerid,COLOR_ERROR,"Musi minac przynajmniej {b}10 minut{/b} od ostatniego odtwarzania.");
    gmTemp[playallLast]=GetTickCount();
    foreach (i)
      if (Audio_IsClientConnected(i) && pTemp[i][playPlayall]==true){
          CallRemoteFunction("WylaczRadio","d",i);
          Audio_Stop(i,pTemp[i][audioHandleId]);
        pTemp[i][audioHandleId]=Audio_PlayStreamed(i,audioid);
        cnt++;
      }
    new buf[128];
    format(buf,sizeof buf,"Odtworzono u {b}%d{/b} graczy", cnt);
    Msg(playerid,COLOR_ERROR,buf);
    format(buf,sizeof buf,"Administrator%s {b}%s{/b} uruchomil komunikat glosowy (/playall)",Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid));
    MSGToAdmins(COLOR_ERROR, buf,true,LEVEL_ADMIN2);
  }
  return 1;
}*/

CMD:visibility(playerid,params[]){
  if (!IsAdmin(playerid)) return 0;

  new tplayer[24],vis,uid;
  if (!sscanf(params,"s[24]d",tplayer,vis)) {
    if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  } else if (!sscanf(params,"d",vis)) {
    uid=playerid;
  } else
    return Msg(playerid,COLOR_ERROR,"/visibility [id gracza] <widocznosc 0-100>");

  if (vis<0 || vis>100)
    return Msg(playerid,COLOR_ERROR,"Podaj widocznosc w zakresie od 0 do 100");

  pTemp[uid][playerColorAlpha]=floatround(0xFF*vis/100,floatround_floor);
  SetPlayerProperColor(uid);
  return Msg(playerid,COLOR_INFO,"Ustawione");
}

CMD:cwh(playerid){
  if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;
  new buf[128];
  foreach(i){
      new wepid,ammo, podejrzany=0;
      for (new slot=0; slot<=12;slot++) {
      wepid=0; ammo=0;
          GetPlayerWeaponData(i,slot,wepid,ammo);
          if (IsGM(i)) continue;
      if (wepid==0 && ammo==0) continue;
      if (wepid>=0 && wepid<=15 && ammo>1) podejrzany+=(ammo<=3?1:2);
      if (ammo==198) podejrzany+=2;
      if ((wepid==36 || wepid==44 || wepid==45 || wepid==39) && !IsPlayerInFullDMArea(i)) podejrzany+=2;
      if ((pData[i][pAttraction]!=A_ARENA && ((wepid==38 && pData[i][pAttraction]!=A_CHOWANY) || wepid==37) && !IsPlayerInFullDMArea(i))) podejrzany++;
      if ((ammo<0 && pData[i][pAttraction]!=A_CHOWANY) || ammo>9000) podejrzany++;
      }
    if (podejrzany>1) {
      format(buf, sizeof buf, "%s(%d) - {b}%d{/b} %s %s", GetPlayerNick(i), i, podejrzany, dli(podejrzany,"punkt","punkty","puntkow"), pTemp[i][cheater]?("ODIZOLOWANY"):(""));
      Msg(playerid,COLOR_INFO,buf,false);
      format(buf, sizeof buf, "%d", i);
      _cwep(playerid,buf);
    }
  }
  SCM(playerid, -1, "--");
  return 1;
}

CMD:toggle(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN2)) return 0;

  if (!strcmp(params,"chat",true) && !isnull(params)) {
    if (!IsAdmin(playerid, LEVEL_ADMIN3)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <podgladpm|protall|join|bombs|troll>{/b}");
    gmTemp[chatDisabled]=(gmTemp[chatDisabled] ? false : true);
    SendClientMessageToAll(-1," ");
    if (gmTemp[chatDisabled])
      SendClientMessageToAll(0xFF6060,"Czat zostal wylaczony przez administratora.");
    else
      SendClientMessageToAll(0xFF6060,"Czat zostal wlaczony.");
    SendClientMessageToAll(-1," ");

    return 1;
  } else if (!strcmp(params,"podgladpm",true) && !isnull(params)) {
    gmTemp[snoopPM]=((gmTemp[snoopPM]+1)%3);
    switch (gmTemp[snoopPM]) {
      case 0:
        MSGToAdmins(COLOR_INFO2, "Podglad wiadomosci PM zostal {b}wylaczony{/b}", true, LEVEL_ADMIN2);
      case 1: {
        MSGToAdmins(COLOR_INFO2, "Podglad wiadomosci PM zostal {b}wlaczony, wiadomosci graczy <1000 respektu{/b}", true, LEVEL_ADMIN2);
      }
      case 2:
        MSGToAdmins(COLOR_INFO2, "Podglad wiadomosci PM zostal {b}wlaczony, dla wszystkich wiadomosci{/b}", true, LEVEL_ADMIN2);
    }

    return 1;
  } else if (!strcmp(params,"join",true) && !isnull(params)) {
    gmTemp[showJoins]=((gmTemp[showJoins]+1)%3);
    switch (gmTemp[showJoins]) {
      case 0:
        MSGToAdmins(COLOR_INFO2, "Informacje o wejsciu/wyjsciu z serwera {b}wylaczone{/b}", true, LEVEL_ADMIN2);
      case 1:
        MSGToAdmins(COLOR_INFO2, "Informacje o wejsciu/wyjsciu z serwera {b}wlaczone, widoczne dla adminow{/b}", true, LEVEL_ADMIN2);
      case 2:
        MSGToAdmins(COLOR_INFO2, "Informacje o wejsciu/wyjsciu z serwera {b}wlaczone, widoczne dla wszystkich graczy{/b}", true, LEVEL_ADMIN2);
    }
    return 1;
  } else if (!strcmp(params,"protall",true) && !isnull(params)) {
    gmTemp[protAll]=(gmTemp[protAll] ? false : true);
    foreach(i) {
      SendClientMessage(i,-1, " ");
      if (gmTemp[protAll]) {
        Msg(i,COLOR_ERROR,"WSZYSCY GRACZE OTRZYMALI IMMUNITET.");
        GameTextForPlayer(i, "~r~Wszyscy gracze ~n~~h~otrzymali immunitet", 2000, 1);
      }
      else  {
        Msg(i,COLOR_ERROR,"WSZYSCY GRACZE UTRACILI IMMUNITET.");
        GameTextForPlayer(i, "~r~Wszyscy gracze ~n~~h~utracili immunitet", 2000, 1);
      }

      SendClientMessage(i,-1, " ");
    }
    return 1;
  } else if (!strcmp(params,"bombs",true) && !isnull(params)) {
    gmTemp[isBombs]=(gmTemp[isBombs] ? false : true);
    if (gmTemp[isBombs])
      MSGToAdmins(COLOR_INFO2, "Podkladanie bomb {b}wlaczone{/b}", true, LEVEL_ADMIN2);
    else
      MSGToAdmins(COLOR_INFO2, "Podkladanie bomb {b}wylaczone{/b}", true, LEVEL_ADMIN2);

    return 1;
  } else if (!strcmp(params,"troll",true) && !isnull(params)) {
    gmTemp[showTroll]=(gmTemp[showTroll] ? false : true);
    if (gmTemp[showTroll])
      MSGToAdmins(COLOR_INFO2, "Pokazywanie wiadomosci /trolli {b}wlaczone{/b}", true, LEVEL_ADMIN2);
    else
      MSGToAdmins(COLOR_INFO2, "Pokazywanie wiadomosci /trolli {b}wylaczone{/b}", true, LEVEL_ADMIN2);

    return 1;
  } else if (!strcmp(params,"gang",true) && !isnull(params)) {
      if (!IsAdmin(playerid, LEVEL_ADMIN3)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <podgladpm|protall|join|bombs|troll>{/b}");
    gmTemp[showGang]=(gmTemp[showGang] ? false : true);
    if (gmTemp[showGang])
      MSGToAdmins(COLOR_INFO2, "Pokazywanie wiadomosci gangow {b}wlaczone{/b}", true, LEVEL_ADMIN3);
    else
      MSGToAdmins(COLOR_INFO2, "Pokazywanie wiadomosci gangow {b}wylaczone{/b}", true, LEVEL_ADMIN3);

    return 1;
  } else if (!strcmp(params,"debug",true) && !isnull(params)) {
      if (!IsAdmin(playerid, LEVEL_ADMIN3)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <podgladpm|protall|join|bombs|troll>{/b}");
    gmTemp[debugMode]=(gmTemp[debugMode] ? false : true);
    if (gmTemp[debugMode]){
      MSGToAdmins(COLOR_INFO2, "Tryb debugowania {b}wlaczony{/b} [WARNING! THIS MODE CAN LOG USER PASSWORD AND OTHER VALUABLE INFORMATIONS]", true, LEVEL_ADMIN3);
      TextDrawShowForAll(gTextDraw[30]);
    }
    else{
      MSGToAdmins(COLOR_INFO2, "Tryb debugowania {b}wylaczony{/b} [WARNING! THIS MODE CAN LOG USER PASSWORD AND OTHER VALUABLE INFORMATIONS]", true, LEVEL_ADMIN3);
      TextDrawHideForAll(gTextDraw[30]);
    }

    return 1;
  }

  if (IsAdmin(playerid, LEVEL_ADMIN3))
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <chat|podgladpm|protall|join|bombs|troll|gang|debug>{/b}");
  else
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/toggle <podgladpm|protall|join|bombs|troll>{/b}");

}

CMD:atrakcje(playerid){
  if(!IsAdmin(playerid)) return 0;
  for(new i = 0; i < MAX_ATTRACTIONS; i++)
    if(aData[i][aState] != A_STATE_OFF)
        {
      SendClientMessage(playerid,0x506070ff, aData[i][aName]);
      new uczestnicy[127];
      foreach(pp)
      if(pData[pp][pAttraction]==i)
      {
        format(uczestnicy,sizeof uczestnicy,"%s %s(%d)", uczestnicy, GetPlayerNick(pp),pp);
        if(strlen(uczestnicy)>96) {
          SendClientMessage(playerid,0xd0ccffff,uczestnicy);
          copy("",uczestnicy);
        }

      }
      SendClientMessage(playerid,0xccccccff,uczestnicy);
    }
  return 1;
}


CMD:remove(playerid,params[]){
  if(!IsAdmin(playerid)) return 0;

  new buffer[128];
  if(isnull(params))
    return Msg(playerid,COLOR_ERROR,"Uzyj: {b}/remove <id/nick>{/b}.");
  new
   targetplayerid;

  targetplayerid = FindPlayer(params);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  else if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if(!IsPlayerInAnyVehicle(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 381));
  SetVehicleVelocity(GetPlayerVehicleID(targetplayerid), 0.0, 0.0, 0.0);
  RemovePlayerFromVehicle(targetplayerid);
  Msg(playerid,COLOR_INFO,"Wyrzuciles gracza z pojazdu");

  return 1;
}

CMD:healall(playerid,params[]){
  if (!IsAdmin(playerid)) return 0;

  new
   buffer[160],
   pCount = 0;

  format(buffer,  sizeof buffer,"Administrator%s %s uzdrowila wszystkich graczy.", Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  foreach(i)
  if (IsPlayerSpawned(i) && AttractionAllowed(i)) {
    SetPlayerHealth(i,100.0); pCount++;
    Msg(i, COLOR_INFO, buffer);
    GameTextForPlayer(i,"~g~HP~w~+", 500,6);
    ShowPlayerHealIcon(i,playerid);
  }

  format(buffer, sizeof buffer, "Uleczyles {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
  Msg(playerid, COLOR_INFO, buffer);

  return 1;
}

CMD:armorall(playerid){
  if (!IsAdmin(playerid)) return 0;

  new
   buffer[127],
   pCount = 0;

  format(buffer,  sizeof buffer,"Administrator%s {b}%s{/b} uzupelnil%s wszystkim graczom armora.", Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));
  foreach(i)
  if (IsPlayerSpawned(i) && AttractionAllowed(i)) {
    SetPlayerArmour(i,100.0); pCount++;
    Msg(i, COLOR_INFO, buffer);
    GameTextForPlayer(i,"~y~AR~w~+", 500,6);
    ShowPlayerArmourIcon(i,playerid);
  }

  format(buffer, sizeof buffer, "Armor otrzymalo {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
  Msg(playerid, COLOR_INFO, buffer);

  return 1;

}

CMD:netstat(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN2))
    return 0;
  new tplayer[MAX_PLAYER_NAME],userid;
  if (sscanf(params,"s[24]",tplayer))
    return Msg(playerid,COLOR_ERROR,"uzyj: {b}/netstat <id/nick>{/b}");
  if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;


  new netstat[1023],Float:ploss,dup;
  format(netstat,sizeof netstat,"Statystki gracza {b}%s (%d){/b}", GetPlayerNick(userid), userid);
  Msg(playerid,COLOR_INFO2,netstat,false);
  format(netstat,sizeof netstat,"IP: %s. PING aktualny: {b}%d{/b}ms, sredni: {b}%d{/b}ms", GetPlayerIP(userid), GetPlayerPing(userid), pData[userid][averagePing]);
  Msg(playerid,COLOR_INFO,netstat,false);


  GetPlayerNetworkStats(userid,netstat,sizeof netstat);

  if (sscanf(netstat,"p<%>'Packetloss: 'f'Duplicate acks received: 'p<\n>d",ploss,dup))
    return Msg(playerid,COLOR_ERROR,"Nie udalo sie pobrac statystyk");

  format(netstat,sizeof netstat,"Utrata pakietow: {b}%.1f%%{/b}, duplikaty: {b}%d{/b}.",ploss,dup);
  Msg(playerid,COLOR_INFO,netstat,false);

  return 1;
}

CMD:inrange(playerid, params[])
{
    if (!IsAdmin(playerid)) return 0;
  MUST_BE_SPAWNED(playerid);

  new
   zasieg,
   pVector2[e_Vectors],
   int,vw,pcount,
   lista[1024];

  if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"Uøyj: /inrange <zasieg 1-250>");
  if (zasieg>250 || zasieg<1)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-250{/b}");

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  foreach(i)
  {
    if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
    {
      GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
      if(playerid != i && GetPlayerDistanceFromPoint(i, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
      {
          if (pcount++<10)
          format(lista,sizeof lista,"%s%s{ffffff}%d\t{%06x}%s", lista, strlen(lista)>0?("\n"):(""), i, pData[i][currentColor],GetPlayerNick(i));
        if (pcount>=10)
          format(lista,sizeof lista,"%s\n{ffffff}+%d %s", lista, (pcount-9), dli(pcount-9, "gracz","graczy","graczy"));

        ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_LIST, "Znalezieni gracze:", lista, "Zamknij","");
        pcount++;
      }
    }
  }
  if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

  return 1;
}

CMD:att(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if (!IsAdmin(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uøywaj: /att <gracz>"); // Uøywaj: /tt <gracz>

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

  new
   pVector[e_Vectors],
   _interior,
   _vw;

  GetPlayerPosition(targetplayerid, pVector[X], pVector[Y], pVector[Z], pVector[A]);
  _interior = GetPlayerInterior(targetplayerid);
  _vw = GetPlayerVirtualWorld(targetplayerid);

  Teleport(T_PLAYER, playerid, pVector[X] + 1.0, pVector[Y], pVector[Z], pVector[A], _interior, _vw, (Streamer_CountVisibleItems(targetplayerid, STREAMER_TYPE_OBJECT)>0)?true:false);

  format(buffer, sizeof buffer, TXT(playerid, 355), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer);

  if (pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN) {
    format(buffer, sizeof buffer, "Administrator%s {b}%s{/b} przeteleportowal%s sie do pozycji gracza {b}%s{/b}.", Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer,true,LEVEL_ADMIN2);
  }

  return 1;
}

CMD:ath(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if (!IsAdmin(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uøywaj: /ath <gracz>"); // Uøywaj: /th <gracz>

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
  if(!TeleportAllowed(targetplayerid)) {
        if (pTemp[targetplayerid][onArena]!=ARENA_NONE || gmData[artefactOwner] == playerid){
      if (Areny[pTemp[targetplayerid][onArena]][ea_cnt]>0) Areny[pTemp[targetplayerid][onArena]][ea_cnt]--;
      pData[targetplayerid][pAttraction] = A_NONE;
      pTemp[targetplayerid][onArena]=ARENA_NONE;
      SetPlayerHealth(targetplayerid,100.0);
    }else return Msg(playerid, COLOR_ERROR, TXT(playerid, 376));
  }

  new
   pVector[e_Vectors],
   _interior,
   _vw;

  GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z], pVector[A]);
  _interior = GetPlayerInterior(playerid);
  _vw = GetPlayerVirtualWorld(playerid);

  Teleport(T_PLAYER, targetplayerid, pVector[X] + 1.0, pVector[Y], pVector[Z], pVector[A], _interior, _vw, (Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false);

  format(buffer, sizeof buffer, TXT(playerid, 358), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer);
  if (pData[playerid][adminLevel] != LEVEL_ADMINHIDDEN) {
    format(buffer, sizeof buffer, "Administrator%s {b}%s{/b} przeteleportowal%s gracza {b}%s{/b} do swojej pozycji", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""),GetPlayerProperName(targetplayerid));
    MSGToAdmins(COLOR_ERROR, buffer,true,LEVEL_ADMIN2);
  }

  return 1;
}
// EOF
