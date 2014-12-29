/*
  Author: bartekdvd
  Thanks to: Y_Less
*/

/*
Example usage:

public sss = 0;
#pragma unused sss

main()
{
  amxFindAMXInfoPointer();
  printf("%s %x %x", amxGetAMXName(), amxGetAMXPointer(), amxGetHeaderPointer()); //current amx
  printf("%s %x %x", amxGetGMName(), amxGetGMAMXPointer(), amxGetGMHeaderPointer()); //gamemode amx
  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
  {
      new name[255];
      name = amxGetFSName(i);
      if (name[i])
        printf("%d %s %x %x",
        i,
        amxGetFSName(i),
        amxGetFSAMXPointerIDX(i),
        amxGetFSHeaderPointerIDX(i)
      ); //fs amx
  }

  amxSetPubVar("sss", 123); //set public variable sss to 123 - it can be in any amx (filterscript or gamemode)
  printf("sss = %d", amxGetPubVar("sss"));

  return 1;
}
*/

#if !defined amx_included

#define amx_included

#include <a_samp>

#define MAX_FILTERSCRIPTS 16

enum AMX
{
  AMX_BASE,
  AMX_DATA,
  AMX_CALLBACK,
  AMX_DEBUG,
  AMX_CIP,
  AMX_FRM,
  AMX_STK,
  AMX_STP,
  AMX_FLAGS,
  AMX_USERTAGS[4],
  AMX_USERDATA[4],
  AMX_ERROR,
  AMX_PARAMCOUNT
};

enum AMX_HEADER
{
  AMX_SIZE,
  AMX_MAGIC16_FILEVER8_AMXVER8,
  AMX_FLAGS16_DEFSIZE16,
  AMX_COD,
  AMX_DAT,
  AMX_HEA,
  AMX_STP,
  AMX_CIP,
  AMX_PUBLICS,
  AMX_NATIVES,
  AMX_LIBRARIES,
  AMX_PUBVARS,
  AMX_TAGS,
  AMX_NAMETABLE
};

#define _AMX_GETMAGIC(%0)     ((%0)[AMX_MAGIC16_FILEVER8_AMXVER8] & 0xffff)
#define _AMX_GETFILEVERSION(%0)   (((%0)[AMX_MAGIC16_FILEVER8_AMXVER8] & 0x00ff0000) >>> 16)
#define _AMX_GETAMXVERSION(%0)    (((%0)[AMX_MAGIC16_FILEVER8_AMXVER8] & 0xff000000) >>> 24)
#define _AMX_GETFLAGS(%0)         ((%0)[AMX_FLAGS16_DEFSIZE16] & 0xffff)
#define _AMX_GETDEFSIZE(%0)       ((%0)[AMX_FLAGS16_DEFSIZE16] >>> 16)

stock AMX_GETMAGIC(amxHeader[AMX_HEADER])
  return _AMX_GETMAGIC(amxHeader);
stock AMX_GETFILEVERSION(amxHeader[AMX_HEADER])
  return _AMX_GETFILEVERSION(amxHeader);
stock AMX_GETAMXVERSION(amxHeader[AMX_HEADER])
  return _AMX_GETAMXVERSION(amxHeader);
stock AMX_GETFLAGS(amxHeader[AMX_HEADER])
  return _AMX_GETFLAGS(amxHeader);
stock AMX_GETDEFSIZE(amxHeader[AMX_HEADER])
  return _AMX_GETDEFSIZE(amxHeader);

enum AMX_ENTRY
{
    AMX_ENTRY_ADDRESS,
    AMX_ENTRY_NAMEOFS
};

#define AMX_NUMENTRIES(%0,%1) (((%0)[(%1) + AMX_HEADER:1] - (%0)[(%1)]) / (_AMX_GETDEFSIZE(%0) > 0 ? _AMX_GETDEFSIZE(%0) : 0x7fffffff))

stock amxGetVariableAt(offset)
{
  #emit LCTRL 1
  #emit LOAD.S.ALT offset
  #emit SUB
  #emit NEG
  #emit STOR.S.PRI offset
  #emit LREF.S.PRI offset
  #emit STOR.S.PRI offset

  return offset;
}

