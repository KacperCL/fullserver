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

#define DLG_POCZTA          (8888)
#define DLG_POCZTA_INFO       (DLG_POCZTA+0)
#define DLG_POCZTA_ODBIORCA     (DLG_POCZTA+1)
#define DLG_POCZTA_ODBIORCA_LIST  (DLG_POCZTA+2)
#define DLG_POCZTA_TRESC      (DLG_POCZTA+3)
#define DLG_POCZTA_LISTY      (DLG_POCZTA+4)
#define DLG_POCZTA_LIST       (DLG_POCZTA+5)

#define FULLSERVER_BOT_UID      (1)   // id gracza o nicku FullServer - od niego beda wychodzic automatyczne wiadomosci

// ile listonosz moze wziasc naraz listow
#define POCZTA_LISTOW 4

new poczta_cp_regulamin;
new Text3D:poczta_3dtext_regulamin;
#pragma unused poczta_3dtext_regulamin
new poczta_cp_nadawanie;
new Text3D:poczta_3dtext_nadawanie;
#pragma unused poczta_3dtext_nadawanie
new poczta_cp_odbior;
new Text3D:poczta_3dtext_odbior;

new poczta_odbiorcy[MAX_PLAYERS]; // przechowuje bazodanowe id graczy po wybraniu ich w dialogu
new poczta_last[MAX_PLAYERS] = { -18000, ... };


enum ep_list {  // list do dostarczenia przez gracza
  epl_dbid, // id listu w bazie danych
  epl_cp,   // checkpoint
  epl_mi    // mapicon
}

new poczta_torby[MAX_PLAYERS][POCZTA_LISTOW][ep_list];

poczta_init(){
  poczta_cp_regulamin=CreateDynamicCP(2317.17,-0.12,26.0,1,0,0,-1,5);
  poczta_3dtext_regulamin=CreateDynamic3DTextLabel("Regulamin\npoczty", -1, 2317.17,-0.12,26.7,20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID,1, 0, 0, -1, 20);

  poczta_cp_nadawanie=CreateDynamicCP(2308.90,-8.56,26.0,1,0,0,-1,5);
  poczta_3dtext_nadawanie=CreateDynamic3DTextLabel("Wyslij\nlist", -1, 2308.87,-8.56,26.7,20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID,1, 0, 0, -1, 20);

  poczta_cp_odbior=CreateDynamicCP(2308.90,-2.24,26.0,1,0,0,-1,5);
  poczta_3dtext_odbior=CreateDynamic3DTextLabel("Brak listow\ndo dostarczenia", -1, 2308.87,-2.24,26.7,20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID,1, 0, 0, -1, 20);
}

poczta_wyczyscTorbe(playerid){  // lol
  // usuwamy informacje o niesionych listach i stworzone CP/MI
  if(pTemp[playerid][useRsp]) return 0;
  new cnt=0;
  for (new i=0;i<POCZTA_LISTOW;i++)
    if (poczta_torby[playerid][i][epl_dbid]>0) {
      poczta_usunListZTorby(playerid,i);
      cnt++;
    }
  return cnt;
}

poczta_usunListZTorby(playerid,i){
  if (IsValidDynamicCP(poczta_torby[playerid][i][epl_cp])) {
    DestroyDynamicCP(poczta_torby[playerid][i][epl_cp]);
    poczta_torby[playerid][i][epl_cp]=-1;
  }
  if (IsValidDynamicMapIcon(poczta_torby[playerid][i][epl_mi])) {
    DestroyDynamicMapIcon(poczta_torby[playerid][i][epl_mi]);
    poczta_torby[playerid][i][epl_mi]=-1;
  }
  poczta_torby[playerid][i][epl_dbid]=0;
}

