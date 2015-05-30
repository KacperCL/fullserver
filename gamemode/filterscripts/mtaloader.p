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
#include <3rdparty/sscanf2>

////////////////////////////////////////////////////////////////////////////////////////////////////// LOADING SECTION
new pliki[][64]={
  "fs-accmissionaryhill.map",
  "fs-bayside.map",
  "fs-bonecounty.map",
  "fs-caltonheights.map",
  "fs-chinatown.map",
  "fs-cityhall.map",
  "fs-commerce.map",
  "fs-kart-race.map", // includes fs-doherty!
  "fs-downtownls.map",
  "fs-downtownsf.map",
  "fs-eastbeach.map",
  "fs-easterbasin.map",
  "fs-easterbayairport.map",
  "fs-eastlossantos.map",
  "fs-elcorona.map",
  "fs-espalande.map",
  "fs-flintcounty.map",
  "fs-fostervalley.map",
  "fs-freight.map",
  "fs-ganton.map",
  "fs-garcia.map",
  "fs-gleenpark.map",
  "fs-idlewood-base.map",
  "fs-idlewood.map",
  "fs-jefferson.map",
  "fs-juniperhh.map",
  "fs-kings.map",
  "fs-lascolinas.map",
  "fs-lasventurasairport.map",
  "fs-littlemexico.map",
  "fs-losflores.map",
  "fs-lossantosinternational.map",
  "fs-marina.map",
  "fs-market.map",
  "fs-militaryfuels.map",
  "fs-mulholland.map",
  "fs-northventuras.map",
  "fs-oceandocks.map",
  "fs-oceanflats.map",
  "fs-oldventuras.map",
  "fs-palisades.map",
  "fs-paradiso.map",
  "fs-playadelseville.map",
  "fs-queens.map",
  "fs-redcounty.map",
  "fs-redsands.map",
  "fs-rodeo.map",
  "fs-santaflora.map",
  "fs-santamariabeach.map",
  "fs-southventuras.map",
  "fs-temple.map",
  "fs-thestrip.map",
  "fs-tierrarobada.map",
  "fs-verdantbluffs.map",
  "fs-veronabeach.map",
  "fs-vinewood.map",
  "fs-weastventuras.map",
  "fs-whetstone.map",
  "fs-willofield.map"
};

public OnFilterScriptInit()
{
  for(new i=0;i<sizeof pliki;i++)
  {
    static fileName[86];
    format(fileName,86,"mta/%s",pliki[i]);
    LoadMtaMap(fileName);
  }
  return 1;
}

public OnFilterScriptExit()
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////// HANDLER CODE SECTION
#pragma tabsize 0

#define MAX_LOADED_ELEMENTS         8000	//Max number of vehicles + objects
#define MAX_MAP_FILE_PATH           80
#define VEHICLE_RESPAWN_DELAY 		300
#define MAP_STREAM_DISTANCE          200.0

#define ADD_OBJECT(%1) 				CreateDynamicObject(%1, interior, world, -1, MAP_STREAM_DISTANCE)
#define MAX_MODS 					14
#define MAX_NUMBERPLATE   32

#define ELEMENT_TYPE_NONE			0
#define ELEMENT_TYPE_OBJECT			1
#define ELEMENT_TYPE_VEHICLE		2

enum MapElement
{
	eElementID,
	eElementType,
	eElementSource[MAX_MAP_FILE_PATH]
}

new
	Float: MapElements[MAX_LOADED_ELEMENTS][MapElement],
	MinElementID = 0, staticVehicleCount;
	
