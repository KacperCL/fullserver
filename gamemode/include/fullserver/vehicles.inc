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

#define DIALOG_TUNEMENU_MAIN    80
#define DIALOG_TUNEMENU_KOLOR1    (DIALOG_TUNEMENU_MAIN+1)
#define DIALOG_TUNEMENU_ALUFELGI  (DIALOG_TUNEMENU_MAIN+2)

#define DIALOG_WARSZTAT       90
#define DIALOG_WARSZTAT_TABLICE   (DIALOG_WARSZTAT+0)
#define DIALOG_WARSZTAT_LIFT    (DIALOG_WARSZTAT+1)

#define DIALOG_POJAZDY          1110
#define DIALOG_POJAZDY_RECZNIE    (DIALOG_POJAZDY+0)
#define DIALOG_POJAZDY_LISTA    (DIALOG_POJAZDY+1)
#define DIALOG_POJAZDY_TYP      (DIALOG_POJAZDY+2)
#define DIALOG_POJAZDY_RODZAJE    (DIALOG_POJAZDY+3)
#define DIALOG_POJAZDY_AD           5124

#define DIALOGLIST_ALUFELGI "Offroad\nShadow\nMega\nRimshine\nWires\nClassic\nTwist\nCutter\nSwitch\nGrove\nImport\nDollar\nTrance\nAtomic\nAhab\nVirtual\nAccess"

forward RespawnVehicle(vid);
forward vehicles_EngageTurbo(playerid,vehicleid,step);

new DATA_alufelgi[]={ 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1096, 1097, 1098};

new staticVehicleCount=0;

new rodzaje[][]={
  "Motory",
  "Lowridery",
  "Kabriolety",
  "Auta kombi",
  "Pojazdy off-road",
  "Auta sedan",
  "Auta sportowe",
  "Pojazdy przemyslowe",
  "Helikoptery",
  "Samoloty",
  "Lodzie",
  "Rowery"
};

new pojazdy[][]={
  {522,581,463,468,462},
  {536,534,535,412},
  {533,439,555},
  {404,479,458,561},
  {568,424,400,500,489,495}, //5
  {445,562,585,419,492,474,546,517,426,536,547,405,580,560},
  {602,429,496,402,541,415},
  {499,422,482,498,609,524,578,455,403},
  {487,563,469},
  {511,512,593,513},
  {473,493,484,454},
  {481,510}
};

new txt_rodzaje[1024]="Wprowadz nazwe (tylko {FFFF00}VIP{ffffff})";
new txt_pojazdy[][]={
  "NRG-500\r\nBF-400\r\nFreeway\r\nSanchez\r\nFaggio",
  "Blade\r\nRemington\r\nSlamvan\r\nVoodoo",
  "Feltzer\r\nStallion\r\nWindsor",
  "Perennial\r\nRegina\r\nSolair\r\nStratum",
  "Bandito\r\nBF Injection\r\nLandstalker\r\nMesa\r\nRancher\r\nSandking",
  "Admiral\r\nElegy\r\nEmperor\r\nEsperanto\r\nGreenwood\r\nHermes\r\nIntruder\r\nMajestic\r\nPremier\r\nPrevion\r\nPrimo\r\nSentinel\r\nStafford\r\nSultan",
  "Alpha\r\nBanshee\r\nBlista compact\r\nBuffalo\r\nBullet\r\nCheetah",
  "Benson\r\nBobcat\r\nBurrito\r\nBoxville\r\nBoxburg\r\nCement truck\r\nDFT-30\r\nFlatbed\r\nLinerunner",
  "Maverick\r\nRaindance\r\nSparrow",
  "Beagle\r\nCropduster\r\nDodo\r\nStuntplane",
  "Dinghy\r\nJetmax\r\nMarquis\r\nTropic",
  "BMX\r\nRower gorski"
};

