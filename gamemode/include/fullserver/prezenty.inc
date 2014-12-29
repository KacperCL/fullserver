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

// WALIZKA: 13% szans na bombe, respekt od 0 do 9, gp od 0 do 5, kasy 0-25k
// PREZENt: 14% szans na bombe, respekt od 2 do 12, gp od 1 do 8, kasy 0-15k
// PODKOWA: bez bomb, respekt 0-4, gp od 0 do 2, kasa 0-35k
// TORBA: bez bomb, bez respektu, kasa 0-55k

#define MAX_GIFT_PICKUPS 4
#define GIFT_WALIZKA   0
#define GIFT_PREZENT   1
#define GIFT_PODKOWA   2
#define GIFT_TORBA       3

enum e_Prezenty
{
    bool:epAktywne,
    epPickup,
    epDynamicArea,
    Float:epXYZ[3],
    epInterior,
    epVirtualWorld,
    epOpis[256],
    epDid,
    epCash,
    epRespect,
    epGamePoints

}

new
 gPrezentyPickup[MAX_GIFT_PICKUPS][e_Prezenty],
 gPrezentyAudio[MAX_PLAYERS];

CMD:walizka(playerid,params[]){
    if(!IsGM(playerid)) {
    if (!gPrezentyPickup[GIFT_WALIZKA][epAktywne])
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej walizki do odnalezienia");  // obecnie nie ma walizki do znalezenia
    else
      return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_WALIZKA][epOpis]);
  }

  if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
    if (Bonus_DestroyPickup(GIFT_WALIZKA))
      return Msg(playerid,COLOR_INFO,"Walizka usunieta");
    else
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej walizki!");
  }
  if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
    if (gPrezentyPickup[GIFT_WALIZKA][epAktywne])
      Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_WALIZKA][epXYZ][0],   gPrezentyPickup[GIFT_WALIZKA][epXYZ][1], gPrezentyPickup[GIFT_WALIZKA][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_WALIZKA][epInterior], gPrezentyPickup[GIFT_WALIZKA][epVirtualWorld]);
    else
      Msg(playerid,COLOR_ERROR, "Nie ma obecnie walizki na mapie");
    return 1;
  }
  new podpowiedz[200];
  if(sscanf(params,"s[200]",podpowiedz)) {
    if (!gPrezentyPickup[GIFT_WALIZKA][epAktywne])
      Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnej walizki.");
    else
      Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_WALIZKA][epOpis]);

    return Msg(playerid,COLOR_ERROR,"/walizka {b}<podpowiedz/tp/usun>{/b}");
  }

  new bool:zaevent=true;
  if (nowyPrezent(playerid,GIFT_WALIZKA)) {
    format(gPrezentyPickup[GIFT_WALIZKA][epOpis],256,"~w~%s zgubil%s walizke.~n~Podpowiedz: ~r~%s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
    if (zaevent)
      ShowAnnouncement2(gPrezentyPickup[GIFT_WALIZKA][epOpis]);
    else
      ShowAnnouncement(gPrezentyPickup[GIFT_WALIZKA][epOpis]);
    format(gPrezentyPickup[GIFT_WALIZKA][epOpis],256,"%s zgubil%s walizke. Podpowiedz: %s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
  }
    return 1;
}

CMD:prezent(playerid,params[]){
    if(!IsGM(playerid)) {
    if (!gPrezentyPickup[GIFT_PREZENT][epAktywne])
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnego prezentu na mapie.");  // obecnie nie ma prezentu do znalezenia
    else
      return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PREZENT][epOpis]);
  }
  if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
    if (Bonus_DestroyPickup(GIFT_PREZENT))
      return Msg(playerid,COLOR_INFO,"Prezent usuniety");
    else
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnego prezentu!");
  }
  if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
    if (gPrezentyPickup[GIFT_PREZENT][epAktywne])
      Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_PREZENT][epXYZ][0],   gPrezentyPickup[GIFT_PREZENT][epXYZ][1], gPrezentyPickup[GIFT_PREZENT][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_PREZENT][epInterior], gPrezentyPickup[GIFT_PREZENT][epVirtualWorld]);
    else
      Msg(playerid,COLOR_ERROR, "Nie ma obecnie prezentu na mapie");
    return 1;
  }

  new podpowiedz[200];
  if(sscanf(params,"s[200]",podpowiedz)) {
    if (!gPrezentyPickup[GIFT_PREZENT][epAktywne])
      Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnego prezentu.");
    else
      Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PREZENT][epOpis]);

    return Msg(playerid,COLOR_ERROR,"/prezent {b}<podpowiedz/tp/usun>{/b}");
  }

  new bool:zaevent=true;

  if (nowyPrezent(playerid,GIFT_PREZENT)) {
    format(gPrezentyPickup[GIFT_PREZENT][epOpis],256,"~w~%s zgubil%s prezent.~n~Podpowiedz: ~b~%s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
    if (zaevent)
      ShowAnnouncement2(gPrezentyPickup[GIFT_PREZENT][epOpis]);
    else
      ShowAnnouncement(gPrezentyPickup[GIFT_PREZENT][epOpis]);
    format(gPrezentyPickup[GIFT_PREZENT][epOpis],256,"%s zgubil%s prezent.~n~Podpowiedz: %s",GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
  }
  return 1;
}

CMD:podkowa(playerid,params[]){
    if(!IsGM(playerid)) {
    if (!gPrezentyPickup[GIFT_PODKOWA][epAktywne])
      return Msg(playerid,COLOR_INFO,__("Obecnie nie ma zadnej ukrytej podkowy"));  // obecnie nie ma prezentu do znalezenia
    else
      return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PODKOWA][epOpis]);
  }

  if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
    if (Bonus_DestroyPickup(GIFT_PODKOWA))
      return Msg(playerid,COLOR_INFO,"Podkowa usunieta");
    else
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej podkowy!");
  }
  if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
    if (gPrezentyPickup[GIFT_PODKOWA][epAktywne])
      Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_PODKOWA][epXYZ][0],   gPrezentyPickup[GIFT_PODKOWA][epXYZ][1], gPrezentyPickup[GIFT_PODKOWA][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_PODKOWA][epInterior], gPrezentyPickup[GIFT_PODKOWA][epVirtualWorld]);
    else
      Msg(playerid,COLOR_ERROR, "Nie ma obecnie podkowy na mapie");
    return 1;
  }

  new podpowiedz[200];
  if(sscanf(params,"s[200]",podpowiedz)) {
    if (!gPrezentyPickup[GIFT_PODKOWA][epAktywne])
      Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnej podkowy.");
    else
      Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PODKOWA][epOpis]);

    return Msg(playerid,COLOR_ERROR,"/podkowa {b}<podpowiedz/tp/usun>{/b}");
  }

  new bool:zaevent=true;

  if (nowyPrezent(playerid,GIFT_PODKOWA)) {
    format(gPrezentyPickup[GIFT_PODKOWA][epOpis],256,"~w~%s ukryl%s gdzies podkowe.~n~Podpowiedz: ~b~%s",GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
    if (zaevent)
      ShowAnnouncement2(gPrezentyPickup[GIFT_PODKOWA][epOpis]);
    else
      ShowAnnouncement(gPrezentyPickup[GIFT_PODKOWA][epOpis]);
    format(gPrezentyPickup[GIFT_PODKOWA][epOpis],256,"%s ukryl%s gdzies podkowe.~n~Podpowiedz: %s",GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
  }
  return 1;

}

CMD:torba(playerid,params[]){
    if(!IsGM(playerid)) {
    if (!gPrezentyPickup[GIFT_TORBA][epAktywne])
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej torby z kasa do odnalezienia"); // obecnie nie ma walizki do znalezenia
    else
      return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_TORBA][epOpis]);
  }

  if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
    if (Bonus_DestroyPickup(GIFT_TORBA))
      return Msg(playerid,COLOR_INFO,"Torba z kasa usunieta");
    else
      return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej torby z kasa!");
  }
  if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
    if (gPrezentyPickup[GIFT_TORBA][epAktywne])
      Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_TORBA][epXYZ][0],   gPrezentyPickup[GIFT_TORBA][epXYZ][1], gPrezentyPickup[GIFT_TORBA][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_TORBA][epInterior], gPrezentyPickup[GIFT_TORBA][epVirtualWorld]);
    else
      Msg(playerid,COLOR_ERROR, "Nie ma obecnie torby z kasa na mapie");
    return 1;
  }
  new podpowiedz[200];
  if(sscanf(params,"s[200]",podpowiedz)) {
    if (!gPrezentyPickup[GIFT_TORBA][epAktywne])
      Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnej torby z kasa.");
    else
      Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_TORBA][epOpis]);

    return Msg(playerid,COLOR_ERROR,"/torba {b}<podpowiedz/tp/usun>{/b}");
  }

  new bool:zaevent=true;

  if (nowyPrezent(playerid,GIFT_TORBA)) {
    format(gPrezentyPickup[GIFT_TORBA][epOpis],256,"~w~%s zgubil%s torbe z kasa.~n~Podpowiedz: ~r~%s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
    if (zaevent)
      ShowAnnouncement2(gPrezentyPickup[GIFT_TORBA][epOpis]);
    else
      ShowAnnouncement(gPrezentyPickup[GIFT_TORBA][epOpis]);
    format(gPrezentyPickup[GIFT_TORBA][epOpis],256,"%s zgubil%s torbe z kasa. Podpowiedz: %s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
  }
    return 1;
}

CMD:bonusy(playerid,params[]){
  new
      tmpMsg[256],
    szMsg[128];
    if(!gPrezentyPickup[GIFT_PREZENT][epAktywne]) Msg(playerid, COLOR_INFO, "{b}Prezent:{/b} Nie ma ¿adnego prezentu!", false);
    else {
      format(tmpMsg,256,"%s",gPrezentyPickup[GIFT_PREZENT][epOpis]);
            strdel(tmpMsg, 0, strfind(tmpMsg, ":", true)+1);
      format(szMsg, sizeof szMsg, "{b}Prezent:{/b} %s",tmpMsg);
      Msg(playerid, COLOR_INFO, szMsg, false);
    }

  if(!gPrezentyPickup[GIFT_WALIZKA][epAktywne]) Msg(playerid, COLOR_INFO, "{b}Walizka:{/b} Nie ma ¿adnej walizki!", false);
    else {
      format(tmpMsg,256,"%s",gPrezentyPickup[GIFT_WALIZKA][epOpis]);
      strdel(tmpMsg, 0, strfind(tmpMsg, ":", true)+1);
      format(szMsg, sizeof szMsg, "{b}Walizka:{/b} %s",tmpMsg);
      Msg(playerid, COLOR_INFO, szMsg, false);
    }

  if(!gPrezentyPickup[GIFT_PODKOWA][epAktywne]) Msg(playerid, COLOR_INFO, "{b}Podkowa:{/b} Nie ma ¿adnej podkowy!", false);
    else {
      format(tmpMsg,256,"%s",gPrezentyPickup[GIFT_PODKOWA][epOpis]);
      strdel(tmpMsg, 0, strfind(tmpMsg, ":", true)+1);
      format(szMsg, sizeof szMsg, "{b}Podkowa:{/b} %s",tmpMsg);
      Msg(playerid, COLOR_INFO, szMsg, false);
    }
  if(!gPrezentyPickup[GIFT_TORBA][epAktywne]) Msg(playerid, COLOR_INFO, "{b}Torba z kas¹:{/b} Nie ma ¿adnej torby z kas¹!", false);
    else {
      format(tmpMsg,256,"%s",gPrezentyPickup[GIFT_TORBA][epOpis]);
      strdel(tmpMsg, 0, strfind(tmpMsg, ":", true)+1);
      format(szMsg, sizeof szMsg, "{b}Torba z kas¹:{/b} %s",tmpMsg);
      Msg(playerid, COLOR_INFO, szMsg, false);
    }
  return 1;
}

CMD:kasa(playerid,params[]){
  return _torba(playerid,params);
}

nowyPrezent(playerid,r){
    if (gPrezentyPickup[r][epAktywne]) {
    Msg(playerid,COLOR_ERROR,"Na mapie znajduje sie juz walizka/prezent.");

    return 0;
  }

    GetPlayerPos(playerid,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2]);
    gPrezentyPickup[r][epInterior]=GetPlayerInterior(playerid);
    gPrezentyPickup[r][epVirtualWorld]=GetPlayerVirtualWorld(playerid);
  for(new i=0;i<MAX_GIFT_PICKUPS;i++) {
    if (r==i || gPrezentyPickup[i][epAktywne]==false ||
      gPrezentyPickup[i][epInterior]!=gPrezentyPickup[r][epInterior] || gPrezentyPickup[i][epVirtualWorld]!=gPrezentyPickup[r][epVirtualWorld]) continue;
    // aktywny bonus
    if (GetDistanceBetweenPoints3D(
        gPrezentyPickup[r][epXYZ][0], gPrezentyPickup[r][epXYZ][1], gPrezentyPickup[r][epXYZ][2],
        gPrezentyPickup[i][epXYZ][0], gPrezentyPickup[i][epXYZ][1], gPrezentyPickup[i][epXYZ][2]) <= 80.0) {
      Msg(playerid,COLOR_ERROR,"W poblizu znajduje sie inny bonus, nie mozesz tu postawic drugiego");
      return 0;
      }
  }

  switch(r){
    case GIFT_WALIZKA:{
      gPrezentyPickup[r][epPickup]=CreateDynamicPickup(1210,1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
        gPrezentyPickup[r][epRespect]=random(13)-3;   // dlaczego losujemy przy tworzeniu walizki?
      gPrezentyPickup[r][epCash]=random(25000);   // zeby umozliwic wlascicielowi artefaktu widzenie zawartosci
            gPrezentyPickup[r][epGamePoints]=random(6);
      // gdy respect==-1 => bomba
    }
    case GIFT_PREZENT:{
      gPrezentyPickup[r][epPickup]=CreateDynamicPickup(19054+random(5),1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
      gPrezentyPickup[r][epRespect]=random(7);
      if (gPrezentyPickup[r][epRespect]==0) gPrezentyPickup[r][epRespect]=-1;
      else gPrezentyPickup[r][epRespect]*=2;
      gPrezentyPickup[r][epCash]=random(15000);
      gPrezentyPickup[r][epGamePoints]=random(11)+2;
    }
    case GIFT_PODKOWA:{
      gPrezentyPickup[r][epPickup]=CreateDynamicPickup(954,1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
      gPrezentyPickup[r][epRespect]=random(5);
      gPrezentyPickup[r][epCash]=random(35000);
            gPrezentyPickup[r][epGamePoints]=random(3);
    }
    case GIFT_TORBA:{
        gPrezentyPickup[r][epPickup]=CreateDynamicPickup(1550,1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
      gPrezentyPickup[r][epRespect]=0;
      gPrezentyPickup[r][epCash]=random(55000);
    }
  }

    gPrezentyPickup[r][epAktywne]=true;
    gPrezentyPickup[r][epDynamicArea]=CreateDynamicSphere(gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],75,gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1);

    gPrezentyPickup[r][epDid]=playerid;


  return 1;
}

Bonus_DestroyPickup(pickuptype){
  if (!gPrezentyPickup[pickuptype][epAktywne]) return false;
  DestroyDynamicPickup(gPrezentyPickup[pickuptype][epPickup]);
  gPrezentyPickup[pickuptype][epAktywne]=false;

  foreach(userid){
    if (IsPlayerInDynamicArea(userid,gPrezentyPickup[pickuptype][epDynamicArea])){
      Audio_Stop(userid, gPrezentyAudio[userid]);
    }
  }
  DestroyDynamicArea(gPrezentyPickup[pickuptype][epDynamicArea]);
  return 1;
}

prezenty_OPEnterDynamicArea(playerid,areaid){
  #pragma unused playerid
    // prezenty (walizka,paczka,...)
    for (new i=0;i<MAX_GIFT_PICKUPS;i++)
  if (gPrezentyPickup[i][epAktywne] && gPrezentyPickup[i][epDynamicArea]==areaid) { // gracz znajduje sie w zasiegu prezentu
      if (!Audio_IsClientConnected(playerid)) return 1;
    if (gPrezentyAudio[playerid]) Audio_Stop(playerid,gPrezentyAudio[playerid]);
      gPrezentyAudio[playerid]=Audio_Play(playerid,AUDIOID_WALIZKA+i,false,true,true);
      Audio_Set3DPosition(playerid, gPrezentyAudio[playerid], gPrezentyPickup[i][epXYZ][0], gPrezentyPickup[i][epXYZ][1], gPrezentyPickup[i][epXYZ][2], 50);
      return 1;
  }
    return 0;
}

prezenty_OPLeaveDynamicArea(playerid,areaid){
    #pragma unused playerid
    for (new i=0;i<MAX_GIFT_PICKUPS;i++)
  if (gPrezentyPickup[i][epDynamicArea]==areaid) {  // gracz znajduje sie w zasiegu prezentu
      if (!Audio_IsClientConnected(playerid)) return 1;
      Audio_Stop(playerid, gPrezentyAudio[playerid]);
      return 1;
  }
    return 0;
}

prezenty_OPPickUpDynamicPickup(playerid,pickupid){
    for (new i=0;i<MAX_GIFT_PICKUPS;i++)
    if (gPrezentyPickup[i][epAktywne] && pickupid==gPrezentyPickup[i][epPickup]) {
    if (IsGM(playerid)) return 1;
    new str[127];

    if (gPrezentyPickup[i][epRespect]<0) {  // bomba
      format(str,sizeof(str),"{ffffff}%s {cccccc}odnalazl/-a bonus, niestety mia³ wybuchowa zawartosc!",GetPlayerProperName(playerid));
      CreateExplosion(gPrezentyPickup[i][epXYZ][0]+random(1)-0.5, gPrezentyPickup[i][epXYZ][1]+random(1)-0.5, gPrezentyPickup[i][epXYZ][2]+random(1)-0.5, 2, 50);
      CreateExplosion(gPrezentyPickup[i][epXYZ][0]+random(1)-0.5, gPrezentyPickup[i][epXYZ][1]+random(1)-0.5, gPrezentyPickup[i][epXYZ][2]+random(1)-0.5, 2, 50);
      CreateExplosion(gPrezentyPickup[i][epXYZ][0]+random(1)-0.5, gPrezentyPickup[i][epXYZ][1]+random(1)-0.5, gPrezentyPickup[i][epXYZ][2]+random(1)-0.5, 2, 50);
    } else {
      if (SkinKobiecy(GetPlayerSkin(playerid)))
        format(str,sizeof(str),"%s {cccccc}odnalazla bonus i otrzymala ${ffffff}%d{cccccc}, {ffffff}%d {cccccc}rsp. i {ffffff}%d {cccccc}GP",GetPlayerProperName(playerid), gPrezentyPickup[i][epCash], gPrezentyPickup[i][epRespect],gPrezentyPickup[i][epGamePoints]);
      else
        format(str,sizeof(str),"%s {cccccc}odnalazl bonus i otrzymal ${ffffff}%d{cccccc}, {ffffff}%d {cccccc}rsp. i {ffffff}%d {cccccc}GP",GetPlayerProperName(playerid), gPrezentyPickup[i][epCash], gPrezentyPickup[i][epRespect],gPrezentyPickup[i][epGamePoints]);

      GivePlayerScore(playerid, gPrezentyPickup[i][epRespect],"walizka/podkowa/prezent");
      GivePlayerGamePoints(playerid, gPrezentyPickup[i][epGamePoints],"walizka/podkowa/prezent");
    }

    foreach(player)
        Msg(player,-1,str);

    GivePlayerMoney(playerid, gPrezentyPickup[i][epCash]);

    avt_record(playerid,e_walizki,1,ART_ADD);

    Bonus_DestroyPickup(i);


    return 1;
    }
    return 0;
}
// EOF
