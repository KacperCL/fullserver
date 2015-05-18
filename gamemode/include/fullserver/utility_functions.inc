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

bool:IsPlayerSpawned(playerid)
{
  new pstate=GetPlayerState(playerid);
  if(pstate != 1 && pstate != 2 && pstate != 3) return false;

  return true;
}

/* == setPlayerSkin hook == */
#if defined CJHIT_MODEL

stock AC_SetPlayerSkin(playerid,skinid){
  if(!IsPlayerConnected(playerid)) return;
  if(!IsValidSkin(skinid)) return;
  SetPlayerSkin(playerid,skinid);

  if(pData[playerid][hudSetting][HUD_CJHIT] && IsPlayerSpawned(playerid)){
    PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT][playerid], skinid);
    PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT2][playerid], skinid);
    ShowPlayerHudElement(playerid, HUD_CJHIT, true);
  }
}

#if defined _ALS_SetPlayerSkin
    #undef SetPlayerSkin
#else
    #define _ALS_SetPlayerSkin
#endif
#define SetPlayerSkin AC_SetPlayerSkin

#endif
/* == setPlayerSkin hook end == */

stock AC_TogglePlayerControllable(playerid,toggle){
  if(!IsPlayerConnected(playerid)) return;
  TogglePlayerControllable(playerid,toggle);
  if(toggle){
      pTemp[playerid][isFreezed]=false;
  }else{
      pTemp[playerid][isFreezed]=true;
  }
}

#if defined _ALS_TogglePlayerControllable
    #undef TogglePlayerControllable
#else
    #define _ALS_TogglePlayerControllable
#endif
#define TogglePlayerControllable AC_TogglePlayerControllable

stock AC_ChangeVehicleColor(vehicleid, color1, color2){
    if(!IsValidVehicle(vehicleid)) return 0;
  ChangeVehicleColor(vehicleid, color1, color2);
  tVehicles[vehicleid][vo_color][0]=color1;
  tVehicles[vehicleid][vo_color][1]=color2;
  return 1;
}

#if defined _ALS_ChangeVehicleColor
    #undef ChangeVehicleColor
#else
    #define _ALS_ChangeVehicleColor
#endif
#define ChangeVehicleColor AC_ChangeVehicleColor

stock AC_CreateVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, addsiren=0){
  new vf=GetVehicleFlagsByModel(modelid);
  if (vf&VF_POLICE==VF_POLICE) addsiren = 1;
  
  new vehicleid = CreateVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, addsiren);
  if(vehicleid==INVALID_VEHICLE_ID) {
      print("[DBG DATA] Reached vehicles limit!");
    RespawnVehicles(true);
    return INVALID_VEHICLE_ID;
  }
  tVehicles[vehicleid][vo_color][0]=color1;
  tVehicles[vehicleid][vo_color][1]=color2;
  return vehicleid;
}

#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif
#define CreateVehicle AC_CreateVehicle

stock AC_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2){
  new vehicleid = AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2);
  if(vehicleid==INVALID_VEHICLE_ID) {
      print("[DBG DATA] Reached vehicles limit!");
    RespawnVehicles(true);
    return INVALID_VEHICLE_ID;
  }
  tVehicles[vehicleid][vo_color][0]=color1;
  tVehicles[vehicleid][vo_color][1]=color2;
  return vehicleid;
}

#if defined _ALS_AddStaticVehicle
    #undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif
#define AddStaticVehicle AC_AddStaticVehicle

stock AC_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2, respawn_delay, addsiren=0){
  new vf=GetVehicleFlagsByModel(modelid);
  if (vf&VF_POLICE==VF_POLICE) addsiren = 1;
  
  new vehicleid = AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2, respawn_delay, addsiren);
  if(vehicleid==INVALID_VEHICLE_ID) {
      print("[DBG DATA] Reached vehicles limit!");
    RespawnVehicles(true);
    return INVALID_VEHICLE_ID;
  }
  tVehicles[vehicleid][vo_color][0]=color1;
  tVehicles[vehicleid][vo_color][1]=color2;
  return vehicleid;
}

#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif
#define AddStaticVehicleEx AC_AddStaticVehicleEx

Delay:AC_Kick[100,](playerid)
{
  if(!IsPlayerConnected(playerid)) return;
  Kick(playerid);
}

#if defined _ALS_Kick
    #undef Kick
#else
    #define _ALS_Kick
#endif
#define Kick AC_Kick

stock AC_PlayAudioStreamForPlayer(playerid, url[], Float:posX = 0.0, Float:posY = 0.0, Float:posZ = 0.0, Float:distance = 50.0, usepos = 0)
{
  if(!IsPlayerConnected(playerid)) return;
  if(!pTemp[playerid][playPlayall]) return;
  PlayAudioStreamForPlayer(playerid,url,posX,posY,posZ,distance,usepos);
}

#if defined _ALS_PlayAudioStreamForPlayer
    #undef PlayAudioStreamForPlayer
#else
    #define _ALS_PlayAudioStreamForPlayer
#endif
#define PlayAudioStreamForPlayer AC_PlayAudioStreamForPlayer

/* === print & printf hooks for debug mode === */
stock AC_printf(const fmat[], {Float, _}:...) {
	static output[128], frm_header[3];
	const output_size = sizeof(output);

	#emit LCTRL      5
	#emit CONST.alt  frm_header
	#emit MOVS       12
	#emit ADD.C    12
	#emit SCTRL    4
	#emit PUSH.C      output_size
	#emit PUSH.C      output
	#emit LOAD.S.pri  8
	#emit ADD.C       8
	#emit PUSH.pri
	#emit SYSREQ.C    format
	#emit LCTRL       5
	#emit SCTRL       4
	#emit MOVE.alt
	#emit CONST.pri   frm_header
	#emit MOVS        12
	#emit LOAD.S.pri  8
	#emit ADD.C       12
	#emit MOVE.alt
	#emit LCTRL       5
	#emit ADD
	#emit LOAD.I
	#emit STOR.S.pri  20

  if(gmTemp[debugMode]){
    static debugmsg[128];
    format(debugmsg,128,"DBG: %s",output);
    MSGToAdmins(-1, debugmsg, true, LEVEL_ADMIN3);
  }

  return print(output);

	#pragma unused fmat
}

#if defined _ALS_printf
    #undef printf
#else
    #define _ALS_printf
#endif
#define printf AC_printf

stock AC_print(const string[]){
  print(string);
  if(gmTemp[debugMode]){
    static debugmsg[128];
    format(debugmsg,128,"DBG: %s",string);
    MSGToAdmins(-1, debugmsg, true, LEVEL_ADMIN3);
  }
}

#if defined _ALS_print
    #undef print
#else
    #define _ALS_print
#endif
#define print AC_print
/* === END: print & printf hooks for debug mode === */ 

stock rot13(string[])
{
	for (new index = 0; index < strlen(string); index++)
	{
		if ('a' <= string[index] <= 'z')
		{
			string[index] = (string[index] - 'a' + 13) % 26 + 'a';
		}
		else if ('A' <= string[index] <= 'Z')
		{
			string[index] = (string[index] - 'A' + 13) % 26 + 'A';
		}
	}
	return string;
}

forward BanExCommit(playerid); // ugly fix for strings in timers
public BanExCommit(playerid)
{
  if(!IsPlayerConnected(playerid)) return;
  BanEx(playerid, gmTemp[banEx]);
}

stock AC_BanEx(playerid, const reason[]) // call only when needed!
{
  if(!IsPlayerConnected(playerid)) return;
  format(gmTemp[banEx],sizeof gmTemp[banEx],"%s",reason);
  SetTimerEx("BanExCommit",1000,false,"d",playerid);
}

stock IsFreeTime()
{
  new hr, minuteeee, seecondd;
  gettime(hr, minuteeee, seecondd);
  if(hr>=22 || hr<=6) return 1;
  return 0;
}

unpliterki(s[]){
  for(new i=0;i<strlen(s);i++)
    if (s[i]>=140)
    switch(s[i]){
      case 185: s[i]='a';
      case 165: s[i]='A';
      case 230: s[i]='c';
      case 198: s[i]='C';
      case 234: s[i]='e';
      case 202: s[i]='E';
      case 179: s[i]='l';
      case 163: s[i]='L';
      case 241: s[i]='n';
      case 209: s[i]='N';
      case 243: s[i]='o';
      case 211: s[i]='O';
      case 156: s[i]='s';
      case 140: s[i]='S';
      case 191: s[i]='z';
      case 175: s[i]='Z';
      case 159: s[i]='z';
      case 143: s[i]='Z';
    }
  return;
}

