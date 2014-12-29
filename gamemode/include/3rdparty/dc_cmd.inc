/*
  Daniel's Command Processing Plugin, modified by FullServer.eu
  © 2012-2013, Daniel_Cortez
  © 2014, FullServer.eu

Description:  fast and compact command processor written in pure ASM.
License:    http://creativecommons.org/licenses/by-sa/3.0/
*/

#if defined __included
  #endinput
#endif
#define __included

native DC_CMD(playerid, cmdtext[]);

forward OnPlayerCommandReceived(playerid, cmdtext[]);
forward OnPlayerCommandPerformed(playerid, cmdtext[], success);

static
  bool:_g_HasOPCS = false,
  bool:_g_HasOPCE = false;

#define COMMAND:%1(%2)          \
      forward _%1(%2); \
      public _%1(%2)

#define CMD:%1(%2) \
      COMMAND:%1(%2)

#define command(%1,%2,%3) \
      COMMAND:%1(%2, %3)

#define cmd(%1,%2,%3) \
      COMMAND:%1(%2, %3)

#define DC_CMD:%1(%2) \
      COMMAND:%1(%2)

#if !defined isnull
  #define isnull(%1) \
        ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

#if defined FILTERSCRIPT

public OnFilterScriptInit()
{
  _g_HasOPCS = funcidx("OnPlayerCommandReceived") != -1;
  _g_HasOPCE = funcidx("OnPlayerCommandPerformed") != -1;
  return CallLocalFunction("zcmd_OnFilterScriptInit", "");
}

#if defined _ALS_OnFilterScriptInit
    #undef OnFilterScriptInit
#else
    #define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit zcmd_OnFilterScriptInit
forward zcmd_OnFilterScriptInit();

#else /*not a filterscript*/

public OnGameModeInit()
{
  _g_HasOPCS = funcidx("OnPlayerCommandReceived") != -1;
  _g_HasOPCE = funcidx("OnPlayerCommandPerformed") != -1;
  if (funcidx("zcmd_OnGameModeInit") != -1)
  {
    return CallLocalFunction("zcmd_OnGameModeInit", "");
  }
  return 1;
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit zcmd_OnGameModeInit
forward zcmd_OnGameModeInit();

#endif /*if defined FILTERSCRIPT*/

public OnPlayerCommandText(playerid, cmdtext[])
{
  if (_g_HasOPCS && !CallLocalFunction("OnPlayerCommandReceived", "is", playerid, cmdtext))
  {
    return 1;
  }

  static bool:success;
  success = false;
  if(DC_CMD(playerid, cmdtext)) success = true;

  if (_g_HasOPCE)
  {
    return CallLocalFunction("OnPlayerCommandPerformed", "isi", playerid, cmdtext, success);
  }
  return success;
}

#if defined _ALS_OnPlayerCommandText
    #undef OnPlayerCommandText
#else
    #define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText _OnPlayerCommandText
forward _OnPlayerCommandText(playerid, cmdtext[]);
// EOF