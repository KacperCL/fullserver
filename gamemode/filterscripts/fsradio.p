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

#include <a_samp>
#define FILTERSCRIPT
#include <3rdparty/dc_cmd>
#include <3rdparty/audio>

#undef MAX_PLAYERS
#define MAX_PLAYERS 200

#define DIALOG_RADIO_OFFSET 2110
#define DIALOG_RADIO_MAIN (DIALOG_RADIO_OFFSET+0)
#define DIALOG_RADIO_OPCJE  (DIALOG_RADIO_OFFSET+1)
#define DIALOG_RADIO_RADIA  (DIALOG_RADIO_OFFSET+2)
#define DIALOG_RADIO_INFO (DIALOG_RADIO_OFFSET+3)
#define DIALOG_RADIO_ABOUT  (DIALOG_RADIO_OFFSET+4)

#define DIALOG_RADIO_INFO_TXT "Aby sluchac radia, musisz miec zainstalowany plugin audio.\r\nPobierz go z adresu:\r\n\r\n{ff6666}http://fullserver.eu/audio/"
#define DIALOG_RADIO_ABOUT_TXT  "{ffffff}Uzyj komendy {6666ff}/cojestgrane{ffffff} aby\r\nsprawdzic jak nazywa sie aktualnie\r\nodtwarzany utwor.\r\n\r\nJesli nie slyszysz zadnego radia,\r\nwpisz {6666ff}/mono{ffffff}, badz wlacz ten tryb\r\nw opcjach.\r\n\r\nSugestie co do nowych kanalow\r\nskladaj na forum {66ff66}www.fullserver.eu\r\n\r\n\r\n{ff6666}Radio FS v.4.14\r\n(c) 2011-2014 XJL, eider dla FullServera"


new graczeKanaly[MAX_PLAYERS];  // handleid zwracany przez audioplugin
new graczeRadia[MAX_PLAYERS]={-1,...};  // nr indeksu wybranego radia
new bool:graczeDownmix[MAX_PLAYERS]={false, ...}; // czy downmiksujemy dzwiek
new graczeVolume[MAX_PLAYERS]={100,...};
new bool:graczeAnn[MAX_PLAYERS]={true, ...};
new coJestGrane[MAX_PLAYERS][127];

new radia[][]={
    "Radio Party","http://radioparty.pl/play/glowny_24aac.m3u",
    "RMF FM","http://www.miastomuzyki.pl/n/rmffm.pls",
    "RMF Maxx","http://www.miastomuzyki.pl/n/rmfmaxxx.pls",
    "Radio ZET","http://91.121.179.221:8050/listen.pls",
    "ESKA Warszawa","http://poznan6.radio.pionier.net.pl:8000/eska-warszawa.mp3",
    "ESKA Lublin","http://gramy01.eska.fm:8000/eska_lublin.mp3.m3u",
    "DI.FM LiquidDNB","http://listen.di.fm/public2/liquiddnb.pls",
    "DJ TOP-50","http://polskastacja.pl/play/aac_djtop50.pls",
    "Planeta FM","http://www.planeta.fm/content/stream/planeta_krakow.pls",

    "PolskaStacja disco polo","http://www.polskastacja.pl/play/aac_discopolo.pls",
    "PolskaStacja DNB","http://www.polskastacja.pl/play/aac_drumbass.pls",
    "PolskaStacja polski HipHop","http://www.polskastacja.pl/play/aac_polskihh.pls",
    "PolskaStacja polskie przeboje","http://www.polskastacja.pl/play/aac_tpp.pls",
    "PolskaStacja przeboje na lato","http://178.33.55.20:80/",
    "PolskaStacja - HOT 100 - Goraca Setka Nowosci","http://www.polskastacja.pl/play/aac_hot100.pls",
    "PolskaStacja -Tylko ROCK","http://www.polskastacja.pl/play/aac_rock.pls",

    "Polskie Radio - Jedynka","http://www.polskieradio.pl/st/program1M.asx",
    "Polskie Radio - Dwójka","http://www.polskieradio.pl/st/program2.asx",
    "Polskie Radio - Trójka","http://www.polskieradio.pl/st/program3M.asx",

    "Antyradio Katowice","http://94.23.88.162:9200/listen.pls",
    "Top HitZ","http://listen.slotex.pl/7596.pls",

    "Black Metal Domain","http://panel9.serverhostingcenter.com/tunein.php/abarnard/playlist.pls",
    "DEATH.FM","http://community.loudcity.com/stations/death-fm/files/show/MP3-hi.pls",
    "Violent Forces Radio","http://69.175.13.130:8080/"

};
new txt_radia[1024];

public OnFilterScriptInit(){
  for(new i = 0; i < ((sizeof radia)/2); i++) {
    if (i>0)
      format(txt_radia,sizeof txt_radia, "%s\r\n%s", txt_radia, radia[i*2]);
    else
      format(txt_radia,sizeof txt_radia, "%s", radia[i*2]);
  }

  return 1;
}