// 0x22BAFBFF - no-dm
// 0x840B0BFF - full-dm

stock RoundGangZoneForAll(const Float: radius, const Float: Xx , const Float: Yy, const color, const size = 20) {
    new
        Float: inc = 90.0 / size,
        Float: deg = inc,
        Float: cos,
        Float: sin,
        zone;

    while(deg < 90.0) {
        cos = floatcos(deg, degrees) * radius;
        sin = floatsin(deg, degrees) * radius;

        zone = GangZoneCreate(Xx - cos, Yy - sin, Xx + cos, Yy + sin);
        GangZoneShowForAll(zone, color);

        deg += inc;
    }
    return zone;
}

stock NormalGangZoneForAll(const Float: minX , const Float: minY, const Float: maxX, const Float: maxY, const color) {

  new zone = GangZoneCreate(minX, minY, maxX, maxY);
  GangZoneShowForAll(zone, color);

    return zone;
}

stock RoundGangZoneForPlayer(const Float: radius, const Float: Xx , const Float: Yy, const color, const playerid, const size = 20) {
    new
        Float: inc = 90.0 / size,
        Float: deg = inc,
        Float: cos,
        Float: sin,
        zone;

    while(deg < 90.0) {
        cos = floatcos(deg, degrees) * radius;
        sin = floatsin(deg, degrees) * radius;

        zone = GangZoneCreate(Xx - cos, Yy - sin, Xx + cos, Yy + sin);
        GangZoneShowForPlayer(playerid, zone, color);

        deg += inc;
    }
    return zone;
}

stock NormalGangZoneForPlayer(const Float: minX , const Float: minY, const Float: maxX, const Float: maxY, const color, const playerid) {

  new zone = GangZoneCreate(minX, minY, maxX, maxY);
  GangZoneShowForPlayer(playerid, zone, color);

    return zone;
}

stock ShiftRGBAToABGR(&color)
{
    new r, g, b, a;
    r = (color >>> 24);
    g = (color >>> 16 & 0xFF);
    b = (color >>> 8 & 0xFF);
    a = (color  & 0xFF);
    color = (a & 0xFF) | ((b & 0xFF) << 8) | ((g & 0xFF) << 16) | (r << 24);
    return color;
}

stock IsPlayerSkydiving(playerid) {
    new index = GetPlayerAnimationIndex(playerid);
    return (index >= 958 && index <= 962);
}

stock IsPlayerUsingParachute(playerid) {
    new index = GetPlayerAnimationIndex(playerid);
    return (index >= 963 && index <= 979);
}

stock IsPlayerAiming(playerid)
{
    new anim = GetPlayerAnimationIndex(playerid);
    switch(anim)
    {
        case 1167, 1365, 1643, 1453, 220:
        {
            return 1;
        }
    }
    return 0;
}

forward TeleportUnfreeze(playerid);
forward TeleportVerify(playerid,Float:vectorX,Float:vectorY,Float:vectorZ);

vehicles_GetSpawnOffset(vid,&Float:vx,&Float:vy,&Float:vz) {
  vehicles_GetSpawnOffsetByModel(GetVehicleModel(vid),vx,vy,vz);
  return;
}
vehicles_GetSpawnOffsetByModel(model,&Float:vx,&Float:vy,&Float:vz) {
  switch (model) {
    case 432:   // rhino
      vz+=2;
    case 525:     // towtruck
      vz+=1;
    case 403,514,515:     // linerunner, petrol, roadtrain
      vz+=1;
    case 506:       // dumper
      vz+=0.5;
    case 443:     // laweta
      vz+=0.5;
    case 532:     // kombajn
      vz+=1.5;
    case 531:
      vz+=0.5;
    case 417,425,553,577,592,548:     // nevada i inne samoloty
      vz+=5;
  }
  return;
}

// quake's fast inverse sqrt
Float:InvSqrt(Float:value)
{
    new i, Float:x, Float:y;
    x = value * 0.5;
    y = value;
    i = _:y;
    i = 0x5f3759df - (i >> 1);
    y = Float:i;
    y = y * (1.5 - (x * y * y));
    //y = y * (1.5 - (x * y * y));
    return y;
}

#define sqrt(%0) (1.0 / InvSqrt((%0)))

bool:PlaySound(playerid, soundid, bool:pVectorInc = false)
{
  if(!IsPlayerConnected(playerid)) return false;

  new
   pVector[e_Vectors];

  if(pVectorInc)
  {
    GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);
  }

  PlayerPlaySound(playerid, soundid, pVector[X], pVector[Y], pVector[Z]);

  return true;
}

Msg(playerid, colorid, text[], bool:playSound = true, bool:displayPrefix = true)
{
  new
   buffer[256],
   curColor,
   tmpColor[16],
   tmpColorCode;

  switch(colorid)
  {
    case COLOR_INFO:
    {
      curColor = gmData[color_chatInfo];
      tmpColorCode = gmData[color_chatInfo_HL];
    }
    case COLOR_INFO2:
    {
      curColor = gmData[color_chatInfo2];
      tmpColorCode = gmData[color_chatInfo2_HL];
    }
    case COLOR_INFO3:
    {
      curColor = gmData[color_chatInfo3];
      tmpColorCode = gmData[color_chatInfo3_HL];
    }
    case COLOR_ERROR:
    {
      curColor = gmData[color_chatError];
      tmpColorCode = gmData[color_chatError_HL];
    }
    case COLOR_JOININFO:
    {
      curColor = gmData[color_joinInfo];
      tmpColorCode = gmData[color_joinInfo_HL];
    }
    case COLOR_LEAVEINFO:
    {
      curColor = gmData[color_leaveInfo];
      tmpColorCode = gmData[color_leaveInfo_HL];
    }
    case COLOR_ADMIN:
    {
      curColor = gmData[color_chatAdmin];
      tmpColorCode = gmData[color_chatAdmin_HL];
    }
    case COLOR_ADMIN3:
    {
      curColor = gmData[color_chatAdmin3];
      tmpColorCode = gmData[color_chatAdmin3_HL];
    }
    case COLOR_PRIVATEMSG:
    {
      curColor = gmData[color_chatPM];
      tmpColorCode = gmData[color_chatPM_HL];
    }
    case COLOR_IC:
    {
      curColor = gmData[color_chatIC];
      tmpColorCode = gmData[color_chatIC_HL];
    }
    case COLOR_ME:
    {
      curColor = gmData[color_chatME];
      tmpColorCode = gmData[color_chatME_HL];
    }
    case COLOR_GM:
    {
      curColor = gmData[color_chatGM];
      tmpColorCode = gmData[color_chatGM_HL];
    }
    case COLOR_VIPSAY:
    {
      curColor = gmData[color_vipSay];
      tmpColorCode = gmData[color_vipSay_HL];
    }
    case COLOR_VIP:
    {
      curColor = gmData[color_chatVip];
      tmpColorCode = gmData[color_chatVip_HL];
    }

    default: curColor = colorid;
  }

  copy(text, buffer);

  format(tmpColor, sizeof tmpColor, "{%06x}", tmpColorCode);

  new f = strfind( buffer, "{b}" );
  while ( f >= 0)
  {
      strdel( buffer, f, f + 3 );
      strins( buffer, tmpColor, f );
      f = strfind( buffer, "{b}" );
  }
  format(tmpColor, sizeof tmpColor, "{%06x}", curColor);

  f = strfind( buffer, "{/b}" );
    while ( f >= 0)
    {
      strdel( buffer, f, f + 4 );
      strins( buffer, tmpColor, f );
        f = strfind( buffer, "{/b}" );
  }

  if(displayPrefix)
    strins(buffer, "››› ", 0);

  SendClientMessage(playerid, curColor * 256, buffer);

  if(playSound)
  {
    if(colorid == COLOR_ERROR)
    {
      PlaySound(playerid, 1085);
    }
    else
    {
      PlaySound(playerid, 1150);
    }
  }

  return 1;
}

