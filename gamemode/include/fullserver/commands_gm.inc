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

forward Usun3dText(t);
public Usun3dText(t){
  if (IsValidDynamic3DTextLabel(Text3D:t))
    DestroyDynamic3DTextLabel(Text3D:t);
}

CMD:evsp(playerid)
{
  if (!IsGM(playerid)) return 0;
  if (!TeleportAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));

  evtp[tpWorld]=GetPlayerVirtualWorld(playerid);
  evtp[tpInterior]=GetPlayerInterior(playerid);
  evtp[tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;

  GetPlayerPos(playerid,evtp[tpX],evtp[tpY],evtp[tpZ]);
  Msg(playerid,COLOR_INFO,"Teleport na event zapisany, dostepny dla graczy pod komenda {b}/evtp{/b}.");
  return 1;
}

CMD:evdel(playerid)
{
  if (!IsGM(playerid)) return 0;
  if (!TeleportAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));

  evtp[tpX]=-1;
  evtp[tpY]=-1;
  evtp[tpZ]=-1;
  Msg(playerid,COLOR_INFO,"Teleport na event usuniety!");
  return 1;
}

CMD:m(playerid, params[])
{
  if(!IsAdmin(playerid, LEVEL_GM)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 242)); // U¿ywaj: /m <tekst>

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  OutputModeratorChat(playerid, params);

  return 1;
}

CMD:nocheats(playerid,params[]){
  if(!IsGM(playerid)) return 0;

  new tplayer[24],userid;

  if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"Uzyj: /nocheats <id/nick>");
  ShowPlayerDialog(userid,DIALOG_PLAYER_INFO,DIALOG_STYLE_MSGBOX,"Ostrzezenie/Warning","Na tym serwerze nie mozna uzywac czitow zadnego rodzaju.\r\nDostosuj sie, albo zostaniesz zbanowany!\r\n\r\n---\r\n\r\nCheats are not allowed on this server\r\nDon't use them or get banned!\r\n\r\n---\r\n\r\nObman zdes' ne dopuskaet sya.\r\nNe obmanut' ili poluchit' ban.","OK","");
  Msg(playerid,COLOR_INFO,"Wyslano informacje do gracza");

  return 1;
}

CMD:givegp(playerid,params[]){
    if(!IsGM(playerid)) return 0;
    new userid,amount,reason[128],maxa=2, reason2[128], bool:mline=false;
    if (sscanf(params,"dds[128]",userid,amount,reason))
    return Msg(playerid,COLOR_INFO,"/givegp {b}<id> <ilosc> <powod>{/b}");
  if(IsAdmin(playerid,LEVEL_ADMIN2)) maxa=5;
  if((amount>maxa || amount<=0 && !IsAdmin(playerid,LEVEL_ADMIN3)))
    return Msg(playerid,COLOR_ERROR,"Nieprawidlowa wartosc! (min. {b}1{/b}, max. dla GM {b}2{/b}, dla ADM: {b}5{/b})");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");

  if (!pData[userid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Ten gracz nie jest zarejestrowany!");
  else
    GivePlayerGamePoints(userid,amount,reason);

    format(reason2, sizeof reason2, "{b}%s{/b} dal%s {b}%d{/b} GamePoints graczowi {b}%s{/b}:",
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""), amount, GetPlayerNick(userid));
  if (strlen(reason2)+strlen(reason)<120) {
    format(reason2, sizeof reason2, "%s {b}%s{/b}", reason2, reason);
  } else {
    format(reason, sizeof reason, "{b}%s{/b}", reason);
    mline=true;
  }

  foreach(i) {
    Msg(i, COLOR_INFO, reason2);
    if (mline)
      Msg(i, COLOR_INFO, reason);
  }

  return 1;
}

CMD:zaevent(playerid,params[]){
    if(!IsGM(playerid)) return 0;
    new userid, reason2[128];
    if (sscanf(params,"d",userid))
    return Msg(playerid,COLOR_INFO,"/zaevent {b}<id>{/b}");
  if (!IsPlayerConnected(userid))
    return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");

  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Ten gracz nie jest zarejestrowany!");
  else{
    new gp=random(3)+1;
    GivePlayerGamePoints(userid,gp,"/event");
    new rsp=random(8)+1;
    GivePlayerScore(userid,rsp,"event");
    new cashh=random(25000)+10000;
    GivePlayerMoney(userid,cashh);

      format(reason2, sizeof reason2, "{b}%s{/b} otrzymal%s ${b}%d{/b} kasy, {b}%d{/b} respektu i {b}%d{/b} GP za event!",
    GetPlayerNick(userid), Kobieta(userid)?("a"):(""), cashh,rsp,gp);

    foreach(i) {
      Msg(i, COLOR_INFO, reason2);
    }
  }
  return 1;
}

CMD:eventvw(playerid)
{
    if(!IsGM(playerid) && NIEMAPERZY) return 0;
    SetPlayerVirtualWorld(playerid,69);
    Msg(playerid,COLOR_GM,"Zostales przeniesiony do swiata eventow");
    return 1;
}

CMD:jetpack(playerid,params[]){
    if((pData[playerid][adminLevel]<LEVEL_GM) && NIEMAPERZY) return 0;

  new tplayer[24],userid;

    if (pData[playerid][adminLevel]==LEVEL_GM)
    userid=playerid;
  else if (!sscanf(params,"s[24]",tplayer)) {
    if (!FindUIDByNameOrID(tplayer,userid,playerid)) return 1;
  } else
    return Msg(playerid,COLOR_ERROR,"Uzyj: /jetpack [id/nick]");

  SetPlayerVelocity(userid,0,0,1);

  SetPlayerSpecialAction(userid, 2);
  if(gmData[artefactOwner] == userid) DropArtefact(userid);
  if(userid!=playerid) {
    new buf[128];
    format(buf,sizeof buf,"{b}%s{/b} dal jetpacka graczowi {b}%s (%d){/b}",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid),GetPlayerProperName(userid),userid);
    MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN1);
  }
  return 1;
}

CMD:wiez(playerid, params[])
{
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 211)); // U¿ywaj: /jail <gracz> <czas w minutach> <powód>

  new
   szPlayerName[24],
   jTime,
   targetplayerid,
   szReason[128],
   buffer[128];

  sscanf(params, "s[24]is[128]", szPlayerName, jTime, szReason);

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
  if(pData[targetplayerid][jail] > 0) return Msg(playerid, COLOR_ERROR, TXT(playerid, 221)); // Ten gracz jest aktualnie w wiêzieniu.

  if(jTime<1) return Msg(playerid,COLOR_ERROR,"Minimalnie 1 minuta");
  switch(pData[playerid][adminLevel]){
    case LEVEL_GM:
      if (jTime>4 && gmTemp[pAdminCount]>0)
        return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 4 minuty.");
      else if (jTime>6)
        return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 6 minut.");
    case LEVEL_ADMIN3,LEVEL_ADMINHIDDEN: {
      }
    case LEVEL_ADMIN2:
        if (jTime>10)
        return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 10 minut.");
    default:  //admin1
      if (jTime>6)
        return Msg(playerid,COLOR_ERROR,"Mozesz wsadzic na maksymalnie 6 minut.");

  }
  if(jTime < 1 || jTime > 20) return Msg(playerid, COLOR_ERROR, TXT(playerid, 212)); // Wartoœæ czasu musi siê mieœciæ w przedziale 1 - 20.

  if(!strlen(szReason)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 211)); // U¿ywaj: /jail <gracz> <czas w minutach> <powód>

  JailPlayer(targetplayerid, jTime);

  format(buffer, sizeof buffer, TXT(playerid, 213), GetPlayerNick(targetplayerid), jTime, GetPeriodName(playerid, 'm', jTime), szReason);
  foreach(i)
    Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ zamkniêty w wiêzeniu na xxx minut z powodu: xxx.

  format(buffer, sizeof buffer, TXT(playerid, 214), jTime, GetPeriodName(playerid, 'm', jTime), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ zamkniêty w wiêzieniu na xxx minut przez admina "xxx".
  format(buffer, sizeof buffer, TXT(playerid, 50), szReason);
  Msg(targetplayerid, COLOR_INFO2, buffer); // Powód: xxx

  return 1;
}

