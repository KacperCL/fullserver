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

/* gang zone wars
 * Funkcje zwiekszania respektu za obecnosc i automatycznego zmniejszania wywolywane sa tylko na serwerze 0.3d (#ifdef SA_MP_03d)
 *
 * gzw_ShowGangZonesForPlayers() - pokazuje strefy gangow wszystkim graczom
 * gzw_LoadGangZones() - wczytuje strefy gangow z bazy danych
 * gzw_UnloadGangZones() - niszczy strefy gangow (wywolywane przy ich przeladowaniu)
 * /ktorastrefa - podaje ID strefy w ktorej znajduje sie admin
 * /gzreload  - wymusza przeladowanie stref gangow (i zmniejszenie licznikow!)
 * gzw_GetZoneID(X,Y) - zwraca id strefy w podanym X,Y
 * Timer:gzw_increaseGZCounters - zwieksza respekt dla stref na ktorych przebywaja czlonkowie
 * gzw_pk - gracz zabija gracza z innego gangu na strefie - zwiekszanie/zmniejszanie respektu - wywolywane z OnPlayerDeath
 * gzw_tk - gracz zabija gracza z tego samego gangu na strefie - zmniejszanie respektu - wywolywane z OnPlayerDeath
 */


#define GZONES_MAX 25
#define GZONE_DEFAULT_COLOR "909090"

enum e_gzone {
  eg_dbid,
  eg_gz,
  Float:eg_XY[4],
  eg_color
}

new gangzones[GZONES_MAX][e_gzone];

gzw_ShowGangZonesForPlayer(playerid)
  for (new i=0;i<GZONES_MAX;i++)
    if (gangzones[i][eg_XY][0]==gangzones[i][eg_XY][0])
      GangZoneShowForPlayer(playerid,gangzones[i][eg_gz],gangzones[i][eg_color]);


