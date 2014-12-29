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

forward FadePlayerAchievement(playerid,step);
forward FadePlayerScore(playerid,step,oldscore);
forward FadePlayerGamePoints(playerid,step,oldscore);

new
  players_achievements_ranks[MAX_PLAYERS][e_achievements];

new
 avt_shortnames[e_achievements][12]={
  "WALIZKI",
  "KILLS",
  "LONGPLAY",
  "RACEFCNT",
  "RACEDIST",
  "DERBYFAN",
  "DERBYWIN",
  "FOTOCASH",
  "DRIFTPTS",
  "LISTONOSZ",
  "CHOWIN",
  "CHOFAN",
  "PACZKI",
  "QUIZ"
};

avt_record(playerid,e_achievements:atype, score, artype=ART_ADD){
  if (pData[playerid][loggedIn] && pData[playerid][accountID]>0) {
    new buffer[1023];
    if (artype==ART_UPDATE) // nadpisujemy wynik
      format(buffer,sizeof buffer,"INSERT INTO fs_players_achievements SET playerid=%d,shortname='%s',score=%d ON DUPLICATE KEY UPDATE score=%d",
        pData[playerid][accountID],avt_shortnames[atype],score,score);
    else // dodajemy do wyniku
      format(buffer,sizeof buffer,"INSERT INTO fs_players_achievements SET playerid=%d,shortname='%s',score=%d ON DUPLICATE KEY UPDATE score=score+(%d)",
        pData[playerid][accountID],avt_shortnames[atype],score,score);
    mysql_query(buffer,SQL_RI_BACKGROUND);
  }
  SetTimerEx("avt_refreshRankDesc",500,false,"dd",playerid,_:atype);
  return 1;
}

forward avt_refreshRankDesc(playerid,atype);
public avt_refreshRankDesc(playerid,atype){
  avt_getCurrentRankDesc(playerid,e_achievements:atype,false);
}


avt_getCurrentRankDesc(playerid,e_achievements:atype,bool:quiet=false){
  if (!(pData[playerid][accountID]>0)) {
    players_achievements_ranks[playerid][atype]=0;
    return;
  }

  new buffer[1023];
  format(buffer,sizeof buffer,"SELECT a.name,ar.id,ar.rank,ar.replacename FROM fs_players_achievements pa JOIN fs_achievements a ON pa.shortname=a.shortname JOIN fs_achievements_ranks ar ON ar.shortname=pa.shortname AND ar.score<=pa.score WHERE pa.playerid=%d AND pa.shortname='%s' ORDER BY ar.score DESC LIMIT 1",
      pData[playerid][accountID],avt_shortnames[atype]);

  mysql_query(buffer);
  mysql_store_result();
  if (!mysql_num_rows()) { mysql_free_result(); return; } // brak wynikow
  mysql_fetch_row_format(buffer,"|");
  mysql_free_result();

  new avt_name[50],avt_rankid,avt_rank[50],fullname[68],bool:replacename;
  sscanf(buffer,"p<|>s[50]ds[50]l",avt_name,avt_rankid,avt_rank,replacename);

  if (!quiet && players_achievements_ranks[playerid][atype]!=avt_rankid) {  // zmienila sie ranga
    if (!replacename)
      format(fullname, sizeof fullname, "%s: %s", avt_name, avt_rank);
    else
      copy(avt_rank,fullname);
    ShowPlayerAchievement(playerid,fullname);

  }
  players_achievements_ranks[playerid][atype]=avt_rankid;
}

avt_getPlayerAchievements(playerid){
  // narazie nie pobieramy tylko zerujemy
  for(new i=0; i<sizeof avt_shortnames;i++) // zoptymalizowac!
    avt_getCurrentRankDesc(playerid,e_achievements:i,true); // po cichu, bez powiadamiania
  return;
}

avt_zeroPlayerAchievements(playerid){
  for(new i=0; i<sizeof avt_shortnames;i++)
    players_achievements_ranks[playerid][e_achievements:i]=0;
  return;
}

