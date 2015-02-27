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

#define MAX_LAB     250

new Float:posx=-23.89; // pozycja poczatkowa labiryntu
new Float:posy=2437.0;
new Float:posz=16.48;
new wii=13; // szerokosc labiryntu
new hii=13; // dlugosc labiryntu

new labiryntObj[1500];
new lobjs=0;

/*CMD:labd(playerid,cmdtext[])
{
	DestroyAllDynamicObjects();
	Labirynt_Generate();
	SendClientMessage(playerid,-1,"Done");
	return 1;
}*/

stock shuffle(t[]) // funkcja ktora miesza elementy w tablicy t
{
	new n = tablenotempty(t);
	new temp[3];
	temp[2] = n;

	while(n >= 2)
	{
		// n is now the last pertinent index
		new k = random(n)+1; // 1 <= k <= n
		// Quick swap
		temp[0] = t[n];
		temp[1] = t[k];
		t[n] = temp[1];
		t[k] = temp[0];
		n--;
	}
}
stock math_floor(i) return floatround(i,floatround_floor);
stock idd(x,y) return ((y*(wii+1))+x);
stock xx(i) return (i%(wii+1));
stock yy(i) return math_floor((i/(wii+1)));

table_insert(t[],value)
{
	for(new d=1; d < MAX_LAB; d++)
		if(t[d] == 999)
		{
			t[d] = value;
			break;
		}
}
table_remove(t[],ile)
{
	new cnt;

	for(new d=ile; d > 0; d--)
		if(t[d] < 999)
		{
			cnt = t[d];
			t[d] = 999;
			return cnt;
		}

	return -1;
}
tablenotempty(variable[])
{
	new
		slots;

	for(new d=1; d < MAX_LAB; d++)
		if(variable[d] < 999)
			slots++;

	return slots;
}

Labirynt_Generate()
{
	new stack[MAX_LAB] = {999,...};
	new bool:visited[MAX_LAB];
	new bool:noWallLeft[MAX_LAB];
	new bool:noWallAbove[MAX_LAB];

	table_insert(stack, idd(0,0));
	new free = tablenotempty(stack);

	while(free > 0)
	{
		new cell = table_remove(stack,free);
		new x = xx(cell);
		new y = yy(cell);
		visited[cell] = true;
		new neighbors[MAX_LAB] = {999,...};
		if(x>0)		table_insert(neighbors,idd(x-1,y));
		if(x<wii-1)	table_insert(neighbors,idd(x+1,y));
		if(y>0)		table_insert(neighbors,idd(x,y-1));
		if(y<hii-1)	table_insert(neighbors,idd(x,y+1));

		shuffle(neighbors);

		new bee = tablenotempty(neighbors);

		while(bee > 0)
		{
			new neighbor = table_remove(neighbors,bee);
			new nx = xx(neighbor);
			new ny = yy(neighbor);
			if(!visited[neighbor])
			{
				table_insert(stack,cell);
				if(y==ny)
					if(nx<x)
						noWallLeft[cell] = true;
					else
						noWallLeft[neighbor] = true;
				else
					if(ny<y)
						noWallAbove[cell] = true;
					else
						noWallAbove[neighbor] = true;

				table_insert(stack,neighbor);
				break;
			}
			bee--;
		}
		free = tablenotempty(stack);
	}

	for(new y; y <= hii; y++)
		for(new x; x <= wii; x++)
		{
			new cell = idd(x,y);
			if(!noWallLeft[cell] && y<hii)
				labiryntObj[lobjs++]=CreateDynamicObject(974, posx+(x*6)-3, posy+y*6, posz+1.0,0,0,90,VW_LABIRYNT,(-1),(-1),500.0);

			if(!noWallAbove[cell] && x<wii)
				labiryntObj[lobjs++]=CreateDynamicObject(974, posx+x*6, posy+(y*6)-3, posz+1.0,0,0,0,VW_LABIRYNT,(-1),(-1),500.0);

			labiryntObj[lobjs++]=CreateDynamicObject(3095, posx+x*6, posy+y*6, posz-1.5, 0.1,0.2,0.1,VW_LABIRYNT,(-1),(-1),500.0);
		}
}

