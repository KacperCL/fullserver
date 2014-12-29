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

#define MAX_HOUSES 380
#define DIALOG_DOMY   3137
#define DIALOG_DOMY_OUTSIDEPICKUP (DIALOG_DOMY+0)
#define DIALOG_DOMY_CZYNSZ  (DIALOG_DOMY+1)
#define DIALOG_DOMY_CZYNSZ_WYBOR  (DIALOG_DOMY+6)
#define DIALOG_DOMY_CZYNSZ_CONFIRM  (DIALOG_DOMY+2)
#define DIALOG_DOMY_INFO      (DIALOG_DOMY+4)
#define DIALOG_DOMY_SPRZEDAZ    (DIALOG_DOMY+5)

enum e_domy{
  ed_id,
  ed_pickup,
  ed_cp,
  Text3D:ed_3dtext,
  ed_mapicon,
  ed_koszt,
  ed_owner,
  ed_ownernick[32],
  Float:ed_entrance[3], // XYZ
  Float:ed_exit[4],   // XYZA
  Float:ed_home[4],   // XYZA
  ed_homei,       // interior
  ed_homevw,        // vw - domyslnie VW_DOMY+ID
  ed_audioURL[128],

  ed_daysLeft,
  Float:ed_paidTo[12],
  bool:ed_open,
  ed_vehiclesallowed,
  Float:ed_vehicleradius,
  bool:ehv_active,
  ehv_id,
  ehv_model,
  Float:ehv_spawnPos[4],
  ehv_color[2],
  ehv_plate[32],
  ehv_components[14],

  ed_restrict_gang
}

new FSDomy[MAX_HOUSES][e_domy];
new IloscDomow=0;

domy_Reload(){
  new dane[128];
  format(dane,sizeof dane,"SELECT * from fs_view_housesandvehicles LIMIT %d",MAX_HOUSES);
  mysql_query(dane);
  domy_OnMysqlQuery(SQL_RI_DOMY_LISTA,0);

  foreach(playerid)
      if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID])
      domy_OnHouseOwnerLogin(playerid,false);
}

domy_OnMysqlQuery(resultid,spareid){
  #pragma unused spareid
  if(resultid!=SQL_RI_DOMY_LISTA) return 0;
  new dane[1024],Float:DX,Float:DY,Float:DZ,opis[127];
  mysql_store_result();
  new i=0;
  if (mysql_num_rows()>0)
  while(mysql_fetch_row_format(dane,"|")) {
    new vcomp[80];
    sscanf(dane,"p<|>dfffffffdds[32]s[11]dffffiiifD(0)F(0)F(0)F(0)F(0)D(0)D(0)S(-)[32]S(-)[128]S(-)[80]D(0)",FSDomy[i][ed_id],DX,DY,DZ,
              FSDomy[i][ed_exit][0], FSDomy[i][ed_exit][1], FSDomy[i][ed_exit][2], FSDomy[i][ed_exit][3],
              FSDomy[i][ed_owner],FSDomy[i][ed_koszt],FSDomy[i][ed_ownernick], FSDomy[i][ed_paidTo], FSDomy[i][ed_daysLeft],
              FSDomy[i][ed_home][0], FSDomy[i][ed_home][1], FSDomy[i][ed_home][2], FSDomy[i][ed_home][3],
              FSDomy[i][ed_homei], FSDomy[i][ed_homevw], FSDomy[i][ed_vehiclesallowed], FSDomy[i][ed_vehicleradius],
              FSDomy[i][ehv_model], FSDomy[i][ehv_spawnPos][0],  FSDomy[i][ehv_spawnPos][1], FSDomy[i][ehv_spawnPos][2], FSDomy[i][ehv_spawnPos][3],
              FSDomy[i][ehv_color][0], FSDomy[i][ehv_color][1], FSDomy[i][ehv_plate],
              FSDomy[i][ed_audioURL], vcomp, FSDomy[i][ed_restrict_gang]
              );

    if (FSDomy[i][ed_homevw]<0) FSDomy[i][ed_homevw]=VW_UNIQUE_HOUSE+FSDomy[i][ed_id];
    sscanf(vcomp,"p<:>dddddddddddddd", FSDomy[i][ehv_components][0], FSDomy[i][ehv_components][1], FSDomy[i][ehv_components][2], FSDomy[i][ehv_components][3],
      FSDomy[i][ehv_components][4], FSDomy[i][ehv_components][5], FSDomy[i][ehv_components][6], FSDomy[i][ehv_components][7],
      FSDomy[i][ehv_components][8], FSDomy[i][ehv_components][9], FSDomy[i][ehv_components][10], FSDomy[i][ehv_components][11],
      FSDomy[i][ehv_components][12], FSDomy[i][ehv_components][13]);

    if (FSDomy[i][ehv_model]>0)
      FSDomy[i][ehv_active]=true;
    else
      FSDomy[i][ehv_active]=false;

    if (FSDomy[i][ehv_active] && FSDomy[i][ehv_id]!=INVALID_VEHICLE_ID && GetVehicleModel(FSDomy[i][ehv_id])!=FSDomy[i][ehv_model]) { // todo sprawdzanie modelu?
      tVehicles[FSDomy[i][ehv_id]][vo_private]=false;
      tVehicles[FSDomy[i][ehv_id]][vo_owningPlayerId]=INVALID_PLAYER_ID;
      FSDomy[i][ehv_id]=INVALID_VEHICLE_ID;
    }

    if (IsValidDynamicPickup(FSDomy[i][ed_pickup])) {
      DestroyDynamicPickup(FSDomy[i][ed_pickup]);
      FSDomy[i][ed_pickup]=-1;
    }
    if (IsValidDynamicCP(FSDomy[i][ed_cp])) {
      DestroyDynamicCP(FSDomy[i][ed_cp]);
      FSDomy[i][ed_cp]=-1;
    }
    if (IsValidDynamic3DTextLabel(FSDomy[i][ed_3dtext]))  {
      DestroyDynamic3DTextLabel(FSDomy[i][ed_3dtext]);
      FSDomy[i][ed_3dtext]=Text3D:-1;
    }

    if (FSDomy[i][ed_daysLeft]<0) FSDomy[i][ed_owner]=0;

    if (FSDomy[i][ed_owner]>0 && FSDomy[i][ed_daysLeft]>=0) {
      format(opis,sizeof opis,"%s", FSDomy[i][ed_ownernick], FSDomy[i][ed_id]);
    } else {

      if (FSDomy[i][ed_restrict_gang]==NO_GANG)
        format(opis,sizeof opis,"NA SPRZEDAZ{AAFFAA}");
      else
        format(opis,sizeof opis,"NA SPRZEDAZ\ntylko {%06x}%s{AAFFAA}", gData[FSDomy[i][ed_restrict_gang]][gColor], gData[FSDomy[i][ed_restrict_gang]][tag]);

      if (FSDomy[i][ed_koszt]==0)
        format(opis,sizeof opis,"%s\nTylko GamePoints",opis);
      else
        format(opis,sizeof opis,"%s\n%d respektu na dobe lub 7,38 SMSem na miesiac", opis, FSDomy[i][ed_koszt]);
      format(opis,sizeof opis,"%s\n{F4FF29}www.fullserver.eu/domy/%d", opis, FSDomy[i][ed_id]);
    }

    if (FSDomy[i][ed_exit][0]==0 && FSDomy[i][ed_exit][1]==0)
      format(opis, sizeof opis,"%s\nBEZ WYJSCIA", opis);

    FSDomy[i][ed_3dtext]=CreateDynamic3DTextLabel(opis,0xf0aaf0ff,DX,DY,DZ+0.5,25,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,1,0,0);

    FSDomy[i][ed_pickup]=CreateDynamicPickup(
          ((FSDomy[i][ed_owner]>0)?PICKUP_HOUSE_PURCHASED:PICKUP_HOUSE_NOTPURCHASED),1, DX,DY,DZ, 0,0, -1, 50.0);
    FSDomy[i][ed_cp]=CreateDynamicCP(DX,DY,DZ,1.5,  0,0, -1, 5.0);


    FSDomy[i][ed_entrance][0]=DX;
    FSDomy[i][ed_entrance][1]=DY;
    FSDomy[i][ed_entrance][2]=DZ;

    i++;
  }
  mysql_free_result();
  new OldIloscDomow=IloscDomow;
  IloscDomow=i;
  for (i=IloscDomow;i<OldIloscDomow;i++) // jesli zmniejszyla sie ilosc domow, to tutaj usuwamy nadmiar
    if (FSDomy[i][ed_id]>0) {
      if (IsValidDynamicPickup(FSDomy[i][ed_pickup])) {
        DestroyDynamicPickup(FSDomy[i][ed_pickup]);
        FSDomy[i][ed_pickup]=-1;
      }
      if (IsValidDynamic3DTextLabel(FSDomy[i][ed_3dtext]))  {
        DestroyDynamic3DTextLabel(FSDomy[i][ed_3dtext]);
        FSDomy[i][ed_3dtext]=Text3D:-1;
      }
      FSDomy[i][ed_id]=0;
    }

  return IloscDomow++;
}