forward ShowPlayerAchievement(playerid,reason[]);
public ShowPlayerAchievement(playerid,reason[]){
  new str[255];
  format(str,sizeof str, __("Nowe osiagniecie: {b}%s{/b}"), reason);
  Msg(playerid,COLOR_INFO2,str);

  if (pTemp[playerid][tdanimAchievement])   // juz trwa animacja wiec ta wklejamy na czacie tylko
    return 1;

  format(str, sizeof str, __("Nowe osiagniecie! ~n~~y~%s"),reason);

  if (Audio_IsClientConnected(playerid))
    Audio_Play(playerid,AUDIOID_ACHIEVEMENT,false,false,true);
  else
    PlaySound(playerid,1149);

  pTemp[playerid][tdanimAchievement]=true;

  pTextDraw[PTD_ACHIEVEMENT][playerid] = CreatePlayerTextDraw(playerid,300.000000, 270.000000, str);
  PlayerTextDrawAlignment(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 3);
  PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 255);
  PlayerTextDrawFont(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
  PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0.330000, 1.100000);
  PlayerTextDrawColor(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0xffffff00);
  PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0);
  PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
  PlayerTextDrawSetShadow(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
  PlayerTextDrawUseBox(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 1);
  PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0xFFA50000);
  PlayerTextDrawTextSize(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0.000000, 162.000000);

  PlayerTextDrawShow(playerid, pTextDraw[PTD_ACHIEVEMENT][playerid]);

  SetTimerEx("FadePlayerAchievement",250,false,"dd",playerid,1);
  return 1;
}

public FadePlayerAchievement(playerid,step){
  // 1-4  pokazywanie
  // 5 pauza
  // 6-10 wygaszanie
  // 11 usuwanie
  new add;
  if (step==11) {
    PlayerTextDrawHide(playerid, pTextDraw[PTD_ACHIEVEMENT][playerid]);
    PlayerTextDrawDestroy(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid]);
    pTemp[playerid][tdanimAchievement]=false;
    return;
  } else if (step==5)
    add=85;
  else if (step<5)
    add=step*17;
  else
    add=(11-step)*16;
  PlayerTextDrawColor(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0xffffff00+(add*3));
  PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_ACHIEVEMENT][playerid], 0xFFA50000+add);
  PlayerTextDrawShow(playerid, pTextDraw[PTD_ACHIEVEMENT][playerid]);
  step++;
  SetTimerEx("FadePlayerAchievement",(step==6 ? 7500 : 120)+(step*4),false,"dd",playerid,step);
  return;
}

/// SCORE
GivePlayerScore(playerid,amount,reason[], bool:show=true){
#pragma unused reason
  if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return;

  if(pTemp[playerid][doubleMode] && amount>0) amount=amount*2;

  pData[playerid][respect] += amount;
  SetPlayerScore(playerid, pData[playerid][respect]);
  if (!show) return;

  if (amount>=15)
    PlaySound(playerid,1137); // sound_checkpoint_amber
  else if (amount>=10)
    PlaySound(playerid,1139); // sound_checkpoint_red
  else if (amount>=5)
    PlaySound(playerid,1138); // sound_checkpoint_green

  new str[5];

  if (pTemp[playerid][tdanimScore]!=0){ // juz trwa animacja naliczania score - wiec dodajemy tylko wynik i przedluzamy (jesli juz znika)
    pTemp[playerid][tdanimScore]+=amount;
    if (pTemp[playerid][tdanimScore]>0)
      format(str,sizeof str,"+%d",pTemp[playerid][tdanimScore]);
    else
      format(str,sizeof str,"%d",pTemp[playerid][tdanimScore]);
    PlayerTextDrawSetString(playerid,pTextDraw[PTD_SCORECHANGE][playerid], str);
    return;
  }

  if (amount>0){
    format(str,sizeof str,"+%d",amount);
  }
  else
  {
    format(str,sizeof str,"%d",amount);
  }

  PlayerPlaySound(playerid, 1058, 0.0,0.0,0.0);

  pTemp[playerid][tdanimScore]=amount;

  pTextDraw[PTD_SCORECHANGE][playerid] = CreatePlayerTextDraw(playerid,395.000000, 410.000000, str);
  PlayerTextDrawAlignment(playerid,pTextDraw[PTD_SCORECHANGE][playerid], 2);
  PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_SCORECHANGE][playerid], 96);
  PlayerTextDrawFont(playerid,pTextDraw[PTD_SCORECHANGE][playerid], 1);
  PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_SCORECHANGE][playerid], 0.639999, 2.099999);
  PlayerTextDrawColor(playerid,pTextDraw[PTD_SCORECHANGE][playerid], (pTemp[playerid][tdanimScore]>0) ? 0x30ff30aa : 0xff3030aa);
  PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_SCORECHANGE][playerid], 1);
  PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_SCORECHANGE][playerid], 0);

  PlayerTextDrawShow(playerid, pTextDraw[PTD_SCORECHANGE][playerid]);

  SetTimerEx("FadePlayerScore",250,false,"ddd",playerid,1,pTemp[playerid][tdanimScore]);
}