stock amxSetVariableAt(offset, val)
{
  #emit LCTRL 1
  #emit LOAD.S.ALT offset
  #emit SUB
  #emit NEG
  #emit STOR.S.PRI offset
  #emit LOAD.S.PRI val
  #emit SREF.S.PRI offset
}

stock amxGetHeader()
{
  new amxheader[AMX_HEADER];
  for (new it = 0, it4 = 0; it < _:AMX_HEADER; it++, it4 += 4)
    amxheader[AMX_HEADER:it] = amxGetVariableAt(it4);
  return amxheader;
}

stock amxGetEntry(amxHeader[AMX_HEADER], AMX_HEADER:table, index)
{
  new entry[AMX_ENTRY];

  if (_:table < _:AMX_HEADER)
  {
    new entryOffset = amxHeader[table] + (index * _AMX_GETDEFSIZE(amxHeader));
    entry[AMX_ENTRY_ADDRESS] = amxGetVariableAt(entryOffset);
    entry[AMX_ENTRY_NAMEOFS] = amxGetVariableAt(entryOffset + 4);
  }

  return entry;
}

stock amxGetEntryName(amxEntry[AMX_ENTRY])
{
  new name[8];
  for (new i = 0, offset = amxEntry[AMX_ENTRY_NAMEOFS]; i < sizeof(name); i++, offset += 4)
        name[i] = swapchars(amxGetVariableAt(offset));

  new outName[32];

  for (new i = 0; i < sizeof(outName); i++)
    outName[i] = name{i};

  return outName;
}

stock amxGetHeaderPointer() //Get memory pointer that points to the start of the current AMX Header
{
  new pHeader, offset;

  #emit LCTRL 0
  #emit MOVE.ALT
  #emit LCTRL 6
  #emit ADD.C 0x2C
  #emit ADD
  #emit STOR.S.PRI offset

  pHeader = amxGetVariableAt(offset);

  #emit CONST.ALT amxGetVariableAt
  #emit LCTRL 0
  #emit ADD
  #emit LOAD.S.ALT pHeader
  #emit SUB.ALT
  #emit STOR.S.PRI pHeader

  return pHeader;
}

stock amxGetVariableAtPtr(base, pointer)
{
  #emit LCTRL 1
  #emit LOAD.S.ALT base
  #emit ADD
  #emit NEG
  #emit LOAD.S.ALT pointer
  #emit ADD
  #emit STOR.S.PRI pointer
  #emit LREF.S.PRI pointer
  #emit STOR.S.PRI pointer

  return pointer;
}

stock amxSetVariableAtPtr(base, pointer, var)
{
  #emit LCTRL 1
  #emit LOAD.S.ALT base
  #emit ADD
  #emit NEG
  #emit LOAD.S.ALT pointer
  #emit ADD
  #emit STOR.S.PRI pointer
  #emit LOAD.S.PRI var
  #emit SREF.S.PRI pointer
}

stock amxGetHeaderPtr(pHeader)
{
  new base = amxGetHeaderPointer();
  new amxheader[AMX_HEADER];
  for (new it = 0, it4 = 0; it < _:AMX_HEADER; it++, it4 += 4)
    amxheader[AMX_HEADER:it] = amxGetVariableAtPtr(base, pHeader + it4);
  return amxheader;
}

stock amxGetEntryPtr(pHeader, amxHeader[AMX_HEADER], AMX_HEADER:table, index)
{
  new base = amxGetHeaderPointer();
  new entry[AMX_ENTRY];

  if (_:table < _:AMX_HEADER)
  {
    new entryOffset = amxHeader[table] + (index * _AMX_GETDEFSIZE(amxHeader));
    entry[AMX_ENTRY_ADDRESS] = amxGetVariableAtPtr(base, pHeader + entryOffset);
    entry[AMX_ENTRY_NAMEOFS] = amxGetVariableAtPtr(base, pHeader + entryOffset + 4);
  }

  return entry;
}