poczta_zapelnijTorbe(playerid) {
  new i=0;
  mysql_query("SELECT p.id,exitX,exitY,exitZ,odbiorca,nadawca FROM fs_poczta p JOIN fs_houses h ON p.odbiorca=h.ownerid where p.dostarczone=0 AND h.exitX IS NOT NULL AND h.exitY IS NOT NULL AND h.exitZ IS NOT NULL ORDER BY RAND() LIMIT " #POCZTA_LISTOW);
  mysql_store_result();
  new Float:P[3],buf[128],odbiorca,nadawca;
  while (mysql_fetch_row_format(buf,"|"))
    if (!sscanf(buf,"p<|>dfffdd", poczta_torby[playerid][i][epl_dbid], P[0],P[1],P[2],odbiorca,nadawca)) {
        if(odbiorca!=GetPlayerAccountID(playerid) && nadawca!=GetPlayerAccountID(playerid)){
        // tworzymy mapikony i cp
        poczta_torby[playerid][i][epl_mi]=CreateDynamicMapIcon(P[0], P[1],P[2], 16, 1, 0,0,playerid,6000);
        Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, poczta_torby[playerid][i][epl_mi], E_STREAMER_STYLE, 3);

        poczta_torby[playerid][i][epl_cp]=CreateDynamicCP(P[0], P[1], P[2], 1, 0,0,playerid,300);

        i++;
      }
    }

  mysql_free_result();
  if (i==0) {
    ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Brak listow do dostarczenia.","OK","");
  } else {
    ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Miejsca docelowe zostaly zaznaczone na Twoim radarze za pomoca literki C.","OK","");
  }
  return 1;
}

poczta_nieprzeczytanych(playerid){
  if (!pData[playerid][loggedIn]) return 0;
  if (pData[playerid][accountID]==0) return 0;

  new wiadomosci;
  format(gstr,sizeof gstr,"SELECT count(id) FROM fs_poczta WHERE odbiorca=%d AND dostarczone=1 AND przeczytane=0;", pData[playerid][accountID]);
  mysql_query(gstr);

  mysql_store_result();
  if (mysql_fetch_row_format(gstr))
    wiadomosci=strval(gstr);
  mysql_free_result();
  return wiadomosci;

}

poczta_czytajPoczte(playerid){
  if (!pData[playerid][loggedIn] || pData[playerid][accountID]==0) {
    ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Nie odnaleziono Twojej skrzynki pocztowej, byc moze musisz sie zarejestrowac?", "OK","");
    return 0;
  }

  new lista[1024],buf[1024];
  format(buf,sizeof lista,"select p.id,p.przeczytane,pl.nick nadawca,CONCAT(SUBSTR(p.tresc,1,16),'...') tresc from fs_poczta p JOIN fs_players pl ON pl.id=p.nadawca where p.odbiorca=%d AND p.dostarczone=1 ORDER BY p.przeczytane ASC,p.ts DESC LIMIT 10;", pData[playerid][accountID]);
  mysql_query(buf);

  mysql_store_result();
  if (mysql_num_rows()==0) {
    ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Nie masz zadnych wiadomosci.","OK","");
    mysql_free_result();
    return 1;
  }
  new msg_id,msg_read,msg_from[32],msg_content[32];

  while (mysql_fetch_row_format(gstr,"|"))
    if (!sscanf(gstr,"p<|>dds[24]s[800]", msg_id, msg_read, msg_from, msg_content))
      format(lista,sizeof lista, "%s%s{000000}%d\t%s%s {b0b0b0}%s", lista, strlen(lista)>0?("\n"):(""), msg_id, msg_read>0?("{d0d0d0}"):("{ffffff}"),msg_from,msg_content);
  mysql_free_result();
  ShowPlayerDialog(playerid, DLG_POCZTA_LISTY, DIALOG_STYLE_LIST, "Twoje wiadomosci:", lista, "OK","Anuluj");
  return 1;
}