stock GetNearestVehicleToPlayer(playerid,Float:maxdist=0.0,bool:fast=false){
  new Float:P[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  return GetNearestVehicleFromXYZ(P[0],P[1],P[2],maxdist,fast);
}

stock GetNearestVehicleFromXYZ(Float:PX,Float:PY,Float:PZ,Float:maxdist=0.0,bool:fast=true,except_vid=0,VF=0){
  new Float:distance=-1.0;
  new foundvid=INVALID_VEHICLE_ID;
  for(new vehicleid = 1; vehicleid != MAX_VEHICLES; vehicleid++){
    if (except_vid==vehicleid) continue;
    if (VF>0 && GetVehicleFlags(vehicleid)&VF!=VF)  continue;

    new Float:V[4];
    GetVehiclePos(vehicleid,V[0],V[1],V[2]);

    GetVehicleZAngle(vehicleid,V[3]);
    V[0]+=(2.5*floatsin(-V[3],degrees));
    V[1]+=(2.5*floatcos(-V[3],degrees));


    if (V[0]==0.0 && V[1]==0.0 && V[2]==0.0) continue;
    new Float:tmpdistance=GetDistanceBetweenPoints3D(PX,PY,PZ,V[0],V[1],V[2]);
    if (maxdist>0.0 && tmpdistance>maxdist) continue;
    else if (fast && (distance==-1.0 || tmpdistance<distance) && (maxdist==0.0 || (tmpdistance<maxdist) ) )
      return vehicleid;
    else if (distance==-1.0 || tmpdistance<distance) {
      distance=tmpdistance;
      foundvid=vehicleid;
    }
  }
  return foundvid;
}

/* Sprawdzamy czy za pojazdem znajduje sie inny pojazd ktory mozna holowac
 * Jesli tak, to podczepiamy go pod hol
 * Wywolywane przez OnPlayerKeyStateChange po weryfikacji pojazdu jako towtruck
 */
TowVehicle(tvid){
  if(IsTrailerAttachedToVehicle(tvid)) return;  // juz cos holujemy
  new Float:TP[3],Float:Angle;
  GetVehiclePos(tvid,TP[0],TP[1],TP[2]);

  // przeksztalcamy te wspolrzedne, tak aby znajdowaly sie za pojazdem - tam gdzie hak
  GetVehicleZAngle(tvid,Angle);
  Angle+=180.0;
  TP[0]+=(3*floatsin(-Angle,degrees));
  TP[1]+=(3*floatcos(-Angle,degrees));

  // szukamy najbliszego pojazdu
  new avid=GetNearestVehicleFromXYZ(TP[0],TP[1],TP[2],3,false,tvid,VF_TOWABLE);
  if (avid==0 || avid==INVALID_VEHICLE_ID)  // nie znaleziono pojazdu do holowania!
    return;

  AttachTrailerToVehicle(avid,tvid);

  tVehicleUsed[avid]=true;
  tVehicles[avid][vo_used]=true;
  tVehicles[avid][vo_occupied]=false;

  // sprawdzamy czy holowany pojazd jest zamkniety
  new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(avid, engine, lights, alarm, doors, bonnet, boot, objective);

  if (doors>0) {  // jest zamkniety, uruchamiamy alarm
    SetVehicleParamsEx(avid, engine, 0, 1, doors, bonnet, boot, objective);
  }
  return;
}

RespawnVehicles(force=false)
{
  new bool:UsedVeh[MAX_VEHICLES] = {false, ...};
  foreach(i) {
    new vid=GetPlayerVehicleID(i);
    if(vid==0 || vid==INVALID_VEHICLE_ID) continue;

    UsedVeh[vid] = true;
    tVehicles[vid][vo_occupied]=true;
    new vmodel=GetVehicleModel(vid);
    if (vmodel==525 || vmodel==403 || vmodel==514 || vmodel==515) { // towtruck, linerunner, petrol, roadtrain
      new tvid=GetVehicleTrailer(vid);
      if (tvid>0 && tvid!=INVALID_VEHICLE_ID) {
        UsedVeh[tvid] = true;
        tVehicles[tvid][vo_occupied]=true;
      }
    }
  }

  // najpierw pojazdy statyczne
  for(new vehicleid = random(3)+1; vehicleid < staticVehicleCount; vehicleid+=3) {
    if(!UsedVeh[vehicleid] && tVehicleUsed[vehicleid]){
      SetVehicleToRespawn(vehicleid);
    }
  }

  // teraz dynamiczne
    for(new vehicleid = staticVehicleCount+1; vehicleid < MAX_VEHICLES; vehicleid+=(force ? 1 : ( random(2)+1 )) ){
    if (!UsedVeh[vehicleid]) {  // czasami przekraczalo indeks z powodu tego ^^ randoma
      if (tVehicles[vehicleid][vo_private] && tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID) {
      } else if (tVehicles[vehicleid][vo_destroyIfEmpty]) {
        DestroyVehicle(vehicleid);
        tVehicles[vehicleid][vo_destroyIfEmpty]=false;
        tVehicleUsed[vehicleid]=false;

      } else {
          if (tVehicleUsed[vehicleid] && !force){
          tVehicleUsed[vehicleid]=false;
          } else if (tVehicleSpawned[vehicleid]) {
          DestroyVehicle(vehicleid); tVehicleSpawned[vehicleid]=false;
          } else
          SetVehicleToRespawn(vehicleid);
      }
    }
  }
    return 1;
}

RespawnVehiclesInRangeOf(playerid)
{
  new bool:UsedVeh[MAX_VEHICLES] = {false, ...};

  foreach(i) {
    new vid=GetPlayerVehicleID(i);
    if(vid==0 || vid==INVALID_VEHICLE_ID) continue;

    UsedVeh[vid] = true;
    tVehicles[vid][vo_occupied]=true;
    new vmodel=GetVehicleModel(vid);
    if (vmodel==525 || vmodel==403 || vmodel==514 || vmodel==515) { // towtruck, linerunner, petrol, roadtrain

      new tvid=GetVehicleTrailer(vid);
      if (tvid>0 && tvid!=INVALID_VEHICLE_ID) {
        UsedVeh[tvid] = true;
        tVehicles[tvid][vo_occupied]=true;
      }
    }
  }

  for(new vehicleid = 1; vehicleid != MAX_VEHICLES; vehicleid+=1)
  {
    if (!UsedVeh[vehicleid] && IsVehicleStreamedIn(vehicleid,playerid)) {
      if (vehicleid<staticVehicleCount) {   // statyczne
        // comment by eider: fucky hax0r assess's changing positions of vehicles by unoccupied sync and we cant respawn those vehicles 'cause they were never occupied (tVehicleUsed[vehicleid] is FALSE)
        if (!UsedVeh[vehicleid]/* && tVehicleUsed[vehicleid]*/) { SetVehicleToRespawn(vehicleid); }
      } else {
        if (tVehicles[vehicleid][vo_private] && tVehicles[vehicleid][vo_owningPlayerId]!=INVALID_PLAYER_ID) {
        } else if (tVehicles[vehicleid][vo_destroyIfEmpty]) {
          DestroyVehicle(vehicleid);
          tVehicles[vehicleid][vo_destroyIfEmpty]=false;
          tVehicleUsed[vehicleid]=false;
        } else {
          if (tVehicleUsed[vehicleid]){
            tVehicleUsed[vehicleid]=false;
          }
          else if (tVehicleSpawned[vehicleid]) {
            DestroyVehicle(vehicleid); tVehicleSpawned[vehicleid]=false;
          } else{
            SetVehicleToRespawn(vehicleid);
          }
        }
      }
    }
  }
  return 1;
}

public RespawnVehicle(vid){
  if (IsTrailerAttachedToVehicle(vid))
    DetachTrailerFromVehicle(vid);

  if (tVehicles[vid][vo_static])
    SetVehicleToRespawn(vid);
  else
    DestroyVehicle(vid);
  tVehicleSpawned[vid]=false;

}

UpdateVehicleNames(resultid=0,spareid=0){
  #pragma unused spareid
  #pragma unused resultid

  mysql_query("SELECT vid,name FROM fs_vehicles order by vid ASC");

  new buffer[40],vid,vehname[32];

  mysql_store_result();
  if (!mysql_num_rows()) { mysql_free_result(); return 0; }
  while (mysql_fetch_row_format(buffer,"|")) {
    sscanf(buffer,"p<|>ds[32]",vid,vehname);
    copy(vehname,vehicleName[vid-400]);
  }

  mysql_free_result();

  return 1;
}

vehicles_RCTiger_shoot(playerid){
  if (GetTickCount()-pTemp[playerid][vehicleSpecialLastUsed]<500) return 0;
  pTemp[playerid][vehicleSpecialLastUsed]=GetTickCount();
  new vid=GetPlayerVehicleID(playerid);
  if (GetVehicleModel(vid)!=564) return 0;

  new Float:TP[3],Float:Angle;
    GetVehiclePos(vid,TP[0],TP[1],TP[2]);
    // przeksztalcamy te wspolrzedne, tak aby znajdowaly sie za pojazdem - tam gdzie hak
    GetVehicleZAngle(vid,Angle);
    TP[0]+=(7*floatsin(-Angle,degrees));
    TP[1]+=(7*floatcos(-Angle,degrees));
  TP[2]+=0.2;

  CreateExplosion(TP[0],TP[1],TP[2],11,3);
  return 1;
}

stock TuneCar(vehicleid) // oparte na kodzie DrunkeR-a
{
  vehicleHasNitro[vehicleid] = true;
  switch(GetVehicleModel(vehicleid))
  {
    case 461,521,522,523,581: // PCJ,FCR,NRG,hpv-1000,bf-400
      tVehicles[vehicleid][vo_hasTurbo]=true;
    case 400:
    AddVehComp(vehicleid,1008,1009,1010,1013,1018,1019,1020,1021,1024,1086,1087);
    case 401:
    AddVehComp(vehicleid,1001,1003,1004,1005,1006,1007,1008,1009,1010,1013,1017,1019,1020,1086,10871142,1143,1144,1145);
    case 404:
    AddVehComp(vehicleid,1000,1002,1007,1008,1009,1010,1013,1016,1017,1019,1020,1021,1086,1087);
    case 405:
    AddVehComp(vehicleid,1000,1001,1008,1009,1010,1014,1018,1019,1020,1021,1023,1086,1087);
    case 410:
    AddVehComp(vehicleid,1001,1003,1007,1008,1009,1010,1013,1017,1019,1020,1021,1023,1024,1086,1087);
    case 415:
    AddVehComp(vehicleid,1001,1003,1007,1008,1009,1010,1017,1018,1019,1023,1086,1087);
    case 418:
    AddVehComp(vehicleid,1002,1006,1008,1009,1010,1016,1020,1021,1086,1087);
    case 420:
    AddVehComp(vehicleid,1001,1003,1004,1005,1008,1009,1010,1019,1021,1086,1087);
    case 421:
    AddVehComp(vehicleid,1000,1008,1009,1010,1014,1016,1018,1019,1020,1021,1023,1086,1087);
    case 422:
    AddVehComp(vehicleid,1007,1008,1009,1010,1013,1017,1019,1020,1021,1086,1087);
    case 426:
    AddVehComp(vehicleid,1001,1003,1004,1005,1006,1008,1009,1010,1019,1021,1086,1087);
    case 429:
    AddVehComp(vehicleid,1008,1009,1010,1085,1087);
    case 436:
    AddVehComp(vehicleid,1001,1003,1006,1007,1008,1009,1010,1013,1017,1019,1020,1021,1022,1086,1087);
    case 439:
    AddVehComp(vehicleid,1001,1003,1007,1008,1009,1010,1013,1017,1023,1086,1087,1142,1143,1144,1145);
    case 477:
    AddVehComp(vehicleid,1006,1007,1008,1009,1010,1017,1018,1019,1020,1021,1086,1087);
    case 478:
    AddVehComp(vehicleid,1004,1005,1008,1009,1010,1012,1013,1020,1021,1022,1024,1086,1087);
    case 489:
    AddVehComp(vehicleid,1000,1002,1004,1005,1006,1008,1009,1010,1013,1016,1018,1019,1020,1024,1086,1087);
    case 491:
    AddVehComp(vehicleid,1003,1007,1008,1009,1010,1014,1017,1018,1019,1020,1021,1023,1086,1087,1142,1143,1144,1145);
    case 492:
    AddVehComp(vehicleid,1000,1004,1005,1006,1008,1009,1010,1016,1086,1087);
    case 496:
    AddVehComp(vehicleid,1001,1002,1003,1006,1007,1008,1009,1010,1011,1017,1019,1020,1023,1086,1087);
    case 500:
    AddVehComp(vehicleid,1008,1009,1010,1013,1019,1020,1021,1024,1086,1087);
    case 505:
    AddVehComp(vehicleid,1000,1002,1004,1005,1006,1008,1009,1010,1013,1016,1018,1019,1020,1024,1086,1087);
    case 516:
    AddVehComp(vehicleid,1000,1002,1004,1007,1008,1009,1010,1015,1016,1017,1018,1019,1020,1021,1086,1087);
    case 517:
    AddVehComp(vehicleid,1002,1003,1007,1008,1009,1010,1016,1017,1018,1019,1020,1023,1086,1087,1142,1143,1144,1145);
    case 518:
    AddVehComp(vehicleid,1001,1003,1005,1006,1007,1008,1009,1010,1013,1017,1018,1020,1023,1086,1087,1142,1143,1144,1145);
    case 527:
    AddVehComp(vehicleid,1001,1007,1008,1009,1010,1014,1015,1017,1018,1020,1021,1086,1087);
    case 529:
    AddVehComp(vehicleid,1001,1003,1006,1007,1008,1009,1010,1011,1012,1017,1018,1019,1020,1023,1086,1087);
    case 534:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087,1100,1101,1106,1122,1123,1124,1125,1126,1127,1178,1179,1180,1185);
    case 535:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087,1109,1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1120,1121);
    case 536:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087,1103,1104,1105,1107,1108,1128,1181,1182,1183,1184);
    case 540:
    AddVehComp(vehicleid,1001,1004,1006,1007,1008,1009,1010,1017,1018,1019,1020,1023,1024,1086,1087,1142,1143,1144,1145);
    case 542:
    AddVehComp(vehicleid,1008,1009,1010,1014,1015,1018,1019,1020,1021,1086,1087,1142,1143,1144,1145);
    case 546:
    AddVehComp(vehicleid,1001,1002,1004,1006,1007,1008,1009,1010,1017,1018,1019,1023,1024,1086,1087,1142,1143,1144,1145);
    case 547:
    AddVehComp(vehicleid,1000,1003,1008,1009,1010,1016,1018,1019,1020,1021,1086,1087);
    case 549:
    AddVehComp(vehicleid,1001,1003,1007,1008,1009,1010,1011,1012,1017,1018,1019,1020,1023,1086,1087,1142,1143,1144,1145);
    case 550:
    AddVehComp(vehicleid,1001,1003,1004,1005,1006,1008,1009,1010,1018,1019,1020,1023,1086,1087,1142,1143,1144,1145);
    case 551:
    AddVehComp(vehicleid,1002,1003,1005,1006,1008,1009,1010,1016,1018,1019,1020,1021,1023,1086,1087);
    case 558:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,1163,1164,1165,1166,1167,1168);
    case 559:
    AddVehComp(vehicleid,1008,1009,1010,1065,1066,1067,1068,1069,1070,1071,1072,1086,1087,1158,1159,1160,1161,1162,1173);
    case 560:
    AddVehComp(vehicleid,1008,1009,1010,1026,1027,1028,1029,1030,1031,1032,1033,1086,1087,1138,1139,1140,1141,1169,1170);
    case 561:
    AddVehComp(vehicleid,1008,1009,1010,1055,1056,1057,1058,1059,1060,1061,1062,1063,1064,1086,1087,1154,1155,1156,1157);
    case 562:
    AddVehComp(vehicleid,1008,1009,1010,1034,1035,1036,1037,1038,1039,1040,1041,1086,1087,1146,1147,1148,1149,1171,1172);
    case 565:
    AddVehComp(vehicleid,1008,1009,1010,1045,1046,1047,1048,1049,1050,1051,1052,1053,1054,1086,1087,1150,1151,1152,1153);
    case 567:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087,1102,1129,1130,1131,1132,1133,1186,1187,1188,1189);
    case 575:
    AddVehComp(vehicleid,1008,1009,1010,1042,1043,1044,1086,1087,1099,1174,1175,1176,1177);
    case 576:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087,1134,1135,1136,1137,1190,1191,1192,1193);
    case 580:
    AddVehComp(vehicleid,1001,1006,1007,1008,1009,1010,1017,1018,1020,1023,1086,1087);
    case 585:
    AddVehComp(vehicleid,1001,1003,1006,1007,1008,1009,1010,1013,1017,1018,1019,1020,1023,1086,1087,1142,1143,1144,1145);
    case 589:
    AddVehComp(vehicleid,1000,1004,1005,1006,1007,1008,1009,1010,1013,1016,1017,1018,1020,1024,1086,1087,1142,1143,1144,1145);
    case 600:
    AddVehComp(vehicleid,1004,1005,1006,1007,1008,1009,1010,1013,1017,1018,1020,1022,1086,1087);
    case 603:
    AddVehComp(vehicleid,1001,1006,1007,1008,1009,1010,1017,1018,1019,1020,1023,1024,1086,1087,1142,1143,1144,1145);
    case 402,409,411,412,419,424,
    438,442,445,451,458,466,
    467,474,475,479,480,506,
    507,526,533,541,545,555,
    566,579,587,602:
    AddVehComp(vehicleid,1008,1009,1010,1086,1087);
    default:
    return 0;
  }
  return 1;
}

