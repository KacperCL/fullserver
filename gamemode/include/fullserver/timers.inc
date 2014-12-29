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

Timer:AttractionBoxUpdate[5000]()
{
      new
     szTitles[256],
     szStates[256],
     aPlayers[MAX_ATTRACTIONS],
     buffer[128];

    foreach(playerid){
      if(pData[playerid][aChowany])     aPlayers[A_CHOWANY]++;
      if(pData[playerid][aHAY])       aPlayers[A_HAY]++;
      if(pData[playerid][aDerby])     aPlayers[A_DERBY]++;
      if(pData[playerid][aRace])      aPlayers[A_RACE]++;
      if(pData[playerid][aDrift])     aPlayers[A_DRIFT]++;
      if(pData[playerid][aWG])      aPlayers[A_WG]++;
      if(pData[playerid][aStrzelnica])  aPlayers[A_STRZELNICA]++;
      if(pData[playerid][aCTF])       aPlayers[A_CTF]++;
      if(pData[playerid][aGG])       aPlayers[A_GG]++;
    }

    for(new i = 0; i < MAX_ATTRACTIONS; i++)
    {
      if(aData[i][aState] == A_STATE_OFF && aPlayers[i] >= aData[i][aStartPlayers])
      {
        aData[i][aState] = A_STATE_STARTING;
        aData[i][aStartTick] = GetTickCount() + (aData[i][aStartingTime] * 1000);
        format(buffer, sizeof buffer, __("{b}%s{/b} rozpocznie sie za {b}%i %s{/b}."), aData[i][aName], aData[i][aStartingTime], dli(aData[i][aStartingTime],__("sekunde"),__("sekundy"),__("sekund")));
        foreach(playerid)
        {
          Msg(playerid, COLOR_INFO3, buffer, gmTemp[pPlayerCount]>10 ? false : true);
        }
      }
      else if(aData[i][aState] == A_STATE_STARTING && aPlayers[i] < aData[i][aStartPlayers])
      {
        aData[i][aState] = A_STATE_OFF;

        foreach(playerid)
        {
          switch(i)
          {
            case A_CHOWANY:   Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}chowanego{/b}, zapisy ponowione."));
            case A_HAY:     Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}HAY{/b}, zapisy ponowione."));
            case A_DERBY:     Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}derby{/b}, zapisy ponowione."));
            case A_RACE:    Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}race{/b}, zapisy ponowione."));
            case A_DRIFT:     Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}Drifting{/b}, zapisy ponowione."));
            case A_WG:      Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}WG{/b}, zapisy ponowione."));
            case A_STRZELNICA:  Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}strzelnice{/b}, zapisy ponowione."));
            case A_CTF:     Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}CTF{/b}, zapisy ponowione."));
            case A_GG:     Msg(playerid, COLOR_INFO3, __("Brak dostatecznej liczby graczy na {b}GunGame{/b}, zapisy ponowione."));
          }
        }
      }
      else if(aData[i][aState] == A_STATE_STARTING && aData[i][aStartTick] <= GetTickCount())
      {
                aData[i][aState] = A_STATE_ON;

        switch(i)
        {
          case A_CHOWANY:   Chowany_Start();
          case A_HAY:     HAY_Start();
          case A_DERBY:     Derby_Start();
          case A_RACE:    Race_Start();
          case A_DRIFT:     Drifting_Start();
          case A_WG:      WG_Start();
          case A_STRZELNICA:  Strzelnica_Start();
          case A_CTF:     CTF_Start();
          case A_GG:     GG_Start();
        }

        if(aData[i][aState] == A_STATE_ON){ //sprawdzmy czy atrakcja aby nie zostala wylaczona w funkcji stratujacej (np jedyny gracz zostal wywalony za afk)
          if (i==A_CHOWANY || i==A_DRIFT)
            format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);
          else if (i==A_WG || i==A_HAY || i==A_CTF || i==A_RACE || i==A_GG)
            format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³o! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);
          else if (i==A_STRZELNICA)
              format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³a! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);
          else
            format(buffer, sizeof buffer, "{b}%s{/b} wystartowa³y! Zapisy na nastêpn¹ kolejkê rozpoczête.", aData[i][aName]);

                  foreach(playerid)
            Msg(playerid, COLOR_INFO3, buffer, gmTemp[pPlayerCount]>10 ? false : true);
        }
      }
    }

    for(new i = 0; i < MAX_ATTRACTIONS; i++)
    {
        switch(aData[i][aState])
        {
          case A_STATE_OFF:
          {
            format(buffer, sizeof buffer, "~g~~h~%s~n~", aData[i][aName]);
            strcat(szTitles, buffer);

            format(buffer, sizeof buffer, __("~g~~h~Zapisy (%i/%i)~n~"), aPlayers[i], aData[i][aStartPlayers]);
            strcat(szStates, buffer);
          }
          case A_STATE_STARTING:
          {
            format(buffer, sizeof buffer, "~y~%s~n~", aData[i][aName]);
            strcat(szTitles, buffer);

            format(buffer, sizeof buffer, __("~y~Startuje (%i)~n~"), aPlayers[i], aData[i][aStartPlayers]);
            strcat(szStates, buffer);
          }
          case A_STATE_ON:
          {
            format(buffer, sizeof buffer, "~r~~h~~h~%s~n~", aData[i][aName]);
            strcat(szTitles, buffer);

            strcat(szStates, __("~r~~h~~h~TRWA~n~"));
          }
        }
    }

    new
      _ac,_acPRO;

    foreach(playerid) {
      if (pTemp[playerid][onArena]==ARENA_PRODM)
        _acPRO++;
      else if (pData[playerid][pAttraction]==A_ARENA || pTemp[playerid][onArena]>ARENA_NONE)
        _ac++;
      }

    format(buffer, sizeof buffer, "~g~~h~~h~/Areny - %d~n~", _ac+_acPRO);
    strins(szTitles, buffer, 0);
    format(buffer, sizeof buffer, "~g~~h~~h~%s - %d~n~",Areny[gmTemp[popularArena]][ea_cmd],Areny[gmTemp[popularArena]][ea_cnt]);
    strins(szStates, buffer, 0);


    TextDrawSetString(gTextDraw[TD_ATTRACTION_TITLES], szTitles);
    TextDrawSetString(gTextDraw[TD_ATTRACTION_STATES], szStates);
}

