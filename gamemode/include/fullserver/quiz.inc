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

new kat[128],pyt[386],odp[386],id_pyt,ttmp[4],dbgo[512];
new lu;// = GetTickCount();

stock pobierzPytanie(){
  mysql_query("SELECT id,kategoria,pytanie,odpowiedz FROM fs_quiz WHERE lu<(NOW()-INTERVAL 1 HOUR) ORDER BY RAND() LIMIT 1");
    if(!mysql_store_result()){
        printf("ID QUIZ: Failed to store mysql result!");
        mysql_free_result();
    return false;
  }
  mysql_fetch_row_format(dbgo,";");
    mysql_get_field("id", ttmp);
  sscanf(ttmp,"d",id_pyt);
  mysql_num_fields();
  mysql_num_rows();
  mysql_get_field("kategoria", kat);
  mysql_get_field("pytanie", pyt);
  mysql_get_field("odpowiedz", odp);
  mysql_free_result();
  new quiery[128];
  format(quiery,sizeof quiery,"UPDATE fs_quiz SET lu=NOW() WHERE id=%i", id_pyt);
  mysql_query(quiery);
  return true;
}

stock string_sub(str[],start,end)
{
  new dest[128];
  strmid(dest,str,start,end);
  return dest;
}

sprawdzOdpowiedz(playerid,msg[])
{
  if(odp[0] == '\0') return;
  new str[128],tolower_odp[128];
  format(tolower_odp,sizeof(tolower_odp),"%s",odp);
  strtolower(msg);
  strtolower(tolower_odp);
  if(!strcmp(msg,tolower_odp))
  {
    format(str,sizeof(str),"[QUIZ] {FFFF00}%s podaje prawid³ow¹ odpowiedŸ: {0FC0FC}%s", GetPlayerNick(playerid), odp);
    foreach(player)
      Msg(player,-1,str);
    kat[0] = pyt[0] = odp[0] = '\0';
    lu = GetTickCount();
    GivePlayerScore(playerid,2,"quiz");
    avt_record(playerid,e_quiz,1,ART_ADD);
  }
}

Timer:spooler[25000]()
{
  if(!gmTemp[forceQuiz]){
    new hr, minuteeee, seecondd;
    gettime(hr, minuteeee, seecondd);
    if(hr!=15 && hr!=17 && hr!=20) return; // dziala w godzinach 15 do 16, od 17 do 18 i od 20 do 21
  }
  new string[128];
  new podpowiedz[386];
  if(pyt[0] == '\0')
  {
    if(GetTickCount()-lu<25000) {
      return; // minimum 25s od ostatniego pytania
    }
    if(!pobierzPytanie()) {
        printf("ID QUIZ: pobierzPytanie() returned false!");
      return;
    }
    lu = GetTickCount();
    format(string,sizeof(string),"[QUIZ] {0FC0FC}[%s]: %s", kat,pyt);
    foreach(player)
      Msg(player,-1,string);
  }
  else if(GetTickCount()-lu<25000)
  {
    podpowiedz = odp;
    for(new i=1; i < math_ceil(strlen(odp)*0.9); i++)
      podpowiedz = replaceChar(podpowiedz,math_random(1,strlen(podpowiedz)));

    format(string,sizeof(string),"[QUIZ] PODPOWIED: {0FC0FC}%s", podpowiedz);
    foreach(player)
      Msg(player,-1,string);
  }
  else if(GetTickCount()-lu<45000)
  {
    podpowiedz = odp;
    for(new i=1; i < math_ceil(strlen(odp)*0.7); i++)
      podpowiedz = replaceChar(podpowiedz,math_random(1,strlen(podpowiedz)));

    format(string,sizeof(string),"[QUIZ] PODPOWIED: {0FC0FC}%s", podpowiedz);
    foreach(player)
      Msg(player,-1,string);
  }
  else if(GetTickCount()-lu<65000)
  {
    podpowiedz = odp;
    for(new i=1; i < math_ceil(strlen(odp)*0.5); i++)
      podpowiedz = replaceChar(podpowiedz,math_random(1,strlen(podpowiedz)));

    format(string,sizeof(string),"[QUIZ] PODPOWIED: {0FC0FC}%s", podpowiedz);
    foreach(player)
      Msg(player,-1,string);
  }else{
    kat[0] = pyt[0] = odp[0] = '\0';
    foreach(player)
      Msg(player,-1,"[QUIZ] {0FC0FC}Nikt nie poda³ prawid³owej odpowiedzi!");
  }
}

math_ceil(Float:value) return floatround(value,floatround_ceil);

math_random(min, max) return (random(max-min) + min);

replaceChar(str[],idx,rep='#')
{
  new strf[128];
  strcat(strf,str);
  if(strf[idx]!=32 || strf[idx]!=45)
    strf[idx] = rep;
    return strf;
}

stock trim(sSource[])
{
  new
    iBegin,
    iEnd,
    iInputLength = strlen(sSource),
    sReturn[128];

  strcat(sReturn, sSource);

  for(iBegin = 0; iBegin < iInputLength; ++iBegin)
  {
    switch(sReturn[iBegin])
    {
      case ' ', '\t', '\r', '\n':
      {
        continue;
      }
      default:
      {
        break;
      }
    }
  }

  for(iEnd = (iInputLength - 1); iEnd > iBegin; --iEnd)
  {
    switch(sReturn[iEnd])
    {
      case ' ', '\t', '\r', '\n':
      {
        continue;
      }
      default:
      {
        break;
      }
    }
  }

  strdel(sReturn, (iEnd + 1), iInputLength);
  strdel(sReturn, 0, iBegin);

  return sReturn;
}

strtolower(text[])
{
    for(new i, j = strlen(text); j != i; ++i)
    {
        if((90 > text[i] > 64) || (222 > text[i] > 191)) text[i] |= 32;
    }
    return true;
}
// EOF
