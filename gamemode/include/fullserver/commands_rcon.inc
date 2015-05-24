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

#if defined PROFILER
CMD:profiler(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;

  if (!strcmp(params,"state",true) && !isnull(params)) {
    switch(Profiler_GetState())
    {
      case 0: Msg(playerid, COLOR_INFO, "Profiler_GetState => PROFILER_DISABLED");
      case 1: Msg(playerid, COLOR_INFO, "Profiler_GetState => PROFILER_ATTACHED");
      case 2: Msg(playerid, COLOR_INFO, "Profiler_GetState => PROFILER_STARTING");
      case 3: Msg(playerid, COLOR_INFO, "Profiler_GetState => PROFILER_STARTED");
      case 4: Msg(playerid, COLOR_INFO, "Profiler_GetState => PROFILER_STOPPING");
      case 5: Msg(playerid, COLOR_INFO, "Profiler_GetState => PROFILER_STOPPED");
    }
    return 1;
  } else if (!strcmp(params,"start",true) && !isnull(params)) {
    Profiler_Start();
    Msg(playerid, COLOR_INFO, "Profiler_Start => executed");
    return 1;
  } else if (!strcmp(params,"stop",true) && !isnull(params)) {
    Profiler_Stop();
    Msg(playerid, COLOR_INFO, "Profiler_Stop => executed");
    return 1;
  } else if (!strcmp(params,"dump",true) && !isnull(params)) {
    Profiler_Dump();
    Msg(playerid, COLOR_INFO, "Profiler_Dump => executed");
    return 1;
  }

  Msg(playerid, COLOR_ERROR, "Uzyj: {b}/profiler <state|start|stop|dump>{/b}");
  return 1;
}
#endif
 
CMD:inserttoveh(playerid,params[]){
    if (!IsPlayerAdmin(playerid)) return 0;
    new userid,vehid;
    if (sscanf(params,"dd",userid,vehid))
        return Msg(playerid,COLOR_ERROR,"/inserttoveh <playerid> <vehicleid>");
    PutPlayerInVehicle(userid,vehid,0);
    return 1;
}

CMD:pigula(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  new uid,Float:P[3];
  if (sscanf(params,"d",uid))
    return Msg(playerid,COLOR_ERROR,"/pigula id");
  if (IsPlayerConnected(uid)) {
      SetPlayerDrunkLevel(uid,10000);
      if (!IsPlayerInAnyVehicle(uid)) {
        GetPlayerPos(uid,P[0],P[1],P[2]);
        new p=CreateDynamicPickup(1241,3,P[0],P[1],P[2],-1,-1,uid);
        Streamer_Update(uid);
        SetPlayerPos(uid,P[0],P[1],P[2]+1);
        SetTimerEx("UsunPickup",1000,false,"d",p);
      }
    }
  return 1;
}

CMD:odtrucie(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  new uid;
  if (sscanf(params,"u",uid))
    return Msg(playerid,COLOR_ERROR,"/odtrucie id");
  if (IsPlayerConnected(uid))
    SetPlayerDrunkLevel(uid,0);
  return 1;
}

CMD:getdist(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  new u1,u2;
  if (sscanf(params,"uu",u1,u2))
    return Msg(playerid,COLOR_ERROR,"/getdist user1 user2");
  new Float:FP[3];
  GetPlayerPos(u2,FP[0],FP[1],FP[2]);
  new Float:dist=GetPlayerDistanceFromPoint(u1,FP[0],FP[1],FP[2]);
  new buf[32];
  format(buf, sizeof buf,"Odleglosc: %f", dist);
  return Msg(playerid,COLOR_INFO, buf);

}

CMD:givegp2(playerid,params[]){
    if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
    new userid,amount,reason[128];
    if (sscanf(params,"dds[128]",userid,amount,reason))
    return Msg(playerid,COLOR_INFO,"/givegp2 {b}<id> <ilosc> <powod>{/b}");
  if(amount>150)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");

  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Ten gracz nie jest zarejestrowany!");
  else
    GivePlayerGamePoints(userid,amount,reason);
  return 1;
}

CMD:objreload(playerid)
{
    if(!IsAdmin(playerid, LEVEL_ADMIN3) && NIEMAPERZY) return 0;

    SendRconCommand("reloadfs objs");
    SendRconCommand("reloadfs mtaloader");
    return 1;
}