Timer:MoneyPickups[12000](){
  if (++gmTemp[timer_MoneyPickups_step]>=4)
      gmTemp[timer_MoneyPickups_step]=2;

  new sec=GetTickCount()/1000;
  for(new i = random(gmTemp[timer_MoneyPickups_step]); i < MAX_MONEY_PICKUPS; i+=gmTemp[timer_MoneyPickups_step]) // co 2, co 3
  {
    if(gMoneyPickup[i][gmpTime] != 0 && (sec - 60) > gMoneyPickup[i][gmpTime])
    {
      if (IsValidDynamicPickup(gMoneyPickup[i][gmpPickupID])) {
        DestroyDynamicPickup(gMoneyPickup[i][gmpPickupID]);
        gMoneyPickup[i][gmpPickupID]=-1;
      }
      if (IsValidDynamic3DTextLabel(gMoneyPickup[i][gmp3DText])) {
        DestroyDynamic3DTextLabel(gMoneyPickup[i][gmp3DText]);
        gMoneyPickup[i][gmp3DText]=Text3D:-1;
      }
      gMoneyPickup[i][gmpTime] = 0;
    }
  }
}

// Zapisujemy do bazy dane gracza ORAZ sledzimy zmiany jego pozycji (co 6 minut dla kazdego)
Timer:SavePlayersData[4000](){
//  printf("T:SavePlayersData");
    // 400 graczy * 1 sekunda = 6.66 minuty     x:1
    // 200 graczy * 2 sekundy           x:2
    // 100 graczy * 4 sekundy           x:4
    // graczy * x  = 400
    // x= 400/graczy

  gmTemp[timer_SavePlayersData_lastidx]++;
  if (gmTemp[timer_SavePlayersData_lastidx]>gmTemp[highestPlayerID])
      gmTemp[timer_SavePlayersData_lastidx]=0;

  for(new i=gmTemp[timer_SavePlayersData_lastidx]; i<=gmTemp[highestPlayerID]; i++)
    if(IsPlayerConnected(i) && !IsPlayerNPC(i) && (GetTickCount()-pTemp[i][lastSessionSaveTick]>6*60*1000 ) ) { // zapisujemy nie czesciej niz raz na 6 minuty

      if (pData[i][loggedIn])
        UpdatePlayerAccountData(i);
      else
        Msg(i,COLOR_INFO2,TXT(i,466), false);

            if(!checkDoubleMode(i)){
            if(pTemp[i][doubleMode]) Msg(i,COLOR_INFO3,"[INFORMACJA] Czas aktywacji trybu furii skonczyl sie!");
        pTemp[i][doubleMode]=false;
      }

      pTemp[i][lastSessionSaveTick] = GetTickCount();

      if (pData[i][adminLevel]<LEVEL_ADMIN2) {
        new Float:P[3],curpos;
        GetPlayerPos(i,P[0],P[1],P[2]);
        curpos=floatround(P[0]) * floatround(P[1]) * floatround(P[2]/4);
        if (pTemp[i][lastPos]!=-1 && curpos==pTemp[i][lastPos] && curpos==pTemp[i][curPos]) {
          SendClientMessage(i,-1,__("[AFK] {FFECCF}Zostajesz wyrzucony/-a z powodu zbyt dlugiej nieaktywnosci!"));
          KickPlayer(i);
          continue;
        } else if (pTemp[i][lastPos]!=-1 && curpos==pTemp[i][curPos] && curpos!=pTemp[i][lastPos]) {
          SendClientMessage(i,-1,__("[AFK] {FFECCF}Rusz sie, albo zostaniesz wyrzucony/-a za nieaktywnosc!"));
        }
        pTemp[i][lastPos]=pTemp[i][curPos];
        pTemp[i][curPos]=curpos;
      }
      return;
  }
}

