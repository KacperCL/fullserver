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

// some includes
#include <a_samp>
#define FILTERSCRIPT // very important for dc_cmd!
#include <3rdparty/dc_cmd>

// some defines which aren't in a_samp but should be
#define INVALID_TIMER_ID 0xFFFF

// hook for KillTimer because original function doesn't reset provided sourceId (our timer id). why kye!? oh why!?
stock __UGLY__KillTimer(&timerid){
  if(timerid>=INVALID_TIMER_ID) return; // why the hell we need to kill invalid timer!?
  KillTimer(timerid); // yeah, now call actual native!
  timerid=INVALID_TIMER_ID; // reset it now, that's why we created this function.
}

#if defined _ALS_KillTimer
    #undef KillTimer
#else
    #define _ALS_KillTimer
#endif
#define KillTimer __UGLY__KillTimer

// our hidden functions! love <3 kye for these!
forward OnClientCheckResponse(playerid,actionId,memAddr,retnData); // OnClientCheckResponse ( PLAYERID playerId, BYTE actionId, int memAddr, BYTE retnData )
native SendClientCheck(playerid,actionId,memAddr,memOffset,bytesCount); // SendClientCheck ( PLAYERID playerId, BYTE actionId, int memAddr, short memOffset, short bytesCount )

// forward our public function
forward KickPlayer(playerid);

// and now some variables for our script, yey!
enum e_pData {
  bool:isBad,
  bool:isChecked,
  bool:haveCleo,
  bool:haveVorbis,
  bool:aacCheck,
  timerId // should be INVALID_TIMER_ID but actually we're using KillTimer in OnFilterScriptInit so it's ok
}
new pData[MAX_PLAYERS][e_pData];

// oh, oh! send checks to these fuckin' cheaters!
sendClientChecks(playerid)
{
  if(!IsPlayerConnected(playerid)) return;
  
  pData[playerid][isBad]=false; // reset variables
  pData[playerid][isChecked]=false;
  pData[playerid][haveCleo]=false;
  pData[playerid][haveVorbis]=false;
  pData[playerid][aacCheck]=true;

  /* vorbisHooked, ASI Loader, Audio Plugin etc. */
  SendClientCheck(playerid, 0x05, 0x00469FEE, 0x0000, 0x00FF);

  /* s0beit and other shit */
  SendClientCheck(playerid, 0x05, 0x005207E3, 0x0000, 0x0004);
  SendClientCheck(playerid, 0x05, 0x006D5259, 0x0000, 0x0004);
  SendClientCheck(playerid, 0x05, 0x005E8606, 0x0000, 0x0004);
  SendClientCheck(playerid, 0x05, 0x007F0BF7, 0x0000, 0x0004); // crack
  SendClientCheck(playerid, 0x05, 0x00543081, 0x0000, 0x0004);
  SendClientCheck(playerid, 0x05, 0x006D6259, 0x0000, 0x0004); // 25ghz, thx to ubi

  /* CLEO */
  SendClientCheck(playerid, 0x05, 0x0057B9FD, 0x0000, 0x0004);
  SendClientCheck(playerid, 0x05, 0x004667DB, 0x0000, 0x0004);
  
  /* AAC by FYP, don't know why, but it works */
  SendClientCheck(playerid, 0x02, 0x00000000, 0x0000, 0x0004);
  
  /* ticks */
  SendClientCheck(playerid, 0x48, 0x00000000, 0x0000, 0x0004);
  
  /* debugging */
  SendClientCheck(playerid, 0x70, 0x00469FEE, 0x0000, 0x00FF); // removed with 0x05!?
  SendClientCheck(playerid, 0x46, 0x00469FEE, 0x0000, 0x00FF);
  SendClientCheck(playerid, 0x47, 0x00469FEE, 0x0000, 0x00FF);
}

// reset timer (just for sure?) and send checks to all.
public OnFilterScriptInit()
{
  for(new playerid; playerid<MAX_PLAYERS; playerid++){
    KillTimer(pData[playerid][timerId]);
    sendClientChecks(playerid);
  }
}