poczta_OPEDCP(playerid, checkpointid){ // OnPlayerEnterDynamicCP
  if (GetPlayerVirtualWorld(playerid)!=0 || GetPlayerInterior(playerid)!=0) return 0;
  // cp dostaw!
  for (new i=0;i<POCZTA_LISTOW;i++)
    if (poczta_torby[playerid][i][epl_dbid]>0 && poczta_torby[playerid][i][epl_cp]==checkpointid) { // dostawa listu
      new query[128];
      format(query,sizeof query,"UPDATE fs_poczta SET dostarczone=1 WHERE id=%d LIMIT 1", poczta_torby[playerid][i][epl_dbid]);
      mysql_query(query);
      poczta_usunListZTorby(playerid,i);
      Msg(playerid,COLOR_INFO2,"Dostarczyles/as list do adresata :-). Otrzymujesz 4 pkt. respektu oraz $55.");

      GivePlayerScore(playerid,4,"Dostarczenie listu");
      GivePlayerMoney(playerid, 55);
      avt_record(playerid,e_listonosz,1,ART_ADD);
      return 1;
    }

  // stale cp w poczcie/banku
  if (checkpointid==poczta_cp_regulamin) {
    ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Regulamin poczty FS", "Zasady korzystania z poczty:\n\n{ff0000}1. Nie spamujemy\n2. Nie obrazamy\n3. Nie przesylamy wulgarnych tresci\n4. List mozna wyslac tylko do gracza posiadajacego dom.\n\n{ffffff}Administracja NIE CHCE czytac waszych wiadomosci\nale jesli bedzie musiala (skargi za pow. punkty) to\nkary beda wysokie.", "Akceptuje","");
    return 1;
  } else if (checkpointid==poczta_cp_nadawanie) {

    new dbid=GetPlayerAccountID(playerid);
    if (dbid==0) {
      ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Wysylanie listow mozliwe jest tylko przez zarejestrowanych graczy.\nWpisz /rej aby sie zarejestrowac.", "OK", "");
      return 1;
    }
    //ograniczenie ilosci listow dla kazdego gracza
        if (GetTickCount()-poczta_last[playerid]<30000)
    return Msg(playerid,COLOR_ERROR,"Musza minac przynajmniej {b}30 sekund{/b} od nadania ostatniego listu.");


    //pokazujemy dialog jezeli minely conajmniej 3 minuty od nadania ostatniego listu przez danego gracza :-)
    ShowPlayerDialog(playerid, DLG_POCZTA_ODBIORCA, DIALOG_STYLE_INPUT, "Adresat listu:", "Podaj nick odbiorcy, lub jego fragment:", "OK","Anuluj");
    return 1;
  } else if (checkpointid==poczta_cp_odbior) {

    poczta_wyczyscTorbe(playerid);
    poczta_zapelnijTorbe(playerid);
    return 1;
  }
  return 0;
}