Timer:WeatherChange[360000](){
SendRconCommand("unbanip *.*.*.*");
    if((GetTickCount()/1000)-gmTemp[lastWeatherChangeTime]<350)
        return;
    gmTemp[currentWeather]=DATA_weathers[random(sizeof DATA_weathers)];
  new seth, setm, sets;
  gettime(seth, setm, sets);
    if(gmTemp[changed_time]==-1) {
      SetWorldTime(seth);
  }else{
    SetWorldTime(gmTemp[changed_time]);
  }
    foreach(i){
    SetPlayerWeather(i,gmTemp[currentWeather]);
    if(gmTemp[changed_time]>-1) {
      SetPlayerTime(i, gmTemp[changed_time], 0);
    }else{
        SetPlayerTime(i, seth, 0);
            if (GetPlayerInterior(i)>0){ // ???
              SetPlayerWeather(i,0);
              SetPlayerTime(i,12,0);
      }
    }
  }
    gmTemp[lastWeatherChangeTime]=GetTickCount()/1000;
}

Timer:fixServer[1800000](){
    SendRconCommand("unbanip *.*.*.*");
    SendRconCommand("reloadbans");
    SendRconCommand("reloadlog");
}

Timer:TimeSync[20000](){  // aktualizacja daty na pasku u dolu ekranu co 20/40 sekund + powiadomienie o minieciu polnocy
  if (gmTemp[timer_TimeSync_step]) {
    gmTemp[timer_TimeSync_step]=false;
    if (gmTemp[pPlayerCount]>100) return; // powyzej 100 graczy, odpalamy co 40 sekund
  } else
    gmTemp[timer_TimeSync_step]=true;

  new
    _time[3],
    _date[3],
    buffer[128];
  gettime(_time[0], _time[1], _time[2]);
  getdate(_date[0], _date[1], _date[2]);
  gmTemp[lastHour] = _time[0];

  format(buffer, sizeof buffer, "%02d:%02d", _time[0], _time[1]);
  TextDrawSetString(gTextDraw[TD_TIME], buffer);
  format(buffer, sizeof buffer, "%d %s (~y~%d~w~/~b~~h~%d~w~/~r~%d~w~) %02i.%02i.%i",
    gmTemp[pPlayerCount], dli(gmTemp[pPlayerCount], __("gracz"),__("graczy"),__("graczy")), gmTemp[pVipCount],gmTemp[pGMCount],gmTemp[pAdminCount],
     _date[2], _date[1], _date[0]);
  TextDrawSetString(gTextDraw[TD_DATETIME], buffer);

  if(_date[2] != gmTemp[lastDay] && GetTickCount()>60000) // 60 sekund po restarcie
  {
    foreach(playerid)
    {
      format(buffer, sizeof buffer, TXT(playerid, 164), _date[2], GetMonthName(playerid, _date[1]), _date[0]);
      Msg(playerid, COLOR_INFO2, buffer);
    }

  }
  gmTemp[lastDay] = _date[2];

}