CMD:unwiez(playerid, params[])
{

  new
   targetplayerid,
   buffer[128];

  if(!IsGM(playerid)) return 0;
  if(isnull(params)) {
    new sec=GetTickCount()/1000;
    new remainTime;
    foreach(i)
      if (pData[i][jail]>0) {
        remainTime = ((pData[i][jail] - sec)/60) + 1;
        format(buffer, sizeof buffer, "%3d {b}%s{/b} - %02dm", i, GetPlayerNick(i), remainTime);
        Msg(playerid,COLOR_INFO, buffer, false);
      }
    Msg(playerid,COLOR_ERROR,"/unwiez <id/nick>");
    return 1;
  }

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

  if(pData[targetplayerid][jail] == 0) return Msg(playerid, COLOR_ERROR, TXT(playerid, 218)); // Podany gracz nie jest w wiêzieniu.

  UnjailPlayer(targetplayerid);

  format(buffer, sizeof buffer, TXT(playerid, 216), GetPlayerNick(targetplayerid));
  foreach(i)
    if (pData[i][pAttraction]==A_NONE && i!=targetplayerid)
      Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ usuniêty z wiêzienia.


  format(buffer, sizeof buffer, TXT(playerid, 217), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ usuniêty z wiêzienia przez admina "xxx".

  return 1;
}

CMD:mute(playerid, params[])
{
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "U¿ywaj: /mute <gracz> <czas> <m/h> <powód>"); //

  new
   szPlayerName[24],
   mTime,
   period,
   targetplayerid,
   szReason[128],
   escaped_reason[128],
   buffer[256];

  sscanf(params, "s[24]ics[128]", szPlayerName, mTime, period, szReason);

  if(period != 'm' && period != 'h')
  {
    Msg(playerid, COLOR_ERROR, TXT(playerid, 29)); // Nieprawid³owy ostatni parametr komendy.
    return Msg(playerid, COLOR_ERROR, "M - minuty, H - godziny");
  }


  switch(pData[playerid][adminLevel]){
    case LEVEL_GM:
    {
        if((mTime < 1 || mTime > 59) && period == 'm') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
        if((mTime < 1 || mTime > 1) && period == 'h') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    }
    case LEVEL_ADMIN3,LEVEL_ADMINHIDDEN:
    {
        if((mTime < 1 || mTime > 59) && period == 'm') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
        if((mTime < 1 || mTime > 23) && period == 'h') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    }
    case LEVEL_ADMIN2:
    {
        if((mTime < 1 || mTime > 59) && period == 'm') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
        if((mTime < 1 || mTime > 23) && period == 'h') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    }
    default:  //admin1
    {
        if((mTime < 1 || mTime > 59) && period == 'm') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
        if((mTime < 1 || mTime > 23) && period == 'h') return Msg(playerid, COLOR_ERROR, "Zbyt duzy przedzial czasu. Sproboj uzyc innego znacznika lub zmniejszyc przedzial czasu");
    }

  }

  if(!strlen(szReason)) return Msg(playerid, COLOR_ERROR, "U¿ywaj: /mute <gracz> <czas> <przedzial czasu> <powód>"); // U¿ywaj: /mute <gracz> <czas w minutach> <powód>

  targetplayerid = FindPlayer(szPlayerName);

  if(targetplayerid == ERROR_NOT_FOUND)
  {
    if(strlen(szPlayerName) < 3 || strlen(szPlayerName) > 24 || !IsNickCorrect(szPlayerName))
    {
      return Msg(playerid, COLOR_ERROR, TXT(playerid, 128)); // Podana nazwa konta jest niepoprawna.
    }

    if(!AccountExists(szPlayerName))
    {
      format(buffer, sizeof buffer, TXT(playerid, 129), params);
      return Msg(playerid, COLOR_ERROR, buffer); // Nie znaleziono konta gracza o nazwie "xxx".
    }

    mysql_real_escape_string(szReason,escaped_reason);

    format(buffer, sizeof buffer, "INSERT INTO fs_mutes (player_muted, player_given, date_created, date_end, reason) VALUES (%i, %i, NOW(), NOW() + INTERVAL %i %s, '%s')",
        GetAccountID(szPlayerName),
        pData[playerid][accountID],
        mTime,
        GetMySQLNameOfPeriod(period),
        escaped_reason
    );

    mysql_query(buffer);

    format(buffer, sizeof buffer, TXT(playerid, 245), szPlayerName, mTime, GetPeriodName(playerid, period, mTime), szReason);
    foreach(i)
      Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ wyciszony na xxx minut z powodu: xxx.
    return 1;
  }

  if(targetplayerid == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if(isPlayerMuted(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 244)); // Ten gracz jest ju¿ wyciszony.

  if(period == 'm') pTemp[targetplayerid][mute] = (GetTickCount() / 1000) + (mTime * 60);
    if(period == 'h') pTemp[targetplayerid][mute] = (GetTickCount() / 1000) + (mTime * 3600);

  format(buffer, sizeof buffer, TXT(playerid, 245), GetPlayerNick(targetplayerid), mTime, GetPeriodName(playerid, period, mTime), szReason);
  foreach(i)
    Msg(i, COLOR_INFO, buffer); // Gracz "xxx" zosta³ wyciszony na xxx minut z powodu: xxx.

  format(buffer, sizeof buffer, TXT(playerid, 246), mTime, GetPeriodName(playerid, period, mTime), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ wyciszony na xxx minut przez admina "xxx".
  format(buffer, sizeof buffer, TXT(playerid, 50), szReason);
  Msg(targetplayerid, COLOR_INFO2, buffer); // Powód: xxx

  GivePlayerScore(targetplayerid,-2,"mute");

  if(pData[playerid][accountID]>0){

    mysql_real_escape_string(szReason,escaped_reason);

    format(buffer, sizeof buffer, "INSERT INTO fs_mutes (player_muted, player_given, date_created, date_end, reason) VALUES (%i, %i, NOW(), NOW() + INTERVAL %i %s, '%s')",
      pData[targetplayerid][accountID],
      pData[playerid][accountID],
      mTime,
      GetMySQLNameOfPeriod(period),
      escaped_reason
    );
    mysql_query(buffer);
  }else{
    pTemp[playerid][mExpireTime]=mTime;
    format(pTemp[playerid][mReason], 100, "%s", szReason);
    format(pTemp[playerid][mPeriod], 10, "%s", GetPeriodName(playerid, period, mTime));
  }
  return 1;
}

CMD:noskill(playerid, params[])
{
  if(!IsGM(playerid)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze z blokad¹ skilla:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][noSkill]) {
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
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/noskill [id/nick]{/b}. Ponowne uzycie komendy wylaczy blokadê skilla.",false);
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
  if(pTemp[targetplayerid][noSkill]) {
    new bool:kobieta=Kobieta(playerid),
      bool:kobietat=Kobieta(targetplayerid);
    pTemp[targetplayerid][noSkill]=false;
    format(buffer,sizeof buffer,"{b}%s{/b} ODBLOKOWA£%s Ci skill.",
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));
    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, "~g~odblokowano skill", 4000, 1);
    format(buffer,sizeof buffer,"{b}%s{/b} ODBLOKOWA£%s skill graczowi {b}%s (%d){/b}.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobietat?("a"):(""),
      GetPlayerProperName(targetplayerid),targetplayerid);
    foreach(i)
      if (i!=targetplayerid)
        Msg(i,COLOR_ADMIN, buffer, false);
  } else {
    new bool:kobieta=Kobieta(playerid),
      bool:kobietat=Kobieta(targetplayerid);

    pTemp[targetplayerid][noSkill]=true;

    format(buffer,sizeof buffer,"{b}%s{/b} ZABLOKOWA£%s Ci skill.",
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));

    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, "~g~blokada skilla", 4000, 1);
    format(buffer,sizeof buffer,"{b}%s{/b} ZABLOKOWA£%s skill graczowi {b}%s (%d){/b}.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobietat?("a"):(""),
      GetPlayerProperName(targetplayerid),targetplayerid);
    foreach(i)
      if (i!=targetplayerid)
        Msg(i,COLOR_ADMIN, buffer, false);
  }
  return 1;
}