stock AddVehComp(vehicleid, ...)
{
  AddVehicleComponent(vehicleid, DATA_alufelgi[random(sizeof(DATA_alufelgi))]);

  new pjob=random(3);
  tVehicles[vehicleid][vo_paintjob]=pjob;
  ChangeVehiclePaintjob(vehicleid, pjob);

  for(new n = 1; n < numargs(); n++)
  AddVehicleComponent(vehicleid, getarg(n, 0));

  return 1;
}

vehicles_TunemenuShow(playerid){
  new vid=GetPlayerVehicleID(playerid);
  if (vid==0 || vid==INVALID_VEHICLE_ID)
    return Msg(playerid, COLOR_ERROR, "Komenda dostepna tylko w pojezdzie!");
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER)
    return Msg(playerid, COLOR_ERROR, "Komenda dostepna tylko dla kierowcy!");
  new listopts[1024];
  strcat( listopts,"{000000}A {ffffff}Auto /tune");
  strcat( listopts,"\n{000000}K {24B6FF}Kolor pojazdu");
  strcat( listopts,"\n{000000}N {38C3FB}Nitro");
  strcat( listopts,"\n{000000}F {8CD0D3}Alufelgi");
  switch(GetVehicleModel(vid)) {
    case 483,534,535,536,558,559,560,561,562,565,567,575,576: {
      strcat( listopts,"\n{000000}P {F1A623}Paintjob");
      if (tVehicles[vid][vo_paintjob]>0)
        strcat( listopts,"\n{000000}U {F1A623}Usun paintjob");
    }

  }


  return ShowPlayerDialog(playerid,DIALOG_TUNEMENU_MAIN, DIALOG_STYLE_LIST, "Tuning pojazdu", listopts, "OK", "Anuluj");
}

