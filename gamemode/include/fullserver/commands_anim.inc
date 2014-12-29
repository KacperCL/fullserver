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

stock LoopingAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
  if (IsPlayerInAnyVehicle(playerid)) {
    Msg(playerid,COLOR_ERROR, "Nie mozna uzywac animacji w pojezdzie.", false);
    return 0;
  }
  if (pData[playerid][pAttraction]==A_DERBY || pData[playerid][pAttraction]==A_RACE || pData[playerid][pAttraction]==A_CHOWANY   || pData[playerid][pAttraction]==A_WG || pData[playerid][pAttraction]==A_STRZELNICA || pTemp[playerid][isFreezed])  {
    Msg(playerid, COLOR_ERROR, "Nie mozesz tego teraz zrobic...", false);
    return 0;
  }

    pTemp[playerid][performingAnim] = true;
  ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp, 1);
  return 1;
}

stock OnePlayAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
  if (IsPlayerInAnyVehicle(playerid)) {
    Msg(playerid,COLOR_ERROR, "Nie mozna uzywac animacji w pojezdzie.", false);
    return 0;
  }
  if (pData[playerid][pAttraction]==A_DERBY || pData[playerid][pAttraction]==A_RACE || pData[playerid][pAttraction]==A_CHOWANY   || pData[playerid][pAttraction]==A_WG || pData[playerid][pAttraction]==A_STRZELNICA || pTemp[playerid][isFreezed]) {
    Msg(playerid, COLOR_ERROR, "Nie mozesz tego teraz zrobic...", false);
    return 0;
  }
    pTemp[playerid][performingAnim] = true;
  ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp, 1);
  return 1;
}

stock StopLoopingAnim(playerid)
{
    pTemp[playerid][performingAnim] = false;
  ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
  ClearAnimations(playerid);
}

CMD:anims(playerid){
  new string[800];
  strcat(string,"/Rece     /Rece2    /Rece3    /Rece4 \n");
  strcat(string,"/Rece5    /Rece6    /Bar2     /Bar3 \n");
  strcat(string,"/Szafka   /Zegarek  /Lez      /Hide\n");
  strcat(string,"/Rzygaj   /Grubas   /Grubas2  /Taichi\n");
  strcat(string,"/Siadaj   /Chat     /Ratunku  /Kopniak\n");
  strcat(string,"/Dance    /Fucku    /Cellin   /Cellout\n");
  strcat(string,"/Pij      /Smoke    /Fsmoke   /Krzeslo\n");
  strcat(string,"/Krzeslo2 /Calus    /Trup     /Trup2\n");
  strcat(string,"/Wankin   /Wankout  /Deal     /Boks\n");
  strcat(string,"/Aresztuj /Opalaj   /Aresztowany /Inbedright\n");
  strcat(string,"/Opalaj2  /Opalaj3  /Turlaj   /Klaps\n");
  strcat(string,"/Swat3    /Piwo     /Drunk    /Swat\n");
  strcat(string,"/Lookout  /Napad    /Papieros /Cpun\n");
  strcat(string,"/Cpun2    /Cpun3    /Cpun4    /Cpun5\n");
  strcat(string,"/Jedz     /Jedz2    /Inbedleft /Rap\n");
  strcat(string,"/Jedz3    /Wino     /Taniec   /Taniec2\n");
  strcat(string,"/Taniec3  /Taniec4  /Taniec5  /Taniec6\n");
  strcat(string,"/Taniec7  /Rolki    /Sprunk   /Swat2\n");

  ShowPlayerDialog(playerid,DIALOG_HELP__ANIMATIONS,0,"CMD> Animacje",string,"OK","OK");
  return 1;
}