CMD:prot50(playerid, params[])
{
  if(!IsGM(playerid)) return 0;
  new
   pVector2[e_Vectors],
   buffer[160],
   int, vw, cnt;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  foreach(targetplayerid)
  if (IsPlayerSpawned(targetplayerid) && GetPlayerInterior(targetplayerid)==int && GetPlayerVirtualWorld(targetplayerid)==vw)
  {
    GetPlayerPosition(targetplayerid, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != targetplayerid && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      if(pTemp[targetplayerid][protkill]) {
        new bool:kobieta=Kobieta(playerid);
        pTemp[targetplayerid][protkill]=false;
        format(buffer,sizeof buffer,"{b}%s{/b} odebral%s Ci immunitet.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));
        Msg(targetplayerid, COLOR_ADMIN, buffer, false);
        PlaySound(targetplayerid, 1137);
        GameTextForPlayer(targetplayerid, "~g~immunitet odebrany", 4000, 1);
      } else {
        new bool:kobieta=Kobieta(playerid);

        pTemp[targetplayerid][protkill]=true;

        format(buffer,sizeof buffer,"{b}%s{/b} dal%s Ci immunitet.",
          (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));

        Msg(targetplayerid, COLOR_ADMIN, buffer, false);
        PlaySound(targetplayerid, 1137);
        GameTextForPlayer(targetplayerid, "~g~otrzymano immunitet", 4000, 1);
      }
      cnt++;
    }
  }
  format(buffer,sizeof buffer,"Nadano/odebrano immunitet %d graczom",cnt);
  Msg(playerid,COLOR_ADMIN,buffer);

  return 1;
}

CMD:prot(playerid, params[])
{
  if(!IsGM(playerid)) return 0;
  new buffer[128];
  if(isnull(params)) {
    Msg(playerid,COLOR_INFO,"Gracze z protem:",false);
    new cnt;
    foreach(i)
      if (pTemp[i][protkill]) {
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
    return Msg(playerid, COLOR_ERROR, "Uzyj: {b}/prot [id/nick]{/b}. Ponowne uzycie komendy wylaczy prota.",false);
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
  if(pTemp[targetplayerid][protkill]) {
    new bool:kobieta=Kobieta(playerid),
      bool:kobietat=Kobieta(targetplayerid);
    pTemp[targetplayerid][protkill]=false;
    format(buffer,sizeof buffer,"{b}%s{/b} odebral%s Ci immunitet.",
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));
    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, "~g~immunitet odebrany", 4000, 1);
    format(buffer,sizeof buffer,"{b}%s{/b} zostal%s pozbawion%s immunitetu przez {b}%s{/b}.", GetPlayerProperName(targetplayerid), kobietat?("a"):(""),
      kobietat?("a"):("y"), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    foreach(i)
      if (i!=targetplayerid)
        Msg(i,COLOR_ADMIN, buffer, false);
  } else {
    new bool:kobieta=Kobieta(playerid),
      bool:kobietat=Kobieta(targetplayerid);

    pTemp[targetplayerid][protkill]=true;

    format(buffer,sizeof buffer,"{b}%s{/b} dal%s Ci immunitet.",
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), kobieta?("a"):(""));

    Msg(targetplayerid, COLOR_ADMIN, buffer, false);
    PlaySound(targetplayerid, 1137);
    GameTextForPlayer(targetplayerid, "~g~otrzymano immunitet", 4000, 1);
    format(buffer,sizeof buffer,"{b}%s{/b} otrzymal%s immunitet od {b}%s{/b}.", GetPlayerProperName(targetplayerid), kobietat?("a"):(""),
      (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
    foreach(i)
      if (i!=targetplayerid)
        Msg(i,COLOR_ADMIN, buffer, false);
  }

  return 1;
}

CMD:getpos(playerid,params[]){
    if(!IsGM(playerid) && NIEMAPERZY) return 0;
    new Float:P[3],int,vw,Float:fa,str[128],userid,Float:MAz;
    if (sscanf(params,"d",userid))
  userid=playerid;
    else if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");
    GetPlayerPos(playerid,P[0],P[1],P[2]);
  new vid=GetPlayerVehicleID(playerid);
  if (vid>0)
    GetVehicleZAngle(vid,fa);
  else
    GetPlayerFacingAngle(playerid,fa);
    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    format(str,sizeof(str),"XYZ: {ff0000}%.2f %.2f %.2f {ffffff}%.2f",P[0],P[1],P[2],fa);
    Msg(playerid,0xffffffff,str,false);

  format(str,sizeof str ,"Interior: {ff0000}%d{ffffff}, vw: {ff0000}%d",int,vw);
  Msg(playerid,0xffffffff,str,false);

  if (int==0) {
    MapAndreas_FindZ_For2DCoord(P[0],P[1],MAz);
    format(str,sizeof(str),"Wysokosc mapy w podanym X,Y wg MapAndreas: {b}%.4f{/b}", MAz);
    Msg(playerid,COLOR_INFO2,str,false);
  }
  return 1;
}

CMD:count(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

  new
    czas,
    isFreeze,
    zasieg,
    buf[127];

    if(sscanf(params,"ddd", czas, zasieg, isFreeze)) return Msg(playerid,COLOR_ERROR,"U¿yj: /count <czas: 5-25> <zasieg: 1-100> <zamrozenie: 0-1>");

  if (zasieg>100 || zasieg<1)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-100{/b}");
    if (czas>25 || czas<1)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalny czas: {b}5-25{/b}");
  if (isFreeze>1 || isFreeze<0)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalne tryby: 0 - brak zamrozenia, 1 - zamrozenie.");

  Odliczanie(czas,zasieg,isFreeze,playerid);

    format(buf, sizeof buf, "{b}%s (%d){/b} uruchomil/a odliczanie (Zamrozenie: {b}%s{/b} | Czas: {b}%ds{/b} | Zasieg: {b}%dm{/b})", GetPlayerProperName(playerid),playerid,isFreeze?("TAK"):("NIE"),czas,zasieg);

    MSGToAdmins(COLOR_ERROR, buf, true, LEVEL_ADMIN2);

  return 1;
}

CMD:vehcolor(playerid,params[]){
  if(!IsGM(playerid) && NIEMAPERZY) return 0;
  if(isnull(params)) return Msg(playerid,COLOR_ERROR,"U¿yj: /vehcolor <color 1> <color 2>");

  new color1,color2,veh;
  sscanf(params, "dd", color1, color2);

  veh = GetPlayerVehicleID(playerid);
  ChangeVehicleColor(veh,color1,color2);

  Msg(playerid,COLOR_INFO,"Zmieniles kolor tego pojazdu");

  return 1;
}

CMD:tp(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 360)); // U¿ywaj: /tp <gracz 1> <gracz 2>

  new
   playerid1,
   playerid2,
   szPlayerName1[24],
   szPlayerName2[24],
   buffer[128];

  sscanf(params, "s[24]s[24]", szPlayerName1, szPlayerName2);
  playerid1 = FindPlayer(szPlayerName1);

  if(playerid1 == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), szPlayerName1);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if(playerid1 == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName1);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if(!TeleportAllowed(playerid1)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 377));

  if (strcmp(szPlayerName2,"lv",true)==0) {
    Teleport(T_PLAYER, playerid1, 2185, 1014, 10.82, 99, 0, 0); // lv
    Msg(playerid1, COLOR_INFO, "Zostales przeniesiony do /lv");
    Msg(playerid, COLOR_INFO, "Gracz zostal przeniesiony");
    return 1;
  }

  playerid2 = FindPlayer(szPlayerName2);

  if(playerid2 == ERROR_NOT_FOUND)
  {
    format(buffer, sizeof buffer, TXT(playerid, 27), szPlayerName2);
    return Msg(playerid, COLOR_ERROR, buffer);
  }
  if(playerid2 == ERROR_FOUND_MULTIPLE)
  {
    format(buffer, sizeof buffer, TXT(playerid, 28), szPlayerName2);
    return Msg(playerid, COLOR_ERROR, buffer);
  }

  if(!TeleportAllowed(playerid2)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 378));
  if(playerid1 == playerid2) return Msg(playerid, COLOR_ERROR, TXT(playerid, 385));

  new
   pVector[e_Vectors],
   _interior,
   _vw;

  GetPlayerPosition(playerid2, pVector[X], pVector[Y], pVector[Z], pVector[A]);
  _interior = GetPlayerInterior(playerid2);
  _vw = GetPlayerVirtualWorld(playerid2);

  Teleport(T_PLAYER, playerid1, pVector[X] + 1.0, pVector[Y], pVector[Z], pVector[A], _interior, _vw);

  format(buffer, sizeof buffer, TXT(playerid, 361), GetPlayerNick(playerid1), GetPlayerNick(playerid2));
  Msg(playerid, COLOR_INFO, buffer); // Teleportowa³eœ gracza xxx do pozycji gracza xxx.

  format(buffer, sizeof buffer, TXT(playerid, 362), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), GetPlayerNick(playerid2));
  Msg(playerid1, COLOR_INFO2, buffer); // Admin xxx teleportowa³ Ciê do pozycji gracza xxx.

  format(buffer, sizeof buffer, TXT(playerid, 363), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), GetPlayerNick(playerid1));
  Msg(playerid2, COLOR_INFO2, buffer); // Admin xxx teleportowa³ gracza xxx do Twojej pozycji.

  return 1;
}


CMD:disarm(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 373)); // U¿ywaj: /disarm <gracz>

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

  ResetPlayerWeapons(targetplayerid);

  format(buffer, sizeof buffer, TXT(playerid, 374), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer); // Rozbroi³eœ gracza xxx.

  if(targetplayerid == playerid) return 1;

  format(buffer, sizeof buffer, "Zostales/as rozbrojony przez {b}%s{/b}", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? ("admina") : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ rozbrojony przez admina xxx.

  return 1;
}