stock amxGetEntryNamePtr(pHeader, amxEntry[AMX_ENTRY])
{
  new base = amxGetHeaderPointer();
  new name[8];
  for (new i = 0, offset = amxEntry[AMX_ENTRY_NAMEOFS]; i < sizeof(name); i++, offset += 4)
      name[i] = swapchars(amxGetVariableAtPtr(base, pHeader + offset));

  new outName[32];

  for (new i = 0; i < sizeof(outName); i++)
    outName[i] = name{i};

  return outName;
}

stock static const linuxBase  =   0x08048000;
stock static const windowsBase =  0x00400000;

enum e_OS
{
  UNKNOWN,
  WINDOWS,
  LINUX
};

stock e_OS:amxGetOS()
{
  static e_OS:os = UNKNOWN;

  if (os == UNKNOWN)
  {
    goto afterSCM;
    SendClientMessage(0,0,"");
afterSCM:

    new header[AMX_HEADER];
    new entry[AMX_ENTRY];
    header = amxGetHeader();
    entry = amxGetEntry(header, AMX_NATIVES, 0);

    if (!(entry[AMX_ENTRY_ADDRESS] > linuxBase))
      os = WINDOWS;
    else
      os = LINUX;
  }

  return os;
}

stock amxFindAMXInfoPointer_unsafe()
{
  static pAMXInfoAddress = 0;
  if (pAMXInfoAddress == 0)
  {
    print("  AMXInfo start scan...");
    new address, ppAMXInfo, tmpVal, pBase = amxGetHeaderPointer();
    if (amxGetOS() == WINDOWS)
    {
      print("  Detected OS: Windows");

      address = windowsBase + 0x50000;

      while (ppAMXInfo == 0)
      {
        tmpVal = amxGetVariableAtPtr(pBase, address);
        if (tmpVal == 0x656D6167)
        {
          if (amxGetVariableAtPtr(pBase, address + 4) == 0x65646F6D)
          {
            tmpVal = amxGetVariableAtPtr(pBase, address + 8);
            if ((tmpVal&0xFF) == 's' && ((tmpVal&0xFF00)>>>8) == '/' && ((tmpVal&0xFF0000)>>>16) != '%')
              if (amxGetVariableAtPtr(pBase, address - 76) != 0)
                ppAMXInfo = address - 76;
          }
        }
        address += 4;
      }
    }
    else
    {
      printf("  Detected OS: Linux");

      address = linuxBase + 0x80000;

      while (ppAMXInfo == 0)
      {
        tmpVal = amxGetVariableAtPtr(pBase, address);

        if (tmpVal == 0x656D6167)
        {
          if (amxGetVariableAtPtr(pBase, address + 4) == 0x65646F6D)
          {
            tmpVal = amxGetVariableAtPtr(pBase, address + 8);
            if ((tmpVal&0xFF) == 's' && ((tmpVal&0xFF00)>>>8) == '/' && ((tmpVal&0xFF0000)>>>16) != '%')
              if (amxGetVariableAtPtr(pBase, address - 56) != 0)
                ppAMXInfo = address - 56;
          }
        }
        address += 4;
      }
    }
    printf("  AMXInfo scan finish: (%x)->(%x)", ppAMXInfo, amxGetVariableAtPtr(pBase, ppAMXInfo));

    pAMXInfoAddress = ppAMXInfo;
  }

  return pAMXInfoAddress;
}

stock amxFindAMXInfoPointer()
{
  static pAMXInfoAddress = 0;
  if (pAMXInfoAddress == 0)
  {
    new header[AMX_HEADER], entry[AMX_ENTRY];
    header = amxGetHeader();
    new num = AMX_NUMENTRIES(header, AMX_NATIVES);

    for (new i = 0; i < num; i++)
    {
      entry = amxGetEntry(header, AMX_NATIVES, i);
      if (strcmp(amxGetEntryName(entry), "SendClientMessage") == 0)
        pAMXInfoAddress = entry[AMX_ENTRY_ADDRESS];
    }

    if (pAMXInfoAddress)
    {
      if (amxGetOS() == WINDOWS)
        pAMXInfoAddress = amxGetVariableAtPtr(amxGetHeaderPointer(), pAMXInfoAddress + 7);
      else
        pAMXInfoAddress = amxGetVariableAtPtr(amxGetHeaderPointer(), pAMXInfoAddress + 16);
    }
    else
      pAMXInfoAddress = amxFindAMXInfoPointer_unsafe();
  }

  return pAMXInfoAddress;
}

