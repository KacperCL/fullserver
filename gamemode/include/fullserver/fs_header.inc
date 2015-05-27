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

#pragma tabsize 0

#define __

#define EXIT 0xBABE

#define SERVER_NONE 0
#define VEHICLE_DEFAULT_HP 999
#define NIEMAPERZY (pData[playerid][levelAddition]!=LEVEL_ADD_MAPPER)
#define GANGMGMT (pData[playerid][adminLevel]>=LEVEL_ADMIN3)
#define FLOAT_NAN     (Float:0xFFFFFFFF)
#define SALT_ALOGIN         "changeme"
#define PASS_SECURE_SALT   "changeme"
#define MAX_IDENT_STEP  5

#define VIP_GP_COST 23
#define HOUSE_GP_COST 23
#define DOUBLE_GP_COST 100
#define GOD_GP_COST 5
#define MONEY_GP_COST 200000

#define KEY_PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define KEY_RELEASED(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
#define HOLDING_KEYS(%0) ((newkeys & (%0)) == (%0))

#define MAX_SERVER_PLAYERS          140
#undef MAX_PLAYERS
#define MAX_PLAYERS         (MAX_SERVER_PLAYERS)
#define MAX_LANGUAGES       1
#define MAX_LANG_ROWS               470
#define MAX_LANG_TEXT       128
#define MAX_LOGIN_ATTEMPS           3
#define MAX_TEXTDRAWS       120
#define MAX_PLAYER_TEXTDRAWS    20
#define MAX_GANGS                   25
#define MAX_ATTRACTIONS             9
#define MAX_MONEY_PICKUPS     32
#define MAX_HUD_ELEMENTS      13
#define MAX_CENSORED_WORDS          35
#define MAX_REPORTS                 8

#define MIN_QUEUE_CHOWANY     3
#define MIN_QUEUE_CTF               2
#define MIN_QUEUE_GG               2
#define MIN_QUEUE_HAY       2
#define MIN_QUEUE_DERBY       2
#define MIN_QUEUE_RACE        1
#define MIN_QUEUE_DRIFT       1
#define MIN_QUEUE_WG        2
#define MIN_QUEUE_STRZELNICA    3

#define MAX_QUEUE_CHOWANY           40
#define MAX_QUEUE_CTF               20
#define MAX_QUEUE_HAY               30
#define MAX_QUEUE_DERBY             40
#define MAX_QUEUE_RACE              40
#define MAX_QUEUE_DRIFT       3
#define MAX_QUEUE_WG                40
#define MAX_QUEUE_STRZELNICA        10
#define MAX_QUEUE_GG               15

#define DB_PLAYERS          "players"
#define DB_GANGS          "gangs"
#define DB_BANS           "bans"
#define DB_IPBANS         "ipbans"
#define DB_SBANS          "sbans"
#define DB_IPLOCKS          "iplocks"
#define DB_HBANS          "hbans"
#define DB_STATS          "stats"
#define DB_PLAYERSINGANGS     "players_in_gangs"
#define DB_CONFIG         "config"

#define LOG_SYSTEM          0
#define LOG_COMMANDS        1
#define LOG_CHAT          2
#define LOG_PM            3
#define LOG_OCHAT         4
#define LOG_ANN           5
#define LOG_PLAYERS         6

#define A_NONE            -1
#define A_CHOWANY         0
#define A_HAY           1
#define A_DERBY           2
#define A_RACE            3
#define A_DRIFT           4
#define A_WG            5
#define A_STRZELNICA        6
#define A_CTF                       7
#define A_GG                     8
#define A_ARENA                     9

#define MINIGAME_NONE       0
#define MINIGAME_FISHING      1

#define A_STATE_OFF         0
#define A_STATE_STARTING      1
#define A_STATE_ON          2

#define VW_UNIQUE_HOUSE       2000
#define VW_UNIQUE_ARENASOLO     3000
#define VW_UNIQUE_GANG        100
#define VW_DEFAULT          0
#define VW_STRZELNICA       1
#define VW_WG           3
#define VW_CHOWANY          4
#define VW_DERBY          5
#define VW_RACE           6
#define VW_HAY            7
#define VW_ARENA          8
#define VW_ARENA2         9
#define VW_ARENA3                   251
#define VW_JAIL           10
#define VW_AD           11
#define VW_MISC           15
#define VW_KSW            12
#define VW_TROLLE         13
#define VW_DRIFT          14
#define VW_CTF                      16
#define VW_GG                      17
#define VW_BOTS           19

#define CITY_LS           0
#define CITY_SF           1
#define CITY_LV           2

#define LEAVE_REASON_TIMEOUT    0
#define LEAVE_REASON_LEAVING    1
#define LEAVE_REASON_KICKBAN    2

#define LEVEL_NORMAL        0
#define LEVEL_GM          1
#define LEVEL_ADMIN1        2
#define LEVEL_ADMIN2        3
#define LEVEL_ADMIN3        4
#define LEVEL_ADMINHIDDEN     5

#define LEVEL_ADD_MAPPER  1

#define ATTACH_TMPMISC  3
#define ATTACH_FACTION  4
#define ATTACH_ARTEFACT 5

#define ATTACH_HEALTH 6
#define ATTACH_ARMOR  7

#define DIALOG_ADD_GP_MONEY         9998
#define DIALOG_ADD_GP_STEP2         9999
#define DIALOG_ADD_GP               8999
#define DIALOG_HOUSE_FOR_GP_HELP    7999
#define DIALOG_NICK_FOR_GP          6999
#define DIALOG_VIP_FOR_GP           5999
#define DIALOG_EXCHANGE_GP      4999
#define DIALOG_HELP_GPOINTS     3999
#define DIALOG_GOD_FOR_GP          2998
#define DIALOG_DOUBLE_FOR_GP          2999

#define DIALOG_LANGUAGE       0
#define DIALOG_LOGIN        1
#define DIALOG_HELP_RULES_REGISTER  6241
#define DIALOG_HELP_RULES_GUEST  2426
#define DIALOG_REGISTER        6218
#define DIALOG_REGISTER_STEP2        6217
#define DIALOG_CONFIG_MAIN      2
#define DIALOG_CONFIG_PCOLORS   3
#define DIALOG_CONFIG_CCOLORS   4
#define DIALOG_CONFIG_PCOLORS_SET 5
#define DIALOG_CONFIG_CCOLORS_SET 6
#define DIALOG_NICK_CHANGE      7
#define DIALOG_BAN_CONFIRM      8
#define DIALOG_HELP_MAIN      9
#define DIALOG_HELP_RULES     10
#define DIALOG_HELP_RESPECT     11
#define DIALOG_HELP_STARS     12
#define DIALOG_HELP_VIP       13
#define DIALOG_HELP_AUTHOR      14
#define DIALOG_HELP_CMD       15
#define DIALOG_HELP__GENERAL    16
#define DIALOG_HELP__ATRACTIONS 17
#define DIALOG_HELP__ACCOUNT    18
#define DIALOG_HELP__HOUSES   19
#define DIALOG_HELP__RESPECT    20
#define DIALOG_HELP__TELEPORTS  21
#define DIALOG_HELP__ANIMATIONS 22
#define DIALOG_HELP__VIP      23
#define DIALOG_HELP__ADMIN    24
#define DIALOG_BANK_MAIN      25
#define DIALOG_BANK_BALANCE     26
#define DIALOG_BANK_DEPOSIT_INPUT 27
#define DIALOG_BANK_DEPOSIT_INFO  28
#define DIALOG_BANK_WITHDRAW_INPUT  29
#define DIALOG_BANK_WITHDRAW_INFO 30
#define DIALOG_BANK_TRANSFER_STEP1  31
#define DIALOG_BANK_TRANSFER_STEP2  32
#define DIALOG_BANK_TRANSFER_STEP3  33
#define DIALOG_BANK_TRANSFER_INFO 34
#define DIALOG_KICK_CONFIRM     35
#define DIALOG_STATS_MAIN     36
#define DIALOG_STATS_PLAYER     37
#define DIALOG_STATS_SERVER     38
#define DIALOG_TOP_MAIN       39
#define DIALOG_TOP_RESPECT      40
#define DIALOG_TOP_FRAGS      41
#define DIALOG_TOP_TIMEPLAYED   42
#define DIALOG_UNBAN_CONFIRM    43
#define DIALOG_BANIP_CONFIRM    44
#define DIALOG_UNBANIP_CONFIRM    45
#define DIALOG_TOP_MONEY      46
#define DIALOG_TOP_SKILL      47
#define DIALOG_STATS_GANG_LIST    48
#define DIALOG_STATS_GANG_INFO    49
#define DIALOG_TOP_GANGS      50
#define DIALOG_CMDSEL       51
#define DIALOG_STYLWALKI      52
#define DIALOG_HUD          53
#define DIALOG_CONFIG_MAXPING   54
#define DIALOG_CONFIG_ASETTINGS   55
#define DIALOG_CONFIG_ASETTINGS_LIST  56
#define DIALOG_CONFIG_ASETTINGS_NAME  57
#define DIALOG_CONFIG_ASETTINGS_QUEUE 58
#define DIALOG_CONFIG_ASETTINGS_TIME  59
#define DIALOG_PASSWORD_CHANGE      60
#define DIALOG_EMAIL_CHANGE     6522
#define DIALOG_EMAIL_FORCE_CHANGE     6523
#define DIALOG_CONFIG_CENSORSHIP    61
#define DIALOG_CONFIG_CENSORSHIP_LIST 62
#define DIALOG_CONFIG_CENSORSHIP_ADD  63
#define DIALOG_CONFIG_CENSORSHIP_DELETE 64
#define DIALOG_REPORTS        65
#define DIALOG_REPORTS_MANAGE   66
#define DIALOG_MSGLIST              2351
#define DIALOG_MSGLIST_MANAGE       2352
#define DIALOG_CHANGE_SPAWN         3251
#define DIALOG_CHANGE_WEAPON        3229
#define DIALOG_VEH_PANEL            3599