stock NextElementID()
{
	Increment:
	MinElementID++;
	if (MinElementID == MAX_LOADED_ELEMENTS)
	{
	  print("[MTA HANDLER] Cannot load any more map elements. Current maximum "#MAX_LOADED_ELEMENTS" has been reached!");
    CallRemoteFunction("NotifyAdmins","s", "[MTA HANDLER] Cannot load any more map elements. Current maximum "#MAX_LOADED_ELEMENTS" has been reached!");
		return false;
	}
	if (MapElements[MinElementID][eElementType]) 	goto Increment;
	return true;
}

stock LoadMtaMap(file[])
{
	if (!fexist(file) || MinElementID == MAX_LOADED_ELEMENTS)
	{
	  printf("[MTA HANDLER] Map %s cannot be loaded", file);
    CallRemoteFunction("NotifyAdmins","s", "[MTA HANDLER] Map %s cannot be loaded");
		return 0;
	}
	new File:MapFile=fopen(file),
	n, vehicleCount, string[1024], numberplate[MAX_NUMBERPLATE],
	Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz,
	modelid, paintjob, interior, world,
	t = GetTickCount();
	
  while(fread(MapFile, string))
	{
	    if (!sscanf(string, "p<\">'object''interior='d'model='d'dimension='d'posX='f'posY='f'posZ='f'rotX='f'rotY='f'rotZ='f", interior, modelid, world, x, y, z, rx, ry, rz))
      {
        //modelid x y z rx ry rz interior world
	      new objectid = ADD_OBJECT(modelid, x, y, z, rx, ry, rz);
	      if (objectid == INVALID_OBJECT_ID) 
        {
          printf("[MTA HANDLER] Error, object could not be created. Make sure you don't exceed max number of objects.");
          CallRemoteFunction("NotifyAdmins","s", "[MTA HANDLER] Error, object could not be created. Make sure you don't exceed max number of objects.");
	      }
        else
	      {
          Streamer_SetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_DRAW_DISTANCE, MAP_STREAM_DISTANCE);
	        MapElements[MinElementID][eElementType] 	= ELEMENT_TYPE_OBJECT;
	        MapElements[MinElementID][eElementID] 		= objectid;
	        strcp(file, MapElements[MinElementID][eElementSource]);
	        n++;
          if (!NextElementID())
          {
            fclose(MapFile);
            return n;
          }
	      }
	    }
	    else if (!sscanf(string, "p<\">'vehicle''paintjob='d'interior='d'model='d'plate='s["#MAX_NUMBERPLATE"]'dimension='d'posX='f'posY='f'posZ='f'rotZ='f", paintjob, interior, modelid, numberplate, world, x, y, z, rz))
	    {
        new col1, col2, colors[48], mods[80], mod[MAX_MODS];
	      sscanf(string, "p<\">'color='s[48] ", colors);
	      sscanf(string, "p<\">'upgrades='s[80] ", mods);
	      sscanf(colors, "p<,>dd", col1, col2);
	      sscanf(mods, "p<,>A<d>(0)["#MAX_MODS"]", mod);
        
        if (col1<0 || col1>127)
          col1=random(127);
        if (col2<0 || col2>127)
          col2=random(127);

        if (modelid<400 || (x==0 && y==0 && z==0)) continue;

        if (modelid==420 || modelid==438) { col1=6; col2=6; }
        if (modelid==596 || modelid==599 || modelid==497 || modelid==597 || modelid==598 || modelid==523 || modelid==585) { col1=0; col2=1; }
        if (modelid==442 || modelid==490) { col1=0; col2=0; }
        if (modelid==416) { col1=1; col2=3; }
        if (modelid==407 || modelid==544) { col1=3; col2=1; }
        if (modelid==435 || modelid==450 || modelid==584 || modelid==583 || modelid==608 || modelid==606 || modelid==607 || modelid==552 || modelid==574 || modelid==409 || modelid==611 || modelid==592\
          || modelid==449 || modelid==537 || modelid==538 || modelid==430 || modelid==473 || modelid==472 || modelid==453 || modelid==578 || modelid==508) { col1=1; col2=1; }
        if (modelid==528 || modelid==601) { col1=79; col2=79; }
        if (modelid==588) { col1=13; col2=13; }
        if (modelid==582) { col1=15; col2=7; }
        if (modelid==428) { col1=44; col2=54; }
        if (modelid==563) { col1=1; col2=6; }
        if (modelid==488) { col1=2; col2=1; }
      
        //modelid x y z rz col1 col2 paintjob interior world
        new vehicleid = CreateVehicle(modelid, x, y, z, rz, col1, col2, VEHICLE_RESPAWN_DELAY);
			
        if (vehicleid == INVALID_VEHICLE_ID)
        {
          print("[MTA HANDLER] Vehicle failed to load! Check if you exceeded maximum of 2000 vehicles.");
          CallRemoteFunction("NotifyAdmins","s", "[MTA HANDLER] Vehicle failed to load! Check if you exceeded maximum of 2000 vehicles.");
        }
        else
        {
          SetVehicleNumberPlate(vehicleid, numberplate);
          SetVehicleToRespawn(vehicleid);
          for (new i; i < MAX_MODS; i++) if (mod[i]) {
            AddVehicleComponent(vehicleid, mod[i]);
          }
          ChangeVehiclePaintjob(vehicleid, paintjob);
          LinkVehicleToInterior(vehicleid, interior);
          SetVehicleVirtualWorld(vehicleid, world);
	            
          MapElements[MinElementID][eElementType] 	= ELEMENT_TYPE_VEHICLE;
          MapElements[MinElementID][eElementID] 		= vehicleid;
          strcp(file, MapElements[MinElementID][eElementSource]);
          
          n++;
          vehicleCount++;
          if (!NextElementID())
          {
            fclose(MapFile);
            return n;
          }
        }
	    }
	}
	fclose(MapFile);
	printf("[MTA HANDLER] %d items loaded from '%s' in %dms", n, file, GetTickCount()-t);
  new errorLine[128];
  format(errorLine, 128, "[MTA HANDLER] %d items loaded from '%s' in %dms", n, file, GetTickCount()-t);
  CallRemoteFunction("NotifyAdmins","s", errorLine);
  
  CallRemoteFunction("vehicles_LoadStatic","d", 1);
  
  staticVehicleCount += vehicleCount;
	return n;
}

forward @@@export@getStaticVehicleCount(pad);
public @@@export@getStaticVehicleCount(pad)
{
  return staticVehicleCount;
}

stock strcp(from[], to[])
{
	new i;
	for (i = 0; from[i]; i++) to[i] = from[i];
	to[i] = '\0';
}
// EOF