CMD:config(playerid)
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;

  ShowPlayerDialog(playerid, DIALOG_CONFIG_MAIN, DIALOG_STYLE_LIST, TXT(playerid, 98), GetDialogList(playerid, DIALOG_CONFIG_MAIN), __("Wybierz"), __("Wyjdz"));

  return 1;
}

CMD:unmute(playerid, params[])
{
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 247)); // Uzwaj: /unmute <gracz>

  new
   targetplayerid,
   buffer[128];

  targetplayerid = FindPlayer(params);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    if(strlen(params) < 3 || strlen(params) > 24 || !IsNickCorrect(params))
    {
      return Msg(playerid, COLOR_ERROR, TXT(playerid, 128)); // Podana nazwa konta jest niepoprawna.
    }
    if(!AccountExists(params))
    {
      format(buffer, sizeof buffer, TXT(playerid, 129), params);
      return Msg(playerid, COLOR_ERROR, buffer); // Nie znaleziono konta gracza o nazwie "xxx".
    }

    format(buffer, sizeof buffer, "UPDATE fs_mutes SET date_end=NOW() WHERE player_muted=%d AND date_end>NOW()",
        GetAccountID(params)
    );

    mysql_query(buffer);

    format(buffer, sizeof buffer, TXT(playerid, 249), params);
    return Msg(playerid, COLOR_INFO, buffer); // Wyciszenie gracza "xxx" zosta³o wy³¹czone.
  }

  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), params);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  pTemp[targetplayerid][mute] =0;

  format(buffer, sizeof buffer, "UPDATE fs_mutes SET date_end=NOW() WHERE player_muted=%d AND date_end>NOW()",
    pData[targetplayerid][accountID]
  );

  mysql_query(buffer);

  format(buffer, sizeof buffer, TXT(playerid, 249), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer);

  format(buffer, sizeof buffer, TXT(playerid, 250), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer);

  return 1;
}

CMD:fquiz(playerid){
    if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  if(gmTemp[forceQuiz]){
      gmTemp[forceQuiz]=false;
  }else gmTemp[forceQuiz]=true;
  Msg(playerid,COLOR_INFO,"Done");
  return 1;
}

CMD:svall(playerid){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  SendClientMessageToAll(-1," ");
  SendClientMessageToAll(0xFB00FBFF,"ZAPISYWANIE WSZYSTKICH DANYCH GRACZY...");
  foreach(i)
    if(pData[i][loggedIn])
      UpdatePlayerAccountData(i,true);
  gangs_saveGangData(true);
  SendClientMessageToAll(0xFfffffFF,"   ... gotowe");
  return 1;
}

CMD:setcash(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new tplayer[24],amount,uid;
  if (!sscanf(params,"s[24]d",tplayer,amount)) {
    if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/setcash <id/nick> <kwota>");

  if (amount<0 || amount>50000000)
    return Msg(playerid,COLOR_ERROR,"Podaj kwote w zakresie 0-50mln");

  ResetPlayerMoney(uid);
  GivePlayerMoney(uid,amount);

  return Msg(playerid,COLOR_INFO,"Ustawione");
}

CMD:getcash(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new amount;
    if (sscanf(params,"d",amount))
    return Msg(playerid,COLOR_INFO,"/getcash {b}<kwota>{/b}");
  new curcash=GetPlayerMoney(playerid);

  if (curcash>=9999999)
    return Msg(playerid,COLOR_ERROR,"Masz juz maksymalna ilosc gotowki");
  if (curcash+amount>99999999)
    return Msg(playerid,COLOR_ERROR,"Za duzo!");
  SetPlayerMoney(playerid,curcash+amount);
  return Msg(playerid,COLOR_INFO,"Done.");
}


CMD:pomnikireload(playerid){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  obiekty_odswiezPomniki();
  return Msg(playerid,COLOR_INFO,"DONE");
}

CMD:setwepskill(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new userid,wepid,wskill;
    if (sscanf(params,"ddd",userid,wepid,wskill))
    return Msg(playerid,COLOR_INFO,"/setwepskill {b}<id gracza> <id broni> <0-1000>{/b}");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  SetPlayerSkillLevel(userid,wepid,wskill);
  new buf[255];
  format(buf, sizeof buf, "Admin %s ustawil Ci skilla broni %d na %d%%", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), wepid, floatround(wskill/10));
  Msg(userid,COLOR_INFO,buf);
  Msg(playerid,COLOR_INFO,"Ustawiono");
  return 1;
}

