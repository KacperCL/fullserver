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

forward checkJail(playerid);

new Float:spawns_jail[][3]={
  {-1952.84, 6823.52, 13.65},
  {-1951.48, 6820.68, 13.65},
  {-1921.19, 6809.36, 13.65},
  {-1908.19, 6835.58, 13.65}
};

MoveToJail(playerid){
    if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
  new r=random(sizeof spawns_jail);
  pTemp[playerid][disableWeaponCheck]=true;
  ResetPlayerWeapons(playerid);
  pTemp[playerid][disableWeaponCheck]=false;
  Teleport(T_PLAYER, playerid,spawns_jail[r][0],spawns_jail[r][1],spawns_jail[r][2], random(360), 0, VW_JAIL,true);

  SetPlayerHealth(playerid, 100.0);
  SetPlayerArmour(playerid, 0.0);
  pTemp[playerid][onArena] = ARENA_NONE;
  pData[playerid][pAttraction] = A_NONE;
  pData[playerid][aChowany] = false;
  pData[playerid][aHAY] = false;
  pData[playerid][aDerby] = false;
  pData[playerid][aRace] = false;
  pData[playerid][aDrift] = false;
  pData[playerid][aWG] = false;
  pData[playerid][aStrzelnica] = false;
  pData[playerid][aCTF] = false;

}

JailPlayer(playerid, time)
{
    if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
  pTemp[playerid][onArena] = ARENA_NONE;
  pData[playerid][pAttraction] = A_NONE;
  pData[playerid][aChowany] = false;
  pData[playerid][aHAY] = false;
  pData[playerid][aDerby] = false;
  pData[playerid][aRace] = false;
  pData[playerid][aDrift] = false;
  pData[playerid][aWG] = false;
  pData[playerid][aStrzelnica] = false;
  pData[playerid][aCTF] = false;

  pTemp[playerid][disableWeaponCheck]=true;
  ResetPlayerWeapons(playerid);
  pTemp[playerid][disableWeaponCheck]=false;
  MoveToJail(playerid);

  SetPlayerHealth(playerid, 100.0);
  SetPlayerArmour(playerid, 0.0);
  GivePlayerScore(playerid,-5,"jail");
  pData[playerid][jail] = (GetTickCount() / 1000) + (time * 60);
}

UnjailPlayer(playerid)
{
    if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
  pData[playerid][jail] = 0;
  SpawnPlayer(playerid);
  GameTextForPlayer(playerid, " ", 1, 3);
  SetPlayerProperColor(playerid,true);
}

public checkJail(playerid){
  if(IsPlayerConnected(playerid) && pData[playerid][jailMysql]>0){
      JailPlayer(playerid, floatround(pData[playerid][jailMysql]/60));
      Msg(playerid, COLOR_INFO2, __("Ostatnio uciekles z wiezienia, musisz odsiedziec reszte swojej kary"));
  }
    return 1;
}
// EOF
