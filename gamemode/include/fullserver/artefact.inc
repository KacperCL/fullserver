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

enum e_DATA_spawnarte
{
  Float: X,
  Float: Y,
  Float: Z,
  nazwa[128]
}

new Float:DATA_spawns_artefact[][e_DATA_spawnarte] = {
{ 1103.95,-1779.04,17.43, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1097.43,-1827.44,29.93, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1098.07,-1829.17,16.59, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1152.06,-1793.54,50.96, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1235.08,-1777.23,49.26, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1165.46,-1720.07,14.23, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1096.24,-1693.40,34.75, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1080.71,-1752.84,35.30, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1047.50,-1704.39,24.37, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 979.13,-1718.24,20.71, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1014.85,-1760.85,24.33, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1164.27,-1667.02,22.23, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 987.68,-1830.34,20.36, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1026.28,-1715.78,14.55, "gdzieœ w okolicy g³ównego parkingu Los Santos" },
{ 1192.58,-1738.11,25.27, "gdzieœ w okolicy g³ównego parkingu Los Santos" },

{ 2113.47,1003.06,56.77, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2098.52,1002.43,45.66, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2099.87,903.07,30.44, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2169.01,943.34,25.78, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2253.74,1039.86,29.38, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2231.38,1089.99,40.80, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2289.69,1110.59,10.82, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2149.33,1131.66,24.31, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2012.38,1073.62,31.98, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2016.63,1007.59,39.09, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2057.48,923.64,9.83, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2113.52,884.38,11.19, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2207.76,907.60,12.26, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2248.92,968.74,18.81, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },
{ 2189.34,1090.09,63.09, "gdzieœ w okolicy g³ównego parkingu Las Venturas" },

{ -1960.95,892.98,62.16, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1945.18,892.26,55.52, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1860.00,889.01,57.93, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1864.55,945.26,64.17, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1912.96,964.72,45.43, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -2037.62,956.14,73.59, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1969.84,805.16,95.01, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1947.55,828.25,77.70, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1975.99,845.32,45.30, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -2016.75,844.79,62.77, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -2047.72,896.47,53.12, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1908.85,902.67,35.17, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1884.72,821.44,43.80, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1909.38,830.97,44.25, "gdzieœ w okolicy g³ównego parkingu San Fierro" },
{ -1993.79,830.31,56.21, "gdzieœ w okolicy g³ównego parkingu San Fierro" },

{ 435.53,2532.19,26.65, "gdzieœ w okolicy pustynnego lotniska" },
{ 381.28,2611.99,16.48, "gdzieœ w okolicy pustynnego lotniska" },
{ 304.34,2619.70,17.38, "gdzieœ w okolicy pustynnego lotniska" },
{ 227.17,2594.85,16.48, "gdzieœ w okolicy pustynnego lotniska" },
{ 200.30,2572.38,29.12, "gdzieœ w okolicy pustynnego lotniska" },
{ 176.03,2439.59,16.91, "gdzieœ w okolicy pustynnego lotniska" },
{ 239.04,2438.00,17.27, "gdzieœ w okolicy pustynnego lotniska" },
{ 404.32,2432.94,29.63, "gdzieœ w okolicy pustynnego lotniska" },
{ 108.93,2403.74,17.83, "gdzieœ w okolicy pustynnego lotniska" },
{ -36.00,2461.73,16.45, "gdzieœ w okolicy pustynnego lotniska" },
{ 80.66,2452.30,20.84, "gdzieœ w okolicy pustynnego lotniska" },
{ 144.78,2621.98,16.79, "gdzieœ w okolicy pustynnego lotniska" },
{ 137.10,2666.32,36.14, "gdzieœ w okolicy pustynnego lotniska" },
{ 354.82,2671.34,32.38, "gdzieœ w okolicy pustynnego lotniska" },
{ 416.34,2530.51,19.18, "gdzieœ w okolicy pustynnego lotniska" },

{ 2540.05,-1628.92,13.42, "gdzieœ w okolicy grove street" },
{ 2541.73,-1692.55,12.66, "gdzieœ w okolicy grove street" },
{ 2431.23,-1662.90,25.65, "gdzieœ w okolicy grove street" },
{ 2374.98,-1698.97,32.19, "gdzieœ w okolicy grove street" },
{ 2352.42,-1725.15,22.96, "gdzieœ w okolicy grove street" },
{ 2330.76,-1639.35,27.77, "gdzieœ w okolicy grove street" },
{ 2271.01,-1711.89,17.60, "gdzieœ w okolicy grove street" },
{ 2240.70,-1631.52,15.71, "gdzieœ w okolicy grove street" },
{ 2393.52,-1686.40,28.26, "gdzieœ w okolicy grove street" },
{ 2326.66,-1642.83,24.09, "gdzieœ w okolicy grove street" },
{ 2376.18,-1725.33,13.55, "gdzieœ w okolicy grove street" },
{ 2486.91,-1727.01,43.24, "gdzieœ w okolicy grove street" },
{ 2448.51,-1699.59,13.52, "gdzieœ w okolicy grove street" },
{ 2428.61,-1633.94,13.42, "gdzieœ w okolicy grove street" },
{ 2401.12,-1638.10,16.17, "gdzieœ w okolicy grove street" },

{ -615.55,-1873.69,56.26, "gdzieœ w wietnamie" },
{ -521.24,-1946.15,44.60, "gdzieœ w wietnamie" },
{ -474.13,-2037.35,58.30, "gdzieœ w wietnamie" },
{ -521.18,-2033.15,52.10, "gdzieœ w wietnamie" },
{ -545.81,-2024.99,78.28, "gdzieœ w wietnamie" },
{ -613.05,-2049.15,48.95, "gdzieœ w wietnamie" },
{ -670.19,-2083.30,25.11, "gdzieœ w wietnamie" },
{ -735.45,-2076.13,25.02, "gdzieœ w wietnamie" },
{ -732.13,-2123.14,66.31, "gdzieœ w wietnamie" },
{ -774.15,-1971.91,5.18, "gdzieœ w wietnamie" },
{ -635.67,-1823.07,32.34, "gdzieœ w wietnamie" },
{ -516.40,-1862.03,14.21, "gdzieœ w wietnamie" },
{ -528.64,-1895.37,4.32, "gdzieœ w wietnamie" },
{ -649.77,-1980.69,43.28, "gdzieœ w wietnamie" },
{ -618.46,-1830.18,44.89, "gdzieœ w wietnamie" },

{ -24.05,1873.18,25.57, "gdzieœ w afganistanie" },
{ 15.81,1812.92,34.32, "gdzieœ w afganistanie" },
{ -50.54,1776.39,20.00, "gdzieœ w afganistanie" },
{ 15.71,1719.10,26.14, "gdzieœ w afganistanie" },
{ 144.75,1693.85,17.64, "gdzieœ w afganistanie" },
{ 64.64,1762.09,17.64, "gdzieœ w afganistanie" },
{ 176.59,1892.51,55.00, "gdzieœ w afganistanie" },
{ 109.19,2025.18,31.99, "gdzieœ w afganistanie" },
{ 254.68,1904.59,6.04, "gdzieœ w afganistanie" },
{ 253.89,1864.43,-32.39, "gdzieœ w afganistanie" },
{ 236.29,1801.76,7.41, "gdzieœ w afganistanie" },
{ 267.11,1841.23,6.82, "gdzieœ w afganistanie" },
{ 262.62,1818.53,1.01, "gdzieœ w afganistanie" },
{ 325.43,1846.59,5.83, "gdzieœ w afganistanie" },
{ 331.86,1837.77,7.83, "gdzieœ w afganistanie" },
{ 271.85,1873.73,8.76, "gdzieœ w afganistanie" },
{ 267.13,1878.11,-4.19, "gdzieœ w afganistanie" },
{ 274.64,1878.56,-30.39, "gdzieœ w afganistanie" }
};