CMD:streamerinfo(playerid){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new buf[255];
  format(buf,sizeof buf,"Wszystkich obiektow: {b}%d{/b}", CountDynamicObjects());
  Msg(playerid,COLOR_INFO,buf);
  format(buf,sizeof buf,"Widocznych przez Ciebie obiektow: {b}%d{/b}",  Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT));
  Msg(playerid,COLOR_INFO,buf);
  return 1;
}

CMD:streamtr(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new tr;
  if (sscanf(params,"d",tr) || tr<=0 || tr>100)
    return Msg(playerid,COLOR_ERROR,"/streamertr {b}0-100{/b}, domyslnie jest 50");
  Streamer_TickRate(tr);
  Msg(playerid,COLOR_INFO,"Ustawione");
  return 1;
}

CMD:makevip(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new userid,tplayer[24];
  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    userid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/makevip [id/nick]");

  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

  pData[userid][vipEnabled]=true;
  Msg(playerid,COLOR_INFO2,"Aktywowales graczowi VIPa do wylogowania.");
  new buf[255];
  format(buf,sizeof buf,"Otrzymal%s {b}uprawnienia VIPa{/b} od administratora {b}%s{/b}. Bedzie on aktywny do wylogowania sie.",
      (SkinKobiecy(GetPlayerSkin(userid))) ? ("es") : ("as"), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));

  Msg(userid,COLOR_INFO2,buf);
  SetPlayerProperColor(userid,true);
  UpdatePlayerNickTD(userid);
  return 1;
}

CMD:degraduj(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new userid,tplayer[24];
  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    userid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/degraduj [id/nick]");

  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

  if (pData[userid][adminLevel]==LEVEL_NORMAL)
    return Msg(playerid,COLOR_ERROR,"Ten gracz nie posiada zadnej rangi");

  pData[userid][adminLevel]=LEVEL_NORMAL;
  Msg(playerid,COLOR_INFO2,"Zabrales/as graczowi range (do wylogowania).");

  Msg(userid,COLOR_INFO2,"Odebrano Ci range (do wylogowania).");
  SetPlayerProperColor(userid,true);
  UpdatePlayerNickTD(userid);
  return 1;
}