public FadePlayerScore(playerid,step,oldscore){
  // 1-4  pokazywanie
  // 5 pauza
  // 6-10 wygaszanie
  // 11 usuwanie
  if (pTemp[playerid][tdanimScore]!=oldscore && step>4) // nastapila doliczenie score w momencie gdy bylo juz pokazywane, resetujemy krok animacji
    step = 4;

  new add;
  if (step==11) {
    PlayerTextDrawHide(playerid, pTextDraw[PTD_SCORECHANGE][playerid]);
    pTemp[playerid][tdanimScore]=0;
    PlayerTextDrawDestroy(playerid,pTextDraw[PTD_SCORECHANGE][playerid]);
    return;
  } else if (step==5)
    add=85;
  else if (step<5)
    add=step*17;
  else
    add=(11-step)*16;
  PlayerTextDrawColor(playerid,pTextDraw[PTD_SCORECHANGE][playerid],((pTemp[playerid][tdanimScore]>0) ? 0x30ff3000 : 0xff303000)+(add*3));
  PlayerTextDrawShow(playerid, pTextDraw[PTD_SCORECHANGE][playerid]);
  step++;
  SetTimerEx("FadePlayerScore",(step==6 ? 4500 : 60)+(step*4),false,"ddd",playerid,step,pTemp[playerid][tdanimScore]);
  return;
}

score_GivePlayerHourBonus(playerid, hour)
{
  if (hour<1) return;
  new
   buffer[128],
   exp, kasa, gp;

  if(!IsFS(playerid))
  {
    kasa=(1000+((hour * 250) + 75));
    exp = (hour * 25) + 75;   // 100, 125, ...
    gp = (hour * 1) + 1; // 2, 3, 4
  }
  else
  {
    kasa=(2000+((hour * 250) + 125));
    exp = (hour * 25) + 125;  // 150, 175, ...
    gp = (hour * 2) + 2; // 4, 8, 16
  }
  
  if (gmTemp[lastHour]<=6 || gmTemp[lastHour]>=23){
    kasa=kasa+(random(500)+500);
    exp=exp+(random(10)+5);
    gp=gp+(random(5)+5);
  }

  if(pTemp[playerid][doubleMode]) {
    gp=gp*2;
  }

  format(buffer, sizeof buffer, __("Dostajesz {b}%i respektu{/b}, {b}%d${/b} oraz {b}%d GP{/b} za nastepna godzine gry!"), exp, kasa, gp);
  Msg(playerid, COLOR_INFO, buffer, false);

  GivePlayerMoney(playerid,kasa);
  avt_record(playerid,e_longplay, 1, ART_ADD);
  GivePlayerScore(playerid, exp, "Bonus za czas gry");
  GivePlayerGamePoints(playerid, gp, "Bonus za czas gry");
}