CreateArtefact(Float:vX=FLOAT_NAN, Float:vY=FLOAT_NAN, Float:vZ=FLOAT_NAN)
{
    if (vX==vX) {
        gPickup[artefact] = CreateDynamicPickup(1276, 1, vX, vY, vZ, 0,0,-1);
      gmTemp[artePos][0]=vX;
    gmTemp[artePos][1]=vY;
    gmTemp[artePos][2]=vZ;

    } else {
    new r=random(sizeof DATA_spawns_artefact);
    new buffer[128];
    DestroyDynamicPickup(gPickup[artefact]);
    gPickup[artefact] = CreateDynamicPickup(1276, 1, DATA_spawns_artefact[r][X], DATA_spawns_artefact[r][Y], DATA_spawns_artefact[r][Z], 0,0,-1);
    format(buffer, sizeof buffer, "{C6E0FF}Artefakt{FFFFFF} pojawi³ siê {C6E0FF}%s", DATA_spawns_artefact[r][nazwa] );
    foreach(playerid)
        Msg(playerid,-1,buffer);
    gmTemp[artefactDropped]=0;
    gmTemp[artePos][0]=DATA_spawns_artefact[r][X];
    gmTemp[artePos][1]=DATA_spawns_artefact[r][Y];
    gmTemp[artePos][2]=DATA_spawns_artefact[r][Z];
  }
}

DropArtefact(playerid)
{
    new
     Float:pVectors[e_Vectors],
     buffer[128];

    if(IsPlayerConnected(playerid))
    {
        RemovePlayerAttachedObject(playerid, ATTACH_ARTEFACT);
        GetPlayerPos(playerid, pVectors[X], pVectors[Y], pVectors[Z]);
        CreateArtefact(pVectors[X], pVectors[Y], pVectors[Z]);
        gmTemp[artefactDropped]=GetTickCount();
        format(buffer, sizeof buffer, "{C6E0FF}%s{FFFFFF} zgubil%s {C6E0FF}artefakt", GetPlayerNick(playerid), Kobieta(playerid) ? ("a") : ("") );
        foreach(i)
            Msg(i, 0xC6E0FFFF, buffer);
    }
    else
    {
        CreateArtefact();
        gmTemp[artefactDropped]=0;
    }

    gmData[artefactOwner] = INVALID_PLAYER_ID;
    if (IsPlayerConnected(playerid))
        SetPlayerProperColor(playerid,true);
}


artefact_SetPlayerHolding(playerid){
    if (gmData[artefactOwner] != playerid) return;
    SetPlayerAttachedObject(playerid, ATTACH_ARTEFACT, 1276, 1, 0.1, -0.2, 0.0, 90.0, 88.0, 0.0);
}

artefact_OnPlayerPickup(playerid){
    if(pData[playerid][pAttraction] == A_NONE && (pData[playerid][adminLevel]<1 || pData[playerid][adminLevel]==5) && !pTemp[playerid][godMode])
    {
        new buffer[127];
        DestroyDynamicPickup(gPickup[artefact]);

        gmData[artefactOwner] = playerid;
        SetPlayerProperColor(playerid,true);
        artefact_SetPlayerHolding(playerid);

        pTemp[playerid][artefactStartTime] = GetTickCount() / 1000;
        format(buffer, sizeof buffer, "{C6E0FF}%s{FFFFFF} podniosl%s {C6E0FF}artefakt", GetPlayerNick(playerid), Kobieta(playerid) ? ("a") : ("") );
        foreach(i)
              Msg(i,-1, buffer);
    }
    return;

}
// EOF