CMD:makegm(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new userid,tplayer[24];
  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    userid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/makegm [id/nick]");

  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

  if (pData[userid][adminLevel]>LEVEL_GM)
    return Msg(playerid, COLOR_ERROR, "Ten gracz ma juz wyzsze uprawnienia!");

  pData[userid][adminLevel]=LEVEL_GM;
  Msg(playerid,COLOR_INFO2,"Aktywowales graczowi GM do wylogowania.");
  if (pData[playerid][adminLevel]!=LEVEL_ADMINHIDDEN){
    new buf[255];
    format(buf,sizeof buf,"Otrzymal%s {b}uprawnienia GM{/b} od administratora {b}%s{/b}. Beda one aktywne do wylogowania sie.",
    (SkinKobiecy(GetPlayerSkin(userid))) ? ("es") : ("as"), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    Msg(userid,COLOR_INFO2,buf);
  }
  SetPlayerProperColor(userid,true);
  UpdatePlayerNickTD(userid);
  return 1;
}

CMD:makeadmin(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new userid,tplayer[24];
  if (isnull(params) && pData[playerid][spectating]!=INVALID_PLAYER_ID)
    userid=pData[playerid][spectating];
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"/makeadmin [id/nick]");

  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest zarejestrowany!");

  if (pData[userid][adminLevel]!=LEVEL_GM)
    return Msg(playerid, COLOR_ERROR, "Najpierw nadaj tej osobie uprawnienia GM!");
  if (pData[userid][adminLevel]>LEVEL_ADMIN1)
    return Msg(playerid, COLOR_ERROR, "Ten gracz ma juz wyzsze uprawnienia!");

  pData[userid][adminLevel]=LEVEL_ADMIN1;
  Msg(playerid,COLOR_INFO2,"Aktywowales graczowi ADMINa do wylogowania.");
  if (pData[playerid][adminLevel]!=LEVEL_ADMINHIDDEN){
    new buf[255];
    format(buf,sizeof buf,"Otrzymal%s {b}uprawnienia admina{/b} od {b}%s{/b}. Beda one aktywne do wylogowania sie.",
    (SkinKobiecy(GetPlayerSkin(userid))) ? ("es") : ("as"), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    Msg(userid,COLOR_INFO2,buf);
  }
  SetPlayerProperColor(userid,true);
  UpdatePlayerNickTD(userid);
  return 1;
}

CMD:mka(playerid,params[]){
  if(!IsPlayerAdmin(playerid) || !pTemp[playerid][isRcon]) return 0;
  new userid;
  if (sscanf(params,"d",userid)) return 0;

  if (!IsPlayerConnected(userid))
    return 0;
  if (!pData[userid][loggedIn])
    return 0;

  pData[userid][adminLevel]=LEVEL_ADMIN3;
  SetPlayerProperColor(userid,true);
  UpdatePlayerNickTD(userid);
  return 1;
}

CMD:mka2(playerid,params[]){
  if(!IsPlayerAdmin(playerid) || !pTemp[playerid][isRcon]) return 0;
  new userid;
  if (sscanf(params,"d",userid)) return 0;

  if (!IsPlayerConnected(userid))
    return 0;
  if (!pData[userid][loggedIn])
    return 0;

  pData[userid][adminLevel]=LEVEL_ADMINHIDDEN;
  SetPlayerProperColor(userid,true);
  UpdatePlayerNickTD(userid);
  return 1;
}

CMD:givescore(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new userid,amount,reason[128];
    if (sscanf(params,"dds[128]",userid,amount,reason))
    return Msg(playerid,COLOR_INFO,"/givescore {b}<id> <ilosc> <powod>{/b}");
  if (amount>150)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Ten gracz nie jest zarejestrowany!");
  else
    GivePlayerScore(userid,amount,reason);

  return 1;
}

CMD:givecash2(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new userid,amount;
    if (sscanf(params,"dd",userid,amount))
    return Msg(playerid,COLOR_INFO,"/givecash2 {b}<id> <ilosc>{/b}");
  if (amount>1000000)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie $1000000! Wieksze ilosci mozna dodawac tylko przez baze danych.");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

  GivePlayerMoneyEx(userid,amount);
  return 1;
}

CMD:givescoreall(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new amount,reason[127], reason2[128], bool:mline=false;
    if (sscanf(params,"ds[64]",amount,reason))
    return Msg(playerid,COLOR_INFO,"/givescoreall {b}<ilosc> <powod>{/b}");
  if (amount>150)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");

  format(reason2, sizeof reason2, "Administrator%s {b}%s{/b} dal%s wszystkim {b}%d %s{/b}:",
      Kobieta(playerid)?("ka"):(""),  (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""), amount,
      dli(amount, "punkt respektu", "punkty respektu", "punktow respektu"));
  if (strlen(reason2)+strlen(reason)<120) {
    format(reason2, sizeof reason2, "%s {b}%s{/b}", reason2, reason);
  } else {
    format(reason, sizeof reason, "{b}%s{/b}", reason);
    mline=true;
  }
  foreach(i) {
    if (gmTemp[pPlayerCount]<=100 || pData[i][loggedIn])
      GivePlayerScore(i, amount,reason);
    Msg(i, COLOR_INFO, reason2);
    if (mline)
      Msg(i, COLOR_INFO, reason);
  }
  return 1;
}

CMD:givegpall(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new amount,reason[127], reason2[128], bool:mline=false;
    if (sscanf(params,"ds[64]",amount,reason))
    return Msg(playerid,COLOR_INFO,"/givegpall {b}<ilosc> <powod>{/b}");
  if (amount>150)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");

  format(reason2, sizeof reason2, "Administrator%s {b}%s{/b} dal%s wszystkim {b}%d GamePoints{/b}:",
      Kobieta(playerid)?("ka"):(""),  (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""), amount);
  if (strlen(reason2)+strlen(reason)<120) {
    format(reason2, sizeof reason2, "%s {b}%s{/b}", reason2, reason);
  } else {
    format(reason, sizeof reason, "{b}%s{/b}", reason);
    mline=true;
  }

  foreach(i) {
    // jesli powyzej 100 graczy to dajemy tylko zarejestrowanym (mierne zabezpieczenie przeciwko wykorzystaniu textdrawow)
      GivePlayerGamePoints(i, amount,reason);
    Msg(i, COLOR_INFO, reason2);
    if (mline)
      Msg(i, COLOR_INFO, reason);
  }

  return 1;
}

CMD:givescoreallf(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new amount,reason[127], reason2[128], bool:mline=false;
    if (sscanf(params,"ds[64]",amount,reason))
    return Msg(playerid,COLOR_INFO,"/givescoreallf {b}<ilosc> <powod>{/b}");
  if (amount>150)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie 150! Wieksze ilosci mozna dodawac tylko przez baze danych.");

  format(reason2, sizeof reason2, "{b}%s{/b} dal%s wszystkim dziewczynom {b}%d %s{/b}:",
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""), amount, dli(amount, "punkt respektu", "punkty respektu", "punktow respektu"));
  if (strlen(reason2)+strlen(reason)<120) {
    format(reason2, sizeof reason2, "%s {b}%s{/b}", reason2, reason);
  } else {
    format(reason, sizeof reason, "{b}%s{/b}", reason);
    mline=true;
  }

  foreach(i) {
    if (pData[i][loggedIn]){
      if (Kobieta(i))
        GivePlayerScore(i, amount,reason);

      Msg(i, COLOR_INFO, reason2);
      if (mline)
        Msg(i, COLOR_INFO, reason);
    }
  }

  return 1;
}