FindString(string1[], string2[], &start = 0, &end = 0)
{
  if(!strlen(string1) || !strlen(string2)) return false;
  new num = strfind(string1, string2, true);
  
  if(num != -1) {
   start = num;
   end = start+strlen(string2);
   return true;
  }

  return false;
}

HexToInt(string[])
{
    if(string[0] == 0) return 0;

    new
     cur = 1,
     res = 0;

    for(new i = strlen(string); i > 0; i--)
    {
        if(string[i - 1] < 58)
        {
            res = res + cur * (string[i - 1] - 48);
        }
        else
        {
            res = res + cur * (string[i - 1] - 65 + 10);
        }
    cur = cur * 16;
    }

    return res;
}

IntToHex(int)
{
  new
   buffer[8],
   i = 0,
   result[8],
   g = 0;

  while(int > 0)
  {
    new tmp = int % 16;

    if(tmp < 10)  buffer[i] = tmp + 48;
    else      buffer[i] = tmp + 55;

    i++;

    int = int / 16;
  }

  for(i = strlen(buffer) - 1; i >= 0; i--)
  {
    result[g++] = buffer[i];
  }

  while(strlen(result) < 6)
  {
    strins(result, "0", 0);
  }

  return result;
}

Text:DrawRectangle(Float:startX, Float:endX, Float:startY, Float:endY, color)
{
  endY -= startY;
  new Text:TD = TextDrawCreate(startX, startY, "~n~");
  TextDrawLetterSize(TD, 0.0, (endY == 0.0) ? 0.0 : (endY / 9.1));
  TextDrawTextSize(TD, endX, endX);
  TextDrawUseBox(TD, 1);
  TextDrawBoxColor(TD, color);

  return TD;
}

Float:GetDistanceBetweenPoints3D(Float:vX1, Float:vY1, Float:vZ1, Float:vX2, Float:vY2, Float:vZ2)
{
  return floatabs(VectorSize(floatsub(vX1, vX2),floatsub(vY1, vY2),floatsub(vZ1, vZ2)));
}