domy_SpawnVehicle(playerid,Float:pX=-1.0, Float:pY=-1.0, Float:pZ=-1.0, Float:pA=-1.0){
  new hid=pTemp[playerid][e_houseid];
  if (hid<0) return 0;
  if (!(FSDomy[hid][ehv_model]>=400))
    return 0;
  // todo usuniecie starego?

  if (FSDomy[hid][ehv_id]!=INVALID_VEHICLE_ID && tVehicles[FSDomy[hid][ehv_id]][vo_private] && tVehicles[FSDomy[hid][ehv_id]][vo_houseid]==hid && GetVehicleModel(FSDomy[hid][ehv_id])==FSDomy[hid][ehv_model]) {
  } else {
    if (FSDomy[hid][ehv_id]!=INVALID_VEHICLE_ID && FSDomy[hid][ehv_id]>0) {
      tVehicles[FSDomy[hid][ehv_id]][vo_private]=false;
      tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]=INVALID_PLAYER_ID;
      tVehicles[FSDomy[hid][ehv_id]][vo_houseid]=-1;
      tVehicles[FSDomy[hid][ehv_id]][vo_destroyIfEmpty]=true;
      tVehicles[FSDomy[hid][ehv_id]][vo_licensePlateSet]=false;
      tVehicles[FSDomy[hid][ehv_id]][vo_color][0]=-1;
      tVehicles[FSDomy[hid][ehv_id]][vo_color][1]=-1;
      tVehicles[FSDomy[hid][ehv_id]][vo_static]=false;
    }
    new cvid=CreateVehicle(FSDomy[hid][ehv_model], pX==-1 ? FSDomy[hid][ehv_spawnPos][0] : pX,  pY==-1 ? FSDomy[hid][ehv_spawnPos][1] : pY, pZ==-1 ? FSDomy[hid][ehv_spawnPos][2] : pZ, pA==-1 ? FSDomy[hid][ehv_spawnPos][3] : pA,
    FSDomy[hid][ehv_color][0], FSDomy[hid][ehv_color][1],4*3600);
    FSDomy[hid][ehv_id]=cvid;
    for(new i=0;i<=13;i++){
      if (FSDomy[hid][ehv_components][i]>0){
        AddVehicleComponent(cvid, FSDomy[hid][ehv_components][i]);
      }
    }
  }

  tVehicles[FSDomy[hid][ehv_id]][vo_destroyIfEmpty]=false;
  tVehicles[FSDomy[hid][ehv_id]][vo_private]=true;
  tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]=playerid;
  tVehicles[FSDomy[hid][ehv_id]][vo_houseid]=hid;
  tVehicles[FSDomy[hid][ehv_id]][vo_static]=false;
  tVehicles[FSDomy[hid][ehv_id]][vo_color][0]=FSDomy[hid][ehv_color][0];
  tVehicles[FSDomy[hid][ehv_id]][vo_color][1]=FSDomy[hid][ehv_color][1];

  if (strlen(FSDomy[hid][ehv_plate])>1) {
    SetVehicleNumberPlate(FSDomy[hid][ehv_id],FSDomy[hid][ehv_plate]);
    tVehicles[FSDomy[hid][ehv_id]][vo_licensePlateSet]=true;
  }

  new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(FSDomy[hid][ehv_id], engine, lights, alarm, doors, bonnet, boot, objective);
  SetVehicleParamsEx(FSDomy[hid][ehv_id], 1, lights, alarm, DOOR_CLOSED, bonnet, boot, objective);

  SetVehicleParamsForPlayer(FSDomy[hid][ehv_id], playerid, 0, DOOR_OPENED);

  ChangeVehicleColor(FSDomy[hid][ehv_id], FSDomy[hid][ehv_color][0], FSDomy[hid][ehv_color][1]);

  vehicleDoorState[FSDomy[hid][ehv_id]] = DOOR_CLOSED;
    vehicleDoorOwner[FSDomy[hid][ehv_id]] = playerid;

  SetVehicleHealth(FSDomy[hid][ehv_id],VEHICLE_DEFAULT_HP);
  RepairVehicle(FSDomy[hid][ehv_id]);
  return 1;

}

