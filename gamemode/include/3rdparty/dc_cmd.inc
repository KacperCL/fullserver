/*
  Daniel's Command Processing Plugin, modified by FullServer.eu
  © 2012-2013, Daniel_Cortez
  © 2014-2015, FullServer.eu

Description:  fast and compact command processor written in pure ASM.
License:    http://creativecommons.org/licenses/by-sa/3.0/
*/

native DC_CMD(playerid, cmdtext[]);

forward OnPlayerCommandReceived(playerid, cmdtext[]);
forward OnPlayerCommandPerformed(playerid, cmdtext[], success);

#define CMD:%1(%2) forward _%1(%2); public _%1(%2)		
#define COMMAND:%1(%2) CMD:%1(%2)
#define DC_CMD:%1(%2) CMD:%1(%2)

#if !defined isnull
  #define isnull(%1) \
        ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

static
  bool:hasOPCR = false,
  bool:hasOPCP = false,
  bool:success = false;

#if defined FILTERSCRIPT

public OnFilterScriptInit()
{
  hasOPCR = funcidx("OnPlayerCommandReceived") != -1;
  hasOPCP = funcidx("OnPlayerCommandPerformed") != -1;
  if (funcidx("DC_OnFilterScriptInit") != -1)
  {
    return CallLocalFunction("DC_OnFilterScriptInit", "");
  }
  return 1;
}

#if defined _ALS_OnFilterScriptInit
    #undef OnFilterScriptInit
#else
    #define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit DC_OnFilterScriptInit
forward DC_OnFilterScriptInit();

#else /*not a filterscript*/

public OnGameModeInit()
{
  hasOPCR = funcidx("OnPlayerCommandReceived") != -1;
  hasOPCP = funcidx("OnPlayerCommandPerformed") != -1;
  if (funcidx("DC_OnGameModeInit") != -1)
  {
    return CallLocalFunction("DC_OnGameModeInit", "");
  }
  return 1;
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit DC_OnGameModeInit
forward DC_OnGameModeInit();

#endif /*if defined FILTERSCRIPT*/

public OnPlayerCommandText(playerid, cmdtext[])
{
  // check if allowed to execute command
  if(hasOPCR && !CallLocalFunction("OnPlayerCommandReceived", "is", playerid, cmdtext) && !defined FILTERSCRIPT)
  {
    // dont allow fs to lock out commands since it will lock out all other fses and gm as well!
    return 1;
  }
  
  // execute it
  if(DC_CMD(playerid, cmdtext)) 
  {
    success = true;
  }
  else
  {
    success = false;
  }

  // inform script that command was executed
  if(hasOPCP)
  {
    // dont allow fs to return any value if command was not found (force it to return 0 thus forwarding command to next fs/mode)
    if(!defined FILTERSCRIPT && !success)
    {
      return CallLocalFunction("OnPlayerCommandPerformed", "isi", playerid, cmdtext, success);
    }
    else
    {
      CallLocalFunction("OnPlayerCommandPerformed", "isi", playerid, cmdtext, success);
    }
    
  }
  return success;
}

#if defined _ALS_OnPlayerCommandText
    #undef OnPlayerCommandText
#else
    #define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText DC_OnPlayerCommandText
forward DC_OnPlayerCommandText(playerid, cmdtext[]);
// EOF