#define DIALOG_PLAYER_INFO      68
#define DIALOG_WEAPON_QUICKBUY    69
#define DIALOG_ARENASOLO_SELECT   70
#define DIALOG_WEAPON_SELECT    71
#define DIALOG_CONFIRM        72

#define DIALOG_INFO         73

#define DIALOG_GANG         200
#define DIALOG_GANG_MAINMENU    (DIALOG_GANG+0) //200
#define DIALOG_GANG_CONFIRMLEAVING  (DIALOG_GANG+1) //201

#define DIALOG_REG_FULL             666
#define DIALOG_REG_FULL2            (DIALOG_REG_FULL+1) //667
#define DIALOG_REG_FULL3            (DIALOG_REG_FULL+2) //668

#define DIALOG_KICK_INFO            3665
#define DIALOG_AD_INFO        3680
#define DIALOG_AD_INFO2       3681

#define TD_DATETIME         33
#define TD_VOTING         34
#define TD_ANNOUNCE         35
#define TD_STARS_START        36
#define TD_STARS_END        66
#define TD_ATTRACTIONBOX      67

#define TD_ARACEBOX         68
#define TD_ANN2           69
#define TD_AWGINFO          70
#define TD_AWGINFOHP        71
#define TD_TIME           72
#define TD_ACTFINFOPOINTS       77
#define TD_ACTFINFO       78
#define TD_ACTFSTATETEXT 79
#define TD_ACTFSTATETITLE 80
// 73,74,75 - fullserver, wersja, tekst reklamowy - widoczny w wybieralce skinow

#define TD_ADRIFTBOX        82
#define TD_INFOBOX          83
#define TD_ATTRACTION_TITLES    84
#define TD_ATTRACTION_STATES    85
#define TD_WELCOMEBOX       86
#define TD_WELCOMEBOX2        94
#define TD_WELCOMETITLE       95
#define TD_WELCOMETEXT        96

#define TD_STAT           97

#define TD_FULLSERVER       114
#define TD_FULLSERVERWEBSITE    115
#define TD_AREAFULLDM       117
#define TD_AREANODM         118
#define TD_WARNING          119

#define PTD_VEHICLEINFO       0
#define PTD_NICK          1
#define PTD_STAT          2
#define PTD_SCORECHANGE       3
#define PTD_ACHIEVEMENT       4
#define PTD_ARACEBOX        5
#define PTD_FPS           6
#define PTD_ADRIFTBOX       8
#define PTD_ARACETIME       9
#define PTD_ADRIFTTIME              10
#define PTD_HPTEXT              11
#define PTD_ARICON              12
#define PTD_HPICON              13
#define PTD_GPCHANGE            14
#define PTD_CJHIT              15
#define PTD_CJHIT_TXT              16
#define PTD_CJHIT_HP              17
#define PTD_CJHIT2              18
#define PTD_AGGINFO             19

#define TDE_WIDE          0
#define TDE_CITYSELECTPLAYERSLS   1
#define TDE_CITYSELECTPLAYERSSF   2
#define TDE_CITYSELECTPLAYERSLV   3
#define TDE_CITYSELECTLS      4
#define TDE_CITYSELECTSF      5
#define TDE_CITYSELECTLV      6
#define TDE_HINT_CITYSELECT     7
#define TDE_VOTING          8
#define TDE_DATETIME        9
#define TDE_CLOCK        10
#define TDE_ATTRACTIONBOX     11
#define TDE_VEHICLEBOX        12
#define TDE_WELCOMEBOX        13
#define TDE_STATS         14
#define TDE_FULLSERVERLOGO      15
#define TDE_FPS           16
#define TDE_WYBIERALKA        17

#define COLOR_3DTEXT_INFORMATION  0xF5CB0060
#define COLOR_3DTEXT_HITMAN     0xB51010FF
#define COLOR_3DTEXT_FREEHOUSE    0x72CB00FF
#define COLOR_3DTEXT_PURCHASEDHOUSE 0x0097FFFF

#define HUD_STATUSBAR       0
#define HUD_ATTRACTIONBOX     1
#define HUD_VEHICLEBOX        2
#define HUD_ANNOUNCEMENTS     3
#define HUD_VOTING          4
#define HUD_SERVERLOGO        5
#define HUD_AREA          6
#define HUD_DATE          7
#define HUD_CLOCK          8
#define HUD_STARS         9
#define HUD_FPS           10
#define HUD_HP              11
#define HUD_CJHIT             12

#define UPDATE_TIMER_TIME     1000
#define AUTO_KEY_REPEAT_TIME    600
#define RESPECT_BONUS_INTERVAL    3600
#define PLAYER_DATA_UPDATE_INTERVAL 60000 * 3
#define PLAYER_COLOR_ALPHA      150
#define NO_GANG           0
#define INVALID_GANG        -1

#define GANG_RANK_NONE  0
#define GANG_RANK_SUSPENDED 1
#define GANG_RANK_MEMBER  2
#define GANG_RANK_LEADER  3
#define GANG_RANK_OWNER   4
#define GANG_RANK_VICEOWNER 5

#define ARENA_MAX   15  // maksymalna ilosc aren
#define ARENA_NONE    0
#define ARENA_SOLO    1
#define ARENA_PUEBLO  2
#define ARENA_SNIPER    3
#define ARENA_MINIGUN 4
#define ARENA_PRODM   5
#define ARENA_ONEDE 6
#define ARENA_RIFLE 7
#define ARENA_RPG   8
#define ARENA_SO  9
#define ARENA_ONEDE2  10
#define ARENA_ONEGUN  11
#define ARENA_DM    12
#define ARENA_CHAINSAW  13
#define ARENA_CS 14

#define ON_START_MONEY        45000

#define A_WG_TEAM_GREEN       0
#define A_WG_TEAM_BLUE        1

#define A_CTF_TEAM_GREEN            0
#define A_CTF_TEAM_BLUE             1

#define A_CHOWANY_HIDDING     0
#define A_CHOWANY_SEARCHING     1

#define PICKUP_HOUSE_NOTPURCHASED 1273
#define PICKUP_HOUSE_PURCHASED    1272

#define ANN_MODE_OFF        0
#define ANN_MODE_FADE_IN      1
#define ANN_MODE_FADE_OUT     2
#define ANN_MODE_ON         3

#define DOOR_OPENED         0
#define DOOR_CLOSED         1

#define BUTTON_NEXT         1
#define BUTTON_BACK         0
#define BUTTON_OK         1
#define BUTTON_QUIT         0
#define BUTTON_YES          1
#define BUTTON_NO         0

#define COLOR_INFO          0
#define COLOR_INFO2         1
#define COLOR_ERROR         2
#define COLOR_JOININFO        3
#define COLOR_LEAVEINFO       4
#define COLOR_ADMIN         5
#define COLOR_ADMIN3        6
#define COLOR_PRIVATEMSG      7
#define COLOR_IC          8
#define COLOR_ME          9
#define COLOR_GM          10
#define COLOR_INFO3         11
#define COLOR_VIPSAY        12
#define COLOR_VIP         13

#define T_PLAYER          0
#define T_BOTH            1
#define T_BOTH_PV         2 // teleport both and preserve velocity

#define f_c_PCOLORS         0
#define f_c_CCOLORS         1
#define f_c_MAXPING         2
#define f_c_ASETTINGS       3
#define f_c_CENSORSHIP        4
#define f_c_CHATCOLORS        5

#define f_c_a_NAME          0
#define f_c_a_QUEUE         1
#define f_c_a_TIME          2

#define f_c_c_ENABLEDISABLE     0
#define f_c_c_LIST          1
#define f_c_c_ADDWORD       2
#define f_c_c_DELETEWORD      3

