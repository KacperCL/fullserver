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

#define HAY_OX  (1882.0)
#define HAY_OY  (-1395.0)
#define HAY_OZ  (500.0)

enum e_HayPiece {
  ehp_object,
}
new HayArray[10][10][e_HayPiece];

new lastWeaponHolder=-1;
new bool:isLastIteration=false;
static maxPlayerId;

CMD:hay(playerid)
{
  MUST_BE_SPAWNED(playerid);
  if(pData[playerid][aHAY]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 253));
  if(GetAttractionPlayerCount(A_HAY) > MAX_QUEUE_HAY) return Msg(playerid, COLOR_ERROR, TXT(playerid, 423));

  pData[playerid][aHAY] = true;
  Msg(playerid, COLOR_INFO, TXT(playerid, 261));

  return 1;
}

CMD:hayexit(playerid){
  if (pData[playerid][pAttraction]==A_HAY) {
    pData[playerid][pAttraction] = A_NONE;
    SpawnPlayer(playerid);
    Msg(playerid,COLOR_INFO,"Opusciles {b}/HAY{/b}");
  } else if (pData[playerid][aHAY]) {
    pData[playerid][aHAY]=false;  // zapisy
    Msg(playerid,COLOR_INFO,"Zrezygnowales z zapisow na {b}/HAY{/b}");
  }
  return 1;
}

HAY_Cleanup()
{
  for (new x=0;x<10;x++)
    for (new y=0;y<10;y++)
      DestroyDynamicObject(HayArray[x][y][ehp_object]);
}

HAY_Start()
{
  new
   aPlayers[MAX_QUEUE_HAY],
   aMaxPlayers = 0;

  foreach(playerid)
  {
    if(pData[playerid][aHAY])
    {
      if(AttractionAllowed(playerid))
      {
        aPlayers[aMaxPlayers++] = playerid;
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
      }
      else
      {
        pData[playerid][aHAY] = false;
        Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
      }
    }
  }

  if(aMaxPlayers < 1)
  {
    foreach(playerid)
      Msg(playerid, COLOR_INFO3, TXT(playerid, 270));
    aData[A_HAY][aState] = A_STATE_OFF;
    return;
  }
  HAY_Cleanup();

  for (new x=0;x<10;x++)
    for (new y=0;y<10;y++) {
      HayArray[x][y][ehp_object]=CreateDynamicObject(3374,HAY_OX+(x*4),HAY_OY+(y*4),HAY_OZ, 0,0,0,  VW_HAY,0,-1,150);
    }

  for(new i = 0; i < aMaxPlayers; i++)
  if (pTemp[aPlayers[i]][staleTime]<5) {
    pData[aPlayers[i]][pAttraction] = A_HAY;
    pData[aPlayers[i]][aHAY] = false;
    new x=random(10);
    new y=random(10);

    Streamer_UpdateEx(aPlayers[i],HAY_OX+(x*4),HAY_OY+(y*4),HAY_OZ+2.0);
    TogglePlayerControllable(aPlayers[i], false);
    ResetPlayerWeapons(aPlayers[i]);

    Teleport(T_PLAYER, aPlayers[i], HAY_OX+(x*4),HAY_OY+(y*4),HAY_OZ+2.0, (floatsin(y)*floatcos(x)*90), 0, VW_HAY);
    Streamer_Update(aPlayers[i]);
  }

  gmTemp[aHAYPlayers] = aPlayers;
  gmTemp[aHAYMaxPlayers] = aMaxPlayers;
  gmTemp[aHAYCount] = 5;
  gmTemp[timerHAYCountdown] = SetTimer("HAY_Countdown", 700, true);
  gmTemp[aHAYStartTick] = GetTickCount();
  lastWeaponHolder = -1;
  isLastIteration = true;
}

forward HAY_Countdown();
public HAY_Countdown()
{
  new
   buffer[64];

  if(gmTemp[aHAYCount] <= 0)
  {
    format(buffer, sizeof buffer, "~g~GO!");
    KillTimer(gmTemp[timerHAYCountdown]);
  }
  else
  {
    format(buffer, sizeof buffer, "~r~%i", gmTemp[aHAYCount]);
  }

  for(new playerid = 0; playerid < gmTemp[aHAYMaxPlayers]; playerid++)
  {
    GameTextForPlayer(gmTemp[aHAYPlayers][playerid], buffer, 1000, 6);

    if(gmTemp[aHAYCount] <= 0)
    {
      PlaySound(gmTemp[aHAYPlayers][playerid], 1057);
      ResetPlayerWeapons(gmTemp[aHAYPlayers][playerid]);
      TogglePlayerControllable(gmTemp[aHAYPlayers][playerid], true);
      SetPlayerHealth(gmTemp[aHAYPlayers][playerid],99999);
      SetPlayerArmour(gmTemp[aHAYPlayers][playerid],0);
      SetPlayerSkillLevel(gmTemp[aHAYPlayers][playerid],WEAPONSKILL_PISTOL_SILENCED, 999);
    }
    else
    {
      PlaySound(gmTemp[aHAYPlayers][playerid], 1056);
    }
  }

  gmTemp[aHAYCount]--;
}