vehicles_TunemenuShowColorSel(playerid){
  new listopts[1024], buffer[64];
  for(new v = 0; v < sizeof vehicleColors; v++) {
      format(buffer, sizeof buffer, "{%06x}%d %s", vehicleColors[v][evc_value], vehicleColors[v][evc_id], vehicleColors[v][evc_name]);
      strcat(listopts, buffer);
      if(v < sizeof vehicleColors)
        strcat(listopts, "\n");
  }

  return ShowPlayerDialog(playerid,DIALOG_TUNEMENU_KOLOR1, DIALOG_STYLE_LIST, "Wybor koloru", listopts, "OK", "Anuluj");
}

vehicles_TunemenuResponse(playerid, dialogid, response, listitem, inputtext[]){
  new vid=GetPlayerVehicleID(playerid);
  if (vid==0 || vid==INVALID_VEHICLE_ID)
    return Msg(playerid,COLOR_ERROR,"Nie jestes w pojezdzie!");

  if (pData[playerid][pAttraction]!=A_NONE &&  pData[playerid][pAttraction]!=A_ARENA)
    return Msg(playerid,COLOR_ERROR,"Nie mozesz tego teraz zrobic!");

  if (dialogid==DIALOG_TUNEMENU_ALUFELGI) {
    if (!response) return vehicles_TunemenuShow(playerid);
    AddVehicleComponent(vid, DATA_alufelgi[listitem]);
    Msg(playerid,COLOR_INFO,"Alufelgi zostaly zmienione", false);
    PlaySound(playerid, 1133);
    return 1;
  } else if (dialogid==DIALOG_TUNEMENU_KOLOR1) {
    if (!response) return vehicles_TunemenuShow(playerid);
    new cval,cname[24];
    if (sscanf(inputtext,"ds[24]", cval,cname)) {
      Msg(playerid,COLOR_ERROR,"Nie udalo sie odczytac wybranego koloru, sprobuj ponownie");
      return vehicles_TunemenuShowColorSel(playerid);
    }

    if (vid==0 || vid==INVALID_VEHICLE_ID) return Msg(playerid, COLOR_ERROR, "Musisz byc w pojezdzie!");
    if (GetPlayerMoney(playerid)<2000) return Msg(playerid, COLOR_ERROR, "Ekspresowe malowanie kosztuje 2000$. Niestety, nie stac Cie na to. Sprobuj odwiedzic {b}/warsztat{/b}.");
    GivePlayerMoney(playerid,-2000);

    ChangeVehicleColor(vid, cval, cval);
    tVehicles[vid][vo_color][0]=cval;
    tVehicles[vid][vo_color][1]=cval;
    PlaySound(playerid, 1134);
    return 1;
  }
  if (!response)
    return 1;
  if (inputtext[0]=='A') {  // auto tune
    _tune(playerid);
    return vehicles_TunemenuShow(playerid);
  } else if (inputtext[0]=='K') {
    Msg(playerid,COLOR_INFO,"Pelny zakres uslug lakierniczych oferuje {b}/warsztat{/b}.", false);
    return vehicles_TunemenuShowColorSel(playerid);
  } else if (inputtext[0]=='N') {
    return _nos(playerid);
  } else if (inputtext[0]=='F') {
    return ShowPlayerDialog(playerid,DIALOG_TUNEMENU_ALUFELGI, DIALOG_STYLE_LIST, "Wybierz alufelgi:", DIALOGLIST_ALUFELGI,"OK", "Anuluj");
  } else if (inputtext[0]=='P' || inputtext[0]=='U') {  // paintjob

    if (vid==0 || vid==INVALID_VEHICLE_ID) return Msg(playerid, COLOR_ERROR, "Musisz byc w pojezdzie!");
    if (GetPlayerMoney(playerid)<2000) return Msg(playerid, COLOR_ERROR, "Ekspresowy paintjob kosztuje 2000$. Niestety, nie stac Cie na to. Sprobuj odwiedzic {b}/warsztat{/b}.");
    GivePlayerMoney(playerid,-2000);
    if (inputtext[0]=='U') {
      ChangeVehiclePaintjob(vid,0);
      tVehicles[vid][vo_paintjob]=0;
      return Msg(playerid, COLOR_INFO, "Paintjob zostal usuniety");
    }
    new mp=0;
    switch (GetVehicleModel(vid)) {
      case 483: mp=1; // camper
      case 575: mp=2; // broadway
      default: mp=3;  // pozostale
    }
    new r=random(mp)+1;
    ChangeVehiclePaintjob(vid,r);
    tVehicles[vid][vo_paintjob]=r;
    PlaySound(playerid, 1134);
    return Msg(playerid, COLOR_INFO, "Pojazd zostal pomalowany.");


  }
  return 1;
}