#define f_h_RULES         0
#define f_h_CMD           1
#define f_h_RESPECT         2
#define f_h_GP              3
#define f_h_STARS             4
#define f_h_VIP             5
#define f_h_AUTHOR            6

#define f_h__GENERAL        0
#define f_h__ATRACTIONS     1
#define f_h__ACCOUNT        2
#define f_h__HOUSES       3
#define f_h__RESPECT        4
#define f_h__TELEPORTS      5
#define f_h__ANIMATIONS     6
#define f_h__VIP          7
#define f_h__ADMIN        8

#define f_b_BALANCE         0
#define f_b_DEPOSIT         1
#define f_b_DEPOSIT_ALL             2
#define f_b_WITHDRAW                3
#define f_b_TRANSFER                4

#define f_s_PLAYERSTATS       0
#define f_s_SERVERSTATS       1
#define f_s_GANGSTATS       2

#define f_t_RESPECT         0
#define f_t_SKILL         1
#define f_t_FRAGS         2
#define f_t_MONEY         3
#define f_t_PLAYEDTIME        4
#define f_t_GANGS         5
#define f_t_LISTY                   6
#define f_t_WALIZKI                 7
#define f_t_QUIZ                    8
#define f_t_METEORY                 9

#define ERROR_NOT_FOUND         0xFFFE
#define ERROR_FOUND_MULTIPLE    0xFFFF

#define foreach(%1) for(new %1 = 0; %1 <= gmTemp[highestPlayerID]; %1++) if(IsPlayerConnected(%1))
#define HidePlayerDialog(%1) ShowPlayerDialog(%1, -1, 0, " ", " ", " ", " ")
#define dli(%1,%2,%3,%4) ((%1==1)?(%2):(((%1% 10>1)&&(%1% 10<5)&&!((%1% 100>=10)&&(%1% 100<=21)))?(%3):(%4)))
#define SCM(%1,%2,%3) SendClientMessage(%1, %2, %3)
#define MUST_BE_SPAWNED(%1) if(!(IsPlayerSpawned(%1) || pData[%1][spectating] != INVALID_PLAYER_ID)) return Msg(%1, COLOR_ERROR, TXT(%1, 24))
#define GetPlayerCurrentSession(%0) ((GetTickCount() - pData[%0][session]) / 1000)
#define IsGM(%0) ((pData[%0][adminLevel]<LEVEL_GM) ? false : true)
#define IsFS(%0) pTemp[%0][isFS] && pData[%0][loggedIn]
#define GetPlayerProperName(%0) GetPlayerNick(%0)
#define TXT(%0,%1) gLang[pData[%0][language]][%1]

#define VEHICLE_REPAIR_INTERVAL 30000 // jak czesto mozna naprawiac pojazdy
#define HEAL_REPAIR_INTERVAL  10000 // jak czesto uzywac /100hp

#define VF_STREET   1 // jezdzi
#define VF_AIRBORNE   2
#define VF_NATATORIAL 4 // plywajace
#define VF_MILITARY   8
#define VF_RAILROAD   16  // kolejowe
#define VF_RC     32  // zdalnie sterowane
#define VF_ROAD     64  // drogowe
#define VF_AIRPLANE   128 // samoloty
#define VF_HELICOPTER 256
#define VF_BIKES    512 // motory i rowery
#define VF_TRAILER    1024
#define VF_TOWABLE    2048  // mozliwe do holowania
#define VF_POLICE     4096

#define SQL_RI_BACKGROUND (-1)  // uzywane przy mysql_query, dla wykonywania zapytan w tle, bez koneicznosci przetwarzania odpowiedz
#define SQL_RI_DOMY_LISTA (2)
#define SQL_RI_VEHNAMES   (3) // nazwy pojazdow

#define ART_ADD 0
#define ART_UPDATE 1

// why this is not in a_players!?
#define BODY_PART_TORSO     3
#define BODY_PART_ASS       4
#define BODY_PART_LEFT_ARM  5
#define BODY_PART_RIGHT_ARM 6
#define BODY_PART_LEFT_LEG  7
#define BODY_PART_RIGHT_LEG 8
#define BODY_PART_HEAD      9

native gpci(playerid, const serial[ ], maxlen);
native IsValidVehicle(vehicleid);

forward bool:IsValidSkin(skinid);

new gstr[256];
new ebuf[256]; // used for error logging

new LastTenJoinTimes[10];
new LastTenJoinIps[10][16];

#if defined CJHIT_MODEL
new boneName[10][11] =
{
    "", "", "", "TORS", "TYLEK", "LEWA REKA", "PRAWA REKA", "LEWA NOGA", "PRAWA NOGA", "GLOWA"
};
#endif

enum e_achievements {
  e_walizki,    // oprogramowane
  e_kills,    // oprogramowane
  e_longplay,   // oprogramowane
  e_racefcnt,   // oprogramowane
  e_racedist,   // oprogramowane
  e_derbyfan,   // oprogramowane
  e_derbywin,   // oprogramowanie
  e_fotocash,   // oprogramowane - kasa wydana na fotoradary
  e_driftpts,   // oprogramowane - punkty driftu
  e_listonosz,
  e_chowin,
  e_chofan,
  e_paczki,
  e_quiz,
}

enum e_gmData
{
  // db config
  DB_hostname[24],
  DB_username[24],
  DB_password[30],
  DB_database[24],
  DB_prefix[5],
  DB_players[32],
  DB_gangs[32],
  DB_bans[32],
  DB_ipbans[32],
  DB_sbans[32],
  DB_iplocks[32],
  DB_hbans[32],
  DB_stats[32],
  DB_playersingangs[32],
  DB_config[32],
  // colors
  color_normalUser,
  color_fsUser,
  color_vipUser,
  color_gmUser,
  color_lvl1User,
  color_lvl2User,
  color_adminUser,
  color_chatInfo,
  color_chatInfo2,
  color_chatInfo3,
  color_chatError,
  color_chatGM,
  color_chatAdmin,
  color_chatAdmin3,
  color_chatPM,
  color_chatIC,
  color_chatME,
  color_joinInfo,
  color_leaveInfo,
  color_vipSay,
  color_chatVip,
  color_chatInfo_HL,
  color_chatInfo2_HL,
  color_chatInfo3_HL,
  color_chatError_HL,
  color_chatGM_HL,
  color_chatAdmin_HL,
  color_chatAdmin3_HL,
  color_chatPM_HL,
  color_chatIC_HL,
  color_chatME_HL,
  color_joinInfo_HL,
  color_leaveInfo_HL,
  color_vipSay_HL,
  color_chatVip_HL,
  // other stuff
  bool:censorship,
  bool:chatColors,
  artefactOwner,
  maxPing,
  maxCensoredWords,
  serverIP[16],
  death_count,
  kill_count,
  join_count
}