domy_OnHouseOwnerLogin(playerid,bool:notify=true){
  if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID]) {
    new hid=pTemp[playerid][e_houseid];
    if (!IsValidDynamicMapIcon(FSDomy[hid][ed_mapicon])) {
      FSDomy[hid][ed_mapicon]=CreateDynamicMapIcon(FSDomy[hid][ed_entrance][0],FSDomy[hid][ed_entrance][1],FSDomy[hid][ed_entrance][2],31,1,0,0,playerid,6000);
      Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, FSDomy[hid][ed_mapicon], E_STREAMER_STYLE, 1);
      if (FSDomy[pTemp[playerid][e_houseid]][ehv_active] && FSDomy[pTemp[playerid][e_houseid]][ehv_model]>0)
        domy_SpawnVehicle(playerid);

    }
    if (notify) {
      new buffer[128];
      SendClientMessage(playerid,-1," ");
      format(buffer, sizeof buffer, __("Czynsz Twojego domu jest oplacony do {b}%s{/b} ({b}%d{/b} %s)"), FSDomy[hid][ed_paidTo], FSDomy[hid][ed_daysLeft], dli(FSDomy[hid][ed_daysLeft],"dzien","dni","dni") );
      Msg(playerid,COLOR_INFO,buffer);

      new wiadomosci=poczta_nieprzeczytanych(playerid);

      if (wiadomosci>0) {
        format(buffer,sizeof buffer,"W Twojej skrzynce jest {b}%d %s{/b}.", wiadomosci, dli(wiadomosci,"nieprzeczytana wiadomosc","nieprzeczytane wiadomosci","nieprzeczytanych wiadomosci"));
        Msg(playerid,COLOR_INFO,buffer);
      }

    }
  }
  return 1;
}

domy_OnHouseOwnerDisconnects(playerid){
  if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID]) {
    new hid=pTemp[playerid][e_houseid];
    new cvid=FSDomy[hid][ehv_id];
    if (cvid>0 && cvid!=INVALID_VEHICLE_ID) {
      tVehicles[cvid][vo_private]=true;
      tVehicles[cvid][vo_owningPlayerId]=INVALID_PLAYER_ID;
      tVehicles[cvid][vo_houseid]=hid;
    }
    if (IsValidDynamicMapIcon(FSDomy[hid][ed_mapicon])) {
      DestroyDynamicMapIcon(FSDomy[hid][ed_mapicon]);
      FSDomy[hid][ed_mapicon]=-1;
    }
  }
}

domy_OPEnterDynamicCP(playerid,checkpointid){
  if (GetPlayerVirtualWorld(playerid)!=0 || GetPlayerInterior(playerid)!=0 || GetPlayerState(playerid)!=PLAYER_STATE_ONFOOT) return 0;

  for (new i=0;i<IloscDomow;i++) {
    if(FSDomy[i][ed_id]>0 && FSDomy[i][ed_cp]==checkpointid) {
      if (IsPlayerInAnyVehicle(playerid)) return 1;

      domy_PokazMenuDomu(playerid,i);

      return 1;

    }
  }
  return 0;
}

domy_tpto(playerid,domid=-1,Float:angl=0.0){
  if (domid<0) {
    if (pTemp[playerid][e_houseid]<0) return;
    domid=pTemp[playerid][e_houseid];
  }
  if (FSDomy[domid][ed_exit][0]!=0 && FSDomy[domid][ed_exit][1]!=0)
    Teleport(T_PLAYER, playerid, FSDomy[domid][ed_exit][0], FSDomy[domid][ed_exit][1], FSDomy[domid][ed_exit][2], FSDomy[domid][ed_exit][3]-angl,  0, 0, true);
  else  // do pickupu
    Teleport(T_PLAYER, playerid, FSDomy[domid][ed_entrance][0], FSDomy[domid][ed_entrance][1], FSDomy[domid][ed_entrance][2], FLOAT_NAN,  0, 0, true);
  return;
}

CMD:dodajdom(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  new opis[64],koszt;
  if (sscanf(params,"ds[64]",koszt,opis))
    return Msg(playerid,COLOR_ERROR,"Uzyj: /dodajdom {b}<koszt> <opis>{b}             Koszt: 0-GP, 1-wzwyz - ilosc respektu na dobe");
  new buffer[255],esc_opis[64],Float:PX,Float:PY,Float:PZ;
  GetPlayerPos(playerid,PX,PY,PZ);
  mysql_real_escape_string(opis,esc_opis);
  format(buffer,sizeof buffer,"INSERT INTO fs_houses SET descr='%s',X=%.2f,Y=%.2f,Z=%.2f,ownerid=0,koszt=%d",
        esc_opis, PX,PY,PZ, koszt);
  mysql_query(buffer);
  format(buffer,sizeof buffer,"Dodano dom o ID {b}%d{/b}. Wpisz {b}/domyreload{/b} aby przeladowac domy, {b}/usundom id{/b} aby go usunac.", mysql_insert_id());
  return Msg(playerid,COLOR_INFO,buffer);
}

CMD:usundom(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  new domid;
  if (sscanf(params,"d",domid))
    return Msg(playerid,COLOR_ERROR,"Uzyj: /usundom {b}id{/b}");
  new buffer[255];
  format(buffer,sizeof buffer,"DELETE FROM fs_houses WHERE id=%d", domid);
  mysql_query(buffer);
  if (mysql_affected_rows()==0){
    Msg(playerid,COLOR_ERROR,"Domu nie usunieto, bledne ID!");
  }else
    Msg(playerid,COLOR_INFO,"Dom zostal usuniety. Wpisz /domyreload aby je przeladowac.");
    format(buffer,sizeof buffer,"DELETE FROM fs_houses_vehicles WHERE houseid=%d", domid);
    mysql_query(buffer);
  return 1;
}

CMD:domtp(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_GM) && NIEMAPERZY) return 0;
  new domid;
  if (sscanf(params,"d",domid))
    return Msg(playerid,COLOR_ERROR,"Uzyj: /domtp {b}id{/b} - aby przeniesc sie do danego domu");

  domid=domy_findReverseID(domid);

  if (domid>=0 && FSDomy[domid][ed_id]>0)
    domy_tpto(playerid,domid,-180);
  else
    Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
  return 1;
}

CMD:domwyjscie(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz wysiasc z pojazdu!");
  new domid;
  if (sscanf(params,"d",domid))
    return Msg(playerid,COLOR_ERROR,"Uzyj: /domwyjscie {b}id{/b} - zapisuje biezace polozenie i kierunek, jako domyslny po wyjsciu z domu");

  domid=domy_findReverseID(domid);

  if (domid>=0 && FSDomy[domid][ed_id]>0) {
    GetPlayerPos(playerid,FSDomy[domid][ed_exit][0], FSDomy[domid][ed_exit][1], FSDomy[domid][ed_exit][2]);
    GetPlayerFacingAngle(playerid,FSDomy[domid][ed_exit][3]);
    new buffer[255];
    format(buffer,sizeof buffer,"UPDATE fs_houses SET exitX=%f,exitY=%f,exitZ=%f,exitA=%f WHERE id=%d",
          FSDomy[domid][ed_exit][0], FSDomy[domid][ed_exit][1], FSDomy[domid][ed_exit][2],
          FSDomy[domid][ed_exit][3],
          FSDomy[domid][ed_id]);
    mysql_query(buffer);
    Msg(playerid,COLOR_INFO2,"Zapisano informacje o domu. Wpisz /domyreload aby je przeladowac.");
  } else
    Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
  return 1;
}