CMD:3dtext(playerid,params[]){
  if (!IsGM(playerid))
    return 0;

  for(new iCh=0; iCh < strlen(params); iCh++)
    if(params[iCh] == '~')  params[iCh] = '-';

  if(isnull(params)) return Msg(playerid, COLOR_ERROR,"Uzyj: /3dtext <czas:1-30> <tekst>");
  new tekst[128],czas;
  sscanf(params,"ds[128]", czas,tekst);
  if (czas>30 || czas<1)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalny czas: {b}1-30{/b}");
  new Float:P[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  new t=_:CreateDynamic3DTextLabel(tekst, 0xFFFFFFFF, P[0], P[1], P[2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 100.0);
  SetTimerEx("Usun3dText", 1000*60*czas, false, "d", t);
  Msg(playerid,COLOR_INFO,"Tekst dodany.");
  return 1;
}

CMD:ann100(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzywaj: {b}/ann100 <tekst>{/b}"); // U¿ywaj: /ann <tekst>
  if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 100.0)
    {
      GameTextForPlayer(i,params,6000,5);
      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Komunikat wyslany do {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");


  return 1;

}

CMD:da50(playerid) {
    if (!IsGM(playerid)) return 0;
  return _da(playerid,"50");
}

CMD:da(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
  MUST_BE_SPAWNED(playerid);

  new
   zasieg,
   pVector2[e_Vectors],
   int,vw,buf[128],pcount;

  sscanf(params,"D(50)", zasieg);
  if (zasieg>200)
    return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}0-200{/b}");

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  switch(pData[playerid][adminLevel]){
    case LEVEL_GM:
      format(buf, sizeof buf, "Moderator%s {b}%s{/b} rozbroil%s wszystkich w promieniu {b}%d %s{/b}.",
        Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""), zasieg, dli(zasieg,"metra","metrow","metrow"));
    case LEVEL_ADMINHIDDEN:
      if (random(2)==1)
        format(buf, sizeof buf, "Nagle zauwazasz ze nie masz zadnej broni.");
      else
        format(buf, sizeof buf, "Niespodziewanie Twoje bronie znikaja.");
    default:
      format(buf, sizeof buf, "Administator%s {b}%s{/b} rozbroil%s wszystkich w promieniu {b}%d %s{/b}.",
        Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""), zasieg, dli(zasieg,"metra","metrow","metrow"));

  }

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw && (pData[i][adminLevel]<LEVEL_GM || pData[i][adminLevel]==LEVEL_ADMINHIDDEN))
  {
    GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
    {
      ResetPlayerWeapons(i);
      Msg(i,COLOR_INFO,buf,false);
      pcount++;
    }
  }
  format(buf,sizeof buf,"Rozbrojono {b}%d{/b} %s", pcount, dli(pcount, "osob", "osoby", "osob"));
  Msg(playerid,COLOR_INFO,buf,false);

  return 1;
}

CMD:gmlogin(playerid){
  new buf[128];
  if (pData[playerid][adminLevel]>=LEVEL_GM)
    return SetPlayerDrunkLevel(playerid,5000);

  if (pData[playerid][allowedLevel]<LEVEL_GM) {
    TextDrawShowForPlayer(playerid, gTextDraw[TD_WARNING]);
    SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");
    SendClientMessage(playerid,0xff0000ff,"Uwaga! {ffffff}Wszystkie proby nieautoryzowanego dostepu");
    SendClientMessage(playerid,0xffffffff,"sa rejestrowane i beda scigane z pelna surowoscia.");
    SendClientMessage(playerid,0xffffffff,"Powtarzajace sie przypadki sa zglaszane bezposrednio");
    SendClientMessage(playerid,0xffffffff,"na policje. {ff0000}Zastanow sie trzy razy, zanim zaczniesz.");
    SendClientMessage(playerid,0xff0000ff,"-------------------------------------------------");

    PlaySound(playerid,1053);
    SetTimerEx("HidePlayerWarning",10000,false,"d",playerid);
    return 1;
  }

  if (pData[playerid][suspended]) {
    format(gstr,sizeof gstr,"Jestes obecnie zawieszon%s w randze. Mozesz odwolac sie od zawieszenia w strefie na forum.", Kobieta(playerid)?("a"):("y"));
    Msg(playerid,COLOR_ERROR,gstr);
    format(gstr,sizeof gstr,"Proba zalogowania sie na moderatora przez zawieszonego gracza %s (%d).", GetPlayerProperName(playerid), playerid);
    MSGToAdmins(COLOR_ERROR, gstr, true, LEVEL_GM);
    return 1;
  }

  if (pData[playerid][allowedLevel]==LEVEL_GM) {
    pData[playerid][adminLevel]=pData[playerid][allowedLevel];
    SetPlayerProperColor(playerid,true);

    new
     szPlayerName[24];

    GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

    foreach(i)
    {
      format(buf, sizeof buf, TXT(i, 241), szPlayerName);
      Msg(i, COLOR_INFO2, buf, false); // "xxx" zalogowa³ siê na admina/moderatora (poziom xxx).
      PlaySound(i, 1133);
      PlayerPlaySound(i,1133, 10,10,10);
    }
  }
  UpdatePlayerNickTD(playerid);
  return 1;
}

CMD:up(playerid,params[]){
    if (!IsGM(playerid) && NIEMAPERZY) return 0;
    new Float:dist,userid,Float:P[3];

  if (isnull(params))
    return Msg(playerid,COLOR_ERROR,"/up [id] <odleglosc>");

  if (!sscanf(params,"df",userid,dist)) {
    if (!IsPlayerConnected(userid))
      return Msg(playerid,COLOR_ERROR,"Gracz nie jest podlaczony");
    else if (!IsPlayerSpawned(userid))
      return Msg(playerid,COLOR_ERROR,"Gracz nie jest zespawnowany");

  }else if (!sscanf(params,"f",dist)) {
    if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
      userid=pData[playerid][spectating];
      else
          userid=playerid;
  }

    if (IsPlayerInAnyVehicle(userid)) {
    new vehid;
    vehid=GetPlayerVehicleID(userid);
    GetVehiclePos(vehid,P[0],P[1],P[2]);

    new Float:V[3];
    GetVehicleVelocity(vehid,V[0],V[1],V[2]);

    SetVehiclePos(vehid,P[0],P[1],P[2]+dist);
    SetVehicleVelocity(vehid,V[0],V[1],V[2]);
    } else {
    GetPlayerPos(userid,P[0],P[1],P[2]);
    SetPlayerPos(userid,P[0],P[1],P[2]+dist);
    }

    // todo powiadomienie
    return 1;

}


CMD:heal(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    new userid=INVALID_PLAYER_ID;
    if (sscanf(params,"d",userid))
    if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
      userid=pData[playerid][spectating];

  if (userid==INVALID_PLAYER_ID)
  return Msg(playerid,COLOR_ERROR,"/heal <id gracza>");

    if (!IsPlayerConnected(userid))
  return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
    SetPlayerHealth(userid,100.0);
  ShowPlayerHealIcon(userid,playerid);
  if (pData[playerid][adminLevel]==LEVEL_GM){
    format(gstr, sizeof gstr, "Zostal%s uleczon%s przez moderator%s {b}%s{/b}.", Kobieta(userid)?("as"):("es"), Kobieta(userid)?("a"):("y"), Kobieta(playerid)?("ke"):("a"), GetPlayerProperName(playerid));
    Msg(userid, COLOR_INFO, gstr);
  }
  else if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN){
    format(gstr, sizeof gstr, "Zostal%s uleczon%s przez administrator%s {b}%s{/b}.", Kobieta(userid)?("as"):("es"), Kobieta(userid)?("a"):("y"), Kobieta(playerid)?("ke"):("a"), GetPlayerProperName(playerid));
    Msg(userid, COLOR_INFO, gstr);
  }

    return 1;
}

CMD:armor(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    new userid=INVALID_PLAYER_ID;
    if (sscanf(params,"d",userid))
    if (pData[playerid][spectating]!=INVALID_PLAYER_ID)
      userid=pData[playerid][spectating];

  if (userid==INVALID_PLAYER_ID)
  return Msg(playerid,COLOR_ERROR,"/armor <id gracza>");

    if (!IsPlayerConnected(userid))
  return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
    SetPlayerArmour(userid,100.0);
  ShowPlayerArmourIcon(userid,playerid);
  if (pData[playerid][adminLevel]==LEVEL_GM){
    format(gstr, sizeof gstr, "Otrzymal%s armor od moderator%s {b}%s{/b}.", Kobieta(userid)?("as"):("es"), Kobieta(playerid)?("ki"):("a"), GetPlayerProperName(playerid));
    Msg(userid, COLOR_INFO, gstr);
  }
  else if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN){
    format(gstr, sizeof gstr, "Dostal%s armor od administrator%s {b}%s{/b}.", Kobieta(userid)?("as"):("es"), Kobieta(playerid)?("ki"):("a"), GetPlayerProperName(playerid));
    Msg(userid, COLOR_INFO, gstr);
  }

    return 1;
}

CMD:vehgod(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    new userid,vehicleid;
    if (sscanf(params,"d",userid)) {
  if (IsPlayerInAnyVehicle(playerid))
      userid=playerid;
  else if (pData[playerid][spectating]!=INVALID_PLAYER_ID && IsPlayerInAnyVehicle(pData[playerid][spectating]))
      userid=pData[playerid][spectating];
    } else if (!IsPlayerInAnyVehicle(userid))
      return Msg(playerid,0xffffffff,"Gracz nie jest w pojezdzie.");

    if (!IsPlayerConnected(userid))
  return Msg(playerid,0xffffffff,"Nie ma takiego gracza!");

    vehicleid=GetPlayerVehicleID(userid);
    SetVehicleHealth(vehicleid, 1000000);
    // todo powiadomienie
    return 1;
}

