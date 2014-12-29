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
#define FILTERSCRIPT
#include <3rdparty/dc_cmd>
#include <3rdparty/streamer>
#include <3rdparty/sscanf2>
#include <3rdparty/a_mysql>

new racecpso=10;

new lastpick;

CMD:racecp(playerid,params[]){
    new Float:PX[3],buf[512],aid;
    if (sscanf(params,"d",aid))
        return SendClientMessage(playerid,0xff0000ff,"/raceaddcp {b}<race id>{/b}");
  if (!IsPlayerInAnyVehicle(playerid))
     GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  else
      GetVehiclePos(GetPlayerVehicleID(playerid),PX[0],PX[1],PX[2]);

    format(buf,sizeof buf,"INSERT INTO fs_races_cp SET aid=%d,X=%.4f,Y=%.4f,Z=%.4f,type=0,size=8,so=%d",
                aid, PX[0], PX[1], PX[2], racecpso);
  printf(buf);
  mysql_query(buf);
  racecpso+=10;
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:driftcp(playerid,params[]){
    new Float:PX[3],buf[512],aid;
    if (sscanf(params,"d",aid))
        return SendClientMessage(playerid,0xff0000ff,"/driftcp {b}<drift id>{/b}");
  if (!IsPlayerInAnyVehicle(playerid))
     GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  else
      GetVehiclePos(GetPlayerVehicleID(playerid),PX[0],PX[1],PX[2]);

    format(buf,sizeof buf,"INSERT INTO fs_drift_tracks_cp SET aid=%d,X=%.4f,Y=%.4f,Z=%.4f,type=0,size=8,so=%d",
                aid, PX[0], PX[1], PX[2], racecpso);
  printf(buf);
  mysql_query(buf);
  racecpso+=10;
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:racesp(playerid,params[]){
    new Float:PX[4],buf[512],aid;
    if (sscanf(params,"d",aid))
        return SendClientMessage(playerid,0xff0000ff,"/racesp {b}<race id>{/b}");
  if (IsPlayerInAnyVehicle(playerid)) {
     GetVehiclePos(GetPlayerVehicleID(playerid),PX[0],PX[1],PX[2]);
     GetVehicleZAngle(GetPlayerVehicleID(playerid),PX[3]);
  } else {
    GetPlayerPos(playerid, PX[0], PX[1],PX[2]);
    GetPlayerFacingAngle(playerid, PX[3]);
  }

    format(buf,sizeof buf,"INSERT INTO fs_races_sp SET aid=%d,X=%.4f,Y=%.4f,Z=%.4f,angle=%.1f",                aid, PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  CreateDynamicPickup(1318,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  SendClientMessage(playerid,0xffffffff,"dodano sp");
  Streamer_Update(playerid);
  return 1;
}

CMD:driftsp(playerid,params[]){
    new Float:PX[4],buf[512],aid;
    if (sscanf(params,"d",aid))
        return SendClientMessage(playerid,0xff0000ff,"/driftsp {b}<drift id>{/b}");
  if (IsPlayerInAnyVehicle(playerid)) {
     GetVehiclePos(GetPlayerVehicleID(playerid),PX[0],PX[1],PX[2]);
     GetVehicleZAngle(GetPlayerVehicleID(playerid),PX[3]);
  } else {
    GetPlayerPos(playerid, PX[0], PX[1],PX[2]);
    GetPlayerFacingAngle(playerid, PX[3]);
  }

    format(buf,sizeof buf,"INSERT INTO fs_drift_tracks_sp SET aid=%d,X=%.4f,Y=%.4f,Z=%.4f,angle=%.1f",                aid, PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  CreateDynamicPickup(1318,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  SendClientMessage(playerid,0xffffffff,"dodano sp");
  Streamer_Update(playerid);
  return 1;
}

CMD:derbysp(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
    new Float:PX[4],buf[512],aid;
    if (sscanf(params,"d",aid))
        return SendClientMessage(playerid,0xff0000ff,"/derbysp {b}<arena id>{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  GetPlayerFacingAngle(playerid,PX[3]);

    format(buf,sizeof buf,"INSERT INTO fs_derby_arena_sp SET aid=%d,X=%.4f,Y=%.4f,Z=%.4f,angle=%.2f",
                aid, PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:wgsp(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"dd",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/wgsp {b}<arena id> {team 0/1}{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  GetPlayerFacingAngle(playerid,PX[3]);

    format(buf,sizeof buf,"INSERT INTO fs_wg_arena_sp SET aid=%d,team=%d,X=%.4f,Y=%.4f,Z=%.4f,A=%.2f",
                aid, team,PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:ctfflaga(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"d",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/ctfflaga {b}<arena id>{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);

    format(buf,sizeof buf,"UPDATE `fullserver`.`fs_ctf_arena` SET `flaga_spawn` = '%f,%f,%f' WHERE `fs_ctf_arena`.`id` =%d;",
                PX[0], PX[1], PX[2],aid);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:ctfteam0(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"d",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/ctfteam0 {b}<arena id>{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);

    format(buf,sizeof buf,"UPDATE `fullserver`.`fs_ctf_arena` SET `flaga_team0` = '%f,%f,%f' WHERE `fs_ctf_arena`.`id` =%d;",
                PX[0], PX[1], PX[2],aid);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:ctfteam1(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"d",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/ctfteam1 {b}<arena id>{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);

    format(buf,sizeof buf,"UPDATE `fullserver`.`fs_ctf_arena` SET `flaga_team1` = '%f,%f,%f' WHERE `fs_ctf_arena`.`id` =%d;",
                PX[0], PX[1], PX[2], aid);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:ctfsp(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"dd",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/ctfsp {b}<arena id> {team 0 - zielony/1 - niebieski}{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  GetPlayerFacingAngle(playerid,PX[3]);

    format(buf,sizeof buf,"INSERT INTO fs_ctf_arena_sp SET aid=%d,team=%d,X=%.4f,Y=%.4f,Z=%.4f,A=%.2f",
                aid, team,PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:adsp(playerid,params[]){
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"dd",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/adsp {b}<arena id> {team 0-atak 1-obrona}{/b}");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  GetPlayerFacingAngle(playerid,PX[3]);

    format(buf,sizeof buf,"INSERT INTO fs_bazyad_sp SET aid=%d,team=%d,X=%.4f,Y=%.4f,Z=%.4f,A=%.2f",
                aid, team,PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:chsp(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
  if (IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid,0xff0000ff,"Wyjdz z pojazdu");
    new Float:PX[4],buf[512],aid,team;
    if (sscanf(params,"dd",aid,team))
        return SendClientMessage(playerid,0xff0000ff,"/chsp {b}<arena id> {0/1}{/b}      1 szukajacy 0 chowajacy sie");
    GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  GetPlayerFacingAngle(playerid,PX[3]);

    format(buf,sizeof buf,"INSERT INTO fs_chowany_arena_sp SET aid=%d,team=%d,X=%.4f,Y=%.4f,Z=%.4f,A=%.2f",
                aid, team,PX[0], PX[1], PX[2], PX[3]);
  printf(buf);
  mysql_query(buf);
  SendClientMessage(playerid,0xffffffff,buf);
  CreateDynamicPickup(1316,1,PX[0],PX[1],PX[2],        -1,-1,-1,100.0);
  Streamer_Update(playerid);
  return 1;
}

CMD:taddpick(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
    new Float:PX[3],pid;
    if (sscanf(params,"d",pid))
        return SendClientMessage(playerid,0xff0000ff,"/taddpick {b}<pid>{/b}");
  GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  lastpick=CreateDynamicPickup(pid,19,PX[0],PX[1],PX[2], -1,-1,-1,100.0);
  return 1;
}

CMD:taddpick2(playerid,params[]){
  if (!IsPlayerAdmin(playerid)) return 0;
    new Float:PX[3],pid;
    if (sscanf(params,"d",pid))
        return SendClientMessage(playerid,0xff0000ff,"/taddpick {b}<pid>{/b}");
  GetPlayerPos(playerid,PX[0],PX[1],PX[2]);
  lastpick=CreateDynamicPickup(pid,13,PX[0],PX[1],PX[2]+20, -1,-1,-1,100.0);
  return 1;
}

public OnPlayerPickUpDynamicPickup(playerid,pickupid){
  if (lastpick!=pickupid) return 0;
  SendClientMessageToAll(-1, "PICK UP");
  return 1;
}
// EOF