// co 120 sekund, ale iterujemy co 4tego gracza 4x czesciej
Timer:PRA[30000](){   // PlayerRespectAddition
  gmTemp[timer_PRA_step]++;
  gmTemp[timer_PRA_step]=gmTemp[timer_PRA_step]%4;

  for(new playerid = gmTemp[timer_PRA_step]; playerid <= gmTemp[highestPlayerID]; playerid+=4)
  if(pData[playerid][loggedIn] && IsPlayerConnected(playerid) && !IsPlayerNPC(playerid) && pTemp[playerid][staleTime]<72) {
    GivePlayerScore(playerid,1 + (IsFS(playerid) ? 1 : 0) ,"automatyczny bonus co 72s", false);
    if (pData[playerid][hudSetting][HUD_STARS])
    {
      RefreshPlayerStars(playerid);
    }

    if ((pTemp[playerid][bonusHours]+1)*3600  < GetPlayerCurrentSession(playerid)) {
      pTemp[playerid][bonusHours]++;
      score_GivePlayerHourBonus(playerid, (pTemp[playerid][bonusHours] > 9) ? 9 : pTemp[playerid][bonusHours]);
    }
  }

}

Timer:EPS[250](){ // odpalany co 250ms, przetwarza za kazdym razem co 4tego gracza
  static fpsbuf[4],sec;

  sec=GetTickCount()/1000;

  gmTemp[timer_EPS_step]++;
  gmTemp[timer_EPS_step]=gmTemp[timer_EPS_step]%4;
  for(new playerid = gmTemp[timer_EPS_step]; playerid <= gmTemp[highestPlayerID]; playerid+=4)
  if(IsPlayerConnected(playerid) && !IsPlayerNPC(playerid)) {
      // AFK
      if (sec%15==0 && pTemp[playerid][staleTime]>(15*60) && pTemp[playerid][AFK] && (GetTickCount()-pData[playerid][session])>(15*60) && IsPlayerSpawned(playerid)) {
          SendClientMessage(playerid, -1, " ");
          SendClientMessage(playerid, -1, __("Zostajesz wyrzucony/-a za zbyt dluga nieaktywnosc."));
          KickPlayer(playerid);
          continue;
      }

      if (pTemp[playerid][staleTime]++>=3 && !pTemp[playerid][AFK] && !pData[playerid][classSelecting]) {
        pTemp[playerid][AFK]=true; pTemp[playerid][lcTicks]=0;
        UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"*AFK*");
      } else if (pTemp[playerid][AFK] && pTemp[playerid][staleTime]>=59 && pTemp[playerid][staleTime]%60==0 ) {
        static str[20];
        format(str,sizeof str, "*AFK %dmin*", floatround(pTemp[playerid][staleTime]/60,floatround_floor));
        UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,str);
      } else if (pTemp[playerid][AFK] && pTemp[playerid][staleTime]<3) {
        pTemp[playerid][AFK]=false;
        UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"");

      } else if (pTemp[playerid][lcTicks]>0) {
        if (--pTemp[playerid][lcTicks]==0) {
            pTemp[playerid][lastDmg]=0;
          UpdateDynamic3DTextLabelText(pTemp[playerid][p3d_status],0xff2222FF,"");
        }
      }
      // CJ HIT
      if (pTemp[playerid][hitTicks]>0) {
        if (--pTemp[playerid][hitTicks]==0) {
            pTemp[playerid][lastDmg]=0;
          returnCJHITNormal(playerid);
        }
      }
      // PRZETWARZANIE ILOSCI FPS
      if (IsPlayerSpawned(playerid) && pData[playerid][hudSetting][HUD_FPS]&&pTemp[playerid][staleTime]<3) {
        new FPS=GetPlayerFPS(playerid);
        if (FPS<1 || FPS>100){
        }else {
          format(fpsbuf,sizeof fpsbuf,"%02d", FPS);
          PlayerTextDrawSetString(playerid,pTextDraw[PTD_FPS][playerid],fpsbuf);
        }
      }
      //jail
      if(pData[playerid][jail] > 0 && IsPlayerSpawned(playerid) && (sec-gmTemp[interval_jailCheck])>=5)
      {
        if(pData[playerid][jail] <= sec)
        {
          UnjailPlayer(playerid);
          Msg(playerid, COLOR_INFO, __("Odsiedziales/as swoja kare, jestes wolny/-a!"));
        }
        else
        {
          new
           remainTime = ((pData[playerid][jail] - sec) / 60) + 1,
           buffer[64];

          format(buffer, sizeof buffer, __("~n~~n~~n~~n~~n~~y~Pozostalo: ~w~%i %s"), remainTime, GetPeriodName2(playerid, 'm', remainTime));
          GameTextForPlayer(playerid, buffer, 15000, 3);

          if(!IsPlayerInDynamicArea(playerid,DAs[eDA_jail]) && GetPlayerCurrentSession(playerid)>30)
          {
            Msg(playerid, COLOR_INFO2, __("Tak szybko nie uciekniesz!"));
            MoveToJail(playerid);
          }
        }
        gmTemp[interval_jailCheck]=sec;
      }
  }
}