CMD:p(playerid,params[]){
  new txt[16];
  if (!IsGM(playerid) && !pData[playerid][vipEnabled])
    return Msg(playerid,COLOR_ERROR,"Komenda dostepna tylko dla VIPow.");

  if (sscanf(params,"s[16]",txt))
    return ShowPlayerDialog(playerid,DIALOG_POJAZDY_RECZNIE, DIALOG_STYLE_INPUT, "Wyszukiwanie pojazdu", "Wpisz nazwe pojazdu lub jej fragment:", "OK", "Anuluj");

  new vname[16];
  mysql_real_escape_string(txt,vname);
  new buffer[1024];
  new poziomy[32];
  if (IsAdmin(playerid))
    copy("'gracz','vip','gm','admin'", poziomy);
  else if (IsGM(playerid))
    copy("'gracz','vip','gm'", poziomy);
  else if (pData[playerid][vipEnabled])
    copy("'gracz','vip'", poziomy);

  if (strlen(vname)>3)
    format(buffer,sizeof buffer,"SELECT vid,name FROM fs_vehicles WHERE minPoziom IN (%s) AND (name LIKE '%%%s%%' OR name SOUNDS LIKE '%s' OR altnames LIKE '%%%s%%' OR altnames SOUNDS LIKE '%s') ORDER BY (name LIKE '%%%s%%')+(name SOUNDS LIKE '%s')+(altnames LIKE '%%%s%%')+(altnames SOUNDS LIKE '%s')+(name LIKE '%s%%') DESC LIMIT 10",
    poziomy, vname, vname,vname,vname,
       vname, vname,vname,vname, vname);
  else
    format(buffer,sizeof buffer,"SELECT vid,name FROM fs_vehicles WHERE minPoziom IN (%s) AND (vid='%s' OR name LIKE '%%%s%%' OR altnames LIKE '%%%s%%') ORDER BY (vid='%s')+((name like '%s%%')*2)+(name LIKE '%%%s%%')+(altnames LIKE '%%%s%%') DESC LIMIT 10",
    poziomy, vname, vname, vname, vname, vname, vname, vname);

  mysql_query(buffer);

  new vid,vehname[33];
  mysql_store_result();
  if (mysql_num_rows()==0)
    Msg(playerid,COLOR_ERROR,"Nie znaleziono zadnego pojazdu o pasujacej nazwie!");
  else if (mysql_num_rows()==1) {
    mysql_fetch_row_format(buffer," ");
        sscanf(buffer,"ds[32]",vid,vehname);
    if (vid>0) {
      new r;
      if (pData[playerid][adminLevel]>=LEVEL_ADMIN1 && GetPlayerState(playerid)==PLAYER_STATE_SPECTATING) // admin specujacy spawnuje pojazdy tam gdzie sie znajduje
        r=spawnVehicleNearPlayer(playerid,vid);
      else
        r=spawnVehicleForPlayer(playerid,vid,true);
      if (r==0 || r==INVALID_VEHICLE_ID)
        return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic!");
      format(buffer,sizeof buffer,"Utworzono pojazd %s", vehname);
      Msg(playerid,COLOR_INFO,buffer);
    }
  }
  else if (mysql_num_rows()>1) {
    new txt_lista[1023];
      for (new i=0;i<mysql_num_rows();i++){
      mysql_fetch_row_format(buffer," ");
          sscanf(buffer,"ds[32]",vid,vehname);
      if (i==0)
        format(txt_lista,sizeof txt_lista,"{000000}%d{ffffff} %s",vid,vehname);
      else
        format(txt_lista,sizeof txt_lista,"%s\n{000000}%d{ffffff} %s",txt_lista,vid,vehname);


    }
    ShowPlayerDialog(playerid,DIALOG_POJAZDY_LISTA,DIALOG_STYLE_LIST,"Znalezione pojazdy:",txt_lista,"OK","Anuluj");
  }
  mysql_free_result();
  return 1;
}