CMD:domwnetrze(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz wysiasc z pojazdu!");
  new domid;
  if (sscanf(params,"d",domid))
    return Msg(playerid,COLOR_ERROR,"Uzyj: /domwnetrze {b}id{/b} - zapisuje biezace polozenie i kierunek, jako domyslne po wyjsciu do domu");

  domid=domy_findReverseID(domid);

  if (domid>=0 && FSDomy[domid][ed_id]>0) {
    GetPlayerPos(playerid,FSDomy[domid][ed_home][0], FSDomy[domid][ed_home][1], FSDomy[domid][ed_home][2]);
    GetPlayerFacingAngle(playerid,FSDomy[domid][ed_home][3]);
    FSDomy[domid][ed_homei]=GetPlayerInterior(playerid);
    new buffer[255];
    format(buffer,sizeof buffer,"UPDATE fs_houses SET homeX=%f,homeY=%f,homeZ=%f,homeA=%f,homeI=%d WHERE id=%d",
          FSDomy[domid][ed_home][0], FSDomy[domid][ed_home][1], FSDomy[domid][ed_home][2],
          FSDomy[domid][ed_home][3], FSDomy[domid][ed_homei],
          FSDomy[domid][ed_id]);
    mysql_query(buffer);
    Msg(playerid,COLOR_INFO2,"Zapisano informacje o domu. Wpisz /domyreload aby je przeladowac.");
  } else
    Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
  return 1;
}

CMD:domkoszt(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  new domid,koszt;
  if (sscanf(params,"dd",domid,koszt))
    return Msg(playerid,COLOR_ERROR,"Uzyj: /domkoszt {b}id{/b} {b}koszt{/b} - Gdzie koszt to ilosc respektu na dobe, lub 0 - platnosc SMS");

  domid=domy_findReverseID(domid);

  if (domid>=0 && FSDomy[domid][ed_id]>0) {
    new buffer[255];
    format(buffer,sizeof buffer,"UPDATE fs_houses SET koszt=%d WHERE id=%d", koszt,FSDomy[domid][ed_id]);
    mysql_query(buffer);
    Msg(playerid,COLOR_INFO2,"Zapisano informacje o domu. Wpisz /domyreload aby je przeladowac.");
  } else
    Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
  return 1;
}

CMD:dom(playerid,params[]){
  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz wlasnego domu, jesli chcesz go miec, to zacznij od rejestracji ({b}/rejestracja{/b} lub {b}/rej{/b})");

  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);

  if (hid==-1)
    return Msg(playerid,COLOR_ERROR,"Nie odnaleziono Twojego domu, wyglada na to, ze go nie posiadasz.");

  if (FSDomy[hid][ed_exit][0]!=0 && FSDomy[hid][ed_exit][1]!=0)
      Teleport(T_PLAYER, playerid, FSDomy[hid][ed_exit][0], FSDomy[hid][ed_exit][1], FSDomy[hid][ed_exit][2], FSDomy[hid][ed_exit][3],  0, 0);
  else  { // do pickupu
      Msg(playerid,COLOR_INFO2,"UWAGA! Twoj dom nie ma ustalonego punktu wyjscia. Popros admina 3-ciego poziomu, aby to ustawil.");
      Teleport(T_PLAYER, playerid, FSDomy[hid][ed_entrance][0]+random(3)-1.5, FSDomy[hid][ed_entrance][1]+random(3)-1.5, FSDomy[hid][ed_entrance][2], FLOAT_NAN,  0, 0);
  }
  return 1;
}

domy_findReverseID(dbid){
  if (dbid==0) return -1;
  for(new i=0;i<sizeof FSDomy;i++)
    if (dbid==FSDomy[i][ed_id]) return i;
  return -1;
}

/*
 * szukamy domu danego gracza // TODO zrobic w SQL podczas laczenia sie klienta
 */
domy_findHouseByOwnerID(oid){
  for(new i=0;i<sizeof FSDomy;i++)
    if(FSDomy[i][ed_owner]==oid) return i;
  return -1;
}

domy_PokazMenuDomu(playerid, houseid=-1){
  if (houseid>=0)
    pTemp[playerid][tmpLastHouse]=houseid;
  else
    houseid=pTemp[playerid][tmpLastHouse];

    pTemp[playerid][hPaymentType]=0;
  new buf[1024];
  if (FSDomy[houseid][ed_home][0]==0 && FSDomy[houseid][ed_home][1]==0)
    return Msg(playerid, COLOR_INFO,"Zamkniete - remont");
  if (
      ( FSDomy[houseid][ed_owner]==0 || FSDomy[houseid][ed_daysLeft]<0  ) &&
      FSDomy[houseid][ed_restrict_gang]!=NO_GANG && FSDomy[houseid][ed_restrict_gang]!=pData[playerid][gang]
    )
    return Msg(playerid, COLOR_INFO, "Tylko dla czlonkow klanu!");
  if (pData[playerid][accountID]>0 && FSDomy[houseid][ed_owner]==pData[playerid][accountID]) {
    strcat(buf, "{000000}W {ffffff}Wejdz");

    new wiadomosci=poczta_nieprzeczytanych(playerid);
    strcat(buf, "\n{000000}L {9090ff}Poczta");
    if (wiadomosci>0)
      format(buf,sizeof buf, "%s (%d %s)", buf, wiadomosci, dli(wiadomosci,"nowa","nowe","nowych"));

    if (FSDomy[houseid][ed_open])
      strcat(buf, "\n{000000}D {ff3030}Zamknij dom");
    else
      strcat(buf, "\n{000000}D {1CC718}Otworz dom");
    if (FSDomy[houseid][ed_vehiclesallowed]>0)
      strcat(buf, "\n{000000}P {09A7FF}Zapisz pojazd");

  } else if (FSDomy[houseid][ed_owner]>0 && FSDomy[houseid][ed_daysLeft]>=0) {  // zajety
    strcat(buf, "{000000}W {ffffff}Wejdz");
    strcat(buf, "\n{000000}S {3030ff}Zapukaj");
  } else if ((!pData[playerid][loggedIn] || FSDomy[houseid][ed_owner]!=pData[playerid][accountID]) && ( FSDomy[houseid][ed_owner]==0 || FSDomy[houseid][ed_daysLeft]<0  ))
    strcat(buf, "{000000}Z {ffffff}Zobacz dom");
  if (  ( FSDomy[houseid][ed_owner]==0 || FSDomy[houseid][ed_daysLeft]<0  )
    &&  (domy_findHouseByOwnerID(pData[playerid][accountID])==-1))  // gracz nie ma domu
    strcat(buf, "\n{000000}K {ffffff}Kup dom");

  if (pData[playerid][accountID]>0 && FSDomy[houseid][ed_owner]==pData[playerid][accountID])
    strcat(buf,"\n{000000}E {A4171A}Sprzedaj dom");

    if (FSDomy[houseid][ed_owner]>0 && FSDomy[houseid][ed_daysLeft]>=0)
        format(buf, sizeof buf, "%s\n{000000}K {ffffff}Oplacony do: %s", buf, FSDomy[houseid][ed_paidTo]);

  format(buf,sizeof buf,  "%s\n {ECFF19}http://www.fullserver.eu/domy/%d", buf,FSDomy[houseid][ed_id]);
  return ShowPlayerDialog(playerid, DIALOG_DOMY_OUTSIDEPICKUP, DIALOG_STYLE_LIST, "DOM", buf, "OK", "Wyjdz");
}