// reset timers, because they will crash server if not! AGAIN! WHY KYE!?!?!?
public OnFilterScriptExit()
{
  for(new playerid; playerid<MAX_PLAYERS; playerid++){
    KillTimer(pData[playerid][timerId]);
  }
}

// player connects
public OnPlayerConnect(playerid)
{
  sendClientChecks(playerid); // send

  // set timer, 5 seconds should be OK
  pData[playerid][timerId] = SetTimerEx("KickPlayer",5000,false,"d",playerid);
  return 1;
}

// player disconnects so kill timer for his playerId;
public OnPlayerDisconnect(playerid, reason)
{
  KillTimer(pData[playerid][timerId]);
}

// oh! our response is ready!
public OnClientCheckResponse(playerid,actionId,memAddr,retnData)
{
  /*
  OnClientCheckResponse: 0 5 469FEE 8A
  OnClientCheckResponse: 0 5 5207E3 A
  OnClientCheckResponse: 0 5 6D5259 88
  OnClientCheckResponse: 0 5 5E8606 A0
  OnClientCheckResponse: 0 5 7F0BF7 80
  OnClientCheckResponse: 0 5 543081 A
  OnClientCheckResponse: 0 5 6D6259 A0
  OnClientCheckResponse: 0 5 57B9FD 20
  OnClientCheckResponse: 0 5 4667DB 80

  -- other --
  7F0BF7 82 -> crack
  469FEE [*] -> vorbisHooked shit (?)
  */
  
  new pName[MAX_PLAYER_NAME+1];
  GetPlayerName(playerid, pName, sizeof(pName));
  printf("OnClientCheckResponse: %d (%s) = %x %x %x",playerid,pName,actionId,memAddr,retnData);
  
  if(pData[playerid][isChecked] && (pData[playerid][isBad] || pData[playerid][haveCleo])) return 0; // we don't want to spam player with these messages, so stop it if player is already marked as cheater!

  /* vorbisHooked, ASI Loader, Audio Plugin etc. */
  if(actionId==0x5 && memAddr==0x469FEE && retnData!=0x8A) pData[playerid][haveVorbis]=true;

  /* s0beit and other shit */
  if(actionId==0x5 && memAddr==0x5207E3 && retnData!=0xA) pData[playerid][isBad]=true;
  if(actionId==0x5 && memAddr==0x6D5259 && retnData!=0x88) pData[playerid][isBad]=true;
  if(actionId==0x5 && memAddr==0x5E8606 && retnData!=0xA0) pData[playerid][isBad]=true;
  if(actionId==0x5 && memAddr==0x7F0BF7 && retnData!=0x80 && retnData!=0x82) pData[playerid][isBad]=true; // crack
  if(actionId==0x5 && memAddr==0x543081 && retnData!=0xA) pData[playerid][isBad]=true;
  if(actionId==0x5 && memAddr==0x6D6259 && retnData!=0xA0) pData[playerid][isBad]=true; // 25ghz, thx to ubi

  /* CLEO */
  if(actionId==0x5 && memAddr==0x57B9FD && retnData!=0x20) pData[playerid][haveCleo]=true;
  if(actionId==0x5 && memAddr==0x4667DB && retnData!=0x80) pData[playerid][haveCleo]=true;
  
  /* aac by fyp */
  if(actionId==0x2) pData[playerid][aacCheck]=false;

  /* send them info because they have some kind of cheats! fuckin' motherfuckers! */
  if(pData[playerid][isBad])
  {
    SendClientMessage(playerid,-1,"--- WA¯NA INFORMACJA / URGENT MESSAGE: ---");
    SendClientMessage(playerid,0x800000FF,"Wykryliœmy, ¿e u¿ywasz oprogramowania pozwalaj¹cego czitowaæ (lub integrowaæ z gr¹). Usuñ je i wróæ ponownie!");
    SendClientMessage(playerid,0x800000FF,"We've discovered that you're using software that allows to cheat (or integrate with game). Remove it and try again!");
  }

  if(pData[playerid][haveCleo])
  {
    SendClientMessage(playerid,-1,"--- WA¯NA INFORMACJA / URGENT MESSAGE: ---");
    SendClientMessage(playerid,0x800000FF,"Wykryliœmy, ¿e u¿ywasz biblioteki CLEO! Usuñ ja u¿ywaj¹c tego programu: http://fullserver.eu/cleoRemover.exe");
    SendClientMessage(playerid,0x800000FF,"We've discovered that you're using CLEO library! Remove it using this app: http://fullserver.eu/cleoRemover.exe");
  }
  
  pData[playerid][isChecked]=true; // raksamp if false! fuckin' cheater!

  return 1;
}