gzw_LoadGangZones(){
  mysql_query("DELETE FROM fs_zones_gangscore WHERE respect=0");
  for (new i=0;i<GZONES_MAX;i++)
    gangzones[i][eg_XY][0]=FLOAT_NAN;

  new i=0;
  new buf[127];

  new tmp_color[12];
  mysql_query("SELECT * FROM (select z.id,z.miny,z.minx,z.maxy,z.maxx,IFNULL(g.color,'" #GZONE_DEFAULT_COLOR "') from fs_zones z LEFT JOIN fs_zones_gangscore zgs ON zgs.id_zone=z.id LEFT JOIN fs_gangs g ON g.id=zgs.id_gang WHERE z.active=1 ORDER BY zgs.respect DESC) s1 GROUP BY id LIMIT " #GZONES_MAX ";");
  mysql_store_result();
  while (mysql_fetch_row_format(buf,"|"))
    if (!sscanf(buf,"p<|>dffffs[64]", gangzones[i][eg_dbid], gangzones[i][eg_XY][0], gangzones[i][eg_XY][1], gangzones[i][eg_XY][2], gangzones[i][eg_XY][3], tmp_color)) {
        gangzones[i][eg_color]=HexToInt(tmp_color)*256+0x90;
        gangzones[i][eg_gz]=GangZoneCreate(gangzones[i][eg_XY][0], gangzones[i][eg_XY][1], gangzones[i][eg_XY][2], gangzones[i][eg_XY][3]);
        GangZoneShowForAll(gangzones[i][eg_gz], gangzones[i][eg_color]);
        i++;
    }
  mysql_free_result();

}

gzw_UnloadGangZones()
  for (new i=0;i<GZONES_MAX;i++)
    if (gangzones[i][eg_XY][0]==gangzones[i][eg_XY][0] && gangzones[i][eg_XY][0]>0)
      GangZoneDestroy(gangzones[i][eg_gz]);

CMD:gzreload(plr){
  if (!IsPlayerAdmin(plr)) return 0;
  gzw_UnloadGangZones();
  gzw_LoadGangZones();
  Msg(plr,COLOR_INFO,"Przeladowano strefy gangow");
  return 1;
}

CMD:ktorastrefa(plr,cmd){
  if (!IsAdmin(plr)) return 0;
  new str[128];
  new Float:P[3];

  GetPlayerPos(plr, P[0],P[1],P[2]);
  new gzid=gzw_GetZoneID(P[0],P[1]);
  if (gzid<0)
    return SendClientMessage(plr,-1,"Nie znaleziono strefy");

  format(str,sizeof str, "Strefa %d (dbid: %d)", gzid, gangzones[gzid][eg_dbid]);
  SendClientMessage(plr,-1,str);
  return 1;
}

gzw_GetZoneID(Float:gX, Float:gY) {
  for (new i=0;i<GZONES_MAX;i++)
    if (gangzones[i][eg_XY][0]==gangzones[i][eg_XY][0] && gangzones[i][eg_XY][0]<=gX && gX<=gangzones[i][eg_XY][2] && gangzones[i][eg_XY][1]<=gY && gY<=gangzones[i][eg_XY][3])
      return i;
  return -1;
}

/* gzw_pk
 * gracz playerid z gangu gang1 zabija gracza killid z gangu gang2
 */
gzw_pk(playerid,gang1,gang2,killid){
  new Float:P[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  new gzid=gzw_GetZoneID(P[0],P[1]);
  if (gzid<0) return 0;
  gzid=gangzones[gzid][eg_dbid];
  // narazie bez buforowania

  new str[128];

  format(str,sizeof str,"UPDATE fs_zones_gangscore SET respect=respect-1 WHERE id_gang=%d AND id_zone=%d AND respect>=1", gang2, gzid);
  mysql_query(str);

  format(str,sizeof str,"INSERT INTO fs_zones_gangscore SET respect=4,id_gang=%d,id_zone=%d ON DUPLICATE KEY UPDATE respect=respect+2", gang1, gzid);
  mysql_query(str);

  Msg(playerid,COLOR_INFO3,"Zabi³eœ cz³onka obcego gangu. Zyskujesz respekt dzielnicy.");
  Msg(killid,COLOR_INFO3,"Cz³onek obcego gangu zabi³ Cie. Tracisz respekt dzielnicy.");

  gzw_UnloadGangZones();
  gzw_LoadGangZones();
  return 1;
}

/* gzw_tk
 * gracz playerid z pgang zabija czlonka swojego gangu
 */
gzw_tk(playerid,pgang){

  new Float:P[3];
  GetPlayerPos(playerid,P[0],P[1],P[2]);
  new gzid=gzw_GetZoneID(P[0],P[1]);
  if (gzid<0) return 0;
  gzid=gangzones[gzid][eg_dbid];

  // narazie bez buforowania

  format(gstr,sizeof gstr,"UPDATE fs_zones_gangscore SET respect=respect-1 WHERE id_gang=%d AND id_zone=%d AND respect>0", pgang, gzid);
  mysql_query(gstr);

  Msg(playerid,COLOR_INFO3,"Zabi³eœ cz³onka swojego gangu. Tracisz respekt dzielnicy.");

  gzw_UnloadGangZones();
  gzw_LoadGangZones();

  return 1;
}

Timer:gzw_increaseCnt[120000](){  // 2 minuty
// sprawdzamy gdzie sa gracze, jesli na dzielnicach to zwiekszamy respekt dla danego gangu
//  new bufor[MAX_GANGS][GZONES_MAX];
  new zid,Float:P[3];
  foreach(i)
   if (IsPlayerConnected(i) && pTemp[i][staleTime] < 3 && GetPlayerVirtualWorld(i)==0 && GetPlayerInterior(i)==0 && pData[i][gang]!=NO_GANG) {

    if (!(pData[i][gang]>0)) continue;
    GetPlayerPos(i,P[0],P[1],P[2]);
    zid=gzw_GetZoneID(P[0],P[1]);
    if (zid>=0) { // gracz jest w gangu i na strefie
      zid=gangzones[zid][eg_dbid];
      format(gstr,sizeof gstr,"INSERT INTO fs_zones_gangscore SET respect=2,id_gang=%d,id_zone=%d ON DUPLICATE KEY UPDATE respect=respect+2", pData[i][gang], zid);
      mysql_query(gstr);
      Msg(i,COLOR_INFO3,"Przebywasz na dzielnicy. Zyskujesz jej respekt!");
    }
   }
  gzw_UnloadGangZones();
  gzw_LoadGangZones();
  return 1;
}

/*Timer:gzw_ReloadGZ[300000](){ // 5minut
  gzw_UnloadGangZones();
  gzw_LoadGangZones();
  return 1;
}*/
// EOF
