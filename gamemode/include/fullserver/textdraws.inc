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

CreateTextDraws()
{
  new
   Float:tmp = 0.0;

  gTextDraw[27] = TextDrawCreate(0.0, 0.0, "~n~~n~~n~~n~~n~~n~~n~");
  TextDrawLetterSize(gTextDraw[27], 0.0, 10.0);
  TextDrawUseBox(gTextDraw[27], 1);
  TextDrawBoxColor(gTextDraw[27], 0xFF0000BB);

  gTextDraw[26] = TextDrawCreate(0.0, 0.0, "~n~~n~~n~~n~~n~~n~~n~");
  TextDrawLetterSize(gTextDraw[26], 0.0, 10.0);
  TextDrawUseBox(gTextDraw[26], 1);
  TextDrawBoxColor(gTextDraw[26], 0xFFFFFFBB);

  for(new i = 0; i < 10; i++)
  {
    gTextDraw[i] = DrawRectangle(0.0, 640.0, 0.0, 60.0 + tmp, 0x00000015);
    tmp += 1.0;
  }

  tmp = 0.0;

  for(new i = 10; i < 20; i++)
  {
    gTextDraw[i] = DrawRectangle(0.0, 640.0, 388.0 - tmp, 480.0, 0x00000015);
    tmp += 1.0;
  }

  gTextDraw[20] = TextDrawCreate(643.000000, 428.000000, "~n~~n~~n~");
  TextDrawAlignment(gTextDraw[20], 3);
  TextDrawBackgroundColor(gTextDraw[20], 255);
  TextDrawFont(gTextDraw[20], 1);
  TextDrawLetterSize(gTextDraw[20], 0.500000, 1.000000);
  TextDrawColor(gTextDraw[20], 320017407);
  TextDrawSetOutline(gTextDraw[20], 0);
  TextDrawSetProportional(gTextDraw[20], 1);
  TextDrawSetShadow(gTextDraw[20], 1);
  TextDrawUseBox(gTextDraw[20], 1);
  TextDrawBoxColor(gTextDraw[20], 320017200);
  TextDrawTextSize(gTextDraw[20], 0.000000, 0.000000);

  gTextDraw[21] = TextDrawCreate(643.000000, 430.000000, "~n~~n~~n~");
  TextDrawAlignment(gTextDraw[21], 3);
  TextDrawBackgroundColor(gTextDraw[21], 255);
  TextDrawFont(gTextDraw[21], 1);
  TextDrawLetterSize(gTextDraw[21], 0.500000, 1.000000);
  TextDrawColor(gTextDraw[21], 320017407);
  TextDrawSetOutline(gTextDraw[21], 0);
  TextDrawSetProportional(gTextDraw[21], 1);
  TextDrawSetShadow(gTextDraw[21], 1);
  TextDrawUseBox(gTextDraw[21], 1);
  TextDrawBoxColor(gTextDraw[21], 320017200);
  TextDrawTextSize(gTextDraw[21], 0.000000, 0.000000);

  gTextDraw[22] = TextDrawCreate(643.000000, 439.000000, "~n~");
  TextDrawAlignment(gTextDraw[22], 3);
  TextDrawBackgroundColor(gTextDraw[22], 255);
  TextDrawFont(gTextDraw[22], 1);
  TextDrawLetterSize(gTextDraw[22], 0.500000, 1.000000);
  TextDrawColor(gTextDraw[22], 320017407);
  TextDrawSetOutline(gTextDraw[22], 0);
  TextDrawSetProportional(gTextDraw[22], 1);
  TextDrawSetShadow(gTextDraw[22], 1);
  TextDrawUseBox(gTextDraw[22], 1);
  TextDrawBoxColor(gTextDraw[22], 320017200);
  TextDrawTextSize(gTextDraw[22], 0.000000, 0.000000);

  gTextDraw[30] = TextDrawCreate(581.000000, 1.000000,"" #GMVERSION " RUNNING WITH DEBUG FLAG");
  TextDrawBackgroundColor(gTextDraw[30], 255);
  TextDrawFont(gTextDraw[30], 1);
  TextDrawLetterSize(gTextDraw[30], 0.170000, 0.799999);
  TextDrawColor(gTextDraw[30], -1734829825);
  TextDrawSetOutline(gTextDraw[30], 0);
  TextDrawSetProportional(gTextDraw[30], 1);
  TextDrawSetShadow(gTextDraw[30], 0);
  TextDrawSetSelectable(gTextDraw[30], 0);
  TextDrawAlignment(gTextDraw[30], 2);

  gTextDraw[31] = TextDrawCreate(224.0, 221.0, __("Podpowiedz"));
  TextDrawLetterSize(gTextDraw[31], 0.22, 0.9);
  TextDrawColor(gTextDraw[31], 0xDDDDDDFF);
  TextDrawFont(gTextDraw[31], 2);
  TextDrawSetShadow(gTextDraw[31], 0);
  TextDrawAlignment(gTextDraw[31], 1);

  // 32 wolne
  // 73,74,75 - fullserver, wersja, tekst reklamowy - widoczny w wybieralce skinow

  gTextDraw[73] = TextDrawCreate(186.000000, 178.000000, "FullServer");
  TextDrawAlignment(gTextDraw[73], 3);
  TextDrawBackgroundColor(gTextDraw[73], 799);
  TextDrawFont(gTextDraw[73], 0);
  TextDrawLetterSize(gTextDraw[73], 1.149999, 3.099998);
  TextDrawColor(gTextDraw[73], -1);
  TextDrawSetOutline(gTextDraw[73], 1);
  TextDrawSetProportional(gTextDraw[73], 1);

  gTextDraw[74] = TextDrawCreate(187.000000, 208.000000, "v" #GMVERSION);
  TextDrawAlignment(gTextDraw[74], 3);
  TextDrawBackgroundColor(gTextDraw[74], 255);
  TextDrawFont(gTextDraw[74], 1);
  TextDrawLetterSize(gTextDraw[74], 0.500000, 1.000000);
  TextDrawColor(gTextDraw[74], 808464432);
  TextDrawSetOutline(gTextDraw[74], 0);
  TextDrawSetProportional(gTextDraw[74], 1);
  TextDrawSetShadow(gTextDraw[74], 0);

  gTextDraw[75] = TextDrawCreate(114.000000, 234.000000, __("Najlepszy serwer SA:MP w Polsce"));
  TextDrawAlignment(gTextDraw[75], 2);
  TextDrawBackgroundColor(gTextDraw[75], 255);
  TextDrawFont(gTextDraw[75], 1);
  TextDrawLetterSize(gTextDraw[75], 0.370000, 1.399999);
  TextDrawColor(gTextDraw[75], -1);
  TextDrawSetOutline(gTextDraw[75], 0);
  TextDrawSetProportional(gTextDraw[75], 1);
  TextDrawSetShadow(gTextDraw[75], 1);
  TextDrawUseBox(gTextDraw[75], 1);
  TextDrawBoxColor(gTextDraw[75], 0);
  TextDrawTextSize(gTextDraw[75], 85.000000, 146.000000);

  gTextDraw[TD_TIME]=TextDrawCreate(579.000000, 22.500000, "08:17");
  TextDrawAlignment(gTextDraw[TD_TIME], 2);
  TextDrawBackgroundColor(gTextDraw[TD_TIME], 255);
  TextDrawFont(gTextDraw[TD_TIME], 3);
  TextDrawLetterSize(gTextDraw[TD_TIME], 0.619999, 2.100000);
  TextDrawColor(gTextDraw[TD_TIME], -1);
  TextDrawSetOutline(gTextDraw[TD_TIME], 1);
  TextDrawSetProportional(gTextDraw[TD_TIME], 0);

  gTextDraw[TD_DATETIME] = TextDrawCreate(635.0, 431.0, "_");
  TextDrawAlignment(gTextDraw[TD_DATETIME],3);
  TextDrawLetterSize(gTextDraw[TD_DATETIME], 0.22, 1.3);
  TextDrawColor(gTextDraw[TD_DATETIME], -1);
  TextDrawFont(gTextDraw[TD_DATETIME], 1);
  TextDrawSetOutline(gTextDraw[TD_DATETIME], 1);
  TextDrawSetProportional(gTextDraw[TD_DATETIME], 1);
  TextDrawBackgroundColor(gTextDraw[TD_DATETIME], 96);

  gTextDraw[TD_VOTING] = TextDrawCreate(300.000000, 298.000000, " ");
  TextDrawAlignment(gTextDraw[TD_VOTING], 3);
  TextDrawBackgroundColor(gTextDraw[TD_VOTING], 255); //52364063);
  TextDrawFont(gTextDraw[TD_VOTING], 1);
  TextDrawLetterSize(gTextDraw[TD_VOTING], 0.309998, 1.100000);// 0.399998, 1.299998);
  TextDrawColor(gTextDraw[TD_VOTING], -1);
  TextDrawSetOutline(gTextDraw[TD_VOTING], 1);
  TextDrawSetProportional(gTextDraw[TD_VOTING], 1);
  TextDrawUseBox(gTextDraw[TD_VOTING], 1);
  TextDrawBoxColor(gTextDraw[TD_VOTING], 64);//-855664316);//1408581440);
  TextDrawTextSize(gTextDraw[TD_VOTING], 0.0,0.0);//90.000000, 0.000000);

  gTextDraw[TD_ANNOUNCE] = TextDrawCreate(320.0, 140.0, " "); // bylo 160 - przy dwoch liniach zaslanialo celownik i wkurwialo na WG
  TextDrawLetterSize(gTextDraw[TD_ANNOUNCE], 0.45, 1.6);
  TextDrawColor(gTextDraw[TD_ANNOUNCE], 0xFFFFD0FF);
  TextDrawFont(gTextDraw[TD_ANNOUNCE], 1);
  TextDrawSetShadow(gTextDraw[TD_ANNOUNCE], 0);
  TextDrawSetOutline(gTextDraw[TD_ANNOUNCE], 1);
  TextDrawBackgroundColor(gTextDraw[TD_ANNOUNCE], 0x00000070);
  TextDrawAlignment(gTextDraw[TD_ANNOUNCE], 2);
  TextDrawTextSize(gTextDraw[TD_ANNOUNCE], 400.0, 400.0);

  gTextDraw[TD_ANN2] = TextDrawCreate(488.000000, 270.000000, "_");
  TextDrawAlignment(gTextDraw[TD_ANN2], 2);
  TextDrawBackgroundColor(gTextDraw[TD_ANN2], 144);
  TextDrawFont(gTextDraw[TD_ANN2], 1);
  TextDrawLetterSize(gTextDraw[TD_ANN2], 0.300000, 1.300000);
  TextDrawColor(gTextDraw[TD_ANN2], -1);
  TextDrawSetOutline(gTextDraw[TD_ANN2], 0);
  TextDrawSetProportional(gTextDraw[TD_ANN2], 1);
  TextDrawSetShadow(gTextDraw[TD_ANN2], 1);
  TextDrawUseBox(gTextDraw[TD_ANN2], 1);
  TextDrawBoxColor(gTextDraw[TD_ANN2], 0);
  TextDrawTextSize(gTextDraw[TD_ANN2], 0.000000, 298.000000);

  for(new i = TD_STARS_START; i < TD_STARS_END; i++)
  {
    switch(i - TD_STARS_START + 1)
    {
      case 1, 7,  13, 19, 25: gTextDraw[i] = TextDrawCreate(618.0, 6.0, "]");
      case 2, 8,  14, 20, 26: gTextDraw[i] = TextDrawCreate(618.0, 6.0, "]~n~]");
      case 3, 9,  15, 21, 27: gTextDraw[i] = TextDrawCreate(618.0, 6.0, "]~n~]~n~]");
      case 4, 10, 16, 22, 28: gTextDraw[i] = TextDrawCreate(618.0, 6.0, "]~n~]~n~]~n~]");
      case 5, 11, 17, 23, 29: gTextDraw[i] = TextDrawCreate(618.0, 6.0, "]~n~]~n~]~n~]~n~]");
      case 6, 12, 18, 24, 30: gTextDraw[i] = TextDrawCreate(618.0, 6.0, "]~n~]~n~]~n~]~n~]~n~]");
    }

    switch(i - TD_STARS_START + 1)
    {
      case 1..6:    TextDrawColor(gTextDraw[i], 0xFA7800FF); // pomarañczowe
      case 7..12:   TextDrawColor(gTextDraw[i], 0xFFEF00FF); // ¿ó³te
      case 13..18:  TextDrawColor(gTextDraw[i], 0x00C100FF); // zielone
      case 19..24:  TextDrawColor(gTextDraw[i], 0x0060FAFF); // niebieskie
      case 25..30:  TextDrawColor(gTextDraw[i], 0xB50000FF); // czerwone
    }

    TextDrawLetterSize(gTextDraw[i], 0.535, 2.3);
    TextDrawFont(gTextDraw[i], 2);
    TextDrawSetShadow(gTextDraw[i], 0);
    TextDrawSetOutline(gTextDraw[i], 1);
    TextDrawBackgroundColor(gTextDraw[i], 0x00000099);
    TextDrawAlignment(gTextDraw[i], 1);
  }

  #define POS_TD_RACE(%0,%1) %0+428.000000, %1+270.000000

  gTextDraw[TD_ARACEBOX] = TextDrawCreate(POS_TD_RACE(0,0), " ");
  TextDrawBackgroundColor(gTextDraw[TD_ARACEBOX], 255);
  TextDrawFont(gTextDraw[TD_ARACEBOX], 1);
  TextDrawLetterSize(gTextDraw[TD_ARACEBOX], 0.270000, 1.000000);
  TextDrawColor(gTextDraw[TD_ARACEBOX], 65535);
  TextDrawSetOutline(gTextDraw[TD_ARACEBOX], 0);
  TextDrawSetProportional(gTextDraw[TD_ARACEBOX], 1);
  TextDrawSetShadow(gTextDraw[TD_ARACEBOX], 1);
  TextDrawUseBox(gTextDraw[TD_ARACEBOX], 1);
  TextDrawBoxColor(gTextDraw[TD_ARACEBOX], 96);
  TextDrawTextSize(gTextDraw[TD_ARACEBOX], 640.000000, 0.000000);

  gTextDraw[TD_ADRIFTBOX] = TextDrawCreate(428.000000, 304.000000, "_");  // textdraw atrakcji na drifcie
  TextDrawBackgroundColor(gTextDraw[TD_ADRIFTBOX], 255);
  TextDrawFont(gTextDraw[TD_ADRIFTBOX], 1);
  TextDrawLetterSize(gTextDraw[TD_ADRIFTBOX], 0.270000, 1.000000);
  TextDrawColor(gTextDraw[TD_ADRIFTBOX], 65535);
  TextDrawSetOutline(gTextDraw[TD_ADRIFTBOX], 0);
  TextDrawSetProportional(gTextDraw[TD_ADRIFTBOX], 1);
  TextDrawSetShadow(gTextDraw[TD_ADRIFTBOX], 0);
  TextDrawUseBox(gTextDraw[TD_ADRIFTBOX], 1);
  TextDrawBoxColor(gTextDraw[TD_ADRIFTBOX], 96);
  TextDrawTextSize(gTextDraw[TD_ADRIFTBOX], 640.000000, 0.000000);

  gTextDraw[TD_AWGINFO] = TextDrawCreate(535.000000, 330.000000, "~g~-   ~w~vs   ~b~-");
  TextDrawAlignment(gTextDraw[TD_AWGINFO], 2);
  TextDrawBackgroundColor(gTextDraw[TD_AWGINFO], -791621584);
  TextDrawFont(gTextDraw[TD_AWGINFO], 1);
  TextDrawLetterSize(gTextDraw[TD_AWGINFO], 0.759998, 2.500000);
  TextDrawColor(gTextDraw[TD_AWGINFO], -1);
  TextDrawSetOutline(gTextDraw[TD_AWGINFO], 1);
  TextDrawSetProportional(gTextDraw[TD_AWGINFO], 0);

  gTextDraw[TD_AWGINFOHP] = TextDrawCreate(535.000000, 361.000000, "~g~-HP               ~r~-HP");
  TextDrawAlignment(gTextDraw[TD_AWGINFOHP], 2);
  TextDrawBackgroundColor(gTextDraw[TD_AWGINFOHP], 255);
  TextDrawFont(gTextDraw[TD_AWGINFOHP], 1);
  TextDrawLetterSize(gTextDraw[TD_AWGINFOHP], 0.340000, 1.399999);
  TextDrawColor(gTextDraw[TD_AWGINFOHP], -1);
  TextDrawSetOutline(gTextDraw[TD_AWGINFOHP], 0);
  TextDrawSetProportional(gTextDraw[TD_AWGINFOHP], 1);
  TextDrawSetShadow(gTextDraw[TD_AWGINFOHP], 1);

  gTextDraw[TD_ACTFINFO] = TextDrawCreate(535.000000, 330.000000, "~g~-   ~w~vs   ~b~-");
  TextDrawAlignment(gTextDraw[TD_ACTFINFO], 2);
  TextDrawBackgroundColor(gTextDraw[TD_ACTFINFO], -791621584);
  TextDrawFont(gTextDraw[TD_ACTFINFO], 1);
  TextDrawLetterSize(gTextDraw[TD_ACTFINFO], 0.759998, 2.500000);
  TextDrawColor(gTextDraw[TD_ACTFINFO], -1);
  TextDrawSetOutline(gTextDraw[TD_ACTFINFO], 1);
  TextDrawSetProportional(gTextDraw[TD_ACTFINFO], 0);

  gTextDraw[TD_ACTFINFOPOINTS] = TextDrawCreate(535.000000, 361.000000, "~g~- PKT               ~b~- PKT");
  TextDrawAlignment(gTextDraw[TD_ACTFINFOPOINTS], 2);
  TextDrawBackgroundColor(gTextDraw[TD_ACTFINFOPOINTS], 255);
  TextDrawFont(gTextDraw[TD_ACTFINFOPOINTS], 1);
  TextDrawLetterSize(gTextDraw[TD_ACTFINFOPOINTS], 0.340000, 1.399999);
  TextDrawColor(gTextDraw[TD_ACTFINFOPOINTS], -1);
  TextDrawSetOutline(gTextDraw[TD_ACTFINFOPOINTS], 0);
  TextDrawSetProportional(gTextDraw[TD_ACTFINFOPOINTS], 1);
  TextDrawSetShadow(gTextDraw[TD_ACTFINFOPOINTS], 1);

  gTextDraw[TD_ACTFSTATETITLE] = TextDrawCreate(36.000000, 301.000000, "Status ~r~/CTF~w~:");
  TextDrawBackgroundColor(gTextDraw[TD_ACTFSTATETITLE], 255);
  TextDrawFont(gTextDraw[TD_ACTFSTATETITLE], 3);
  TextDrawLetterSize(gTextDraw[TD_ACTFSTATETITLE], 0.480000, 1.500000);
  TextDrawColor(gTextDraw[TD_ACTFSTATETITLE], -1);
  TextDrawSetOutline(gTextDraw[TD_ACTFSTATETITLE], 0);
  TextDrawSetProportional(gTextDraw[TD_ACTFSTATETITLE], 1);
  TextDrawSetShadow(gTextDraw[TD_ACTFSTATETITLE], 1);

  gTextDraw[TD_ACTFSTATETEXT] = TextDrawCreate(85.000000, 316.000000, "~y~Flaga na spawnie");
  TextDrawAlignment(gTextDraw[TD_ACTFSTATETEXT], 2);
  TextDrawBackgroundColor(gTextDraw[TD_ACTFSTATETEXT], 255);
  TextDrawFont(gTextDraw[TD_ACTFSTATETEXT], 3);
  TextDrawLetterSize(gTextDraw[TD_ACTFSTATETEXT], 0.350000, 1.100000);
  TextDrawColor(gTextDraw[TD_ACTFSTATETEXT], -1);
  TextDrawSetOutline(gTextDraw[TD_ACTFSTATETEXT], 0);
  TextDrawSetProportional(gTextDraw[TD_ACTFSTATETEXT], 1);
  TextDrawSetShadow(gTextDraw[TD_ACTFSTATETEXT], 1);

  tmp = 0.0;

  for(new i = TD_INFOBOX; i < TD_INFOBOX + 8; i++)
  {
    gTextDraw[i] = DrawRectangle(470.0 + tmp, 600.0 - tmp, 345.0 + tmp, 415.0 - tmp, 0x0000000A);
    tmp += 0.5;
  }

  gTextDraw[TD_ATTRACTIONBOX] = DrawRectangle(171.0, 299.0, 343.0, 417.0, 0x00000030);  // 171, 299, 343, 411 | szer start, szer end, wys start, wys end

  gTextDraw[TD_ATTRACTION_TITLES] = TextDrawCreate(173.0, 343.8, " "); // 343.8
  TextDrawLetterSize(gTextDraw[TD_ATTRACTION_TITLES], 0.20, 0.79);
  TextDrawColor(gTextDraw[TD_ATTRACTION_TITLES], 0x00FF20FF);
  TextDrawFont(gTextDraw[TD_ATTRACTION_TITLES], 2);
  TextDrawSetShadow(gTextDraw[TD_ATTRACTION_TITLES], 1);
  TextDrawSetOutline(gTextDraw[TD_ATTRACTION_TITLES], 0);
  TextDrawBackgroundColor(gTextDraw[TD_ATTRACTION_TITLES], 0x00000080);
  TextDrawAlignment(gTextDraw[TD_ATTRACTION_TITLES], 1);

  gTextDraw[TD_ATTRACTION_STATES] = TextDrawCreate(295.0, 343.8, " ");
  TextDrawLetterSize(gTextDraw[TD_ATTRACTION_STATES], 0.20, 0.79);
  TextDrawColor(gTextDraw[TD_ATTRACTION_STATES], 0x00FF20FF);
  TextDrawFont(gTextDraw[TD_ATTRACTION_STATES], 2);
  TextDrawSetShadow(gTextDraw[TD_ATTRACTION_STATES], 1);
  TextDrawSetOutline(gTextDraw[TD_ATTRACTION_STATES], 0);
  TextDrawBackgroundColor(gTextDraw[TD_ATTRACTION_STATES], 0x00000080);
  TextDrawAlignment(gTextDraw[TD_ATTRACTION_STATES], 3);

  gmTemp[fsaCurrentColor]=fsLogoColors[0];
  gTextDraw[TD_FULLSERVER] = TextDrawCreate(59.0, 420.0, "FullServer");
  TextDrawLetterSize(gTextDraw[TD_FULLSERVER], 0.69999, 1.80);
  TextDrawColor(gTextDraw[TD_FULLSERVER], gmTemp[fsaCurrentColor]);
  TextDrawFont(gTextDraw[TD_FULLSERVER], 0);
  TextDrawSetShadow(gTextDraw[TD_FULLSERVER], 1);
  TextDrawSetOutline(gTextDraw[TD_FULLSERVER], 0);
  TextDrawBackgroundColor(gTextDraw[TD_FULLSERVER], 0x000000FF);
  TextDrawAlignment(gTextDraw[TD_FULLSERVER], 2);

  gTextDraw[TD_FULLSERVERWEBSITE] = TextDrawCreate(59.0, 436.0, "WWW.FULLSERVER.EU");
  TextDrawLetterSize(gTextDraw[TD_FULLSERVERWEBSITE], 0.249999, 0.899999);
  TextDrawColor(gTextDraw[TD_FULLSERVERWEBSITE], 0xFFFFFFFF);
  TextDrawFont(gTextDraw[TD_FULLSERVERWEBSITE], 2);
  TextDrawSetShadow(gTextDraw[TD_FULLSERVERWEBSITE], 1);
  TextDrawSetOutline(gTextDraw[TD_FULLSERVERWEBSITE], 0);
  TextDrawBackgroundColor(gTextDraw[TD_FULLSERVERWEBSITE], 0x000000FF);
  TextDrawAlignment(gTextDraw[TD_FULLSERVERWEBSITE], 2);

  gTextDraw[TD_STAT] = TextDrawCreate(275.0, 429.0, __("Czas gry: Ping:     Respekt:  Skill:  GameP:"));
  TextDrawLetterSize(gTextDraw[TD_STAT], 0.24, 0.8);
  TextDrawColor(gTextDraw[TD_STAT], -112);
  TextDrawFont(gTextDraw[TD_STAT], 1);
  TextDrawSetOutline(gTextDraw[TD_STAT], 1);
  TextDrawSetProportional(gTextDraw[TD_STAT], 0);
  TextDrawBackgroundColor(gTextDraw[TD_STAT], 96);

  gTextDraw[TD_AREAFULLDM]=TextDrawCreate(554.000000, 5.000000, __("STREFA ~r~SMIERCI"));
  TextDrawAlignment(gTextDraw[TD_AREAFULLDM], 2);
  TextDrawBackgroundColor(gTextDraw[TD_AREAFULLDM], 255);
  TextDrawFont(gTextDraw[TD_AREAFULLDM], 3);
  TextDrawLetterSize(gTextDraw[TD_AREAFULLDM], 0.399998, 1.299998);
  TextDrawColor(gTextDraw[TD_AREAFULLDM], 0xffffffff);
  TextDrawSetOutline(gTextDraw[TD_AREAFULLDM], 1);
  TextDrawSetProportional(gTextDraw[TD_AREAFULLDM], 1);

  gTextDraw[TD_AREANODM]=TextDrawCreate(554.000000, 5.000000, __("STREFA ~b~~h~BEZ DM"));
  TextDrawAlignment(gTextDraw[TD_AREANODM], 2);
  TextDrawBackgroundColor(gTextDraw[TD_AREANODM], 255);
  TextDrawFont(gTextDraw[TD_AREANODM], 3);
  TextDrawLetterSize(gTextDraw[TD_AREANODM], 0.399998, 1.299998);
  TextDrawColor(gTextDraw[TD_AREANODM], 0xffffffff);
  TextDrawSetOutline(gTextDraw[TD_AREANODM], 1);
  TextDrawSetProportional(gTextDraw[TD_AREANODM], 1);

  gTextDraw[TD_WARNING] = TextDrawCreate(320.000000, 171.000000, __("~n~Otrzymano ostrzezenie od administratora!~n~~n~You have received a warning!~n~~n~"));
  TextDrawAlignment(gTextDraw[TD_WARNING], 2);
  TextDrawBackgroundColor(gTextDraw[TD_WARNING], 255);
  TextDrawFont(gTextDraw[TD_WARNING], 1);
  TextDrawLetterSize(gTextDraw[TD_WARNING], 0.559999, 2.299999);
  TextDrawColor(gTextDraw[TD_WARNING], -1);
  TextDrawSetOutline(gTextDraw[TD_WARNING], 0);
  TextDrawSetProportional(gTextDraw[TD_WARNING], 1);
  TextDrawSetShadow(gTextDraw[TD_WARNING], 2);
  TextDrawUseBox(gTextDraw[TD_WARNING], 1);
  TextDrawBoxColor(gTextDraw[TD_WARNING], -805306272);
  TextDrawTextSize(gTextDraw[TD_WARNING], 50.000000, 410.000000);
}
CreatePlayerTextDraws(playerid)
{
    pTextDraw[PTD_HPTEXT][playerid] = CreatePlayerTextDraw(playerid,552.000000, 46.000000, "100~n~~n~~n~~n~~r~~h~100");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_HPTEXT][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_HPTEXT][playerid], 1);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_HPTEXT][playerid], 0.149900, 0.610000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_HPTEXT][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_HPTEXT][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_HPTEXT][playerid], 0);

    pTextDraw[PTD_ARICON][playerid] = CreatePlayerTextDraw(playerid,551.000000, 44.500000, "hud:radar_tshirt");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_ARICON][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_ARICON][playerid], 4);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_ARICON][playerid], 0.100000, 1.000000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_ARICON][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_ARICON][playerid], 0);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_ARICON][playerid], 1);
    PlayerTextDrawSetShadow(playerid,pTextDraw[PTD_ARICON][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_ARICON][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_ARICON][playerid], 255);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_ARICON][playerid], -9.000000, 10.000000);

    pTextDraw[PTD_HPICON][playerid] = CreatePlayerTextDraw(playerid,551.000000, 66.500000, "hud:radar_girlfriend");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_HPICON][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_HPICON][playerid], 4);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_HPICON][playerid], 0.100000, 1.000000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_HPICON][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_HPICON][playerid], 0);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_HPICON][playerid], 1);
    PlayerTextDrawSetShadow(playerid,pTextDraw[PTD_HPICON][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_HPICON][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_HPICON][playerid], 255);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_HPICON][playerid], -8.000000, 9.000000);

    pTextDraw[PTD_NICK][playerid] = CreatePlayerTextDraw(playerid,195.000000, 431.000000, "_");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_NICK][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_NICK][playerid], 96);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_NICK][playerid], 1);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_NICK][playerid], 0.260000, 1.300000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_NICK][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_NICK][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_NICK][playerid], 1);

    pTextDraw[PTD_STAT][playerid] = CreatePlayerTextDraw(playerid,276.0, 436.0, "_ ");
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_STAT][playerid], 0.24, 1.1);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_STAT][playerid], 96);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_STAT][playerid], -1);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_STAT][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_STAT][playerid], 0);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_STAT][playerid], 0);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_STAT][playerid], 1);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_STAT][playerid],640.0,0.0);

    #define POS_TD_VEHICLEINFO(%0,%1) %0+532.000000, %1+386.000000
    pTextDraw[PTD_VEHICLEINFO][playerid] = CreatePlayerTextDraw(playerid,POS_TD_VEHICLEINFO(0,0), " "); //~r~~h~Helikopter policyjny~n~~b~~h~~h~~h~200 ~w~km/h    ~y~]]]~l~]]");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 2);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 0.360000, 1.600000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 0);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 1);
    PlayerTextDrawSetShadow(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 3145776);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_VEHICLEINFO][playerid], 394.000000, 209.000000);

    pTextDraw[PTD_FPS][playerid] = CreatePlayerTextDraw(playerid,620.000000, 411.000000, "--");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_FPS][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_FPS][playerid], 144);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_FPS][playerid], 3);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_FPS][playerid], 0.649999, 2.499999);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_FPS][playerid], -65281);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_FPS][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_FPS][playerid], 1);

    // attraction textdraws

    pTextDraw[PTD_ARACEBOX][playerid] = CreatePlayerTextDraw(playerid,POS_TD_RACE(108.000000, -16.000000), "~b~3/10    ~h~5/18");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_ARACEBOX][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_ARACEBOX][playerid], 48);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_ARACEBOX][playerid], 3);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_ARACEBOX][playerid], 0.689999, 2.400000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_ARACEBOX][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_ARACEBOX][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_ARACEBOX][playerid], 0);

    pTextDraw[PTD_ARACETIME][playerid] = CreatePlayerTextDraw(playerid,POS_TD_VEHICLEINFO(0,-13.000000), "Czas: ~y~~h~00.00s");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_ARACETIME][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_ARACETIME][playerid], 48);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_ARACETIME][playerid], 1);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_ARACETIME][playerid], 0.270000, 1.000000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_ARACETIME][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_ARACETIME][playerid], 0);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_ARACETIME][playerid], 1);
    PlayerTextDrawSetShadow(playerid,pTextDraw[PTD_ARACETIME][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_ARACETIME][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_ARACETIME][playerid], 96);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_ARACETIME][playerid], 394.000000, 209.000000);

    pTextDraw[PTD_ADRIFTBOX][playerid] = CreatePlayerTextDraw(playerid,536.000000, 288.000000, "~b~3/10    ~h~5/18");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], 48);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], 3);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], 0.689999, 2.400000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_ADRIFTBOX][playerid], 0);
    
    pTextDraw[PTD_AGGINFO][playerid] = CreatePlayerTextDraw(playerid,536.000000, 288.000000, "~b~Killi: ~r~0~n~~b~Level: ~r~1");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_AGGINFO][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_AGGINFO][playerid], 48);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_AGGINFO][playerid], 3);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_AGGINFO][playerid], 0.689999, 2.400000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_AGGINFO][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_AGGINFO][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_AGGINFO][playerid], 0);

    pTextDraw[PTD_ADRIFTTIME][playerid] = CreatePlayerTextDraw(playerid,POS_TD_VEHICLEINFO(0,-13.000000), "Czas: ~y~~h~00.00s");
    PlayerTextDrawAlignment(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 2);
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 48);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 1);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 0.270000, 1.000000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], -1);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 0);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 1);
    PlayerTextDrawSetShadow(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 96);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_ADRIFTTIME][playerid], 394.000000, 209.000000);

