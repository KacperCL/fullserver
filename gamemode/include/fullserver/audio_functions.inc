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

#define AUDIOID_KICK  2
#define AUDIOID_BAN   3
#define AUDIOID_PREPARE 4
#define AUDIOID_WARN_MALE 5
#define AUDIOID_WARN_FEMALE 6

#define AUDIOID_INTRO 20
#define AUDIOID_ACHIEVEMENT 30

#define AUDIOID_FXSUBMARINE 42
#define AUDIOID_FXKOSCIOL 43
#define AUDIOID_FXTUNEL1 44
#define AUDIOID_FXTUNEL2 45
#define AUDIOID_FXAFGANISTAN  46
#define AUDIOID_FXWIETNAM   47
#define AUDIOID_FXHACJENDA    48
#define AUDIOID_FXYAKUZA    49

#define AUDIOID_FXFOTORADAR   50
#define AUDIOID_FXDOMPUKANIE  51

#define AUDIOID_WALIZKA 60
#define AUDIOID_PREZENT 61
#define AUDIOID_PODKOWA 62
#define AUDIOID_TORBA   63

#define AUDIOID_BEEP1 70
#define AUDIOID_BEEP2 71
#define AUDIOID_BEEP3 72
#define AUDIOID_BEEP4 73

#define AUDIOID_DOUBLE 300
#define AUDIOID_TRIPLE 301
#define AUDIOID_EXCELLENT 302
#define AUDIOID_MULTTI 303
#define AUDIOID_SPREEE 304
#define AUDIOID_ULTRA 305
#define AUDIOID_MONSTER 306
#define AUDIOID_TERMINATION 307
#define AUDIOID_DOMINATING 308
#define AUDIOID_HEAD 309

static audio_radiaAirtraffic[][]={
  "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1115871",
  "http://mtl2.liveatc.net/epwa.m3u"
};

static audio_radiaPolice[][]={
  "http://yp.shoutcast.com/sbin/tunein-station.pls?id=594724"
};

static audio_radiaCB[][]={
  "http://s5.myradiostream.com/66484.pls"
};

new backgroundSounds[MAX_PLAYERS];

Audio_OdtworzPowitanie(playerid){
  Audio_PlayStreamed(playerid, "http://fscdn.pl/fullserver_eu/fs.mp3");
  return 1;
}

Audio_PlayBGSound(playerid,audioid,bool:pause=false,bool:loop=true,bool:downmix=true){
  if (!Audio_IsClientConnected(playerid)) return 0;
  if (pData[playerid][spectating] != INVALID_PLAYER_ID) return 0;
  if(!pTemp[playerid][playPlayall]) return 0;

  if (backgroundSounds[playerid]) Audio_Stop(playerid,backgroundSounds[playerid]);
  backgroundSounds[playerid]=Audio_Play(playerid,audioid,pause,loop,downmix);
  return backgroundSounds[playerid];
}

stock Audio_PlayBGStream(playerid,streamurl[],bool:pause=false,bool:loop=true,bool:downmix=true){
  if (!Audio_IsClientConnected(playerid)) return 0;
  if (pData[playerid][spectating] != INVALID_PLAYER_ID) return 0;
  if(!pTemp[playerid][playPlayall]) return 0;

  if (backgroundSounds[playerid]) Audio_Stop(playerid,backgroundSounds[playerid]);
  backgroundSounds[playerid]=Audio_PlayStreamed(playerid,streamurl,pause,loop,downmix);
  return backgroundSounds[playerid];
}

Audio_StopBGSound(playerid){
  if (!Audio_IsClientConnected(playerid)) return 0;
  if (backgroundSounds[playerid]) Audio_Stop(playerid,backgroundSounds[playerid]);
  return 1;
}

public Audio_OnClientConnect(playerid) {
    Audio_TransferPack(playerid);
  backgroundSounds[playerid]=0;
}

forward Audio_RetransferPack(playerid);
public Audio_RetransferPack(playerid){
  Audio_TransferPack(playerid);
  return 1;
}

public Audio_OnClientDisconnect(playerid){
  backgroundSounds[playerid]=0;
  if (!IsPlayerConnected(playerid)) return 0;
  return Msg(playerid,COLOR_ERROR, "Polaczenie z pluginem audio {b}utracone{/b}.");
}

public Audio_OnTransferFile(playerid,file[],current,total,result) {
    if (current==1) {
    Msg(playerid, COLOR_INFO, "Rozpoczeto synchronizacje plikow audio", false, true);
  }
    if (total==current) {
    Msg(playerid, COLOR_INFO2, "Synchronizacja plikow audio zakonczona", true, true);
    DisablePlayerSounds(playerid);  // standradowe dzwieki gta, byc moze leci jeszcze melodyjka ze startu
    Audio_Play(playerid,AUDIOID_INTRO, false, false, true);
  }
    return 1;
}

Audio_OnPlayerStateChange(playerid,newstate,oldstate){
  if (!Audio_IsClientConnected(playerid)) return;
  if (oldstate==2 && pTemp[playerid][audio_vehicle]>-1) {
    Audio_Stop(playerid, pTemp[playerid][audio_vehicle]);
    pTemp[playerid][audio_vehicle]=-1;
  } else if (newstate==2) {
      if(!pTemp[playerid][playPlayall]) return;
    new vm=GetVehicleModel(GetPlayerVehicleID(playerid));
    switch(vm){
      case 520,519,553,577,592,593,460,476,511,512,513: {
        new r=random(sizeof audio_radiaAirtraffic);
        pTemp[playerid][audio_vehicle]=Audio_PlayStreamed(playerid, audio_radiaAirtraffic[r], false, true, true);
      }
      case 596,597,598,523,497: {
        new r=random(sizeof audio_radiaPolice);
        pTemp[playerid][audio_vehicle]=Audio_PlayStreamed(playerid, audio_radiaPolice[r], false, true, true);
      }
      case 403,514,515: {
        new r=random(sizeof audio_radiaCB);
        pTemp[playerid][audio_vehicle]=Audio_PlayStreamed(playerid, audio_radiaCB[r], false, true, true);
      }
    }

  }
  return;
}

stock AC_Audio_PlayStreamed(playerid, const url[], bool:pause = false, bool:loop = false, bool:downmix = false)
{
  if(!IsPlayerConnected(playerid)) return -1;
  if(!pTemp[playerid][playPlayall]) return -1;
  return Audio_PlayStreamed(playerid,url,pause,loop,downmix);
}

#if defined _ALS_Audio_PlayStreamed
    #undef Audio_PlayStreamed
#else
    #define _ALS_Audio_PlayStreamed
#endif
#define Audio_PlayStreamed AC_Audio_PlayStreamed

stock AC_Audio_Play(playerid, audioid, bool:pause = false, bool:loop = false, bool:downmix = false)
{
  if(!IsPlayerConnected(playerid)) return -1;
  if(!pTemp[playerid][playPlayall]) return -1;
  return Audio_Play(playerid,audioid,pause,loop,downmix);
}

#if defined _ALS_Audio_Play
    #undef Audio_Play
#else
    #define _ALS_Audio_Play
#endif
#define Audio_Play AC_Audio_Play

// EOF
