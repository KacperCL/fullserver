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
forward OnClientCheckResponse(playerid,action,major,minor);
native SendClientCheck(playerid,action,major,minor,crc);

// forward our public function
forward KickPlayer(playerid);

// and now some variables for our script, yey!
enum e_pData {
  bool:isBad,
  bool:isChecked,
  timerId // should be INVALID_TIMER_ID but actually we're using KillTimer in OnFilterScriptInit so it's ok
}
new pData[MAX_PLAYERS][e_pData];

// oh, oh! send checks to these fuckin' cheaters!
sendClientChecks(playerid)
{
  if(!IsPlayerConnected(playerid)) return;
  
  pData[playerid][isBad] = false;
  pData[playerid][isChecked]=false;

  /* simple check for RakSAMP */
  SendClientCheck(playerid, 0x02, 0x00000000, 0x0000, 0x0004);
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
public OnClientCheckResponse(playerid,action,major,minor)
{
  new pName[MAX_PLAYER_NAME+1];
  GetPlayerName(playerid, pName, sizeof(pName));
  printf("OnClientCheckResponse: %d (%s) = %x %x %x",playerid,pName,action,major,minor);
  
  pData[playerid][isChecked]=true; // raksamp if false! fuckin' cheater!

  return 1;
}

// kick their sorry asses
public KickPlayer(playerid)
{
  if(!pData[playerid][isChecked]) // fuck it! we should send them information anyway, so do it here if they didn't respond to checks.
  {
    SendClientMessage(playerid,-1,"--- WA¯NA INFORMACJA / URGENT MESSAGE: ---");
    SendClientMessage(playerid,0x800000FF,"Wykryliœmy, ¿e u¿ywasz oprogramowania pozwalaj¹cego czitowaæ (lub integrowaæ z gr¹). Usuñ je i wróæ ponownie!");
    SendClientMessage(playerid,0x800000FF,"We've discovered that you're using software that allows to cheat (or integrate with game). Remove it and try again!");
    
    KillTimer(pData[playerid][timerId]); // just for sure
    pData[playerid][timerId] = SetTimerEx("KickPlayer",2000,false,"d",playerid); // and new timer. we can't kick them now!
   
    pData[playerid][isChecked] = true; // set it here so in next iteriation we'll kick them!
    pData[playerid][isBad] = true;
    return;
  }
  if(pData[playerid][isBad]) Kick(playerid);
}
// EOF