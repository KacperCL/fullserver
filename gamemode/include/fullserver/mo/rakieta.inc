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

#define RAKIETA_H 8000
#define RAKIETA_S 100

new rakieta_e[15];
new rakieta_piasek;
new bool:rakieta_leci;

rakieta_init(){
  rakieta_piasek=CreateDynamicObject(18697,-2894.16943359,436.64916992,4.01179218,0.00000000,0.00000000,0.00000000,0,0,-1,1200); //object(heli_dust) (1)
  rakieta_e[0]=CreateDynamicObject(17000,-2894.32128906,437.12216187,3.91406250,0.00000000,0.00000000,270.00000000,0,0,-1,2000); //object(wt6suppsxc) (1)
  rakieta_e[2]=CreateDynamicObject(18694,-2895.98095703,437.10720825,19.11520576,270.00000000,90.00000000,0.00000000,0,0,-1); // amethrower) (1)
  rakieta_e[3]=CreateDynamicObject(18694,-2897.10400391,438.70050049,19.79021263,269.99450684,89.99450684,0.00000000,0,0,-1); //object(flamethrower) (2)
  rakieta_e[4]=CreateDynamicObject(18694,-2894.97998047,438.88446045,19.79021263,269.99450684,89.99450684,0.00000000,0,0,-1); //object(flamethrower) (3)
  rakieta_e[5]=CreateDynamicObject(18694,-2896.98925781,435.26464844,19.79021263,269.98901367,89.99450684,0.00000000,0,0,-1); //object(flamethrower) (4)
  rakieta_e[6]=CreateDynamicObject(18694,-2895.06323242,435.24649048,19.79021263,269.99450684,89.99450684,0.00000000,0,0,-1); //object(flamethrower) (5)
  rakieta_e[7]=CreateDynamicObject(18694,-2897.82373047,436.74099731,19.79021263,269.98901367,89.99450684,0.00000000,0,0,-1); //object(flamethrower) (4)
  rakieta_e[8]=CreateDynamicObject(18694,-2893.92529297,437.13635254,19.79021263,269.98901367,89.99450684,0.00000000,0,0,-1); //object(flamethrower) (4)
  rakieta_e[9]=CreateDynamicObject(3666,-2894.31689453,440.14334106,21.03767967,0.00000000,0.00000000,0.00000000,0,0,-1); //object(airuntest_las) (1)
  rakieta_e[10]=CreateDynamicObject(3666,-2891.67773438,438.62774658,21.03767967,0.00000000,0.00000000,0.00000000,0,0,-1); //object(airuntest_las) (2)
  rakieta_e[11]=CreateDynamicObject(3666,-2891.65405273,435.55056763,21.03767967,0.00000000,0.00000000,0.00000000,0,0,-1); //object(airuntest_las) (3)
  rakieta_e[12]=CreateDynamicObject(3666,-2894.32128906,434.05865479,21.03767967,0.00000000,0.00000000,0.00000000,0,0,-1); //object(airuntest_las) (4)
  rakieta_e[13]=CreateDynamicObject(3666,-2896.96142578,435.56622314,21.03767967,0.00000000,0.00000000,0.00000000,0,0,-1); //object(airuntest_las) (5)
  rakieta_e[14]=CreateDynamicObject(3666,-2896.96240234,438.61471558,21.03767967,0.00000000,0.00000000,0.00000000,0,0,-1); //object(airuntest_las) (6)
  rakieta_leci=false;
}

rakieta_liftoff(){
  new time;
  if (rakieta_leci) return 0;
  rakieta_leci=true;
  rakieta_e[1]=CreateDynamicObject(18718,-2894.31542969,437.09564209,35.23916245,0.00000000,0.00000000,0.00000000,0,0,-1,1200); // iskry
  DestroyDynamicObject(rakieta_piasek);
  for (new i=0;i<sizeof rakieta_e;i++) {
    new Float:P[3];
    GetDynamicObjectPos(rakieta_e[i],P[0],P[1],P[2]);
    if (i==0) { // glowny element rakiety
      CreateExplosion(P[0],P[1],P[2],9,20);
      CreateExplosion(P[0]+1,P[1]+1,P[2],7,30);
    }
    time=MoveDynamicObject(rakieta_e[i],P[0],P[1],P[2]+RAKIETA_H,RAKIETA_S);
  }
  rakieta_wybuch(20);
  SetTimer("rakieta_reset",time,false);
  return 1;
}

forward rakieta_reset();
public rakieta_reset(){
  for (new i=0;i<sizeof rakieta_e;i++)
    DestroyDynamicObject(rakieta_e[i]);
  rakieta_init();
  return 1;
}

forward rakieta_wybuch(c);
public rakieta_wybuch(c){
  new Float:P[3];
  GetDynamicObjectPos(rakieta_e[0],P[0],P[1],P[2]);
  CreateExplosion(P[0],P[1],P[2],9,20);
  if (c>=18)
    CreateExplosion(P[0],P[1],P[2],7,30);
  if (--c>0)
    SetTimerEx("rakieta_wybuch",random(250)+50,false,"d",c);
}

forward rakieta_countdown(playerid);
public rakieta_countdown(playerid){
  if (!rakieta_liftoff())
    return Msg(playerid,COLOR_ERROR,"Teraz nie mozna odpalic rakiety");
  return 1;
}

rakieta_confirmliftoff(playerid){
  if (!pData[playerid][vipEnabled] && !IsGM(playerid))
    return Msg(playerid,COLOR_ERROR,"Komenda tylko dla VIPow");
  if (rakieta_leci)
    return Msg(playerid,COLOR_ERROR,"Rakiety jeszcze nie mozna odpalic");

  copy("rakieta_countdown",pTemp[playerid][dialogCB]);

  ShowPlayerDialog(playerid,DIALOG_CONFIRM,DIALOG_STYLE_MSGBOX, "Start rakiety","Potwierdz odpalenia rakiety","Odpalaj","Anuluj");
  return 1;
}
// EOF