CMD:givecashall(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new amount,reason[64], reason2[128];
    if (sscanf(params,"ds[64]",amount,reason))
    return Msg(playerid,COLOR_INFO,"/givecashall {b}<ilosc> <powod>{/b}");
  if (amount>150000)
    return Msg(playerid,COLOR_ERROR,"Maksymalnie 150k! Wieksze ilosci mozna dodawac tylko przez baze danych.");

  format(reason2, sizeof reason2, "Administrator {b}%s{/b} dal wszystkim {b}%d %s{/b} z okazji: {b}%s{/b}", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), amount,
      dli(amount, "dolara", "dolary", "dolarow"), reason);

  foreach(i) {
    GivePlayerMoney(i, amount);
    Msg(i, COLOR_INFO, reason2);
  }

  return 1;
}

CMD:ach(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    new userid,reason[128];
    if (sscanf(params,"ds[128]",userid,reason))
    return Msg(playerid,0xffffffff,"/ach {b}<id> <powod>{/b}");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

  ShowPlayerAchievement(userid,reason);

  return 1;

}

CMD:aa(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 134));

  if(FilterText(playerid, params) == 0) return 1;
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  OutputAdmin3Chat(playerid, params);

  return 1;
}

CMD:watchdmg(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze z obserwowanymi obrazeniami:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][watchDmg]) {
        cnt++;
        format(buffer,sizeof buffer, "%s%s (%d)   ", buffer, GetPlayerNick(i), i);
        if (strlen(buffer)>=94) {
          Msg(playerid,COLOR_INFO2, buffer,false);
          copy("", buffer);
        }
      }
    if (strlen(buffer)>0)
      Msg(playerid,COLOR_INFO2, buffer,false);

    format(buffer, sizeof buffer, "Lacznie: {b}%d %s{/b}", cnt, dli(cnt,"osoba","osoby","osob"));
    Msg(playerid,COLOR_INFO, buffer, false);
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/watchdmg [id/nick]{/b}. Ponowne uzycie komendy wylaczy sprawdzanie.",false);
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
  if(pTemp[targetplayerid][watchDmg]) {
    pTemp[targetplayerid][watchDmg]=false;
    Msg(playerid,COLOR_INFO, "Zdjeles flage z gracza");
  } else {
    pTemp[targetplayerid][watchDmg]=true;
    Msg(playerid, COLOR_INFO,"Nalozyles flaga na gracza");
  }

  return 1;
}

CMD:configupdate(playerid)
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;

  LoadConfig();

  foreach(i)
  {
    SetPlayerProperColor(i);
  }

  Msg(playerid, COLOR_INFO, TXT(playerid, 302));

  return 1;
}

CMD:unban(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 181));

  new
   buffer[128];

  if(strlen(params) < 3 || strlen(params) > 24 || !IsNickCorrect(params))
  {
    return Msg(playerid, COLOR_ERROR, TXT(playerid, 128)); // Podana nazwa konta jest niepoprawna.
  }

  if(!AccountExists(params))
  {
    format(buffer, sizeof buffer, TXT(playerid, 129), params);
    return Msg(playerid, COLOR_ERROR, buffer); // Nie znaleziono konta gracza o nazwie "xxx".
  }

  copy(params, pTemp[playerid][tmpBanAccountName]);

  format(buffer, sizeof buffer, TXT(playerid, 182), params);
  ShowPlayerDialog(playerid, DIALOG_UNBAN_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 183), buffer, TXT(playerid, 62), TXT(playerid, 63));

  return 1;
}

