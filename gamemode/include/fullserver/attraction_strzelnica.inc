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

Strzelnica_Start()
{
  new
   aPlayers[MAX_QUEUE_STRZELNICA],
   aMaxPlayers = 0,
   aShooter,
   Float:aPosition[] = {283.3859, -135.1865},
   Float:t_radius,
   Float:t_angle,
   t_index = 1,
   aWeapons[] = {24, 25, 29, 30, 31, 33, 34};

  foreach(playerid)
  {
    if(pData[playerid][aStrzelnica])
    {
      if(AttractionAllowed(playerid) && pTemp[playerid][staleTime]<5 && !pTemp[playerid][godMode])
      {
        aPlayers[aMaxPlayers++] = playerid;
        if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
      }
      else
      {
        pData[playerid][aStrzelnica] = false;
        Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
      }
    }
  }

  if(aMaxPlayers < 3)
  {
    foreach(playerid)
    {
      Msg(playerid, COLOR_INFO3, TXT(playerid, 276));
    }

    aData[A_STRZELNICA][aState] = A_STATE_OFF;

    return;
  }

  aShooter = aPlayers[random(aMaxPlayers)];
  gmTemp[aStrzelnicaShooter] = aShooter;

  TogglePlayerControllable(aShooter,false);
  SetPlayerInterior(aShooter,7);
  SetPlayerVirtualWorld(aShooter,VW_STRZELNICA);
  Teleport(T_PLAYER, aShooter, 300.3222, -134.0520, 1005.0625, 90.0);
  ResetPlayerWeapons(aShooter);
  GivePlayerWeapon(aShooter, aWeapons[random(sizeof aWeapons)], 9800);
  SetPlayerHealth(aShooter, 9999999.0);

  t_radius = 5;
  t_angle = 360.0 / (aMaxPlayers - 1);

  for(new i = 0; i < aMaxPlayers; i++)
  {
    pData[aPlayers[i]][pAttraction] = A_STRZELNICA;
    pData[aPlayers[i]][aStrzelnica] = false;
    TogglePlayerControllable(aPlayers[i], false);
    SetPlayerArmour(aPlayers[i], 0.0);

    SetPlayerInterior(aPlayers[i],7);
    SetPlayerVirtualWorld(aPlayers[i],VW_STRZELNICA);

    if(aPlayers[i] == aShooter) continue;

    aPosition[0] += (t_radius * floatsin(t_angle * (t_index/2), degrees));
    aPosition[1] += (t_radius * floatcos(t_angle * (t_index/2), degrees));
    t_index++;

    Teleport(T_PLAYER, aPlayers[i], aPosition[0], aPosition[1], 1004.0625, 270.0);
    ResetPlayerWeapons(aPlayers[i]);
    SetPlayerHealth(aPlayers[i], 99.0);
  }

  gmTemp[aStrzelnicaPlayers] = aPlayers;
  gmTemp[aStrzelnicaMaxPlayers] = aMaxPlayers;
  gmTemp[aStrzelnicaCount] = 5;
  gmTemp[timerStrzelnicaCountdown] = SetTimer("Strzelnica_Countdown", 700, true);
  gmTemp[aStrzelnicaStartTick] = GetTickCount();
}

public Strzelnica_Countdown()
{
  new
   buffer[64];

  if(gmTemp[aStrzelnicaCount] <= 0)
  {
    format(buffer, sizeof buffer, "~g~GO!");
    KillTimer(gmTemp[timerStrzelnicaCountdown]);
  }
  else
  {
    format(buffer, sizeof buffer, "~r~%i", gmTemp[aStrzelnicaCount]);
  }

  for(new playerid = 0; playerid < gmTemp[aStrzelnicaMaxPlayers]; playerid++)
  {
    GameTextForPlayer(gmTemp[aStrzelnicaPlayers][playerid], buffer, 1000, 6);

    if(gmTemp[aStrzelnicaCount] == 0)
    {
      PlaySound(gmTemp[aStrzelnicaPlayers][playerid], 1057);
      TogglePlayerControllable(gmTemp[aStrzelnicaPlayers][playerid], true);
    }
    else
    {
      PlaySound(gmTemp[aStrzelnicaPlayers][playerid], 1056);
    }
  }
  gmTemp[aStrzelnicaCount]--;
}