domy_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
  #pragma unused listitem
  new buf[255];

  switch(dialogid){
   case DIALOG_DOMY_INFO: {
    return ShowPlayerDialog(playerid, DIALOG_DOMY_OUTSIDEPICKUP, DIALOG_STYLE_LIST, "Dom", buf, "OK", "Wyjdz");
   }
   case DIALOG_DOMY_OUTSIDEPICKUP: {
    if (!response) return 1;
    switch(inputtext[0]){
      case 'W':
        return domy_Enter(playerid, pTemp[playerid][tmpLastHouse]);
      case 'Z':
        return domy_Enter(playerid, pTemp[playerid][tmpLastHouse]);
      case 'K':
        return domy_OplacCzynsz(playerid,pTemp[playerid][tmpLastHouse]);
      case 'E': {
        new buf2[1024],koszt;
        koszt=FSDomy[pTemp[playerid][tmpLastHouse]][ed_daysLeft]*FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt]*8+315;
        format(buf2, sizeof buf2, "Mozesz opuscic ten dom. Za pozostaly czas wynajmu zostanie Ci zwrocony\nekwiwalent w postaci gotowki: {90ff90}%d${ffffff}.\n\nJesli jestes pewien, ze chcesz tego dokonac, wpisz w ponizszym polu\nwyraz: {ffffff}TAK",  koszt);
        return ShowPlayerDialog(playerid, DIALOG_DOMY_SPRZEDAZ, DIALOG_STYLE_INPUT, "Wyprowadzka", buf2, "Potwierdz", "Anuluj");
      }
      case 'S': {
        Msg(playerid,COLOR_INFO,"Pukasz do drzwi");
        return domy_Zapukaj(playerid, pTemp[playerid][tmpLastHouse]);
      }
      case 'D': {
        new houseid=pTemp[playerid][tmpLastHouse];
        if (FSDomy[houseid][ed_open]) {
            FSDomy[houseid][ed_open]=false;
            Msg(playerid,COLOR_INFO,"Twoj dom zostal {b}zamkniety{/b}.");
        } else {
            FSDomy[houseid][ed_open]=true;
            Msg(playerid,COLOR_INFO,"Twoj dom zostal {b}otwarty{/b}.");
        }
        return domy_PokazMenuDomu(playerid);
      }
      case 'P': {
        return domy_ZapiszPojazd(playerid, pTemp[playerid][tmpLastHouse]);
      }
      case 'L': {
        poczta_czytajPoczte(playerid);
        return 1;
      }
      default:
        return 1;
    }
   }
   case DIALOG_DOMY_CZYNSZ_WYBOR: {
      if(response==1){
          pTemp[playerid][hPaymentType]=2;
    }else if(response==0){
        if(FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt]==0) return 1;
        pTemp[playerid][hPaymentType]=1;
    }
    return domy_OplacCzynsz(playerid,pTemp[playerid][tmpLastHouse]);
   }
   case DIALOG_DOMY_CZYNSZ: {

    if (!pData[playerid][loggedIn])
      return Msg(playerid, COLOR_ERROR, __("Tylko zarejestrowani gracze moga kupowac domy. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac!"));

    if (domy_findHouseByOwnerID(pData[playerid][accountID])>=0 && domy_findHouseByOwnerID(pData[playerid][accountID])!=pTemp[playerid][tmpLastHouse])
      return Msg(playerid,COLOR_ERROR,__("Masz juz dom! Sprzedaj go i wtedy kup ten."));

    if (!response)
      return domy_PokazMenuDomu(playerid);
    new dni;
    if (sscanf(inputtext,"d",dni))
      return domy_OplacCzynsz(playerid,-1,__("Podano nieprawidlowa wartosc!"));


    pTemp[playerid][tmpDialogValue]=dni;
    new lacznie=dni*FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt]; // to dziala tylko dla respektu!

    if (dni<1 || (lacznie<1 && pTemp[playerid][hPaymentType]==1))
      return domy_OplacCzynsz(playerid,-1,__("Podano nieprawidlowa wartosc!"));

        if(pTemp[playerid][hPaymentType]==1)
        {
          if (FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt]<=0)
        return Msg(playerid, COLOR_ERROR, __("Ten domek mozesz kupic tylko poprzez GamePoints (690 punktow na miesiac)."));

      if(dni>14)
          return domy_OplacCzynsz(playerid,-1,__("Nie mozesz oplacic domku respektem na wiecej niz 14 dni!"));

      if (FSDomy[pTemp[playerid][tmpLastHouse]][ed_daysLeft]>=14)
        return Msg(playerid, COLOR_ERROR, __("Ten domek jest oplacony na wiecej niz 14 dni - nie mozesz go oplacic na wiecej dni respektem!"));

          if (FSDomy[pTemp[playerid][tmpLastHouse]][ed_daysLeft]+dni>14)
        return Msg(playerid, COLOR_ERROR, __("Nie mozesz oplacic domku respektem na wiecej niz 14 dni!"));

            if (pData[playerid][respect]<lacznie)
        return domy_OplacCzynsz(playerid,-1,__("Nie masz wystarczajacej ilosci respektu! Sprobuj ponownie pozniej, lub skorzystaj z mo¿liwoœci do³¹dowania GameP za SMS."));
    }

    if(pTemp[playerid][hPaymentType]==2)
        {
      if(dni<15)
          return domy_OplacCzynsz(playerid,-1,__("Nie mozesz oplacic domku poprzez GamePoints na mniej niz 15 dni."));

        if (pData[playerid][gamep]<HOUSE_GP_COST*dni)
        return domy_OplacCzynsz(playerid,-1,__("Nie masz wystarczajacej ilosci GamePoints! Sprobuj ponownie pozniej, lub skorzystaj z mo¿liwoœci do³¹dowania GameP za SMS."));
    }

    if(pTemp[playerid][hPaymentType]==1){ // respekt
      format(buf, sizeof buf, __("Ilosc respektu: %d\nIlosc dni: %d\n\nZawsze mozesz kupic ten domek za GamePoints (690 za miesiac) i zaoszczedzic respekt"), lacznie, dni);
      ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ_CONFIRM, DIALOG_STYLE_MSGBOX, __("Potwierdz wplate"), buf, __("OK"), __("Anuluj"));
    }else if(pTemp[playerid][hPaymentType]==2){ // gamep
      format(buf, sizeof buf, __("Ilosc GamePoints: %d\nIlosc dni: %d"), HOUSE_GP_COST*dni, dni);
      ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ_CONFIRM, DIALOG_STYLE_MSGBOX, __("Potwierdz wplate"), buf, __("OK"), __("Anuluj"));
    }

   }
   case DIALOG_DOMY_CZYNSZ_CONFIRM: {
    if (!response)
      return domy_PokazMenuDomu(playerid);
    if (!pData[playerid][loggedIn])
      return Msg(playerid, COLOR_ERROR, __("Tylko zarejestrowani gracze moga kupowac domy. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac!"));

    if (pTemp[playerid][tmpDialogValue]<1)
      return domy_OplacCzynsz(playerid,-1,__("Podano nieprawidlowa wartosc!"));
    new lacznie=pTemp[playerid][tmpDialogValue]*FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt];
    if (pData[playerid][respect]<pTemp[playerid][tmpDialogValue] && pData[playerid][gamep]<HOUSE_GP_COST*pTemp[playerid][tmpDialogValue])
      return domy_OplacCzynsz(playerid,-1,__("Nie masz wystarczajacej ilosci respektu ani GamePoints! Sprobuj ponownie pozniej, lub skorzystaj z mozliwosci oplacenia domu przez SMS."));
    new domid=FSDomy[pTemp[playerid][tmpLastHouse]][ed_id];
    if (domid<0)
      return Msg(playerid, COLOR_ERROR, __("Wystapil blad. Nie odnaleziono domu. Operacja anulowana."));

    if (FSDomy[pTemp[playerid][tmpLastHouse]][ed_daysLeft]<0 || FSDomy[pTemp[playerid][tmpLastHouse]][ed_owner]==0)
      format(buf,sizeof buf,"UPDATE fs_houses set paidTo=NOW() + INTERVAL %d DAY, ownerid=%d WHERE id=%d AND (paidTo<=NOW() OR paidTO IS NULL OR ownerid IS NULL OR ownerid=0)", pTemp[playerid][tmpDialogValue], pData[playerid][accountID],domid);
    else
      format(buf,sizeof buf,"UPDATE fs_houses set paidTo=paidTo + INTERVAL %d DAY WHERE ownerid=%d AND id=%d", pTemp[playerid][tmpDialogValue], pData[playerid][accountID],domid);
    if (!mysql_query(buf) || mysql_affected_rows()<1)
      return Msg(playerid,COLOR_ERROR,__("Wystapil blad podczas dokonywania operacji. Zostala ona anulowana."));
    Msg(playerid,COLOR_INFO,__("Gratulacje! Oplaciles swoj dom!"));

    format(gstr,sizeof gstr,"Oplacil%s swoj dom na %d %s. Mozesz rowniez doladowac swoj stan GamePoints za SMS na stronie {FF0000}http://fullserver.eu/doladowania",
        Kobieta(playerid)?("as"):("es"), pTemp[playerid][tmpDialogValue], dli(pTemp[playerid][tmpDialogValue],"dzien","dni","dni"));
    poczta_powiadomGracza(pData[playerid][accountID],gstr);

    if(pTemp[playerid][hPaymentType]==1){
      printf("ZAKUP DOMKU ZA RESPEKT, ACCOUNT: %d, ILOSC DNI: %d, KOSZT: %d",pData[playerid][accountID],pTemp[playerid][tmpDialogValue],lacznie);
      GivePlayerScore(playerid,-lacznie,"zakup domku");
    }else if(pTemp[playerid][hPaymentType]==2){
      printf("ZAKUP DOMKU ZA GAME POINTS, ACCOUNT: %d, ILOSC DNI: %d, KOSZT: %d",pData[playerid][accountID],pTemp[playerid][tmpDialogValue],HOUSE_GP_COST*pTemp[playerid][tmpDialogValue]);
      GivePlayerGamePoints(playerid,-HOUSE_GP_COST*pTemp[playerid][tmpDialogValue],"zakup domku");
    }
    pTemp[playerid][hPaymentType]=0;
    pTemp[playerid][e_houseid]=pTemp[playerid][tmpLastHouse];//aqq
    SetPlayerScore(playerid,pData[playerid][respect]);
    SavePlayerData(playerid);
    domy_Reload();
    return 1;
   }
   case DIALOG_DOMY_SPRZEDAZ: {
    if (!response)
      return domy_PokazMenuDomu(playerid);
    if (strcmp(inputtext,"tak",true)!=0) {
      format(buf, sizeof buf, "Nie potwierdzil%s checi sprzedazy domu.", Kobieta(playerid)?("as"):("es"));
      Msg(playerid,COLOR_INFO2,buf);
      return domy_PokazMenuDomu(playerid);
    }

    new domid=pTemp[playerid][tmpLastHouse];

    if (FSDomy[domid][ed_owner]==0 || FSDomy[domid][ed_owner]!=pData[playerid][accountID]) {
      Msg(playerid, COLOR_ERROR, __("Wystapil blad! Ten dom nie nalezy do Ciebie!"));
      return 1;
    }
    new domidDB=FSDomy[domid][ed_id];
    new kwota=FSDomy[domid][ed_daysLeft]*FSDomy[domid][ed_koszt]*8+315;
    GivePlayerMoney(playerid, kwota);
    format(buf, sizeof buf, __("Dostajesz {b}%d${/b} zwrotu za pozostaly czynsz."), kwota);
    Msg(playerid, COLOR_INFO, buf);
    format(buf,sizeof buf,"UPDATE fs_houses set paidTo='0000-00-00', ownerid=0 WHERE id=%d", domidDB);
    mysql_query(buf);
    domy_Reload();
    domy_tpto(playerid,domid);
   }
  }
  return 1;
}