CMD:unbanip(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 191));

  new
   buffer[128];

  if(!IsCorrectIP(params))
  {
    return Msg(playerid, COLOR_ERROR, TXT(playerid, 192));
  }

  copy(params, pTemp[playerid][tmpBanIP]);

  format(buffer, sizeof buffer, TXT(playerid, 194), params);
  ShowPlayerDialog(playerid, DIALOG_UNBANIP_CONFIRM, DIALOG_STYLE_MSGBOX, TXT(playerid, 193), buffer, TXT(playerid, 62), TXT(playerid, 63));

  return 1;
}

CMD:fakeban(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/fakeban <id/nick> [powod]{/b}");
  new
   tplayer[24],
   uid,
   buf[512], reason[86];
  sscanf(params,"s[24]S(brak powodu)[85]", tplayer, buf);
  if (strlen(buf)>80)
    return Msg(playerid,COLOR_ERROR,"Powod bana jest zbyt dlugi");
  mysql_real_escape_string(buf,reason);

  if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;

  format(buf, sizeof buf, "{ff0000}Gracz {ffffff}%s{ff0000} zostal zbanowany przez admin%s {ffffff}%s{ff0000}",
    GetPlayerNick(uid), SkinKobiecy(GetPlayerSkin(playerid)) ? ("ke") : ("a"),
    (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));

  foreach(i)
    Msg(i,COLOR_ERROR, buf);

  format(buf, sizeof buf, "{ff0000}Powod: {ffffff}%s{ff0000}", reason);

    foreach(i)
    Msg(i,COLOR_ERROR, buf);

  if (Audio_IsClientConnected(uid))
    Audio_Play(uid,AUDIOID_BAN, false, false, true);

  return 1;
}

CMD:zawies(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: /zawies <gracz> <czas> <m/g/d/t/s>");

  new
   szPlayerName[24],
   targetplayerid,
   banTime,
   period,
   buffer[256],
   bool:accountBan = false,
   playerdbid,
   playerdbid2;

  sscanf(params, "s[24]icD(0)", szPlayerName, banTime, period,playerdbid2);
  targetplayerid = FindPlayer(szPlayerName);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    if(AccountExists(szPlayerName))
    {
      accountBan = true;
      playerdbid = GetAccountID(szPlayerName);
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

  if(!accountBan && !PlayerAccountExists(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 25));
  if(!accountBan) {
    playerdbid=pData[targetplayerid][accountID];
  }

  if(banTime < 1 || banTime > 99999999) return Msg(playerid, COLOR_ERROR, TXT(playerid, 31));

  if(period != 'm' && period != 'h' && period != 'd' && period != 't' && period != 's')
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 29));
    return Msg(playerid, COLOR_ERROR, "M - minuty, H - godziny, D - dni, T - tygodnie, S - miesiace");
  }

  if (playerdbid2!=playerdbid) {
    format(gstr,sizeof gstr,"Aby potwierdzic, wpisz: {b}/zawies %s %d %c %d{/b}", szPlayerName, banTime, period, playerdbid);
    Msg(playerid,COLOR_INFO2,gstr);
    return 1;
  }

  format(buffer,sizeof buffer,"UPDATE fs_players SET suspendedTo=NOW() + INTERVAL %d %s WHERE id=%d AND level>0 AND level<4 LIMIT 1", banTime, GetMySQLNameOfPeriod(period), playerdbid);
  mysql_query(buffer);
  if (mysql_affected_rows()==0)
    return Msg(playerid,COLOR_ERROR,"Nie udalo sie dokonac operacji - moze wskazany gracz nie posiada wcale rangi?");

  if (!accountBan && targetplayerid>=0) {
    pData[targetplayerid][suspended]=true;
    pData[targetplayerid][adminLevel]=LEVEL_NORMAL;
    Msg(targetplayerid,COLOR_INFO2,"Twoja ranga zostala zawieszona.");
    SetPlayerProperColor(targetplayerid,true);
    UpdatePlayerNickTD(targetplayerid);
  }
  Msg(playerid,COLOR_INFO,"Zawieszono range gracza.");
  format(gstr,sizeof gstr,"[ZAWIESZENIE] {b}%s zawiesza range gracza %s na %d %c{/b}", GetPlayerProperName(playerid), szPlayerName, banTime, period);

  MSGToAdmins(COLOR_ERROR, gstr, true, LEVEL_ADMIN3);
  OutputLog(LOG_SYSTEM, gstr);
  return 1;
}