CMD:gun50(playerid,params[]){
  if (!IsGM(playerid)) return 0;
  if(!isnull(params)){
    new wepid,ammo;
      if (sscanf(params,"dd",wepid,ammo))
      return Msg(playerid,COLOR_ERROR,"Uzycie: /gun50 {b}<id broni> <amunicja>{/b}");
    if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
      return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
    if (wepid<=15 && ammo>1)
      ammo=1;
    if (ammo>10000 || ammo<0)
      ammo=10000;

    if (wepid<=15)
        ammo=1;

    new
      pVector2[e_Vectors],
      buffer[160],
      pCount = 0,
      int, vw;

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
    {
      GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
      if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
      {
        GivePlayerWeapon(i,wepid,ammo);
        pCount++;
      }
    }

    if (pCount) {
      format(buffer, sizeof buffer, "Rozdano bron {b}%d %s{/b}.", pCount, dli(pCount,"graczu","graczom","graczom"));
      Msg(playerid, COLOR_INFO, buffer);
    } else
      Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");
  }else{
      Msg(playerid,COLOR_INFO,"Mozesz uzyc takze: /gun50 {b}<id broni> <amunicja>{/b}");
    ShowWeaponSelectMenu(playerid, "Daj bron wszystkim w promieniu 50m", "GM_gun50CB",-1,-1);
  }
  return 1;

}

CMD:gun100(playerid,params[]){
  if (!IsGM(playerid)) return 0;
  if(!isnull(params)){
    new wepid,ammo;
      if (sscanf(params,"dd",wepid,ammo))
      return Msg(playerid,COLOR_ERROR,"Uzycie: /gun100 {b}<id broni> <amunicja>{/b}");
    if (wepid<0 || wepid>43 || wepid==19 || wepid==20 || wepid==21 || wepid==13 || wepid==36 || wepid==44 || wepid==45)
      return Msg(playerid,COLOR_ERROR,"Bledne ID broni");
    if (wepid<=15 && ammo>1)
      ammo=1;
    if (ammo>10000 || ammo<0)
      ammo=10000;

    if (wepid<=15)
        ammo=1;

    new
      pVector2[e_Vectors],
      buffer[160],
      pCount = 0,
      int, vw;

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
    {
      GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
      if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 100.0)
      {
        GivePlayerWeapon(i,wepid,ammo);
        pCount++;
      }
    }

    if (pCount) {
      format(buffer, sizeof buffer, "Rozdano bron {b}%d %s{/b}.", pCount, dli(pCount,"graczu","graczom","graczom"));
      Msg(playerid, COLOR_INFO, buffer);
    } else
      Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");
  }else{
      Msg(playerid,COLOR_INFO,"Mozesz uzyc takze: /gun100 {b}<id broni> <amunicja>{/b}");
    ShowWeaponSelectMenu(playerid, "Daj bron wszystkim w promieniu 50m", "GM_gun100CB",-1,-1);
  }
  return 1;

}

forward GM_gun50CB(playerid,wepid);
public GM_gun50CB(playerid,wepid){
  new ammo;
  for (new i=0; i<sizeof quickbuyWeapons;i++)
    if (quickbuyWeapons[i][ewd_id]==wepid) {  ammo=quickbuyWeapons[i][ewd_baseAmmo]; break; }
  if (ammo==0)
    return Msg(playerid, COLOR_ERROR,"Niewlasciwy wybor");

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      GivePlayerWeapon(i,wepid,ammo);
      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Rozdano bron {b}%d %s{/b}.", pCount, dli(pCount,"graczu","graczom","graczom"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

  return 1;

}

forward GM_gun100CB(playerid,wepid);
public GM_gun100CB(playerid,wepid){
  new ammo;
  for (new i=0; i<sizeof quickbuyWeapons;i++)
    if (quickbuyWeapons[i][ewd_id]==wepid) {  ammo=quickbuyWeapons[i][ewd_baseAmmo]; break; }
  if (ammo==0)
    return Msg(playerid, COLOR_ERROR,"Niewlasciwy wybor");

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 100.0)
    {
      GivePlayerWeapon(i,wepid,ammo);
      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Rozdano bron {b}%d %s{/b}.", pCount, dli(pCount,"graczu","graczom","graczom"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");

  return 1;

}

CMD:heal50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"{b}%s{/b} uleczyl%s Cie.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      SetPlayerHealth(i,100.0);
      Msg(i, COLOR_INFO, buffer, true);
      pCount++;
      ShowPlayerHealIcon(i,playerid);
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Uleczyles {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:acolor50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new color;
  if(sscanf(params,"D(-1)",color))
    return Msg(playerid,COLOR_ERROR,"Uzyj: {b}/acolor50 [id koloru]. Nie podawaj id koloru aby zresetowac do domyslnego dla gracza.");

  if ((color == 5 || color==11) && pData[playerid][adminLevel]==LEVEL_GM)
    return Msg(playerid, COLOR_ERROR, "Kolor bialy i kolor czarny moze byc nadany tylko przez administratora");

  new nkolory[]={
    0xAFAFAF, // 1 - szary
    0x24FF0a,
    0xAA3333,
    0xFFFF00, // 4
    0xFFFFFF, //  5
    0x0000BB,
    0xFF9900,
    0xFF66FF,// ping 8
    0x39484B,
    0x9955DE,
    0x000000, // czarny 11
    0x8F4747, // brown
    0x33ccff,
    0x33aa33,
    0xFF8080, // krem
    0x303030  // prawie czarny
    };
  color--;


  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"%s zmienil%s Ci kolor na taki jak ten.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      if (color>=0 && color<sizeof nkolory) {
        pData[i][currentColor]=nkolory[color];
        SetPlayerColor(i, pData[i][currentColor]*256+pTemp[i][playerColorAlpha]);

      } else {
        SetPlayerProperColor(i,true);
      }
      SendClientMessage(i, pData[i][currentColor]*256+0xff, buffer);

      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Zmieniono kolor {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:armor50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"{b}%s{/b} uzupelnil%s Ci armora.",(pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      SetPlayerArmour(i,100.0);
      Msg(i, COLOR_INFO, buffer, true);
      pCount++;
      ShowPlayerArmourIcon(i,playerid);

    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Dales/as armora {b}%d %s{/b}.", pCount, dli(pCount,"graczowi","graczom","graczom"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:unarmor50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"{b}%s{/b} zabral%s Ci armor.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      SetPlayerArmour(i,0.0);
      Msg(i, COLOR_INFO, buffer, true);
      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Odebrano armora {b}%d %s{/b}.", pCount, dli(pCount,"graczowi","graczom","graczom"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:freeze50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;
   
  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"%s {b}%s{/b} zamrozil%s Cie.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      Msg(i, COLOR_INFO, buffer, true);
      TogglePlayerControllable(i,0);
      pTemp[i][isFreezed]=true;
      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Zamroziles/as {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:unfreeze50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"Administrator%s {b}%s{/b} odmrozil%s Cie.", Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      Msg(i, COLOR_INFO, buffer, true);
      TogglePlayerControllable(i,1);
      pTemp[i][isFreezed]=false;
      pCount++;
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Odmroziles/as {b}%d %s{/b}.", pCount, dli(pCount,"gracza","graczy","graczy"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:napraw50(playerid,params[]){
  if (!IsGM(playerid)) return 0;

  new
   pVector2[e_Vectors],
   buffer[160],
   pCount = 0,
   int, vw;

  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);

  format(buffer,sizeof buffer,"{b}%s{/b} naprawil%s Ci pojazd.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));

  foreach(i)
  if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
  {
    GetPlayerPosition(i, pVector2[X], pVector2[Y], pVector2[Z]);
    if(GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= 50.0)
    {
      if(IsPlayerInAnyVehicle(i)) {
                new vid=GetPlayerVehicleID(i);
          new Float:vhp;
        GetVehicleHealth(vid,vhp);
        if (vhp<10000)  /* vehgod */ {
          RepairVehicle(vid);
          SetVehicleHealth(vid,VEHICLE_DEFAULT_HP);
          Msg(i, COLOR_INFO, buffer, true);
          pCount++;
        }
      }
    }
  }

  if (pCount) {
    format(buffer, sizeof buffer, "Naprawiles pojazd {b}%d %s{/b}.", pCount, dli(pCount,"graczowi","graczom","graczom"));
    Msg(playerid, COLOR_INFO, buffer);
  } else
    Msg(playerid, COLOR_ERROR, "Nikogo nie ma w poblizu");



  return 1;
}

CMD:delcar(playerid){
  if (!IsGM(playerid)) return 0;
  new vehicleid=GetPlayerVehicleID(playerid);
  if (vehicleid==0 || vehicleid==INVALID_VEHICLE_ID)
    vehicleid=GetNearestVehicleToPlayer(playerid,4);
  if (vehicleid==0 || vehicleid==INVALID_VEHICLE_ID) {
    Msg(playerid,COLOR_ERROR,"Wsiadz albo stan obok pojazdu do usuniecia");
    return 1;
  }

  RespawnVehicle(vehicleid);
  return 1;
}

CMD:tenwoz(playerid,params[]){
  if (!IsGM(playerid)) return 0;
  new target;
  if(sscanf(params,"d", target)) return Msg(playerid, COLOR_ERROR, "Uzywaj: /tenwoz <id gracza>");
  if (!IsPlayerInAnyVehicle(target)) return SendClientMessage(playerid,-1,"Podany gracz musi byæ w pojeŸdzie");

  new vid=GetPlayerVehicleID(target);
  new engine,lights,alarm,doors,bonnet,boot,objective;
  GetVehicleParamsEx(vid,engine,lights,alarm,doors,bonnet,boot,objective);
  SetVehicleParamsEx(vid,1,lights,alarm,doors,bonnet,boot,(objective>0?0:1));
  return 1;
}

CMD:me(playerid, params[])  //gracze nie potrafia z tego skorzystac :/
{
  if (!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 144)); // U¿ywaj: /me <tekst>

  if(FilterText(playerid, params) == 0) return 1;
  if(!IsAdmin(playerid)) CensorText(params);
  if(gmData[chatColors] && pData[playerid][adminLevel]>=LEVEL_ADMIN3) FilterTextWithColors(params);

  new
   buffer[160];

  format(buffer, sizeof buffer, "%s {%s}%s", GetPlayerProperName(playerid), IntToHex(gmData[color_chatME]), params);
  SendClientMessageToAll(gmData[color_chatME_HL] * 256, buffer);

  format(buffer, sizeof buffer, "[ME] %s %s", GetPlayerNick(playerid), params);
  OutputLog(LOG_OCHAT, buffer);

  return 1;
}

CMD:rspa(playerid,params[]){
    if (!IsGM(playerid)) return 0;
    RespawnVehiclesInRangeOf(playerid);
    return 1;
}

CMD:strefa(playerid,params[]){
  if (!IsGM(playerid)) return 0;
  if (!TeleportAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));
  new rozmiar;
  if (sscanf(params,"d",rozmiar))
    return Msg(playerid,COLOR_INFO2,"Uzyj: {b}/strefa <5-100>{/b} aby utworzyc strefe bez DM. {b}/strefa 0{/b} aby ja usunac");

  if (rozmiar==0) {
    foreach(i)
      if (IsPlayerInDynamicArea(playerid,StrefyNoDM[ea_przenosna])) {
        pTemp[i][wStrefieNODM]=false;
        TextDrawHideForPlayer(i,gTextDraw[TD_AREANODM]);
    }
    DestroyDynamicArea(StrefyNoDM[ea_przenosna]);

    if (IsValidDynamicObject(Os[eO_przenosna_strefa]) && Streamer_GetIntData(STREAMER_TYPE_OBJECT, Os[eO_przenosna_strefa], E_STREAMER_MODEL_ID) == 1872) {
      DestroyDynamicObject(Os[eO_przenosna_strefa]);
      Os[eO_przenosna_strefa] = INVALID_OBJECT_ID;
    }

    StrefyNoDM[ea_przenosna]=CreateDynamicSphere(0,0,0,1,0,0,-1);
    format(gstr,sizeof gstr, "{b}%s{/b} skasowal%s przenosna strefe bez DM", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));
    MSGToAdmins(COLOR_GM, gstr, false, LEVEL_GM);
    return 1;
  }

  if (rozmiar>100) {
    rozmiar=100;
    Msg(playerid,COLOR_ERROR,"Rozmiar strefy zmniejszony do 100");
  } else if (rozmiar<5) {
    rozmiar=5;
    Msg(playerid,COLOR_ERROR,"Rozmiar strefy zwiekszony do 5");
  }
  if (IsValidDynamicObject(Os[eO_przenosna_strefa]) && Streamer_GetIntData(STREAMER_TYPE_OBJECT, Os[eO_przenosna_strefa], E_STREAMER_MODEL_ID) == 1872) {
      DestroyDynamicObject(Os[eO_przenosna_strefa]);
      Os[eO_przenosna_strefa] = INVALID_OBJECT_ID;
  }

  foreach(i)
    if (IsPlayerInDynamicArea(playerid,StrefyNoDM[ea_przenosna])) {
      pTemp[i][wStrefieNODM]=false;
      TextDrawHideForPlayer(i,gTextDraw[TD_AREANODM]);
  }
  DestroyDynamicArea(StrefyNoDM[ea_przenosna]);

  new Float:P[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  StrefyNoDM[ea_przenosna]=CreateDynamicSphere(P[0],P[1],P[2],rozmiar,GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid),-1);
  Os[eO_przenosna_strefa]=CreateDynamicObject(18728, P[0],P[1],P[2]+10.0, 0.0,0.0,0.0,  GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid),-1);
  format(gstr,sizeof gstr, "{b}%s{/b} zmienil%s polozenie przenosnej strefy bez DM", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));
  MSGToAdmins(COLOR_GM, gstr, false, LEVEL_GM);
  return 1;
}

