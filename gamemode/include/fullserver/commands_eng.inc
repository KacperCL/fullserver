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

CMD:help(playerid)
  return _pomoc(playerid);

CMD:newnick(playerid, params[])
  return _nowynick(playerid, params);

CMD:k(playerid, params[])
  return _kick(playerid, params);

CMD:lastskin(playerid)
  return _mojskin(playerid);

CMD:ms(playerid)
  return _mojskin(playerid);

CMD:teleports(playerid)
  return _teleporty(playerid);

CMD:stat(playerid)
  return _staty(playerid);

CMD:stats(playerid)
  return _staty(playerid);

CMD:toplista(playerid)
  return _top(playerid);

CMD:toplist(playerid)
  return _top(playerid);

CMD:zamknij(playerid)
  return _lock(playerid);

CMD:otworz(playerid)
  return _unlock(playerid);

CMD:zlecenie(playerid, params[])
  return _hitman(playerid, params);

CMD:dajkase(playerid, params[])
  return _givecash(playerid, params);

CMD:komendy(playerid)
  return _cmd(playerid);

CMD:tdpanel(playerid)
  return _hud(playerid);

CMD:newpassword(playerid, params[])
  return _zmienhaslo(playerid, params);

CMD:cz(playerid)
  return _clearchat(playerid);

CMD:report(playerid, params[])
  return _raport(playerid, params);

CMD:admini(playerid)
  return _admins(playerid);

CMD:vtp(playerid, params[])
  return _tp(playerid, params);

CMD:vipy(playerid)
  return _vips(playerid);

CMD:authors(playerid)
  return _credits(playerid);

CMD:author(playerid)
  return _credits(playerid);

CMD:autor(playerid)
  return _credits(playerid);

CMD:autorzy(playerid)
  return _credits(playerid);

CMD:respect(playerid)
  return _respekt(playerid);

CMD:church(playerid)
  return _kosciol(playerid);

CMD:parachute(playerid)
  return _spadochron(playerid);

CMD:ch(playerid)
  return _chowany(playerid);

CMD:chilliad(playerid)
  return _chiliad(playerid);

CMD:pustynia(playerid)
  return _stlot(playerid);

CMD:ulecz(playerid)
  return _100hp(playerid);

CMD:strzelnicaexit(playerid)
  return _strzelexit(playerid);

CMD:chowanyexit(playerid)
  return _chexit(playerid);

CMD:wojsko2(playerid)
  return _wietnam(playerid);

CMD:wojsko3(playerid)
  return _marynarka(playerid);

CMD:afganistan(playerid)
  return _wojsko(playerid);

CMD:poczta(playerid)
  return _bank(playerid);

CMD:sf3(playerid)
  return _salon(playerid);

CMD:warsztat(playerid)
  return _warsztatlv(playerid);

CMD:komisariat(playerid)
  return _pd(playerid);

CMD:ztp(playerid)
  return _ksw(playerid);

CMD:wwk(playerid)
  return _ksw(playerid);

CMD:lc(playerid)
  return _libertycity(playerid);
  
CMD:reg(playerid)
  return _rejestracja(playerid);
  
CMD:rej(playerid)
  return _rejestracja(playerid);
  
CMD:register(playerid)
  return _rejestracja(playerid);
  
CMD:rejestruj(playerid)
  return _rejestracja(playerid);
  
CMD:setplay(playerid)
  return _audio(playerid);

CMD:gungame(playerid)
  return _gg(playerid);
  
CMD:gungameexit(playerid)
  return _ggexit(playerid);
  
CMD:driftingexit(playerid)
  return _driftexit(playerid);
  
CMD:jetmax(playerid)
  return _molo(playerid);
// EOF