CMD:pojazdy(playerid){
  return ShowPlayerDialog(playerid,DIALOG_POJAZDY_TYP,DIALOG_STYLE_LIST,"Wybierz rodzaj pojazdu:",txt_rodzaje,"OK","Anuluj");
}

CMD:cars(playerid)
  return _pojazdy(playerid);

MenuPojazdow(playerid,listitem)
{
  ShowPlayerDialog(playerid,DIALOG_POJAZDY_RODZAJE+listitem-1, DIALOG_STYLE_LIST, "Wybierz rodzaj pojazdu:", txt_pojazdy[listitem-1], "OK", "Anuluj");
  return 1;
}

ZespawnujPojazd(playerid,rodzaj,listitem){
  if(GetPlayerMoney(playerid)<1000) return Msg(playerid,COLOR_ERROR,"Koszt spawnu pojazdu to $1000. Niestety nie stac Cie na taki wydatek.");
  new r=spawnVehicleForPlayer(playerid,pojazdy[rodzaj][listitem],true);
  if (r==0 || r==INVALID_VEHICLE_ID)
    return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic");
  Msg(playerid,COLOR_INFO,"Koszt spawnu pojazdu to $1000. Ta suma zosta³a automatycznie pobrana z Twojego konta.");
  GivePlayerMoney(playerid,-1000);
  return 1;
}