Timer:PingUpdate[10000](){
  // przetwarzamy co 10 sekund, nie wiecej niz 60 graczy
  new i=0;
  if (gmTemp[timer_PingUpdate_lastidx]>gmTemp[highestPlayerID])
      gmTemp[timer_PingUpdate_lastidx]=0;

  for(new playerid=gmTemp[timer_PingUpdate_lastidx];playerid<=gmTemp[highestPlayerID]; playerid++)

  if(IsPlayerConnected(playerid))
      { // zapisujemy nie czesciej niz raz na 6 minut
    gmTemp[timer_PingUpdate_lastidx]=playerid;
    if(GetPlayerCurrentSession(playerid) > 3) // 3 minuty po polaczeniu z serwerem
    {
      new curping=GetPlayerPing(playerid);
      pTemp[playerid][pingSum] += curping;    // todo to jest zjebane przeciez - ale przed poprawieniem trzeba sprawdzic czy nie bedzie wywalac wszystkich
      pTemp[playerid][pingChecks]++;
      pData[playerid][averagePing] = pTemp[playerid][pingSum] / pTemp[playerid][pingChecks];

      if(pTemp[playerid][protping]) {
      } else
      if( ((pData[playerid][averagePing] > gmData[maxPing]) || (curping>(gmData[maxPing]*2))) && pTemp[playerid][pingChecks]>3 )
      {
        switch(++pTemp[playerid][pingWarningCount])
        {
          case 1..3: Msg(playerid, COLOR_INFO2, TXT(playerid, 178));
          case 4:
          {
            Msg(playerid, COLOR_INFO2, TXT(playerid, 179));
            KickPlayer(playerid);
          }
        }
      }
      else if (pTemp[playerid][pingWarningCount]>0)
        pTemp[playerid][pingWarningCount]--;
    }
    if (++i>60) return;
  }
  gmTemp[timer_PingUpdate_lastidx]=0;
}