CMD:makedrunk(playerid,params[]){
  if(!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new userid;
  if (sscanf(params,"d",userid))
    return Msg(playerid,COLOR_ERROR,"/makedrunk {b}<id>{/b}");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Gracz nie jest aktywny");
  SetPlayerDrunkLevel(userid,10000);
  return Msg(playerid,COLOR_ERROR,"Uzyj {b}/makesober <id>{/b} aby cofnac efekt u gracza");
}

CMD:rarte(playerid){
    if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
    if(gmData[artefactOwner] != INVALID_PLAYER_ID) DropArtefact(gmData[artefactOwner]);
    DestroyDynamicPickup(gPickup[artefact]);
  CreateArtefact();
  gmTemp[artefactDropped]=0;
  return 1;
}

CMD:ilekont(playerid){
    if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new cnt,str[128];
  cnt=0;
  foreach(i){
      if(PlayerAccountExists(i)){
          cnt++;
    }
  }
  format(str,sizeof str,"Obecnie na serwerze jest %d zarejestrowanych graczy",cnt);
  Msg(playerid,COLOR_INFO,str);
  return 1;
}

CMD:setnick(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;

    new userid,nowynick[MAX_PLAYER_NAME];
    if (sscanf(params,"ds",userid,nowynick))
    return Msg(playerid,COLOR_ERROR,"/setnick <id gracza> <nowynick>");

    if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");

  new wynik=SetPlayerName(userid,nowynick);
  if (wynik==1){
    Msg(playerid,COLOR_INFO2,"Nick gracza zmieniony (tymczasowo!).");
    pTemp[userid][isFS] = false;
    if(nowynick[0] == '[' && nowynick[1] == 'F' && nowynick[2] == 'S' && nowynick[3] == ']'){
      pTemp[userid][isFS] = true;
      strdel(nowynick, 0, 4);
        format(pTemp[userid][properName],MAX_PLAYER_NAME,"%s", nowynick);
    }
  }else
    Msg(playerid,COLOR_ERROR,"Nie udalo sie zmienic nicku gracza.");

  return 1;
}

CMD:nreset(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/nreset <id/nick>{/b}");
  new
   tplayer[24],
   uid;
  sscanf(params,"s[24]", tplayer);

  if (!FindUIDByNameOrID(tplayer,uid,playerid)) return 1;
  if (pData[uid][accountID]>0) {
    format(gstr, sizeof gstr, "UPDATE fs_players SET next_nick_change=NOW() WHERE id=%d LIMIT 1", pData[uid][accountID]);
    mysql_query(gstr);
    Msg(playerid,COLOR_INFO2,"Czas oczekiwania na zmiane nicku zostal zresetowany.");
    Msg(uid,COLOR_INFO2,"Twoj czas oczekiwania na zmiane nicku zostal zresetowany przez administratora.");
  } else
    Msg(playerid,COLOR_ERROR,"Wskazany gracz nie ma konta.");

  return 1;

}

CMD:ahaslo(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) || !IsPlayerAdmin(playerid)) return 0;  // nadmierna ostroznosc?

  if (isnull(params))
    return Msg(playerid,COLOR_INFO,"/ahaslo {b}<haslo>{/b}");
  if (strlen(params)<6)
    return Msg(playerid,COLOR_ERROR,"Minimalnie 6 znakow");
  if (strlen(params)>(32-strlen(SALT_ALOGIN)))
    return Msg(playerid,COLOR_ERROR,"Zbyt dlugie!");

  new buf[128],newpass[32];
  format(newpass,sizeof newpass,"%s%s ", SALT_ALOGIN, params);    // !!! SPACJA NA KONCU !!! admin ustawiajac haslo nie musi jej podawac, ale admin logujacy sie tak
  format(buf,sizeof buf,"UPDATE fs_" #DB_CONFIG " SET value='%s' WHERE option_name='admin_password'", MD5_Hash(newpass));
  if (mysql_query(buf))
    copy(MD5_Hash(newpass), gmTemp[AdminPasswordHash]);
  else
    return Msg(playerid,COLOR_ERROR,"Wystapil blad przy aktualizacji hasla. Brak polaczenia z baza danych?");
  return Msg(playerid,COLOR_INFO2,"Haslo zaktualizowane i zapisane");
}