vehicles_PojazdyResponse(playerid,dialogid,response,listitem,inputtext[]){
  switch(dialogid){
    case DIALOG_POJAZDY_AD: {
      if (!response) return 1;
      new vid,vname[33];
      sscanf(inputtext,"ds[32]",vid,vname);
      if (vid>=400) {
        new r=spawnVehicleForPlayer(playerid,vid,true);
        if (r==0 || r==INVALID_VEHICLE_ID)
          return SendClientMessage(playerid,-1,"Nie mozesz teraz tego zrobic!");
      }
      return 1;
    }
    case DIALOG_POJAZDY_RECZNIE: {
      if(!response) return 1;
      return _p(playerid,inputtext);
    }

    case DIALOG_POJAZDY_LISTA: {
      if (!response) return 1;
      new vid,vname[33];
      sscanf(inputtext,"ds[32]",vid,vname);
      if (vid>=400) {
        new r=spawnVehicleForPlayer(playerid,vid,true);
        if (r==0 || r==INVALID_VEHICLE_ID)
          return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic");
      }
      return 1;
    }
    case DIALOG_POJAZDY_TYP: {
      if (!response) return 1;
      switch(listitem){
        case 0:
          return _p(playerid,"");
        default:
          return MenuPojazdow(playerid,listitem);
      }
    }
  }
  if (dialogid>=DIALOG_POJAZDY_RODZAJE && dialogid<=DIALOG_POJAZDY_RODZAJE+sizeof(pojazdy)) {
    if (!response) return _pojazdy(playerid);
    return ZespawnujPojazd(playerid,dialogid-DIALOG_POJAZDY_RODZAJE,listitem);
  }
  return 0;
}

stock vehicles_getNeonOffset(type,&Float:ox,&Float:oy,&Float:oz){
	if(type==0) ox = -0.8; oy = 0.0; oz = -0.70;
  if(type==1) ox = 0.8; oy = 0.0; oz = -0.70;
}