stock amxGetFSTablePointer()
{
  new pAMXInfo = amxFindAMXInfoPointer();
  new base = amxGetHeaderPointer();
  new ptr = amxGetVariableAtPtr(base, pAMXInfo);
  if (ptr)
    return amxGetVariableAtPtr(base, ptr + 4);
  return 0;
}

stock amxGetFSCount()
{
  new ptr = amxGetFSTablePointer();
  if (ptr)
    return amxGetVariableAtPtr(amxGetHeaderPointer(), ptr + 0x1030);
  return 0;
}

stock amxGetGMName()
{
  new outName[255];
  new name[64];
  new base = amxGetHeaderPointer();
  new ptr = amxFindAMXInfoPointer() + (amxGetOS() == WINDOWS ? 76 : 56);
  if (ptr)
  {
    for (new i = 0; i < sizeof(name); i++)
    {
      name[i] = swapchars(amxGetVariableAtPtr(base, ptr + (i*4)));
      if ((name[i]&0xFF) == 0 || (name[i]&0xFF00) == 0 || (name[i]&0xFF0000) == 0 || (name[i]&0xFF000000) == 0)
        i = sizeof(name);
    }

    new it, i, bool:zerodot;

    while (name{it}) it++;

    if (it > 0)
      it--;

    while (it > -1 && name{it} != '/' && name{it} != '\\')
    {
      if (!zerodot && name{it} == '.')
        name{it} = 0;
      it--;
    }
    it++;

    while (name{it})
    {
      outName[i] = name{it};
      it++; i++;
    }
  }

  return outName;
}

stock amxGetFSName(idx)
{
  new outName[255];
  new name[64];
  new base = amxGetHeaderPointer();
  new ptr = amxGetFSTablePointer();
  if (ptr)
  {
    for (new i = 0; i < sizeof(name); i++)
    {
      name[i] = swapchars(amxGetVariableAtPtr(base, ptr + 0x40 + (idx * sizeof(outName)) + (i*4)));
      if ((name[i]&0xFF) == 0 || (name[i]&0xFF00) == 0 || (name[i]&0xFF0000) == 0 || (name[i]&0xFF000000) == 0)
        i = sizeof(name);
    }

    new it, i;

    while (name{it}) it++;

    if (it > 0)
      it--;

    while (it > -1 && name{it} != '/' && name{it} != '\\') it--;

    it++;

    while (name{it})
    {
      outName[i] = name{it};
      it++; i++;
    }
  }

  return outName;
}

stock amxGetFSAMXPointerIDX(idx)
{
  new ptr = amxGetFSTablePointer();
  if (ptr)
    return amxGetVariableAtPtr(amxGetHeaderPointer(), ptr + idx*4);
  return 0;
}

stock amxGetFSAMXPointer(name[])
{
  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
  {
    if (strcmp(name, amxGetFSName(i), true) == 0)
      return amxGetFSAMXPointerIDX(i);
  }
  return 0;
}

stock amxGetGMAMXPointer()
{
  new pAMXInfo = amxFindAMXInfoPointer();
  new base = amxGetHeaderPointer();
  new ptr = amxGetVariableAtPtr(base, pAMXInfo);
  if (ptr)
    return amxGetVariableAtPtr(base, ptr);
  return ptr;
}

stock amxGetFSHeaderPointerIDX(idx)
{
  new ptr = amxGetFSAMXPointerIDX(idx);
  if (ptr)
    return amxGetVariableAtPtr(amxGetHeaderPointer(), ptr);
  return 0;
}

stock amxGetFSHeaderPointer(name[])
{
  new ptr = amxGetFSAMXPointer(name);
  if (ptr)
    return amxGetVariableAtPtr(amxGetHeaderPointer(), ptr);
  return 0;
}

stock amxGetGMHeaderPointer()
{
  new ptr = amxGetGMAMXPointer();
  if (ptr)
    return amxGetVariableAtPtr(amxGetHeaderPointer(), ptr);
  return 0;
}

