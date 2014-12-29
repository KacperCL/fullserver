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

new balon1;
new balon1_pidx=0;

new Float:balon1_punkty[][5] = {
//    x y   z   predkosc  ile sekund czekac po tym przystanku */
  {1975.8,960.6,52.7, 2.0,  5.0}, // 4smoki dach
  {1993.0,974.0,79.9, 2.0,  0.0}, // nad kasynem
  {2224.0,1064.0,93.87,2.0, 0.0}, // nad domkami
  {2243.59,1076.17,33.52,2.0, 5.0}, // domki
  {2256.84,1084.66,101.46,2.0,  0.0}, // nad domkami2
  {2326.10,1282.81,120.26,2.0,  0.0}, // nad piramida
  {2329.64,1413.50,42.82,2.0, 5.0}, // garaz
  {2038.36,1160.12,85.74,2.0, 0.0},
  {1993.0,974.0,89.9, 2.0,  0.0}  // nad kasynem
};

new balon2;
new balon2_pidx=0;
new Float:balon2_punkty[][5] = {

{2439.09, -1788.55, 17.53,    2.0,    5.0},    // /getto
{2433.84, -1785.72, 31.52,    2.0,    0.0},    // nad /getto
{2246.95,-1627.07,61.11,2.0,    0.0},    // trasa na skatepark
{1981.51, -1438.56, 48.41,2.0,    0.1},    // nad /skatepark
{1868.15, -1381.43, 13.51,2.0,    5.0},    // /skatepark
{1836.63, -1400.35, 62.81,2.0,    0.0},    // nad /skatepark2
{1511.90, -1630.90, 107.31,2.0,    0.0},    // trasa na /ls
{1060.26, -1733.41, 53.91,2.0,    0.1},    // nad /ls
{1017.97, -1753.42, 24.20,    2.0,    5.0},    // wejscie snajpera /ls
{1021.21, -1759.96, 40.69,    2.0,    0.0},    // nad /ls2
{1131.71, -1955.08, 117.78,2.0,    0.1},    // trasa
{1190.71, -2037.08, 69.00,2.0,    5.0},    // willa w ls
{1282.80, -2018.43, 83.88,    2.0,    0.0},    // trasa na getto
{2364.36, -1817.45, 58.05,    2.0,    0.1}    // nad /getto
};

/*new balon3;
new balon3_pidx=0;

new Float:balon3_punkty[][5] = { //urodziny

  {186.2369,-1872.6927,2.7074,2.0,6.0}, //urodziny
  {398.1367,-1820.4058,14.7278,2.0,3.0}, //urodziny
  {364.9092,-2062.3018,15.3952,2.0,3.0} //urodziny
};*/

forward balon1_go();
public balon1_go(){
  new time=MoveDynamicObject(balon1, balon1_punkty[balon1_pidx][ 0 ], balon1_punkty[balon1_pidx][1], balon1_punkty[balon1_pidx][2], balon1_punkty[balon1_pidx][3], 0,0, random(90)-45);
  time+=floatround(balon1_punkty[balon1_pidx][4]*1000);
  balon1_pidx++;
  if (balon1_pidx>=sizeof balon1_punkty)  balon1_pidx=0;

  SetTimer("balon1_go", time,false);

  return 1;
}

forward balon2_go();
public balon2_go(){
  new time=MoveDynamicObject(balon2, balon2_punkty[balon2_pidx][ 0 ], balon2_punkty[balon2_pidx][1], balon2_punkty[balon2_pidx][2], balon2_punkty[balon2_pidx][3], 0,0, random(90)-45);
  time+=floatround(balon2_punkty[balon2_pidx][4]*1000);
  balon2_pidx++;
  if (balon2_pidx>=sizeof balon2_punkty)  balon2_pidx=0;

  SetTimer("balon2_go", time,false);

  return 1;
}

/*forward balon3_go(); //urodziny
public balon3_go(){
  new time=MoveDynamicObject(balon3, balon3_punkty[balon3_pidx][ 0 ], balon3_punkty[balon3_pidx][1], balon3_punkty[balon3_pidx][2], balon3_punkty[balon3_pidx][3], 0,0, random(90)-45);
  time+=floatround(balon3_punkty[balon3_pidx][4]*1000);
  balon3_pidx++;
  if (balon3_pidx>=sizeof balon3_punkty)  balon3_pidx=0;

  SetTimer("balon3_go", time,false);

  return 1;
}*/

balony_init(){
  balon1=CreateDynamicObject(19332,balon1_punkty[balon1_pidx][0],balon1_punkty[balon1_pidx][1],balon1_punkty[balon1_pidx][2],0,0,0,0,0,-1,1200);
  balon1_go();
  balon2=CreateDynamicObject(19332,balon2_punkty[balon2_pidx][0],balon2_punkty[balon2_pidx][1],balon2_punkty[balon2_pidx][2],0,0,0,0,0,-1,1200);
  balon2_go();
  /*balon3=CreateDynamicObject(19338,balon3_punkty[balon3_pidx][0],balon3_punkty[balon3_pidx][1],balon3_punkty[balon3_pidx][2],0,0,0,0,0,-1,1200);
  balon3_go();*/
}
// EOF