enum e_gmTemp
{
  bool:debugMode,
  bool:ann_switchToNew,
  bool:updateTimerRunning,
  bool:protAll,
  bool:snoopCMD,
  bool:chatDisabled,
  bool:isBombs,
  bool:showTroll,
  bool:forceQuiz,
  bool:showGang,
  bool:timer_TimeSync_step,
  specialVehicle[MAX_VEHICLES],
  specialVehicleO[MAX_VEHICLES],
  banEx[64],
  ann_switchToNewText[128],
  aStrzelnicaPlayers[MAX_QUEUE_STRZELNICA],
  aHAYPlayers[MAX_QUEUE_HAY],
  aWGPlayers[MAX_QUEUE_WG],
  aWGDescr[64],
  aCTFPlayers[MAX_QUEUE_CTF],
  aCTFDescr[64],
  aGGPlayers[MAX_QUEUE_GG],
  aRacePlayers[MAX_QUEUE_RACE],
  aRacePlayersR[MAX_PLAYERS],
  aRaceDescr[70],
  aDriftPlayers[MAX_QUEUE_DRIFT],
  aDriftPlayersR[MAX_PLAYERS],
  aDriftDescr[70],
  aDerbyPlayers[MAX_QUEUE_DERBY],
  aChowanyPlayers[MAX_QUEUE_CHOWANY],
  voteCount[2], // tak, nie
  voteQuestion[70],
  AdminPasswordHash[33],
  chatLastText[127],
  Float:artePos[3],
  Float:aWGCurrentArea[6],
  Float:aCTFCurrentArea[6],
  Float:aChowanyCurrentArea[6],
  Float:aCTFWorldBounds[4],
  Float:aWGWorldBounds[4],
  interval_hourBonus,
  interval_starsUpdate,
  interval_jailCheck,
  interval_aTextDrawUpdate,
  interval_aStrzelnicaUpdate,
  interval_artefact,
  interval_statsUpdate,
  interval_FPScheck,
  interval_aWGUpdate,
  interval_aCTFUpdate,
  interval_aGGUpdate,
  interval_aDerbyUpdate,
  interval_aRaceUpdate,
  interval_aDriftUpdate,
  interval_aHAYUpdate,
  interval_aChowanyUpdate,
  interval_vehicleRespawn,
  interval_AFKCheck,
  interval_voteCheck,
  interval_gangs,
  timer_MoneyPickups_step,
  timer_SavePlayersData_step, // automatyczna regulacja czestotliwosci zapisywania danych graczy w zaleznosci od ich ilosci
  timer_SavePlayersData_lastidx,
  timer_PingUpdate_lastidx,
  timer_EPS_step,
  timer_PRA_step,
  timer_Artefakt_step,
  lastWeatherChangeTime,
  gametime_offset,
  changed_time,
  lastHour,
  lastDay,
  ann_effect_alpha,
  ann_effect_mode,
  ann2_step,
  timerAnnounce,
  timerAnnounceEnd,
  timerAnnounce2,
  aStrzelnicaMaxPlayers,
  aStrzelnicaCount,
  aStrzelnicaStartTick,
  timerStrzelnicaCountdown,
  aStrzelnicaShooter,
  moneyPickupIdx,
  fsaIndex,
  fsclIndex,
  fsaTimer,
  fsaCurrentColor,
  aHAYMaxPlayers,
  aHAYCount,
  timerHAYCountdown,
  aHAYStartTick,
  aWGMaxPlayers,
  aWGCount,
  timerWGCountdown,
  timerWGWeapSel,
  aWGStartTick,
  aWGArena,
  aWGInterior,
  aWGForceArena,
  Bar:aWGPBar,
  aGGMaxPlayers,
  aGGCount,
  timerGGCountdown,
  aGGStartTick,
  aCTFMaxPlayers,
  aCTFCount,
  timerCTFCountdown,
  aCTFStartTick,
  aCTFArena,
  aCTFInterior,
  aCTFFlaga,
  aCTFFlagaObj,
  aCTFFlagaCP,
  timerCTFWeapSel,
  aCTFForceArena,
  Bar:aCTFBar,
  Float:aRaceDistance,
  aRaceMaxPlayers,
  aRaceCount,
  aRaceArena,
  aRaceInterior,
  aRaceVehicle,
  aRaceCountdown,
  aRaceStartTick,
  aRaceNumCP,
  timerRaceCountdown,
  aRaceForceCircuit,
  Float:aDriftDistance,
  aDriftMaxPlayers,
  aDriftCount,
  aDriftArena,
  aDriftInterior,
  aDriftVehicle,
  aDriveCountdown,
  aDriftStartTick,
  aDriftNumCP,
  aDriftParTime,
  timerDriftCountdown,
  aDriftForceCircuit,
  aDerbyMinZ,
  aDerbyMaxPlayers,
  aDerbyCount,
  aDerbyArena,
  aDerbyArenaDescr[25],
  aDerbyInterior,
  aDerbyVehicle,
  timerDerbyCountdown,
  aDerbyStartTick,
  aDerbyForceArena,
  aChowanyMaxPlayers,
  aChowanyCount,
  aChowanyArena,
  aChowanyForceArena,
  aChowanyDescr[70],
  aChowanyInterior,
  timerChowanyCountdown,
  aChowanyStartTick,
  voteTimeleft,
  voteAuthor,
  pPlayerCount,
  pAdminCount,
  pAbsAdminCount, // wraz z ukrytym
  pVipCount,
  pGMCount,
  highestPlayerID,
  performBenchmark,
  chatLastMSGSender,
  chatMSGCount,
  currentWeather,
  snoopPM,  // 0 - nie, 1 - lajt, 2 - all
  showJoins,  // 0 - nie, 1 - dla adminow, 2 - all
  odliczLast,
  playallLast,
  artefactDropped,
  startTime,
  popularArena, // id popularnie obecnie areny
  lastZoneIdNoDM,
  lastZoneIdFullDM,
  firstZoneIdNoDM,
  firstZoneIdFullDM,
}

enum e_pData
{
  bool:reported,
  bool:loggedIn,
  bool:vipEnabled,
  bool:suspended, // czy jest to zawieszony gracz z ranga
  bool:logonDialog,
  bool:destroyMyVehicle,
  bool:classSelecting,
  bool:aChowany,
  bool:aHAY,
  bool:aDerby,
  bool:aRace,
  bool:aDrift,
  bool:aWG,
  bool:aStrzelnica,
  bool:aCTF,
  bool:aGG,
  bool:statsShowed,
  bool:hudSetting[MAX_HUD_ELEMENTS],
  vipToDate[12],  // chyba 10
  pDzaloz[32],
  isLocked,
  selectedWeap,
  spawnData,
  language,
  vipDaysLeft,
  allowedLevel, // na jaki poziom moze sie zalogowac
  adminLevel,   // na jaki poziom sie zalogowal
  levelAddition,
  session,
  gang,
  gangRank,
  respect,
  gamep,
  pmoney,
  accountID,
  spectating,
  lastVehicle,
  lastUsedSkin,
  kills,
  teamkills,
  deaths,
  suicides,
  averagePing,
  skill,
  jail,
  jailMysql,
  pAttraction,
  hitman,
  currentColor,
}

#define ACCEPTS_NONE    0
#define ACCEPTS_REGISTERED  1
#define ACCEPTS_GANG    2
#define ACCEPTS_VIPS    4
#define ACCEPTS_GMS     8
#define ACCEPTS_ADMINS    16
#define ACCEPTS_ALL     (ACCEPTS_ADMINS+ACCEPTS_GMS+ACCEPTS_VIPS+ACCEPTS_GANG+ACCEPTS_REGISTERED)

enum e_acceptation {
  eac_pm,
  eac_solo
}

enum e_pTemp
{
  bool:lostedByAdmin,
  bool:gongAll,
  bool:fRsp,
  bool:doubleMode,
  bool:godMode,
  bool:uglyRejoinHack,
  bool:rampy,
  bool:watchDmg,
  bool:isRcon,
  bool: holdingKey,
  bool:useRsp,
  bool:firstSelectingScreen,
  bool:specPosReturn,
  bool:aWGDead,
  bool:aCTFDead,
  bool:aGGDead,
  bool:aCHDead,
  bool:AFK,
  bool:performingAnim,
  bool:hasVoted,
  bool:tdanimAchievement,
  bool:firstClassSelection,
  bool:stopBGSound,
  bool:disableWeaponCheck,
  bool:wStrefieNODM,
  bool:wStrefieFULLDM,
  bool:dead,
  bool:cenzura,
  bool:protkill,
  bool:protping,
  bool:lockedPos,
  bool:weaponsAllowed,
  bool:troll,
  bool:cheater,
  bool:fakeAFK,
  bool:seesNametags,
  bool:isFreezed,
  bool:playPlayall,
  bool:noSkill,
  bool:fakeKillWarn,
  bool:desync,
  bool:aCTFexit,
  bool:aGGexit,
  bool:joypadDetected,
  bool:fastKillWarn,
  GGlevelCount,
  GGactKillCount,
  GGprevKillCount,
  lastDeathHeadshottedBy,
  ept_dialogid,
  tmpRegisterPass[56],
  bloodId[2],
  tmpReason[128],
  bankTransferAName[24],
  tmpPlayerName[24],
  tmpBanAccountName[24],
  tmpBanIP[24],
  newPassword[32],
  newEmail[56],
  reportedTick[MAX_SERVER_PLAYERS],
  reportSync[MAX_REPORTS],
  reportTmpReason[128],
  dialogCB[32],
  urlAudio[128],
  mPeriod[10],
  mReason[100],
  newNick[24],
  Float:specPosition[4],
  Float:bombPos[3],
  mute,
  mExpireTime,
  shamalId,
  skinBeforeChange,
  rampTimer,
  rampId,
  lastResyncByKey,
  lastHealUsed,
  lastKillTickCount,
  loginAttemps,
  timerFlashScreen,
  timerFlashScreenRed,
  lastDialog,
  lastDialogItem,
  tmpTargetPlayerID,
  tmpBanTime,
  tmpPeriod,
  timerFreezePlayer,
  timerPutInVehicleAfterTeleport,
  timerStopPlayerMusic,
  bankTransferAID,
  bankTransferAmount,
  lastMsgTick,
  lastFireKeypress, // uzywane do antygl
  spamCount,
  pingSum,
  pingChecks,
  pingWarningCount,
  lastSessionSaveTick,
  lastWeaponUsed,
  lastWeaponHolded,
  lastPlayerKilled,
  killStreak,
  specInterior,
  specVirtualWorld,
  statsGangListID[MAX_GANGS],
  Text3D:FactionName,
  artefactStartTime,
  tmpConfigAttraction,
  reportTmpPlayerId,
  MsgTmpPlayerId,
  aWGTeam,
  aCTFTeam,
  selectedRowCTF,
  aChowanySide,
  staleTime,
  lcTicks,  // local chat ticks
  hitTicks,
  Float:lastDmg,
  Text3D:p3d_status,
  tdanimScore,  // zawiera wartosc aktualnie pokazywana (ilosc otrzymanego respektu)
  tdanimGp,
  warningReceived,
  onArena,
  pickupDelay,
  teleCPDelay,
  bonusHours,
  miniGame,
  accepts[e_acceptation],
  tmpLastHouse,
  tmpDialogValue,
  e_houseid,
  drunkLevel,drunkLevelL,
  playerColorAlpha,
  tmpAlpha,
  TPInv,
  curPos,
  lastPos,  // uzywane do wykrywania nieruchomych graczy
  audio_vehicle,
  vehicleSpecialLastUsed,
  weaponSkill_silenced,
  weaponSkill_pistol,
  weaponSkill_sawnoff,
  weaponSkill_uzi,
  bannedPlayersCnt, // banned players respect sum - suma respektu zbanowanych graczy - zabezpieczenie przeciwko zbanowaniu wszystkich na serwerze
  bannedPlayersRS,
  faction,
  arenaScore[ARENA_MAX],
  ept_fps,
  ept_fpswarns,
//  audioHandleId,
  bombId,
  bombTimer,
  vipHeal,
  vipArmor,
  isFS,
  properName[MAX_PLAYER_NAME],
  antygl,
  identStep,
  hPaymentType,
  smsType,
  opuStep1,
  opuStep2,
  opuStep3,
}