Labirynt_Start()
{
	new
	 aPlayers[MAX_QUEUE_LABIRYNT],
	 aMaxPlayers = 0;
	 //randomSpawnIdx,
	 //randomCheckpointIdx;
	
	foreach(playerid)
	{
		if(pData[playerid][aLabirynt])
		{
			if(AttractionAllowed(playerid))
			{
				aPlayers[aMaxPlayers++] = playerid;
				if(gmData[artefactOwner] == playerid) DropArtefact(playerid);
			}
			else
			{
				pData[playerid][aLabirynt] = false;
				Msg(playerid, COLOR_ERROR, __("Jestes nieosiegalny/a i zostajesz usuniety/a z kolejki na ta atrakcje."));
			}
		}
	}
	
	if(aMaxPlayers < 2)
	{
		foreach(playerid)
		{
			Msg(playerid, COLOR_INFO3, TXT(playerid, 272));
		}
		
		aData[A_LABIRYNT][aState] = A_STATE_OFF;
		
		return;
	}
	
	//randomCheckpointIdx = random(sizeof aLabiryntCheckpoints);
	
	Labirynt_Generate();
	
	for(new i = 0; i < aMaxPlayers; i++)
	{
 		SetPlayerPos(aPlayers[i],(((posx+(hii*6))+0.8)-11)+(random(6)-3), (((posy+wii*6)+0.8)-8)+(random(6)-3), posz+3);
 		TogglePlayerControllable(aPlayers[i], false);
 		Streamer_Update(aPlayers[i]);
	            
		pData[aPlayers[i]][pAttraction] = A_LABIRYNT;
		pData[aPlayers[i]][aLabirynt] = false;
		
		
		pTemp[aPlayers[i]][isFreezed]=true;
		SetPlayerArmour(aPlayers[i], 0.0);
		SetPlayerHealth(aPlayers[i], 100.0);
		ResetPlayerWeapons(aPlayers[i]);
		SetPlayerInterior(aPlayers[i],0);
		SetPlayerVirtualWorld(aPlayers[i],VW_LABIRYNT);
		
		if(pData[aPlayers[i]][hudSetting][HUD_DATE]) ShowElement(aPlayers[i], TDE_DATETIME, false);
		if(pData[aPlayers[i]][hudSetting][HUD_STATUSBAR]) ShowElement(aPlayers[i], TDE_STATS, false);
		if(pData[aPlayers[i]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(aPlayers[i], TDE_ATTRACTIONBOX, false);
		
		//randomSpawnIdx = random(sizeof aLabiryntSpawns);
		
		//Teleport(T_PLAYER, aPlayers[i], aLabiryntSpawns[randomSpawnIdx][X], aLabiryntSpawns[randomSpawnIdx][Y], aLabiryntSpawns[randomSpawnIdx][Z], aLabiryntSpawns[randomSpawnIdx][A], 0, VW_LABIRYNT);

		//SetPlayerCheckpoint(aPlayers[i], aLabiryntCheckpoints[randomCheckpointIdx][X], aLabiryntCheckpoints[randomCheckpointIdx][Y], aLabiryntCheckpoints[randomCheckpointIdx][Z], 2.0);
	}
	
	gmTemp[aLabiryntPlayers] = aPlayers;
	gmTemp[aLabiryntMaxPlayers] = aMaxPlayers;
	gmTemp[aLabiryntCount] = 5;
	gmTemp[timerLabiryntCountdown] = SetTimer("Labirynt_Countdown", 700, true);
	gmTemp[aLabiryntStartTick] = GetTickCount();
}


labirynt_AttractionUpdate(tick){
	new
	 aPlayers = 0,
	 bool:aBreak = false;

	for(new i = 0; i < gmTemp[aLabiryntMaxPlayers]; i++)
	{
		if(gmTemp[aLabiryntPlayers][i] == INVALID_PLAYER_ID) continue;
			
		if(pData[gmTemp[aLabiryntPlayers][i]][pAttraction] == A_LABIRYNT && GetPlayerVirtualWorld(gmTemp[aLabiryntPlayers][i])==VW_LABIRYNT)
		//&& IsPlayerInCube(gmTemp[aLabiryntPlayers][i], 1549.6411, -1514.4098, 26.5469, 1445.6764, -1578.9307, 22.5469))
		{
			aPlayers++;
		}
	}
		
	if(aPlayers == 0)
	{
		aBreak = true;
		
		foreach(playerid)
			Msg(playerid, COLOR_INFO3, TXT(playerid, 424)); // Labirynt zosta³ przerwany z powodu braku graczy.
	}
	else if(tick - gmTemp[aLabiryntStartTick] >= 420000) // 7 minut
	{
		aBreak = true;
		
		foreach(playerid)
			Msg(playerid, COLOR_INFO3, TXT(playerid, 427)); // Labirynt zosta³ przerwany z powodu limitu czasu na ukoñczenie.
	}
	
	if(aBreak)
		labirynt_Cleanup();
}

labirynt_RemovePlayer(playerid){

    if(pData[playerid][hudSetting][HUD_DATE]) ShowElement(playerid, TDE_DATETIME, true);
	if(pData[playerid][hudSetting][HUD_STATUSBAR]) ShowElement(playerid, TDE_STATS, true);
	if(pData[playerid][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(playerid, TDE_ATTRACTIONBOX, true);
	
	pData[playerid][pAttraction] = A_NONE;
}

labirynt_Cleanup(){
		aData[A_LABIRYNT][aState] = A_STATE_OFF;
	
		for(new i = 0; i < gmTemp[aLabiryntMaxPlayers]; i++)
		{
			if(gmTemp[aLabiryntPlayers][i] == INVALID_PLAYER_ID || GetPlayerVirtualWorld(gmTemp[aLabiryntPlayers][i])!=VW_LABIRYNT) continue;
			DisablePlayerSounds(gmTemp[aLabiryntPlayers][i]);
			DisablePlayerCheckpoint(gmTemp[aLabiryntPlayers][i]);
			pData[gmTemp[aLabiryntPlayers][i]][pAttraction] = A_NONE;
			SpawnPlayer(gmTemp[aLabiryntPlayers][i]);
			if(pData[gmTemp[aLabiryntPlayers][i]][hudSetting][HUD_DATE]) ShowElement(gmTemp[aLabiryntPlayers][i], TDE_DATETIME, true);
			if(pData[gmTemp[aLabiryntPlayers][i]][hudSetting][HUD_STATUSBAR]) ShowElement(gmTemp[aLabiryntPlayers][i], TDE_STATS, true);
			if(pData[gmTemp[aLabiryntPlayers][i]][hudSetting][HUD_ATTRACTIONBOX]) ShowElement(gmTemp[aLabiryntPlayers][i], TDE_ATTRACTIONBOX, true);
		}
		for(new i = 0; i < (lobjs+1); i++){
		    DestroyDynamicObject(labiryntObj[i]);
		}
		lobjs=0;

}

public Labirynt_Countdown()
{
	new
	 buffer[64];
	
	if(gmTemp[aLabiryntCount] <= 0)
	{
		format(buffer, sizeof buffer, "~g~GO!");
		KillTimer(gmTemp[timerLabiryntCountdown]);
	}
	else
	{
		format(buffer, sizeof buffer, "~r~%i", gmTemp[aLabiryntCount]);
	}
	
	for(new playerid = 0; playerid < gmTemp[aLabiryntMaxPlayers]; playerid++)
	{
		GameTextForPlayer(gmTemp[aLabiryntPlayers][playerid], buffer, 1000, 6);
		
		if(gmTemp[aLabiryntCount] == 0)
		{
			PlaySound(gmTemp[aLabiryntPlayers][playerid], 1057);
			TogglePlayerControllable(gmTemp[aLabiryntPlayers][playerid], true);
			pTemp[gmTemp[aLabiryntPlayers][playerid]][isFreezed]=false;
			PlaySound(gmTemp[aLabiryntPlayers][playerid], 1068);
            SetPlayerCheckpoint(gmTemp[aLabiryntPlayers][playerid],posx+0.8, posy+0.8, posz+0.4,2.0);
		}
		else
		{
			PlaySound(gmTemp[aLabiryntPlayers][playerid], 1056);
		}
	}
	
	gmTemp[aLabiryntCount]--;
}