Timer:LogoAnimation[50000](){ //bylo 50 sekund!
  gmTemp[fsaIndex] = 0;
  gmTemp[fsaTimer] = SetTimer("AnimateFullServerLogo", gmTemp[pPlayerCount]<100?120:240, true);
}

Timer:VehiclesCleanup[480000](){  // 8 minut
  foreach(playerid){
    if(pTemp[playerid][faction]==FACTION_POLICE){
        new kasa=5400+random(1200+200), buf[128];
            format(buf,sizeof buf,__("[FOTORADAR] {ff5050}Jestes policjantem i otrzymujesz kase z mandatow jako wynagrodzenie! {ffffff}+%d$"), kasa);
            SendClientMessage(playerid,-1,buf);
      GivePlayerMoney(playerid, kasa);
    }
  }
    RespawnVehicles();
}

Timer:Artefakt[60000](){ // 1 minuty
  gmTemp[timer_Artefakt_step]++;
  gmTemp[timer_Artefakt_step]=gmTemp[timer_Artefakt_step]%5;

  if(gmData[artefactOwner] == INVALID_PLAYER_ID) {
    if (gmTemp[artefactDropped]>0 && (GetTickCount()-gmTemp[artefactDropped])>(60*1000*5)) {
      CreateArtefact();

      gmTemp[artefactDropped]=0;
    }
    return;
  }
  if (pTemp[gmData[artefactOwner]][staleTime]>30) {
    Msg(gmData[artefactOwner], COLOR_ERROR, __("Niestety, zbyt dlugo byles/as AFK i {b}artefakt{/b} postanowil Cie opuscic!"));
    DropArtefact(gmData[artefactOwner]);
    return;
  }

  if(GetPlayerInterior(gmData[artefactOwner])>0 || pTemp[gmData[artefactOwner]][wStrefieNODM]==true || (tVehicles[pData[gmData[artefactOwner]][lastVehicle]][vo_driver]!=INVALID_PLAYER_ID && pTemp[tVehicles[pData[gmData[artefactOwner]][lastVehicle]][vo_driver]][godMode])) {  // artefakt nie dziala w interiorach
    switch (random(3)) {
      case 0: Msg(gmData[artefactOwner], COLOR_INFO2, __("Czujesz nieprzyjemne mrowienie na plecach..."));
      case 1: Msg(gmData[artefactOwner], COLOR_INFO2, __("Artefakt wydaje sie byc bardzo ciezki..."));
      case 2: Msg(gmData[artefactOwner], COLOR_INFO2, __("Po twoich plecach przechodzi dreszcz..."));
    }
      DropArtefact(gmData[artefactOwner]);

    return;
  }

  new
   baseTime = (GetTickCount() / 1000) - pTemp[gmData[artefactOwner]][artefactStartTime];

  if (baseTime>30*60) baseTime=30*60;
  new bonus=floatround((baseTime*sqrt(gmTemp[pPlayerCount])/400)/5)+1;
  // pol godziny 1800*10/400
  if(pData[gmData[artefactOwner]][loggedIn])
  GivePlayerScore(gmData[artefactOwner], bonus, "Od artefaktu");
  GivePlayerMoney(gmData[artefactOwner], floatround(baseTime*gmTemp[pPlayerCount]/8)+1);

  switch((gmTemp[timer_Artefakt_step]+random(2))%5){
    case 0,1,2: {
      new Float:php;
      GetPlayerHealth(gmData[artefactOwner], php);
      if(php<90) {
        php+=40.0;
        if (php>100) php=100.0;
        SetPlayerHealth(gmData[artefactOwner], php);
        Msg(gmData[artefactOwner], COLOR_INFO, __("Artefakt odnawia Ci HP"), false);
        return;
      }
    }
    case 3,4: {
      new Float:par;
      GetPlayerArmour(gmData[artefactOwner], par);
      if(par<90) {
        par+=20.0+random(25);
        if (par>100) par=100.0;
        SetPlayerArmour(gmData[artefactOwner], par);
        Msg(gmData[artefactOwner], COLOR_INFO, __("Artefakt odnawia Ci armor"), false);
        return;
      }

    }
  }

}
// EOF