enum e_gData
{
  respect,
  name[32],
  tag[6],
  gColor,
  Float:gBaseSpawn[4],
  Float:gSpawnPoint[4],
  www[50],
  wyspaId,
}

enum e_gPickup
{
  ship,
  artefact
}

enum e_Vectors
{
  Float:X,
  Float:Y,
  Float:Z,
  Float:A
}

enum e_tpLOC
{
  Float:tpX,Float:tpY,Float:tpZ,Float:tpAngle,
  autor[32],
  tpWorld,
  tpInterior,
  bool:tpStreamed
}

enum e_tpRET
{
  Float: tpX,Float:tpY,Float:tpZ,Float:tpA,
  tpDestInt,tpInterior,tpWorld,bool:tpStreamed
}

enum e_aData
{
  aName[32],
  aStartPlayers,
  aState,
  aStartTick,
  aStartingTime
}

enum e_gMoneyPickups
{
  gmpPickupID,
  gmpMoney,
  gmpTime,
  Text3D:gmp3DText
}

enum e_gReports
{
  bool:rOccupiedIndex,
  rPlayerId,
  rPlayerName[24],
  rReportingPlayerName[24],
  rReason[51]
}

enum e_vehOptions {
  bool:vo_static,
  bool:vo_destroyIfEmpty,
  bool:vo_licensePlateSet,
  bool:vo_licensePlate[30],
  bool:vo_private,
  vo_houseid,
  vo_owningPlayerId,
  bool:vo_used,
  bool:vo_hasTurbo,   // nitro wspomagane turbem
  bool:vo_occupied,
  vo_color[2],
  vo_paintjob,
  vo_driver,
  vo_lastRepair // czas ostatniej naprawy
}

new
 gmData[e_gmData],
 gLang[MAX_LANGUAGES][MAX_LANG_ROWS][MAX_LANG_TEXT],
 pData[MAX_SERVER_PLAYERS][e_pData],
 gData[MAX_GANGS + 1][e_gData],
 aData[MAX_ATTRACTIONS][e_aData],
 gmTemp[e_gmTemp],
 pTemp[MAX_SERVER_PLAYERS][e_pTemp],
 gPickup[e_gPickup],
 gMoneyPickup[MAX_MONEY_PICKUPS][e_gMoneyPickups],
 gCensoredWord[MAX_CENSORED_WORDS][32],
 gReports[MAX_REPORTS][e_gReports],
 Text:gTextDraw[MAX_TEXTDRAWS],
 PlayerText:pTextDraw[MAX_PLAYER_TEXTDRAWS][MAX_SERVER_PLAYERS],
 vehicleDoorState[MAX_VEHICLES],
 vehicleDoorOwner[MAX_VEHICLES],
 bool:vehicleHasNitro[MAX_VEHICLES]={false,...},
 hMySQL,
 bool:tVehicleUsed[MAX_VEHICLES]={false,...},
 bool:tVehicleSpawned[MAX_VEHICLES]={false,...},
 bool:tVehicles[MAX_VEHICLES][e_vehOptions],
 oKratki[MAX_PLAYERS]={INVALID_OBJECT_ID,...};

new fNames_config[][] = {
 "Kolory graczy",
 "Kolory informacji na czacie",
 "Maksymalny dopuszczalny ping",
 "Ustawienia atrakcji",
 "Cenzura s³ów",
 "Kolorowanie czatu"
};

new fNames_config_pColors[][] = {
 "Domyœlny",
 "Gracz z tagiem [FS]",
 "VIP",
 "Game Moderator",
 "Admin - poziom 1",
 "Admin - poziom 2",
 "Admin - poziom 3 / RCON"
};

enum e_WeaponData {
  ewd_id,
  ewd_PLname[21],
  ewd_baseCost,
  ewd_baseAmmo,
  ewd_slot,
  bool:ewd_quickBuy,
  bool:ewd_damagingWeapon
}

new quickbuyWeapons[][e_WeaponData]={
  {0, "Piesci", 0, 0, 0, false, true},
  {1, "Kastet", 100, 1, 0, true, true},
  {2, "Kij golfowy", 200, 1, 1, true, true},
  {3, "Palka policyjna", 500, 1, 1, true, true},
  {4, "Noz", 250, 1, 1, true, true},
  {5, "Baseball", 400, 1, 1, true, true},
  {6, "£opata", 400, 1, 1, true, true},
  {8, "Katana", 2500, 1, 1, true, true},
  {9, "Pila mechaniczna", 10000, 1, 1, true, true},
  {14,"Kwiaty", 300,1, 10, true, true},
  {22,"Pistolet 9mm", 4000, 500, 2, true, true},
  {23,"Pistolet z tlumikiem", 4000, 500, 2, true, true},
  {24,"Desert Eagle", 5000, 1000, 2, true, true},
  {25,"Shotgun", 5000, 500, 3, true, true},
  {26,"Sawn-off Shotgun", 8000, 500, 3, true, true},
  {27,"Combat Shotgun", 25000, 500, 3, true, true},
  {28,"Micro SMG",10000, 1000, 4, true, true},
  {29,"MP5",10000, 1000, 4, true, true},
  {30,"AK-47",18000,1000, 5, true, true},
  {31,"M4",20000,1000, 5, true, true},
  {32,"Tec9",10000,1000, 4, true, true},
  {33,"Strzelba",5000,250,6, true, true},
  {34,"Snajperka",20000,400,6, true, true},
  {42,"Gasnica", 500, 250,9, true, true},
  {43,"Aparat", 500, 100, 9, true, false},
  {46,"Spadochron", 250, 1, 11, true, false},
  {35,"RPG",0,600,7, false, true},
  {38,"Minigun",0,1500,7, false, true},
  {16,"Granaty",0,80,7, false, true}
};

new weaponSlots[]={0,0,1,1,1,1,1,1,1,1,10,10,10,10,10,10,8,8,8,-1,-1,-1,2,2,2,3,3,3,4,4,5,5,4,6,6,7,7,7,7,8,12,9,9,9};
new weaponName[][22]={
  "Piêœci", // 0
  "Kastet",
  "Kij golfowy",
  "Pa³ka policyjna",
  "Nó¿",    // 4
  "Kij baseballowy",
  "£opata",
  "Kij bilardowy",
  "Katana",
  "Pi³a mechaniczna",
  "Fioletowe dildo",
  "Bia³e dildo",
  "D³ugie bia³e dildo",
  "Bia³e dildo",
  "Kwiaty",
  "Laska",  // 15
  "Granaty",
  "Gas ³zawi¹cy",
  "Koktail molotova",
  "none",
  "none",
  "none",
  "Pistolet 9mm",
  "Pistolet z t³umikiem",
  "Desert Eagle",
  "Shotgun",
  "Sawn-off Shotgun",
  "Combat Shotgun",
  "Micro Uzi",
  "MP5",
  "AK-47",
  "M4",
  "Tec9",
  "Strzelba",
  "Sniper Rifle",
  "RPG",
  "Wyrzutnia rakiet",
  "Miotacz ognia",
  "Minigun",
  "£adunki wybuchowe",
  "Detonator",
  "Spray",
  "Gaœnica",
  "Aparat",
  "Gogle noktowizyjne",
  "Gogle termiczne",
  "Spadochron"
};

