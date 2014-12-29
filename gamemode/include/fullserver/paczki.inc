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

#define MAX_PACZKI 300

enum e_paczka
{
  dbident,
  objid
}

new paczki[MAX_PACZKI][e_paczka];

zaladujPaczki(){
  for(new i=0; i<MAX_PACZKI;i++){
    DestroyDynamicPickup(paczki[i][objid]);
    paczki[i][objid]=-1;
  }

  mysql_query("SELECT id,x,y,z,i,vw FROM fs_paczki");
  mysql_store_result();
  new Float:pP[3], pI, pVW, buffer[255],dbide;
  new i=0;
  while(mysql_fetch_row_format(buffer,"|")){
      if(i>MAX_PACZKI) break;
    if (IsValidDynamicPickup(paczki[i][objid])) {
      DestroyDynamicPickup(paczki[i][objid]);
      paczki[i][objid]=-1;
    }

    if (sscanf(buffer,"p<|>dfffdd",
        dbide, pP[0], pP[1], pP[2], pI, pVW)) continue;

    paczki[i][objid]=CreateDynamicPickup(1279, 2, pP[0], pP[1], pP[2]+0.25, pVW, pI, -1);
    paczki[i][dbident]=dbide;
    i++;
  }
  mysql_free_result();
}

OPEDP_Paczki(playerid,pickupid)
{
  for(new i=0; i<MAX_PACZKI;i++)
  {
      if(paczki[i][objid]==pickupid)
    {
      if(!pData[playerid][loggedIn]){
        Msg(playerid,COLOR_ERROR,"Musisz byæ zarejestrowany aby podnieœæ paczkê.");
        return 1;
      }

      // rejestrujemy odnalezienie paczki
      new buf[128],zapisano;
      format(buf,sizeof buf,"SELECT * FROM fs_paczki_gracze WHERE id_gracza=%d AND id_paczki=%d",pData[playerid][accountID],paczki[i][dbident]);
      mysql_query(buf);
      mysql_store_result();
      zapisano=mysql_num_rows();
      mysql_free_result();
      format(buf,sizeof buf,"INSERT IGNORE INTO fs_paczki_gracze SET id_paczki=%d,id_gracza=%d", paczki[i][dbident], pData[playerid][accountID]);
      mysql_query(buf);
      if(zapisano>0){
        Msg(playerid,COLOR_ERROR,"Ta paczka by³a ju¿ przez Ciebie odnaleziona.");
        return 1;
      }
      else
      {
        // nadajemy punkty respektu
        format(buf,sizeof buf,"SELECT count(id_paczki) il FROM fs_paczki_gracze WHERE id_gracza=%d", pData[playerid][accountID]);
        new iloscPodkow[12],il;
        mysql_query(buf);
        mysql_store_result();
        mysql_retrieve_row();
        mysql_get_field("il", iloscPodkow);
        mysql_free_result();

        sscanf(iloscPodkow,"d",il);

        GivePlayerScore(playerid,il,"paczka");
        avt_record(playerid,e_paczki,1,ART_ADD);
        if (il>1 && il%10==0){
            format(buf,sizeof buf,"{b}%s{/b} odnajduje swoj¹ {b}%d{/b} paczkê i otrzymuje {b}%d{/b} respektu!",GetPlayerNick(playerid),il,il);
          foreach(players){
            Msg(players,COLOR_INFO3,buf);
          }
        }else{
            format(buf,sizeof buf,"Odnajdujesz swoj¹ {b}%d{/b} paczkê i otrzymujesz {b}%d{/b} respektu!",il,il);
          Msg(playerid,COLOR_INFO3,buf);
        }
      }
    }
  }
  return 0;
}

CMD:paczkireload(playerid)
{
    if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
    zaladujPaczki();
    Msg(playerid,COLOR_INFO3,"Done");
    return 1;
}

CMD:dodajpaczke(playerid,params[])
{
  if(!IsAdmin(playerid, LEVEL_ADMIN3)) return 0;
  new opis[128],Float:x,Float:y,Float:z,interior,vw;
  if(sscanf(params,"s[128]",opis))
      return Msg(playerid,COLOR_ERROR,"Uzycie: /dodajpaczke <opis>");
  GetPlayerPos(playerid,x,y,z);
  interior=GetPlayerInterior(playerid);
  vw=GetPlayerVirtualWorld(playerid);
  new buf[128];
  format(buf,sizeof buf,"INSERT INTO fs_paczki SET x=%f,y=%f,z=%f,vw=%d,i=%d,opis=\"%s\"",x,y,z,vw,interior,opis);
  mysql_query(buf);
  Msg(playerid,COLOR_INFO3,"Done");
  return 1;
}
// EOF