ShowElement(playerid, tdElement, bool:show)
{
  switch(tdElement)
  {
    case TDE_WIDE:
    {
      if(show)
      {
        for(new i = 0; i < 10; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
        for(new i = 10; i < 20; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
      }
      else
      {
        for(new i = 0; i < 10; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
        for(new i = 10; i < 20; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
      }
    }

    case TDE_WYBIERALKA:
    {
      if (show){
        TextDrawShowForPlayer(playerid, gTextDraw[73]);
        TextDrawShowForPlayer(playerid, gTextDraw[74]);
        TextDrawShowForPlayer(playerid, gTextDraw[75]);
      } else {
        TextDrawHideForPlayer(playerid, gTextDraw[73]);
        TextDrawHideForPlayer(playerid, gTextDraw[74]);
        TextDrawHideForPlayer(playerid, gTextDraw[75]);
      }
    }

    case TDE_VOTING:
    {
      if(show) TextDrawShowForPlayer(playerid, gTextDraw[TD_VOTING]);
      else TextDrawHideForPlayer(playerid, gTextDraw[TD_VOTING]);
    }

    case TDE_DATETIME:
    {
      if(show) {
        TextDrawShowForPlayer(playerid, gTextDraw[TD_DATETIME]);
      }
      else {
        TextDrawHideForPlayer(playerid, gTextDraw[TD_DATETIME]);
      }
    }
    case TDE_CLOCK:
    {
      if(show) {
        TextDrawShowForPlayer(playerid, gTextDraw[TD_TIME]);
      }
      else {
        TextDrawHideForPlayer(playerid, gTextDraw[TD_TIME]);
      }
    }
    case TDE_FPS:
    {
      if(show) PlayerTextDrawShow(playerid, pTextDraw[PTD_FPS][playerid]);
      else PlayerTextDrawHide(playerid, pTextDraw[PTD_FPS][playerid]);
    }

    case TDE_ATTRACTIONBOX:
    {
      if(show)
      {
//        for(new i = TD_ATTRACTIONBOX; i < TD_ATTRACTIONBOX + 8; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
        TextDrawShowForPlayer(playerid, gTextDraw[TD_ATTRACTIONBOX]);
        TextDrawShowForPlayer(playerid, gTextDraw[TD_ATTRACTION_TITLES]);
        TextDrawShowForPlayer(playerid, gTextDraw[TD_ATTRACTION_STATES]);
      }
      else
      {
//        for(new i = TD_ATTRACTIONBOX; i < TD_ATTRACTIONBOX + 8; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
        TextDrawHideForPlayer(playerid,gTextDraw[TD_ATTRACTIONBOX]);
        TextDrawHideForPlayer(playerid, gTextDraw[TD_ATTRACTION_TITLES]);
        TextDrawHideForPlayer(playerid, gTextDraw[TD_ATTRACTION_STATES]);
      }
    }

    case TDE_VEHICLEBOX:
      if(show)
        PlayerTextDrawShow(playerid, pTextDraw[PTD_VEHICLEINFO][playerid]);
      else
        PlayerTextDrawHide(playerid, pTextDraw[PTD_VEHICLEINFO][playerid]);

    case TDE_STATS:
    {
      if(show)
      {
        TextDrawShowForPlayer(playerid, gTextDraw[20]);
        TextDrawShowForPlayer(playerid, gTextDraw[21]);
        TextDrawShowForPlayer(playerid, gTextDraw[22]);
        TextDrawShowForPlayer(playerid, gTextDraw[TD_STAT]);
        PlayerTextDrawShow(playerid, pTextDraw[PTD_STAT][playerid]);
        PlayerTextDrawShow(playerid, pTextDraw[PTD_NICK][playerid]);

        pData[playerid][statsShowed] = true;
      }
      else
      {
        TextDrawHideForPlayer(playerid, gTextDraw[20]);
        TextDrawHideForPlayer(playerid, gTextDraw[21]);
        TextDrawHideForPlayer(playerid, gTextDraw[22]);
        TextDrawHideForPlayer(playerid, gTextDraw[TD_STAT]);
        PlayerTextDrawHide(playerid, pTextDraw[PTD_STAT][playerid]);
        PlayerTextDrawHide(playerid, pTextDraw[PTD_NICK][playerid]);

        pData[playerid][statsShowed] = false;
      }
    }

    case TDE_FULLSERVERLOGO:
    {
      if(show)
      {
        TextDrawShowForPlayer(playerid, gTextDraw[TD_FULLSERVER]);
        TextDrawShowForPlayer(playerid, gTextDraw[TD_FULLSERVERWEBSITE]);
      }
      else
      {
        TextDrawHideForPlayer(playerid, gTextDraw[TD_FULLSERVER]);
        TextDrawHideForPlayer(playerid, gTextDraw[TD_FULLSERVERWEBSITE]);
      }
    }
  }
}

GetPlayerCount()
{
  new
   result = 0;

  foreach(i)
  {
    result++;
  }

  return result;
}

bool:StringToBool(string[])
{
  if(string[0] == '0') return false;
  else return true;
}

BoolToInt(bool:boolean)
{
  if(boolean) return 1;
  else return 0;
}

StringToInt(string[])
{
  return strval(string);
}

stock copy(string1[], string2[])
{

  for(new i = 0; i < strlen(string1); i++)
  {
      string2[i] = string1[i];
  }

  string2[strlen(string1)] = EOS;
}

FindPlayer(playerName[])
{
    new result,
      foundCount = 0;

  if(IsNumeric(playerName))
  {
    result = strval(playerName);
    if(!IsPlayerConnected(result)) return ERROR_NOT_FOUND;
  }
  else
  {
      foreach(i)
      {
      if(FindString(GetPlayerNick(i), playerName))  // bylo GetPlayerProperName
          {
        foundCount++;
        result = i;
          }
      }
    if(foundCount < 1) return ERROR_NOT_FOUND;
    else if(foundCount > 1) return ERROR_FOUND_MULTIPLE;
  }

  return result;
}

GetPeriodName(playerid, period, number)
{
  static foundPeriodName[10];
  foundPeriodName="?";
#pragma unused playerid
  switch(period) {
    case 'c': foundPeriodName=dli(number,"sekunde","sekundy","sekund");
    case 'm': foundPeriodName=dli(number,"minute","minuty","minut");
    case 'g','h': foundPeriodName=dli(number,"godzine","godziny","godzin");
    case 'd': foundPeriodName=dli(number,"dzien","dni","dni");
    case 't': foundPeriodName=dli(number,"tydzien","tygodnie","tygodni");
    case 's': foundPeriodName=dli(number,"miesiac","miesiace","miesiecy");
  }
  return foundPeriodName;
}

GetPeriodName2(playerid, period, number)
{
  switch(number)
  {
    case 1:
    {
      switch(period)
      {
        case 'c': return gLang[pData[playerid][language]][161];
        case 'm': return gLang[pData[playerid][language]][162];
        case 'g': return gLang[pData[playerid][language]][163];
        case 'h': return gLang[pData[playerid][language]][163];
        case 'd': return gLang[pData[playerid][language]][35];
        case 't': return gLang[pData[playerid][language]][36];
        case 's': return gLang[pData[playerid][language]][37];
      }
    }
    case 2..4:
    {
      switch(period)
      {
        case 'c': return gLang[pData[playerid][language]][73];
        case 'm': return gLang[pData[playerid][language]][38];
        case 'g','h': return gLang[pData[playerid][language]][39];

        case 'd': return gLang[pData[playerid][language]][40];
        case 't': return gLang[pData[playerid][language]][41];
        case 's': return gLang[pData[playerid][language]][42];
      }
    }
    default:
    {
      switch(period)
      {
        case 'c': return gLang[pData[playerid][language]][74];
        case 'm': return gLang[pData[playerid][language]][43];
        case 'g','h': return gLang[pData[playerid][language]][44];
        case 'd': return gLang[pData[playerid][language]][45];
        case 't': return gLang[pData[playerid][language]][46];
        case 's': return gLang[pData[playerid][language]][47];
      }
    }
  }

  return gLang[pData[playerid][language]][43];
}

GetMonthName(playerid, month)
{
  return gLang[pData[playerid][language]][164 + month];
}

PassHash(const playerName[], const password[])
{
  new
   hashed[80];

  format(hashed, sizeof hashed, "%s%s%s", playerName, PASS_SECURE_SALT, password);

  return hashed;
}

GetMySQLNameOfPeriod(period)
{
  new
   result[8];

  switch(period)
  {
    case 'm': copy("MINUTE", result);
    case 'g','h': copy("HOUR", result);
    case 'd': copy("DAY", result);
    case 't': copy("WEEK", result);
    case 's': copy("MONTH", result);
  }

  return result;
}

GetPlayerIP(playerid)
{
  new
   szIP[16];

  GetPlayerIp(playerid, szIP, sizeof szIP);

  return szIP;
}

GetOptimalTimeUnit(&seconds, &unit)
{
    if(seconds < 60) // seconds
    {
        unit = 'c';
    }
    else if(seconds >= 60 && seconds < 3600) // minutes
    {
        unit = 'm';
        seconds /= 60;
    }
    else if(seconds >= 3600 && seconds < 864000) // hours
    {
        unit = 'g';
        seconds /= 3600;
    }
    else // days
    {
        unit = 'd';
        seconds /= 86400;
    }
}

bool:IsAdmin(playerid, level = 2)
{
  if(pData[playerid][adminLevel] < level) return false;
  return true;
}

Teleport(type, playerid, Float:vectorX, Float:vectorY, Float:vectorZ, Float:angle = 0.0, interior = 0xFFF, virtualWorld = 0xFFF, bool:prestream=false)
{
  if(interior != 0xFFF) SetPlayerInterior(playerid, interior);
  if(virtualWorld != 0xFFF) SetPlayerVirtualWorld(playerid, virtualWorld);

  if (prestream)
    Streamer_UpdateEx(playerid,vectorX,vectorY,vectorZ);

  pTemp[playerid][specPosReturn]=false;

  if(type == T_PLAYER || !IsPlayerInAnyVehicle(playerid) ||  GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
  {
    if(GetPlayerAnimationIndex(playerid)==1134) RemovePlayerWeapon(playerid,46);
    SetPlayerPos(playerid, vectorX, vectorY, vectorZ);
    if(interior != 0xFFF) SetPlayerInterior(playerid, interior);
    if(virtualWorld != 0xFFF) SetPlayerVirtualWorld(playerid, virtualWorld);
    if (prestream) {
      FreezePlayer(playerid,750);
      SetPlayerPos(playerid, vectorX, vectorY, vectorZ);
      Streamer_Update(playerid);
    }
    if (angle==angle)
      SetPlayerFacingAngle(playerid, angle);

  } else if(type == T_BOTH) {

    new
     vehicleid = GetPlayerVehicleID(playerid);

    vehicles_GetSpawnOffset(vehicleid, vectorX, vectorY, vectorZ);
    if (GetPlayerInterior(playerid)>0 && GetPlayerInterior(playerid)!=interior) {
      SetPlayerInterior(playerid,interior);
      LinkVehicleToInterior(vehicleid,interior);
    }
      if (GetPlayerVirtualWorld(playerid)>0 && GetPlayerVirtualWorld(playerid)!=virtualWorld && !pTemp[playerid][cheater]) {
      SetPlayerVirtualWorld(playerid,virtualWorld);
      SetVehicleVirtualWorld(vehicleid,virtualWorld);
    }
    SetVehiclePos(vehicleid, vectorX, vectorY, vectorZ);
    if (angle==angle)
      SetVehicleZAngle(vehicleid, angle);
  } else if(type == T_BOTH_PV) {
    new
     vehicleid = GetPlayerVehicleID(playerid),
     Float:V[3];
    GetVehicleVelocity(vehicleid,V[0],V[1],V[2]);

    if (GetPlayerInterior(playerid)>0 && GetPlayerInterior(playerid)!=interior) {
      SetPlayerInterior(playerid,interior);
      LinkVehicleToInterior(vehicleid,interior);
    }
    if (GetPlayerVirtualWorld(playerid)>0 && GetPlayerVirtualWorld(playerid)!=virtualWorld && !pTemp[playerid][cheater]) {
      SetPlayerVirtualWorld(playerid,virtualWorld);
      SetVehicleVirtualWorld(vehicleid,virtualWorld);
    }

    SetVehiclePos(vehicleid, vectorX, vectorY, vectorZ);
    if (angle==angle)
      SetVehicleZAngle(vehicleid, angle);
    SetVehicleVelocity(vehicleid, V[0], V[1], V[2]);

  }
  if (prestream)
    SetTimerEx("TeleportVerify",1000,false,"dfff",playerid,vectorX,vectorY,vectorZ);

  pTemp[playerid][lastPos]=-1;
  pTemp[playerid][curPos]=1;

  SetCameraBehindPlayer(playerid);
}

public TeleportVerify(playerid,Float:vectorX,Float:vectorY,Float:vectorZ){
  new Float:PP[3];

  if (IsPlayerInAnyVehicle(playerid)) {
    GetVehiclePos(GetPlayerVehicleID(playerid),PP[0],PP[1],PP[2]);
    if (vectorZ-PP[2]>2)
      SetVehiclePos(GetPlayerVehicleID(playerid),vectorX,vectorY,vectorZ);
  } else {
    GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
    if (vectorZ-PP[2]>1)
      SetPlayerPos(playerid,vectorX,vectorY,vectorZ);
  }

  return 1;
}

bool:IsValidSkin(skinid)
{
  if(skinid < 0 || skinid > 311) return false;
  switch(skinid)
    {
      case 74: return false;
    }
  return true;
}

bool:IsPlayerInCube(playerid, Float:maxX, Float:maxY, Float:maxZ, Float:minX, Float:minY, Float:minZ)
{
  new
   Float:pVector[e_Vectors];

  GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);

  if(pVector[X] > minX && pVector[Y] > minY && pVector[Z] > minZ && pVector[X] < maxX && pVector[Y] < maxY && pVector[Z] < maxZ)
  {
    return true;
  }

  return false;
}

GetPlayerPosition(playerid, &Float:vX, &Float:vY, &Float:vZ, &Float:vA = 1.1111)
{
  GetPlayerPos(playerid, vX, vY, vZ);
  if(vA != 1.1111) GetPlayerFacingAngle(playerid, vA);  // co to za magic number, TODO zamienic na FLOAT_NAN. Idealny material na bohr-buga
}

bool:IsCloseToATM(playerid)
{
  new
   Float:pDistance;

  for(new i = 0; i < sizeof DATA_ATM; i++)
  {
    pDistance = GetPlayerDistanceFromPoint(playerid, DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z]);

    if(pDistance < 1.3) return true;
  }

  return false;
}

DownCase(letter)
{
  if(letter >= 'A' && letter <= 'Z')
  {
    return letter + 32;
  }

  switch(letter)
  {
    case 'Ê': return 'ê';
    case 'Ó': return 'ó';
    case 'Œ': return 'œ';
    case '£': return '³';
    case '¯': return '¿';
    case '': return 'Ÿ';
    case 'Æ': return 'æ';
    case 'Ñ': return 'ñ';
  }

  return letter;
}

SetAlpha(color, alpha)
{
  return (((color >> 24) & 0xFF) << 24 | ((color >> 16) & 0xFF) << 16 | ((color >> 8) & 0xFF) << 8 | floatround((float(color & 0xFF) / 255) * alpha));
}

bool:IsCorrectIP(szIP[])
{
  if(!strlen(szIP) || strlen(szIP) > 15) return false;

  new
   _numbers,
   _dots,
   _stars;

  for(new i = 0; i < strlen(szIP); i++)
  {
    if((szIP[i] < '0' && szIP[i] > '9') && szIP[i] != '.' && szIP[i] != '*') return false;

    switch(szIP[i])
    {
      case '0'..'9': _numbers++;
      case '.': _dots++;
      case '*': _stars++;
      default: return false;
    }
  }

  if(_numbers > 13) return false;
  if(_dots != 3) return false;
  if(_stars > 4) return false;
  if(_numbers + _stars < 4) return false;

  return true;
}

RefreshPlayerStars(playerid)
{
  new
   pLevel;

  if(pData[playerid][respect] <= 4)                     pLevel = 1;
  if(pData[playerid][respect] >= 5 && pData[playerid][respect] <= 9)      pLevel = 2;
  if(pData[playerid][respect] >= 10 && pData[playerid][respect] <= 14)    pLevel = 3;
  if(pData[playerid][respect] >= 15 && pData[playerid][respect] <= 19)    pLevel = 4;
  if(pData[playerid][respect] >= 20 && pData[playerid][respect] <= 24)    pLevel = 5;
  if(pData[playerid][respect] >= 25 && pData[playerid][respect] <= 29)    pLevel = 6;
  if(pData[playerid][respect] >= 30 && pData[playerid][respect] <= 59)    pLevel = 7;
  if(pData[playerid][respect] >= 60 && pData[playerid][respect] <= 89)    pLevel = 8;
  if(pData[playerid][respect] >= 90 && pData[playerid][respect] <= 119)   pLevel = 9;
  if(pData[playerid][respect] >= 120 && pData[playerid][respect] <= 149)    pLevel = 10;
  if(pData[playerid][respect] >= 150 && pData[playerid][respect] <= 179)    pLevel = 11;
  if(pData[playerid][respect] >= 180 && pData[playerid][respect] <= 209)    pLevel = 12;
  if(pData[playerid][respect] >= 210 && pData[playerid][respect] <= 309)    pLevel = 13;
  if(pData[playerid][respect] >= 310 && pData[playerid][respect] <= 409)    pLevel = 14;
  if(pData[playerid][respect] >= 410 && pData[playerid][respect] <= 509)    pLevel = 15;
  if(pData[playerid][respect] >= 510 && pData[playerid][respect] <= 609)    pLevel = 16;
  if(pData[playerid][respect] >= 610 && pData[playerid][respect] <= 709)    pLevel = 17;
  if(pData[playerid][respect] >= 710 && pData[playerid][respect] <= 809)    pLevel = 18;
  if(pData[playerid][respect] >= 810 && pData[playerid][respect] <= 1809)   pLevel = 19;
  if(pData[playerid][respect] >= 1810 && pData[playerid][respect] <= 2809)  pLevel = 20;
  if(pData[playerid][respect] >= 2810 && pData[playerid][respect] <= 3809)  pLevel = 21;
  if(pData[playerid][respect] >= 3810 && pData[playerid][respect] <= 4809)  pLevel = 22;
  if(pData[playerid][respect] >= 4810 && pData[playerid][respect] <= 5809)  pLevel = 23;
  if(pData[playerid][respect] >= 5810 && pData[playerid][respect] <= 6809)  pLevel = 24;
  if(pData[playerid][respect] >= 6810 && pData[playerid][respect] <= 16809) pLevel = 25;
  if(pData[playerid][respect] >= 16810 && pData[playerid][respect] <= 26809)  pLevel = 26;
  if(pData[playerid][respect] >= 26810 && pData[playerid][respect] <= 36809)  pLevel = 27;
  if(pData[playerid][respect] >= 36810 && pData[playerid][respect] <= 46809)  pLevel = 28;
  if(pData[playerid][respect] >= 46810 && pData[playerid][respect] <= 56809)  pLevel = 29;
  if(pData[playerid][respect] >= 56810)                   pLevel = 30;

  for(new i = TD_STARS_START; i <= TD_STARS_END; i++)
  {
    TextDrawHideForPlayer(playerid, gTextDraw[i]);
  }

  switch(pLevel)
  {
    case 1..6:
    {
      for(new i = 1; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
    }

    case 7..12:
    {
      for(new i = 7; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
    }

    case 13..18:
    {
      for(new i = 13; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
    }

    case 19..24:
    {
      for(new i = 19; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
    }

    case 25..30:
    {
      for(new i = 25; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
    }
  }
}

GetVehicleSpeed(vehicleid)
{
  // 1000 jednostek w grze, przejechanych z predkoscia 100km/h zajmuje 28minut
  // wychodzi 46.2km
  // odl [j] / 21.645 = odl [km/h]
  new
   vVector[e_Vectors];

  GetVehicleVelocity(vehicleid, vVector[X], vVector[Y], vVector[Z]);

  return floatround(floatmul(VectorSize(vVector[X], vVector[Y], vVector[Z]), 190.0));

// bylo:
//    result = floatsqroot(floatpower(floatabs(vVector[X]), 2.0) + floatpower(floatabs(vVector[Y]), 2.0) + floatpower(floatabs(vVector[Z]), 2.0));
//  return floatround(result * 100 * 1.61);
}

Float:GetPlayerSpeed(playerid)
{
  new
    Float:vVector[3];
  GetPlayerVelocity(playerid, vVector[0], vVector[1], vVector[2]);

  return VectorSize(vVector[0], vVector[1], vVector[2]);
}

bool:CheckTildes(szText[])
{
  new
   tCount;

  for(new i = 0; i < strlen(szText); i++)
  {
    if(szText[i] == '~') tCount++;
  }

  if(tCount & 1) return false;
  else return true;
}

GetPlayerNick(playerid)
{
  new
   szPlayerName[24];

  GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);

  return szPlayerName;
}

bool:SpaceCheck(string[])
{
  for(new i = 0; i < strlen(string); i++)
  {
    if(string[i] == ' ') return true;
  }

  return false;
}

bool:TeleportAllowed(playerid)
{
  if (pTemp[playerid][cheater]) return false;
  if(gmData[artefactOwner] == playerid){
    switch(random(3)){
      case 0:
        Msg(playerid,COLOR_INFO2,"Czujesz dziwne mrowienie na plecach...", false);
      case 1:
        Msg(playerid,COLOR_INFO2,"Artefakt przez chwile staje sie bardzo ciezki...", false);
      case 2:
        Msg(playerid,COLOR_INFO2,"Artefakt na sekunde robi sie bardzo goracy...", false);
    }
    return false;
  }
  if(pData[playerid][jail] > 0) return false;
  if (pData[playerid][pAttraction]==A_ARENA) {
      Msg(playerid,COLOR_INFO,"Jestes na arenie. Aby moc korzystac z komend opusc ja za pomoca komendy {b}/arenaexit{/b}.");
      GameTextForPlayer(playerid,"_~n~_~n~/arenaexit", 1000,5);
      return false;
  }
  if(pData[playerid][pAttraction] == A_WG && pTemp[playerid][aWGDead]) { }
  else if(pData[playerid][pAttraction] != A_NONE) return false;

  if(!IsPlayerSpawned(playerid) || pData[playerid][spectating] != INVALID_PLAYER_ID) return false;

  if(GetPlayerInterior(playerid)==1 && IsPlayerInAnyVehicle(playerid)) return false;

  if(pTemp[playerid][isFreezed]) return false;

  if(pTemp[playerid][desync]) return false;

  if(IsPlayerInAnyVehicle(playerid) && GetVehicleModel(GetPlayerVehicleID(playerid))==537 || GetVehicleModel(GetPlayerVehicleID(playerid))==538) return false;

    if(GetPVarInt(playerid,"Zaladowano")>0) return false;

    if(GetPVarInt(playerid,"jetmax")>0) return false;

  return true;
}

bool:AttractionAllowed(playerid)
{
  if(GetPlayerInterior(playerid)==1 && IsPlayerInAnyVehicle(playerid)) return false;
  if(pTemp[playerid][cheater]) return false;
  if(pData[playerid][pAttraction] != A_NONE) return false;
  if(pData[playerid][spectating] != INVALID_PLAYER_ID) return false;
  if(!IsPlayerSpawned(playerid)) return false;
  if(pData[playerid][jail] > 0) return false;
  if(pTemp[playerid][staleTime]>5) return false;
  if(pTemp[playerid][isFreezed]) return false;

  return true;
}

bool:SpecAllowed(playerid)
{
  if(pData[playerid][jail] > 0) return false;
  if(pData[playerid][pAttraction] != A_NONE) return false;
  if(GetPlayerInterior(playerid)==1 && IsPlayerInAnyVehicle(playerid)) return false;
  if(pTemp[playerid][isFreezed]) return false;

  return true;
}

GetAttractionPlayerCount(attractionId)
{
  new
   result = 0;

  foreach(playerid)
  {
    switch(attractionId)
    {
      case A_CHOWANY: if(pData[playerid][aChowany]) result++;
      case A_HAY: if(pData[playerid][aHAY]) result++;
      case A_DERBY: if(pData[playerid][aDerby]) result++;
      case A_RACE: if(pData[playerid][aRace]) result++;
      case A_DRIFT: if(pData[playerid][aDrift]) result++;
      case A_WG: if(pData[playerid][aWG]) result++;
      case A_STRZELNICA: if(pData[playerid][aStrzelnica]) result++;
      case A_CTF: if(pData[playerid][aCTF]) result++;
      case A_GG: if(pData[playerid][aGG]) result++;
    }
  }

  return result;
}

GetAttractionMinimumQueueValue(attractionId)
{
  switch(attractionId)
  {
    case A_CHOWANY: return MIN_QUEUE_CHOWANY;
    case A_HAY: return MIN_QUEUE_HAY;
    case A_DERBY: return MIN_QUEUE_DERBY;
    case A_RACE: return MIN_QUEUE_RACE;
    case A_DRIFT: return MIN_QUEUE_DRIFT;
    case A_WG: return MIN_QUEUE_WG;
    case A_STRZELNICA: return MIN_QUEUE_STRZELNICA;
    case A_CTF: return MIN_QUEUE_CTF;
    case A_GG: return MIN_QUEUE_GG;
  }

  return 0;
}

GetAttractionMaximumQueueValue(attractionId)
{
  switch(attractionId)
  {
    case A_CHOWANY: return MAX_QUEUE_CHOWANY;
    case A_HAY: return MAX_QUEUE_HAY;
    case A_DERBY: return MAX_QUEUE_DERBY;
    case A_RACE: return MAX_QUEUE_RACE;
    case A_DRIFT: return MAX_QUEUE_DRIFT;
    case A_WG: return MAX_QUEUE_WG;
    case A_STRZELNICA: return MAX_QUEUE_STRZELNICA;
    case A_CTF: return MAX_QUEUE_CTF;
    case A_GG: return MAX_QUEUE_GG;
  }

  return 0;
}

stock bool:IsPlayerSwimming(playerid){
  new index = GetPlayerAnimationIndex(playerid);
  if ((index >=  1538 && index <= 1541) || index == 1544) return true;

  return false;
}

bool:IsPlayerNearWater(playerid){
  new vehid=GetPlayerVehicleID(playerid);
  if (vehid>0 && vehid!=INVALID_VEHICLE_ID)
    switch(GetVehicleModel(vehid)) {
      case 472,473,493,595,484,430,453,452,446,454,539,447,460:
        return true;
    }
  // nie jest w pojezdzie

  new index = GetPlayerAnimationIndex(playerid);
  if ((index >=  1538 && index <= 1541) || index == 1544) return true;

  new Float:P[3],Float:ElevDelta;

  GetPlayerPos(playerid,P[0],P[1],P[2]);
  if (P[2]<-2 || P[2]>41) return false; // kolo tamy

  MapAndreas_FindZ_For2DCoord(P[0],P[1],ElevDelta);
  if (P[2]-ElevDelta>9) return false;
  if (P[2]<(ElevDelta-2) && (P[2]<-2 || P[2]>2))  // gracz jest pod najwyzszym punktem i nie na poziomie wody
    return false;
  if (P[2]>=-2 && P[2]<=2) return true;
  // todo czy obok jest punkt o wysokosci rowno 0.0000

  return false;
}

stock IsPlayerInArea(playerid, Float:minx, Float:maxx, Float:miny, Float:maxy)
{
  new Float:x, Float:y, Float:z;
  GetPlayerPos(playerid, x, y, z);
  if (x > minx && x < maxx && y > miny && y < maxy) return 1;
  return 0;
}

bool:IsNatatorialVehicleModel(vehiclemodel){
  switch(vehiclemodel){
    case 472,473,493,595,484,430,453,452,446,454,460:
                return true;
  }
  return false;
}

bool:IsBikeVehicle(vehiclemodel){
  switch(vehiclemodel){
    case 481,509,510: // same rowery
      return true;
  }
  return false;
}

spawnVehicleNearPlayer(playerid,vehicleid){
  if (GetPlayerInterior(playerid)!=0 || GetPlayerVirtualWorld(playerid)!=0)
    return INVALID_VEHICLE_ID;
  new Float:P[4],vehid;
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  GetPlayerFacingAngle(playerid,P[3]);
  vehicles_GetSpawnOffsetByModel(vehicleid,P[0],P[1],P[2]);
  vehid=CreateVehicle(vehicleid, P[0],P[1],P[2],P[3],-1,-1,600);
  tVehicleSpawned[vehid]=true;

  SetVehicleParamsForPlayer(vehid, playerid, 0, DOOR_OPENED);
  vehicleDoorState[vehid] = DOOR_OPENED;
  vehicleDoorOwner[vehid] = playerid;

  vehicleHasNitro[vehid]=false;
  tVehicles[vehid][vo_hasTurbo]=false;
  SetVehicleHealth(vehid, VEHICLE_DEFAULT_HP);
  return vehid;
}

public spawnVehicleForPlayer(playerid,vehicleid,bool:replace){
  if (GetPlayerVirtualWorld(playerid)!=0 && !IsGM(playerid))
    return INVALID_VEHICLE_ID;

  if (GetPlayerInterior(playerid)!=0) {
    if (pData[playerid][vipEnabled] && IsBikeVehicle(vehicleid))
      { }
    else if (IsGM(playerid)) { }
    else
      return INVALID_VEHICLE_ID;
  }

  if (!IsGM(playerid) && IsPlayerInNoDMArea(playerid)) {
    Msg(playerid,COLOR_INFO2,"Nie mozna spawnowac pojazdow w strefie bez DM.");
    return INVALID_VEHICLE_ID;
  }

  if(IsPlayerUsingParachute(playerid)){
      Msg(playerid,COLOR_INFO2,"Nie mozna spawnowac pojazdow podczas lotu spadochronem.");
    return INVALID_VEHICLE_ID;
  }

    new Float:P[4],vehid;
    GetPlayerPos(playerid,P[0],P[1],P[2]);
  if (IsPlayerInAnyVehicle(playerid))
    GetVehicleZAngle(GetPlayerVehicleID(playerid),P[3]);
  else
    GetPlayerFacingAngle(playerid,P[3]);

  if (IsNatatorialVehicleModel(vehicleid) && !IsPlayerNearWater(playerid))
    return INVALID_VEHICLE_ID;

    if (replace && GetPlayerVehicleSeat(playerid)==0) {
    vehid=GetPlayerVehicleID(playerid);
    RemovePlayerFromVehicle(playerid);
    RespawnVehicle(vehid);
    }

  vehicles_GetSpawnOffsetByModel(vehicleid,P[0],P[1],P[2]);

    vehid=CreateVehicle(vehicleid, P[0],P[1],P[2],P[3],-1,-1,600);

    tVehicleSpawned[vehid]=true;

  SetVehicleParamsForPlayer(vehid, playerid, 0, DOOR_OPENED);
  vehicleDoorState[vehid] = DOOR_OPENED;
  vehicleDoorOwner[vehid] = playerid;

  vehicleHasNitro[vehid]=false;
  tVehicles[vehid][vo_hasTurbo]=false;

    LinkVehicleToInterior(vehid,GetPlayerInterior(playerid));
  SetVehicleVirtualWorld(vehid,GetPlayerVirtualWorld(playerid));
    PutPlayerInVehicle(playerid,vehid,0);

  SetVehicleHealth(vehid, VEHICLE_DEFAULT_HP);

  if (GetVehicleModel(vehid)==594)    // doniczka
    SetPlayerArmedWeapon(playerid,0);
  if(!pTemp[playerid][weaponsAllowed] || IsPlayerInNoDMArea(playerid)) SetPlayerArmedWeapon(playerid,0);
  if(!IsFreeTime()) SetPlayerArmedWeapon(playerid,0);

  SetTimerEx("OnPlayerStateChange",1500,false,"ddd",playerid, 2, 1);

  return vehid;
}

stock RemovePlayerWeapon(playerid, weaponid)
{
  new plyWeapons[12];
  new plyAmmo[12];


  for(new slot = 0; slot != 12; slot++)
  {
    new wep, ammo;
    GetPlayerWeaponData(playerid, slot, wep, ammo);

    if(wep != weaponid)
      GetPlayerWeaponData(playerid, slot, plyWeapons[slot], plyAmmo[slot]);
  }

  pTemp[playerid][disableWeaponCheck]=true;

  ResetPlayerWeapons(playerid);
  for(new slot = 0; slot != 12; slot++)
  {
    GivePlayerWeapon(playerid, plyWeapons[slot], plyAmmo[slot]);
  }
  pTemp[playerid][disableWeaponCheck]=false;
}

stock RemoveDangerousWeapon(playerid)
{
  new plyWeapons[12];
  new plyAmmo[12];

  for(new slot = 0; slot != 12; slot++)
  {
    new wep, ammo;
    GetPlayerWeaponData(playerid, slot, wep, ammo);
    switch(wep){
      case 0,35,36,37,38,39,40,44,45: {
      }
      default:
        GetPlayerWeaponData(playerid, slot, plyWeapons[slot], plyAmmo[slot]);
    }
  }
  pTemp[playerid][disableWeaponCheck]=true;
  ResetPlayerWeapons(playerid);

  for(new slot = 0; slot != 12; slot++)
    GivePlayerWeapon(playerid, plyWeapons[slot], plyAmmo[slot]);

  pTemp[playerid][disableWeaponCheck]=false;
}

stock GetVehicleFlags(vehicleid){
  new vm=GetVehicleModel(vehicleid);
  new r;

  // airborne, airplanes
  switch (vm){
    case 425,548,417,487,497,563,469,447,488: // sea sparrow
    {
      r+=VF_AIRBORNE;
      r+=VF_HELICOPTER;
    }
    case 460,476,511,512,520,593,592,553,519,513,577: // samoloty
    {
      r+=VF_AIRBORNE;
      r+=VF_AIRPLANE;
    }
    case 539:               // vortex
      r+=VF_AIRBORNE;
  }

  // wojskowe
  switch(vm){
    case 425,520,432: // hunter, hydra, rhino
      r+=VF_MILITARY;
  }
  // plywajace
  switch(vm){
    case 472,473,493,595,484,430,453,452,446,454, // lodzie
      539,    // vortex
      447,460:  // sea sparrow, skimmer
      r+=VF_NATATORIAL;
  }
  // zdalnie sterowane
  switch(vm){
    case 441,464,465,501,564,594: r+=VF_RC;
  }
  // motory i rowery
  switch(vm){
    case 471,468,586,463,523,521,461,522,581,448,462,510,481,509:
      r+=VF_BIKES;
  }
  // drogowe
  if (vm==539 ||            // vortex
    (r&VF_NATATORIAL!=VF_NATATORIAL &&  // lodzie
    r&VF_AIRBORNE!=VF_AIRBORNE)   // lotnicze
    )
      r+=VF_STREET;

  // mozliwe do holowania
  switch(vm){
    case 437,544,431,407,408,   // coach, fire truck, bus, fire truck, trashmaster
      570,569,538,537,449,    // przyczepy do pociagow, pociagi, tram
      532,524,          // kombajn, cement truck
      403,514,515,443,      // tiry, packer,
      435,            // przyczepy
      433,432,          // barracks, rhino
      406:            // dumper
    { }

    default:
      if (
          r&VF_NATATORIAL!=VF_NATATORIAL  &&  // lodzie
          r&VF_AIRBORNE!=VF_AIRBORNE  &&    // lotnicze
          r&VF_RC!=VF_RC        &&    // zdalnie sterowane
          r&VF_BIKES!=VF_BIKES
      )   r+=VF_TOWABLE;

  }
  
  // police
  switch(vm){
    case 416,523,427,490,407,544,596,598,597,599:
      r+=VF_POLICE;
  }
  return r;
}

stock GetVehicleFlagsByModel(vm){
  new r;

  // airborne, airplanes
  switch (vm){
    case 425,548,417,487,497,563,469,447: // sea sparrow
    {
      r+=VF_AIRBORNE;
      r+=VF_HELICOPTER;
    }
    case 460,511,512,520,593,553,519,513: // samoloty
    {
      r+=VF_AIRBORNE;
      r+=VF_AIRPLANE;
    }
    case 539:               // vortex
      r+=VF_AIRBORNE;
  }

  // wojskowe
  switch(vm){
    case 425,520,432: // hunter, hydra, rhino
      r+=VF_MILITARY;
  }
  // plywajace
  switch(vm){
    case 472,473,493,595,484,430,453,452,446,454, // lodzie
      539,    // vortex
      447,460:  // sea sparrow, skimmer
      r+=VF_NATATORIAL;
  }
  // zdalnie sterowane
  switch(vm){
    case 441,464,465,501,564,594: r+=VF_RC;
  }
  // motory i rowery
  switch(vm){
    case 471,468,586,463,523,521,461,522,581,448,462,510,481,509:
      r+=VF_BIKES;
  }
  // drogowe
  if (vm==539 ||            // vortex
    (r&VF_NATATORIAL!=VF_NATATORIAL &&  // lodzie
    r&VF_AIRBORNE!=VF_AIRBORNE)   // lotnicze
    )
      r+=VF_STREET;

  // mozliwe do holowania
  switch(vm){
    case 437,544,431,407,408,   // coach, fire truck, bus, fire truck, trashmaster
      570,569,538,537,449,    // przyczepy do pociagow, pociagi, tram
      532,524,          // kombajn, cement truck
      403,514,515,443,      // tiry, packer,
      435,            // przyczepy
      433,432,          // barracks, rhino
      406:            // dumper
    { }
    default:
      if (
          r&VF_NATATORIAL!=VF_NATATORIAL  &&  // lodzie
          r&VF_AIRBORNE!=VF_AIRBORNE  &&    // lotnicze
          r&VF_RC!=VF_RC        &&    // zdalnie sterowane
          r&VF_BIKES!=VF_BIKES
      )   r+=VF_TOWABLE;

  }
  
  // police
  switch(vm){
    case 416,523,427,490,407,544,596,598,597,599:
      r+=VF_POLICE;
  }
  return r;
}

stock bool:IsPlayerInAirVehicle(playerid){
  if (!IsPlayerInAnyVehicle(playerid)) return false;
  new vm=GetVehicleModel(GetPlayerVehicleID(playerid));
  switch (vm){
    case 548,417,487,497,563,469, 511,512,593,553,519,460,513:
      return true;
    default:
      return false;
  }
  return false;
}

SyncPlayerGameTime(playerid){
  if(gmTemp[changed_time]>-1) {
      SetPlayerTime(playerid, gmTemp[changed_time], 0);
  }else{
      new sHour, sMinute, sSecond;
    gettime(sHour, sMinute, sSecond);
    SetPlayerTime(playerid, sHour, 0);
  }
  return;
}
SyncPlayerWeather(playerid){
  if (GetPlayerInterior(playerid)==0)
    SetPlayerWeather(playerid,gmTemp[currentWeather]);
  return;
}

bool:SkinKobiecy(s){ // 53 usuniety bo czesto go uzywaja
    switch(s){
        case 9,10..13,31,38..41, 54..56, 63, 64, 69, 75..77, 85, 87, 88..93,129, 130, 131,138..141,145,148,150..152,157,169,172,178,190..199,201,205,207,211,214,215,
            216,218,219,224..226,231,232,233,237,238,243,244..246, 251,256,257,263,298,306..309:
            return true;
    }
    return false;
}

bool:Kobieta(playerid)
  return SkinKobiecy(GetPlayerSkin(playerid));


SaveReturnPath(playerid,destint){
  retpos[playerid][tpDestInt]=destint;
  retpos[playerid][tpWorld]=GetPlayerVirtualWorld(playerid);
  retpos[playerid][tpInterior]=GetPlayerInterior(playerid);
  retpos[playerid][tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;
  new Float:PP[3],Float:VV[3];
  GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
  GetPlayerVelocity(playerid,VV[0],VV[1],VV[2]);
  retpos[playerid][tpX]=PP[0]-(VV[0]*3);
  retpos[playerid][tpY]=PP[1]-(VV[1]*3);
  retpos[playerid][tpZ]=PP[2]-(VV[2]*1.5);
  pTemp[playerid][pickupDelay]=GetTickCount();
  GetPlayerFacingAngle(playerid,retpos[playerid][tpA]);
  retpos[playerid][tpA]-=180;
  return 1;
}

UseReturnPath(playerid){
  if (GetPlayerInterior(playerid)!=retpos[playerid][tpDestInt]) // nie mamy informacji gdzie wracac, prawdpodobnie gracz przeteleportowal sie do innego interiora
    return Msg(playerid,COLOR_ERROR,"Zamkniete!");
  else if (retpos[playerid][tpX]==retpos[playerid][tpX]) {
    pTemp[playerid][pickupDelay]=GetTickCount();
    Teleport(T_PLAYER, playerid, retpos[playerid][tpX],retpos[playerid][tpY],retpos[playerid][tpZ], retpos[playerid][tpA], retpos[playerid][tpInterior], retpos[playerid][tpWorld], retpos[playerid][tpStreamed]);
    Audio_StopBGSound(playerid);
  }
  return 1;
}

GetPlayerWeaponList(playerid,uid){
  new buf[127];
  if (!IsPlayerConnected(uid)) return;
  if (!IsPlayerSpawned(uid)) return;
  new wepid,ammo;
  new warn=0;
  new clr;
  for (new slot=0; slot<=12;slot++) {
    GetPlayerWeaponData(uid,slot,wepid,ammo);
    if (ammo==0) continue;
    warn=0;
    if (wepid<=15 && ammo>1) warn++;
    if (wepid<=15 && ammo>10) warn++;
    if (ammo<0) warn++;
    if (!pTemp[uid][wStrefieFULLDM]) {
        if (wepid==38 && pData[uid][pAttraction]!=A_CHOWANY) warn++;  // minigun
        else if (wepid==37) warn++;
        else if (wepid==35) warn++;
        else if (wepid==39) warn++;
    }
    if (wepid==36 || wepid==44 || wepid==45) // hs rpg, gogle
      warn=warn+2;
    // max warn = 3, dla bezpieczenstwa kodujemy na 4
    clr=0xff - (warn*60);
    format(buf, sizeof buf, "%s {ff%02x%02x}%d-%s(%d)", buf, clr, clr, wepid, weaponName[wepid], ammo);
    if (strlen(buf)>100) {
      SCM(playerid,-1,buf);
      format(buf, sizeof buf, "");
    }
  }
  if (strlen(buf)>0)
    SCM(playerid,-1,buf);
  return;
}

stock strtok(const string[], &index,seperator=' ')
{
  new length = strlen(string);
  new offset = index;
  new sresult[128];
  while ((index < length) && (string[index] != seperator) && ((index - offset) < (sizeof(sresult) - 1)))
  {
    sresult[index - offset] = string[index];
    index++;
  }

  sresult[index - offset] = EOS;
  if ((index < length) && (string[index] == seperator))
  {
    index++;
  }
  return sresult;
}

GetPlayerFPS(playerid)
{
  pTemp[playerid][drunkLevel]=GetPlayerDrunkLevel(playerid);
  if(pTemp[playerid][drunkLevel]<100)
    SetPlayerDrunkLevel(playerid, 2000);
  else{
    if(pTemp[playerid][drunkLevelL]!=pTemp[playerid][drunkLevel]) {
      if(IsPlayerSpawned(playerid))
      {
        new FPS=pTemp[playerid][drunkLevelL]-pTemp[playerid][drunkLevel];
        pTemp[playerid][drunkLevelL]=pTemp[playerid][drunkLevel];
        pTemp[playerid][ept_fps]=((pTemp[playerid][ept_fps]>1?pTemp[playerid][ept_fps]:FPS)+FPS-1)/2;
        return FPS-1;
      }else{
        return pTemp[playerid][ept_fps]; // return old fps because drunk level is not accurate when player is not spawned!
      }
    }
  }
  return 0;
}

FindUIDByNameOrID(params[],&uid,playerid=INVALID_PLAYER_ID){
  uid = FindPlayer(params);
  new buffer[128];
  if(uid == ERROR_NOT_FOUND)
  {
    if(playerid!=INVALID_PLAYER_ID) {
      format(buffer, sizeof buffer, __("Nie znaleziono gracza o nicku lub ID '{b}%s{/b}'."), params);
      Msg(playerid, COLOR_ERROR, buffer);
    }
    uid=INVALID_PLAYER_ID;
    return 0;

  }
  if(uid == ERROR_FOUND_MULTIPLE)
  {
    if(playerid!=INVALID_PLAYER_ID) {
      format(buffer, sizeof buffer, __("Znaleziono wiecej niz jednego gracza z fraza '{b}%s{/b}'."), params);
      Msg(playerid, COLOR_ERROR, buffer);
    }
    uid=INVALID_PLAYER_ID;
    return 0;
  }
  return 1;
}

stock GetDotXY(Float:StartPosX, Float:StartPosY, &Float:NewX, &Float:NewY, Float:alpha, Float:dist)
{
   NewX = StartPosX + (dist * floatsin(alpha, degrees));
   NewY = StartPosY + (dist * floatcos(alpha, degrees));
}

forward RespawnPlayer(playerid);
public RespawnPlayer(playerid){
  TogglePlayerSpectating(playerid,0);
  return 1;
}

forward DeathSpec(playerid,killerid);
public DeathSpec(playerid,killerid){
  switch(GetPlayerState(killerid)) {
    case PLAYER_STATE_WASTED,PLAYER_STATE_SPECTATING,PLAYER_STATE_NONE: { return 1; }
  }
  TogglePlayerSpectating(playerid,1);
  if(IsPlayerInAnyVehicle(killerid)) {
      new vid=GetPlayerVehicleID(killerid);
    PlayerSpectateVehicle(playerid, vid);
  }else
    PlayerSpectatePlayer(playerid,killerid);
  SetTimerEx("RespawnPlayer",3000,false,"d",playerid);
  return 1;
}

forward clearPlayerBombs(playerid);
public clearPlayerBombs(playerid){
  if(pTemp[playerid][bombId]==-1) return 1;

  DestroyPickup(pTemp[playerid][bombId]);
  pTemp[playerid][bombPos][0]=-1;
  pTemp[playerid][bombPos][1]=-1;
  pTemp[playerid][bombPos][2]=-1;
  pTemp[playerid][bombId]=-1;
  KillTimer(pTemp[playerid][bombTimer]);
  return 1;
}
// EOF