//GAME POINTS
GivePlayerGamePoints(playerid,amount,reason[], bool:show=true){
#pragma unused reason
  if (!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return;

  // if(pTemp[playerid][doubleMode] && amount>0) amount=amount*2;

  pData[playerid][gamep] += amount;
  if (!show) return;

  if (amount>=3)
    PlaySound(playerid,1137); // sound_checkpoint_amber
  else if (amount>=2)
    PlaySound(playerid,1139); // sound_checkpoint_red
  else if (amount>=1)
    PlaySound(playerid,1138); // sound_checkpoint_green

    new str[5];

  if (pTemp[playerid][tdanimGp]!=0){  // juz trwa animacja naliczania score - wiec dodajemy tylko wynik i przedluzamy (jesli juz znika)
    pTemp[playerid][tdanimGp]+=amount;
    if (pTemp[playerid][tdanimGp]>0)
      format(str,sizeof str,"+%d",pTemp[playerid][tdanimGp]);
    else
      format(str,sizeof str,"%d",pTemp[playerid][tdanimGp]);
    PlayerTextDrawSetString(playerid,pTextDraw[PTD_GPCHANGE][playerid], str);
    return;
  }

  if (amount>0){
    format(str,sizeof str,"+%d",amount);
  }
  else
  {
    format(str,sizeof str,"%d",amount);
  }

  PlayerPlaySound(playerid, 1058, 0.0,0.0,0.0);

  pTemp[playerid][tdanimGp]=amount;

  pTextDraw[PTD_GPCHANGE][playerid] = CreatePlayerTextDraw(playerid,440.000000, 410.000000, str);
  PlayerTextDrawAlignment(playerid,pTextDraw[PTD_GPCHANGE][playerid], 2);
  PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_GPCHANGE][playerid], 96);
  PlayerTextDrawFont(playerid,pTextDraw[PTD_GPCHANGE][playerid], 1);
  PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_GPCHANGE][playerid], 0.639999, 2.099999);
  PlayerTextDrawColor(playerid,pTextDraw[PTD_GPCHANGE][playerid], (pTemp[playerid][tdanimGp]>0) ? 0x1c2f80aa : 0xff3030aa);
  PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_GPCHANGE][playerid], 1);
  PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_GPCHANGE][playerid], 0);

  PlayerTextDrawShow(playerid, pTextDraw[PTD_GPCHANGE][playerid]);

  SetTimerEx("FadePlayerGamePoints",250,false,"ddd",playerid,1,pTemp[playerid][tdanimGp]);
}

public FadePlayerGamePoints(playerid,step,oldscore){
  // 1-4  pokazywanie
  // 5 pauza
  // 6-10 wygaszanie
  // 11 usuwanie
  if (pTemp[playerid][tdanimGp]!=oldscore && step>4)  // nastapila doliczenie score w momencie gdy bylo juz pokazywane, resetujemy krok animacji
    step = 4;

  new add;
  if (step==11) {
    PlayerTextDrawHide(playerid, pTextDraw[PTD_GPCHANGE][playerid]);
    pTemp[playerid][tdanimGp]=0;
    PlayerTextDrawDestroy(playerid,pTextDraw[PTD_GPCHANGE][playerid]);
    return;
  } else if (step==5)
    add=85;
  else if (step<5)
    add=step*17;
  else
    add=(11-step)*16;
  PlayerTextDrawColor(playerid,pTextDraw[PTD_GPCHANGE][playerid],((pTemp[playerid][tdanimGp]>0) ? 0x1c2f8000 : 0xff303000)+(add*3));
  PlayerTextDrawShow(playerid, pTextDraw[PTD_GPCHANGE][playerid]);
  step++;
  SetTimerEx("FadePlayerGamePoints",(step==6 ? 4500 : 60)+(step*4),false,"ddd",playerid,step,pTemp[playerid][tdanimGp]);
  return;
}
// EOF