CMD:klaps(playerid){
  if(LoopingAnim(playerid,"SWEET","sweet_ass_slap",4.0,0,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Dajesz klapsa komus");
  return 1;
}

CMD:bar2(playerid){
  if (LoopingAnim(playerid, "BAR", "Barserve_bottle", 4.000000, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Podajesz zimne piwko");
  return 1;
}

CMD:bar3(playerid){
  if (LoopingAnim(playerid, "BAR", "Barserve_give", 4.000000, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Podajesz zimne piwko");
  return 1;
}

CMD:opalaj(playerid){
  if (LoopingAnim(playerid, "BEACH", "ParkSit_W_loop", 4.000000, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Opalasz sie");
  return 1;

}

CMD:opalaj2(playerid){
  if (LoopingAnim(playerid, "BEACH", "SitnWait_loop_W", 4.000000, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Opalasz sie");
  return 1;
}

CMD:rozmowaauto(playerid){
  if (LoopingAnim(playerid, "CAR_CHAT", "car_talkm_loop", 4.000000, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Rozmawiasz z kims w aucie");
  return 1;
}

CMD:szafka(playerid){
  if (LoopingAnim(playerid, "COP_AMBIENT", "Copbrowse_nod", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Bierzesz cos z szafki");
  return 1;
}

CMD:zegarek(playerid){
  if (LoopingAnim(playerid, "COP_AMBIENT", "Coplook_nod", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Patrzysz na zegarek");
  return 1;
}

CMD:cpun2(playerid){
  if (LoopingAnim(playerid, "CRACK", "crckdeth1", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
  return 1;
}

CMD:cpun3(playerid){
  if (LoopingAnim(playerid, "CRACK", "crckdeth3", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
  return 1;
}

CMD:cpun4(playerid){
  if (LoopingAnim(playerid, "CRACK", "crckdeth4", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
  return 1;
}

CMD:cpun5(playerid){
  if (LoopingAnim(playerid, "CRACK", "crckidle4", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
  return 1;

}

CMD:rolki(playerid){
  if (LoopingAnim(playerid, "SKATE", "skate_sprint", 4.0999, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Jedziesz na rolkach");
  return 1;

}

CMD:taniec(playerid){
  if (LoopingAnim(playerid, "DANCING", "bd_clap", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:taniec2(playerid){
  if (LoopingAnim(playerid, "DANCING", "bd_clap1", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:taniec3(playerid){
  if (LoopingAnim(playerid, "DANCING", "DAN_Down_A", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:taniec4(playerid){
  if (LoopingAnim(playerid, "DANCING", "DAN_Left_A", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:taniec5(playerid){
  if (LoopingAnim(playerid, "DANCING", "DAN_Right_A", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:taniec6(playerid){
  if (LoopingAnim(playerid, "DANCING", "DAN_Up_A", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:taniec7(playerid){
  if (LoopingAnim(playerid, "DANCING", "dnce_M_a", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Tanczysz");
  return 1;

}

CMD:grubas(playerid){
  if (LoopingAnim(playerid, "FAT", "FatWalk", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Idziesz jak grubas");
  return 1;

}

CMD:grubas2(playerid){
  if (LoopingAnim(playerid, "FAT", "FatRun", 4.000000, 1, 1, 1, 1, 1))
    Msg(playerid, COLOR_INFO2, "Biegniesz jak grubas");
  return 1;

}

CMD:jedz2(playerid){
  if (LoopingAnim(playerid, "FOOD", "FF_Die_Bkw", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Jesz cos");
  return 1;

}

CMD:jedz3(playerid){
  if (LoopingAnim(playerid, "FOOD", "FF_Sit_Eat1", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Jesz cos");
  return 1;

}

CMD:rece4(playerid){
  if (LoopingAnim(playerid, "GANGS", "hndshkfa", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Wymachujesz Rekami");
  return 1;

}

CMD:rece5(playerid){
  if (LoopingAnim(playerid, "GHANDS", "gsign1", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Wymachyjesz Rekami");
  return 1;

}

CMD:rece6(playerid){
  if (LoopingAnim(playerid, "GHANDS", "gsign1LH", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Wymachyjesz Rekami");
  return 1;

}

CMD:krzeslo2(playerid){
  if (LoopingAnim(playerid, "JST_BUISNESS", "girl_02", 4.000000, 1, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Siadasz na krzesle jak kobieta");
  return 1;

}

CMD:turlaj(playerid){
  if (LoopingAnim(playerid, "MD_CHASE", "MD_BIKE_Lnd_Roll_F", 4.000000, 1, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Turlasz sie xD");
  return 1;

}

CMD:boks(playerid){
  if (LoopingAnim(playerid, "GYMNASIUM", "GYMshadowbox", 4.000000, 1, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Trenujesz boks");
  return 1;

}

CMD:kopniak(playerid){
  if (LoopingAnim(playerid, "POLICE", "Door_Kick", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Kopiesz :P");
  return 1;

}

CMD:opalaj3(playerid){
  if (LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_idleA", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Opalasz sie");
  return 1;

}

CMD:swat(playerid){
  if (LoopingAnim(playerid, "SWAT", "swt_breach_01", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Biegasz jak SWAT");
  return 1;

}

CMD:swat2(playerid){
  if (LoopingAnim(playerid, "SWAT", "swt_breach_02", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Biegasz jak SWAT");
  return 1;

}

CMD:swat3(playerid){
  if (LoopingAnim(playerid, "SWAT", "swt_breach_03", 4.000000, 0, 1, 1, 1, 0))
    Msg(playerid, COLOR_INFO2, "Biegasz jak SWAT");
  return 1;

}

CMD:trup2(playerid){
  if (LoopingAnim(playerid, "SWAT","gnstwall_injurd",4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Udajesz trupa");
  return 1;

}

CMD:ratunku(playerid){
  if (LoopingAnim(playerid,"BD_FIRE","BD_Panic_03",4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Machasz o pomoc!");
  return 1;

}

CMD:trup(playerid){
  if (LoopingAnim(playerid, "WUZI", "CS_Dead_Guy", 4.1,0,1,1,1,1))
    Msg(playerid, COLOR_INFO2, "Udajesz trupa");
  return 1;

}

CMD:calus(playerid){
  if (LoopingAnim(playerid, "KISSING", "Playa_Kiss_02", 4.0,0,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Calujesz kogos");
  return 1;

}

CMD:sikaj(playerid){
  SetPlayerSpecialAction(playerid,68);
  Msg(playerid, COLOR_INFO2, "Odlewasz sie");
  return 1;

}

CMD:krzeslo(playerid){
  if (LoopingAnim(playerid,"INT_OFFICE","OFF_Sit_Idle_Loop",4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Siadasz na krzesle");
  return 1;

}

CMD:rece(playerid){
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);
  Msg(playerid, COLOR_INFO2, "Poddajesz sie");
  return 1;

}

CMD:wankin(playerid){
  if (LoopingAnim(playerid, "PAULNMAC", "wank_loop", 4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Walisz konia");
  return 1;

}

CMD:wankout(playerid){
  if (LoopingAnim(playerid, "PAULNMAC", "wank_out", 4.0, 0, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Robisz wytrysk xD");
  return 1;

}

CMD:cellin(playerid){
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USECELLPHONE);
  Msg(playerid, COLOR_INFO2, "Dzwonisz do mamy");
  return 1;

}

CMD:piwo(playerid){
  SetPlayerDrunkLevel(playerid,10000);
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_BEER);
  SetPlayerFightingStyle(playerid, FIGHT_STYLE_ELBOW);
  Msg(playerid, COLOR_INFO2, "Pijesz Piwo!");

  return 1;

}

CMD:wino(playerid){
  SetPlayerDrunkLevel(playerid,1000000);
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_WINE);
  SetPlayerFightingStyle(playerid, FIGHT_STYLE_GRABKICK);
  Msg(playerid, COLOR_INFO2, "Pijesz Wino!");

  return 1;

}

CMD:papieros(playerid){
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);
  Msg(playerid, COLOR_INFO2, "Palisz Papierosa!");
  return 1;

}

CMD:sprunk(playerid){
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_SPRUNK);
  SetPlayerDrunkLevel(playerid, 0);
  Msg(playerid, COLOR_INFO2, "Pijesz Sprunka!");
  return 1;

}

CMD:cellout(playerid){
  SetPlayerSpecialAction(playerid,SPECIAL_ACTION_STOPUSECELLPHONE);
  Msg(playerid, COLOR_INFO2, "Odkladasz telefon ");
  return 1;

}

CMD:drunk(playerid){
  if (LoopingAnim(playerid,"PED","WALK_DRUNK",4.0,1,1,1,1,1))
    Msg(playerid, COLOR_INFO2, "Chodzisz jak pijany(a)");
  return 1;

}

CMD:bomb(playerid){
  ClearAnimations(playerid);
  if (OnePlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Podkladasz bombe");
  return 1;

}

CMD:aresztuj(playerid){
  if (LoopingAnim(playerid,"ped", "ARRESTgun", 4.0, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Aresztujesz kogos");
  return 1;

}

CMD:rap(playerid){
  if (LoopingAnim(playerid, "RAPPING", "RAP_B_Loop", 4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Rapujesz ");
  return 1;

}

CMD:lookout(playerid){
  if (OnePlayAnim(playerid, "SHOP", "ROB_Shifty", 4.0, 0, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Rozgladasz sie");
  return 1;

}

CMD:napad(playerid){
  if (LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Robisz napad");
  return 1;

}

CMD:rece2(playerid){
  if (LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1))
    Msg(playerid, COLOR_INFO2, "Zakladasz rece");
  return 1;

}

CMD:lez(playerid){
  if (LoopingAnim(playerid,"SUNBATHE","batherdown",4.1,0,1,1,1,1))
    Msg(playerid, COLOR_INFO2, "Lezysz ");
  return 1;

}

CMD:hide(playerid){
  if (LoopingAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Oslaniasz sie. ");
  return 1;

}

CMD:rzygaj(playerid){
  if (OnePlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Blee! ");
  return 1;

}

CMD:jedz(playerid){
  if (OnePlayAnim(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Jesz cos ");
  return 1;

}

CMD:deal(playerid){
  if (OnePlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Dealujesz z kims ");
  return 1;

}

CMD:cpun(playerid){
  if (LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
  return 1;

}

CMD:smoke(playerid){
  if (LoopingAnim(playerid,"SMOKING","M_smklean_loop",4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Palisz papierosa");
  return 1;

}

CMD:fsmoke(playerid){
  if (LoopingAnim(playerid,"SMOKING","F_smklean_loop",4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Palisz papierosa jak kobieta ");
  return 1;

}

CMD:smoke2(playerid){
  if (LoopingAnim(playerid,"SMOKING","M_smkstnd_loop",4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Palisz papierosa");
  return 1;

}

CMD:rece3(playerid){
  if (LoopingAnim(playerid,"DAM_JUMP","DAM_Dive_Loop",4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Stoisz na rekach");
  return 1;

}

CMD:siadaj(playerid){
  if (LoopingAnim(playerid,"BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0))
    Msg(playerid, COLOR_INFO2, "Siadasz");
  return 1;

}

CMD:chat(playerid){
  if (LoopingAnim(playerid,"PED","IDLE_CHAT", 4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Rozmawiasz z kims");
  return 1;

}

CMD:fucku(playerid){
  if (OnePlayAnim(playerid,"PED","fucku",4.0,0,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Pokazujesz SPIERDALAJ ");
  return 1;

}

CMD:taichi(playerid){
  if (LoopingAnim(playerid,"PARK","Tai_Chi_Loop",4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Umiesz takewondo ");
  return 1;

}

CMD:pij(playerid){
  if (LoopingAnim(playerid,"BAR","dnk_stndF_loop",4.0,1,0,0,0,0))
    Msg(playerid, COLOR_INFO2, "Pijesz cos ");
  return 1;

}

CMD:inbedright(playerid){
  if (LoopingAnim(playerid,"INT_HOUSE","BED_In_R",4.1,0,1,1,1,1))
    Msg(playerid, COLOR_INFO2, "Idzisz spac");
  return 1;

}

CMD:inbedleft(playerid){
  if (LoopingAnim(playerid,"INT_HOUSE","BED_In_L",4.1,0,1,1,1,1))
    Msg(playerid, COLOR_INFO2, "Idziesz spac");
  return 1;

}

CMD:dance(playerid,cmdtext[]){
  if (IsPlayerInAnyVehicle(playerid)) {
    Msg(playerid,COLOR_ERROR, "Nie mozna uzywac animacji w pojezdzie.", false);
    return 0;
  }

  new dance;
  if(sscanf(cmdtext,"d",dance)){
      Msg(playerid,COLOR_ERROR,"U¿yj: /dance [1-4]");
    return 1;
  }

  if(dance < 1 || dance > 4) {
    Msg(playerid,COLOR_ERROR,"U¿yj: /dance [1-4]");
    return 1;
  }

  if(dance == 1) {
    SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
  } else if(dance == 2) {
    SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
  } else if(dance == 3) {
    SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
  } else if(dance == 4) {
    SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
  }

  return 1;
}
// EOF
