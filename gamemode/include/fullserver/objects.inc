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

#define MIN_PICKUP_DELAY 2500
#define MIN_TELECP_DELAY 4000

forward Pobiekty_odswiezPomniki();

enum e_telepickup {
  etp_pickupid,
  Float:etp_tPX,  Float:etp_tPY,  Float:etp_tPZ,  Float:etp_tPA,
  etp_tI, etp_tVW,
  bool:etp_pokazopis,
  etp_opis[64],
  Text3D:etp_t3d,
  etp_mapicon,
  etp_restrict_gang,
  etp_restrict_level
}

#define MAX_TELEPICKUPS 600
#define MAX_RETURNPICKUPS 500
#define MAX_TELECHECKPOINTS 300
#define MAX_MISCPICKUPS 890
#define MAX_MAPICONS 450

new TelePickupsCount=0, ReturnPickupsCount=0, TeleCPCount=0, MapIconsCount=0, MiscPickupsCount=0;
new TelePickups[MAX_TELEPICKUPS][e_telepickup];
new ReturnPickups[MAX_RETURNPICKUPS];
new MiscPickups[MAX_MISCPICKUPS];
new MapIcons[MAX_MAPICONS];

enum e_telecheckpoint {
  etcp_cp[2],
  Float:etcp_P1[4],
  Float:etcp_P2[4],
  etcp_I[2],
  etcp_opis1[64],
  etcp_opis2[64],
  Text3D:etcp_t3d[2]
}

new TeleCheckpoints[MAX_TELECHECKPOINTS][e_telecheckpoint];


enum e_DynamicAreas {
  eDA_andromeda,
  eDA_jail,
  eDA_tuneleKrzyk,  // prostopadloscian o wysokosc 1j, przecinajacy kanaly - po przejsciu przez niego rozlegnie sie krzyk
  eDA_getto
}

enum e_Pickups {
  eP_F_policja_LS,
  eP_F_policja_LV,
  eP_F_policja_SF,
  eP_F_wojsko1,
  eP_F_wojsko2,
  eP_F_dilerzy,
  eP_F_mechanicy,
  eP_F_wojsko3,
  eP_F_Piloci_LV,
  eP_F_Piloci_LS,
  eP_F_Piloci_SF,
  eP_F_Maszynisci_1,
  eP_F_Maszynisci_2,
  eP_F_Maszynisci_3,
  eP_F_Maszynisci_4,
  eP_F_kierowca_1,
  eP_F_kierowca_2,
  eP_F_kierowca_3,
  eP_F_kierowca_4,
  eP_F_kierowca_5
}

enum e_Object {
  eO_przenosna_strefa,
  eO_przenosna_strefaDM,
}

enum e_StrefyNoDM {
  ea_warsztat,
  ea_statek,
  ea_kosciol,
  ea_kosciolINT,
  ea_wyspa,
  ea_bankPC,    // palomino creek
  ea_hacjenda,
  ea_burdel,
  ea_skatepark,
  ea_szpital,
  ea_przenosna,   // przenosna strefa bez DM
  ea_klatka,  // klatka KSW/ZTP/WWK
  ea_WWK, // walki w klatkach
  ea_KSW, // w onplayerdeath jest specjalny wyjatek aby nie pakowac do paki za zabojstwo tutaj
  ea_wiezienie,
  //ea_urodziny2013
}

enum e_StrefyFullDM {
  ea_wojsko,
  ea_minigun,
  ea_kopalnia,
  ea_arenasolo1,
  ea_tunele,
  ea_wietnam,
  ea_przenosna, // przenosna strefa smierci
  ea_marynarka
}

enum e_PomnikiRodzaje {
  ea_misc1,
  ea_czasgry,
  ea_skill,
  ea_lowskill,
  ea_derby,
  ea_kickban,
  ea_racedist,
  ea_soloklrs,  // solo killers
  ea_fotocash,
  ea_walizki,
  ea_cash,
  ea_listy
}

enum e_checkpoints {
  ec_warsztat_tablice,
  ec_warsztatlv_skup,
  ec_warsztatlv_myjnia,
  ec_warsztatlv_podnosnik,
  ec_warsztatlv_spray,
  ec_sfrakieta
}

new DAs[e_DynamicAreas];  // rozne strefy dynamiczne
new Os[e_Object];
new StrefyNoDM[e_StrefyNoDM];
new StrefyFullDM[e_StrefyFullDM];
new PICKUPs[e_Pickups];   // rozne pickupy
new Text3D:TekstyPomniki[e_PomnikiRodzaje][4];
new Text3D:PPRanking[3];

new Text3D:ArenaRanking[ARENA_MAX][2];  // ranking najlepszych na danej arenie

new Checkpoints[e_checkpoints];
new Fotoradary[9];

new KillZones[11];  // strefy po wejsciu do ktorych nastepuje smierc - np. spadniecie pod arene na derby

new neony[10][3];
new podnosniki[3][6];
new bool:podnosniki_stan[3];