domy_OplacCzynsz(playerid,hid=-1,komunikat[]=""){
  new buf[1024];
  if (hid==-1) hid=pTemp[playerid][tmpLastHouse];

  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Tylko zarejestrowani gracze moga kupowac domy. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac!");
  if(FSDomy[hid][ed_owner]>0 && FSDomy[hid][ed_owner]!=pData[playerid][accountID])
  {
      Msg(playerid, COLOR_ERROR, __("Wystapil blad! Ten dom nie nalezy do Ciebie!"));
      return 1;
  }
  if (pData[playerid][respect]<FSDomy[hid][ed_koszt] && pData[playerid][gamep]<HOUSE_GP_COST) {
    return Msg(playerid,COLOR_ERROR,"Niestety! Nie masz wystarczajaco respektu ani GamePoints, by oplacic chociaz dzien wynajmu! Sprobuj pozniej, lub skorzystaj z mo¿liwoœci do³¹dowania GameP za SMS.");
  }
  if(pTemp[playerid][hPaymentType]==1){ // respekt
    format(buf, sizeof buf, "Czynsz za dobe wynosi %d respektu.", FSDomy[hid][ed_koszt]);
    if (FSDomy[hid][ed_daysLeft]>=0 && FSDomy[hid][ed_owner]>0)
      format(buf,sizeof buf, "%s Obecnie dom jest oplacony do %s (dni: %d).", buf, FSDomy[hid][ed_paidTo], FSDomy[hid][ed_daysLeft]);
    strcat(buf, "\nPodaj, na ile dni chcesz oplacic dom:");
    if (strlen(komunikat)>2) { strcat(buf, "\n{ff0000}"); strcat(buf, komunikat); }
    return ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ, DIALOG_STYLE_INPUT, "Oplacanie domu", buf, "OK", "Anuluj");
  }else if(pTemp[playerid][hPaymentType]==2){ // gamep
      format(buf, sizeof buf, "Czynsz za dobe wynosi %d GamePoints.", HOUSE_GP_COST);
    if (FSDomy[hid][ed_daysLeft]>=0 && FSDomy[hid][ed_owner]>0)
      format(buf,sizeof buf, "%s Obecnie dom jest oplacony do %s (dni: %d).", buf, FSDomy[hid][ed_paidTo], FSDomy[hid][ed_daysLeft]);
    strcat(buf, "\nPodaj, na ile dni chcesz oplacic dom:");
    if (strlen(komunikat)>2) { strcat(buf, "\n{ff0000}"); strcat(buf, komunikat); }
    return ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ, DIALOG_STYLE_INPUT, "Oplacanie domu", buf, "OK", "Anuluj");
  }else if(pTemp[playerid][hPaymentType]==0){ // wybor
      if(FSDomy[hid][ed_koszt]==0){
        ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ_WYBOR, DIALOG_STYLE_MSGBOX, "Oplacanie domu", "Ten domek mo¿na op³aciæ wy³¹cznie poprzez GamePoints.\nKoszt wynajmu na jeden dzieñ do 690GP. Chcesz konynuowac?", "TAK", "NIE");
      }else{
        ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ_WYBOR, DIALOG_STYLE_MSGBOX, "Oplacanie domu", "W jaki sposob chcesz oplacic czynsz tego domku?", "GamePoints", "Respekt");
      }
  }
  return 1;
}

