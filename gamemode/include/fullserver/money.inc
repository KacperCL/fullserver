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

forward GivePlayerMoneyEx(playerid,money);
public GivePlayerMoneyEx(playerid,money)
  return AC_GivePlayerMoney(playerid,money);

forward GetPlayerMoneyEx(playerid);
public GetPlayerMoneyEx(playerid)
  return AC_GetPlayerMoney(playerid);

stock AC_GivePlayerMoney(playerid, money, bool:transfer=false)
{
#pragma unused transfer
  if(!IsPlayerConnected(playerid)) return 0;
  if (GetPlayerMoney(playerid)!=pData[playerid][pmoney]) {
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid,pData[playerid][pmoney]);
  }
    if(GetPlayerMoney(playerid)+money>800000)
  {
    if(!pData[playerid][loggedIn]) Msg(playerid,COLOR_ERROR,"Nie mozesz przechowywaæ w portfelu wiêcej ni¿ {b}$800000{/b}. Wplaæ obecn¹ kwotê do banku - znajdziesz go na {b}/bank{/b.");
    if(pData[playerid][loggedIn]) {
      if(money>2100000000)
      {
        Msg(playerid,COLOR_ERROR,"Wystapil nieoczekiwany wyjatek i powinienes to zglosic! Kod bledu: #AUTO_TRANSFER_MAX_AMOUNT");
        return 0;
      }
      if((StringToInt(GetPlayerAccountData(playerid, "bank_money")) + money)>2100000000)
      {
          Msg(playerid,COLOR_ERROR,"Nie mozesz przechowywaæ w portfelu wiêcej ni¿ {b}$800000{/b}. Niestety Twoje konto bankowe jest takze przepelnione :(");
          return 0;
      }

      AddPlayerAccountData(playerid, "bank_money", money);

      Msg(playerid,COLOR_ERROR,"Nie mozesz przechowywaæ w portfelu wiêcej ni¿ {b}$800000{/b}. Nie mieszczace sie pieniadze zostaly wplacone do {b}/bank{/b}u.");
    }
    return 0;
  }

  // if(pTemp[playerid][doubleMode] && !transfer && money>0) money=money*2;

  pData[playerid][pmoney]+=money;

    return GivePlayerMoney(playerid, money);
}

stock AC_ResetPlayerMoney(playerid)
{
  pData[playerid][pmoney]=0;
  return ResetPlayerMoney(playerid);
}

stock AC_GetPlayerMoney(playerid)
{
  if (GetPlayerMoney(playerid)!=pData[playerid][pmoney]) {
    if (floatabs(GetPlayerMoney(playerid)-pData[playerid][pmoney])>=199999 && !pTemp[playerid][dead]) {
      new pstate=GetPlayerState(playerid);
      if ((pstate==PLAYER_STATE_ONFOOT || pstate==PLAYER_STATE_DRIVER || pstate == PLAYER_STATE_PASSENGER)) {
        new buf[128];
        format(buf, sizeof buf, __("Prawdopodobny money hack: %s (%d) ma %d$ a powinien %d$"), GetPlayerNick(playerid), playerid, GetPlayerMoney(playerid), pData[playerid][pmoney]);
        MSGToAdmins(COLOR_INFO2, buf, false, LEVEL_ADMIN1);
      }

    }
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid,pData[playerid][pmoney]);
  }
  return pData[playerid][pmoney];
}

stock SetPlayerMoney(playerid, money)
{
    AC_ResetPlayerMoney(playerid);
    AC_GivePlayerMoney(playerid, money);
}

#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney AC_GivePlayerMoney

#if defined _ALS_ResetPlayerMoney
    #undef ResetPlayerMoney
#else
    #define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney AC_ResetPlayerMoney

#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney AC_GetPlayerMoney
// EOF
