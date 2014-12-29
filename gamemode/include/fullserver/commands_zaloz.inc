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

CMD:zaloz(playerid,params[]){

  copy(params,pData[playerid][pDzaloz]);
  
  if(!isnull(params)) {

  if(pData[playerid][respect]>=150000){
      if(!strcmp(params,"papuga",true)){
          return SetPlayerAttachedObject(playerid, 0, 19079, 3, 0.0, 0.0, 0.0, 0.0, 0.0, 180.0);
    }
  }
  if(pData[playerid][respect]>=100000) {
    if (!strcmp(params,"laser",true)) {
      zaloz_laser(playerid);
      return 1;
    }
    if (!strcmp(params,"laser2",true)) {
      zaloz_laser2(playerid);
      return 1;
    }
  }
  if(pData[playerid][respect]>=50000) {
    if (!strcmp(params,"murzyn",true))
      return SetPlayerAttachedObject( playerid, 0, 18963, 2, 0.138115, 0.035240, 0.000000, 90.000000, 90.000000, 0.000000, 1.100000, 1.100000, 1.100000 ); // CJElvisHead - glowaxjl
    else if (!strcmp(params,"rogi",true))
            return SetPlayerAttachedObject(playerid, 0, 19314, 2, 0.1, 0.05, 0.0, 0.0, 0.0, -75.0, 0.4, 0.4, 0.4); // rogi od 50k respa

  }
  if(pData[playerid][respect]>=15000) {
    if (!strcmp(params,"koparka",true)){
      return SetPlayerAttachedObject( playerid, 0, 18248, 2, 0.315063, 0.321018, -0.221677, 258.909912, 96.667274, 277.171569, 0.050388, 0.050388, 0.050388 ); // cuntwjunk01 - koparka od 10000 respa
    }
    else if (!strcmp(params,"lom",true))
      return SetPlayerAttachedObject( playerid, 0, 18634, 5, 0.106173, 0.030468, -0.015657, 258.909912, 269.149536, 14.354298, 1.163060, 1.163060, 1.163060 ); // GTASACrowbar1 - Â³om od 15k respa
    else if (!strcmp(params,"wigwam",true))
      return SetPlayerAttachedObject( playerid, 0, 3243, 1, -1.057782, -0.016429, -0.009905, 0.000000, 85.779541, 354.995361, 0.308334, 0.308334, 0.308334 ); // tepee_room_ - wigwam 15000 respa
    else if (!strcmp(params,"maska",true)){
            switch(random(3)){
        case 0: SetPlayerAttachedObject(playerid, 0, 19036, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); //maska hokejowa biala
        case 1: SetPlayerAttachedObject(playerid, 0, 19037, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); //maska hokejowa czerwona
        case 2: SetPlayerAttachedObject(playerid, 0, 19038, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); //maska hokejowa zielona
                }
            return 1;
        }
    }
  if(pData[playerid][respect]>=5000) {
    if (!strcmp(params,"boombox",true))
      return SetPlayerAttachedObject( playerid, 0, 2226, 1, 0.000000, -0.203173, -0.080502, 0.000000, 22.671218, 0.000000, 1.000000, 1.000000, 1.000000 ); // LOW_HI_FI_3 - magnetofon 5000 respa
    else if (!strcmp(params,"mikolaj",true))
      return SetPlayerAttachedObject( playerid, 0, 19065, 2, 0.113131, 0.018596, 0.000000, 94.697052, 90.118164, 0.000000, 1.226595, 1.226595, 1.226595 ); // SantaHat2 - czapkamikolaja 5000 respa
    else if (!strcmp(params,"mlotek",true))
      return SetPlayerAttachedObject( playerid, 0, 18635, 2, -0.250711, -0.464394, 0.159861, 258.909912, 45.141162, 5.301577, 1.920960, 1.920960, 1.920960 ); // GTASAHammer1 - mlotek od 2500 respa
    else if (!strcmp(params,"srubokret",true))
      return SetPlayerAttachedObject( playerid, 0, 18644, 2, 0.151123, 0.345173, 0.019663, 275.692535, 10.100483, 357.343811, 1.924180, 1.924180, 1.924180 ); // Screwdriver1 - srubokret od 2500 respa
        else if (!strcmp(params,"kask",true)){
      switch(random(4)){
            case 0: SetPlayerAttachedObject(playerid, 0, 18645, 2, 0.07, 0, 0, 88, 75, 0);
            case 1: SetPlayerAttachedObject(playerid, 0, 18976, 2, 0.07, 0, 0, 88, 75, 0);
              case 2: SetPlayerAttachedObject(playerid, 0, 18976, 2, 0.07, 0, 0, 88, 75, 0);
              case 3: SetPlayerAttachedObject(playerid, 0, 18976, 2, 0.07, 0, 0, 88, 75, 0);
        }
      return 1;
    }
  }
  if(pData[playerid][respect]>=2500) {
    if (!strcmp(params,"spoiler",true))
      return SetPlayerAttachedObject( playerid, 0, 1060, 1, 0.239907, -0.077574, 0.000871, 2.348074, 87.804405, 357.280487, 0.652846, 0.652846, 0.652846 ); // spl_c_st_r - spojler2 2500 respa
    else if (!strcmp(params,"gitara",true))
      return SetPlayerAttachedObject( playerid, 0, 19319, 1, 0.000000, -0.083173, -0.080502, 0.000000, 22.671218, 0.000000, 1.000000, 1.000000, 1.000000 ); //
        else if (!strcmp(params,"okulary",true)){
            switch(random(8)){
        case 0: SetPlayerAttachedObject(playerid, 0, 19022, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary czarne
        case 1: SetPlayerAttachedObject(playerid, 0, 19023, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary niebieskie
        case 2: SetPlayerAttachedObject(playerid, 0, 19024, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary fioletowe
        case 3: SetPlayerAttachedObject(playerid, 0, 19025, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary rozowe
        case 4: SetPlayerAttachedObject(playerid, 0, 19026, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary lososiowe
        case 5: SetPlayerAttachedObject(playerid, 0, 19027, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary pomaranczowe
        case 6: SetPlayerAttachedObject(playerid, 0, 19028, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary zolte
        case 7: SetPlayerAttachedObject(playerid, 0, 19029, 2, 0.09, 0.03, 0.0, 90.0, 90.0, 0.0, 1.0, 1.0, 1.0); // okulary zielone
                }
            return 1;
        }
    }
  if(pData[playerid][respect]>=500) {
    if(!strcmp(params,"afro",true))
      return SetPlayerAttachedObject( playerid, 0, 18975, 2, 0.086262, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000 ); // Hair2 - peruka od 500 respa

    else if (!strcmp(params,"grabie",true))
      return SetPlayerAttachedObject( playerid, 0, 18890, 5, 0.037106, 0.096538, 0.019960, 337.156921, 4.156515, 252.433853, 1.000000, 1.000000, 1.000000 ); // Rake1 - grabie od 500 respa
    else if (!strcmp(params,"kurczak",true))
      return SetPlayerAttachedObject( playerid, 0, 19137, 2, 0.092990, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.076031, 1.076031, 1.076031 ); // CluckinBellHat1 - kurczak 1000 respa
    else if (!strcmp(params,"czapka",true))
      return SetPlayerAttachedObject( playerid, 0, 18929, 2, 0.129550, 0.000189, -0.006934, 0.000000, 0.000000, 0.000000, 1.089081, 1.089081, 1.089081 ); // Hat4 - czapkadaniva
    else if (!strcmp(params,"sluchawki",true))
      return SetPlayerAttachedObject(playerid, 0, 19424, 2, 0.07, 0, 0, 90, 0, 270);
    else if (!strcmp(params,"telefon",true))
      return SetPlayerAttachedObject( playerid, 0, 18873, 1, -0.012269, 0.021618, 0.112074, 255.530456, 177.323989, 63.659469, 4.356958, 4.356958, 4.356958 ); // MobilePhone9 - telefon id 0 respa 500
    else if (!strcmp(params,"bandana2",true))
      return SetPlayerAttachedObject( playerid, 0, 18918, 2, 0.066215, 0.027532, 0.000000, 275.669555, 5.640456, 265.868133, 1.000000, 1.000000, 1.000000 );
    else if (!strcmp(params,"bandana",true))
      return SetPlayerAttachedObject( playerid, 0, 18912, 2, 0.086431, 0.022617, -0.000721, 90.245780, 190.631896, 89.033378, 1.054243,  1.054243, 1.054243 ); // Mask2 - bandana Sayu
  }
  
  if (!strcmp(params,"cygaro",true))
    return SetPlayerAttachedObject( playerid, 0, 1485, 2, 0.102237, 0.094420, -0.038719, 61.115173, 0.000000, 140.122970 ); // CJ_CIGGY - cygaro
  else if (!strcmp(params,"kapelusz",true))
    return SetPlayerAttachedObject( playerid, 0, 18962, 2, 0.146981, 0.013410, 0.000000, 0.000000, 0.000000, 0.000000, 1.223083, 1.223083, 1.223083 ); // CowboyHat2 - kapelusz
  else if (!strcmp(params,"katana",true))
    return SetPlayerAttachedObject( playerid, 0, 339, 2, 0.178890, 0.004188, 0.130645, 346.583984, 188.009109, 12.984556 );
  else if (!strcmp(params,"pacholek",true))
    return SetPlayerAttachedObject( playerid, 0, 1238, 2, 0.435544, -0.001684, 0.000000, 0.000000, 90.000000, 285.874969 );
  else if (!strcmp(params,"rycerz",true))
    return SetPlayerAttachedObject( playerid, 0, 1549, 1, -0.173378, 0.021022, -0.012099, 3.122609, 87.657714, 355.966491, 1.000000, 1.000000, 1.000000 ); // CJ_ASHTRAY_b - Smietnik
  else if (!strcmp(params,"siano",true))
    return SetPlayerAttachedObject( playerid, 0, 1453, 1, -0.202300, 0.010707, -0.029694, 0.737283, 267.933135, 0.000000, 1.000000, 1.000000, 1.000000 ); // DYN_H_BALE1 - Siano
  else if (!strcmp(params,"swietlik",true))
    return SetPlayerAttachedObject( playerid, 0, 3785, 1, 0.073756, -0.125172, 0.002187, 30.842874, 88.859939, 234.537429, 1.000000, 1.000000, 1.000000 ); // bulkheadlight - 9
  else if (!strcmp(params,"telefon",true))
    return SetPlayerAttachedObject( playerid, 0, 18873, 1, -0.012269, 0.021618, 0.112074, 255.530456, 177.323989, 63.659469, 4.356958, 4.356958, 4.356958 ); // MobilePhone9 - telefon id 0 respa 500
  else if (!strcmp(params,"torba",true))
    return SetPlayerAttachedObject( playerid, 0, 2663, 2, 0.233547, 0.028803, 0.017788, 271.072326, 88.000000, 0.000000 );
  else if (!strcmp(params,"zolwik",true))
    return SetPlayerAttachedObject( playerid, 0, 1609, 1, 0.041243, -0.101723, 0.016250, 89.706207, 349.184875, 272.334106, 0.190168, 0.190168, 0.190168 ); // turtle - plecaczek
  //else if(!strcmp(params,"dynia",true))
  //    return SetPlayerAttachedObject( playerid, 0, 19320, 2, 0.138115, 0.035240, 0.000000, 90.000000, 90.000000, 0.000000, 0.800000, 0.800000, 0.800000 );

  }
  copy("",pData[playerid][pDzaloz]);
  RemovePlayerAttachedObject(playerid,0);

  Msg(playerid, COLOR_INFO,"Uzyj: /zaloz <nazwa>");
  Msg(playerid, COLOR_INFO, "Uzyj: {b}/zz{/b} aby zapisac zalozony obiekt na stale (wymaga posiadania domu).");
  Msg(playerid, COLOR_INFO, "Uzyj: {b}/zd{/b} aby usunac zapisane obiekty.");
  SendClientMessage(playerid,-1,"");
  //SendClientMessage(playerid,0xe0e0e0ff,"HALLOWEEN: {ffffff}dynia");
  SendClientMessage(playerid,0xe0e0e0ff,"Dla wszystkich: {ffffff}cygaro, kapelusz, katana, pacholek, rycerz, siano, swietlik, telefon, torba, zolwik");
  SendClientMessage(playerid,0xe0e0e0ff,"Od 500 respektu: {ffffff}afro, czapka, bandana, bandana2, grabie, kurczak, telefon, sluchawki");
  SendClientMessage(playerid,0xd0e0d0ff,"Od 2.5k respektu: {ffffff}spoiler, gitara, okulary");
  SendClientMessage(playerid,0xd0e0d0ff,"Od 5k respektu: {ffffff}boombox, mikolaj, mlotek, srubokret, kask");
  SendClientMessage(playerid,0xc0e0c0ff,"Od 15k respektu: {ffffff}koparka, lom, wigwam, maska");
  SendClientMessage(playerid,0xb0b0b0ff,"Od 50k respektu: {ffffff}murzyn, rogi");
  SendClientMessage(playerid,0xb0b0b0ff,"Od 100k respektu: {ffffff}laser, laser2");
  SendClientMessage(playerid,0xb0b0b0ff,"Od 150k respektu: {ffffff}papuga");
  return 1;
}

CMD:zz(playerid){
  if (!pData[playerid][loggedIn]) return Msg(playerid,COLOR_ERROR,"Komenda dostepna tylko dla zarejestrowanych uzytkownikow.");

  new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);

  if (hid==-1)
    return Msg(playerid,COLOR_ERROR,"Musisz miec dom aby moc zapisac/usuwaæ przedmiot.");

  if (strlen(pData[playerid][pDzaloz])<1)
    return Msg(playerid,COLOR_ERROR,"Nie masz obecnie zadnego zapisanego przedmiotu - uzyj komendy /zaloz");

  new e_zaloz[40];
  mysql_real_escape_string(pData[playerid][pDzaloz],e_zaloz);
  format(gstr,sizeof gstr,"UPDATE fs_players SET zaloz='%s' WHERE id=%d LIMIT 1", e_zaloz, pData[playerid][accountID]);
  mysql_query(gstr);

  Msg(playerid,COLOR_INFO,"Przedmioty zostaly zapisane.");

  return 1;
}

CMD:zd(playerid){
    if (!pData[playerid][loggedIn]) return Msg(playerid,COLOR_ERROR,"Komenda dostepna tylko dla zarejestrowanych uzytkownikow.");

  new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);

  if (hid==-1)
    return Msg(playerid,COLOR_ERROR,"Musisz miec dom aby moc zapisac/usuwaæ przedmiot.");

  //usuwamy przedmioty
  copy("",pData[playerid][pDzaloz]);
  RemovePlayerAttachedObject(playerid,0);

  //usuwamy zapisane przedmioty z bazy
  format(gstr,sizeof gstr,"UPDATE fs_players SET zaloz=NULL WHERE id=%d LIMIT 1", pData[playerid][accountID]);
  mysql_query(gstr);

  Msg(playerid,COLOR_INFO,"Przedmioty zostaly usuniête.");

  return 1;
}

zaloz_laser(playerid) { // dopasowane przez timesa
 switch(GetPlayerSkin(playerid)){
  case 0: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.492748, 0.040626, 0.034564, 0.000000, 0.000000, 94.857414, 1.000000, 1.000000, 1.000000 ); // LaserPointer1 - skin 0 laser w oku
  case 1: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474780, 0.045132, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 1
  case 2: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.450285, 0.045132, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 2
  case 7: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.469900, 0.053491, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 7
  case 8: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.469900, 0.053491, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 8
  case 9: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.444506, 0.038463, 0.038085, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 9
  case 10: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.352782, 0.032301, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 10
  case 11: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474426, 0.042763, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 11
  case 12: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.464383, 0.043365, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 12
  case 13: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.512461, 0.043365, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 13
  case 14: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.477542, 0.070707, 0.041043, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 14
  case 15: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.461280, 0.034678, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 15
  case 16: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.511655, 0.059833, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 16
  case 17: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.462779, 0.045729, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 17
  case 18: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.449220, 0.035496, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 18
  case 19: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.528088, 0.045958, 0.033756, 0.000000, 0.000000, 102.224014, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 19
  case 20: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485164, 0.044673, 0.029965, 0.000000, 0.000000, 90.414581, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 20
  case 21: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485164, 0.059621, 0.029965, 0.000000, 0.000000, 90.414581, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 21
  case 22: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485164, 0.059621, 0.029965, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 22
  case 23: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485164, 0.059621, 0.029965, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 23
  case 24: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485164, 0.044223, 0.029965, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 24
  case 25: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.479192, 0.051794, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 25
  case 26: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.500002, 0.058987, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 26
  case 27: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.492902, 0.058987, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 27
  case 28: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.492902, 0.058987, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 28
  case 29: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481123, 0.067273, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 29
  case 30: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481123, 0.064109, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 30
  case 31: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.391128, 0.054122, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 31
  case 32: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.459490, 0.018460, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 32
  case 33: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.426369, 0.040455, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 33
  case 34: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473469, 0.040455, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 34
  case 35: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473469, 0.034219, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 35
  case 36: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473469, 0.034219, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 36
  case 37: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473469, 0.034219, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 -  skin 37
  case 38: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.365544, 0.034219, 0.024311, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 38
  case 39: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.365544, 0.034219, 0.033679, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 39
  case 40: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.476661, 0.038572, 0.033679, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 40
  case 41: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.515075, 0.055830, 0.033679, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 41
  case 43: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.405384, 0.047376, 0.042409, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 43
  case 44: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.419853, 0.047376, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 44
  case 45: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.453621, 0.034325, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 45
  case 46: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473560, 0.066463, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 46
  case 47: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473560, 0.066463, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 47
  case 48: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.498652, 0.045415, 0.033450, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 48
  case 49: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.461969, 0.045415, 0.033450, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 49
  case 50: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486132, 0.037348, 0.029287, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 50
  case 51: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.507574, 0.052731, 0.029287, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 51
  case 52: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.507574, 0.052731, 0.029287, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 52
  case 53: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.371913, 0.045770, 0.023926, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 53
  case 54: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.366441, 0.044709, 0.033294, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 54
  case 55: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.475944, 0.043055, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 55
  case 56: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480748, 0.044709, 0.033294, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 56
  case 57: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.501559, 0.067237, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 57
  case 58: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.425442, 0.037320, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 58
  case 59: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.475944, 0.059975, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 59
  case 60: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.483749, 0.058542, 0.029382, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 60
  case 61: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474777, 0.049670, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 61
  case 62: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474777, 0.049670, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 62
  case 63: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488164, 0.049670, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 63
  case 64: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.501529, 0.065169, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 64
  case 66: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.483108, 0.038340, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 66
  case 67: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.483108, 0.038340, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 67
  case 68: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.472559, 0.062585, 0.031010, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 68
  case 69: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480141, 0.049087, 0.033945, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 69
  case 70: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480141, 0.046438, 0.037162, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 70
  case 71: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480141, 0.044445, 0.037162, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 71
  case 73: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480141, 0.052731, 0.037162, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 73
  case 75: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495800, 0.064034, 0.033945, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 75
  case 76: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.482457, 0.047576, 0.038213, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 76
  case 77: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.393399, 0.047576, 0.034152, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 77
  case 78: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.524892, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 78
  case 79: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.479710, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 79
  case 80: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.504613, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 80
  case 81: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.504613, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 81
  case 82: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488299, 0.052069, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 82
  case 83: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488299, 0.041835, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 83
  case 84: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488299, 0.041835, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 84
  case 85: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.517124, 0.062655, 0.035193, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 85
  case 87: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484702, 0.069927, 0.034297, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 87
  case 88: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.381865, 0.044695, 0.032057, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 88
  case 89: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.385028, 0.026502, 0.028343, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 89
  case 90: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488628, 0.056313, 0.042152, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 90
  case 91: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.477238, 0.048436, 0.042152, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 91
  case 92: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484922, 0.044621, 0.030932, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 92
  case 93: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484922, 0.044621, 0.030932, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 93
  case 94: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.394960, 0.031984, 0.030932, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 94
  case 95: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.427377, 0.027987, 0.031799, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 95
  case 96: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.516738, 0.054109, 0.029650, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 96
  case 97: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.457793, 0.047358, 0.034751, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 97
  case 98: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474203, 0.064047, 0.034751, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 98
  case 99: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.511697, 0.053627, 0.034751, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 99
  case 100: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.491432, 0.053627, 0.040903, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 100
  case 101: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.513469, 0.053627, 0.034802, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 101
  case 102: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486972, 0.059822, 0.031711, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 102
  case 103: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.520674, 0.059822, 0.031711, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 103
  case 104: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489225, 0.052609, 0.035853, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 104
  case 105: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.519800, 0.052609, 0.032823, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 105
  case 106: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484007, 0.052609, 0.032823, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 106
  case 107: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485734, 0.054785, 0.034904, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 107
  case 108: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.502007, 0.054785, 0.031028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 108
  case 109: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488848, 0.053621, 0.031028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 109
  case 110: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488848, 0.057682, 0.031028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 110
  case 111: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.497134, 0.052560, 0.032028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 111
  case 112: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.437568, 0.044190, 0.024814, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 112
  case 113: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486990, 0.054652, 0.038871, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 113
  case 114: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486990, 0.060804, 0.029503, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 114
  case 115: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486990, 0.056284, 0.033900, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 115
  case 116: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.478787, 0.056284, 0.033900, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 117    116?
  case 117: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.435513, 0.045768, 0.034714, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 117
  case 119: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.435513, 0.045768, 0.034714, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 119
  case 120: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486136, 0.050808, 0.034714, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 120
  case 121: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495671, 0.052921, 0.036724, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 121
  case 122: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495671, 0.052921, 0.029613, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 122
  case 123: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486041, 0.060444, 0.035110, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 123
  case 124: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480657, 0.054292, 0.034721, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 124
  case 125: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480699, 0.048222, 0.030952, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 125
  case 126: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.496136, 0.052148, 0.038165, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 126
  case 127: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.480711, 0.059362, 0.042349, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 127
  case 128: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.476693, 0.065462, 0.033212, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 128
  case 129: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.370846, 0.038625, 0.025369, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 129
  case 130: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.384656, 0.038625, 0.033300, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 130
  case 131: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.494632, 0.064126, 0.035578, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 131
  case 132: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.415663, 0.036475, 0.035578, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 132
  case 133: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470698, 0.049545, 0.031518, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 133
  case 134: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.404390, 0.030418, 0.031518, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 134
  case 135: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486369, 0.055371, 0.033630, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 135
  case 136: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.403795, 0.025090, 0.031350, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 136
  case 137: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.422532, 0.017877, 0.031350, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 137
  case 138: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485461, 0.045619, 0.031350, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 138
  case 139: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485461, 0.045619, 0.041812, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 139
  case 140: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485461, 0.057944, 0.041812, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 140
  case 141: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474471, 0.045650, 0.037752, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 141
  case 142: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.499258, 0.057089, 0.032496, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 142
  case 143: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.483897, 0.057089, 0.032496, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 143
  case 144: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.500313, 0.055030, 0.026344, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 144
  case 145: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.493014, 0.045806, 0.037332, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 145
  case 146: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506443, 0.051221, 0.033272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 146
  case 147: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506443, 0.047160, 0.040485, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 147
  case 148: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.475639, 0.047160, 0.035105, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 148
  case 150: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.478669, 0.047160, 0.039373, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 150
  case 151: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.469203, 0.054430, 0.034272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 151
  case 152: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.469203, 0.044443, 0.034272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 152
  case 153: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.498365, 0.044443, 0.035272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 153
  case 154: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.458358, 0.044162, 0.034965, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 154
  case 155: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.455225, 0.044443, 0.034965, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 155
  case 156: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481597, 0.055729, 0.034965, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 156
  case 157: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489041, 0.057971, 0.035735, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 157
  case 158: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489041, 0.050965, 0.040836, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 158
  case 159: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484565, 0.057096, 0.036182, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 159
  case 160: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.450995, 0.032149, 0.026813, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 160
  case 161: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484311, 0.053632, 0.039496, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 161
  case 162: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.491402, 0.053632, 0.036204, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 162
  case 163: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506502, 0.055294, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 163
  case 164: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506502, 0.048449, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 164
  case 165: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.417092, 0.049858, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 165
  case 166: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.417092, 0.049858, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 166
  case 167: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487683, 0.049858, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 167
  case 168: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.477365, 0.049858, 0.026390, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 168
  case 169: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481425, 0.041779, 0.034849, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 169
  case 170: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481425, 0.058393, 0.028475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 170
  case 171: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481425, 0.050293, 0.036761, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 171
  case 172: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481425, 0.048513, 0.037119, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 172
  case 173: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488857, 0.062322, 0.033983, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 173
  case 174: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488857, 0.057282, 0.033983, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 174
  case 175: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488857, 0.057282, 0.033983, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 175
  case 176: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488857, 0.057282, 0.024759, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 176
  case 177: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488857, 0.057282, 0.024759, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 177
  case 178: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.490625, 0.051130, 0.037583, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 178
  case 179: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487377, 0.053140, 0.027589, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 179
  case 180: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487377, 0.053140, 0.023912, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 180
  case 181: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487377, 0.053140, 0.023912, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 181
  case 182: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.461986, 0.040948, 0.033280, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 182
  case 183: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.472127, 0.040948, 0.033280, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 183
  case 184: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488741, 0.063235, 0.023078, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 184
  case 185: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488741, 0.041770, 0.039499, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 185
  case 186: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.478631, 0.048983, 0.037196, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 186
  case 187: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.504893, 0.040698, 0.046378, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 187
  case 188: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.493123, 0.052825, 0.034475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 188
  case 189: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.482915, 0.052825, 0.034475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 189
  case 190: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.499487, 0.052825, 0.034475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 190
  case 191: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506408, 0.052825, 0.037388, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 191
  case 192: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506408, 0.052825, 0.037388, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 192
  case 193: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506408, 0.052825, 0.035654, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 193
  case 194: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.506408, 0.052825, 0.037447, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 194
  case 195: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.512633, 0.049476, 0.032615, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 195
  case 196: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.377072, 0.037533, 0.026463, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 196
  case 197: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.398839, 0.036482, 0.026463, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 197
  case 198: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.494765, 0.057264, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 198
  case 199: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.386138, 0.039158, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 199
  case 200: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.507551, 0.049621, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 200
  case 201: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495757, 0.056979, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 201
  case 202: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.472661, 0.049998, 0.028575, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 202
  case 203: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487608, 0.064333, 0.028575, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 203
  case 204: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487608, 0.059231, 0.028575, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 204
  case 205: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487608, 0.052141, 0.042004, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 205
  case 206: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470769, 0.052141, 0.029209, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 206
  case 207: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473758, 0.047120, 0.035526, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 207
  case 209: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.420530, 0.032689, 0.027240, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 209
  case 210: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.403049, 0.032689, 0.032341, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 210
  case 211: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.485224, 0.047745, 0.035371, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 211
  case 212: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.394802, 0.044400, 0.027179, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 212
  case 213: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484012, 0.053022, 0.033299, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 213
  case 214: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.476887, 0.047921, 0.035902, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 214
  case 215: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.468350, 0.047921, 0.038394, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 215
  case 216: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.479501, 0.047921, 0.038394, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 216
  case 217: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495022, 0.047921, 0.029120, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 217
  case 218: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.380311, 0.042933, 0.029120, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 218
  case 219: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.467397, 0.042933, 0.036294, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 219
  case 220: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.462776, 0.061754, 0.044622, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 220
  case 221: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489326, 0.055391, 0.034892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 221
  case 222: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489326, 0.058421, 0.036273, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 222
  case 223: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.477118, 0.071921, 0.039574, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 223
  case 224: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470047, 0.047042, 0.035006, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 224
  case 225: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470047, 0.047042, 0.035006, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 225
  case 226: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.478536, 0.047042, 0.036974, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 226
  case 227: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.494057, 0.067845, 0.031934, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 227
  case 228: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486657, 0.080244, 0.038588, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 228
  case 229: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.425390, 0.040964, 0.032876, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 229
  case 230: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.445428, 0.042720, 0.034939, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 230
  case 231: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.372797, 0.043825, 0.028656, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 231
  case 232: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.386538, 0.014045, 0.028656, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 232
  case 233: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.483468, 0.049352, 0.035649, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 233
  case 234: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.421060, 0.015586, 0.027633, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 234
  case 235: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.404351, 0.025830, 0.035139, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 235
  case 236: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.427529, 0.040777, 0.035139, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 236
  case 237: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.469063, 0.040777, 0.034382, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 237
  case 238: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.497537, 0.058097, 0.039174, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 238
  case 239: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.524220, 0.042505, 0.037174, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 239
  case 240: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.473191, 0.057818, 0.035164, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 240
  case 241: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.481825, 0.063372, 0.039404, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 241
  case 242: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470107, 0.040658, 0.039404, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 242
  case 243: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470107, 0.049820, 0.039404, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 243
  case 244: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.470107, 0.066434, 0.040173, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 244
  case 245: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.475735, 0.057961, 0.036864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 245
  case 246: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.484896, 0.063249, 0.036864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 246
  case 247: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495140, 0.056159, 0.036864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 247
  case 248: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.486518, 0.063404, 0.038851, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 248
  case 249: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.478119, 0.053632, 0.047740, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 249
  case 250: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487488, 0.059784, 0.032241, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 250
  case 251: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487488, 0.054744, 0.035391, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 251
  case 252: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.497836, 0.054744, 0.036107, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 252
  case 253: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.424969, 0.035129, 0.030383, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 253
  case 254: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.498688, 0.055503, 0.037815, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 254
  case 255: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.498688, 0.040602, 0.037815, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 255
  case 256: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489527, 0.056101, 0.037815, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 256
  case 257: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.503907, 0.070578, 0.036147, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 257
  case 258: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.496130, 0.060534, 0.036147, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 258
  case 259: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.496130, 0.060534, 0.037388, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 259
  case 260: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.490328, 0.051230, 0.035165, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 260
  case 261: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.401477, 0.030430, 0.029013, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 261
  case 262: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.455521, 0.050570, 0.033281, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 262
  case 263: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.479342, 0.050570, 0.037586, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 263
  case 264: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.479342, 0.066910, 0.037586, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 264
  case 265: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.475841, 0.019462, 0.031311, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 265
  case 266: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.449961, 0.036117, 0.031311, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 266
  case 267: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.672227, 0.050490, -0.016834, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 267
  case 269: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.493552, 0.035639, 0.037131, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 269
  case 270: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.510810, 0.055933, 0.037131, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 270
  case 271: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.510810, 0.044261, 0.027450, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 271
  case 272: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.488742, 0.055372, 0.034548, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 272
  case 274: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.471382, 0.040425, 0.031892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 274
  case 275: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.471382, 0.040425, 0.031892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 275
  case 276: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.472203, 0.040425, 0.031892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 276
  case 277: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.515627, 0.060499, 0.041897, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 277
  case 278: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.515627, 0.060499, 0.041897, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 278
  case 279: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.515627, 0.060499, 0.041897, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 279
  case 280: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 280
  case 281: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 281
  case 282: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 282
  case 283: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 283
  case 284: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 284
  case 285: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.494541, 0.057693, 0.038969, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 285
  case 286: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.471158, 0.040950, 0.034909, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 286
  case 287: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.490773, 0.055387, 0.045121, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 287
  case 288: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.476259, 0.043820, 0.033743, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 288
  case 290: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.471478, 0.043820, 0.033743, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 290
  case 291: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487084, 0.045016, 0.033743, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 291
  case 292: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487084, 0.038864, 0.027591, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 292
  case 293: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.495370, 0.053386, 0.027591, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 293
  case 294: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.500573, 0.039449, 0.034681, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 294
  case 295: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.425444, 0.044973, 0.034681, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 295
  case 296: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.457822, 0.044973, 0.039864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 296
  case 297: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.487348, 0.050074, 0.046016, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 297
  case 298: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.489450, 0.039604, 0.033712, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 298
  case 299: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.518868, 0.061255, 0.034489, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 299
  default: SetPlayerAttachedObject( playerid, 0, 19083, 1, 0.518868, 0.061255, 0.034489, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 299
 }
}

zaloz_laser2(playerid) {  // dopasowane przez timesa
 switch(GetPlayerSkin(playerid)){
  case 0: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.492748, 0.040626, 0.034564, 0.000000, 0.000000, 94.857414, 1.000000, 1.000000, 1.000000 ); // LaserPointer1 - skin 0 laser w oku
  case 1: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474780, 0.045132, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 1
  case 2: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.450285, 0.045132, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 2
  case 7: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.469900, 0.053491, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 7
  case 8: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.469900, 0.053491, 0.030871, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 8
  case 9: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.444506, 0.038463, 0.038085, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 9
  case 10: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.352782, 0.032301, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 10
  case 11: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474426, 0.042763, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 11
  case 12: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.464383, 0.043365, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 12
  case 13: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.512461, 0.043365, 0.034035, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 13
  case 14: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.477542, 0.070707, 0.041043, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 14
  case 15: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.461280, 0.034678, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 15
  case 16: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.511655, 0.059833, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 16
  case 17: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.462779, 0.045729, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 17
  case 18: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.449220, 0.035496, 0.033756, 0.000000, 0.000000, 84.005485, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 18
  case 19: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.528088, 0.045958, 0.033756, 0.000000, 0.000000, 102.224014, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 19
  case 20: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485164, 0.044673, 0.029965, 0.000000, 0.000000, 90.414581, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 20
  case 21: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485164, 0.059621, 0.029965, 0.000000, 0.000000, 90.414581, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 21
  case 22: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485164, 0.059621, 0.029965, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 22
  case 23: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485164, 0.059621, 0.029965, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 23
  case 24: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485164, 0.044223, 0.029965, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 24
  case 25: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.479192, 0.051794, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 25
  case 26: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.500002, 0.058987, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 26
  case 27: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.492902, 0.058987, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 27
  case 28: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.492902, 0.058987, 0.034962, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 28
  case 29: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481123, 0.067273, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 29
  case 30: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481123, 0.064109, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 30
  case 31: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.391128, 0.054122, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 31
  case 32: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.459490, 0.018460, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 32
  case 33: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.426369, 0.040455, 0.038594, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 33
  case 34: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473469, 0.040455, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 34
  case 35: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473469, 0.034219, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 35
  case 36: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473469, 0.034219, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 36
  case 37: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473469, 0.034219, 0.035564, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 -  skin 37
  case 38: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.365544, 0.034219, 0.024311, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 38
  case 39: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.365544, 0.034219, 0.033679, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 39
  case 40: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.476661, 0.038572, 0.033679, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 40
  case 41: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.515075, 0.055830, 0.033679, 0.000000, 0.000000, 92.750900, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 41
  case 43: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.405384, 0.047376, 0.042409, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 43
  case 44: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.419853, 0.047376, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 44
  case 45: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.453621, 0.034325, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 45
  case 46: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473560, 0.066463, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 46
  case 47: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473560, 0.066463, 0.035195, 0.000000, 0.000000, 88.319694, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 47
  case 48: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.498652, 0.045415, 0.033450, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 48
  case 49: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.461969, 0.045415, 0.033450, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 49
  case 50: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486132, 0.037348, 0.029287, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 50
  case 51: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.507574, 0.052731, 0.029287, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 51
  case 52: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.507574, 0.052731, 0.029287, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 52
  case 53: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.371913, 0.045770, 0.023926, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 53
  case 54: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.366441, 0.044709, 0.033294, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 54
  case 55: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.475944, 0.043055, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 55
  case 56: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480748, 0.044709, 0.033294, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 56
  case 57: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.501559, 0.067237, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 57
  case 58: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.425442, 0.037320, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 58
  case 59: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.475944, 0.059975, 0.034356, 0.000000, 0.000000, 92.575027, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 59
  case 60: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.483749, 0.058542, 0.029382, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 60
  case 61: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474777, 0.049670, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 61
  case 62: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474777, 0.049670, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 62
  case 63: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488164, 0.049670, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 63
  case 64: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.501529, 0.065169, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 64
  case 66: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.483108, 0.038340, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 66
  case 67: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.483108, 0.038340, 0.039845, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 67
  case 68: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.472559, 0.062585, 0.031010, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 68
  case 69: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480141, 0.049087, 0.033945, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 69
  case 70: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480141, 0.046438, 0.037162, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 70
  case 71: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480141, 0.044445, 0.037162, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 71
  case 73: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480141, 0.052731, 0.037162, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 73
  case 75: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495800, 0.064034, 0.033945, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 75
  case 76: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.482457, 0.047576, 0.038213, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 76
  case 77: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.393399, 0.047576, 0.034152, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 77
  case 78: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.524892, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 78
  case 79: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.479710, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 79
  case 80: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.504613, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 80
  case 81: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.504613, 0.038446, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 81
  case 82: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488299, 0.052069, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 82
  case 83: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488299, 0.041835, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 83
  case 84: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488299, 0.041835, 0.039253, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 84
  case 85: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.517124, 0.062655, 0.035193, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 85
  case 87: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484702, 0.069927, 0.034297, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 87
  case 88: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.381865, 0.044695, 0.032057, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 88
  case 89: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.385028, 0.026502, 0.028343, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 89
  case 90: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488628, 0.056313, 0.042152, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 90
  case 91: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.477238, 0.048436, 0.042152, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 91
  case 92: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484922, 0.044621, 0.030932, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 92
  case 93: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484922, 0.044621, 0.030932, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 93
  case 94: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.394960, 0.031984, 0.030932, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 94
  case 95: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.427377, 0.027987, 0.031799, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 95
  case 96: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.516738, 0.054109, 0.029650, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 96
  case 97: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.457793, 0.047358, 0.034751, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 97
  case 98: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474203, 0.064047, 0.034751, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 98
  case 99: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.511697, 0.053627, 0.034751, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 99
  case 100: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.491432, 0.053627, 0.040903, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 100
  case 101: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.513469, 0.053627, 0.034802, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 101
  case 102: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486972, 0.059822, 0.031711, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 102
  case 103: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.520674, 0.059822, 0.031711, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 103
  case 104: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489225, 0.052609, 0.035853, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 104
  case 105: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.519800, 0.052609, 0.032823, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 105
  case 106: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484007, 0.052609, 0.032823, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 106
  case 107: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485734, 0.054785, 0.034904, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 107
  case 108: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.502007, 0.054785, 0.031028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 108
  case 109: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488848, 0.053621, 0.031028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 109
  case 110: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488848, 0.057682, 0.031028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 110
  case 111: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.497134, 0.052560, 0.032028, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 111
  case 112: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.437568, 0.044190, 0.024814, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 112
  case 113: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486990, 0.054652, 0.038871, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 113
  case 114: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486990, 0.060804, 0.029503, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 114
  case 115: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486990, 0.056284, 0.033900, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 115
  case 116: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.478787, 0.056284, 0.033900, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 117    116?
  case 117: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.435513, 0.045768, 0.034714, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 117
  case 119: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.435513, 0.045768, 0.034714, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 119
  case 120: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486136, 0.050808, 0.034714, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 120
  case 121: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495671, 0.052921, 0.036724, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 121
  case 122: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495671, 0.052921, 0.029613, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 122
  case 123: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486041, 0.060444, 0.035110, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 123
  case 124: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480657, 0.054292, 0.034721, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 124
  case 125: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480699, 0.048222, 0.030952, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 125
  case 126: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.496136, 0.052148, 0.038165, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 126
  case 127: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.480711, 0.059362, 0.042349, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 127
  case 128: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.476693, 0.065462, 0.033212, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 128
  case 129: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.370846, 0.038625, 0.025369, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 129
  case 130: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.384656, 0.038625, 0.033300, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 130
  case 131: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.494632, 0.064126, 0.035578, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 131
  case 132: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.415663, 0.036475, 0.035578, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 132
  case 133: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470698, 0.049545, 0.031518, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 133
  case 134: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.404390, 0.030418, 0.031518, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 134
  case 135: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486369, 0.055371, 0.033630, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 135
  case 136: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.403795, 0.025090, 0.031350, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 136
  case 137: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.422532, 0.017877, 0.031350, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 137
  case 138: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485461, 0.045619, 0.031350, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 138
  case 139: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485461, 0.045619, 0.041812, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 139
  case 140: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485461, 0.057944, 0.041812, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 140
  case 141: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474471, 0.045650, 0.037752, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 141
  case 142: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.499258, 0.057089, 0.032496, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 142
  case 143: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.483897, 0.057089, 0.032496, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 143
  case 144: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.500313, 0.055030, 0.026344, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 144
  case 145: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.493014, 0.045806, 0.037332, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 145
  case 146: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506443, 0.051221, 0.033272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 146
  case 147: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506443, 0.047160, 0.040485, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 147
  case 148: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.475639, 0.047160, 0.035105, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 148
  case 150: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.478669, 0.047160, 0.039373, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 150
  case 151: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.469203, 0.054430, 0.034272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 151
  case 152: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.469203, 0.044443, 0.034272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 152
  case 153: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.498365, 0.044443, 0.035272, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 153
  case 154: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.458358, 0.044162, 0.034965, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 154
  case 155: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.455225, 0.044443, 0.034965, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 155
  case 156: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481597, 0.055729, 0.034965, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 156
  case 157: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489041, 0.057971, 0.035735, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 157
  case 158: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489041, 0.050965, 0.040836, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 158
  case 159: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484565, 0.057096, 0.036182, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 159
  case 160: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.450995, 0.032149, 0.026813, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 160
  case 161: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484311, 0.053632, 0.039496, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 161
  case 162: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.491402, 0.053632, 0.036204, 0.000000, 0.000000, 92.520606, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 162
  case 163: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506502, 0.055294, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 163
  case 164: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506502, 0.048449, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 164
  case 165: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.417092, 0.049858, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 165
  case 166: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.417092, 0.049858, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 166
  case 167: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487683, 0.049858, 0.035348, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 167
  case 168: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.477365, 0.049858, 0.026390, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 168
  case 169: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481425, 0.041779, 0.034849, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 169
  case 170: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481425, 0.058393, 0.028475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 170
  case 171: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481425, 0.050293, 0.036761, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 171
  case 172: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481425, 0.048513, 0.037119, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 172
  case 173: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488857, 0.062322, 0.033983, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 173
  case 174: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488857, 0.057282, 0.033983, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 174
  case 175: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488857, 0.057282, 0.033983, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 175
  case 176: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488857, 0.057282, 0.024759, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 176
  case 177: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488857, 0.057282, 0.024759, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 177
  case 178: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.490625, 0.051130, 0.037583, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 178
  case 179: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487377, 0.053140, 0.027589, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 179
  case 180: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487377, 0.053140, 0.023912, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 180
  case 181: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487377, 0.053140, 0.023912, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 181
  case 182: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.461986, 0.040948, 0.033280, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 182
  case 183: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.472127, 0.040948, 0.033280, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 183
  case 184: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488741, 0.063235, 0.023078, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 184
  case 185: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488741, 0.041770, 0.039499, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 185
  case 186: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.478631, 0.048983, 0.037196, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 186
  case 187: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.504893, 0.040698, 0.046378, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 187
  case 188: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.493123, 0.052825, 0.034475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 188
  case 189: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.482915, 0.052825, 0.034475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 189
  case 190: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.499487, 0.052825, 0.034475, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 190
  case 191: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506408, 0.052825, 0.037388, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 191
  case 192: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506408, 0.052825, 0.037388, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 192
  case 193: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506408, 0.052825, 0.035654, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 193
  case 194: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.506408, 0.052825, 0.037447, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 194
  case 195: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.512633, 0.049476, 0.032615, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 195
  case 196: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.377072, 0.037533, 0.026463, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 196
  case 197: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.398839, 0.036482, 0.026463, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 197
  case 198: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.494765, 0.057264, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 198
  case 199: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.386138, 0.039158, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 199
  case 200: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.507551, 0.049621, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 200
  case 201: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495757, 0.056979, 0.033676, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 201
  case 202: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.472661, 0.049998, 0.028575, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 202
  case 203: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487608, 0.064333, 0.028575, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 203
  case 204: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487608, 0.059231, 0.028575, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 204
  case 205: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487608, 0.052141, 0.042004, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 205
  case 206: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470769, 0.052141, 0.029209, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 206
  case 207: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473758, 0.047120, 0.035526, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 207
  case 209: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.420530, 0.032689, 0.027240, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 209
  case 210: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.403049, 0.032689, 0.032341, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 210
  case 211: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.485224, 0.047745, 0.035371, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 211
  case 212: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.394802, 0.044400, 0.027179, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 212
  case 213: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484012, 0.053022, 0.033299, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 213
  case 214: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.476887, 0.047921, 0.035902, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 214
  case 215: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.468350, 0.047921, 0.038394, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 215
  case 216: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.479501, 0.047921, 0.038394, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 216
  case 217: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495022, 0.047921, 0.029120, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 217
  case 218: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.380311, 0.042933, 0.029120, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 218
  case 219: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.467397, 0.042933, 0.036294, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 219
  case 220: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.462776, 0.061754, 0.044622, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 220
  case 221: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489326, 0.055391, 0.034892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 221
  case 222: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489326, 0.058421, 0.036273, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 222
  case 223: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.477118, 0.071921, 0.039574, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 223
  case 224: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470047, 0.047042, 0.035006, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 224
  case 225: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470047, 0.047042, 0.035006, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 225
  case 226: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.478536, 0.047042, 0.036974, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 226
  case 227: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.494057, 0.067845, 0.031934, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 227
  case 228: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486657, 0.080244, 0.038588, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 228
  case 229: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.425390, 0.040964, 0.032876, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 229
  case 230: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.445428, 0.042720, 0.034939, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 230
  case 231: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.372797, 0.043825, 0.028656, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 231
  case 232: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.386538, 0.014045, 0.028656, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 232
  case 233: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.483468, 0.049352, 0.035649, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 233
  case 234: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.421060, 0.015586, 0.027633, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 234
  case 235: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.404351, 0.025830, 0.035139, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 235
  case 236: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.427529, 0.040777, 0.035139, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 236
  case 237: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.469063, 0.040777, 0.034382, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 237
  case 238: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.497537, 0.058097, 0.039174, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 238
  case 239: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.524220, 0.042505, 0.037174, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 239
  case 240: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.473191, 0.057818, 0.035164, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 240
  case 241: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.481825, 0.063372, 0.039404, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 241
  case 242: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470107, 0.040658, 0.039404, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 242
  case 243: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470107, 0.049820, 0.039404, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 243
  case 244: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.470107, 0.066434, 0.040173, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 244
  case 245: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.475735, 0.057961, 0.036864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 245
  case 246: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.484896, 0.063249, 0.036864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 246
  case 247: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495140, 0.056159, 0.036864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 247
  case 248: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.486518, 0.063404, 0.038851, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 248
  case 249: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.478119, 0.053632, 0.047740, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 249
  case 250: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487488, 0.059784, 0.032241, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 250
  case 251: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487488, 0.054744, 0.035391, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 251
  case 252: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.497836, 0.054744, 0.036107, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 252
  case 253: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.424969, 0.035129, 0.030383, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 253
  case 254: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.498688, 0.055503, 0.037815, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 254
  case 255: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.498688, 0.040602, 0.037815, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 255
  case 256: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489527, 0.056101, 0.037815, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 256
  case 257: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.503907, 0.070578, 0.036147, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 257
  case 258: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.496130, 0.060534, 0.036147, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 258
  case 259: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.496130, 0.060534, 0.037388, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 259
  case 260: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.490328, 0.051230, 0.035165, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 260
  case 261: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.401477, 0.030430, 0.029013, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 261
  case 262: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.455521, 0.050570, 0.033281, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 262
  case 263: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.479342, 0.050570, 0.037586, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 263
  case 264: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.479342, 0.066910, 0.037586, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 264
  case 265: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.475841, 0.019462, 0.031311, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 265
  case 266: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.449961, 0.036117, 0.031311, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 266
  case 267: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.672227, 0.050490, -0.016834, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 267
  case 269: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.493552, 0.035639, 0.037131, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 269
  case 270: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.510810, 0.055933, 0.037131, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 270
  case 271: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.510810, 0.044261, 0.027450, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 271
  case 272: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.488742, 0.055372, 0.034548, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 272
  case 274: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.471382, 0.040425, 0.031892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 274
  case 275: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.471382, 0.040425, 0.031892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 275
  case 276: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.472203, 0.040425, 0.031892, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 276
  case 277: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.515627, 0.060499, 0.041897, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 277
  case 278: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.515627, 0.060499, 0.041897, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 278
  case 279: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.515627, 0.060499, 0.041897, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 279
  case 280: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 280
  case 281: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 281
  case 282: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 282
  case 283: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 283
  case 284: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.474926, 0.043772, 0.033766, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 284
  case 285: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.494541, 0.057693, 0.038969, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 285
  case 286: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.471158, 0.040950, 0.034909, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 286
  case 287: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.490773, 0.055387, 0.045121, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 287
  case 288: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.476259, 0.043820, 0.033743, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 288
  case 290: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.471478, 0.043820, 0.033743, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 290
  case 291: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487084, 0.045016, 0.033743, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 291
  case 292: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487084, 0.038864, 0.027591, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 292
  case 293: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.495370, 0.053386, 0.027591, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 293
  case 294: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.500573, 0.039449, 0.034681, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 294
  case 295: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.425444, 0.044973, 0.034681, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 295
  case 296: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.457822, 0.044973, 0.039864, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 296
  case 297: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.487348, 0.050074, 0.046016, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 297
  case 298: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.489450, 0.039604, 0.033712, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 298
  case 299: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.518868, 0.061255, 0.034489, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 299
  default: SetPlayerAttachedObject( playerid, 0, 18643, 1, 0.518868, 0.061255, 0.034489, 0.000000, 0.000000, 92.186920, 1.000000, 1.000000, 1.000000 ); // LaserPointer5 - skin 299
 }
}
// EOF