domy_Enter(playerid,houseid){
  if (pData[playerid][adminLevel]>=LEVEL_ADMIN3 )

  pTemp[playerid][tmpLastHouse]=houseid;

  if (!FSDomy[houseid][ed_open] && FSDomy[houseid][ed_owner]>0 && FSDomy[houseid][ed_owner]!=pData[playerid][accountID])  // dom zamkniety i ma wlasciciela, nie jest nim gracz
    return ShowPlayerDialog(playerid, DIALOG_DOMY_INFO, DIALOG_STYLE_MSGBOX, "Dom", "Zamkniete!", "OK", "");

  domy_SaveReturnPath(playerid,houseid);
  Teleport(T_PLAYER, playerid, FSDomy[houseid][ed_home][0],FSDomy[houseid][ed_home][1],FSDomy[houseid][ed_home][2], FSDomy[houseid][ed_home][3],  FSDomy[houseid][ed_homei], FSDomy[houseid][ed_homevw], true);
  if (FSDomy[houseid][ed_owner]==playerid)
    GameTextForPlayer(playerid, __("Witaj w domu"), 1000, 1);
  else if (FSDomy[houseid][ed_owner]>0) {
    new buf[64];
    format(buf, sizeof buf, __("Dom %s"), FSDomy[houseid][ed_ownernick]);
    GameTextForPlayer(playerid, buf, 1000, 1);
  } else {
    // todo sprawdzanie czy ktos jest w domu
    GameTextForPlayer(playerid, __("Dom na sprzedaz"), 1000, 1);
  }
  return 1;
}