CMD:radio(playerid){
  if (!Audio_IsClientConnected(playerid)) {
    return pokazInfoPlugin(playerid);
  }
  new str[1024];
  if (graczeKanaly[playerid]>0)
    str="Zmien radio\r\n{60ff60}Opcje\r\n{ff6060}Wylacz radio\r\n{6666ff}Informacje";
  else
    str="Wybierz radio\r\n{6666ff}Informacje";

  if (strlen(coJestGrane[playerid])>1)
    format(str,sizeof str,"%s\r\n{e6cc61}Co jest grane?",str);

  return ShowPlayerDialog(playerid,DIALOG_RADIO_MAIN,DIALOG_STYLE_LIST,"Odtwarzanie radia:",str,"OK","Anuluj");
}

CMD:mono(playerid){
  if (!Audio_IsClientConnected(playerid)) {
    return pokazInfoPlugin(playerid);
  }
  return zmienStereoMono(playerid,1);
}

CMD:cojestgrane(playerid){
  if (!Audio_IsClientConnected(playerid))
    return pokazInfoPlugin(playerid);
  else if (graczeRadia[playerid]==-1)
    return SendClientMessage(playerid,0xff6666ff,"Obecnie nie jest odtwarzane zadne radio");
  else if (strlen(coJestGrane[playerid])<=1)
    return SendClientMessage(playerid,0xff6666ff,"Brak informacji o aktualnie odtwarzanym utworze");

  new str[255];
  format(str,sizeof str,"[RADIO] {ffffff}Odtwarzanie: {66ff66}%s",coJestGrane[playerid]);
  SendClientMessage(playerid,0xff3333ff,str);

  if (!graczeAnn[playerid]) return 1;

  format(str,sizeof str,"~n~~n~~n~~n~~n~~n~%s",coJestGrane[playerid]);
  GameTextForPlayer(playerid,str,3000,5);

  return 1;
}

pokazWyborKanalu(playerid){
  return ShowPlayerDialog(playerid,DIALOG_RADIO_RADIA,DIALOG_STYLE_LIST,"Wybierz kanal:",txt_radia,"OK","Anuluj");
}

pokazInfoPlugin(playerid){
  return ShowPlayerDialog(playerid,DIALOG_RADIO_INFO,DIALOG_STYLE_MSGBOX,"Informacja",DIALOG_RADIO_INFO_TXT,"OK","");
}
pokazAbout(playerid){
  return ShowPlayerDialog(playerid,DIALOG_RADIO_ABOUT,DIALOG_STYLE_MSGBOX,"Informacje",DIALOG_RADIO_ABOUT_TXT,"OK","");
}

zmienGlosnosc(playerid){
  if (graczeVolume[playerid]==100)
    graczeVolume[playerid]=75;
  else if (graczeVolume[playerid]==75)
    graczeVolume[playerid]=50;
  else if (graczeVolume[playerid]==50)
    graczeVolume[playerid]=25;
  else if (graczeVolume[playerid]==25)
    graczeVolume[playerid]=100;

  Audio_SetVolume(playerid,graczeKanaly[playerid],graczeVolume[playerid]);
  new str[64];
  format(str,sizeof str,"[RADIO] {ffffff}Glosnosc zmieniona na {66ff66}%d%%",graczeVolume[playerid]);
  return SendClientMessage(playerid,0xff3333ff,str);
}

zmienStereoMono(playerid,tryb=0){
  if (tryb==0)
    graczeDownmix[playerid]=!graczeDownmix[playerid];
  else if (tryb==1) // wymuszamy mono
    graczeDownmix[playerid]=true;

  puscRadio(playerid);
  return 1;
}

zmienAnn(playerid){
  graczeAnn[playerid]=!graczeAnn[playerid];
  return 1;
}

pokazOpcje(playerid){
  new str[1023];
  format(str,sizeof str,"Glosnosc: {90ff90}%d%%{ffffff}\r\n",graczeVolume[playerid]);

  if (graczeDownmix[playerid])
    format(str,sizeof str, "%s{ffffff}Przelacz na STEREO (obecnie {90ff90}MONO{ffffff})", str);
  else
    format(str,sizeof str, "%s{ffffff}Przelacz na MONO (obecnie: {90ff90}STEREO{ffffff})", str);

  if (graczeAnn[playerid])
    format(str,sizeof str, "%s\r\n{ffffff}Tytuly utworow {90ff90}pokazywane{ffffff} na ekranie", str);
  else
    format(str,sizeof str, "%s\r\n{ffffff}Tytuly utworow {90ff90}nie pokazywane{ffffff} na ekranie", str);

  format(str,sizeof str, "%s\r\n{ff3030}Zrestartuj plugin audio.", str);

  return ShowPlayerDialog(playerid,DIALOG_RADIO_OPCJE,DIALOG_STYLE_LIST,"Opcje odtwarzania:",str,"OK","Anuluj");
}

zresetujOpcje(playerid){
  graczeKanaly[playerid]=0;
  graczeRadia[playerid]=-1;
  graczeDownmix[playerid]=false;
  graczeVolume[playerid]=100;
  graczeAnn[playerid]=true;
  coJestGrane[playerid]="";
  return 1;
}