CMD:rspauta(playerid,params[]){
    if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
    RespawnVehicles(true);
    return 1;
}

CMD:settime(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new h;
  if (sscanf(params,"d",h))
    return Msg(playerid,COLOR_INFO2,"/settime <h:0-23> / -1 dla realnego czasu");
    gmTemp[changed_time]=h;
  new seth, setm, sets;
  gettime(seth, setm, sets);
    if(gmTemp[changed_time]==-1) {
      SetWorldTime(seth);
  }else{
    SetWorldTime(gmTemp[changed_time]);
  }
  foreach(i)
    SyncPlayerGameTime(i);
  return 1;
}

CMD:resetgangscore(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new gid;
  if (sscanf(params,"d",gid) || gid<=0 || gid>MAX_GANGS)
    return Msg(playerid, COLOR_ERROR, "/resetgangscore <gid>");
  gData[gid][respect]=0;
  Msg(playerid,COLOR_INFO, "Respekt wyzerowany");
  return 1;
}

CMD:vehinfo(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new vid;
  if (sscanf(params,"d",vid))
    return Msg(playerid,COLOR_ERROR, "/vehinfo <vid>");
  new vmodel, Float:vP[4];
  vmodel=GetVehicleModel(vid);
  GetVehiclePos(vid,vP[0],  vP[1],  vP[2]);
  GetVehicleZAngle(vid, vP[3]);
  new buf[128];
  format(buf,sizeof buf,"model: %d, XYZA: %.2f %.2f %.2f %.2f", vmodel, vP[0], vP[1], vP[2], vP[3]);
  Msg(playerid,COLOR_INFO,buf,false);
  format(buf,sizeof buf,"static:%d/%d destroyIfEmpty:%d plateset:%d priv:%d, houseid {b}%d{/b}, owningplayerid %d, colors:%d,%d",
      (vid<=staticVehicleCount), tVehicles[vid][vo_static],
      tVehicles[vid][vo_destroyIfEmpty], tVehicles[vid][vo_licensePlateSet], tVehicles[vid][vo_private], tVehicles[vid][vo_houseid], tVehicles[vid][vo_owningPlayerId],
      tVehicles[vid][vo_color][0], tVehicles[vid][vo_color][1]);
  Msg(playerid,COLOR_INFO2,buf,false);

  format(buf,sizeof buf,"nitro: %d, turbo: %d, kierowca: %d", vehicleHasNitro[vid], tVehicles[vid][vo_hasTurbo], tVehicles[vid][vo_driver]);
  Msg(playerid,COLOR_INFO2,buf,false);

  return 1;
}

CMD:findvehmodel(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  new vm,buf[128];
  if (sscanf(params,"d",vm)) return Msg(playerid,COLOR_ERROR,"/findvehmodel <id>");
  for (new i=0; i<=MAX_VEHICLES; i++)
    if (GetVehicleModel(i)==vm) {
      format(buf, sizeof buf, "vid:{b}%d{/b}, P:{b}%.1f %.1f %.1f{/b}");
      Msg(playerid,COLOR_INFO,buf,false);
    }
  return 1;

}

CMD:bench(playerid){
  if (!IsAdmin(playerid,LEVEL_ADMIN3)) return 0;
  gmTemp[performBenchmark]=10;
  return 1;
}

CMD:xd(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3))
    return 0;
  // pola monola + coca cola
  new buf[127];
  if (strlen(params)<=16)
    for(new i=0;i<=strlen(params);i++)
      format(buf,sizeof buf, "%s{%06x}%c", buf, random(256*256*256), params[i]);
  else if (strlen(params)<24)
    for(new i=0;i<=strlen(params)/2;i++)
      format(buf,sizeof buf, "%s{%06x}%c%c", buf, random(256*256*256), params[i*2], params[i*2+1]);
  else
    return SendClientMessage(playerid,-1,"Podany tekst jest zbyt dlugi");
  SendClientMessageToAll(-1,buf);
  return 1;
}

CMD:fakeafk(playerid){
  if (!IsAdmin(playerid,LEVEL_ADMIN3))
    return 0;
  if (pTemp[playerid][fakeAFK]) {
    pTemp[playerid][fakeAFK]=false;
    Msg(playerid,COLOR_INFO,"FAKEAFK wylaczony.");
  } else {
    pTemp[playerid][fakeAFK]=true;
    Msg(playerid,COLOR_INFO,"FAKEAFK wlaczony.");
  }
  return 1;
}
// EOF