domy_SaveReturnPath(playerid,hid) {
  pTemp[playerid][tmpLastHouse]=hid;

  if (FSDomy[hid][ed_exit][0]==0 && FSDomy[hid][ed_exit][1]==0) {
    pTemp[playerid][pickupDelay]=GetTickCount();
    return SaveReturnPath(playerid, FSDomy[hid][ed_homei]);
  }

  retpos[playerid][tpDestInt]=FSDomy[hid][ed_homei];
  retpos[playerid][tpWorld]=GetPlayerVirtualWorld(playerid);
  retpos[playerid][tpInterior]=GetPlayerInterior(playerid);
  retpos[playerid][tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;

  retpos[playerid][tpX]=FSDomy[hid][ed_exit][0];
  retpos[playerid][tpY]=FSDomy[hid][ed_exit][1];
  retpos[playerid][tpZ]=FSDomy[hid][ed_exit][2];
  retpos[playerid][tpA]=FSDomy[hid][ed_exit][3];

  return 1;
}

domy_Zapukaj(playerid,hid){
  #pragma unused playerid
  if (FSDomy[hid][ed_homevw]==0) return 1;
  foreach(p)
    if (GetPlayerVirtualWorld(p)==FSDomy[hid][ed_homevw]) {
      Msg(p,COLOR_INFO2,"Ktos puka do drzwi");
      Audio_Play(p,AUDIOID_FXDOMPUKANIE);
    }
  return 1;
}

domy_ZapiszPojazd(playerid, houseid){
  if (!pData[playerid][loggedIn] || pData[playerid][accountID]!=FSDomy[houseid][ed_owner] ||
    pData[playerid][lastVehicle]==INVALID_VEHICLE_ID || houseid!=domy_findHouseByOwnerID(pData[playerid][accountID])
    )
    return Msg(playerid,COLOR_ERROR,"Aby zapisac pojazd przy domu, zaparkuj go w poblizu wejscia i uaktywnij ta opcje");

  if (FSDomy[houseid][ed_vehiclesallowed]<0)
    return Msg(playerid,COLOR_ERROR,"Przy Twoim domie nie da sie zapisac pojazdu");

  new Float:vP[4];
  new vid=pData[playerid][lastVehicle];

  GetVehiclePos(vid,vP[0],vP[1],vP[2]);
  GetVehicleZAngle(vid,vP[3]);
  new Float:vdist=GetVehicleDistanceFromPoint(vid, FSDomy[houseid][ed_exit][0], FSDomy[houseid][ed_exit][1], FSDomy[houseid][ed_exit][2]);
  if (vdist>FSDomy[houseid][ed_vehicleradius] && vdist<(FSDomy[houseid][ed_vehicleradius]*4))
    return Msg(playerid,COLOR_ERROR,"Pojazd stoi za daleko.");
  else if (vdist>FSDomy[houseid][ed_vehicleradius])
    return Msg(playerid,COLOR_ERROR,"Zaparkuj pojazd pod domem.");
  new buf[450];
  format(buf, sizeof buf, "DELETE FROM fs_houses_vehicles WHERE houseid=%d", FSDomy[houseid][ed_id]);
  mysql_query(buf);
  new vmodel=GetVehicleModel(vid);
  if(
    //samoloty
    vmodel==460 || vmodel==476 || vmodel==511 || vmodel==512 || vmodel==513 || vmodel==519 || vmodel==520 ||
    vmodel==553 || vmodel==577 || vmodel==592 || vmodel==593 ||
    //helikoptery
    vmodel==548 || vmodel==425 || vmodel==417 || vmodel==487 || vmodel==488 || vmodel==497 || vmodel==563 ||
    vmodel==447 || vmodel==469 ||
    //pojazdy rc
    vmodel==441 || vmodel==464 || vmodel==465 || vmodel==501 || vmodel==564 || vmodel==594  ||
    //przyczepy
    vmodel==435 || vmodel==450 || vmodel==569 || vmodel==570 || vmodel==584 || vmodel==590 || vmodel==591 ||
    vmodel==606 || vmodel==607 || vmodel==608 || vmodel==610 || vmodel==611 ||
    //lodzie
    vmodel==472 || vmodel==473 || vmodel==493 || vmodel==595 || vmodel==484 || vmodel==430 || vmodel==453 ||
    vmodel==452 || vmodel==446 || vmodel==454 ||
    //inne zakazane pojazdy (rhino, autobusy etc)
    vmodel==416 || vmodel==433 || vmodel==431 || vmodel==437 || vmodel==427 || vmodel==407 || vmodel==544 ||
    vmodel==432 || vmodel==601 || vmodel==485 || vmodel==538 || vmodel==483 || vmodel==532 || vmodel==486 ||
    vmodel==406 || vmodel==530 || vmodel==537 || vmodel==588 || vmodel==508 || vmodel==572 || vmodel==423 ||
    vmodel==428 || vmodel==449
  ) {
      return Msg(playerid,COLOR_ERROR,"Nie mozesz zapisac tego pojazdu pod domem!");
  }
  new vcomp[128];
  for(new i=0;i<=13;i++) {
    FSDomy[houseid][ehv_components][i]=0;
    FSDomy[houseid][ehv_components][i]=GetVehicleComponentInSlot(vid,i);
    format(vcomp,sizeof vcomp,"%s%s%d", vcomp, (strlen(vcomp)>0 ? (":") : ("")), FSDomy[houseid][ehv_components][i]);
  }

  sscanf(vcomp,"p<:>dddddddddddddd", FSDomy[houseid][ehv_components][0], FSDomy[houseid][ehv_components][1], FSDomy[houseid][ehv_components][2], FSDomy[houseid][ehv_components][3],
      FSDomy[houseid][ehv_components][4], FSDomy[houseid][ehv_components][5], FSDomy[houseid][ehv_components][6], FSDomy[houseid][ehv_components][7],
      FSDomy[houseid][ehv_components][8], FSDomy[houseid][ehv_components][9], FSDomy[houseid][ehv_components][10], FSDomy[houseid][ehv_components][11],
      FSDomy[houseid][ehv_components][12], FSDomy[houseid][ehv_components][13]);


  format(buf, sizeof buf, "INSERT INTO fs_houses_vehicles SET houseid=%d,model=%d,X=%.2f,Y=%.2f,Z=%.2f,A=%.2f,color1=%d,color2=%d,components='%s',plate='%s'",
        FSDomy[houseid][ed_id], vmodel, vP[0], vP[1], vP[2], vP[3], tVehicles[vid][vo_color][0], tVehicles[vid][vo_color][1], vcomp, GetPlayerProperName(playerid));
  mysql_query(buf);
  Msg(playerid,COLOR_INFO,"Pojazd zostal zapisany. Uzyj {b}/pp{/b} aby go przywolac lub {b}/mp{/b} aby teleportowac sie do niego!");

  FSDomy[houseid][ehv_active]=true;
  FSDomy[houseid][ehv_model]=vmodel;
  FSDomy[houseid][ehv_spawnPos][0]=vP[0];
  FSDomy[houseid][ehv_spawnPos][1]=vP[1];
  FSDomy[houseid][ehv_spawnPos][2]=vP[2];
  FSDomy[houseid][ehv_spawnPos][3]=vP[3];
  FSDomy[houseid][ehv_color][0]=tVehicles[vid][vo_color][0];
  FSDomy[houseid][ehv_color][1]=tVehicles[vid][vo_color][1];

    if (!tVehicles[vid][vo_static] && !tVehicles[vid][vo_private])
        DestroyVehicle(vid);
    else
        SetVehicleToRespawn(vid);

    for(new i=0;i<=13;i++){
    if (FSDomy[houseid][ehv_components][i]>0){
      AddVehicleComponent(vid, FSDomy[houseid][ehv_components][i]);
    }
  }

  domy_SpawnVehicle(playerid);

  return 1;
}

CMD:pp(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!TeleportAllowed(playerid) || pData[playerid][pAttraction]!=A_NONE || GetPlayerInterior(playerid)!=0 || GetPlayerVirtualWorld(playerid)!=0)
    return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");

  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Zapisywanie i przywolywanie pojazdu jest mozliwe tylko dla zarejestrowanych graczy.");
  new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
  if (hid<0)
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz domu!");

  if (!FSDomy[hid][ehv_active])
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz zapisanego pojazdu!");

  if (FSDomy[hid][ed_vehiclesallowed]<1)
    return Msg(playerid,COLOR_ERROR,"Przy Twoim domie nie da sie zapisac pojazdu.");

  if (IsPlayerInAnyVehicle(playerid))
    return Msg(playerid,COLOR_ERROR,"Najpierw wysiadz z pojazdu");

  if(FSDomy[hid][ehv_id]==INVALID_VEHICLE_ID || tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]!=playerid) domy_SpawnVehicle(playerid); // only respawn if actual vehicle is not owned (or not exists) by actual house owner (prevent auto-fix)

  new Float:pP[4];
  GetPlayerPos(playerid,pP[0], pP[1], pP[2]);
  LinkVehicleToInterior(FSDomy[hid][ehv_id],0);
  SetVehiclePos(FSDomy[hid][ehv_id], pP[0], pP[1], pP[2]+1.5);

  PutPlayerInVehicle(playerid,FSDomy[hid][ehv_id], 0);

  return 1;
}

CMD:mp(playerid){
  MUST_BE_SPAWNED(playerid);
  if (!TeleportAllowed(playerid) || pData[playerid][pAttraction]!=A_NONE)
    return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");

  if (!pData[playerid][loggedIn])
    return Msg(playerid,COLOR_ERROR,"Zapisywanie i przywolywanie pojazdu jest mozliwe tylko dla zarejestrowanych graczy.");
  new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
  if (hid<0)
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz domu!");

  if (!FSDomy[hid][ehv_active])
    return Msg(playerid,COLOR_ERROR,"Nie posiadasz zapisanego pojazdu!");

  if (FSDomy[hid][ed_vehiclesallowed]<1)
    return Msg(playerid,COLOR_ERROR,"Przy Twoim domie nie da sie zapisac pojazdu.");

  if (IsPlayerInAnyVehicle(playerid))
    return Msg(playerid,COLOR_ERROR,"Najpierw wysiadz z pojazdu");

  if(FSDomy[hid][ehv_id]==INVALID_VEHICLE_ID || tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]!=playerid) domy_SpawnVehicle(playerid); // only respawn if actual vehicle is not owned (or not exists) by actual house owner (prevent auto-fix)

  new Float:pP[4];
  GetVehiclePos(FSDomy[hid][ehv_id],pP[0], pP[1], pP[2]);
  SetPlayerInterior(playerid,0);
  SetPlayerPos(playerid, pP[0], pP[1], pP[2]+1.5);
  SetPlayerVirtualWorld(playerid,0);

  PutPlayerInVehicle(playerid,FSDomy[hid][ehv_id], 0);

  return 1;
}

CMD:domyreload(playerid){
  if(!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  Msg(playerid,COLOR_INFO2,"Przeladowywanie domow");
  domy_Reload();
  return 1;
}
// EOF
