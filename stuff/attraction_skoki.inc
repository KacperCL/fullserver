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

new Float:aSkokiSpawns[][3] =
{
	{1541.5370, -1359.3562, 329.4632}, //komisariat w ls
	{-1796.39, 576.02, 234.88}, //wiezowiec w sf
	{-2233.48, -1735.51, 480.83}, //chiliad
	{-2662.31, 1594.48, 225.75}, //bayside
	{2187.83, 2422.03, 74.43}, //komisariat lv
	{1568.62, -1226.38, 277.87}, //kolo /wiezowiec
	{-910.50, 1944.04, 114.96}, //kolo tamy w LV
	{1972.60, 1914.82, 130.93}, //kolo kasyn w LV
	{1482.23, -1791.10, 156.75}, //komisariat w ls
	{-1779.67, 577.48, 234.89} //wielki wiezowiec sf / komisariat sf
};

new Float:aSkokiCps[][4] =
{
	{1569.06, -1720.07, 28.34, 3.0},
	{-1970.61, 643.20, 46.56, 3.0},
	{-1851.23, -1700.06, 40.86, 3.0},
	{-2688.00, 1933.52, 3.22, 3.0},
	{2251.42, 2486.47, 10.99, 3.0},
	{1661.79, -993.41, 31.11, 3.0},
	{-816.56, 1913.33, 7.00, 3.0},
	{2023.49, 1923.53, 12.33, 3.0},
	{1366.73, -1690.08, 13.11, 3.0},
	{-1762.03, 744.79, 34.28, 3.0}
};
//WAZNE! Numer kolejny spawnu i checkpointu musza sie zgadzac inaczej wszystko sie wysypie i z SF musimy doleciec do LS :-)
//NIE UMIESZCZAC CP NA DODANYCH OBIEKTACH!

