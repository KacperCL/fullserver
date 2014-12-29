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
#include <3rdparty/streamer>

new hPojazdy[7],
  hObiekty[6];

public OnFilterScriptInit()
{
  hPojazdy[0] = AddStaticVehicle(485,1073.0000000,-1037.6999500,7.2000000,270.0000000,-1,-1); //Baggage
  hPojazdy[1] = AddStaticVehicle(607,1069.1999500,-1037.6999500,7.6000000,270.0000000,-1,-1); //Luggage Trailer B
  hPojazdy[2] = AddStaticVehicle(607,1065.4027100,-1037.6992200,7.6043500,270.0000000,-1,-1); //Luggage Trailer B
  hPojazdy[3] = AddStaticVehicle(607,1061.6572300,-1037.6992200,7.6074000,270.0000000,-1,-1); //Luggage Trailer B
  hPojazdy[4] = AddStaticVehicle(607,1057.9062500,-1037.6992200,7.5874000,270.0000000,-1,-1); //Luggage Trailer B
  hPojazdy[5] = AddStaticVehicle(607,1053.5999800,-1037.6999500,7.6000000,270.0000000,-1,-1); //Luggage Trailer B
  hPojazdy[6] = AddStaticVehicle(607,1050.4034400,-1037.6992200,7.5420200,270.0000000,-1,-1); //Luggage Trailer B

  hObiekty[0] = CreateDynamicObject(3092,1139.3000500,-1034.5999800,11.6000000,0.0000000,0.0000000,0.0000000); //object(dead_tied_cop) (8)
  hObiekty[1] = CreateDynamicObject(3092,1176.3000500,-1035.3000500,11.6000000,0.0000000,0.0000000,0.0000000); //object(dead_tied_cop) (9)
  hObiekty[2] = CreateDynamicObject(3092,1245.0999800,-1030.0999800,11.6000000,0.0000000,0.0000000,0.0000000); //object(dead_tied_cop) (10)
  hObiekty[3] = CreateDynamicObject(3092,1278.8000500,-1024.3000500,11.6000000,0.0000000,0.0000000,0.0000000); //object(dead_tied_cop) (11)
  hObiekty[4] = CreateDynamicObject(3092,1354.6999500,-1012.2000100,11.6000000,0.0000000,0.0000000,0.0000000); //object(dead_tied_cop) (12)
  hObiekty[5] = CreateDynamicObject(3092,1420.5999800,-1004.5000000,11.6000000,0.0000000,0.0000000,0.0000000); //object(dead_tied_cop) (13)

  for(new i=0; i<sizeof(hPojazdy); i++){
    LinkVehicleToInterior(hPojazdy[i], 51);
    SetVehicleVirtualWorld(hPojazdy[i], 51);
  }
  SetVehicleParamsEx(hPojazdy[0], 1, 0, 0, 1, 0, 0, 1);

  ConnectNPC("Straszny_kosciotrup","halloween");
  PokazStraszne();
  return 1;
}

public OnFilterScriptExit()
{
  for(new i=0; i<sizeof(hPojazdy); i++){
    DestroyVehicle(hPojazdy[i]);
    DestroyVehicle(hPojazdy[i]);
  }
  return 1;
}

public OnPlayerSpawn(playerid)
{
  if(IsPlayerNPC(playerid))
    {
      SetPlayerInterior(playerid, 51);
      SetPlayerVirtualWorld(playerid, 51);
      PutPlayerInVehicle(playerid, hPojazdy[0], 0);
    }
  return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
  if(IsPlayerNPC(playerid)) return 1;
  if(newinteriorid==51) PlayAudioStreamForPlayer(playerid, "http://fullserver.eu/halloween.mp3");
  if(newinteriorid!=51) StopAudioStreamForPlayer(playerid);
  return 1;
}

forward PokazStraszne();
public PokazStraszne(){
  for(new i=0; i<sizeof(hObiekty); i++){
      static Float:Ox,Float:Oy,Float:Oz;
      GetDynamicObjectPos(hObiekty[i],Ox,Oy,Oz);
      MoveDynamicObject(hObiekty[i], Ox, Oy, Oz-10, 5.0);
  }
  SetTimer("UkryjStraszne",2500,false);
}

forward UkryjStraszne();
public UkryjStraszne(){
  for(new i=0; i<sizeof(hObiekty); i++){
      static Float:Ox,Float:Oy,Float:Oz;
      GetDynamicObjectPos(hObiekty[i],Ox,Oy,Oz);
      MoveDynamicObject(hObiekty[i], Ox, Oy, Oz+10, 5.0);
  }
  SetTimer("PokazStraszne",2500,false);
}