new fNames_config_cColors[][] = {
 "Normalna osobista informacja",
 "Wa¿na osobista informacja",
 "Globalna informacja",
 "B³¹d",
 "Do³¹czenie do serwera",
 "Opuszczenie serwera",
 "Czat moderatora",
 "Czat admina",
 "Czat admina (poziom 3)",
 "Prywatna wiadomoœæ",
 "Czat lokalny",
 "/me",
 "(wyró¿nienie) Normalna osobista informacja",
 "(wyró¿nienie) Wa¿na osobista informacja",
 "(wyró¿nienie) Globalna informacja",
 "(wyró¿nienie) B³¹d",
 "(wyró¿nienie) Do³¹czenie do serwera",
 "(wyró¿nienie) Opuszczenie serwera",
 "(wyró¿nienie) Czat moderatora",
 "(wyró¿nienie) Czat admina",
 "(wyró¿nienie) Czat admina (poziom 3)",
 "(wyró¿nienie) Prywatna wiadomoœæ",
 "(wyró¿nienie) Czat lokalny",
 "(wyró¿nienie) /me"
};

new fNames_config_attraction[][] = {
 "Nazwa",
 "Wielkoœæ kolejki",
 "Czas startowania"
};

new fNames_help[][] = {
 "Regulamin serwera",
 "Komendy",
 "Respekt",
 "Game Points",
 "Gwiazdki",
 "Konta VIP",
 "O autorach"
};

new fNames_help_cmd[][] = {
 "Podstawowe",
 "Atrakcje",
 "Zarz¹dzanie kontem",
 "Domy",
 "Respekt",
 "Teleporty",
 "Animacje",
 "VIP"
};

new fNames_bank[][] = {
 "Stan konta",
 "Wp³ata",
 "Wp³ata wszystkiego",
 "Wyp³ata",
 "Przelew"
};

new fNames_stats[][] = {
 "Twoje statystyki",
 "Statystyki serwera",
 "Statystyki gangów"
};

new fNames_top[][] = {
 "Respekt",
 "Skill",
 "Fragi",
 "Pieni¹dze",
 "Czas gry",
 "Gangi",
 "Listonosze",
 "Poszukiwacze bonusów",
 "Rozwiazane QUIZy"
};

new fNames_hud[][] = {
 "Pasek stanu",
 "Tabela atrakcji",
 "Status pojazdu",
 "Ogloszenia (ann)",
 "Glosowania",
 "Logo serwera",
 "Strefa DM",
 "Data, ilosc graczy",
 "Godzina",
 "Gwiazdki",
 "Licznik FPS",
#if defined CJHIT_MODEL
 "Wskaznik HP",
 "Wskaznik obrazen"
#else
  "Wskaznik HP"
#endif
};

new fNames_censorship_off[][] = {
 "W³¹cz cenzurê",
 "Lista s³ów",
 "Dodaj nowe s³owo",
 "Usuñ s³owo"
};

new fNames_censorship_on[][] = {
 "Wy³¹cz cenzurê",
 "Lista s³ów",
 "Dodaj nowe s³owo",
 "Usuñ s³owo"
};

new szHelpRules[][] = {
  "1. Zakaz czitowania (tak¿e makra),",
  "2. Zakaz uleczania sie lub bycia uleczanym w trakcie walki,",
  "3. Zakaz strzelania z pojazdów wojskowych do graczy poza takimi pojazdami (nie dotyczy stref smierci i godzin od 22.00 do 7.00),",
  "4. Zakaz wykorzystywania luk w grze i gamemodzie,",
  "5. Zakaz psucia zabawy innym graczom i pracy administracji oraz stosowania wyzwisk do innych graczy i serwerow,",
  "6. Zakaz udostepniania i sprzedazy kont oraz posiadania wiecej niz jednego konta,",
  "7. Dozwolone bugi gry: GL, TGL, TK, FR.",
  "8. Zakaz naduzywania komend w celu zdobycia przewagi nad innymi (szczegolnie VIP),",
  "9. Graczom posiadajacym artefakt zakazuje sie opuszczania ZIEMNYCH granic mapy, podrozowania w niezniszczalnych pojazdach oraz teleportacji,",
  "10. Zakaz reklamowania innych serwerow SA-MP o tematyce DM (prosimy nie reklamowac FS na innych serwerach!),",
  "11. Zakaz omijania kar nadanych przez administracjê (np. granie po banie).",
  " ",
  "Pelny regulamin dostepny jest na forum {ff3030}http://www.fullserver.eu/"
};

new szHelpRespect[][] = {
 "Respekt zdobywasz poprzez:",
 " - gre - automatycznie co ok. minute dostajesz kilka punktow, oraz premie o kazdej pelnej godzinie gry",
 " - udzial w atrakcjach - punkty respektu sa nagroda w kazdej z atrakcji na zapisy",
 " - zbieranie bonusow: walizek, prezentow, podkow,",
 " - zabijanie innych graczy",
 " ",
 "Dzieki wyzszemu respektowi:",
 " - masz lepsze uzbrojonie na spawnie,",
 " - uzyskujesz dostep do szerszego arsenalu broni (granaty, koktajle molotowa!),",
 " - zdobywasz uznanie wsrod innych graczy,",
 " ",
 "Mozesz kupic kilka udogodnien, w zamian za swoj respekt. Wpisz /rcmd aby zakupic: niewidzialnosc, armor,",
 "wiekszy skill broni + dwie bronie na spawnie (dualso/uzi/pistolety). Respekt mozesz rowniez spozytkowac",
 "na oplacenie wynajetego domu."
};

new szHelpGPoints[][] = {
 "{ffffff}GamePoints zdobywasz poprzez:",
 " - Zakup poprzez SMS bezposrednio w grze poprzez komende /GP lub na stronie {ff3030}http://fullserver.eu/doladowania{ffffff}",
 " - Zakup poprzez kasê z gry bezpoœrednio w grze poprzez komende /GP",
 "oraz dodatkowo poprzez:",
 " - Pomoc na rzecz serwera",
 " - Uczesnictwo w evencie",
 " - Aktywna gre (zbieranie bonusow etc.)",
 " - Pomoc administracji/moderacji",
 " - Godzinny bonus tak samo jak respekt i kasê",
 " ",
 "GamePoints sluza wymianie na udogodnienia takie jak konto VIP. Wpisz /GP aby zobaczyc pelna liste mozliwosci."
};

new szHelpStars[][] = {
 "{FF6501}Pomaranczowe {ffffff} - 1-29 respektu",
 "{E3C759}Zolte {ffffff} - 30-209 respektu",
 "{30FF30}Zielone {ffffff} - 210-809 respektu",
 "{3030FF}Niebieskie {ffffff} - 810-6809 respektu",
 "{FF3030}Czerwone {ffffff} - 6810+ respektu",
 " ",
 "Bronie na spawnie: ",
 " 0+\tkastet/kwiaty, deagle, mp5",
 " 25+\t1/4 kamizelki",
 " ",
 " 1000+\tshotgun",
 " 1500+\t1/2 kamizelki",
 " 2000+\tm4",
 " 3500+\tdodatkowa amunicja, 1/2 kamizelki",
 " 5000+\t3/4 kamizelki",
 " 7000+\tsnajperka",
 " 10000+\tnoz",
 " 15000+\tcala kamizelka",
 " 40000+\tkoktajle molotowa, combat shotgun",
 " 75000+\tgranaty"
};

new szHelpVIP[][] = {
  "{FFEF00}Mozliwosci konta VIP:",
  "{a9c4e4} ",
  "* Spawnowanie za darmo wszystkich pojazdow",
  "* Spawnowanie dowolnego pojazdu podajac jego nazwe/ID",
  "* Dodawanie sobie jedna komenda zestawu broni za darmo",
    "* Dodawanie sobie dowolnej broni wraz z amunicja",
    "* Pisanie na czacie jako VIP",
    "* Pisanie na prywatnym czacie Vipow i Adminow",
    "* Naprawianie pojazdu dowolnemu graczowi za darmo",
    "* Posiadanie wyrozniajacego sie koloru Zoltego",
    "* Uzdrawianie dowolnego gracza za darmo",
    "* Dodawanie sobie/komus kamizelki kuloodpornej za darmo",
  "* Teleportowanie graczy do siebie i od siebie",
  "* Wszystkie komendy z /rcmd za darmo! (patrz /vpomoc)",
  "* Darmowy automatyczny tuning pojazdu",
  "* Limit biznesow zwiekszony do 3! (1 normalnie)",
  " ",
  "Zostan VIPem - doladuj swoje konto poprzez GamePoints! {FFEF00}Wiêcej informacji: /GP"
};