Skoki_Start()
{
	new
	 aPlayers[MAX_QUEUE_SKOKI],
	 aMaxPlayers = 0,
	 randomIdx;

	foreach(playerid)
	{
		if(pData[playerid][aSkoki])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aSkoki] = false;
				Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
			}
		}
	}

	if(aMaxPlayers < 1)
	{
		foreach(playerid)
		{
			Msg(playerid, COLOR_INFO3, "Brak dostatecznej liczby graczy na {b}/skoki{/b}. Zapisy ponowione.");
		}

		aData[A_SKOKI][aState] = A_STATE_OFF;

		return;
	}
	
	randomIdx = random(sizeof(aSkokiSpawns));
	for(new i = 0; i < aMaxPlayers; i++)
	{
		pData[aPlayers[i]][pAttraction] = A_SKOKI;
		pData[aPlayers[i]][aSkoki] = false;

		TogglePlayerControllable(aPlayers[i], false);
		pTemp[aPlayers[i]][isFreezed]=true;
		SetPlayerArmour(aPlayers[i], 0.0);
		SetPlayerHealth(aPlayers[i], 100.0);
		ResetPlayerWeapons(aPlayers[i]);
		
		if(pData[aPlayers[i]][hudSetting][HUD_DATE]) ShowElement(aPlayers[i], TDE_DATETIME, false);
		if(pData[aPlayers[i]][hudSetting][HUD_STATUSBAR]) ShowElement(aPlayers[i], TDE_STATS, false);
		if(pData[aPlayers[i]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(aPlayers[i], TDE_ATTRACTIONBOX, false);
		
		/*SetPlayerPos(aPlayers[i], 1541.5370,-1359.3562,329.4632);
		SetPlayerCheckpoint(aPlayers[i], 1569.06, -1720.07, 28.34, 3.0); -- debug */
		SetPlayerVirtualWorld(aPlayers[i],VW_SKOKI);
		SetPlayerInterior(aPlayers[i],0);
		SetPlayerPos(aPlayers[i], aSkokiSpawns[randomIdx][0]+random(2)-1, aSkokiSpawns[randomIdx][1]+random(2)-1, aSkokiSpawns[randomIdx][2]);
		GivePlayerWeapon(aPlayers[i],46,1);
		//Msg(aPlayers[i],COLOR_INFO,"Skoki nadal sa w fazie testow. Wszelkie bledy i propozycje co do nowych aren zglaszaj na forum!");
		Msg(aPlayers[i],COLOR_INFO2,"{b}INSTRUKCJA:{/b} Aby wygrac musisz doleciec pierwszy do czerwonego punktu zaznaczonego na mapie (checkpoint)");
		Msg(aPlayers[i],COLOR_INFO2,"{b}WA¯NE:{/b} W razie problemów ze spadochronem zalecamy po³¹czenie siê z serwerem ponownie!");
	}

	gmTemp[aSkokiPlayers] = aPlayers;
	gmTemp[aSkokiMaxPlayers] = aMaxPlayers;
	gmTemp[aSkokiCount] = 5;
	gmTemp[timerSkokiCountdown] = SetTimerEx("Skoki_Countdown", 700, true, "d", randomIdx);
	gmTemp[aSkokiStartTick] = GetTickCount();
}


skoki_AttractionUpdate(tick){
	new
	 aPlayers = 0,
	 bool:aBreak = false;

	for(new i = 0; i < gmTemp[aSkokiMaxPlayers]; i++)
	{
		if(gmTemp[aSkokiPlayers][i] == INVALID_PLAYER_ID) continue;

		if(pData[gmTemp[aSkokiPlayers][i]][pAttraction] == A_SKOKI && GetPlayerVirtualWorld(gmTemp[aSkokiPlayers][i])==VW_SKOKI)
		{
			aPlayers++;
		}
	}

	if(aPlayers == 0)
	{
		aBreak = true;

		foreach(playerid)
			Msg(playerid, COLOR_INFO3, "{b}Skoki{/b} zostaly przerwane z powodu braku graczy");
	}
	else if(tick - gmTemp[aSkokiStartTick] >= 300000) // 5 minut
	{
		aBreak = true;

		foreach(playerid)
			Msg(playerid, COLOR_INFO3,"{b}Skoki{/b} zostaly przerwane z powodu limitu czasu na ukonczenie{/b}");
	}

	if(aBreak)
		skoki_Cleanup();
}

skoki_RemovePlayer(playerid){

    if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
	if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
	if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
	
	pData[playerid][pAttraction] = A_NONE;
}

skoki_Cleanup(){
		aData[A_SKOKI][aState] = A_STATE_OFF;

		for(new i = 0; i < gmTemp[aSkokiMaxPlayers]; i++)
		{
			if(gmTemp[aSkokiPlayers][i] == INVALID_PLAYER_ID || GetPlayerVirtualWorld(gmTemp[aSkokiPlayers][i])!=VW_SKOKI) continue;
			DisablePlayerSounds(gmTemp[aSkokiPlayers][i]);
			DisablePlayerCheckpoint(gmTemp[aSkokiPlayers][i]);
			pData[gmTemp[aSkokiPlayers][i]][pAttraction] = A_NONE;
			SpawnPlayer(gmTemp[aSkokiPlayers][i]);
			if(pData[gmTemp[aSkokiPlayers][i]][hudSetting][HUD_DATE]) ShowElement(gmTemp[aSkokiPlayers][i], TDE_DATETIME, true);
			if(pData[gmTemp[aSkokiPlayers][i]][hudSetting][HUD_STATUSBAR]) ShowElement(gmTemp[aSkokiPlayers][i], TDE_STATS, true);
			if(pData[gmTemp[aSkokiPlayers][i]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(gmTemp[aSkokiPlayers][i], TDE_ATTRACTIONBOX, true);
		}

}

public Skoki_Countdown(randomIdx)
{
	new
	 buffer[64];

	if(gmTemp[aSkokiCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerSkokiCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aSkokiCount]);
	}

	for(new playerid = 0; playerid < gmTemp[aSkokiMaxPlayers]; playerid++)
	{
	    if(!IsPlayerConnected(gmTemp[aSkokiPlayers][playerid])) continue;
		GameTextForPlayer(gmTemp[aSkokiPlayers][playerid], buffer, 1000, 6);

		if(gmTemp[aSkokiCount] == 0)
		{
			PlaySound(gmTemp[aSkokiPlayers][playerid], 1057);
			TogglePlayerControllable(gmTemp[aSkokiPlayers][playerid], true);
			pTemp[gmTemp[aSkokiPlayers][playerid]][isFreezed]=false;
			PlaySound(gmTemp[aSkokiPlayers][playerid], 1068);
			SetPlayerCheckpoint(gmTemp[aSkokiPlayers][playerid], aSkokiCps[randomIdx][0], aSkokiCps[randomIdx][1], aSkokiCps[randomIdx][2], aSkokiCps[randomIdx][3]);
		}
		else
		{
			PlaySound(gmTemp[aSkokiPlayers][playerid], 1056);
		}
	}

	gmTemp[aSkokiCount]--;
}