CMD:strefas(playerid,params[]){
  if (!IsGM(playerid)) return 0;
  if (!TeleportAllowed(playerid))
    return Msg(playerid,COLOR_ERROR,TXT(playerid,452));
  new rozmiar;
  if (sscanf(params,"d",rozmiar))
    return Msg(playerid,COLOR_INFO2,"Uzyj: {b}/strefas <15-100>{/b} aby utworzyc strefe smierci. {b}/strefa 0{/b} aby ja usunac");

  if (rozmiar==0) 
  {
    foreach(i)
    {
      if (IsPlayerInDynamicArea(playerid,StrefyFullDM[ea_przenosna])) 
      {
        pTemp[i][wStrefieFULLDM]=false;
        TextDrawHideForPlayer(i,gTextDraw[TD_AREAFULLDM]);
      }
    }
    DestroyDynamicArea(StrefyFullDM[ea_przenosna]);

    if (IsValidDynamicObject(Os[eO_przenosna_strefaDM]) && Streamer_GetIntData(STREAMER_TYPE_OBJECT, Os[eO_przenosna_strefaDM], E_STREAMER_MODEL_ID) == 18728) {
      DestroyDynamicObject(Os[eO_przenosna_strefaDM]);
      Os[eO_przenosna_strefaDM] = INVALID_OBJECT_ID;
    }

    StrefyFullDM[ea_przenosna]=CreateDynamicSphere(0,0,0,1,0,0,-1);
    format(gstr,sizeof gstr, "{b}%s{/b} skasowal%s przenosna strefe smierci", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));
    MSGToAdmins(COLOR_GM, gstr, false, LEVEL_GM);
    return 1;
  }

  if (rozmiar>100) 
  {
    rozmiar=100;
    Msg(playerid,COLOR_ERROR,"Rozmiar strefy zmniejszony do 100");
  } 
  else if (rozmiar<15) 
  {
    rozmiar=15;
    Msg(playerid,COLOR_ERROR,"Rozmiar strefy zwiekszony do 15");
  }
  if (IsValidDynamicObject(Os[eO_przenosna_strefaDM]))
  {
    DestroyDynamicObject(Os[eO_przenosna_strefaDM]);
    Os[eO_przenosna_strefaDM] = INVALID_OBJECT_ID;
  }
  
  foreach(i)
  {
    if (IsPlayerInDynamicArea(playerid,StrefyFullDM[ea_przenosna]) && Streamer_GetIntData(STREAMER_TYPE_OBJECT, Os[eO_przenosna_strefaDM], E_STREAMER_MODEL_ID) == 1872) 
    {
      pTemp[i][wStrefieFULLDM]=false;
      TextDrawHideForPlayer(i,gTextDraw[TD_AREAFULLDM]);
    }
  }
  DestroyDynamicArea(StrefyFullDM[ea_przenosna]);

  new Float:P[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  StrefyFullDM[ea_przenosna]=CreateDynamicSphere(P[0],P[1],P[2],rozmiar,GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid),-1);
  Os[eO_przenosna_strefaDM]=CreateDynamicObject(18728, P[0],P[1],P[2]+10.0, 0.0,0.0,0.0,  GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid),-1);
  format(gstr,sizeof gstr, "{b}%s{/b} zmienil%s polozenie przenosnej strefy smierci", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));
  MSGToAdmins(COLOR_GM, gstr, false, LEVEL_GM);
  return 1;
}

CMD:zestaw(playerid){
  if (!IsGM(playerid)) return 0;
  ResetPlayerWeapons(playerid);
  GivePlayerWeapon(playerid,4,1); // noz        1
  GivePlayerWeapon(playerid,24,1000); // deagle   2
  GivePlayerWeapon(playerid,27,1000); // combat shot  3
  GivePlayerWeapon(playerid,29,1000); // mp5      4
  GivePlayerWeapon(playerid,31,1000); // m4       5
  GivePlayerWeapon(playerid,34,1000); // sniper   6
  if (pData[playerid][adminLevel]>=LEVEL_ADMIN1)
  {
    GivePlayerWeapon(playerid,38,1000); // minigun    7
    GivePlayerWeapon(playerid,16,1000); // granaty    8
    GivePlayerWeapon(playerid,42,1000); // gasnica    9
  }
  SetPlayerArmour(playerid,100.0);
  SetPlayerHealth(playerid,100);
  if (pData[playerid][adminLevel]>=LEVEL_ADMIN1) Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni admina");
  else Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni GMa");
  return 1;
}