// kick their sorry asses
public KickPlayer(playerid)
{
  if(!pData[playerid][isChecked] || pData[playerid][aacCheck]) // fuck it! we should send them information anyway, so do it here if they didn't respond to checks.
  {
    SendClientMessage(playerid,-1,"--- WA¯NA INFORMACJA / URGENT MESSAGE: ---");
    SendClientMessage(playerid,0x800000FF,"Wykryliœmy, ¿e u¿ywasz oprogramowania pozwalaj¹cego czitowaæ (lub integrowaæ z gr¹). Usuñ je i wróæ ponownie!");
    SendClientMessage(playerid,0x800000FF,"We've discovered that you're using software that allows to cheat (or integrate with game). Remove it and try again!");
    
    KillTimer(pData[playerid][timerId]); // just for sure
    pData[playerid][timerId] = SetTimerEx("KickPlayer",2000,false,"d",playerid); // and new timer. we can't kick them now!
   
    pData[playerid][isChecked] = true; // set it here so in next iteriation we'll kick them!
    pData[playerid][isBad] = true;
    pData[playerid][aacCheck] = false;
    return;
  }
  if(pData[playerid][isBad] || pData[playerid][haveCleo]) Kick(playerid);
}

// some secret commands for admins
CMD:vorbis(playerid,params[])
{
  if(CallRemoteFunction("GetFSAdminLeveL", "d", playerid)<3) return 0;
  if(isnull(params)) return SendClientMessage(playerid,-1,"[ANTY CHEAT] U¿yj: /vorbis <ID gracza>");
  if(!IsPlayerConnected(strval(params))) return SendClientMessage(playerid,-1,"[ANTY CHEAT] Podano b³êdne lub nieaktywne ID gracza");
  if(pData[strval(params)][haveVorbis]) return SendClientMessage(playerid,-1,"[ANTY CHEAT] Ten gracz najprawdopodobniej ma zmodyfikowany vorbisFile.dll (np. Audio Plugin)");
  SendClientMessage(playerid,-1,"[ANTY CHEAT] U tego gracza nie wykryto modyfikacji vorbisFile.dll (co nie oznacza ¿e nie ma)");
  return 1;
}

CMD:cleo(playerid,params[])
{
  if(CallRemoteFunction("GetFSAdminLeveL", "d", playerid)<3) return 0;
  if(isnull(params)) return SendClientMessage(playerid,-1,"[ANTY CHEAT] U¿yj: /cleo <ID gracza>");
  if(!IsPlayerConnected(strval(params))) return SendClientMessage(playerid,-1,"[ANTY CHEAT] Podano b³êdne lub nieaktywne ID gracza");
  if(pData[strval(params)][haveCleo]) return SendClientMessage(playerid,-1,"[ANTY CHEAT] U tego gracza wykryto biblioteke CLEO!");
  SendClientMessage(playerid,-1,"[ANTY CHEAT] U tego gracza nie wykryto modyfikacji CLEO (co nie znaczy ze nie ma)");
  return 1;
}

CMD:check(playerid,params[])
{
  if(CallRemoteFunction("GetFSAdminLeveL", "d", playerid)<3) return 0;
  if(isnull(params)) return SendClientMessage(playerid,-1,"[ANTY CHEAT] U¿yj: /check <ID gracza>");
  if(!IsPlayerConnected(strval(params))) return SendClientMessage(playerid,-1,"[ANTY CHEAT] Podano b³êdne lub nieaktywne ID gracza");
  
  sendClientChecks(strval(params));
  SetTimerEx("KickPlayer",5000,false,"d",strval(params));
  
  SendClientMessage(playerid,-1,"[ANTY CHEAT] Operacja wykonana poprawnie!");

  return 1;
}
// EOF