poczta_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
  #pragma unused listitem

  switch(dialogid) {
    case DLG_POCZTA_INFO: {
      return 1;
    }
    case DLG_POCZTA_ODBIORCA: {
      if (!response) return 1;

      if (strlen(inputtext)>32) {
        Msg(playerid,COLOR_ERROR,"Wprowadzony nick jest zbyt dlugi.");
        ShowPlayerDialog(playerid, DLG_POCZTA_ODBIORCA, DIALOG_STYLE_INPUT, "Adresat listu:", "Podaj nick odbiorcy, lub jego fragment:", "OK","Anuluj");
        return 1;
      } else if (strlen(inputtext)<3) {
        Msg(playerid,COLOR_ERROR,"Wprowadzony nick jest zbyt krotki.");
        ShowPlayerDialog(playerid, DLG_POCZTA_ODBIORCA, DIALOG_STYLE_INPUT, "Adresat listu:", "Podaj nick odbiorcy, lub jego fragment:", "OK","Anuluj");
        return 1;

      }

      new odbiorca_escaped[50];
      mysql_real_escape_string(inputtext, odbiorca_escaped);
      // szukamy pasujacych nickow
      new query[512],buf[255],lista[1024],odbiorca_id,odbiorca_respekt,odbiorca_nick[32],odbiorca_dnitemu;
      // tylkko gracze z domami
      format(query,sizeof query, "SELECT p.id,p.respect,p.nick,datediff(now(),p.datetime_last) dnitemu FROM fs_players p JOIN fs_houses h ON h.ownerid=p.id WHERE (p.nick like '%%%s%%' or p.nick sounds like '%s') ORDER BY dnitemu ASC,p.respect DESC limit 6;",odbiorca_escaped,odbiorca_escaped);
      mysql_query(query);
      mysql_store_result();
      if (mysql_num_rows()==0) {
        ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Nie odnaleziono zadnego pasujacego odbiorcy - moze nie posiada domu?. Sprobuj ponownie.", "OK","");
        mysql_free_result();
        return 1;
      }
      while (mysql_fetch_row_format(buf,"|"))
        if (!sscanf(buf,"p<|>dds[24]d", odbiorca_id, odbiorca_respekt, odbiorca_nick, odbiorca_dnitemu))
          format(lista,sizeof lista, "%s%s{000000}%d\t%s%s (respekt: %d)", lista, strlen(lista)>0?("\n"):(""), odbiorca_id, odbiorca_dnitemu>0?("{d0d0d0}"):("{ffffff}"),odbiorca_nick,odbiorca_respekt);

      mysql_free_result();
      ShowPlayerDialog(playerid, DLG_POCZTA_ODBIORCA_LIST, DIALOG_STYLE_LIST, "Wybierz odbiorce z listy:", lista, "OK","Anuluj");

      return 1;
    }
    case DLG_POCZTA_ODBIORCA_LIST: {
      if (!response) return 1;

      if (sscanf(inputtext,"d",poczta_odbiorcy[playerid])) {
        SendClientMessage(playerid,-1,"[POCZTA] Nie wskazano wlasciwego odbiorcy");
        return 1;
      }
      ShowPlayerDialog(playerid,DLG_POCZTA_TRESC, DIALOG_STYLE_INPUT, "Tresc listu", "Wprowadz tresc wiadomosci:", "OK", "Anuluj");
      return 1;
    }
    case DLG_POCZTA_TRESC: {
      if (!response) return 1;
      if (strlen(inputtext)<5) {
        Msg(playerid,COLOR_ERROR,"Wprowadzony tekst jest zbyt krotki.");
        ShowPlayerDialog(playerid,DLG_POCZTA_TRESC, DIALOG_STYLE_INPUT, "Tresc listu", "Wprowadz tresc wiadomosci:", "OK", "Anuluj");
        return 1;
      }
      if (strlen(inputtext)>800) {
        Msg(playerid,COLOR_ERROR,"Wprowadzony tekst jest za dlugi.");
        ShowPlayerDialog(playerid,DLG_POCZTA_TRESC, DIALOG_STYLE_INPUT, "Tresc listu", "Wprowadz tresc wiadomosci:", "OK", "Anuluj");
        return 1;
      }
      //zabezpieczenie przed pustymi wiadomosciami
      if (inputtext[0]==' ') {
        Msg(playerid,COLOR_ERROR,"Wprowadzony tekst jest zbyt krotki.");
        ShowPlayerDialog(playerid,DLG_POCZTA_TRESC, DIALOG_STYLE_INPUT, "Tresc listu", "Wprowadz tresc wiadomosci:", "OK", "Anuluj");
        return 1;
      }
      // nie powinno sie zdarzyc, ale sprawdzimy dla pewnosci
      new dbid=GetPlayerAccountID(playerid);
      if (dbid==0) {
        ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Wysylanie listow mozliwe jest tylko przez zarejestrowanych graczy.\nWpisz /rej aby sie zarejestrowac.", "OK", "");
        return 1;
      }

      if(dbid==poczta_odbiorcy[playerid])
      {
          ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Wysylanie listow do samego siebie jest idiotyczne :-)", "OK", "");
        return 1;
      }

      if (GetPlayerMoney(playerid)<250) {
        ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Koszt wyslania jednego listu to $250. Nie posiadasz takiej ilosci pieniedzy przy sobie", "OK", "");
        return 1;
      }

      // filtrujemy tresc
      for(new i=0; i<strlen(inputtext); i++) {
        if(inputtext[i] == '{') inputtext[i] = '[';
        if(inputtext[i] == '%') inputtext[i] = '#';
        if(inputtext[i] == '|') inputtext[i] = '!';
      }

      unpliterki(inputtext);

      new tresc_escaped[1024];
      mysql_real_escape_string(inputtext,tresc_escaped);
      new query[2048];
      format(query,sizeof query,"INSERT INTO fs_poczta SET nadawca=%d,odbiorca=%d,tresc='%s',ts=NOW();",dbid,poczta_odbiorcy[playerid],tresc_escaped);
      mysql_query(query);
      ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Wiadomosc zostala nadana, wkrotce trafi do odbiorcy. \nKoszt nadania listu: $250 - kwota zostala pobrana z Twojego konta", "OK", "");
      poczta_last[playerid]=GetTickCount();
      poczta_spool();
      GivePlayerMoney(playerid,-250);
      return 1;
    }

    case DLG_POCZTA_LIST: {
      if (!response) {
        // usuwamy wiadomosc
        format(gstr,sizeof gstr, "DELETE FROM fs_poczta WHERE odbiorca=%d AND id=%d", pData[playerid][accountID], pTemp[playerid][tmpDialogValue]);
        mysql_query(gstr);
      } else {
        // oznaczamy wiadomosc jako przeczytana
        format(gstr,sizeof gstr,"UPDATE fs_poczta SET przeczytane=1 WHERE odbiorca=%d AND id=%d", pData[playerid][accountID], pTemp[playerid][tmpDialogValue]);
        mysql_query(gstr);
      }
      poczta_czytajPoczte(playerid);
      return 1;
    }
    case DLG_POCZTA_LISTY: {
      if (!response) return 1;

      // odszukujemy id wiadomosci
      new msgid;
      if (sscanf(inputtext,"d",msgid)) {
        poczta_czytajPoczte(playerid);
        return 1;
      }

      // pokazujemy wiadomosc na ekranie
      new buf[1024];
      format(buf,sizeof buf,"SELECT p.id,p.ts,CONCAT(p.tresc,' ') tresc,pl.nick nadawca FROM fs_poczta p JOIN fs_players pl ON pl.id=p.nadawca  WHERE p.id=%d AND p.dostarczone=1;", msgid);
      mysql_query(buf);
      mysql_store_result();

      if (!mysql_fetch_row_format(buf, "|")) {
        ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Nie odnaleziono pozadanej wiadomosci.","OK","");
        mysql_free_result();
        return 1;
      }
      new msg_id,msg_ts[20],msg_tresc[800],msg_nadawca[32];
      printf("%s",buf);
      if (sscanf(buf,"p<|>ds[128]s[800]s[24]",msg_id,msg_ts,msg_tresc,msg_nadawca)) {
        ShowPlayerDialog(playerid,DLG_POCZTA_INFO,DIALOG_STYLE_MSGBOX,"Informacja", "Nie udalo sie wczytac tej wiadomosci.","OK",""); // nie powinno sie wydarzyc
        mysql_free_result();
        return 1;
      }
      mysql_free_result();

      format(buf,sizeof buf, "Od:\t%s\nData:\t%s\n\n{ffffff}%s", msg_nadawca,msg_ts,msg_tresc);
      format(gstr,sizeof gstr,"Wiadomosc nr %d", msg_id);
      pTemp[playerid][tmpDialogValue]=msg_id; // do usuwania
      ShowPlayerDialog(playerid,DLG_POCZTA_LIST,DIALOG_STYLE_MSGBOX,gstr, buf,"Wroc","Usun");

      return 1;
    }
  }
  return 0;
}