CMD:zestaw2(playerid){
  if (!IsGM(playerid)) return 0;
  ResetPlayerWeapons(playerid);
  GivePlayerWeapon(playerid,4,1); // noz        1
  GivePlayerWeapon(playerid,24,1000); // deagle   2
  GivePlayerWeapon(playerid,27,1000); // combat shot  3
  GivePlayerWeapon(playerid,29,1000); // mp5      4
  GivePlayerWeapon(playerid,31,1000); // m4       5
  GivePlayerWeapon(playerid,34,1000); // sniper   6
  GivePlayerWeapon(playerid,16,1000); // granaty    8
  SetPlayerArmour(playerid,100.0);
  SetPlayerHealth(playerid,100);
  if (pData[playerid][adminLevel]>=LEVEL_ADMIN1) Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni admina");
  else Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni GMa");
  return 1;
}

CMD:zestaw3(playerid){
  if (!IsAdmin(playerid)) return 0;
  ResetPlayerWeapons(playerid);
  GivePlayerWeapon(playerid,23,1000); // silenced
  GivePlayerWeapon(playerid,26,1000); // sawnoff   t  3
  GivePlayerWeapon(playerid,28,1000); // mp5      4
  GivePlayerWeapon(playerid,30,1000); // ak       5
  GivePlayerWeapon(playerid,33,1000); // sniper   6
  GivePlayerWeapon(playerid,17,1000); // granaty    8
  if (pData[playerid][adminLevel]>=LEVEL_ADMIN1) GivePlayerWeapon(playerid,35,1000);  // rpg      7
  GivePlayerWeapon(playerid,41,1000); // spray
  GivePlayerWeapon(playerid,15,1);  // laska      10
  SetPlayerArmour(playerid,100.0);
  SetPlayerHealth(playerid,100);
  if (pData[playerid][adminLevel]>=LEVEL_ADMIN1) Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni admina");
  else Msg(playerid,COLOR_INFO,"Otrzymales zestaw broni GMa");
  return 1;
}

CMD:ann2(playerid, params[])
{
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "/ann2 {b}<tekst>{/b}"); // U¿ywaj: /ann <tekst>
  if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.

  new buffer[255];
  new tdtext[1023];
  format(buffer, sizeof buffer, "%s: %s", GetPlayerNick(playerid), params);
  OutputLog(LOG_ANN, buffer);
  if (pData[playerid][adminLevel]<LEVEL_ADMINHIDDEN)
    format(tdtext,sizeof tdtext, "%s ~r~--_~h~%s", params, GetPlayerProperName(playerid));
  else
    format(tdtext,sizeof tdtext, "%s", params);
  ShowAnnouncement2(tdtext);
  return 1;
}

CMD:kratka(playerid,params[])
{
  if(!IsGM(playerid)) return 0;
  new userid;
  if (sscanf(params,"d",userid))
  {
    Msg(playerid,COLOR_ERROR,"U¿yj: /kratka <id>");
    new lista[1024], cnt;
    for (new i=0; i<MAX_PLAYERS;i++)
    {
      if (IsValidDynamicObject(oKratki[i])) 
      {
        if (cnt++<10) format(lista,sizeof lista,"%s%s{ffffff}%d\t{%06x}%s", lista, strlen(lista)>0?("\n"):(""), i, pData[i][currentColor],GetPlayerNick(i));
        if (cnt>=10) format(lista,sizeof lista,"%s\n{ffffff}+%d %s", lista, (cnt-9), dli(cnt-9, "gracz","graczy","graczy"));
        ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_LIST, "Znalezione kratki graczy:", lista, "Zamknij","");
        cnt++;
      }
      if(cnt==0) return Msg(playerid,COLOR_INFO2,"Brak kratek.");
      return 1;
    }
  }
  if (!IsPlayerConnected(userid))
  return Msg(playerid,COLOR_ERROR,"Nie ma takiego gracza!");
  
  new Float:P[3],int,vw;
  GetPlayerPos(userid,P[0],P[1],P[2]);
  int=GetPlayerInterior(userid);
  vw=GetPlayerVirtualWorld(userid);

  if(IsValidDynamicObject(oKratki[userid])) DestroyDynamicObject(oKratki[userid]);
  oKratki[userid] = CreateDynamicObject(19075, P[0],  P[1],     P[2]+0.5, 0.0, 0.0, 0.0, vw,int,-1,300);
  SetPlayerPos(userid,P[0],P[1],P[2]);
  Streamer_Update(userid);
  PlayerPlaySound(userid, 1137, 0,0,0);
  return 1;
}

CMD:tpkratka(playerid,params[]){
  if(!IsGM(playerid)) return 0;
  new userid;
  if (sscanf(params,"d",userid)) return Msg(playerid,COLOR_ERROR,"U¿yj: /tpkratka <id gracza>");
  if (!IsValidDynamicObject(oKratki[userid])) return Msg(playerid,COLOR_ERROR,"Nie odnaleziono podanej kratki.");

  new Float:P[3],int,vw;
  GetDynamicObjectPos(oKratki[userid],P[0],P[1],P[2]);
  int=Streamer_GetIntData(STREAMER_TYPE_OBJECT, oKratki[userid], E_STREAMER_INTERIOR_ID);
  vw=Streamer_GetIntData(STREAMER_TYPE_OBJECT, oKratki[userid],E_STREAMER_WORLD_ID);
  if (vw==-1) vw=GetPlayerVirtualWorld(playerid);
  if (int==-1) int=GetPlayerInterior(playerid);

  Teleport(T_PLAYER, playerid, P[0], P[1], P[2], 0, int, vw, true);
  SetPlayerInterior(playerid,int);
  SetPlayerVirtualWorld(playerid,vw);
  Msg(playerid,COLOR_INFO2,"Przeniesiono do kratki");
  return 1;
}

CMD:unkratka(playerid,params[]){
  if(!IsGM(playerid)) return 0;
  new userid;
  if (sscanf(params,"d",userid)) return Msg(playerid,COLOR_ERROR,"U¿yj: /unkratka <id gracza>");
  if (IsValidDynamicObject(oKratki[userid])) DestroyDynamicObject(oKratki[userid]);
  oKratki[userid]=-1;
  Streamer_Update(userid);
  return 1;
}

CMD:unkratkaall(playerid){
  if(!IsGM(playerid)) return 0;
  new cnt=0;

  for (new i=0; i<MAX_PLAYERS;i++)
  {
    if (IsValidDynamicObject(oKratki[i])) 
    {
      DestroyDynamicObject(oKratki[i]);
      oKratki[i]=-1;
      cnt++;
    }
  }
  if (cnt==0) Msg(playerid,COLOR_INFO2,"Brak kratek do usuniecia.");
  else 
  {
    format(gstr, sizeof gstr, "%s usun%s wszystkie (%d) kratki.", (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("ela"):("al"),cnt);
    MSGToAdmins(COLOR_INFO2, gstr, true, LEVEL_GM);
  }
  return 1;
}

CMD:tt(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 354)); // U¿ywaj: /tt <gracz>

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
    format(buffer, sizeof buffer, "%s%s {b}%s{/b} przeteleportowal%s sie do Twojej pozycji", pData[playerid][adminLevel]==LEVEL_GM?("Moderator"):("Administrator"), Kobieta(playerid)?("ka"):(""), GetPlayerProperName(playerid), Kobieta(playerid)?("a"):(""));
    Msg(targetplayerid, COLOR_INFO2, buffer);
  }

  return 1;
}

CMD:th(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 357)); // U¿ywaj: /th <gracz>

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
  if(!TeleportAllowed(targetplayerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 376));

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
    format(buffer, sizeof buffer, TXT(playerid, 359),  GetPlayerNick(playerid));
    Msg(targetplayerid, COLOR_INFO2, buffer);
  }

  return 1;
}

CMD:freeze(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 367)); // U¿ywaj: /freeze <gracz>

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

  TogglePlayerControllable(targetplayerid, false);
  pTemp[targetplayerid][isFreezed]=true;

  format(buffer, sizeof buffer, TXT(playerid, 368), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer); // Zamrozi³eœ gracza xxx.

  if(targetplayerid == playerid) return 1;

  format(buffer, sizeof buffer, TXT(playerid, 369), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ zamro¿ony przez admina xxx.

  return 1;
}

CMD:unfreeze(playerid, params[])
{
  MUST_BE_SPAWNED(playerid);
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 370)); // U¿ywaj: /unfreeze <gracz>

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
  pTemp[targetplayerid][isFreezed]=false;

  TogglePlayerControllable(targetplayerid, true);

  format(buffer, sizeof buffer, TXT(playerid, 371), GetPlayerNick(targetplayerid));
  Msg(playerid, COLOR_INFO, buffer); // Odmrozi³eœ gracza xxx.

  if(targetplayerid == playerid) return 1;

  format(buffer, sizeof buffer, TXT(playerid, 372), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid));
  Msg(targetplayerid, COLOR_INFO2, buffer); // Zosta³eœ odmro¿ony przez admina xxx.

  return 1;
}