public vehicles_EngageTurbo(playerid,vehicleid,step){
  if (!pData[playerid][loggedIn]) {
    Msg(playerid,COLOR_ERROR,"Aby skorzystac z {b}TURBO{/b}, musisz sie zarejestrowac. Wpisz {b}/rej{/b}");
    return;
  }

  if (step==0 && !tVehicles[vehicleid][vo_hasTurbo])  return;
  if (step>0 && (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER || !tVehicles[vehicleid][vo_hasTurbo]))
    step=19;

  switch(step){
    case 0: {
      if (GetTickCount()-pTemp[playerid][vehicleSpecialLastUsed]<7000) return;

      if (!pData[playerid][loggedIn]) {
        Msg(playerid,COLOR_ERROR,"Aby skorzystac z {b}TURBO{/b}, musisz sie zarejestrowac. Wpisz {b}/rej{/b}");
        return;
      }

      new spd=GetVehicleSpeed(vehicleid);
      if (spd<10) return;
      if (spd<80) {
        PlaySound(playerid,1190);
        GameTextForPlayer(playerid,"~n~~n~~n~~g~Jedziesz zbyt wolno",500,5);
        return;
      } else if (spd>300)
        return;

      pTemp[playerid][vehicleSpecialLastUsed]=GetTickCount();

      SetPlayerAttachedObject( playerid, ATTACH_TMPMISC, 18718, 1, 0.000000, 0.500000, 0.000000, 90.000000, 335.000000, 25.000000, 1.000000, 1.000000, 1.000000 );
      vehicles_EngageTurbo(playerid, vehicleid, step+1);
      return;
    }
    case 1..18: {
      new Float:PV[3],Float:VA;

      new Keys,ud,lr;
      GetPlayerKeys(playerid,Keys,ud,lr);

      new spd=GetVehicleSpeed(vehicleid);
      GetVehicleVelocity(vehicleid,PV[0],PV[1],PV[2]);
      GetVehicleZAngle(vehicleid,VA);

      if (step==1) {
        new Float:PP[3];
        GetVehiclePos(vehicleid,PP[0],PP[1],PP[2]);
        CreateExplosion(PP[0]-(PV[0]*4), PP[1]-(PV[1]*4), PP[2]-PV[2],12,1.0);
      }
      PV[0]+=(0.3*floatsin(-VA,degrees));
      PV[1]+=(0.3*floatcos(-VA,degrees));

      if (Keys&KEY_SPRINT==KEY_SPRINT) {
        if (spd<350)
          SetVehicleVelocity(vehicleid,PV[0],PV[1],PV[2]*1.05);
      } else if (Keys&KEY_JUMP!=KEY_JUMP && random(2)==1 && step>1)
        step--;
      else if (Keys&KEY_JUMP==KEY_JUMP && step<18) step++;  // hamulec



      SetTimerEx("vehicles_EngageTurbo", Keys&KEY_SPRINT==KEY_SPRINT ? 130 : 250, false, "ddd", playerid, vehicleid, step+1);
    }
    case 19: {
      RemovePlayerAttachedObject(playerid, ATTACH_TMPMISC);
      artefact_SetPlayerHolding(playerid);
      return;
    }

  }

  return;
}

forward vehicles_AddNitro(vid);
public vehicles_AddNitro(vid)
  return AddVehicleComponent(vid, 1010);

stock vehicles_wyrzutnia(playerid,checkpointid){
#pragma unused checkpointid
  new vid=GetPlayerVehicleID(playerid);
  if (vid==0 || vid==INVALID_VEHICLE_ID)
    return 1;
  new Float:PV[3];
  GetVehicleVelocity(vid,PV[0],PV[1],PV[2]);
  SetVehicleVelocity(vid,PV[0],PV[1]*8,PV[2]*2);
  return 1;

}

forward vehicles_LoadStatic(sourceId);
public vehicles_LoadStatic(sourceId){
  staticVehicleCount = CallRemoteFunction("@@@export@getStaticVehicleCount","d",0); // pad!
  for(new v=0; v<staticVehicleCount; v++) {
    tVehicles[v][vo_static]=true;
    tVehicles[v][vo_driver]=INVALID_PLAYER_ID;
    SetVehicleParamsEx(v, VEHICLE_PARAMS_ON, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, DOOR_OPENED, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF);
    SetVehicleHealth(v, VEHICLE_DEFAULT_HP);
  }
  ////////////////// neony ////////////////////////
  /*new Float:neonX[2],Float:neonY[2],Float:neonZ[2];
  vehicles_getNeonOffset(0,neonX[0],neonY[0],neonZ[0]);
  vehicles_getNeonOffset(1,neonX[1],neonY[1],neonZ[1]);
  
  for(new i=0;i<10;i++) {
    new neonVehicle = random(staticVehicleCount)+1;
    new vf=GetVehicleFlags(neonVehicle);
    while(vf&VF_STREET!=VF_STREET)
    {
      neonVehicle = random(staticVehicleCount)+1;
      vf=GetVehicleFlags(neonVehicle);
    }
    AttachObjectToVehicle(neony[i][0], neonVehicle, neonX[0], neonY[0], neonZ[0], 0.0, 0.0, 0.0);
    AttachObjectToVehicle(neony[i][1], neonVehicle, neonX[1], neonY[1], neonZ[1], 0.0, 0.0, 0.0);
    neony[i][2]=neonVehicle;
  }*/
}
// EOF