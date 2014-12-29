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
#include <3rdparty/streamer>

enum e_bordy {
  eb_modelid,
  Float:eb_pos[3],
  Float:eb_rot[3],
  eb_id
};

new bordy[][e_bordy]={
  { 19480, {1907.4278, -1354.2766, 25.8136}, {0.2999, 1.0000, -89.3789}, 0},
  { 19480, {1976.2366, -1427.0885, 30.2568}, {0.0000, 0.0000, 179.9486}, 0},
  { 19480, {1977.3334, -1371.1467, 37.7383}, {0.3999, -0.5999, -179.6561}, 0},
  { 19480, {1748.9255, -1420.3499, 41.3533}, {0.0000, 0.0000, -119.6871}, 0},
  { 19480, {1988.3371, -1476.5570, 24.5247}, {0.0000, 0.0000, 89.4593}, 0},
  { 19480, {1977.0510, -1426.9090, 30.7080}, {0.0000, 0.0000, 0.1985}, 0},
  { 19480, {1977.8809, -1371.0616, 37.5137}, {0.0000, 0.0000, -0.4533}, 0},
  { 19480, {1907.5191, -1353.3818, 26.1468}, {0.0000, 0.0000, 89.4956}, 0},
  { 19480, {1751.6282, -1483.5322, 30.0614}, {0.0000, 0.0000, -50.4256}, 0},
  { 19480, {1685.0848, -1176.3601, 64.4401}, {0.0000, 0.0000, 124.2751}, 0},
  { 19480, {1583.5576, -1176.6380, 56.4216}, {0.0000, 0.0000, 60.6832}, 0},
  { 19480, {1536.0476, -1181.0617, 60.4271}, {0.0000, 0.0000, 144.4995}, 0},
  { 19480, {1475.7640, -990.9024, 61.7234}, {0.0000, 0.0000, 40.1327}, 0}
};

new bordy_teksty[][]={
  "Chcesz stuningowac swoje auto?\nOdwiedz {6666ff}/warsztat{ffffff} w Las Venturas!",
  "Poszukiwani doswiadczeni kierowcy TIRow\nDolacz na {6666FF}/tir",
  "Widzisz czitera?\n{FF3030}/raport nick powod",
  "Odwiedz nas w Internecie\n{9090FF}WWW.FULLSERVER.EU",
  "Chcesz kupic dom?\n{9090ff}www.fullserver.eu/domy/",
  "Kosciol w San Fierro zaprasza wszystkich wiernych\n{E33E00}/kosciol",
  "Chcesz sie rozerwac?\nOdwiedz {ff9090}/minigun",
  "Polub nas na Facebooku!\n{9090FF}www.facebook.com/fullserver",
  "Dolacz do pracy w policji.\n{FFFF00}/policja{ffffff} i wez odznake!",
  "Czujesz sie zmeczony? {ff9090}Odprez sie lowiac rybki\n{9090ff}- najlepsze biora podobno na {6666ff}/wietnam",
  "Chcesz pisac lub dostarczac listy?\nOdwiedz {ff0000}poczte {ffffff}w budynku {ff0000}/bank",
  "{facb00}Czujesz silna potrzebe grupowej rywalizacji?\n{e33e00}Dolacz do ktoregos z FullServerowych gangow!",
    "Przyziemne wrazenia Ci nie wystarczaja?\n{00a1ff}Wybierz sie w kosmos rakieta NASA\n{ffffff}w poblizu {0000ff}/plazasf",
    "Odtwarzacz w Twym samochodzie gra stale te same kawalki?\nPosluchaj ktorejs ze stacji na {00ff00}/radio {ffffff}online!",
    "Jestes fanem skokow narciarskich?\nKoniecznie odwiedz {facb00}skocznie motorowerowa\n{00a1ff}im. Oleja {ffffff}w poblizu {00ff00}/domek",
  "Chcialbys poznac swiat z lotu ptaka?\nZalap sie na {9090ff}przelot jednym z balonow\n{ffffff}ladujacych na /g1 lub /skatepark",
    "{9090ff}Czy jestes na tyle dobry,\nby znalezc sie w jednej z kategorii\n{e33e00}/TOP10 {9090ff}FullServerowych graczy?",
    "{909090}Posiadasz dusze eksploratora?\nSzukaj ukrytych w calym San Andreas\n{ff9090}paczek {909090}i zdobywaj respekt!",
  "Masz sie za omnibusa?\nUdowodnij innym swa wiedze w {ff0000}quizie\n{ffffff}odbywajacym sie {00ff00}codziennie {ffffff}w godz. {00ff00}11-12 i 19-20!",
  "Masz z kims problem i chcesz to zalatwic bez swiadkow?\nWyzwij go na {00a1ff}/solo",
  "Chcesz zorganizowac event? - pisz na {58a100}/togm\n{ffffff}Masz pytanie do administracji? - pisz na {c00000}/toadmin",
  "DM to dla Ciebie podstawa?\nSprawdz {ff0000}/areny {ffffff}i wybierz ktoras!",
  "Chcesz byc jedyny i wyjatkowy?\nZdobadz przedmiot pozadania wielu\n- serwerowy {ffff00}/artefakt",
  "Lubisz zwiedzac?\nWpisz {0000ff}/teleporty {ffffff}i odwiedz ciekawe miejsca!"
};

public OnFilterScriptInit(){
  for (new i=0; i<sizeof bordy; i++) {
    new r=random(sizeof bordy_teksty);
    bordy[i][eb_id] = CreateDynamicObject(bordy[i][eb_modelid], bordy[i][eb_pos][0], bordy[i][eb_pos][1], bordy[i][eb_pos][2], bordy[i][eb_rot][0], bordy[i][eb_rot][1], bordy[i][eb_rot][2]);
    SetDynamicObjectMaterialText(bordy[i][eb_id], 0, bordy_teksty[r], 110, "Arial", 21, 0, -1, -16777216, 1);
  }
}
// EOF
