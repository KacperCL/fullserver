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

bool:ContainsIP(const string[])
{
  static RegEx:rCIP;

  if ( !rCIP ) rCIP = regex_build(".*[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,7}[0-9]{3,5}.*");

  if (regex_match_exid(string, rCIP)) return true;
  else return false;
}

bool:NinjaBan(const string[])
{
   #pragma unused string
   return false;
}

bool:IsValidEmail(const string[])
{
  static RegEx:rEMAIL;

  if ( !rEMAIL ) rEMAIL = regex_build("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}$");

  if (regex_match_exid(string, rEMAIL)) return true;
  else return false;
}

bool:IsNumeric(const string[])
{
  static RegEx:rNUMER;

  if ( !rNUMER ) rNUMER = regex_build("^[0-9]+$");

  if (regex_match_exid(string, rNUMER)) return true;
  else return false;
}

bool:IsNickCorrect(szNick[])
{
  static RegEx:rNICK;

  if ( !rNICK ) rNICK = regex_build("^[a-zA-Z0-9_$@()=\\[\\]]{3,20}+$");

  if (regex_match_exid(szNick, rNICK)) return true;
  else return false;
}

bool:IsPasswordCorrect(szPassword[])
{
  static RegEx:rPASS;

  if ( !rPASS ) rPASS = regex_build("^[a-zA-Z0-9!@#\\$%\\^&*()_+-\\[\\];\\\\<>,\\.\\/?`~\"]{5,30}+$");

  if (regex_match_exid(szPassword, rPASS)) return true;
  else return false;
}

bool:IsHex(szHex[])
{
  static RegEx:rHEX;

  if ( !rHEX ) rHEX = regex_build("^[0-9A-Fa-f]{1,6}+$");

  if (regex_match_exid(szHex, rHEX)) return true;
  else return false;
}

bool:IsCorrectWordForCensorship(szWord[])
{
  static RegEx:rWORD;

  if ( !rWORD ) rWORD = regex_build("^[0-9A-Za-z]{1,30}+$");

  if (regex_match_exid(szWord, rWORD)) return true;
  else return false;
}