Timer:poczta_spool[1800000](){  // pol godziny
  // dostarczamy automatycznie wiadomosci, ktore nie zostaly dostarczone recznie przez ostatnie 24h
  mysql_query("UPDATE fs_poczta SET dostarczone=1 WHERE DATEDIFF(now(),ts)>1;");
  // sprawdzamy czy sa jakies wiadomosci do dostarczenia
  mysql_query("SELECT count(id) FROM fs_poczta WHERE dostarczone=0;");
  new buf[127],wiadomosci;
  mysql_store_result();
  if (mysql_fetch_row_format(buf))
    wiadomosci = strval(buf);
  mysql_free_result();

  if (wiadomosci<1)
    UpdateDynamic3DTextLabelText(poczta_3dtext_odbior, 0xffffffFF, "Brak listow\ndo dostarczenia");
  else if (wiadomosci<9)
    UpdateDynamic3DTextLabelText(poczta_3dtext_odbior, 0xd0d0ffFF, "Kilka listow\ndo dostarczenia");
  else
    UpdateDynamic3DTextLabelText(poczta_3dtext_odbior, 0xb0b0ffFF, "Duzo listow\ndo dostarczenia");
  return 1;
}

/* wysyla list do gracza z uid FULLSERVER_BOT_UID o podanej tresci */
poczta_powiadomGracza(uid,tresc[]) {
  new tresc_escaped[1024];
  mysql_real_escape_string(tresc,tresc_escaped);
  new query[2048];
  format(query,sizeof query,"INSERT INTO fs_poczta SET nadawca=%d,odbiorca=%d,tresc='%s',ts=NOW();",FULLSERVER_BOT_UID,uid,tresc_escaped);
  mysql_query(query);
  return 1;
}
// EOF