CMD:glosowanie(playerid,params[]){
  if(!IsGM(playerid)) return 0;
  if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Uzycie: {b}/glosowanie <pytanie/STOP>{/b}"); // U¿ywaj: /ann <tekst>
  if (strcmp(params,"stop",false)==0) { // przerywamy glosowaine!
    if (gmTemp[voteTimeleft]==0)
      return Msg(playerid,COLOR_ERROR,"Obecnie nie ma zadnego glosowania!");

    gmTemp[voteTimeleft]=0;
    TextDrawHideForAll(gTextDraw[TD_VOTING]);
    foreach (i)
      Msg(i,COLOR_INFO,TXT(playerid,460));
    return 1;
  }
  if(!CheckTildes(params)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 205)); // Iloœæ tyld "~" musi byæ parzysta.
  if (gmTemp[voteTimeleft]!=0)
    return Msg(playerid,COLOR_ERROR, "Obecnie trwa juz jakies glosowanie!");

  new str[1024];

  gmTemp[voteCount][0]=0;
  gmTemp[voteCount][1]=0;
  gmTemp[voteAuthor]=playerid;
  gmTemp[voteTimeleft]=15;

  foreach(i)
    pTemp[i][hasVoted]=false;

  copy(params,gmTemp[voteQuestion]);

  format(str,sizeof str,"%s %s~n~~y~%2d       ~g~%d   ~w~/TAK   ~r~%d   ~w~/NIE",
          gmTemp[voteQuestion], (strlen(gmTemp[voteQuestion])<35) ? ("~n~ ") : (" "),
          gmTemp[voteTimeleft],gmTemp[voteCount][0],gmTemp[voteCount][1]);

  TextDrawSetString(gTextDraw[TD_VOTING],str);
  foreach(i)
    if (pData[i][hudSetting][HUD_VOTING])
      TextDrawShowForPlayer(i,gTextDraw[TD_VOTING]);  // dorobic sprawdzanie

  if (pData[playerid][adminLevel]==LEVEL_ADMINHIDDEN) return 1;
  format(gstr, sizeof gstr, "Autor glosowania: {b}%s{/b}", GetPlayerProperName(playerid));
  MSGToAdmins(COLOR_INFO2, gstr, false, LEVEL_GM);
  return 1;
}

CMD:wiezienie(playerid,params[]){
  if(!IsGM(playerid)) return 0;
  MoveToJail(playerid);
  return 1;
}

CMD:invisible(playerid){
  if (!IsGM(playerid)) return 0;

  pTemp[playerid][playerColorAlpha]=0;
  SetPlayerProperColor(playerid);

  return Msg(playerid,COLOR_INFO,"Jestes niewidzialny na radarze!");
}

CMD:acolor(playerid,params[]){
  if(!IsGM(playerid)) return 0;

  new playerSz[25],color,buffer[128];
  if(sscanf(params,"s[24]D(-1)",playerSz,color))
    return Msg(playerid,COLOR_ERROR,"Uzyj: {b}/acolor <id/nick> [id koloru]. Nie podawaj id koloru aby zresetowac do domyslnego dla gracza.");

  new
   targetplayerid;

  targetplayerid = FindPlayer(playerSz);

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

  if ((color == 5 || color==11) && pData[playerid][adminLevel]==LEVEL_GM)
    return Msg(playerid, COLOR_ERROR, "Kolor bialy i kolor czarny moze byc nadany tylko przez administratora");

  new nkolory[]={
    0xAFAFAF, // 1 - szary
    0x24FF0a,
    0xAA3333,
    0xFFFF00, // 4
    0xFFFFFF, //  5
    0x0000BB,
    0xFF9900,
    0xFF66FF,// ping 8
    0x39484B,
    0x9955DE,
    0x000000, // czarny 11
    0x8F4747, // brown
    0x33ccff,
    0x33aa33,
    0xFF8080, // krem
    0x303030  // prawie czarny
    };
  color--;
  if (color>=0 && color<sizeof nkolory) {
    pData[targetplayerid][currentColor]=nkolory[color];
    SetPlayerColor(targetplayerid, pData[targetplayerid][currentColor]*256+pTemp[targetplayerid][playerColorAlpha]);
    SendClientMessage(playerid, pData[targetplayerid][currentColor]*256+0xff,"Zmieniono kolor gracza na taki jak ten");
    format(gstr,sizeof gstr, "%s%s %s zmienil%s Ci kolor na taki jak ten.", pData[playerid][adminLevel]==LEVEL_GM ? ("Moderator"):("Administrator"), Kobieta(playerid)?("ka"):(""), (pData[playerid][adminLevel] == LEVEL_ADMINHIDDEN) ? TXT(playerid, 416) : GetPlayerNick(playerid), Kobieta(playerid)?("a"):(""));
    SendClientMessage(targetplayerid, pData[targetplayerid][currentColor]*256+0xff, gstr);
  } else {
    SetPlayerProperColor(targetplayerid,true);
    Msg(playerid,COLOR_INFO,"Kolor gracza zostal zresetowany.");
  }

  return 1;
}

CMD:gracze(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

    new
     zasieg,
     pVector2[e_Vectors],
     int,vw,pcount,
     lista[2048];

    if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"U¿yj: /gracze <zasieg 10-100>");
    if (zasieg>100 || zasieg<10)
        return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}10-100{/b}");

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    {
        if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
        {
            GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
            if(playerid != i && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
            {
                if (pcount++<20)
                    format(lista,sizeof lista,"%s%s{ffffff}%d\t{%06x}%s", lista, strlen(lista)>0?("\n"):(""), i, pData[i][currentColor],GetPlayerNick(i));
                if (pcount>=20)
                    format(lista,sizeof lista,"%s\n{ffffff}+%d %s", lista, (pcount-9), dli(pcount-9, "gracz","graczy","graczy"));

                ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_LIST, "Znalezieni gracze:", lista, "Zamknij","");
                pcount++;
            }
        }
    }
    if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

    return 1;
}

CMD:snaj(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

    new
     zasieg,
     pVector2[e_Vectors],
     int,vw,pcount,
     lista[1024];

    if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"U¿yj: /snaj <zasieg 1-750>");
    if (zasieg>750 || zasieg<1)
        return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-750{/b}");

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    {
        if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
        {
            GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
            if(playerid != i && GetPlayerWeapon(i)==34 && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
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

CMD:hyd(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

    new
     zasieg,
     pVector2[e_Vectors],
     int,vw,pcount,
     lista[1024],
     vehicle;

    if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"U¿yj: /hyd <zasieg 1-500>");
    if (zasieg>500 || zasieg<1)
        return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-500{/b}");

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    {
        if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
        {
            vehicle = GetPlayerVehicleID(i);
            if(vehicle > 0)
            {
                GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
                if(playerid != i && GetVehicleModel(vehicle)==520 && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
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
    }
    if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

    return 1;
}

CMD:hunt(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

    new
     zasieg,
     pVector2[e_Vectors],
     int,vw,pcount,
     lista[1024],
     vehicle;

    if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"U¿yj: /hunt <zasieg 1-500>");
    if (zasieg>500 || zasieg<1)
        return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-500{/b}");

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    {
        if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
        {
            vehicle = GetPlayerVehicleID(i);
            if(vehicle > 0)
            {
                GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
                if(playerid != i && GetVehicleModel(vehicle)==425 && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
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
    }
    if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

    return 1;
}

CMD:rust(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

    new
     zasieg,
     pVector2[e_Vectors],
     int,vw,pcount,
     lista[1024],
     vehicle;

    if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"U¿yj: /rust <zasieg 1-500>");
    if (zasieg>500 || zasieg<1)
        return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-500{/b}");

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    {
        if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
        {
            vehicle = GetPlayerVehicleID(i);
            if(vehicle > 0)
            {
                GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
                if(playerid != i && GetVehicleModel(vehicle)==476 && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
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
    }
    if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

    return 1;
}

CMD:baron(playerid, params[])
{
    if (!IsGM(playerid)) return 0;
    MUST_BE_SPAWNED(playerid);

    new
     zasieg,
     pVector2[e_Vectors],
     int,vw,pcount,
     lista[1024],
     vehicle;

    if(sscanf(params,"D(50)", zasieg)) return Msg(playerid,COLOR_ERROR,"U¿yj: /baron <zasieg 1-500>");
    if (zasieg>500 || zasieg<1)
        return Msg(playerid,COLOR_ERROR,"Dopuszczalny zasieg: {b}1-500{/b}");

    int=GetPlayerInterior(playerid);
    vw=GetPlayerVirtualWorld(playerid);

    foreach(i)
    {
        if (IsPlayerSpawned(i) && GetPlayerInterior(i)==int && GetPlayerVirtualWorld(i)==vw)
        {
            vehicle = GetPlayerVehicleID(i);
            if(vehicle > 0)
            {
                GetPlayerPos(i, pVector2[X], pVector2[Y], pVector2[Z]);
                if(playerid != i && GetVehicleModel(vehicle)==464 && GetPlayerDistanceFromPoint(playerid, pVector2[X], pVector2[Y], pVector2[Z]) <= zasieg)
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
    }
    if(pcount==0) return Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego gracza!");

    return 1;
}
// EOF