CreateObjects()
{
  // neony
  for(new i=0;i<10;i++) {
    neony[i][0]=CreateDynamicObject(18647+(i%6),0,0,0,0,0,0);
    neony[i][1]=CreateDynamicObject(18647+(i%6),0,0,0,0,0,0);
    neony[i][2]=INVALID_VEHICLE_ID;
  }
  rakieta_init();
  balony_init();
  poczta_init();

  podnosniki[0][0]=CreateDynamicObject(8851,1083.61914062,1735.26416016,12.00000000,0.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[0][1]=CreateDynamicObject(8851,1083.61914062,1735.26367188,12.00000000,180.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[0][2]=CreateDynamicObject(16091,1083.65625000,1735.17016602,11.20584679,0.00000000,179.99450684,0.00000000,0,0,-1); //object(des_pipestrut04) (2) - 13.5
  podnosniki[0][3]=CreateDynamicObject(8851,1086.31323242,1735.26416016,12.00000000,0.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[0][4]=CreateDynamicObject(8851,1086.31250000,1735.26367188,12.00000000,180.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[0][5]=CreateDynamicObject(16091,1086.34497070,1735.17016602,11.20584679,0.00000000,180.00000000,0.00000000,0,0,-1); //object(des_pipestrut04) (1) - 13.5

  podnosniki[1][0]=CreateDynamicObject(8851,1077.13085938,1735.26416016,12.00000000,0.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[1][1]=CreateDynamicObject(8851,1077.13085938,1735.26367188,12.00000000,180.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[1][2]=CreateDynamicObject(16091,1077.17089844,1735.17016602,11.20584679,0.00000000,179.99450684,0.00000000,0,0,-1); //object(des_pipestrut04) (4) - 13.5
  podnosniki[1][3]=CreateDynamicObject(8851,1079.63183594,1735.26416016,12.00000000,0.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[1][4]=CreateDynamicObject(8851,1079.63183594,1735.26367188,12.00000000,180.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 13.5
  podnosniki[1][5]=CreateDynamicObject(16091,1079.67309570,1735.17016602,11.20584679,0.00000000,179.99450684,0.00000000,0,0,-1); //object(des_pipestrut04) (3) - 14.3

  podnosniki[2][0]=CreateDynamicObject(8851,1070.65234375,1735.26416016,12.00000000,0.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[2][1]=CreateDynamicObject(8851,1070.65234375,1735.26367188,12.00000000,180.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[2][2]=CreateDynamicObject(16091,1070.68310547,1735.17016602,11.20584679,0.00000000,179.99450684,0.00000000,0,0,-1); //object(des_pipestrut04) (6) - 13.5
  podnosniki[2][3]=CreateDynamicObject(8851,1074.68457031,1735.26367188,12.00000000,0.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[2][4]=CreateDynamicObject(8851,1074.68457031,1735.26367188,12.00000000,180.00000000,0.00000000,0.00000000,0,0,-1); //object(vgeplntr01_lvs) (1) - 14.3
  podnosniki[2][5]=CreateDynamicObject(16091,1074.71484375,1735.17016602,11.20584679,0.00000000,180.00000000,0.00000000,0,0,-1); //object(des_pipestrut04) (5) - 13.5

  PICKUPs[eP_F_policja_LS]=CreateDynamicPickup(1247,2,256.86,69.47,1003.64,0,6,-1,150.0);
  PICKUPs[eP_F_policja_LV]=CreateDynamicPickup(1247,2,229.13,164.49,1003.02,0,3,-1,150.0);
  PICKUPs[eP_F_policja_SF]=CreateDynamicPickup(1247,2,246.79,123.24,1003.21,0,10,-1,150.0);

  PICKUPs[eP_F_wojsko1]=CreateDynamicPickup(1254,2,211.6909,1814.8363,21.8672,0,0,-1,150.0);
  PICKUPs[eP_F_wojsko2]=CreateDynamicPickup(1254,2,223.80,1822.85,6.41,0,0,-1,150.0);

  PICKUPs[eP_F_dilerzy]=CreateDynamicPickup(1241,2,264.65,1395.45,-7.90,0,0,-1,150.0);
  PICKUPs[eP_F_wojsko3]=CreateDynamicPickup(1254,2,246.79,123.24,1003.21,0,10,-1,150.0);

  PICKUPs[eP_F_mechanicy]=CreateDynamicPickup(1247,2,1049.71,1744.19,12.43,0,0,-1,25.0);

  PICKUPs[eP_F_Piloci_LV]=CreateDynamicPickup(1310,2,1306.8137,1252.0004,10.8203,0,0,-1,25.0);
  PICKUPs[eP_F_Piloci_LS]=CreateDynamicPickup(1310,2,1954.04,-2177.17,13.54,0,0,-1,25.0);
  PICKUPs[eP_F_Piloci_SF]=CreateDynamicPickup(1310,2,-1221.43,60.21,14.13,0,0,-1,25.0);

  PICKUPs[eP_F_Maszynisci_1]=CreateDynamicPickup(1314,2,1433.55, 2645.07, 11.39,0,0,-1,25.0); //lv
  PICKUPs[eP_F_Maszynisci_2]=CreateDynamicPickup(1314,2,2858.88, 1278.88, 11.39,0,0,-1,25.0); //lv2
  PICKUPs[eP_F_Maszynisci_3]=CreateDynamicPickup(1314,2,1704.45, -1948.76, 14.11,0,0,-1,25.0); //ls
  PICKUPs[eP_F_Maszynisci_4]=CreateDynamicPickup(1314,2,-1968.74, 112.24, 27.68,0,0,-1,25.0); //sf

  PICKUPs[eP_F_kierowca_1]=CreateDynamicPickup(330,2,642.1879,1235.5638,11.6784,0,0,-1,25.0);
  PICKUPs[eP_F_kierowca_2]=CreateDynamicPickup(330,2,-467.1047,-480.3860,25.5178,0,0,-1,25.0);
  PICKUPs[eP_F_kierowca_3]=CreateDynamicPickup(330,2,-242.2922,-236.0676,2.4297,0,0,-1,25.0);
  PICKUPs[eP_F_kierowca_4]=CreateDynamicPickup(330,2,2813.8262,891.4141,10.7578,0,0,-1,25.0);
  PICKUPs[eP_F_kierowca_5]=CreateDynamicPickup(330,2,2211.8435,-2285.0813,14.7647,0,0,-1,25.0);

  // rankingi przestepcow na posterunkach
  PPRanking[0]=CreateDynamic3DTextLabel("Poszukiwani przestepcy\n\n{ffffff}- brak",0xff0000ff, 252.9, 113, 1006, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 10, -1, 50);
  PPRanking[1]=CreateDynamic3DTextLabel("Poszukiwani przestepcy\n\n{ffffff}- brak",0xff0000ff, 228.6, 148.5, 1004.6, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, 3, -1, 50);
  PPRanking[2]=CreateDynamic3DTextLabel("Poszukiwani przestepcy\n\n{ffffff}- brak",0xff0000ff, 255.18, 62.7, 1005.0, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, 6, -1, 50);

  ArenaRanking[ARENA_ONEDE][0]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE\n\n{ffffff}",0xff0000ff, 265.38, 187.46, 1009.47, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, VW_ARENA, 3, -1, 20);
  ArenaRanking[ARENA_ONEDE][1]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE\nw tym tygodniu\n{ffffff}",0xff0000ff, 198.0019,172.1439,1004.7749, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, VW_ARENA, 3, -1, 20);

  ArenaRanking[ARENA_ONEDE2][0]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE2\n\n{ffffff}",0xff0000ff, 265.38, 187.46, 1009.47, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, VW_ARENA2, 3, -1, 20);
  ArenaRanking[ARENA_ONEDE2][1]=CreateDynamic3DTextLabel("Najlepsi na /ONEDE2\nw tym tygodniu\n{ffffff}",0xff0000ff, 198.0019,172.1439,1004.7749, 50, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, VW_ARENA2, 3, -1, 20);

  Checkpoints[ec_warsztat_tablice]=CreateDynamicCP(1040.22,1778.65,11.75,1.5,0,0,-1,20.0);
  Checkpoints[ec_warsztatlv_myjnia]=CreateDynamicCP(1107.62,1762.57,11.40,4.0,0,0,-1,20.0);
  Checkpoints[ec_warsztatlv_podnosnik]=CreateDynamicCP(1070.96,1749.25,12.90, 1.0, 0,0,-1,20.0);

  Checkpoints[ec_warsztatlv_spray]=CreateDynamicCP(1052.3052,1728.5016,12.4370, 1.0, 0,0,-1,20.0);
  CreateDynamic3DTextLabel("Podnosnik", 0xffffffff, 1070.96,1749.23,13.5, 20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0, -1, 20);
  Checkpoints[ec_warsztatlv_skup]=CreateDynamicCP(1064,1735,12.43,7.0, 0,0,-1,20.0);
  Checkpoints[ec_warsztatlv_skup]=CreateDynamicCP(886.9,1911.5,10.00,7.0, 0,0,-1,20.0);
  CreateDynamic3DTextLabel("Skup\npojazdow", 0xffffffff, 886.9,1911.5,10.9, 20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0, -1, 20);
  
  Checkpoints[ec_sfrakieta]=CreateDynamicCP(-2898.35,489.05,30.5,1.0,    0,0,-1,20.0);
  CreateDynamic3DTextLabel("Start\nrakiety", 0xffffffff, -2898.35,489.05,30.9, 20, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0, -1, 20);

  Fotoradary[0]=CreateDynamicSphere(2057.50,1303.54,10.93,20,-1,0,-1);
  Fotoradary[1]=CreateDynamicSphere(1217.44,1804.47,7.62,20,-1,0,-1);
  Fotoradary[2]=CreateDynamicSphere(858.23,-1777.20,13.77,20,-1,0,-1);
  Fotoradary[3]=CreateDynamicSphere(2878.24,-1076.36,10.88,20,-1,0,-1);
  Fotoradary[4]=CreateDynamicSphere(-2316.96,1067.26,55.84,30,-1,0,-1);
  Fotoradary[5]=CreateDynamicSphere(-1013.58,1267.92,33.37,40,-1,0,-1);
  Fotoradary[6]=CreateDynamicSphere(91.33, 674.53,5.62,40,-1,0,-1);
  Fotoradary[7]=CreateDynamicSphere(2232.12,-1576.44,2.57,40,-1,0,-1);
  Fotoradary[8]=CreateDynamicSphere(-1904.03,-626.86,40.26,40,-1,0,-1);

  // no dm
  StrefyNoDM[ea_przenosna]=CreateDynamicSphere(0,0,0,1,0,0,-1);

  // 0x22BAFBFF - no-dm
  // 0x840B0BFF - full-dm

  // stock RoundGangZoneForAll(const Float: radius, const Float: X, const Float: Y, const color, const size = 20) {
  // stock NormalGangZoneForAll(const Float: minX , const Float: minY, const Float: maxX, const Float: maxY, const color) {

  // normal
  // StrefyNoDM[ea_bankPC]=CreateDynamicCube(2299.0,-24.0,20,  2324.0,0.5,30.0);
  // NormalGangZoneForAll(2299.0, -24.0, 2324.0, 0.5, 0x22BAFBFF);
  StrefyNoDM[ea_bankPC]=CreateDynamicSphere(2293.9817,-14.6879,26.3359,30);
  RoundGangZoneForAll(30,2293.9817,-14.6879, 0x22BAFBFF);

  StrefyNoDM[ea_skatepark]=CreateDynamicCube(1862,-1450.5,8,  1976.6,-1351,47);
  NormalGangZoneForAll(1862, -1450.5, 1976.6, -1351, 0x22BAFBFF);

  StrefyNoDM[ea_warsztat]=CreateDynamicCube(1014,1716,9,1119,1842,35, 0,0,-1);
  NormalGangZoneForAll(1014, 1716, 1119, 1842, 0x22BAFBFF);

  StrefyNoDM[ea_statek]=CreateDynamicCube(1992.69,  1513.90, 9.20,  2007.89, 1574.56, 45.00,  0,0,-1);
  gmTemp[firstZoneIdNoDM]=NormalGangZoneForAll(1992.69, 1513.90, 2007.89, 1574.56, 0x22BAFBFF);

  /*StrefyNoDM[ea_kart]=CreateDynamicCube(-2095,-280,34,-2010,-90, 60,0,0,-1);
  NormalGangZoneForAll(-2095, -280, -2010, -90, 0x22BAFBFF);*/

  StrefyNoDM[ea_kosciol]=CreateDynamicSphere(-2751,-294,7,65, 0,0,-1);
  RoundGangZoneForAll(65, -2751, -294, 0x22BAFBFF);

  StrefyNoDM[ea_kosciolINT]=CreateDynamicSphere(1964,-360,1092,50,  0,1,-1);
  // brak widocznej strefy

  StrefyNoDM[ea_burdel]=CreateDynamicSphere(950,-51,1001,15);
  // brak widocznej strefy

  StrefyNoDM[ea_klatka]=CreateDynamicSphere(1266.90,-815.0,116.0,45, 18,0,-1);
  // brak widocznej strefy

  StrefyNoDM[ea_WWK]=CreateDynamicSphere(1227.60,-787.3,97.0,45, 17,0,-1);
  // brak widoczne strefy

  StrefyNoDM[ea_KSW]=CreateDynamicSphere(1274.0,-786.4,126.3,45, 15,0,-1);
  // brak widocznej strefy

  StrefyNoDM[ea_wiezienie]=CreateDynamicSphere(-1952.83, 6823.52, 13.65, 120, VW_JAIL,0,-1);
  RoundGangZoneForAll(120, -1952.83, 6823.52, 0x22BAFBFF);

  /*StrefyNoDM[ea_urodziny2013]=CreateDynamicSphere(331.8578,-1802.0775, 4, 320, 0,0,-1);
  RoundGangZoneForAll(320, 331.8578,-1802.0775, 0x22BAFBFF);*/

  StrefyNoDM[ea_wyspa]=CreateDynamicSphere(3500,430,0, 80,  0,0,-1);
  gmTemp[lastZoneIdNoDM]=RoundGangZoneForAll(80, 3500, 430, 0x22BAFBFF);

  // full dm
  StrefyFullDM[ea_przenosna]=CreateDynamicSphere(0,0,0,1,0,0,-1);

  // normal
  StrefyFullDM[ea_wojsko]=CreateDynamicRectangle(-136.32, 1620.74, 418.40,2164.52,    0,    0,-1);
  gmTemp[firstZoneIdFullDM]=NormalGangZoneForAll(-136.32, 1620.74, 418.40, 2164.52, 0x840B0BFF);

  // round
  StrefyFullDM[ea_minigun]=CreateDynamicSphere(2613.0,2729.0,36.5, 170, 0,0,-1);
  RoundGangZoneForAll(170, 2613.0, 2729.0, 0x840B0BFF);

  StrefyFullDM[ea_kopalnia]=CreateDynamicSphere(606.0,868.0,-40.0, 170, 0,0,-1);
  RoundGangZoneForAll(170, 606.0, 868.0, 0x840B0BFF);

  StrefyFullDM[ea_arenasolo1]=CreateDynamicCircle(-1102.00,-986.00,40.0,  -1, 9,-1);
  // brak widocznej strefy

  StrefyFullDM[ea_tunele]=CreateDynamicSphere( 2464.79,2610.81,-71.56,  300,  -1, 11,-1);
  // brak widocznej strefy

  StrefyFullDM[ea_wietnam]=CreateDynamicSphere( -641.98,-1945,25.88,  235,  0,  0,-1);
  RoundGangZoneForAll(235, -641.98, -1945, 0x840B0BFF);

  StrefyFullDM[ea_marynarka]=CreateDynamicSphere(-1438.7457,360.4542,30.0859, 250,  0,  0,-1);
  gmTemp[lastZoneIdFullDM]=RoundGangZoneForAll(250, -1438.7457, 360.4542, 0x840B0BFF);

  // inne
  DAs[eDA_tuneleKrzyk]=CreateDynamicCube(2372, 2478, -68, 2698, 2781, -67,  -1, 11,-1);
  DAs[eDA_andromeda]=CreateDynamicSphere(315,1050,1935,20,-1,5,-1);
  CreateDynamicObject( 18674,     316.00,1035.00,1935.00,   5.00,5.00,0.00, -1,9,-1); // chmury_andromeda

  DAs[eDA_jail]=CreateDynamicRectangle(-2005,6797.21,-1889.31,  6865.63,VW_JAIL,0,-1);

  CreateDynamicObject(16192, -4706+8206.0215, -1661.8997+2161, -39.7614, 0.8594, 354.8434, 358.2811,  0,0,-1,600);
  CreateDynamicObject(16061, -4706+8215.8184, -1678.8613+2161, 3.1932, 0.0, 0.0, 0.0, 0,0,-1,300);  // drzewo
  CreateDynamicObject(16061, -4706+8236.5693, -1700.8350+2161, 1.4077, 0.0, 1.7189, 289.5262, 0,0,-1,300);
  CreateDynamicObject(16061, -4706+8198.5410, -1683.7899+2161, 2.5793, 0.0, 0.0, 0.0, 0,0,-1,300);
  CreateDynamicObject(739, -4706+8198.6855, -1718.5924+2161, -37.9492, 0.0, 0.0, 0.0, 0,0,-1,300);
  CreateDynamicObject(619, -4706+8234.5820, -1731.2274+2161, 3.4925, 0.0, 0.0, 0.0, 0,0,-1,200);
  CreateDynamicObject(18259, -4706+8215.6787, -1724.3081+2161, 5.9702, 0.0, 0.0, 84.2248, 0,0,-1,300);
  CreateDynamicObject(1481, -4706+8224.1533, -1730.5194+2161, 6.7092, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1495, -4706+8214.9482, -1729.4093+2161, 6.0508, 0.0, 0.0, 208.7390, 0,0,-1,150);
  CreateDynamicObject(14805, -4706+8221.1162, -1725.1243+2161, 6.9429, 0.0, 0.0, 0.0, 0,0,-1,150);
  CreateDynamicObject(2297, -4706+8221.3672, -1720.1305+2161, 6.0501, 0.0, 0.0, 315.3093, 0,0,-1,150);
  CreateDynamicObject(1667, -4706+8221.9932, -1726.3534+2161, 6.8197, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1828, -4706+8222.0391, -1722.8575+2161, 6.0288, 0.0, 0.0, 84.1208,  0,0,-1,150);
  CreateDynamicObject(1829, -4706+8224.0840, -1721.6119+2161, 6.5206, 0.0, 1.7189, 268.0403,  0,0,-1,150);
  CreateDynamicObject(14527, -4706+8221.2822, -1724.7813+2161, 7.0098, 0.0, 0.0, 0.0, 0,0,-1,150);
  CreateDynamicObject(1815, -4706+8223.3389, -1728.3270+2161, 6.0553, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1668, -4706+8224.0420, -1727.6943+2161, 6.7184, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1669, -4706+8224.0439, -1727.8624+2161, 6.7184, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1667, -4706+8223.5752, -1727.6796+2161, 6.6403, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1664, -4706+8224.2734, -1727.8196+2161, 6.6934, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1665, -4706+8223.7666, -1727.9882+2161, 6.5872, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(1512, -4706+8224.1240, -1728.0743+2161, 6.7242, 0.0, 0.0, 0.0,  0,0,-1,150);
  CreateDynamicObject(2344, -4706+8218.9346, -1725.0416+2161, 32.8490, 0.0, 0.0, 0.0, 0,0,-1,150);
  CreateDynamicObject(14446, -4706+8210.6387, -1724.0170+2161, 6.6564, 0.0, 0.0, 86.6991, 0,0,-1,150);
  CreateDynamicObject(1742, -4706+8208.6084, -1728.1372+2161, 6.0516, 0.0, 0.0, 83.2614);
  CreateDynamicObject(2190, -4706+8210.1758, -1719.8883+2161, 6.8767, 0.0, 0.0, 0.0);
  CreateDynamicObject(2568, -4706+8209.7500, -1720.0713+2161, 6.0483, 0.0, 0.0, 0.0);
  CreateDynamicObject(1671, -4706+8210.4912, -1720.9667+2161, 6.4644, 0.0, 0.0, 210.4580);
  CreateDynamicObject(643, -4706+8222.7764, -1734.0599+2161, 6.5016, 0.0, 0.0, 0.0);
  CreateDynamicObject(1280, -4706+8222.3604, -1730.1859+2161, 6.4575, 0.0, 0.0, 88.4180);
  CreateDynamicObject(1243, -4706+8185.5571, -1824.7396+2161, -1.9524, 0.0, 0.0, 0.0);
  CreateDynamicObject(1243, -4706+8148.4775, -1789.2531+2161, -2.5265, 0.0, 0.0, 0.0);
  CreateDynamicObject(11496, -4706+8201.7275, -1768.8059+2161, 1.7526, 0.8594, 0.0, 226.7873);

  CreateDynamicObject(1608, -4706+8190.7637, -1793.9025+2161, -1.3131, 0.0, 0.0, 0.0, 0,0,-1,300);    // rekiny
  CreateDynamicObject(1608, -4706+8188.0542, -1778.4365+2161, -1.3246, 4.2972, 0.0, 249.1327, 0,0,-1,300);
  CreateDynamicObject(1608, -4706+8173.2729, -1794.7434+2161, -1.2060, 0.0, 0.0, 0.0, 0,0,-1,300);

  CreateDynamicObject(16097, -4706+8270.3359, -1702.6177+2161, -10.9683, 0.0, 0.0, 0.0,0,0,-1,600); // lad
  CreateDynamicObject(1458, -4706+8219.5928, -1740.3041+2161, 5.8318, 0.0, 0.0, 0.0);
  CreateDynamicObject(1457, -4706+8232.0752, -1728.2046+2161, 7.2815, 0.0, 0.0, 0.0);
  CreateDynamicObject(3461, -4706+8218.5352, -1759.6958+2161, 3.6359, 0.0, 0.0, 0.0);
  CreateDynamicObject(2035, -4706+8222.0869, -1725.6338+2161, 6.7555, 0.0, 0.0, 0.0);
  CreateDynamicObject(854, -4706+8218.7852, -1759.3384+2161, 5.2185, 0.0, 0.0, 0.0);
  CreateDynamicObject(1280, -4706+8220.2988, -1759.7511+2161, 5.6858, 0.0, 0.0, 0.0);

  /*KillZones[2]=CreateDynamicSphere(-1093.04,  -993.44,  138.53, 15.0, 0,0,-1);  // lawa
  CreateDynamicObject( 18752, -1090.00,-990.00,116.00,      0.00,0.00,250.00, 0,0,-1,2100); // wulkan
  CreateDynamicObject( 3673,  -1099.00,-992.00,111.00,      0.00,0.00,0.00, 0,0,-1,300); // dym*/

  // Strzelnica (AmmuNation)
  CreateDynamicObject(7191, 280.4880, -115.7953, 1005.0371, 0.0, 180.0, 240.0);
  CreateDynamicObject(7191, 280.3250, -150.6972, 1005.0294, 0.0, 0.0, 300.0);
  CreateDynamicObject(9339, 299.6453, -131.8039, 1005.8615, 0.0, 0.0, 0.0);

  //ctf z fulldm
  CreateDynamicObject(13120,243.6452000,-3991.4800000,-1.2284000,9.0003000,358.0004000,0.0000000); //
  CreateDynamicObject(13120,274.9820000,-3903.1345000,-1.0129000,358.9973000,354.9981000,52.0008000); //
  CreateDynamicObject(13120,161.5177000,-3861.0862000,-1.7459000,358.9973000,354.9981000,164.9995000); //
  CreateDynamicObject(13120,123.1418000,-3886.6279000,2.9700000,9.0003000,358.0004000,181.0023000); //
  CreateDynamicObject(13120,100.2187000,-4015.8040000,-7.8532000,9.0003000,359.9992000,269.9998000); //
  CreateDynamicObject(9958,66.8275000,-3782.9307000,4.9946000,0.0000000,0.0000000,324.7631000); //
  CreateDynamicObject(5167,404.0061000,-4039.5134000,9.9901000,0.0000000,0.0000000,70.4738000); //
  CreateDynamicObject(5160,443.3620000,-3944.0044000,7.9636000,0.0000000,0.0000000,73.0521000); //
  CreateDynamicObject(5166,443.1785000,-3944.6667000,7.9288000,0.0000000,0.0000000,73.9116000); //
  CreateDynamicObject(5157,450.3926000,-3891.2251000,19.5005000,0.0000000,0.0000000,73.0521000); //
  CreateDynamicObject(5156,420.0397000,-3989.9126000,13.8464000,0.0000000,0.0000000,72.1927000); //
  CreateDynamicObject(5155,429.5918000,-3961.8857000,24.6866000,0.0000000,0.0000000,71.3332000); //
  CreateDynamicObject(5154,390.3324000,-4072.1631000,18.6873000,0.0000000,0.0000000,72.1927000); //
  CreateDynamicObject(1608,381.6452000,-3889.0591000,-1.0057000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(1608,274.9706000,-3790.0349000,-0.8271000,0.0000000,0.0000000,106.5702000); //
  CreateDynamicObject(9237,288.2194000,-3998.2544000,10.3929000,0.0000000,0.0000000,183.0601000); //
  CreateDynamicObject(9245,127.1004000,-3809.8223000,14.0702000,0.0000000,0.0000000,219.1565000); //
  CreateDynamicObject(11426,165.9868000,-3918.9392000,48.0078000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(11426,185.8652000,-3921.7590000,49.3074000,355.7028000,355.7028000,108.2890000); //
  CreateDynamicObject(11440,165.4467000,-3899.7419000,47.8996000,356.5623000,357.4217000,338.5141000); //
  CreateDynamicObject(11442,191.3581000,-3903.3967000,49.4453000,0.0000000,0.0000000,256.0082000); //
  CreateDynamicObject(11445,249.0653000,-3924.6042000,39.8590000,0.0000000,353.1245000,238.0640000); //
  CreateDynamicObject(11446,177.4079000,-3863.6235000,37.6656000,0.0000000,3.4377000,173.6063000); //
  CreateDynamicObject(11443,191.1420000,-3981.7751000,44.5986000,2.5783000,0.0000000,0.0000000); //
  CreateDynamicObject(11442,113.5986000,-3971.4360000,39.5729000,0.0000000,0.0000000,291.2451000); //
  CreateDynamicObject(11444,201.6510000,-3924.8738000,49.2864000,16.3293000,6.0161000,34.3775000); //
  CreateDynamicObject(11428,208.5548000,-3876.0054000,43.9119000,0.0000000,0.0000000,218.1929000); //
  CreateDynamicObject(11427,150.4198000,-3913.9329000,53.1415000,3.4377000,16.3293000,188.1126000); //
  CreateDynamicObject(16137,345.8878000,-3895.5193000,2.1544000,0.0000000,5.1566000,0.0000000); //
  CreateDynamicObject(16359,311.3015000,-3952.8064000,5.8750000,1.7189000,356.5623000,147.8232000); //
  CreateDynamicObject(18234,172.5726000,-3801.6436000,3.0355000,0.0000000,0.0000000,270.6185000); //
  CreateDynamicObject(3363,221.2878000,-3897.4067000,38.1082000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(3364,133.4679000,-3963.4255000,38.9873000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(16317,211.5517000,-3958.3599000,41.7393000,359.1406000,9.4538000,0.0000000); //
  CreateDynamicObject(1407,169.9494000,-3905.9768000,49.8695000,0.0000000,0.0000000,67.8955000); //
  CreateDynamicObject(1413,180.3537000,-3921.0884000,50.7177000,0.0000000,4.2972000,188.2167000); //
  CreateDynamicObject(3275,257.7465000,-3910.6279000,37.2530000,0.0000000,345.3896000,311.0121000); //
  CreateDynamicObject(3279,242.6988000,-3935.2668000,40.2683000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(3279,176.0110000,-3971.5823000,45.7395000,3.4377000,0.8594000,0.0000000); //
  CreateDynamicObject(3787,136.9468000,-3801.6514000,3.2803000,356.5623000,356.5623000,0.0000000); //
  CreateDynamicObject(3788,140.2662000,-3799.4812000,3.2268000,352.2651000,0.0000000,38.6747000); //
  CreateDynamicObject(3786,140.2256000,-3799.4712000,3.4385000,0.0000000,0.0000000,38.6747000); //
  CreateDynamicObject(3795,133.4141000,-3799.8579000,2.6614000,0.0000000,353.1245000,312.7310000); //
  CreateDynamicObject(1369,169.3206000,-3915.9099000,49.4946000,353.9839000,358.2811000,90.2409000); //
  CreateDynamicObject(12957,179.4917000,-3910.2537000,50.6008000,0.0000000,354.8434000,0.0000000); //
  CreateDynamicObject(2780,428.9532000,-3963.5400000,44.5835000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(3267,383.7121000,-4069.4773000,21.4198000,0.0000000,0.0000000,59.3011000); //
  CreateDynamicObject(3267,390.9246000,-4068.5232000,21.4198000,0.0000000,0.0000000,347.1084000); //
  CreateDynamicObject(3267,396.9620000,-4074.1560000,21.4198000,0.0000000,0.0000000,268.8998000); //
  CreateDynamicObject(3267,242.7402000,-3935.4463000,55.9068000,0.0000000,0.0000000,30.0803000); //
  CreateDynamicObject(3461,179.6569000,-3912.4595000,49.3024000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(1654,318.3210000,-3946.9358000,5.6876000,0.0000000,0.0000000,38.6747000); //
  CreateDynamicObject(1654,318.3214000,-3946.6987000,5.6883000,0.0000000,359.1406000,116.0240000); //
  CreateDynamicObject(2064,211.0846000,-3958.8469000,45.5964000,0.0000000,348.8273000,213.8957000); //
  CreateDynamicObject(1368,118.3433000,-3973.8423000,40.3179000,353.1245000,0.0000000,200.2489000); //
  CreateDynamicObject(1487,119.4445000,-3973.5945000,40.3916000,354.8434000,13.7510000,268.0403000); //
  CreateDynamicObject(1487,116.5628000,-3974.8159000,39.6364000,354.8434000,90.2409000,310.1527000); //
  CreateDynamicObject(1487,116.7483000,-3974.7180000,39.6374000,0.0000000,91.9597000,172.6427000); //
  CreateDynamicObject(1487,116.6440000,-3974.4546000,39.6363000,0.0000000,89.3814000,68.7549000); //
  CreateDynamicObject(1329,115.4032000,-3971.4883000,40.4470000,0.0000000,3.4377000,252.5704000); //
  CreateDynamicObject(3512,53.8203000,-3918.2258000,1.4037000,9.4538000,347.1084000,305.8554000); //
  CreateDynamicObject(3512,52.3829000,-3928.9888000,1.2662000,9.4538000,347.1084000,339.3735000); //
  CreateDynamicObject(1598,45.2992000,-3923.2642000,0.2255000,0.0000000,0.0000000,0.0000000); //
  CreateDynamicObject(2406,51.5443000,-3929.5354000,2.1099000,347.9679000,355.7028000,312.7310000); //
  CreateDynamicObject(2226,52.9213000,-3927.2222000,1.5289000,14.6104000,8.5944000,241.3977000); //
  CreateDynamicObject(1646,53.4114000,-3921.3193000,1.7310000,18.9076000,359.1406000,265.4620000); //
  CreateDynamicObject(1646,53.2478000,-3922.8618000,1.7279000,18.9076000,359.1406000,265.4620000); //
  CreateDynamicObject(1646,53.0957000,-3925.7522000,1.7871000,18.9076000,359.1406000,265.4620000); //
  CreateDynamicObject(1815,52.6069000,-3924.8110000,1.3063000,358.2811000,340.2330000,0.0000000); //
  CreateDynamicObject(1670,52.9641000,-3924.2686000,1.9566000,358.2811000,340.2330000,0.0000000); //

  // lodz podwodna
  CreateDynamicObject(3117, 538.02746582031, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 534.50219726563, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 530.94000244141, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 527.388671875, -2431, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(9820, 533.20812988281, -2434.4289550781, 1203.9852294922, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(3117, 538.01861572266, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 534.501953125, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 530.939453125, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 527.388671875, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 523.837890625, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 520.2861328125, -2433.1201171875, 1203.025390625, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 535.32977294922, -2429.962890625, 1207.4162597656, 90, 0, 180,0,1,-1,300);
  CreateDynamicObject(3095, 533.54315185547, -2432.1628417969, 1207.4665527344, 90, 0, 180,0,1,-1,300);
  CreateDynamicObject(3095, 539.5791015625, -2428.357421875, 1205.7175292969, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 540.30078125, -2434.3935546875, 1206.4172363281, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 543.3359375, -2435.3017578125, 1205.6912841797, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 544.634765625, -2432.837890625, 1205.6922607422, 90, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(3095, 531.34338378906, -2434.4631347656, 1206.4172363281, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 544.28686523438, -2435.7065429688, 1203.1677246094, 180, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 539.814453125, -2438.6484375, 1205.7423095703, 90, 0, 270,0,1,-1,300);
  CreateDynamicObject(3095, 544.1376953125, -2438.8251953125, 1204.4417724609, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(2944, 539.60577392578, -2434.2741699219, 1204.5452880859, 0, 0, 261.20385742188,0,1,-1,300);
  CreateDynamicObject(3095, 534.77545166016, -2434.1518554688, 1207.4167480469, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 533.5185546875, -2431.5751953125, 1207.4672851563, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(9819, 542.25457763672, -2437.5654296875, 1203.9002685547, 0, 0, 270.27026367188,0,1,-1,300);
  CreateDynamicObject(1663, 541.26226806641, -2436.9877929688, 1203.6245117188, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 526.58880615234, -2428.4353027344, 1202.9417724609, 90, 0, 270,0,1,-1,300);
  CreateDynamicObject(3095, 522.07385253906, -2432.9611816406, 1202.6665039063, 90, 0, 180.27014160156,0,1,-1,300);
  CreateDynamicObject(3095, 522.36553955078, -2434.474609375, 1206.4172363281, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(16782, 539.89959716797, -2435.6579589844, 1204.6827392578, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 524.52368164063, -2434.1721191406, 1202.7666015625, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(2944, 530.34967041016, -2434.3896484375, 1204.5452880859, 0, 0, 351.41479492188,0,1,-1,300);
  CreateDynamicObject(3095, 530.46459960938, -2438.6704101563, 1203.1427001953, 179.99450683594, 0, 0,0,1,-1,300);
  CreateDynamicObject(10227, 530.46575927734, -2436.7365722656, 1204.9790039063, 0, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 526.03820800781, -2438.9262695313, 1203.2921142578, 90, 0, 270,0,1,-1,300);
  CreateDynamicObject(3095, 534.91082763672, -2438.8146972656, 1207.3671875, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 530.50500488281, -2438.7270507813, 1203.2661132813, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 530.658203125, -2439.1062011719, 1205.8416748047, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 524.54742431641, -2434.7482910156, 1203.3907470703, 90, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(3095, 534.77416992188, -2434.7270507813, 1207.4172363281, 90, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(2690, 529.82861328125, -2438.5744628906, 1203.4986572266, 0, 0, 192.54040527344,0,1,-1,300);
  CreateDynamicObject(918, 533.12744140625, -2433.9267578125, 1203.5219726563, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(918, 532.19744873047, -2433.884765625, 1203.5219726563, 0, 0, 300.18017578125,0,1,-1,300);
  CreateDynamicObject(2312, 530.33752441406, -2432.1123046875, 1205.3646240234, 6, 0, 0,0,1,-1,300);
  CreateDynamicObject(2886, 529.6884765625, -2432.1235351563, 1204.5522460938, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(2886, 532.06359863281, -2432.1220703125, 1204.5522460938, 0, 0, 1.9849853515625,0,1,-1,300);
  CreateDynamicObject(2312, 532.68737792969, -2432.107421875, 1205.3646240234, 5.99853515625, 0, 0,0,1,-1,300);
  CreateDynamicObject(2063, 535.6181640625, -2432.4604492188, 1203.9842529297, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(1242, 534.58074951172, -2432.4663085938, 1204.0692138672, 0, 0, 338.1201171875,0,1,-1,300);
  CreateDynamicObject(1242, 534.97930908203, -2432.4438476563, 1204.0692138672, 0, 0, 23.905090332031,0,1,-1,300);
  CreateDynamicObject(1242, 535.40185546875, -2432.4643554688, 1204.0692138672, 0, 0, 351.96069335938,0,1,-1,300);
  CreateDynamicObject(2043, 534.83953857422, -2432.5034179688, 1203.5206298828, 0, 0, 270.27026367188,0,1,-1,300);
  CreateDynamicObject(2043, 535.92388916016, -2432.52734375, 1203.5206298828, 0, 0, 70.869232177734,0,1,-1,300);
  CreateDynamicObject(2969, 536.19653320313, -2432.4033203125, 1203.9854736328, 0, 0, 357.75,0,1,-1,300);
  CreateDynamicObject(2690, 534.41552734375, -2431.447265625, 1203.4986572266, 0, 0, 190.50842285156,0,1,-1,300);
  CreateDynamicObject(2690, 534.86975097656, -2431.4250488281, 1203.4986572266, 0, 0, 250.32806396484,0,1,-1,300);
  CreateDynamicObject(2359, 535.0068359375, -2432.4235839844, 1204.9534912109, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(2359, 536.29071044922, -2432.3913574219, 1204.9534912109, 0, 0, 356.03002929688,0,1,-1,300);
  CreateDynamicObject(2358, 535.44219970703, -2432.48828125, 1204.4558105469, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(1271, 531.34405517578, -2431.2106933594, 1203.5003662109, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(1271, 532.06781005859, -2431.1884765625, 1203.5003662109, 0, 0, 358.78002929688,0,1,-1,300);
  CreateDynamicObject(1271, 531.79602050781, -2431.1955566406, 1204.1760253906, 0, 0, 359.99841308594,0,1,-1,300);
  CreateDynamicObject(3879, 522.79968261719, -2430.4990234375, 1216.5125732422, 0, 180, 180,0,1,-1,300);
  CreateDynamicObject(3095, 520.33489990234, -2430.3913574219, 1202.5418701172, 90, 0, 270,0,1,-1,300);
  CreateDynamicObject(2634, 521.62847900391, -2433.548828125, 1206.4168701172, 270, 0, 270.02026367188,0,1,-1,300);
  CreateDynamicObject(2690, 543.03540039063, -2433.1096191406, 1203.4986572266, 0, 0, 312.13012695313,0,1,-1,300);
  CreateDynamicObject(3111, 524.69171142578, -2434.1579589844, 1204.6546630859, 270, 90, 270,0,1,-1,300);
  CreateDynamicObject(3386, 528.38543701172, -2434.1892089844, 1203.1496582031, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(12985, 530.96057128906, -2429.4606933594, 1200.5476074219, 0, 0, 270.25,0,1,-1,300);
  CreateDynamicObject(3095, 530.28283691406, -2425.46875, 1207.6424560547, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 530.02172851563, -2428.9338378906, 1202.0416259766, 90, 0, 180,0,1,-1,300);
  CreateDynamicObject(3095, 527.61779785156, -2429.9677734375, 1198.6169433594, 90, 0, 359.75,0,1,-1,300);
  CreateDynamicObject(3095, 529.77532958984, -2425.46875, 1206.4172363281, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 535.2802734375, -2425.4484863281, 1202.9177246094, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 533.5009765625, -2425.4375, 1202.0418701172, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 533.0244140625, -2434.4638671875, 1198.9937744141, 179.99450683594, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 531.04742431641, -2425.9897460938, 1199.6657714844, 48, 180, 90.25,0,1,-1,300);
  CreateDynamicObject(1800, 528.29656982422, -2431.0593261719, 1198.9904785156, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(1800, 528.2958984375, -2431.05859375, 1199.8658447266, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(1800, 528.04467773438, -2434.041015625, 1198.9904785156, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(1800, 528.322265625, -2437.9799804688, 1199.8656005859, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(1800, 528.33276367188, -2437.978515625, 1198.9904785156, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(1800, 531.681640625, -2437.94921875, 1198.9904785156, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(1800, 531.68212890625, -2437.94921875, 1199.8653564453, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(3095, 524.11560058594, -2434.4887695313, 1198.9937744141, 179.99450683594, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 529.396484375, -2440.3095703125, 1198.1677246094, 90, 0, 270,0,1,-1,300);
  CreateDynamicObject(3095, 528.82373046875, -2440.3239746094, 1198.1677246094, 90, 0, 90.290161132813,0,1,-1,300);
  CreateDynamicObject(2514, 520.80499267578, -2433.5725097656, 1198.9903564453, 0, 0, 89.730010986328,0,1,-1,300);
  CreateDynamicObject(2739, 521.42553710938, -2433.4611816406, 1198.9903564453, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(2739, 522.27471923828, -2433.4450683594, 1198.9903564453, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(2944, 526.4873046875, -2432.8095703125, 1200.3696289063, 0, 0, 99.55810546875,0,1,-1,300);
  CreateDynamicObject(3095, 533.1416015625, -2434.5673828125, 1201.7415771484, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 524.17797851563, -2434.5625, 1201.7415771484, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3117, 533.58203125, -2430.0791015625, 1202.0744628906, 89.324340820313, 0, 359.74731445313,0,1,-1,300);
  CreateDynamicObject(3095, 527.64892578125, -2430.5646972656, 1197.2668457031, 90, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(2527, 526.66650390625, -2437.2890625, 1198.9655761719, 0, 0, 180,0,1,-1,300);
  CreateDynamicObject(2527, 528.35589599609, -2437.2658691406, 1198.9655761719, 0, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(11631, 534.91815185547, -2430.9252929688, 1200.2359619141, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 538.060546875, -2430.2431640625, 1197.3377685547, 90, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(1663, 535.23059082031, -2431.5083007813, 1199.4191894531, 0, 0, 160.60052490234,0,1,-1,300);
  CreateDynamicObject(2007, 526.5126953125, -2435.2265625, 1198.9904785156, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(2416, 527.42669677734, -2431.1213378906, 1198.8720703125, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(1800, 528.0439453125, -2434.041015625, 1199.6906738281, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(2690, 532.3095703125, -2438.2602539063, 1199.3232421875, 0, 0, 170.74377441406,0,1,-1,300);
  CreateDynamicObject(2690, 536.43225097656, -2430.5341796875, 1199.3232421875, 0, 0, 335.35363769531,0,1,-1,300);
  CreateDynamicObject(3095, 539.2568359375, -2438.7294921875, 1198.2911376953, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 537.32739257813, -2436.1455078125, 1198.2918701172, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(2944, 537.42340087891, -2431.7294921875, 1200.3696289063, 0, 0, 324.76245117188,0,1,-1,300);
  CreateDynamicObject(1800, 536.50280761719, -2437.6374511719, 1198.9654541016, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(1800, 536.501953125, -2437.63671875, 1199.8663330078, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3386, 537.34405517578, -2438.6044921875, 1198.9655761719, 0, 0, 319.61853027344,0,1,-1,300);
  CreateDynamicObject(2007, 529.81372070313, -2436.6037597656, 1198.9654541016, 0, 0, 89.684936523438,0,1,-1,300);
  CreateDynamicObject(2007, 536.85040283203, -2433.5270996094, 1198.9654541016, 0, 0, 270.22143554688,0,1,-1,300);
  CreateDynamicObject(2007, 525.03350830078, -2433.8515625, 1198.9654541016, 0, 0, 180.48974609375,0,1,-1,300);
  CreateDynamicObject(3095, 537.94476318359, -2434.2509765625, 1197.2440185547, 179.99450683594, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 546.93872070313, -2434.2585449219, 1197.2440185547, 179.99450683594, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 547.01922607422, -2430.2373046875, 1197.3377685547, 90, 0, 179.99450683594,0,1,-1,300);
  CreateDynamicObject(3095, 548.23669433594, -2438.7329101563, 1198.2906494141, 90, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 542.13323974609, -2434.6682128906, 1201.1927490234, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 550.76525878906, -2434.6833496094, 1198.3181152344, 90, 0, 90,0,1,-1,300);
  CreateDynamicObject(3095, 551.06671142578, -2434.6713867188, 1201.1927490234, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3095, 538.14105224609, -2436.1105957031, 1197.1429443359, 90, 0, 270,0,1,-1,300);
  CreateDynamicObject(3787, 541.83905029297, -2432.0568847656, 1197.7816162109, 0, 0, 89.730010986328,0,1,-1,300);
  CreateDynamicObject(8572, 538.388671875, -2433.0817871094, 1197.7409667969, 0, 0, 89.730041503906,0,1,-1,300);
  CreateDynamicObject(964, 539.80517578125, -2434.8369140625, 1197.2407226563, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(964, 540.00012207031, -2433.5756835938, 1197.2407226563, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(964, 539.96453857422, -2434.0815429688, 1198.1666259766, 0, 0, 352.06005859375,0,1,-1,300);
  CreateDynamicObject(964, 541.27093505859, -2434.435546875, 1197.2407226563, 0, 0, 344.1201171875,0,1,-1,300);
  CreateDynamicObject(964, 540.37884521484, -2432.2858886719, 1197.2407226563, 0, 0, 358.01416015625,0,1,-1,300);
  CreateDynamicObject(964, 540.44445800781, -2430.9887695313, 1197.2407226563, 0, 0, 359.01147460938,0,1,-1,300);
  CreateDynamicObject(964, 540.23645019531, -2431.6215820313, 1198.1666259766, 0, 0, 352.3212890625,0,1,-1,300);
  CreateDynamicObject(3117, 537.99005126953, -2431.9519042969, 1197.8988037109, 89.324340820313, 0, 89.977294921875,0,1,-1,300);
  CreateDynamicObject(3788, 539.72692871094, -2438.126953125, 1197.7603759766, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3788, 545.88702392578, -2432.2800292969, 1197.7600097656, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3788, 545.87335205078, -2434.2255859375, 1197.7600097656, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3788, 545.85906982422, -2436.2758789063, 1197.7600097656, 0, 0, 0,0,1,-1,300);
  CreateDynamicObject(3786, 546.21185302734, -2432.2673339844, 1197.9476318359, 0, 0, 180,0,1,-1,300);
  CreateDynamicObject(3786, 546.21862792969, -2434.2109375, 1197.9476318359, 0, 0, 180,0,1,-1,300);
  CreateDynamicObject(3786, 546.20245361328, -2436.2770996094, 1197.9476318359, 0, 0, 180,0,1,-1,300);
  CreateDynamicObject(14450, 549.74066162109, -2434.4230957031, 1200.4025878906, 0, 0, 270,0,1,-1,300);
  CreateDynamicObject(927, 526.06927490234, -2436.9543457031, 1199.9611816406, 0, 0, 89.730010986328,0,1,-1,300);
  CreateDynamicObject(927, 523.02197265625, -2434.1232910156, 1199.9611816406, 0, 0, 179.70520019531,0,1,-1,300);
  CreateDynamicObject(934, 550.01379394531, -2433.0434570313, 1198.5690917969, 0, 0, 0,0,1,-1,300);

  // kosciol
  CreateDynamicObject(9931,1947.54003906,-368.51269531,1108.01086426,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(church_sfe) (1)
  CreateDynamicObject(3976,1970.65722656,-341.23883057,1100.22949219,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(policest02_lan) (1)
  CreateDynamicObject(9931,1980.55761719,-368.51562500,1108.01086426,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(church_sfe) (2)
  CreateDynamicObject(3976,1962.70117188,-400.46679688,1109.52941895,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(policest02_lan) (2)
  CreateDynamicObject(11472,1964.78710938,-372.02050781,1089.22351074,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(des_swtstairs1) (1)
  CreateDynamicObject(11472,1963.28967285,-372.02139282,1089.22351074,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(des_swtstairs1) (2)
  CreateDynamicObject(11472,1964.04479980,-372.98049927,1089.46386719,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(des_swtstairs1) (3)
  CreateDynamicObject(2896,1959.29248047,-370.91723633,1093.34313965,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(casket_law) (1)
  CreateDynamicObject(11472,1962.66918945,-373.82989502,1087.66955566,90.00000000,180.00000000,270.00000000,0,1,-1,250); //object(des_swtstairs1) (4)
  CreateDynamicObject(11472,1965.41259766,-373.81991577,1087.66992188,90.00000000,180.00000000,90.00000000,0,1,-1,250); //object(des_swtstairs1) (5)
  CreateDynamicObject(970,1970.61437988,-368.20404053,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (1)
  CreateDynamicObject(970,1968.51074219,-368.20703125,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (2)
  CreateDynamicObject(970,1957.52185059,-368.25833130,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (3)
  CreateDynamicObject(970,1959.61914062,-368.25585938,1093.28039551,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(6959,1963.97839355,-368.12149048,1072.75439453,90.00000000,0.00000000,0.25000000,0,1,-1,250); //object(vegasnbball1) (2)
  CreateDynamicObject(2960,1964.05883789,-348.98986816,1101.36645508,0.00000000,90.00000000,89.99450684,0,1,-1,250); //object(kmb_beam) (1)
  CreateDynamicObject(2960,1963.98291016,-349.00000000,1101.70666504,90.00000000,0.00000000,179.99450684,0,1,-1,250); //object(kmb_beam) (2)
  CreateDynamicObject(2960,1964.05871582,-348.99044800,1100.00585938,0.00000000,90.00000000,90.00000000,0,1,-1,250); //object(kmb_beam) (3)
  CreateDynamicObject(3872,1964.02685547,-346.40850830,1102.33020020,0.00000000,155.25000000,90.27026367,0,1,-1,250); //object(ws_floodbeams) (1)
  CreateDynamicObject(1667,1962.98828125,-370.41271973,1093.62292480,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(propwineglass1) (1)
  CreateDynamicObject(1664,1962.83520508,-370.42520142,1093.70104980,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(propwinebotl2) (1)
  CreateDynamicObject(2868,1965.95605469,-370.30142212,1093.53430176,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_ornament03) (1)
  CreateDynamicObject(2869,1964.02185059,-373.78527832,1093.66992188,0.00000000,0.00000000,323.75000000,0,1,-1,250); //object(gb_ornament04) (1)
  CreateDynamicObject(2870,1965.26269531,-373.64062500,1093.66992188,0.00000000,0.00000000,350.03002930,0,1,-1,250); //object(gb_ornament05) (1)
  CreateDynamicObject(2270,1965.47998047,-352.47622681,1093.48364258,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(frame_wood_6) (1)
  CreateDynamicObject(2257,1964.05615234,-373.94995117,1095.86132812,0.00000000,0.00000000,180.00000000,0,1,-1,250); //object(frame_clip_4) (1)
  CreateDynamicObject(2271,1962.60437012,-352.58059692,1093.48803711,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(frame_wood_1) (1)
  CreateDynamicObject(2357,1964.05090332,-370.68905640,1093.12402344,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(dunc_dinning) (1)
  CreateDynamicObject(2808,1957.51367188,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (1)
  CreateDynamicObject(2808,1959.68164062,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (2)
  CreateDynamicObject(2808,1970.61328125,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (3)
  CreateDynamicObject(2808,1968.44238281,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (4)
  CreateDynamicObject(2808,1961.85546875,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (5)
  CreateDynamicObject(2808,1966.27246094,-364.26171875,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2868,1962.14697266,-370.30624390,1093.53430176,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_ornament03) (4)
  CreateDynamicObject(948,1966.09277344,-374.17285156,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (1)
  CreateDynamicObject(948,1961.96313477,-374.22888184,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (2)
  CreateDynamicObject(2894,1963.95080566,-370.94277954,1093.53430176,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(kmb_rhymesbook) (1)
  CreateDynamicObject(3462,1959.30407715,-374.06838989,1094.24255371,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(csrangel_lvs) (1)
  CreateDynamicObject(3462,1968.78125000,-374.06835938,1094.24255371,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(csrangel_lvs) (2)
  CreateDynamicObject(949,1956.02258301,-353.46194458,1095.83190918,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (1)
  CreateDynamicObject(2946,1965.59985352,-349.21301270,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(cr_door_03) (2)
  CreateDynamicObject(2946,1962.47033691,-349.19955444,1091.94543457,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(cr_door_03) (3)
  CreateDynamicObject(2808,1970.60681152,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1968.43054199,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1966.25378418,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1957.52233887,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1959.69580078,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1961.86865234,-360.67248535,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1957.51660156,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1959.69250488,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1961.86779785,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1970.61279297,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1968.43701172,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(2808,1966.26782227,-357.18194580,1092.59191895,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(cj_pizza_chair4) (6)
  CreateDynamicObject(14705,1962.68139648,-373.67498779,1093.91894531,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(int2vase) (1)
  CreateDynamicObject(14410,1968.61401367,-349.16000366,1092.01757812,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(carter-stairs03) (1)
  CreateDynamicObject(11472,1971.98046875,-351.97091675,1092.19567871,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(des_swtstairs1) (6)
  CreateDynamicObject(11472,1971.98046875,-353.19616699,1092.19567871,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(des_swtstairs1) (7)
  CreateDynamicObject(14410,1959.47412109,-349.41287231,1092.01660156,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(carter-stairs03) (2)
  CreateDynamicObject(11472,1956.10046387,-351.96487427,1092.19567871,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(des_swtstairs1) (8)
  CreateDynamicObject(11472,1956.10046387,-353.19616699,1092.19567871,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(des_swtstairs1) (9)
  CreateDynamicObject(11472,1965.10974121,-347.94531250,1095.44091797,0.00000000,90.00000000,180.00000000,0,1,-1,250); //object(des_swtstairs1) (10)
  CreateDynamicObject(11472,1962.98315430,-347.94104004,1095.44018555,0.00000000,270.00000000,179.99450684,0,1,-1,250); //object(des_swtstairs1) (11)
  CreateDynamicObject(1698,1962.46606445,-352.53399658,1095.31420898,0.00000000,0.00000000,270.27026367,0,1,-1,250); //object(esc_step8) (1)
  CreateDynamicObject(1698,1962.87316895,-352.53399658,1095.53918457,0.00000000,0.00000000,270.26916504,0,1,-1,250); //object(esc_step8) (2)
  CreateDynamicObject(1698,1963.24682617,-352.53399658,1095.76416016,0.00000000,0.00000000,270.26916504,0,1,-1,250); //object(esc_step8) (3)
  CreateDynamicObject(1698,1965.60021973,-352.53399658,1095.31420898,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(esc_step8) (4)
  CreateDynamicObject(1698,1965.20434570,-352.53399658,1095.53918457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(esc_step8) (5)
  CreateDynamicObject(1698,1964.72375488,-352.53399658,1095.76416016,0.00000000,0.00000000,270.26916504,0,1,-1,250); //object(esc_step8) (6)
  CreateDynamicObject(970,1964.05688477,-353.88497925,1096.74243164,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1959.94470215,-353.88497925,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1955.77246094,-353.88589478,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1968.17297363,-353.88497925,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1972.25000000,-353.88589478,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1959.97607422,-351.29064941,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1968.10302734,-351.29064941,1095.74719238,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1965.96203613,-349.21618652,1096.74169922,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(970,1962.13757324,-349.21618652,1096.74243164,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(fencesmallb) (4)
  CreateDynamicObject(2887,1964.04821777,-349.34906006,1094.78955078,90.00000000,0.00000000,0.00000000,0,1,-1,250); //object(a51_spotbulb) (1)
  CreateDynamicObject(949,1971.95068359,-353.46038818,1095.83190918,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (2)
  CreateDynamicObject(948,1965.68554688,-352.45767212,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (1)
  CreateDynamicObject(948,1962.38427734,-352.45767212,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_10) (1)
  CreateDynamicObject(1742,1955.84399414,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (1)
  CreateDynamicObject(1742,1957.28173828,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (2)
  CreateDynamicObject(1742,1958.71166992,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (3)
  CreateDynamicObject(1742,1960.15112305,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (4)
  CreateDynamicObject(1742,1971.77673340,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (5)
  CreateDynamicObject(1742,1970.34277344,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (6)
  CreateDynamicObject(1742,1968.90930176,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (7)
  CreateDynamicObject(1742,1967.47534180,-353.83831787,1091.94543457,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(med_bookshelf) (8)
  CreateDynamicObject(949,1961.60729980,-354.34103394,1092.58166504,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (3)
  CreateDynamicObject(949,1966.46325684,-354.34805298,1092.58166504,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(plant_pot_4) (4)
  CreateDynamicObject(1720,1970.09338379,-373.59497070,1092.72888184,0.00000000,0.00000000,180.00000000,0,1,-1,250); //object(rest_chair) (1)
  CreateDynamicObject(1720,1970.91198730,-373.58316040,1092.72888184,0.00000000,0.00000000,179.99450684,0,1,-1,250); //object(rest_chair) (2)
  CreateDynamicObject(741,1969.29675293,-369.22723389,1093.71252441,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(pot_01) (1)
  CreateDynamicObject(3440,1969.29125977,-370.86264038,1091.08007812,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(arptpillar01_lvs) (1)
  CreateDynamicObject(14455,1955.32812500,-348.54144287,1096.87585449,0.00000000,0.00000000,270.00000000,0,1,-1,250); //object(gs_bookcase) (1)
  CreateDynamicObject(14455,1972.78356934,-352.90487671,1096.86743164,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gs_bookcase) (2)
  CreateDynamicObject(2842,1964.51916504,-365.60891724,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (1)
  CreateDynamicObject(2842,1964.51843262,-363.77880859,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (2)
  CreateDynamicObject(2842,1964.51867676,-361.94879150,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (3)
  CreateDynamicObject(2842,1964.52172852,-360.12713623,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (4)
  CreateDynamicObject(2842,1964.52636719,-358.30560303,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (5)
  CreateDynamicObject(2842,1964.52600098,-356.47940063,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (6)
  CreateDynamicObject(2842,1964.52343750,-354.64611816,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (7)
  CreateDynamicObject(2842,1964.52136230,-352.82815552,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (8)
  CreateDynamicObject(2842,1964.51867676,-350.99310303,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (9)
  CreateDynamicObject(2842,1964.52026367,-349.16046143,1091.94543457,0.00000000,0.00000000,90.00000000,0,1,-1,250); //object(gb_bedrug04) (10)
  CreateDynamicObject(2833,1963.55749512,-369.65057373,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_livingrug02) (1)
  CreateDynamicObject(2834,1968.81579590,-372.52481079,1092.72888184,0.00000000,0.00000000,0.00000000,0,1,-1,250); //object(gb_livingrug03) (1)
  CreateDynamicObject(6959,1951.26586914,-360.22650146,1095.56030273,310.00000000,180.00000000,90.00000000,0,1,-1,250); //object(vegasnbball1) (1)
  CreateDynamicObject(6959,1976.87695312,-356.75421143,1095.51965332,310.00000000,180.00000000,270.00000000,0,1,-1,250); //object(vegasnbball1) (3)
  CreateDynamicObject(6959,1958.95715332,-378.23306274,1096.33813477,309.99572754,179.99450684,180.00000000,0,1,-1,250); //object(vegasnbball1) (4)

  // pomniki
  CreateDynamicMapIcon(-2359,-1637,482.70,33,1, 0,0,-1,1200);     // chiliad
  CreateDynamicObject(11489, -2359,-1637,482.70,  0,0,90, 0,0,-1,300);
  TekstyPomniki[ea_czasgry][0]=CreateDynamic3DTextLabel(__("Najdluzej grajace osoby"),0xFF5219FF,-2359,-1637,482.7+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);
  TekstyPomniki[ea_czasgry][1]=CreateDynamic3DTextLabel("_",0xffffffff,-2359,-1637,482.7+3, 28, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);
  TekstyPomniki[ea_czasgry][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-2359+2, -1637+5 , 482.7+2, 28, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);
  TekstyPomniki[ea_czasgry][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-2359+2, -1637-5 , 482.7+2, 28, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0,0,0);

  CreateDynamicMapIcon(2191,1004,9.80,33,1, 0,0,-1,1200);     // parking lv
  CreateDynamicObject(11489, 2191,1004,9.80,  0,0,270,  0,0,-1,300);
  TekstyPomniki[ea_skill][0]=CreateDynamic3DTextLabel(__("Urodzeni zabojcy\ngracze z najwiekszym skillem"),0xFF5219FF, 2191, 1004,9.8+1,  100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_skill][1]=CreateDynamic3DTextLabel("_",0xffffffff, 2191, 1004,9.8+3, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_skill][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0, 2191-2,  1004+5 ,   9.8+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_skill][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0, 2191-2,  1004-5 ,   9.8+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  CreateDynamicMapIcon(1083,-1813,12.50,33,1, 0,0,-1,1200);     // parking lv
  CreateDynamicObject(11489, 1083,-1813,12.50,  0,0,270,  0,0,-1,300);
  TekstyPomniki[ea_walizki][0]=CreateDynamic3DTextLabel(__("Poszukiwacze bonusow"),0xFF5219FF, 1083,-1813,12.50+1,  100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_walizki][1]=CreateDynamic3DTextLabel("_",0xffffffff, 1083,-1813,12.5+3,  30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_walizki][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0, 1083-2, -1813+5 ,  12.5+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_walizki][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0, 1083-2, -1813-5 ,  12.5+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  CreateDynamicMapIcon(-2225,1804,-62.50,33,1,  0,0,-1,1200);     // woda kolo /sf
  CreateDynamicObject(11489,-2225,1804,-62.50,  0,0,0,  0,0,-1,300);
  TekstyPomniki[ea_lowskill][0]=CreateDynamic3DTextLabel(__("Gracze z najnizszym skillem"),0xFF5219FF,-2225, 1804,-62.50+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_lowskill][1]=CreateDynamic3DTextLabel("_",0xffffffff,-2225, 1804,-62.50+3, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_lowskill][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-2225+5,  1804-2, -62.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_lowskill][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-2225-5,  1804-2 ,-62.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  CreateDynamicMapIcon(-1739,56,2.50,33,1,  0,0,-1,1200);     // sf suchy dok
  CreateDynamicObject(11489,-1739,56,2.5, 0,0,90, 0,0,-1,300);
  TekstyPomniki[ea_derby][0]=CreateDynamic3DTextLabel(__("Mistrzowie /derby"),0xFF5219FF,-1739, 56,2.50+1,  100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_derby][1]=CreateDynamic3DTextLabel("_",0xffffffff,-1739, 56,2.50+3,  30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_derby][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-1739+2,  56+5,2.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_derby][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-1739+2,  56-5,2.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  CreateDynamicMapIcon(-2960,-2927,-58.29,33,1, 0,0,-1,1200);     // gdzies w rogu mapy w wodzie
  CreateDynamicObject(11489,-2960,-2927,-58.29, 0,0,90, 0,0,-1,300);
  TekstyPomniki[ea_kickban][0]=CreateDynamic3DTextLabel(__("Najczesciej wykopywani"),0xFF5219FF,-2960,-2927,-58.29+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_kickban][1]=CreateDynamic3DTextLabel("_",0xffffffff,-2960,-2927,-58.29+3,  30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_kickban][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-2960+2,-2927+5,-58.29+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_kickban][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-2960+2,-2927-5,-58.29+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  // lvlot racedist
  CreateDynamicMapIcon(1632,1678,9.80,33,1,0,0,-1,1200);
  CreateDynamicObject(11489,1632,1678,9.80, 0,0,90, 0,0,-1,300);
  TekstyPomniki[ea_racedist][0]=CreateDynamic3DTextLabel(__("Mistrzowie /race\nNajwiecej przejechanych km."),0xFF5219FF, 1632,1678,  9.80+1,  100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_racedist][1]=CreateDynamic3DTextLabel("_",0xffffffff, 1632, 1678,  9.80+3, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_racedist][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0, 1632+2, 1678+5,  9.80+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_racedist][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0, 1632+2, 1678-5,  9.80+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  // arenasolo ilosc wygranych
  CreateDynamicMapIcon(-1154.50,-1025,128.2,33,1,VW_UNIQUE_ARENASOLO,9,-1,1200);
  CreateDynamicObject(11489,-1154.50,-1025,128.2, 0,0,90, VW_UNIQUE_ARENASOLO, 9, -1, 300);
  TekstyPomniki[ea_soloklrs][0]=CreateDynamic3DTextLabel(__("Mistrzowie /solo\nNajwiecej wygranych solowek."),0xFF5219FF,-1154.50,-1025,128.20+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);
  TekstyPomniki[ea_soloklrs][1]=CreateDynamic3DTextLabel("_",0xffffffff,-1154.50,-1025,128.20+3,  30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);
  TekstyPomniki[ea_soloklrs][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,-1154.50+2,-1025+5,128.20+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);
  TekstyPomniki[ea_soloklrs][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,-1154.50+2,-1025-5,128.20+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,VW_UNIQUE_ARENASOLO,9);

  // kwota mandatow
  CreateDynamicMapIcon(625.0,-590.0,15.5,33,1,0,0,-1,1200);
  CreateDynamicObject(11489,625,-590,15.5,  0,0,90, 0,0, -1, 300);
  TekstyPomniki[ea_fotocash][0]=CreateDynamic3DTextLabel(__("Najwieksi piraci drogowi\nNajwieksze mandaty za predkosc"),0xFF5219FF,625.00,-590,15.50+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_fotocash][1]=CreateDynamic3DTextLabel("_",0xffffffff,625.00,-590,15.50+3,  30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_fotocash][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,625.00+2,-590+5,15.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_fotocash][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,625.0+2,-590-5,15.50+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  // kasa w banku
  CreateDynamicMapIcon(2301.64, 6.71, 26.48,33,1,0,0,-1,1200);
  CreateDynamicObject(11489,2286.16, 5.42, 25.48, 0,0,90, 0,0, -1, 300);
  TekstyPomniki[ea_cash][0]=CreateDynamic3DTextLabel(__("Gracze z najwieksza iloscia pieniedzy w banku"),0xFF5219FF,2286.16, 5.42, 25.48+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_cash][1]=CreateDynamic3DTextLabel("_",0xffffffff,2286.16, 5.42, 25.48+3, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_cash][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,2286.16+2, 5.42+5, 25.48+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_cash][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,2286.16+2, 5.42-5, 25.48+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  // najlepsi listonosze
  CreateDynamicMapIcon(2301.64, 6.71, 26.48,33,1,0,0,-1,1200);
  CreateDynamicObject(11489,2301.64, 6.71, 25.48, 0,0,270,  0,0, -1, 300);
  TekstyPomniki[ea_listy][0]=CreateDynamic3DTextLabel(__("Najlpesi listonosze\nGracze dostarczajacy najwiecej listow"),0xFF5219FF,2301.64, 6.71, 25.48+1, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_listy][1]=CreateDynamic3DTextLabel("_",0xffffffff,2301.64, 6.71, 25.48+3,  30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_listy][2]=CreateDynamic3DTextLabel("_",0xe0e0e0e0,2301.64-2, 6.71+5, 25.48+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);
  TekstyPomniki[ea_listy][3]=CreateDynamic3DTextLabel("_",0xd0d0d0d0,2301.64-2, 6.71-5, 25.48+2, 30, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1,0,0);

  obiekty_odswiezPomniki(true);
  SetTimer("Pobiekty_odswiezPomniki",1000*60*1,true);
}

obiekty_OPPickUpDynamicPickup(playerid,pickupid){
  new bool:pickupDelayActive=false;
  if (GetTickCount()-pTemp[playerid][pickupDelay] < MIN_PICKUP_DELAY)
    pickupDelayActive=true;

  // rozdzielic na dwie tablice dla roznych interiorow TODO
  if (!pickupDelayActive) {
    for(new i=0;i<TelePickupsCount;i++)
      if(pickupid==TelePickups[i][etp_pickupid]) {
        if (pTemp[playerid][cheater]){
          Msg(playerid,COLOR_INFO2,__("Zamkniete."));
          return 1;
        }
        if (TelePickups[i][etp_restrict_gang]>0 &&
        (TelePickups[i][etp_restrict_gang]!=pData[playerid][gang]) && pData[playerid][adminLevel]<4) {
          new buf[128];
          format(buf, sizeof buf, __("Tylko dla czlonkow klanu {b}%s{/b}!"), gData[TelePickups[i][etp_restrict_gang]][name]);
          Msg(playerid,COLOR_ERROR,buf);
          return 1;
        }
        if (TelePickups[i][etp_restrict_level]>0 && pData[playerid][adminLevel]<TelePickups[i][etp_restrict_level] && pData[playerid][adminLevel]!=LEVEL_ADMINHIDDEN) {
          // telepickup dla wyzszego poziomu
          switch(TelePickups[i][etp_restrict_level]) {
            case 1:
              Msg(playerid,COLOR_ERROR,__("Wejscie tylko dla {b}moderatorow{/b}!"));
            case 2,3:
              Msg(playerid,COLOR_ERROR,__("Wejscie tylko dla {b}administratorow{/b}!"));
            case 4:
              Msg(playerid,COLOR_ERROR,__("Wejscie tylko dla {b}RCONow{/b}!"));
          }
          return 1;
        }
        if (TelePickups[i][etp_tPX]==0 && TelePickups[i][etp_tPY]==0)
          return Msg(playerid, COLOR_INFO2, __("Przejscie jest zamkniete."));
        SaveReturnPath(playerid,TelePickups[i][etp_tI]);
        Teleport(T_PLAYER,playerid,TelePickups[i][etp_tPX],TelePickups[i][etp_tPY],TelePickups[i][etp_tPZ],TelePickups[i][etp_tPA],TelePickups[i][etp_tI],TelePickups[i][etp_tVW],true);
        return 1;
      }
    }

    if (GetPlayerInterior(playerid)==0) {
      if (pickupid==PICKUPs[eP_F_wojsko1] || pickupid==PICKUPs[eP_F_wojsko2]) {
        factions_PlayerJoin(playerid,FACTION_ARMY);
        return 1;
      } else if (pickupid==PICKUPs[eP_F_dilerzy]) {
        factions_PlayerJoin(playerid,FACTION_DILERZY);
        return 1;
      } else if (pickupid==PICKUPs[eP_F_mechanicy]) {
        factions_PlayerJoin(playerid,FACTION_MECHANICY);
        return 1;
      } else if (pickupid==PICKUPs[eP_F_Piloci_LV] || pickupid==PICKUPs[eP_F_Piloci_LS] || pickupid==PICKUPs[eP_F_Piloci_SF]) {
        factions_PlayerJoin(playerid,FACTION_PILOCI);
        return 1;
      } else if (pickupid==PICKUPs[eP_F_Maszynisci_1] || pickupid==PICKUPs[eP_F_Maszynisci_2] || pickupid==PICKUPs[eP_F_Maszynisci_3] || pickupid==PICKUPs[eP_F_Maszynisci_4]) {
        factions_PlayerJoin(playerid,FACTION_MASZYNISCI);
        return 1;
      } else if (pickupid==PICKUPs[eP_F_kierowca_1] || pickupid==PICKUPs[eP_F_kierowca_2] || pickupid==PICKUPs[eP_F_kierowca_3] || pickupid==PICKUPs[eP_F_kierowca_4] || pickupid==PICKUPs[eP_F_kierowca_5]) {
        factions_PlayerJoin(playerid,FACTION_KIEROWCY);
        return 1;
      }
    } else { // interior inny niz 0
    if (pickupid==PICKUPs[eP_F_policja_LS] || pickupid==PICKUPs[eP_F_policja_LV] || pickupid==PICKUPs[eP_F_policja_SF]) {
      factions_PlayerJoin(playerid,FACTION_POLICE);
      return 1;
    } else {
      if (!pickupDelayActive)
      for(new i=0;i<ReturnPickupsCount;i++){
        if(pickupid==ReturnPickups[i]) {
          UseReturnPath(playerid);
          return 1;
        }
      }
    }
  }
  return 0;
}

obiekty_OPLeaveDynamicCP(playerid,checkpointid){
#pragma unused checkpointid
  if (GetPlayerVirtualWorld(playerid)==0) {
    return 0; // WTF?
  }
  return 0;
}

obiekty_OPEnterDynamicCP(playerid,checkpointid){
  if (GetPlayerVirtualWorld(playerid)==0) {

  if (checkpointid==Checkpoints[ec_warsztat_tablice]) {
    warsztat_zmianaTablicy(playerid);
    return 1;
  } else if (checkpointid==Checkpoints[ec_warsztatlv_skup])
    return warsztat_CPskup(playerid);
  else if (checkpointid==Checkpoints[ec_warsztatlv_myjnia])
    return warsztat_CPmyjnia(playerid);
  else if (checkpointid==Checkpoints[ec_warsztatlv_podnosnik])
    return warsztat_CPpodnosnik(playerid);
  else if (checkpointid==Checkpoints[ec_sfrakieta])
    return rakieta_confirmliftoff(playerid);
  }
  if (GetTickCount()-pTemp[playerid][teleCPDelay] > MIN_TELECP_DELAY)
    for(new i=0; i < TeleCPCount; i++)
      if(TeleCheckpoints[i][etcp_cp][0]==checkpointid || TeleCheckpoints[i][etcp_cp][1]==checkpointid)
        return obiekty_OPETeleCheckpoint(playerid,checkpointid,i);

  if (!IsPlayerInAnyVehicle(playerid)){
    for(new i = 0; i < sizeof ATMS; i++)
      if(ATMS[i][eatm_cp]==checkpointid) {
        if(!pData[playerid][loggedIn]) return Msg(playerid, COLOR_ERROR, TXT(playerid, 110)); // Komenda dostêpna tylko dla zarejestrowanych u¿ytkowników.
        ShowPlayerDialog(playerid, DIALOG_BANK_MAIN, DIALOG_STYLE_LIST, __("Bankomat"), GetDialogList(playerid, DIALOG_BANK_MAIN), __("Wybierz"), __("Wyjdz"));
        return 1;
      }
    }
  return 0;
}

CreateATMs()
{
  for(new i = 0; i < sizeof DATA_ATM; i++)
  {
    CreateDynamicObject(2618, DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z], DATA_ATM[i][rX], DATA_ATM[i][rY], DATA_ATM[i][rZ]);
    ATMS[i][eatm_mapicon]=CreateDynamicMapIcon(DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z], 52, 6, 0, DATA_ATM[i][atmInterior], -1, 300.0);
    Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, ATMS[i][eatm_mapicon], E_STREAMER_STYLE, 2);
    ATMS[i][eatm_cp]=CreateDynamicCP(DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z], 1.3, 0,DATA_ATM[i][atmInterior],-1,15.0);
  }
}

CreatePickups()
{
// WTF?
}

obiekty_odswiezTelePickups(bool:tdebug=false){
  mysql_query("SELECT id,pX,pY,pZ,pi,pvw,pickupid,tX,tY,tZ,ti,tvw,tA,pokazopis,IFNULL(opis,'-'),mapicon,mapicon_type,opis_color,restrict_gang,restrict_level FROM fs_telepickups");
  mysql_store_result();
  new Float:pP[3], pI, pVW, pickupid, buffer[255], mapicon, id, mapicon_type, opis_color;
  new i=0;
  while(mysql_fetch_row_format(buffer,"|")){
    if (IsValidDynamicPickup(TelePickups[i][etp_pickupid])) {
      DestroyDynamicPickup(TelePickups[i][etp_pickupid]);
      TelePickups[i][etp_pickupid]=-1;
    }
    if (IsValidDynamic3DTextLabel(TelePickups[i][etp_t3d])) {
      DestroyDynamic3DTextLabel(TelePickups[i][etp_t3d]);
      TelePickups[i][etp_t3d]=Text3D:-1;
    }
    if (IsValidDynamicMapIcon(TelePickups[i][etp_mapicon])) {
      DestroyDynamicMapIcon(TelePickups[i][etp_mapicon]);
      TelePickups[i][etp_mapicon]=-1;
    }

    if (sscanf(buffer,"p<|>dfffdddfffddfds[64]ddddd", id,
        pP[0], pP[1], pP[2], pI, pVW, pickupid,
        TelePickups[i][etp_tPX],
        TelePickups[i][etp_tPY],TelePickups[i][etp_tPZ], TelePickups[i][etp_tI],TelePickups[i][etp_tVW], TelePickups[i][etp_tPA],
        TelePickups[i][etp_pokazopis], TelePickups[i][etp_opis], mapicon, mapicon_type, opis_color, TelePickups[i][etp_restrict_gang], TelePickups[i][etp_restrict_level])) continue;

    TelePickups[i][etp_pickupid]=CreateDynamicPickup(pickupid, 1, pP[0], pP[1], pP[2]+0.25, pVW, pI, -1);

    if (TelePickups[i][etp_pokazopis]) {
      if (tdebug) format(TelePickups[i][etp_opis], 64, "%s[%d]", TelePickups[i][etp_opis], id);
      TelePickups[i][etp_t3d]=CreateDynamic3DTextLabel(TelePickups[i][etp_opis],opis_color*256+0xff, pP[0], pP[1], pP[2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, pVW, pI,-1,100.0);
    } else if (tdebug) {
      format(TelePickups[i][etp_opis], 64, "[%d]", id);
      TelePickups[i][etp_t3d]=CreateDynamic3DTextLabel(TelePickups[i][etp_opis],opis_color*256+0xff, pP[0], pP[1], pP[2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, pVW, pI,-1,100.0);
    }
    if (mapicon>0) {
      TelePickups[i][etp_mapicon]=CreateDynamicMapIcon(pP[0], pP[1], pP[2], mapicon, 1, pVW, pI, -1, 1200.0);
      Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, TelePickups[i][etp_mapicon], E_STREAMER_STYLE, mapicon_type);
    }
    i++;
  }
  new oldTelePickupsCount=TelePickupsCount;
  TelePickupsCount=i;
  for (;i<oldTelePickupsCount;i++) {
    if (IsValidDynamicPickup(TelePickups[i][etp_pickupid])) {
      DestroyDynamicPickup(TelePickups[i][etp_pickupid]);
      TelePickups[i][etp_pickupid]=-1;
    }
    if (IsValidDynamic3DTextLabel(TelePickups[i][etp_t3d])) {
      DestroyDynamic3DTextLabel(TelePickups[i][etp_t3d]);
      TelePickups[i][etp_t3d]=Text3D:-1;
    }
    if (IsValidDynamicMapIcon(TelePickups[i][etp_mapicon])) {
      DestroyDynamicMapIcon(TelePickups[i][etp_mapicon]);
      TelePickups[i][etp_mapicon]=-1;
    }

  }
  mysql_free_result();
  return TelePickupsCount;
}

obiekty_odswiezReturnPickups(){
  mysql_query("SELECT pX,pY,pZ,pi,pvw,pickupid FROM fs_returnpickups");
  mysql_store_result();
  new Float:pP[3], pI, pVW, pickupid, buffer[255];
  new i=0;
  while(mysql_fetch_row_format(buffer,"|")){
    if (IsValidDynamicPickup(ReturnPickups[i])) {
      DestroyDynamicPickup(ReturnPickups[i]);
      ReturnPickups[i]=-1;
    }

    if (sscanf(buffer,"p<|>fffddd",
        pP[0], pP[1], pP[2], pI, pVW, pickupid)) continue;

    ReturnPickups[i]=CreateDynamicPickup(pickupid, 1, pP[0], pP[1], pP[2], pVW, pI, -1);

    i++;
  }
  mysql_free_result();
  ReturnPickupsCount=i;
  for (;i<MAX_RETURNPICKUPS;i++)
    if (IsValidDynamicPickup(ReturnPickups[i])) {
      DestroyDynamicPickup(ReturnPickups[i]);
      ReturnPickups[i]=-1;
    }
  return ReturnPickupsCount;
}

obiekty_odswiezMapIcons(){
  mysql_query("SELECT pX,pY,pZ,pi,pvw,mapicon,type,loadingdistance,opis FROM fs_mapicons");
  mysql_store_result();
  new Float:pP[3], pI, pVW, mapicon, type, Float:dist, opis[65], buffer[255];
  new i=0;
  while(mysql_fetch_row_format(buffer,"|")){

    if (IsValidDynamicMapIcon(MapIcons[i])) {
      DestroyDynamicMapIcon(MapIcons[i]);
      MapIcons[i]=-1;
    }

    if (sscanf(buffer,"p<|>fffddddfs[64]",
        pP[0], pP[1], pP[2], pI, pVW, mapicon, type, dist, opis)) continue;

    if (dist<=0) dist=1200;

    if (pI<=0)  {   // mapikonki nie dzialaja w interiorach
      MapIcons[i]=CreateDynamicMapIcon(pP[0], pP[1], pP[2], mapicon, 1, pVW, pI, -1, dist);
      Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, MapIcons[i], E_STREAMER_STYLE, type);
    }
    i++;
  }
  mysql_free_result();
  MapIconsCount=i;
  for (;i<MAX_MAPICONS;i++)
    if (IsValidDynamicMapIcon(MapIcons[i])) {
      DestroyDynamicMapIcon(MapIcons[i]);
      MapIcons[i]=-1;
    }

  return MapIconsCount;
}

obiekty_odswiezTeleCheckpoints(){
  mysql_query("SELECT fPOS,IFNULL(fOPIS,'-'),dPOS,IFNULL(dOPIS,'-'),rozmiar FROM fs_telecheckpoints WHERE aktywny=1");
  mysql_store_result();
  new buffer[255];
  new i=0;
  while(mysql_fetch_row_format(buffer,"|")){
    new fPOS[64], dPOS[64], cpSize;
    if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][0])) {
      DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][0]);
      TeleCheckpoints[i][etcp_cp][0]=-1;
    }
    if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][1])) {
      DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][1]);
      TeleCheckpoints[i][etcp_cp][1]=-1;
    }

    if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0])) {
      DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0]);
      TeleCheckpoints[i][etcp_t3d][0]=Text3D:-1;
    }

    if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1])) {
      DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1]);
      TeleCheckpoints[i][etcp_t3d][1]=Text3D:-1;
    }

    if (sscanf(buffer,"p<|>s[64]s[64]s[64]s[64]d",
        fPOS, TeleCheckpoints[i][etcp_opis1], dPOS, TeleCheckpoints[i][etcp_opis2], cpSize)) continue;
    if (sscanf(fPOS, "p<,>ffffd",
      TeleCheckpoints[i][etcp_P1][0], TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2], TeleCheckpoints[i][etcp_P1][3], TeleCheckpoints[i][etcp_I][0]))
        continue;
    if (sscanf(dPOS, "p<,>ffffd",
      TeleCheckpoints[i][etcp_P2][0], TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2], TeleCheckpoints[i][etcp_P2][3], TeleCheckpoints[i][etcp_I][1]))
        continue;

    TeleCheckpoints[i][etcp_cp][0]=CreateDynamicCP(TeleCheckpoints[i][etcp_P1][0], TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2], cpSize, 0, TeleCheckpoints[i][etcp_I][0], -1);
    TeleCheckpoints[i][etcp_cp][1]=CreateDynamicCP(TeleCheckpoints[i][etcp_P2][0], TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2], cpSize, 0, TeleCheckpoints[i][etcp_I][1], -1);

    if (strlen(TeleCheckpoints[i][etcp_opis1])>2)
      TeleCheckpoints[i][etcp_t3d][0]=CreateDynamic3DTextLabel(TeleCheckpoints[i][etcp_opis1],0xFFFFFFAA, TeleCheckpoints[i][etcp_P1][0], TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, TeleCheckpoints[i][etcp_I][0]);
    if (strlen(TeleCheckpoints[i][etcp_opis2])>2)
      TeleCheckpoints[i][etcp_t3d][1]=CreateDynamic3DTextLabel(TeleCheckpoints[i][etcp_opis2],0x0E073CFF, TeleCheckpoints[i][etcp_P2][0], TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2]+1, 100.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, TeleCheckpoints[i][etcp_I][1]);

    i++;
  }
  mysql_free_result();
  TeleCPCount=i;
  for (;i<MAX_TELECHECKPOINTS;i++) {
    if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][0])) {
      DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][0]);
      TeleCheckpoints[i][etcp_cp][0]=-1;
    }
    if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0])) {
      DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][0]);
      TeleCheckpoints[i][etcp_t3d][0]=Text3D:-1;
    }

    if (IsValidDynamicCP(TeleCheckpoints[i][etcp_cp][1])) {
      DestroyDynamicCP(TeleCheckpoints[i][etcp_cp][1]);
      TeleCheckpoints[i][etcp_cp][1]=-1;
    }

    if (IsValidDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1])) {
      DestroyDynamic3DTextLabel(TeleCheckpoints[i][etcp_t3d][1]);
      TeleCheckpoints[i][etcp_t3d][1]=Text3D:-1;
    }


  }
  return TeleCPCount;
}

obiekty_clearAllData()
{
  Os[eO_przenosna_strefaDM] = INVALID_OBJECT_ID;
  Os[eO_przenosna_strefa] = INVALID_OBJECT_ID;
}

obiekty_odswiezMiscPickups(){
  mysql_query("SELECT pX,pY,pZ,interior,vw,pickupid FROM fs_miscpickups");
  mysql_store_result();
  new Float:pP[3], pI, pVW, pickupid, buffer[512];
  new i=0;
  while(mysql_fetch_row_format(buffer,"|")){
    if (IsValidDynamicPickup(MiscPickups[i])) {
      DestroyDynamicPickup(MiscPickups[i]);
      MiscPickups[i]=-1;
    }

    if (sscanf(buffer,"p<|>fffddd",
        pP[0], pP[1], pP[2], pI, pVW, pickupid)) continue;

    MiscPickups[i]=CreateDynamicPickup(pickupid, 2, pP[0], pP[1], pP[2], pVW, pI, -1);

    i++;
  }
  mysql_free_result();
  MiscPickupsCount=i;
  for (;i<MAX_MISCPICKUPS;i++)
    if (IsValidDynamicPickup(MiscPickups[i])) {
      DestroyDynamicPickup(MiscPickups[i]);
      MiscPickups[i]=-1;
    }
  return MiscPickupsCount;
}

obiekty_OPETeleCheckpoint(playerid,checkpointid,i){
  if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, __("Ta komenda jest teraz niedostepna."));

  pTemp[playerid][teleCPDelay]=GetTickCount();

  if (TeleCheckpoints[i][etcp_cp][0]==checkpointid) {
    Teleport(T_BOTH, playerid, TeleCheckpoints[i][etcp_P2][0] , TeleCheckpoints[i][etcp_P2][1], TeleCheckpoints[i][etcp_P2][2], TeleCheckpoints[i][etcp_P2][3]+random(10)-5, TeleCheckpoints[i][etcp_I][1], VW_DEFAULT);
    if (strlen(TeleCheckpoints[i][etcp_opis2])>2)
      GameTextForPlayer(playerid, TeleCheckpoints[i][etcp_opis2], 1000, 1);

  }
  else if (TeleCheckpoints[i][etcp_cp][1]==checkpointid) {
    Teleport(T_BOTH, playerid, TeleCheckpoints[i][etcp_P1][0] , TeleCheckpoints[i][etcp_P1][1], TeleCheckpoints[i][etcp_P1][2], TeleCheckpoints[i][etcp_P1][3]+random(10)-5, TeleCheckpoints[i][etcp_I][0], VW_DEFAULT);
    if (strlen(TeleCheckpoints[i][etcp_opis1])>2)
      GameTextForPlayer(playerid, TeleCheckpoints[i][etcp_opis1], 1000, 1);

  }
  return 1;
}

CMD:tepireload(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  new buf[128];
  new cnt=obiekty_odswiezTelePickups(!isnull(params));
  format(buf, sizeof buf, "TelePickups: Odswiezono %d elementow", cnt);
  Msg(playerid, COLOR_INFO, buf);

  cnt=obiekty_odswiezReturnPickups();
  format(buf, sizeof buf, "ReturnPickups: Odswiezono %d elementow", cnt);
  Msg(playerid, COLOR_INFO, buf);

  cnt=obiekty_odswiezMapIcons();
  format(buf, sizeof buf, "MapIcons: Odswiezono %d elementow", cnt);
  Msg(playerid, COLOR_INFO, buf);


  cnt=obiekty_odswiezTeleCheckpoints();
  format(buf, sizeof buf, "TeleCheckpoints: Odswiezono %d elementow", cnt);
  Msg(playerid, COLOR_INFO, buf);

  cnt=obiekty_odswiezMiscPickups();
  format(buf, sizeof buf, "MiscPickups: Odswiezono %d elementow", cnt);
  return Msg(playerid, COLOR_INFO, buf);
}

CMD:addtepi(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");
  if (isnull(params)) return Msg(playerid,COLOR_ERROR,"/addtepi <nazwa>");
  new Float:PP[3], int, vw;
  GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);
  new buf[256],desc[64];
  mysql_real_escape_string(params,desc);
  format(buf,sizeof buf,"INSERT INTO fs_telepickups SET pX=%.3f,pY=%.3f,pZ=%.3f,pi=%d,pvw=%d,pickupid=1318,tX=0,tY=0,tZ=5,ti=0,tA=0,opis='%s'",
        PP[0], PP[1], PP[2], int, vw, desc);
  mysql_query(buf);
  format(buf, sizeof buf, "Dodano telepickup, ID: %d", mysql_insert_id());
  Msg(playerid,COLOR_INFO,buf);
  return 1;
}

CMD:addrepi(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");
  if (GetPlayerInterior(playerid)==0) return Msg(playerid,COLOR_ERROR,"Pickupy powrotne nie moga byc w interiorze 0");
  new Float:PP[3], int;
  GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
  int=GetPlayerInterior(playerid);
  new buf[256],desc[64];
  mysql_real_escape_string(params,desc);
  format(buf,sizeof buf,"INSERT INTO fs_returnpickups SET pX=%.3f,pY=%.3f,pZ=%.3f,pi=%d,pvw=-1,pickupid=1318,opis='%s'",
        PP[0], PP[1], PP[2], int, desc);
  mysql_query(buf);
  format(buf, sizeof buf, "Dodano returnpickup, ID: %d", mysql_insert_id());
  Msg(playerid,COLOR_INFO,buf);
  return 1;
}

CMD:addmipi(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");
  new Float:PP[3], int, pickupid, opis[65];

  if (sscanf(params,"ds[64]",pickupid, opis))
    return Msg(playerid, COLOR_ERROR, "/addmipi <pickupid> <opis>");

  GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
  int=GetPlayerInterior(playerid);
  new buf[256],desc[70];
  mysql_real_escape_string(opis,desc);
  format(buf,sizeof buf,"INSERT INTO fs_miscpickups SET pX=%.3f,pY=%.3f,pZ=%.3f,interior=%d,vw=0,pickupid=%d,descr='%s',addedby=%d",
        PP[0], PP[1], PP[2], int, pickupid, desc, pData[playerid][accountID]);
  mysql_query(buf);
  format(buf, sizeof buf, "Dodano miscpickup, ID: %d", mysql_insert_id());
  Msg(playerid,COLOR_INFO,buf);
  return 1;
}

CMD:tepitu(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  new Float:PP[4], int, pid, vw;

  if (sscanf(params,"d",pid))
    return Msg(playerid,COLOR_INFO2,"/tepitu [tepi id]");

  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");

  GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
  int=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);
  GetPlayerFacingAngle(playerid,PP[3]);

  new buf[256];
  format(buf,sizeof buf, "UPDATE fs_telepickups SET tX=%.3f,tY=%.3f,tZ=%.3f,ti=%d,tvw=%d,tA=%.1f WHERE id=%d",
      PP[0], PP[1], PP[2], int, vw, PP[3], pid);
  mysql_query(buf);
  Msg(playerid,COLOR_INFO,"Zrobione");
  return 1;
}

CMD:rmtepi(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  new pid;

  if (sscanf(params,"d",pid))
    return Msg(playerid,COLOR_INFO2,"/rmtepi [tepi id]");

  new buf[256];
  format(buf,sizeof buf, "DELETE FROM fs_telepickups WHERE id=%d LIMIT 1", pid);
  mysql_query(buf);
  Msg(playerid,COLOR_INFO,"Zrobione");
  return 1;
}

CMD:addmapicon(playerid,params[]){
  if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
  if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz byc pieszo!");

  new Float:PP[3], int, mapicon, opis[64]="-";

  if (sscanf(params,"ds[63]",mapicon,opis))
    return Msg(playerid,COLOR_INFO2,"/addmapicon <id-ikonki> [opis]");

  if (mapicon<0 || mapicon>63)
    return Msg(playerid,COLOR_INFO2,"Prawidlowy zakres id ikonki to 1-63. Lista pod http://wiki.sa-mp.com/wiki/MapIcons");
  GetPlayerPos(playerid, PP[0], PP[1], PP[2]);
  int=GetPlayerInterior(playerid);
  new buf[256],desc[64];
  mysql_real_escape_string(opis,desc);
  format(buf,sizeof buf,"INSERT INTO fs_mapicons SET pX=%.3f,pY=%.3f,pZ=%.3f,pi=%d,pvw=0,mapicon=%d,opis='%s'",
        PP[0], PP[1], PP[2], int, mapicon, desc);
  mysql_query(buf);
  format(buf, sizeof buf, "Dodano mapicon, ID: %d", mysql_insert_id());
  Msg(playerid,COLOR_INFO,buf);
  return 1;
}

public Pobiekty_odswiezPomniki()
  obiekty_odswiezPomniki();

obiekty_odswiezPomniki(bool:force=false){
 new pid, pval, pval2, pnick[32], buffer[255], prespect,i;
 if (force || random(3)==1) {
  mysql_query("SELECT id,session,nick,respect FROM `fs_players` ORDER BY session DESC LIMIT 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>dds[32]d", pid, pval, pnick, prespect))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, "%d. %s\n{ffffff}%dh\n{d0d0d0}Respekt: %d", i, pnick, floatround(pval/3600), prespect);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_czasgry][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 if (force || random(3)==1) {
  // skill
  mysql_query("SELECT id,skill,nick,respect FROM `fs_players` ORDER BY skill DESC LIMIT 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>dds[32]d", pid, pval, pnick, prespect))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}Skill: %d\n{d0d0d0}Respekt: %d"), i, pnick, pval, prespect);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_skill][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // bonusy
 if (force || random(3)==1) {
  mysql_query("select p.nick,pa.score from fs_players_achievements pa join fs_players p on pa.playerid=p.id where pa.shortname='WALIZKI' ORDER BY score DESC limit 3");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>s[32]d", pnick, pval))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d %s", i, pnick, pval, dli(pval,__("znaleziony bonus"),__("znalezione bonusy"),__("znalezionych bonusow")));
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_walizki][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // niski skill
 if (force || random(3)==1) {
  mysql_query("SELECT id,skill,nick,respect FROM `fs_players` ORDER BY skill ASC LIMIT 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>dds[32]d", pid, pval, pnick, prespect))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}Skill: %d\n{d0d0d0}Respekt: %d"), i, pnick, pval, prespect);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_lowskill][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // najwiecej listow
 if (force || random(3)==1) {
  mysql_query("SELECT pa.playerid,p.nick,pa.score FROM fs_players_achievements pa JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='LISTONOSZ' ORDER BY pa.score DESC limit 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>ds[32]d", pid, pnick, prespect))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}Ilosc listow: %d"), i, pnick, prespect);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_listy][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // najwiecej kasy w banku
 if (force || random(3)==1) {
  mysql_query("SELECT id,nick,bank_money FROM `fs_players` ORDER BY bank_money DESC LIMIT 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>ds[32]d", pid, pnick, prespect))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}Kasa w banku: %d"), i, pnick, prespect);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_cash][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // mistrzowie /derby
 if (force || random(3)==1) {
  mysql_query("SELECT pa.playerid,p.nick,pa.score derbywon,floor(pa.score/pa2.score*100) procent FROM fs_players_achievements pa JOIN fs_players_achievements pa2 ON pa2.playerid=pa.playerid AND pa2.shortname='DERBYFAN' JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='DERBYWIN' ORDER BY pa.score DESC limit 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>ds[32]dd", pid, pnick, pval, pval2))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}Wygranych: %d\n{d0d0d0}Wygrywa %01d%% pojedynkow"), i, pnick, pval, pval2);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_derby][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // mandaty
 if (force || random(3)==1) {
  mysql_query("SELECT pa.playerid,p.nick,pa.score mandaty FROM fs_players_achievements pa JOIN fs_players p on pa.playerid=p.id WHERE pa.shortname='FOTOCASH' ORDER BY pa.score DESC limit 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>ds[32]d", pid, pnick, pval))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d$", i, pnick, pval);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_fotocash][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // Kick/ban
 if (force || random(5)==1) {
  mysql_query("SELECT nick,kick_count,ban_count from fs_players order by kick_count desc limit 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>s[32]dd", pnick, pval, pval2))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, "%d. %s\n{ffffff}%d %s\n{d0d0d0}%d %s.", i, pnick, pval, dli(pval,__("kick"),__("kicki"),__("kickow")), pval2, dli(pval2,__("ban"),__("bany"),__("banow")));
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_kickban][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // mistrzowie /race
 if (force || random(3)==1) {
  mysql_query("select p.nick,pa.score racedist,pa2.score racefcnt from fs_players_achievements pa JOIN fs_players p ON p.id=pa.playerid JOIN fs_players_achievements pa2 ON pa2.playerid=pa.playerid AND pa2.shortname='RACEFCNT' WHERE pa.shortname='RACEDIST' order by pa.score DESC limit 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>s[32]dd", pnick, pval, pval2))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}Przejechanych %dKM\n{d0d0d0}w %d %s."), i, pnick, pval, pval2, dli(pval2,__("wyscigu"),__("wyscigach"),__("wyscigach")));//
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_racedist][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();
 }
 // solo killers
 if (force || random(5)==1) { // bardzo czasochlonne
  mysql_query("select p.nick,count(sm.id) pojedynkow,round(avg(fightlen)) sredniczas from fs_solo_matches sm join fs_players p ON p.id=sm.killerid GROUP by sm.killerid order by pojedynkow desc LIMIT 3;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(sscanf(buffer,"p<|>s[32]dd", pnick, pval, pval2))
        format(buffer,sizeof buffer, "-");
    else
      format(buffer,sizeof buffer, __("%d. %s\n{ffffff}%d %s\n{d0d0d0}Sredni czas walki: %dm%ds."), i, pnick, pval, dli(pval,__("wygrany pojedynek"),__("wygrane pojedynki"),__("wygranych pojedynkow")),floatround(pval2/60,floatround_floor),pval2%60);
    UpdateDynamic3DTextLabelText(TekstyPomniki[ea_soloklrs][i], 0xFF5219FF, buffer);
  }
  mysql_free_result();

 }
 // posterunek policji
 new longbuf[1023];
 mysql_query("select nick,hitman_prize  from fs_players WHERE timediff(NOW(),datetime_last)<'01:00:00' and hitman_prize>0 order by hitman_prize desc limit 5;");
 mysql_store_result();
 i=0;
 format(longbuf, sizeof longbuf, __("Poszukiwani przestepcy\n\n"));
 if (mysql_num_rows())
  while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
      format(longbuf,sizeof longbuf, "%s\n{ffffff}%d. %s {60ff60}%d$", longbuf, i, pnick, pval);

 }
 mysql_free_result();
 for (i=0;i<3;i++)
  UpdateDynamic3DTextLabelText(PPRanking[i], 0xFF0000FF, longbuf);

 // najlepsi na ONEDE2
 /*if (force || random(3)==1) {
  format(longbuf, sizeof longbuf, __("Najlepsi na /ONEDE2\n\n{ffffff}"));
  mysql_query("select p.nick,pa.kills from fs_players_arenascore pa JOIN fs_players p on p.id=pa.id_player WHERE pa.id_arena=10 ORDER BY pa.kills DESC limit 5;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
    while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
      format(longbuf,sizeof longbuf, "%s\n{ffffff}%d. %s {60ff60}%d %s", longbuf, i, pnick, pval, dli(pval,"frag","fragi","fragow"));
    }
  mysql_free_result();
  UpdateDynamic3DTextLabelText(ArenaRanking[ARENA_ONEDE2][0], 0xFF0000FF, longbuf);
 }*/
 // najlepsi na ONEDE
 if (force || random(3)==1) {
  format(longbuf, sizeof longbuf, __("Najlepsi na /ONEDE\n\n{ffffff}"));
  mysql_query("select p.nick,pa.kills from fs_players_arenascore pa JOIN fs_players p on p.id=pa.id_player WHERE pa.id_arena=6 ORDER BY pa.kills DESC limit 5;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
    while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
      format(longbuf,sizeof longbuf, "%s\n{ffffff}%d. %s {60ff60}%d %s", longbuf, i, pnick, pval, dli(pval,"frag","fragi","fragow"));
    }
  mysql_free_result();
  UpdateDynamic3DTextLabelText(ArenaRanking[ARENA_ONEDE][0], 0xFF0000FF, longbuf);
 }
 // najlepsi na ONEDE (tyg)
 if (force || random(3)==1) {
  format(longbuf, sizeof longbuf, __("Najlepsi na /ONEDE\nw tym tygodniu\n{ffffff}"));
  mysql_query("select p.nick,pa.kills from fs_players_arenascore_week pa JOIN fs_players p on p.id=pa.id_player WHERE pa.id_arena=6 ORDER BY pa.kills DESC limit 5;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
    while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
      format(longbuf,sizeof longbuf, "%s\n{ffffff}%d. %s {60ff60}%d %s", longbuf, i, pnick, pval, dli(pval,"frag","fragi","fragow"));
    }
  mysql_free_result();
  UpdateDynamic3DTextLabelText(ArenaRanking[ARENA_ONEDE][1], 0xFF0000FF, longbuf);
 }
 // najlepsi na ONEDE2 (tyg)
 /*if (force || random(3)==1) {
  format(longbuf, sizeof longbuf, __("Najlepsi na /ONEDE2\nw tym tygodniu\n{ffffff}"));
  mysql_query("select p.nick,pa.kills from fs_players_arenascore_week pa JOIN fs_players p on p.id=pa.id_player WHERE pa.id_arena=10 ORDER BY pa.kills DESC limit 5;");
  mysql_store_result();
  i=0;
  if (mysql_num_rows())
    while (mysql_fetch_row_format(buffer,"|")) {
    i++;
    if(!sscanf(buffer,"p<|>s[32]d", pnick, pval))
      format(longbuf,sizeof longbuf, "%s\n{ffffff}%d. %s {60ff60}%d %s", longbuf, i, pnick, pval, dli(pval,"frag","fragi","fragow"));
    }
  mysql_free_result();
  UpdateDynamic3DTextLabelText(ArenaRanking[ARENA_ONEDE2][1], 0xFF0000FF, longbuf);
 }*/
}

IsPlayerInNoDMArea(playerid, bool:spec=false){
  if (!IsPlayerConnected(playerid)) return 0;
  new pstate=GetPlayerState(playerid);
  if (pstate==PLAYER_STATE_NONE) return 0;
  if (!spec) if(pstate==PLAYER_STATE_SPECTATING) return 0;

  for(new i=0;i<sizeof StrefyNoDM;i++)
    if (StrefyNoDM[e_StrefyNoDM:i]!=StrefyNoDM[ea_KSW] && StrefyNoDM[e_StrefyNoDM:i]!=StrefyNoDM[ea_WWK] && IsPlayerInDynamicArea(playerid,StrefyNoDM[e_StrefyNoDM:i]))
      return 1;
  return 0;
}

bool:IsPlayerInFullDMArea(playerid){
  if (!IsPlayerConnected(playerid)) return false;
  for(new i=0;i<sizeof StrefyFullDM;i++)
    if (IsPlayerInDynamicArea(playerid,StrefyFullDM[e_StrefyFullDM:i]))
      return true;
  return false;
}

obiekty_OPEnterDynamicArea(playerid,areaid){
  if (areaid==StrefyNoDM[ea_kosciolINT]) {
    Audio_PlayBGSound(playerid, AUDIOID_FXKOSCIOL);
  } else if (areaid==StrefyFullDM[ea_tunele]) {
    Audio_PlayBGSound(playerid, AUDIOID_FXTUNEL1);
  } else if (areaid==DAs[eDA_tuneleKrzyk]) {
    Audio_PlayBGSound(playerid,AUDIOID_FXTUNEL2);
    return 1;
  }
  for(new i=0;i<sizeof KillZones;i++)
    if (areaid==KillZones[i]) {
        new Float:P[6];
        GetPlayerPos(playerid,P[0],P[1],P[2]);
      GetPlayerVelocity(playerid,P[3],P[4],P[5]);

        CreateExplosion(P[0],P[1],P[2], 2, 35.0);
        CreateExplosion(P[0]+random(2)-1,P[1]+random(2)-1,P[2]+random(2)-1, 2, 35.0);
        CreateExplosion(P[0]+random(2)-1,P[1]+random(2)-1,P[2]+random(2)-1, 2, 35.0);

      CreateExplosion(P[0]+P[3],P[1]+P[4],P[2]+P[5], 2, 35.0);
      CreateExplosion(P[0]+(P[3]*2),P[1]+(P[4]*2),P[2]+(P[5]*2), 2, 35.0);
      SetPlayerHealth(playerid,0);

      return 1;
  }
  for(new i=0;i<sizeof StrefyNoDM;i++)
    if (areaid==StrefyNoDM[e_StrefyNoDM:i])
    {
      if(e_StrefyNoDM:i==ea_hacjenda)
        Audio_PlayBGSound(playerid,AUDIOID_FXHACJENDA,false,false,true);
            //if(e_StrefyNoDM:i==ea_urodziny2013)
      //  PlayAudioStreamForPlayer(playerid, "http://listen.slotex.pl/7596.pls");
      // Msg(playerid,COLOR_INFO2,"Wkraczasz do wolnej strefy, obowiazuje zakaz zabijania");
      // to wyzej sie buguje ;/

      TextDrawHideForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
      TextDrawShowForPlayer(playerid,gTextDraw[TD_AREANODM]);
      SetPlayerArmedWeapon(playerid,0); // piesci
      pTemp[playerid][wStrefieNODM]=true;
      return 1;
    }
  for(new i=0;i<sizeof StrefyFullDM;i++)
    if (areaid==StrefyFullDM[e_StrefyFullDM:i])
    {
      new pstate=GetPlayerState(playerid);
      if (pstate==PLAYER_STATE_WASTED || pstate==PLAYER_STATE_SPECTATING || pstate==PLAYER_STATE_NONE) return 1;

      if(e_StrefyFullDM:i==ea_wojsko)                       // w petli zle brzmia
        Audio_PlayBGSound(playerid,AUDIOID_FXAFGANISTAN);
      else if(e_StrefyFullDM:i==ea_wietnam)
        Audio_PlayBGSound(playerid,AUDIOID_FXWIETNAM);
      pTemp[playerid][wStrefieFULLDM]=true;
      TextDrawHideForPlayer(playerid,gTextDraw[TD_AREANODM]);
      TextDrawShowForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
      return 1;
    }
  for (new i=0;i<sizeof Fotoradary;i++)
    if (Fotoradary[i]==areaid)
      return obiekty_Fotokomorka(playerid);
  return 0;
}

obiekty_OPLeaveDynamicArea(playerid,areaid){
    StopAudioStreamForPlayer(playerid);

  for(new i=0;i<sizeof StrefyNoDM;i++)
    if (areaid==StrefyNoDM[e_StrefyNoDM:i]) {
      Audio_StopBGSound(playerid);
      if (IsPlayerInNoDMArea(playerid)) return 1;
      pTemp[playerid][wStrefieNODM]=false;
      return TextDrawHideForPlayer(playerid,gTextDraw[TD_AREANODM]);
    }

  for(new i=0;i<sizeof StrefyFullDM;i++)
    if (areaid==StrefyFullDM[e_StrefyFullDM:i]) {
      Audio_StopBGSound(playerid);

      new pstate=GetPlayerState(playerid);

      if (pstate==PLAYER_STATE_WASTED || pstate==PLAYER_STATE_SPECTATING || pstate==PLAYER_STATE_NONE) return 1;
      if (IsPlayerInFullDMArea(playerid)) return 1;
      else if (pData[playerid][pAttraction]==A_NONE) {
        RemoveDangerousWeapon(playerid);
        SetPlayerArmedWeapon(playerid,0);
      }
      pTemp[playerid][wStrefieFULLDM]=false;
      return TextDrawHideForPlayer(playerid,gTextDraw[TD_AREAFULLDM]);
    }
  if (areaid==DAs[eDA_andromeda]) {
    new count = GetTickCount()/3000;
    new x= (count % 3500);
    new y= (count/x)%3500;
    x=x-1850+random(150);
    y=y-1850+random(150);
    SetPlayerInterior(playerid,0);
    SetPlayerPos(playerid,x,y,1500.0);
    return 1;
  }
  return 0;
}

obiekty_Fotokomorka(playerid){
  if (!IsPlayerInAnyVehicle(playerid)) return 1;
  if (GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return 1;
  if (pData[playerid][pAttraction]!=A_NONE) return 1;
  new vid=GetPlayerVehicleID(playerid);
  
  new vf=GetVehicleFlags(vid);
  if (vf&VF_MILITARY==VF_MILITARY || vf&VF_AIRBORNE==VF_AIRBORNE || vf&VF_NATATORIAL==VF_NATATORIAL || vf&VF_POLICE==VF_POLICE) return 1;
  if (pTemp[playerid][faction]==FACTION_POLICE || pTemp[playerid][faction]==FACTION_ARMY) return 1;
  
  new spd=GetVehicleSpeed(vid);
  new Float:VV[3];
  GetVehicleVelocity(vid,VV[0],VV[1],VV[2]);
  if (spd>110 && floatabs(VV[2])<(floatabs(VV[0])+floatabs(VV[1]))) {
      new buf[128];
      FlashScreen(playerid);
      format(buf,sizeof buf,__("_~n~_~n~_~n~_~n~_~n~~r~%d KM/h~n~~w~Jedziesz zbyt szybko!"), spd);
      GameTextForPlayer(playerid, buf, 4000, 6);
      if (Audio_IsClientConnected(playerid))
        Audio_Play(playerid,50,false,false,true);

      if(GetPlayerMoney(playerid)>=(floatround(spd)*3)){
        format(buf,sizeof buf,__("[FOTORADAR] {ff5050}Przekroczyles dopuszczalna predkosc! {50ff50}Mandat: {ffffff}%d$"), (floatround(spd)*3));
        GivePlayerMoney(playerid, -(floatround(spd)*3));
      }else{
          format(buf,sizeof buf,__("[FOTORADAR] {ff5050}Przekroczyles dopuszczalna predkosc! {50ff50}Zadluzony mandat: {ffffff}%d$"), (floatround(spd)*3));
          IncreasePlayerHitman(playerid, (floatround(spd)*3));
      }
      SendClientMessage(playerid,-1,buf);
      avt_record(playerid,e_fotocash,(floatround(spd)*3),ART_ADD);
  }
  return 1;
}

forward removePlayerBloodObj(playerid);
public removePlayerBloodObj(playerid)
{
    for(new i; i < 2 ; i++) DestroyObject(pTemp[playerid][bloodId][i]);
}

RemoveBuildings(playerid) // RemoveBuildingForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius);
{
  
  // VENDING MACHINES
  RemoveBuildingForPlayer(playerid, 1302, 0.0, 0.0, 0.0, 6000.0);
  RemoveBuildingForPlayer(playerid, 1209, 0.0, 0.0, 0.0, 6000.0);
  RemoveBuildingForPlayer(playerid, 955, 0.0, 0.0, 0.0, 6000.0);
  RemoveBuildingForPlayer(playerid, 956, 0.0, 0.0, 0.0, 6000.0);
  RemoveBuildingForPlayer(playerid, 1775, 0.0, 0.0, 0.0, 6000.0);
  RemoveBuildingForPlayer(playerid, 1776, 0.0, 0.0, 0.0, 6000.0);
  RemoveBuildingForPlayer(playerid, 1977, 0.0, 0.0, 0.0, 6000.0);

  // THE STRIP
  RemoveBuildingForPlayer(playerid, 8498, 2231.8047, 1035.7188, 46.8203, 0.25);
  RemoveBuildingForPlayer(playerid, 8499, 2265.5625, 1130.2656, 46.8203, 0.25);
  RemoveBuildingForPlayer(playerid, 8705, 2231.8047, 1035.7188, 46.8203, 0.25);
  RemoveBuildingForPlayer(playerid, 8929, 2265.5625, 1130.2656, 46.8203, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2292.4453, 984.8594, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2172.8125, 1181.3750, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2202.6563, 1181.6953, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2236.5313, 1181.6953, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2336.0156, 985.3281, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2336.0156, 1024.5781, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2336.0156, 1073.6953, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2305.1328, 1181.6953, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2336.0156, 1134.6797, 9.6953, 0.25);
  RemoveBuildingForPlayer(playerid, 3509, 2336.0156, 1181.6953, 9.6953, 0.25);

  // LAS COLINAS
  RemoveBuildingForPlayer(playerid,1409,2426.91919,-1017.08392,53.95939,2.8137845993042);
  RemoveBuildingForPlayer(playerid,5449,2124.89063,-1023.35937,53.6875,99.250221252441);

  // GLEEN PARK
  RemoveBuildingForPlayer(playerid, 5464, 1902.42969, -1309.53906, 29.91406, 91.840255737305);

  // WILLOFIELD
  RemoveBuildingForPlayer(playerid, 3744, 2159.8281, -1930.6328, 15.0781, 0.25);
  RemoveBuildingForPlayer(playerid, 3574, 2159.8281, -1930.6328, 15.0781, 0.25);
  RemoveBuildingForPlayer(playerid, 3567, 2142.91406, -1947.42187, 13.26563, 11.069401741028);

  // OCEAN DOCKS
  RemoveBuildingForPlayer(playerid, 1315, 2334.4021, -2341.60498, 15.31512, 7.384117603302);
  RemoveBuildingForPlayer(playerid, 1412, 2323.6563, -2353.5625, 13.7422, 42.250714302063);
  RemoveBuildingForPlayer(playerid, 3578, 2235.16406, -2231.85156, 13.25781, 7.250714302063);
  RemoveBuildingForPlayer(playerid, 3569, 2234.39063, -2244.82812, 14.9375, 11.431450843811);
  RemoveBuildingForPlayer(playerid, 3569, 2219.42188, -2259.52344, 14.88281, 11.431450843811);
  RemoveBuildingForPlayer(playerid, 3578, 2165.07031, -2288.96875, 13.25781, 7.250714302063);
  RemoveBuildingForPlayer(playerid, 3578, 2194.47656, -2242.875, 13.25781, 7.250714302063);
  RemoveBuildingForPlayer(playerid, 3574, 2165.29688, -2317.5, 14.8125, 13.891983032227);
  RemoveBuildingForPlayer(playerid, 3744, 2165.29688, -2317.5, 14.8125, 13.891983032227);

  // MARKET
  RemoveBuildingForPlayer(playerid, 1412, 1327.4297, -1239.9844, 13.9375, 7.250714302063);
  RemoveBuildingForPlayer(playerid, 6253, 1305.4688, -1619.7422, 13.3984, 0.25);
  RemoveBuildingForPlayer(playerid, 6046, 1305.4688, -1619.7422, 13.3984, 0.25);
  RemoveBuildingForPlayer(playerid, 1219, 1333.87109, -1242.06335, 13.33703, 3.8763394355774);

  // FLINT COUNTY
  RemoveBuildingForPlayer(playerid, 3260, -1063.59448, -1155.05823, 129.22318, 3.798544883728);
  RemoveBuildingForPlayer(playerid, 3260, -1037.6687, -1185.38892, 129.55029, 3.798544883728);

  // DOHERTY
  RemoveBuildingForPlayer(playerid, 11372, -2076.4375, -107.9297, 36.9688, 0.25);
  RemoveBuildingForPlayer(playerid, 11014, -2076.4375, -107.9297, 36.9688, 0.25);
  RemoveBuildingForPlayer(playerid, 11223, -2049.17187, 250.32031, 33.07813, 14.529824256897);

  // KOLO
  RemoveBuildingForPlayer(playerid, 6463, 389.7734, -2028.4688, 19.8047, 0.5);
  RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2035.3828, 29.9531, 50);
  RemoveBuildingForPlayer(playerid, 6298, 389.7734, -2028.4688, 19.8047, 0.5);
  RemoveBuildingForPlayer(playerid, 6461, 389.7734, -2028.5000, 20.1094, 0.5);
  RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2028.5000, 32.2266, 50);

  // MILITARY FUELS
  RemoveBuildingForPlayer(playerid, 9436, -2381.3828, 2216.0000, 11.8750, 0.25);
  RemoveBuildingForPlayer(playerid, 9237, -2381.3828, 2216.0000, 11.8750, 0.25);

  // BAYSIDE
  RemoveBuildingForPlayer(playerid, 985, 2497.4063, 2777.0703, 11.5313, 0.25);
  RemoveBuildingForPlayer(playerid, 986, 2497.4063, 2769.1094, 11.5313, 0.25);
  
  // BAZA GANGOW: EAST BEACH
  RemoveBuildingForPlayer(playerid, 17550, 2870.24219, -1589.39062, 16.5625, 53.024227142334);
  RemoveBuildingForPlayer(playerid, 716, 2865.18286, -1593.97058, 12.15054, 12.400197029114);
  RemoveBuildingForPlayer(playerid, 3942, -787.36719, 499.64844, 1374.71094, 16.465103149414);
  RemoveBuildingForPlayer(playerid, 3944, -787.49219, 505.875, 1374.71875, 12.289823532104);
  RemoveBuildingForPlayer(playerid, 3921, -778.97656, 493.96094, 1375.70313, 6.142128944397);
  
  // FUCKIN' CHAIRS ON /ONEDE AND /ONEGUN
  RemoveBuildingForPlayer(playerid, 1715, 235.8438, 110.8203, 1009.2188, 6000.0);
  RemoveBuildingForPlayer(playerid, 1714, 237.8438, 109.8906, 1009.2734, 6000.0);
  RemoveBuildingForPlayer(playerid, 1806, 271.2109, 110.6094, 1007.8047, 6000.0);
  RemoveBuildingForPlayer(playerid, 2356, 273.3594, 115.7969, 1007.7969, 6000.0);
  RemoveBuildingForPlayer(playerid, 1722, 274.0078, 126.6406, 1007.8203, 6000.0);

}
// EOF