stock amxGetAMXPointer()
{
  new base = amxGetHeaderPointer();
  new ptr = amxGetGMAMXPointer();
  if (ptr)
    if (base == amxGetVariableAtPtr(base, ptr))
      return ptr;

  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
  {
    ptr = amxGetFSAMXPointerIDX(i);
    if (ptr)
      if (base == amxGetVariableAtPtr(base, ptr))
        return ptr;
  }

  return 0;
}

stock amxGetAMXName()
{
  new base = amxGetHeaderPointer();
  new ptr = amxGetGMAMXPointer();
  new name[255];
  if (ptr)
    if (base == amxGetVariableAtPtr(base, ptr))
    {
      name = amxGetGMName();
      return name;
    }

  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
  {
    ptr = amxGetFSAMXPointerIDX(i);
    if (ptr)
      if (base == amxGetVariableAtPtr(base, ptr))
      {
        name = amxGetFSName(i);
        return name;
      }
  }

  return name;
}

stock amxGetPubVarInGM(pubVarName[])
{
  new header[AMX_HEADER], entry[AMX_ENTRY], pHeader = amxGetGMHeaderPointer();
  if (pHeader)
  {
    header = amxGetHeaderPtr(pHeader);
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
      {
        new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        return amxGetVariableAtPtr(amxGetHeaderPointer(), pVar);
      }
    }
  }
  return 0;
}

stock amxSetPubVarInGM(pubVarName[], val)
{
  new header[AMX_HEADER], entry[AMX_ENTRY], pHeader = amxGetGMHeaderPointer();
  if (pHeader)
  {
    header = amxGetHeaderPtr(pHeader);
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
      {
        new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        amxSetVariableAtPtr(amxGetHeaderPointer(), pVar, val);
        return 1;
      }
    }
  }
  return 0;
}

stock amxGetPubVarInFSIDX(idx, pubVarName[])
{
  new header[AMX_HEADER], entry[AMX_ENTRY], pHeader = amxGetFSHeaderPointerIDX(idx);

  if (pHeader)
  {
    header = amxGetHeaderPtr(pHeader);
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
      {
        new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        return amxGetVariableAtPtr(amxGetHeaderPointer(), pVar);
      }
    }
  }
  return 0;
}

stock amxGetPubVarInFS(amxName[], pubVarName[])
{
  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
    if (strcmp(amxName, amxGetFSName(i), true) == 0)
      return amxGetPubVarInFSIDX(i, pubVarName);
  return 0;
}

stock amxSetPubVarInFSIDX(idx, pubVarName[], val)
{
  new header[AMX_HEADER], entry[AMX_ENTRY], pHeader = amxGetFSHeaderPointerIDX(idx);

  if (pHeader)
  {
    header = amxGetHeaderPtr(pHeader);
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
      {
        new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        amxSetVariableAtPtr(amxGetHeaderPointer(), pVar, val);
        return 1;
      }
    }
  }
  return 0;
}

stock amxSetPubVarInFS(amxName[], pubVarName[], val)
{
  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
    if (strcmp(amxName, amxGetFSName(i), true) == 0)
      return amxSetPubVarInFSIDX(i, pubVarName, val);
  return 0;
}

stock amxGetPubVar(pubVarName[], bool:ignoreCurrent = false)
{
  new header[AMX_HEADER], entry[AMX_ENTRY], pBase = amxGetHeaderPointer();

  if (!ignoreCurrent)
  {
    header = amxGetHeader();
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntry(header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryName(entry), pubVarName) == 0)
      {
        new offset = header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        return amxGetVariableAt(offset);
      }
    }
  }

  new pHeader = amxGetGMHeaderPointer();

  if (pHeader != pBase && pHeader)
  {
    header = amxGetHeaderPtr(pHeader);
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
      {
        new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        return amxGetVariableAtPtr(pBase, pVar);
      }
    }
  }

  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
  {
    pHeader = amxGetFSHeaderPointerIDX(i);
    if (pHeader != pBase && pHeader)
    {
      header = amxGetHeaderPtr(pHeader);
      new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

      for (new j = 0; j < num; j++)
      {
        entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
        if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
        {
          new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
          return amxGetVariableAtPtr(pBase, pVar);
        }
      }
    }
  }

  return 0;
}

