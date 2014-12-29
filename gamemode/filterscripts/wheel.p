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

#define MAX_SERVER_PLAYERS 240

new
  Float:oKoloOffsets[10][3]=
  {
    {0.0699,0.0600,-11.7500},
    {-6.9100,-0.0899,-9.5000},
    {11.1600,0.0000,-3.6300},
    {-11.1600,-0.0399,3.6499},
    {-6.9100,-0.0899,9.4799},
    {0.0699,0.0600,11.7500},
    {6.9599,0.0100,-9.5000},
    {-11.1600,-0.0399,-3.6300},
    {11.1600,0.0000,3.6499},
    {7.0399,-0.0200,9.3600}
  },
  Float:koloAngle=0.0,
  koloAlternate=0,
  oKolo[12];

public OnFilterScriptInit()
{
  oKolo[10]=CreateObject(18877, 389.7734, -2028.4688, 22, 0, 0, 90, 300);
  oKolo[11]=CreateObject(18878, 389.7734, -2028.4688, 22, 0, 0, 90, 300);

  for(new i=0; i<sizeof(oKolo)-2; i++){
    oKolo[i]=CreateObject(18879,389.7734,-2028.4688,22,0,0,90,300);
    AttachObjectToObject(oKolo[i], oKolo[10], oKoloOffsets[i][0], oKoloOffsets[i][1], oKoloOffsets[i][2], 0.0, 0.0, 90, 0); // no streamer!?
  }

  SetTimer("krecWheel",3000,false);
  return 1;
}

public OnFilterScriptExit()
{
  for(new i=0; i<sizeof(oKolo); i++){
    DestroyObject(oKolo[i]);
  }
  return 1;
}

public OnObjectMoved(objectid)
{
  if(objectid==oKolo[10]){
    SetTimer("krecWheel",3000,false);
  }
  return 1;
}

forward krecWheel();
public krecWheel()
{
  koloAngle+=36;

  if(koloAngle>=360){
    koloAngle=0;
  }
  if(koloAlternate){
    koloAlternate=0;
  }
  else{
    koloAlternate=1;
  }

  new Float:koloModZPos=0.0;
  if(koloAlternate){
    koloModZPos=0.05;
  }

  MoveObject(oKolo[10], 389.7734, -2028.4688, 22.0+koloModZPos, 0.005,0, koloAngle, 90);
}
// EOF