#if defined CJHIT_MODEL
    pTextDraw[PTD_CJHIT][playerid] = CreatePlayerTextDraw(playerid,69.000000, 317.000000, "_");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_CJHIT][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_CJHIT][playerid], 5);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_CJHIT][playerid], 0.500000, 1.000000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_CJHIT][playerid], 0xA1A1A1FF);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_CJHIT][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_CJHIT][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_CJHIT][playerid], 0xFFFFFF00);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_CJHIT][playerid], -93.000000, 107.000000);
    PlayerTextDrawBackgroundColor(playerid, pTextDraw[PTD_CJHIT][playerid], 0xFFFFFF00);
    PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT][playerid], 292);
    PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT][playerid]);
    
    pTextDraw[PTD_CJHIT2][playerid] = CreatePlayerTextDraw(playerid,69.000000, 317.000000, "_");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_CJHIT2][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_CJHIT2][playerid], 5);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_CJHIT2][playerid], 0.500000, 1.000000);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_CJHIT2][playerid], -16776961);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_CJHIT2][playerid], 1);
    PlayerTextDrawUseBox(playerid,pTextDraw[PTD_CJHIT2][playerid], 1);
    PlayerTextDrawBoxColor(playerid,pTextDraw[PTD_CJHIT2][playerid], 0xFFFFFF00);
    PlayerTextDrawTextSize(playerid,pTextDraw[PTD_CJHIT2][playerid], -93.000000, 107.000000);
    PlayerTextDrawBackgroundColor(playerid, pTextDraw[PTD_CJHIT2][playerid], 0xFFFFFF00);
    PlayerTextDrawSetPreviewModel(playerid, pTextDraw[PTD_CJHIT2][playerid], 292);
    PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT2][playerid]);

    pTextDraw[PTD_CJHIT_TXT][playerid] = CreatePlayerTextDraw(playerid,31.000000, 336.000000, "_");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], 1);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], 0.150000, 0.699999);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], -16776961);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], 1);
    PlayerTextDrawSetSelectable(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], 0);

    pTextDraw[PTD_CJHIT_HP][playerid] = CreatePlayerTextDraw(playerid,31.000000, 413.000000, "_");
    PlayerTextDrawBackgroundColor(playerid,pTextDraw[PTD_CJHIT_HP][playerid], 255);
    PlayerTextDrawFont(playerid,pTextDraw[PTD_CJHIT_HP][playerid], 1);
    PlayerTextDrawLetterSize(playerid,pTextDraw[PTD_CJHIT_HP][playerid], 0.150000, 0.699998);
    PlayerTextDrawColor(playerid,pTextDraw[PTD_CJHIT_HP][playerid], -16776961);
    PlayerTextDrawSetOutline(playerid,pTextDraw[PTD_CJHIT_HP][playerid], 1);
    PlayerTextDrawSetProportional(playerid,pTextDraw[PTD_CJHIT_HP][playerid], 1);
    PlayerTextDrawSetSelectable(playerid,pTextDraw[PTD_CJHIT_HP][playerid], 0);
#endif

  #undef POS_TD_RACE
  #undef POS_TD_VEHICLEINFO
}

#if defined CJHIT_MODEL
stock returnCJHITNormal(playerid)
{
    PlayerTextDrawHide(playerid, pTextDraw[PTD_CJHIT2][playerid]);
    PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_TXT][playerid], "_");
    PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT_TXT][playerid]);
    PlayerTextDrawSetString(playerid,pTextDraw[PTD_CJHIT_HP][playerid], "_");
    PlayerTextDrawShow(playerid, pTextDraw[PTD_CJHIT_HP][playerid]);
}
#endif
// EOF