HAY_Update()
{
  if (aData[A_HAY][aState] != A_STATE_ON || gmTemp[aHAYCount]>0)
    return;
  new activePlayers=0,lastPlayer=INVALID_PLAYER_ID;
  for(new i = 0; i < gmTemp[aHAYMaxPlayers]; i++)
    if (gmTemp[aHAYPlayers][i]!=INVALID_PLAYER_ID) {
      if (!IsPlayerConnected(gmTemp[aHAYPlayers][i]) || GetPlayerVirtualWorld(gmTemp[aHAYPlayers][i])!=VW_HAY || pTemp[gmTemp[aHAYPlayers][i]][staleTime]>5) {
        SetPlayerVirtualWorld(gmTemp[aHAYPlayers][i],0);
        Msg(gmTemp[aHAYPlayers][i],COLOR_INFO,"Zostales usuniety z {b}/HAY{/b} za AFK");
        pData[gmTemp[aHAYPlayers][i]][pAttraction] = A_NONE;
        SpawnPlayer(gmTemp[aHAYPlayers][i]);
        gmTemp[aHAYPlayers][i]=INVALID_PLAYER_ID;
        continue;
      }
      new Float:PP[3];
      GetPlayerPos(gmTemp[aHAYPlayers][i],PP[0],PP[1],PP[2]);
      if(floatabs(PP[2]-HAY_OZ)>10) {
        Msg(gmTemp[aHAYPlayers][i],COLOR_INFO,"Wypadleœ z areny {b}/HAY{/b}");
        SetPlayerVirtualWorld(gmTemp[aHAYPlayers][i],0);
        pData[gmTemp[aHAYPlayers][i]][pAttraction] = A_NONE;
          SpawnPlayer(gmTemp[aHAYPlayers][i]);
        gmTemp[aHAYPlayers][i]=INVALID_PLAYER_ID;
        continue;
      }
      if (++activePlayers==1) lastPlayer=i;
      maxPlayerId=i;
    }
    if(!isLastIteration){
        lastWeaponHolder++;
        isLastIteration = true;
        if(lastWeaponHolder>maxPlayerId) lastWeaponHolder=0;
        if(gmTemp[aHAYPlayers][lastWeaponHolder]!=INVALID_PLAYER_ID){
            ResetPlayerWeapons(gmTemp[aHAYPlayers][lastWeaponHolder]);
      GivePlayerWeapon(gmTemp[aHAYPlayers][lastWeaponHolder],23,1);
    }
  }else{
      isLastIteration = false;
  }
  if (activePlayers==1) {
    new resp=5+floatround(gmTemp[aHAYMaxPlayers]/2);
    format(gstr,sizeof gstr, "HAY wygrywa, jako ostatni%s - {ffff00}%s{ffffff} - otrzymuje {ffff00}%d{ffffff} %s.",
      Kobieta(gmTemp[aHAYPlayers][lastPlayer])?("a"):(""), GetPlayerProperName(gmTemp[aHAYPlayers][lastPlayer]), resp, dli(resp,"punkt respektu","punkty respektu","punktow respektu"));
    foreach(playerid)
      Msg(playerid,-1,gstr);

    GivePlayerScore(gmTemp[aHAYPlayers][lastPlayer], resp, "HAY");

    pData[gmTemp[aHAYPlayers][lastPlayer]][pAttraction] = A_NONE;
      SpawnPlayer(gmTemp[aHAYPlayers][lastPlayer]);
    gmTemp[aHAYPlayers][lastPlayer]=INVALID_PLAYER_ID;
    aData[A_HAY][aState]=A_STATE_OFF;
    return;
  }
  if (activePlayers==0) {
    aData[A_HAY][aState]=A_STATE_OFF;
  }
  return;
}

HAY_Finish(){
  aData[A_HAY][aState]=A_STATE_OFF;
  for(new i = 0; i < gmTemp[aHAYMaxPlayers]; i++)
    if (gmTemp[aHAYPlayers][i]!=INVALID_PLAYER_ID) {
      SetPlayerVirtualWorld(gmTemp[aHAYPlayers][i],0);
      pData[gmTemp[aHAYPlayers][i]][pAttraction] = A_NONE;
      gmTemp[aHAYPlayers][i]=INVALID_PLAYER_ID;
    }
  return;
}
// EOF