new szHelpAuthor[][] = {
 "{0092F5}FullServer™ {F5C172}XyzzyDM",
 GMVERSION, GMCOMPILED,
 " ",
 "{A9C4E4}Kod: {C1D0E5}carg0, Mateusz \"eider\" Cichon{A9C4E4}, {C1D0E5}Arnold \"SeriouS\" Fornalczyk {A9C4E4}, {C1D0E5}Lukasz \"W\" Biegaj{A9C4E4}, {C1D0E5}XJL",
 " ",
 "{A9C4E4}Specjalne podziekowania dla tworcow elementow gry i testerow:",
 "{C1D0E5}CTX123, Bas_, DavO, LU, NeutroN, Kicia, Sayu, Saracen, Sebox, Surv, Times, Theo, WUBE, Pr0f3ssi0n4LisT, 7hd i innych",
 " ",
 "{A9C4E4}Podziêkowania dla: {C1D0E5}DracoBlue, Incognito, Y_Less, Sacky, StrickenKid, Zeex, SA-MP Team, Fro1sha",
 " ",
 "{A9C4E4}© Copyright 2010-2015 - FullServer™ - www.FullServer.eu"
};

new szHelpCmdGeneral[][] = {
 "{60ff90}/100hp /bronie /rsp /spadochron /kill /lp /sp /telpos /savepos /staty",
 "{30ff60}/p /pojazdy /raport /pm /l /gang /top /hitman /fps /pomoc /admins",
 "{6030ff}/lock /unlock /cpanel /napraw /flip /tune /tunemenu /credits /vips",
 "{ff6030}/bonusy /spawn /audio /frsp /bs /gp /furia"
};

new szHelpCmdAtractions[][] = {
 "{ffffff}Na zapisy:{A5C3E7}",
 "   /chowany, /derby, /race, /wg, /strzelnica, /drifting",
 "   /hay, /ctf, /gungame",
 "{ffffff}/areny:{A5C3E7}",
 "   /pro, /minigun, /arenasolo, /solo, /pier, /so, /rpg",
 "   /onede, /onede2, /rifle",
 "   /chainsaw, /pueblo, /sniper, /onegun, /dm, /cs",
 "{ffffff}Minigry:{A5C3E7}",
 "   /tir, /warsztat, /policja, /dilerzy, /biznesy, /lot, /jetmax",
 "{ffffff}stunt+fun:{A5C3E7}",
 "   /stunt, /uboot, /kosciol, /andromeda",
 "{ffffff}Full DM:{A5C3E7}",
 "   /afganistan, /wietnam, /lv /marynarka"
};

new szHelpCmdAccount[][] = {
 "/rejestracja",
 "/nowynick - pozwala przeniesc statystyki na nowy nick",
 "/zmienhaslo /zmienemail",
 "/pmon, /pmoff",
 "/soloacc"
};

new szHelpCmdHouses[][] = {
 "Aby zarzadzac domem, wejdz w pickup przy wejsciu",
 "i wybierz pozadana opcje z MENU. Nie ma potrzeby",
 "pamietania kilkudziesieciu komend, wystarcza tylko dwie:",
 " ",
 "/dom - przenosi pod dom",
 "/mp - przenosi do zapisanego pojazdu",
 "/pp - teleportuje pojazd do ciebie",
 " ",
 "Mozesz przegladnac liste wszystkich domow, zobaczyc je na mapie,",
 "a takze dokonac zakupu/przedluzenia waznosci domu (bez koniecznosci",
 "wydawania respektu) na naszej stronie:",
 " ",
 "{ff3030}www.fullserver.eu/domy/{ffffff}"
};

new szHelpCmdTeleports[][] = {
 "/wyspa /wyspas /osiedle(1-5)  /g(1-4)  /wojsko  /wojsko2  /tama  /miasteczko  /impra  /tunels",
 "/tunesf  /tunelv  /molo  /zadupie  /lslot  /sflot  /lvlot  /kosciol  /chiliad  /lot  /stlot",
 "/zakochani  /ammo  /plazasf  /wiezowiec  /plaza  /pgr  /dd  /db  /pustynia /bayside",
 "/kss  /vc  /statek /stunt /4smoki  /rcbatte /vcstadium /ddw /skocznia /trackstadium",
 "/gs /ls  /sf /sf2 /sf3 /lv  /drift  /rcshop  /cpn  /calligula  /andromeda  /wooziebed",
 "/jaysdin  /woc  /tsdin  /wh  /wh2  /resta  /brothel  /brothel2  /libertycity ",
 "/uboot /poszukiwacze"
};

new szHelpCmdAnimations[][] = {
 "komendy animacji"
};

new szHelpCmdVIP[][] = {
 "{FFEF00}Komendy VIP-ów",
 "{A9C4E4} ",
 "/varmor [id/nick] - dodawanie armora sobie/komus",
 "/vheal [id/nick] - uleczanie siebie/kogos",
 "/vtt [id/nick] - przenoszenie do gracza (za zgoda)",
 "/vth [id/nick] - przenoszenie gracza do siebie (za zgoda)",
 "/vsay <tekst> - pisanie na czacie jako VIP",
 "/v - prywatny czat VIPow, GMow i Adminow",
 "/vbron - sprawnowanie broni",
 "/vzestaw - zestaw broni VIPA",
 "/vpozostalo - sprawdzanie waznosci konta VIP",
 "/vnapraw [id/nick] - naprawia pojazd swoj/gracza",
 "/vinvisible - daje niewidzialnosc",
 "/vdualso - podwojne SO",
 "/vdualuzi - podwojne UZI",
 "/vdualpistols - podwojne pistolety",
 "/vtune - automatyczny tuning za darmo",
 " ",
 "Doladowania kont VIP pod adresem",
 "{FFEF00}http://www.fullserver.eu/doladowania/"
};

new szHelpCmdAdmin[][] = {
 "komendy admina"
};

new szStatsPlayer[][] = {
 "{A9C4E4}Data rejestracji: {FFFFFF}%s",
 "{A9C4E4}Respekt: {FFFFFF}%i",
 "{A9C4E4}Skill: {FFFFFF}%i",
 "{A9C4E4}Game Points: {FFFFFF}%i",
 "{A9C4E4}Fragów (swoich cz³onków gangu): {FFFFFF}%i (%i)",
 "{A9C4E4}Œmierci (samobójstw): {FFFFFF}%i (%i)",
 "{A9C4E4}Liczba kicków: {FFFFFF}%s",
 "{A9C4E4}Liczba banów: {FFFFFF}%s",
 "{A9C4E4}Œredni ping: {FFFFFF}%i",
 "{A9C4E4}£¹czny czas gry: {FFFFFF}%i %s"
};

new szStatsServer[][] = {
 "{A9C4E4}Najwiêcej graczy: {FFFFFF}%s (%s)",
 "{A9C4E4}Zarejestrowanych graczy: {FFFFFF}%i",
 "{A9C4E4}Wejœæ na serwer: {FFFFFF}%d",
 "{A9C4E4}Fragów: {FFFFFF}%d",
 "{A9C4E4}Œmierci: {FFFFFF}%d",
 "{A9C4E4}Kicków: {FFFFFF}%s",
 "{A9C4E4}Banów: {FFFFFF}%s"
};

new szTopRespect[][] = {
  "respekt"
};

new szTopFrags[][] = {
  "fragi"
};

new szTopPlayedTime[][] = {
  "czas gry"
};

new DATA_weathers[] = {0, 1, 2, 3, 4, 5, 7, 10, 12, 14, 15, 17, 20};