stock amxSetPubVar(pubVarName[], val, bool:ignoreCurrent = false)
{
  new header[AMX_HEADER], entry[AMX_ENTRY], pBase = amxGetHeaderPointer();

  if (!ignoreCurrent)
  {
    header = amxGetHeader();
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntry(header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryName(entry), pubVarName) == 0)
      {
        new offset = header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        amxSetVariableAt(offset, val);
        return 1;
      }
    }
  }

  new pHeader = amxGetGMHeaderPointer();

  if (pHeader != pBase && pHeader)
  {
    header = amxGetHeaderPtr(pHeader);
    new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

    for (new j = 0; j < num; j++)
    {
      entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
      if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
      {
        new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
        amxSetVariableAtPtr(pBase, pVar, val);
        return 1;
      }
    }
  }

  for (new i = 0; i < MAX_FILTERSCRIPTS; i++)
  {
    pHeader = amxGetFSHeaderPointerIDX(i);
    if (pHeader != pBase && pHeader)
    {
      header = amxGetHeaderPtr(pHeader);
      new num = AMX_NUMENTRIES(header, AMX_PUBVARS);

      for (new j = 0; j < num; j++)
      {
        entry = amxGetEntryPtr(pHeader, header, AMX_PUBVARS, j);
        if (strcmp(amxGetEntryNamePtr(pHeader, entry), pubVarName) == 0)
        {
          new pVar = pHeader + header[AMX_DAT] + entry[AMX_ENTRY_ADDRESS];
          amxSetVariableAtPtr(pBase, pVar, val);
          return 1;
        }
      }
    }
  }

  return 0;
}

stock amxCall(amxEntry[AMX_ENTRY], fmt[], {Float,_}:...)
{
  new numArgs = numargs(), it;
  for (it = numArgs - 1; it > 1; it--)
  {
    switch (fmt[it - 2])
    {
      case 'd', 'i', 'f':
      {
        #emit ADDR.ALT amxEntry
        #emit LOAD.S.PRI it
        #emit SHL.C.PRI 2
        #emit ADD
        #emit LOAD.I
        #emit LOAD.I
        #emit PUSH.PRI
      }

      default:
      {
          #emit ADDR.ALT amxEntry
        #emit LOAD.S.PRI it
        #emit SHL.C.PRI 2
        #emit ADD
        #emit LOAD.I
        #emit PUSH.PRI
      }
    }
  }

  numArgs -= 2;

  #emit LOAD.S.PRI numArgs
  #emit SHL.C.PRI 2
  #emit PUSH.PRI
  #emit LCTRL 6
  #emit ADD.C 0x20
  #emit PUSH.PRI
  #emit LOAD.S.PRI amxEntry
  #emit LOAD.I
  #emit SCTRL 6
  #emit STACK 8
  #emit RETN

  return 0;
}

stock amxCallIDX(index, fmt[], {Float,_}:...)
{
  new header[AMX_HEADER];
  new entry[AMX_ENTRY];
  new numArgs = numargs(), it;

  header = amxGetHeader();

  if (index < AMX_NUMENTRIES(header, AMX_PUBLICS))
  {
    entry = amxGetEntry(header, AMX_PUBLICS, index);

    for (it = numArgs - 1; it > 1; it--)
    {
      switch (fmt[it - 2])
      {
        case 'd', 'i', 'f':
        {
          #emit ADDR.ALT index
          #emit LOAD.S.PRI it
          #emit SHL.C.PRI 2
          #emit ADD
          #emit LOAD.I
          #emit LOAD.I
          #emit PUSH.PRI
        }

        default:
        {
            #emit ADDR.ALT index
          #emit LOAD.S.PRI it
          #emit SHL.C.PRI 2
          #emit ADD
          #emit LOAD.I
          #emit PUSH.PRI
        }
      }
    }
  }

  numArgs -= 2;

  #emit LOAD.S.PRI numArgs
  #emit SHL.C.PRI 2
  #emit PUSH.PRI
  #emit LCTRL 6
  #emit ADD.C 0x1C
  #emit PUSH.PRI
  #emit LOAD.S.PRI entry
  #emit SCTRL 6
  #emit STACK 0x48
  #emit RETN

  return 0;
}

#endif