puscRadio(playerid,radio=-1){
  if (radio==-1) radio=graczeRadia[playerid];
  if (graczeRadia[playerid]!=radio) // zmiana radia - zerujemy info o aktualnie odtwarzanym kawalku
    coJestGrane[playerid]="";

  if (graczeKanaly[playerid]>0)
    Audio_Stop(playerid,graczeKanaly[playerid]);

  graczeRadia[playerid]=radio;
  graczeKanaly[playerid]=Audio_PlayStreamed(playerid,radia[(radio*2)+1],false,true,graczeDownmix[playerid]);
  Audio_SetVolume(playerid,graczeKanaly[playerid],graczeVolume[playerid]);

  new str[128];
  format(str,sizeof str,"[RADIO] {ffffff}Odtwarzana stacja: {606060}%s{ffffff}, tryb {60ff60}%s{ffffff}.",radia[radio*2],((graczeDownmix[playerid])?("mono"):("stereo")));
  SendClientMessage(playerid,0xff3333ff,str);
}

public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[]){
  switch(dialogid){
    case DIALOG_RADIO_ABOUT:
      return _radio(playerid);
    case DIALOG_RADIO_INFO:
      return 1;
    case DIALOG_RADIO_MAIN: {
      if (!response) return 1;
      switch(listitem){
        case 0: // wybor kanalu
          return pokazWyborKanalu(playerid);
        case 1: { // opcje lub informacje
          if (graczeKanaly[playerid]==0)
            return pokazAbout(playerid);
          return pokazOpcje(playerid);
        }
        case 2: { // wylacz radio
          Audio_Stop(playerid,graczeKanaly[playerid]);
          graczeKanaly[playerid]=0;
          graczeRadia[playerid]=-1;
          coJestGrane[playerid]="";
          SendClientMessage(playerid,0xff3333ff,"[RADIO] {ffffff}Radio wylaczone");
          return 1;
        }
        case 3:
          return pokazAbout(playerid);
        case 4:
          return _cojestgrane(playerid);
      }
      return 1;
    }
    case DIALOG_RADIO_OPCJE: {
      if (!response) return 1;
      switch(listitem){
        case 0:   // zmiana glosnosnosci
          zmienGlosnosc(playerid);
        case 1:   // zmiana stereo,mono
          zmienStereoMono(playerid);
        case 2:
          zmienAnn(playerid);
        case 3: { // restart pluginu audio

          Audio_Stop(playerid,graczeKanaly[playerid]);
          graczeKanaly[playerid]=0;
          graczeRadia[playerid]=-1;
          coJestGrane[playerid]="";
          SendClientMessage(playerid,0xff3333ff,"[RADIO] {ffffff}Radio wylaczone");

          Audio_TransferPack(playerid);
          zresetujOpcje(playerid);


          return _radio(playerid);
        }
      }
      return pokazOpcje(playerid);
    }
    case DIALOG_RADIO_RADIA: {
      if (!response) return 1;
      if (!Audio_IsClientConnected(playerid))
        return pokazInfoPlugin(playerid);
      puscRadio(playerid,listitem);
      return 1;
    }
  }
  return 0;
}

public OnPlayerConnect(playerid){
  zresetujOpcje(playerid);
  return 1;
}

public OnFilterScriptExit(){
  for (new i=0; i<sizeof graczeKanaly; i++)
    if (graczeKanaly[i]>0) {
      SendClientMessage(i,0xff3333ff,"[RADIO] {ffffff}Radia internetowe {ff0000}wylaczone");
      Audio_Stop(i,graczeKanaly[i]);
    }
  return 1;
}

public Audio_OnTrackChange(playerid, handleid, track[]){
  if (handleid!=graczeKanaly[playerid]) return 0;
  new str[255];
  Clarify(track);
  format(coJestGrane[playerid],127,"%s",track);
  format(str,sizeof str,"[RADIO] {ffffff}Odtwarzanie: {66ff66}%s",track);
  SendClientMessage(playerid,0xff3333ff,str);

  if (!graczeAnn[playerid]) return 1;

  format(str,sizeof str,"~n~~n~~n~~n~~n~~n~%s",track);
  GameTextForPlayer(playerid,str,3000,5);

  return 1;
}

Clarify(string[])
{
  new len = strlen(string);
  if (string[0]==0) return;
  for (new i=0;i<len;i++)
    switch(string[i]){
      case 32,38,45,48..58,63,65..90,97..122:{
      }
      default:
        string[i]=32; // spacja
    }
}

forward WylaczRadio(playerid);
public WylaczRadio(playerid){
  Audio_Stop(playerid,graczeKanaly[playerid]);
  graczeKanaly[playerid]=0;
  graczeRadia[playerid]=-1;
  coJestGrane[playerid]="";
  return;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
  if(success)
  {
    CallRemoteFunction("PlayerCommandPerformedOutside", "isi", playerid, cmdtext, success);
  }
}
// EOF