new vehicleName[][33] = {
 "Landstalker",
 "Bravura",
 "Buffalo",
 "Linerunner",
 "Perenial",
 "Sentinel",
 "Dumper",
 "Firetruck",
 "Trashmaster",
 "Stretch",
 "Manana",
 "Infernus",
 "Voodoo",
 "Pony",
 "Mule",
 "Cheetah",
 "Ambulance",
 "Leviathan",
 "Moonbeam",
 "Esperanto",
 "Taxi",
 "Washington",
 "Bobcat",
 "Mr Whoopee",
 "BF Injection",
 "Hunter",
 "Premier",
 "Enforcer",
 "Securicar",
 "Banshee",
 "Predator",
 "Bus",
 "Rhino",
 "Barracks",
 "Hotknife",
 "Trailer",
 "Previon",
 "Coach",
 "Cabbie",
 "Stallion",
 "Rumpo",
 "RC Bandit",
 "Romero",
 "Packer",
 "Monster",
 "Admiral",
 "Squalo",
 "Seasparrow",
 "Pizzaboy",
 "Tram",
 "Trailer",
 "Turismo",
 "Speeder",
 "Reefer",
 "Tropic",
 "Flatbed",
 "Yankee",
 "Caddy",
 "Solair",
 "Berkley's RC Van",
 "Skimmer",
 "PCJ-600",
 "Faggio",
 "Freeway",
 "RC Baron",
 "RC Raider",
 "Glendale",
 "Oceanic",
 "Sanchez",
 "Sparrow",
 "Patriot",
 "Quad",
 "Coastguard",
 "Dinghy",
 "Hermes",
 "Sabre",
 "Rustler",
 "ZR-350",
 "Walton",
 "Regina",
 "Comet",
 "BMX",
 "Burrito",
 "Camper",
 "Marquis",
 "Baggage",
 "Dozer",
 "Maverick",
 "News Chopper",
 "Rancher",
 "FBI Rancher",
 "Virgo",
 "Greenwood",
 "Jetmax",
 "Hotring",
 "Sandking",
 "Blista Compact",
 "Police Maverick",
 "Boxville",
 "Benson",
 "Mesa",
 "RC Goblin",
 "Hotring Racer",
 "Hotring Racer",
 "Bloodring Banger",
 "Rancher",
 "Super GT",
 "Elegant",
 "Journey",
 "Bike",
 "Mountain Bike",
 "Beagle",
 "Cropdust",
 "Stunt",
 "Tanker",
 "RoadTrain",
 "Nebula",
 "Majestic",
 "Buccaneer",
 "Shamal",
 "Hydra",
 "FCR-900",
 "NRG-500",
 "HPV1000",
 "Cement Truck",
 "Tow Truck",
 "Fortune",
 "Cadrona",
 "FBI Truck",
 "Willard",
 "Forklift",
 "Tractor",
 "Combine",
 "Feltzer",
 "Remington",
 "Slamvan",
 "Blade",
 "Freight",
 "Streak",
 "Vortex",
 "Vincent",
 "Bullet",
 "Clover",
 "Sadler",
 "Firetruck",
 "Hustler",
 "Intruder",
 "Primo",
 "Cargobob",
 "Tampa",
 "Sunrise",
 "Merit",
 "Utility",
 "Nevada",
 "Yosemite",
 "Windsor",
 "Monster",
 "Monster",
 "Uranus",
 "Jester",
 "Sultan",
 "Stratum",
 "Elegy",
 "Raindance",
 "RC Tiger",
 "Flash",
 "Tahoma",
 "Savanna",
 "Bandito",
 "Freight",
 "Trailer",
 "Kart",
 "Mower",
 "Duneride",
 "Sweeper",
 "Broadway",
 "Tornado",
 "AT-400",
 "DFT-30",
 "Huntley",
 "Stafford",
 "BF-400",
 "Newsvan",
 "Tug",
 "Trailer",
 "Emperor",
 "Wayfarer",
 "Euros",
 "Hotdog",
 "Club",
 "Freight box",
 "Trailer",
 "Andromada",
 "Dodo",
 "RC Cam",
 "Launch",
 "Police Car (LSPD)",
 "Police Car (SFPD)",
 "Police Car (LVPD)",
 "Police Ranger",
 "Picador",
 "S.W.A.T. Van",
 "Alpha",
 "Phoenix",
 "Glendale",
 "Sadler",
 "Luggage Trailer",
 "Luggage Trailer",
 "Stair Trailer",
 "Boxville",
 "Farm Plow",
 "Utility Trailer"
};

new defaultColorsFields[][] = {
// Player colors
 "color_normalUser",
 "color_fsUser",
 "color_vipUser",
 "color_gmUser",
 "color_lvl1User",
 "color_lvl2User",
 "color_adminUser",
// Chat colors
 "color_chatInfo",
 "color_chatInfo2",
 "color_chatInfo3",
 "color_chatError",
 "color_chatGM",
 "color_chatAdmin",
 "color_chatAdmin3",
 "color_chatPM",
 "color_chatIC",
 "color_chatME",
 "color_joinInfo",
 "color_leaveInfo",
 "color_vipSay",
 "color_chatVip",
// Chat colors (highlight)
 "color_chatInfo_HL",
 "color_chatInfo2_HL",
 "color_chatInfo3_HL",
 "color_chatError_HL",
 "color_chatGM_HL",
 "color_chatAdmin_HL",
 "color_chatAdmin3_HL",
 "color_chatPM_HL",
 "color_chatIC_HL",
 "color_chatME_HL",
 "color_joinInfo_HL",
 "color_leaveInfo_HL",
 "color_vipSay_HL",
 "color_chatVip_HL",
// Attractions
 "a_chowany",
 "a_sps",
 "a_derby",
 "a_race",
 "a_drift",
 "a_wg",
 "a_strzelnica",
 "a_ctf",
 "a_gg",
 "censorship", // Censorship
 "max_ping" // Max ping
};

new defaultColorsData[][] = {
// Player colors
 "909090",
 "00A1FF",
 "FACB00",
 "58A100",
 "900000",
 "A50000",
 "C00000",
// Chat colors
 "FFE5A1",
 "E53E00",
 "7DBB00",
 "B01010",
 "FFFF00", //"4EA100",
 "FF0000", //"B00000",
 "FF3366", //"C80000",
 "FFD0AC",
 "FFA115",
 "72C100",
 "A0A0A0",
 "808080",
 "0097C1",
 "008DE5",
// Chat colors (highlight)
 "EAB171",
 "AC3E00",
 "589700",
 "EA2A2A",
 "247D00",
 "830000",
 "9C0000",
 "CFA07C",
 "DF8110",
 "429100",
 "707070",
 "505050",
 "0077A1",
 "006DCB",
// Attractions
 "/Chowany 5 20",
 "/HAY 1 20",
 "/Derby 5 20",
 "/Race 1 20",
 "/Drifting 1 10",
 "/WG 4 20",
 "/Strzelnica 5 20",
 "/CTF 4 20",
 "/GUNGAME 4 20",
 "1", // Censorship
 "200" // Max ping
};

enum e_vehcolors {
  evc_id,
  evc_value,
  evc_name[24],
  bool:evc_quickSelect
}

new vehicleColors[][e_vehcolors] = {
 {0, 0x000000, "Czarny", true},
 {1, 0xFFFFFF, "Bialy", true},
 {2, 0x2975a6, "Jelly Bean", true},
 {3, 0x840510, "Red Devil", true},
 {4, 0x253739, "Outer Space", true},
 {5, 0x87446f, "Cannon Pink", false},
 {6, 0xd68f11, "Golden Bell", true},
 {7, 0x4c75b7, "Steel Blue", false},
 {8, 0xBDBDC5, "French Gray", true},
 {13, 0x58595B, "Abbey", false},
 {14, 0xD5DAD4, "Tasman", true},
 {15, 0x9CA1A4, "Gray Chateau", false},
 {16, 0x335F3E, "Killarney", true},
 {17, 0x7C0A2B, "Monarch", false},
 {20, 0x3B4E79, "East Bay", true},
 {54, 0x282f4c, "Pickled Bluewood", false},
 {79, 0x0D316D, "Madison", true},
 {92, 0x6D6D6F, "Mid Gray", false},
 {93, 0x0f6a89, "Surfie Green", true},
 {112, 0x596D86, "Blue Bayoux", false},
 {113, 0x483433, "Woody Brown", true},
 {114, 0x456250, "Mineral Green", false},
 {118, 0xa3adc6, "Cadet Blue", true},
 {126, 0xed6aae, "Brilliant Rose", false},
 {161, 0x993E4D, "Salmon", true},           //zamiana niedzialajacych kolorow z 0.3c na nowe z 0.3x
 {191, 0x368452, "Eucalyptus", false},
 {155, 0x1E9999, "Azure", true},
 {222, 0x99311E, "Orange", false},
 {179, 0x3C1737, "Purple", true},
 {211, 0x241663, "Plum", false},
 {171, 0x53387E, "Violet", true},
 {245, 0x041804, "Dark Green", true},
 {229, 0x197F19, "Brilliant Green", false}
};

new fsLogoColors[] = {
 0x0080FFFF,
 0xC62F2FFF,
 0x1A8D1AFF,
 0xF5AC00FF,
 0xB16DEAFF
};

new telpos[e_tpLOC];
new evtp[e_tpLOC];
new privpos[MAX_PLAYERS][e_tpLOC];
new privpos2[MAX_PLAYERS][e_tpLOC];
new retpos[MAX_PLAYERS][e_tpRET];

enum e_arenasolo {
  eas_nazwa[32],
  eas_code,
  eas_interior,
  Float:eas_ar,
  Float:eas_spawn1[4],
  Float:eas_spawn2[4]
};

new DATA_arenysolo[][e_arenasolo]={
  { "Boxing ring",  10, 9, 0.0, { -1111.19, -1000.82,2132.82, -135.0 }, { -1106.98, -1004.98,2132.82 } }, // 0
  { "Mordownia",  20, 9, 99.0, { -1025.00,-963.34, 129.26,  175.0 }, { -1025.00,-1023.34, 129.26,  356.0 } }, // 1
  { "Rotunda", 30, 9, 99.0, { -1100.1,-1014.64, 129.52, 0.1 }, { -1100.1,-971.68, 129.52, 180.1 } }, // 2
  { "Sky", 40, 7, 99.0, { -1100.1,-1014.64, 129.52, 0.1 }, { -1100.1,-971.68, 129.52, 180.1 } }, // 3
  { "WH", 50, 1, 99.0, { 1413.93, 3.22, 1000.92, 135.0 }, { 1372.25, -35.94, 1000.92, 315.0 } } // 4
};
// EOF