strzelnica_AttractionUpdate(tick,bool:force=false)
{
  new
     aPlayers = 0,
     bool:aBreak = false,
     aLastPlayer,
     buffer[128];

    if(!IsPlayerConnected(gmTemp[aStrzelnicaShooter]) || !IsPlayerInCube(gmTemp[aStrzelnicaShooter], 305.5774, -125.0515, 1006.8765, 269.3169, -145.3345, 1003.0625))
    {
      aBreak = true;

      SetPlayerHealth(gmTemp[aStrzelnicaShooter],100.0);

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}Strzelnica{/b} zostala przerwana z powodu braku graczy")); // Strzelnica zosta³a przerwana z powodu braku graczy.
    }
    else if(tick - gmTemp[aStrzelnicaStartTick] >= 480000) // 8 minut
    {
      aBreak = true;

      SetPlayerHealth(gmTemp[aStrzelnicaShooter],100.0);

      foreach(playerid)
        Msg(playerid, COLOR_INFO3, __("{b}Strzelnica{/b} zostala przerwana z powodu limitu czasu na ukonczenie")); // Strzelnica zosta³a przerwana z powodu limitu czasu na ukoñczenie.
    }

    if(aBreak || force)
    {
      aData[A_STRZELNICA][aState] = A_STATE_OFF;

      for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
      {
        if(gmTemp[aStrzelnicaPlayers][i] == INVALID_PLAYER_ID) continue;

        pData[gmTemp[aStrzelnicaPlayers][i]][pAttraction] = A_NONE;
        SetPlayerHealth(gmTemp[aStrzelnicaShooter],100.0);
        SpawnPlayer(gmTemp[aStrzelnicaPlayers][i]);
      }
    }
    else
    {
      for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
      {
        if(gmTemp[aStrzelnicaPlayers][i] == INVALID_PLAYER_ID || gmTemp[aStrzelnicaPlayers][i] == gmTemp[aStrzelnicaShooter]) continue;

        if(pData[gmTemp[aStrzelnicaPlayers][i]][pAttraction] == A_STRZELNICA && IsPlayerInCube(gmTemp[aStrzelnicaPlayers][i], 305.5774, -125.0515, 1006.8765, 269.3169, -145.3345, 1003.0625))
        {
          if (random(2)==1) SetPlayerPos(gmTemp[aStrzelnicaPlayers][i], 286.28+random(5)-2.5, -133.93+random(5)-2.5, 1004.16);
          aPlayers++;
          aLastPlayer = gmTemp[aStrzelnicaPlayers][i];
        }
      }

      if(aPlayers == 1)
      {
        foreach(playerid)
        {
          if(pData[playerid][pAttraction] == A_STRZELNICA) pData[playerid][pAttraction] = A_NONE;
          if(playerid == aLastPlayer) continue;

          format(buffer, sizeof buffer, __("{b}%s{/b} wygrywa strzelnice!"), GetPlayerNick(aLastPlayer));
          Msg(playerid, COLOR_INFO3, buffer);
        }

        Msg(aLastPlayer, COLOR_INFO2, __("Wygrywasz strzelnice i otrzymujesz +10 respektu")); // Wygrywasz strzelnicê! +10 respekt

        GivePlayerScore(aLastPlayer, 10, "wygrana na strzelnicy");

        aData[A_STRZELNICA][aState] = A_STATE_OFF;

        pData[aLastPlayer][pAttraction] = A_NONE;
        pData[gmTemp[aStrzelnicaShooter]][pAttraction] = A_NONE;

        SpawnPlayer(aLastPlayer);
        SetPlayerHealth(gmTemp[aStrzelnicaShooter],100.0);
        SpawnPlayer(gmTemp[aStrzelnicaShooter]);
      }
      else if(aPlayers == 0)
      {
        aData[A_STRZELNICA][aState] = A_STATE_OFF;

        for(new i = 0; i < gmTemp[aStrzelnicaMaxPlayers]; i++)
        {
          if(gmTemp[aStrzelnicaPlayers][i] == INVALID_PLAYER_ID) continue;

          pData[gmTemp[aStrzelnicaPlayers][i]][pAttraction] = A_NONE;
          SetPlayerHealth(gmTemp[aStrzelnicaShooter],100.0);
          SpawnPlayer(gmTemp[aStrzelnicaPlayers][i]);
          SetPlayerHealth(gmTemp[aStrzelnicaPlayers][i], 100.0);
        }

        foreach(playerid)
        {
          Msg(playerid, COLOR_INFO3, __("{b}Strzelnica{/b} zostaje przerwana z powodu braku graczy"));
        }
      }
    }
}
// EOF
