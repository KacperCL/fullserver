-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Czas wygenerowania: 30 Kwi 2014, 01:23
-- Wersja serwera: 5.5.35
-- Wersja PHP: 5.4.4-14+deb7u7

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Baza danych: `db_2152`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_achievements`
--

CREATE TABLE IF NOT EXISTS `fs_achievements` (
  `name` varchar(32) NOT NULL,
  `shortname` varchar(10) NOT NULL,
  PRIMARY KEY (`shortname`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_achievements_ranks`
--

CREATE TABLE IF NOT EXISTS `fs_achievements_ranks` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `shortname` varchar(10) CHARACTER SET ascii NOT NULL,
  `rank` varchar(48) NOT NULL,
  `score` int(10) unsigned NOT NULL,
  `replacename` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `shortname` (`shortname`),
  KEY `shortname_2` (`shortname`,`score`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=153 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_admin_activity`
--

CREATE TABLE IF NOT EXISTS `fs_admin_activity` (
  `id_player` int(11) NOT NULL,
  `data` date NOT NULL,
  `minut` smallint(3) NOT NULL,
  UNIQUE KEY `id_player` (`id_player`,`data`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_arena`
--

CREATE TABLE IF NOT EXISTS `fs_arena` (
  `id` int(10) unsigned NOT NULL,
  `descr` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_bans`
--

CREATE TABLE IF NOT EXISTS `fs_bans` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `player_banned` int(11) NOT NULL,
  `player_given` int(11) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_end` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `reason` text NOT NULL,
  PRIMARY KEY (`id`),
  KEY `player_banned` (`player_banned`,`date_created`,`date_end`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_chowany_arena`
--

CREATE TABLE IF NOT EXISTS `fs_chowany_arena` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `descr` varchar(64) NOT NULL,
  `interior` smallint(5) unsigned NOT NULL DEFAULT '0',
  `minplayers` smallint(5) unsigned NOT NULL DEFAULT '2',
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT '2',
  `wb_cube` varchar(64) CHARACTER SET ascii COLLATE ascii_bin DEFAULT NULL COMMENT 'prostopadloscian x1,y1,z1,x2,y2,z2',
  `wb_mode` enum('oraz','lub') CHARACTER SET ascii NOT NULL DEFAULT 'lub' COMMENT 'oraz - gracz musi byc w obu polach, lub - musi byc w jednym z nich',
  `wb_sphere` varchar(64) CHARACTER SET ascii DEFAULT NULL COMMENT 'kula/sfera: x,y,z,r',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=44 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_chowany_arena_sp`
--

CREATE TABLE IF NOT EXISTS `fs_chowany_arena_sp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `team` tinyint(1) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `A` double NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2291 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_config`
--

CREATE TABLE IF NOT EXISTS `fs_config` (
  `option_name` varchar(32) NOT NULL,
  `value` text,
  PRIMARY KEY (`option_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_ctf_arena`
--

CREATE TABLE IF NOT EXISTS `fs_ctf_arena` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `descr` varchar(64) NOT NULL,
  `interior` smallint(5) unsigned NOT NULL DEFAULT '0',
  `minplayers` smallint(5) unsigned NOT NULL DEFAULT '2',
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT '2',
  `wb_cube` varchar(64) CHARACTER SET ascii COLLATE ascii_bin DEFAULT NULL COMMENT 'prostopadloscian x1,y1,z1,x2,y2,z2',
  `wb_mode` enum('oraz','lub') CHARACTER SET ascii NOT NULL DEFAULT 'lub' COMMENT 'oraz - gracz musi byc w obu polach, lub - musi byc w jednym z nich',
  `wb_sphere` varchar(64) CHARACTER SET ascii DEFAULT NULL COMMENT 'kula/sfera: x,y,z,r',
  `flaga_spawn` varchar(64) NOT NULL,
  `flaga_team0` varchar(64) NOT NULL COMMENT 'zielony',
  `flaga_team1` varchar(64) NOT NULL COMMENT 'niebieski',
  PRIMARY KEY (`id`),
  KEY `minplayers` (`minplayers`,`maxplayers`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=91 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_ctf_arena_sp`
--

CREATE TABLE IF NOT EXISTS `fs_ctf_arena_sp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `team` tinyint(1) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `A` double NOT NULL,
  `randomorder` tinyint(3) unsigned NOT NULL COMMENT 'wartosc losowa, aktualizowana automatycznie',
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`,`team`,`randomorder`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3414 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_derby_arena`
--

CREATE TABLE IF NOT EXISTS `fs_derby_arena` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `minPlayers` tinyint(3) unsigned NOT NULL DEFAULT '2',
  `maxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '4',
  `vehicle` smallint(5) unsigned NOT NULL DEFAULT '444',
  `interior` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `descr` varchar(255) DEFAULT NULL,
  `nitro` tinyint(1) NOT NULL DEFAULT '0',
  `minZ` int(11) NOT NULL DEFAULT '-1000',
  PRIMARY KEY (`id`),
  KEY `minPlayers` (`minPlayers`,`maxPlayers`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=19 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_derby_arena_sp`
--

CREATE TABLE IF NOT EXISTS `fs_derby_arena_sp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `angle` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=447 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_dragrace_records`
--

CREATE TABLE IF NOT EXISTS `fs_dragrace_records` (
  `id_player` int(10) unsigned NOT NULL,
  `vehicleModel` int(10) unsigned NOT NULL,
  `time` double(6,3) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ping` int(11) DEFAULT NULL,
  UNIQUE KEY `id_player` (`id_player`,`vehicleModel`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_drift_records`
--

CREATE TABLE IF NOT EXISTS `fs_drift_records` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `playerid` int(10) unsigned NOT NULL,
  `raceid` smallint(10) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `recordtime` float unsigned NOT NULL,
  `opponents` mediumint(9) NOT NULL DEFAULT '1',
  `finalscore` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `raceid` (`raceid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_drift_tracks`
--

CREATE TABLE IF NOT EXISTS `fs_drift_tracks` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `minPlayers` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `maxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '10',
  `vehicle` smallint(5) unsigned NOT NULL DEFAULT '444',
  `interior` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `descr` varchar(255) DEFAULT NULL,
  `nitro` tinyint(1) NOT NULL DEFAULT '0',
  `allowRepairs` tinyint(1) NOT NULL DEFAULT '0',
  `allowFlip` tinyint(1) NOT NULL DEFAULT '0',
  `distance` double unsigned NOT NULL DEFAULT '100',
  `scx` double DEFAULT NULL,
  `scy` double DEFAULT NULL,
  `scz` double DEFAULT NULL,
  `limitrand` tinyint(4) NOT NULL DEFAULT '100',
  `active` tinyint(1) NOT NULL DEFAULT '1',
  `partime` float unsigned NOT NULL DEFAULT '100',
  PRIMARY KEY (`id`),
  KEY `minPlayers` (`minPlayers`,`maxPlayers`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=15 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_drift_tracks_cp`
--

CREATE TABLE IF NOT EXISTS `fs_drift_tracks_cp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `type` smallint(3) unsigned NOT NULL DEFAULT '0',
  `size` double NOT NULL DEFAULT '0',
  `so` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=234 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_drift_tracks_sp`
--

CREATE TABLE IF NOT EXISTS `fs_drift_tracks_sp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `angle` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=46 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_gangs`
--

CREATE TABLE IF NOT EXISTS `fs_gangs` (
  `id` smallint(1) unsigned NOT NULL,
  `name` varchar(32) NOT NULL,
  `tag` varchar(5) NOT NULL,
  `color` char(6) NOT NULL DEFAULT '000000',
  `respect` int(11) NOT NULL DEFAULT '0',
  `datetime_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `basespawn` varchar(64) NOT NULL DEFAULT '2023,1008,12,270' COMMENT 'Miejsce spawnu gangu - WYSPA',
  `spawnpoint` varchar(64) NOT NULL DEFAULT '2023,1008,11,270',
  `active` tinyint(4) NOT NULL DEFAULT '1',
  `www` varchar(30) NOT NULL,
  `wyspa_sphere` varchar(64) NOT NULL COMMENT 'kula/sfera: x,y,z,r',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  UNIQUE KEY `tag` (`tag`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_gangs_alliance`
--

CREATE TABLE IF NOT EXISTS `fs_gangs_alliance` (
  `g1` smallint(2) unsigned NOT NULL,
  `g2` smallint(2) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`g1`,`g2`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_houses`
--

CREATE TABLE IF NOT EXISTS `fs_houses` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `descr` varchar(64) CHARACTER SET utf8 DEFAULT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `ownerid` int(10) unsigned DEFAULT NULL,
  `koszt` smallint(4) unsigned NOT NULL DEFAULT '0',
  `exitX` float NOT NULL DEFAULT '0',
  `exitY` float NOT NULL DEFAULT '0',
  `exitZ` float NOT NULL DEFAULT '0',
  `exitA` float NOT NULL DEFAULT '0',
  `paidTo` date DEFAULT NULL,
  `homeX` float NOT NULL,
  `homeY` float NOT NULL,
  `homeZ` float NOT NULL,
  `homeA` float NOT NULL,
  `homeI` smallint(6) NOT NULL,
  `homeVW` mediumint(9) NOT NULL DEFAULT '-1',
  `vehicles_allowed` tinyint(4) NOT NULL DEFAULT '1',
  `vehicle_radius` double unsigned NOT NULL DEFAULT '20',
  `audioURL` varchar(128) DEFAULT NULL,
  `restrict_gang` tinyint(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `ownerid` (`ownerid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=905 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_houses_vehicles`
--

CREATE TABLE IF NOT EXISTS `fs_houses_vehicles` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `houseid` int(10) unsigned NOT NULL,
  `model` smallint(5) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `A` double NOT NULL,
  `color1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `color2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `plate` varchar(32) NOT NULL DEFAULT 'bryka',
  `components` varchar(80) NOT NULL DEFAULT ' ',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=10907 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_ipbans`
--

CREATE TABLE IF NOT EXISTS `fs_ipbans` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(24) NOT NULL,
  `player_given` int(11) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_end` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `reason` text NOT NULL,
  `target_nick` varchar(32) DEFAULT NULL,
  `target_accountid` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `date_created` (`date_created`,`date_end`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_iplocks`
--

CREATE TABLE IF NOT EXISTS `fs_iplocks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip_class` varchar(16) NOT NULL DEFAULT 'NONE' COMMENT 'tylko nie za zbyt szerokie ;-)',
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment` tinytext NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `serial` (`ip_class`),
  KEY `date_created` (`date_created`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_mapicons`
--

CREATE TABLE IF NOT EXISTS `fs_mapicons` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mapicon` smallint(6) NOT NULL DEFAULT '1',
  `pX` double NOT NULL,
  `pY` double NOT NULL,
  `pZ` double NOT NULL,
  `pi` mediumint(9) NOT NULL DEFAULT '0',
  `pvw` mediumint(9) NOT NULL DEFAULT '0',
  `opis` varchar(64) DEFAULT NULL,
  `type` smallint(1) unsigned NOT NULL DEFAULT '1',
  `loadingdistance` int(10) unsigned NOT NULL DEFAULT '1200',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=68 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_miscpickups`
--

CREATE TABLE IF NOT EXISTS `fs_miscpickups` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `pickupid` mediumint(8) unsigned NOT NULL DEFAULT '371',
  `interior` int(11) NOT NULL DEFAULT '0',
  `vw` int(11) NOT NULL DEFAULT '0',
  `pX` double NOT NULL,
  `pY` double NOT NULL,
  `pZ` double NOT NULL,
  `descr` varchar(64) DEFAULT NULL,
  `addedby` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=841 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_mutes`
--

CREATE TABLE IF NOT EXISTS `fs_mutes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `player_muted` int(11) NOT NULL,
  `player_given` int(11) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_end` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `reason` text NOT NULL,
  PRIMARY KEY (`id`),
  KEY `player_banned` (`player_muted`,`date_created`,`date_end`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_paczki`
--

CREATE TABLE IF NOT EXISTS `fs_paczki` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `i` smallint(5) unsigned NOT NULL DEFAULT '0',
  `vw` int(10) unsigned NOT NULL DEFAULT '0',
  `opis` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=134 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_paczki_gracze`
--

CREATE TABLE IF NOT EXISTS `fs_paczki_gracze` (
  `id_paczki` int(10) unsigned NOT NULL,
  `id_gracza` int(10) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id_paczki`,`id_gracza`),
  KEY `idg` (`id_gracza`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_players`
--

CREATE TABLE IF NOT EXISTS `fs_players` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nick` varchar(24) CHARACTER SET ascii NOT NULL,
  `email` varchar(56) NOT NULL,
  `password` varchar(32) NOT NULL,
  `ip_registered` varchar(16) NOT NULL,
  `ip_last` varchar(16) NOT NULL,
  `serial_registered` varchar(50) NOT NULL,
  `serial_last` varchar(50) NOT NULL,
  `datetime_registered` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `datetime_last` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `session` bigint(11) NOT NULL,
  `level` tinyint(1) unsigned NOT NULL,
  `suspendedTo` timestamp NULL DEFAULT NULL COMMENT 'zawieszenie rangi do daty',
  `vip` date NOT NULL,
  `doubleMode` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'tryb furii',
  `ban_count` mediumint(9) unsigned NOT NULL,
  `kick_count` mediumint(9) unsigned NOT NULL,
  `login_count` mediumint(9) unsigned NOT NULL,
  `kill_count` mediumint(9) unsigned NOT NULL,
  `teamkill_count` mediumint(9) unsigned NOT NULL,
  `death_count` mediumint(9) unsigned NOT NULL,
  `suicide_count` mediumint(9) unsigned NOT NULL,
  `respect` int(11) unsigned NOT NULL,
  `gamep` int(11) unsigned NOT NULL,
  `skill` mediumint(9) unsigned NOT NULL,
  `next_nick_change` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `bank_money` decimal(32,0) unsigned NOT NULL,
  `wallet_money` int(11) unsigned NOT NULL,
  `hitman_prize` int(11) unsigned NOT NULL,
  `jail` mediumint(9) NOT NULL DEFAULT '-1',
  `last_skin` smallint(3) NOT NULL DEFAULT '-1',
  `active_server` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `zaloz` varchar(32) DEFAULT NULL,
  `spawnData` tinyint(1) unsigned NOT NULL DEFAULT '5',
  `selectedWeap` smallint(1) unsigned NOT NULL DEFAULT '1',
  `hudData` varchar(64) NOT NULL DEFAULT '1,1,1,1,1,1,1,1,1,1',
  `isLocked` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'blokada konta',
  PRIMARY KEY (`id`),
  UNIQUE KEY `nick` (`nick`),
  KEY `session` (`session`),
  KEY `respect` (`respect`),
  KEY `skill` (`skill`),
  KEY `ip_last` (`ip_last`),
  KEY `serial_last` (`serial_last`),
  KEY `gamep` (`gamep`),
  KEY `serial_registered` (`serial_registered`),
  KEY `ip_registered` (`ip_registered`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_players_achievements`
--

CREATE TABLE IF NOT EXISTS `fs_players_achievements` (
  `playerid` int(10) unsigned NOT NULL,
  `shortname` enum('DERBYFAN','DERBYWIN','KILLS','LONGPLAY','RACEDIST','RACEFCNT','WALIZKI','FOTOCASH','DRIFTPTS','LISTONOSZ','CHOFAN','CHOWIN','QUIZ','PACZKI') CHARACTER SET ascii NOT NULL,
  `score` int(10) unsigned NOT NULL,
  PRIMARY KEY (`playerid`,`shortname`),
  KEY `shortname` (`shortname`,`score`),
  KEY `playerid` (`playerid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_players_arenascore`
--

CREATE TABLE IF NOT EXISTS `fs_players_arenascore` (
  `id_player` int(10) unsigned NOT NULL,
  `id_arena` smallint(5) unsigned NOT NULL,
  `kills` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id_player`,`id_arena`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_players_arenascore_week`
--

CREATE TABLE IF NOT EXISTS `fs_players_arenascore_week` (
  `id_player` int(10) unsigned NOT NULL,
  `id_arena` smallint(5) unsigned NOT NULL,
  `kills` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id_player`,`id_arena`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_players_in_gangs`
--

CREATE TABLE IF NOT EXISTS `fs_players_in_gangs` (
  `id_gang` smallint(1) unsigned NOT NULL,
  `id_player` int(10) unsigned NOT NULL,
  `rank` enum('member','owner','leader','suspended','viceowner') NOT NULL DEFAULT 'member',
  `join_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id_gang`,`id_player`),
  KEY `player_in_gang` (`id_player`,`rank`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_poczta`
--

CREATE TABLE IF NOT EXISTS `fs_poczta` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `nadawca` int(10) unsigned NOT NULL,
  `odbiorca` int(10) unsigned NOT NULL,
  `tresc` varchar(1024) CHARACTER SET cp1250 COLLATE cp1250_polish_ci NOT NULL,
  `dostarczone` tinyint(1) NOT NULL DEFAULT '0',
  `przeczytane` tinyint(1) NOT NULL DEFAULT '0',
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_polecenia`
--

CREATE TABLE IF NOT EXISTS `fs_polecenia` (
  `polecona` int(11) unsigned NOT NULL,
  `polecil` int(11) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ip` varchar(32) DEFAULT NULL,
  `ua` varchar(128) DEFAULT NULL,
  `rewarded` tinyint(4) NOT NULL DEFAULT '0',
  `reward_date` timestamp NULL DEFAULT NULL,
  `uniewazniony` tinyint(4) NOT NULL DEFAULT '0',
  `uniewazniony_powod` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`polecona`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_quiz`
--

CREATE TABLE IF NOT EXISTS `fs_quiz` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `kategoria` varchar(64) CHARACTER SET utf8 NOT NULL,
  `pytanie` varchar(255) CHARACTER SET utf8 NOT NULL,
  `odpowiedz` varchar(64) CHARACTER SET utf8 NOT NULL,
  `lu` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `kategoria` (`kategoria`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1612 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_races`
--

CREATE TABLE IF NOT EXISTS `fs_races` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `minPlayers` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `maxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '10',
  `vehicle` smallint(5) unsigned NOT NULL DEFAULT '444',
  `interior` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `descr` varchar(255) DEFAULT NULL,
  `nitro` tinyint(1) NOT NULL DEFAULT '0',
  `allowRepairs` tinyint(1) NOT NULL DEFAULT '0',
  `allowFlip` tinyint(1) NOT NULL DEFAULT '0',
  `distance` double unsigned NOT NULL DEFAULT '100',
  `scx` double DEFAULT NULL,
  `scy` double DEFAULT NULL,
  `scz` double DEFAULT NULL,
  `limitrand` tinyint(4) NOT NULL DEFAULT '100',
  `active` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `minPlayers` (`minPlayers`,`maxPlayers`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_races_cp`
--

CREATE TABLE IF NOT EXISTS `fs_races_cp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `type` smallint(3) unsigned NOT NULL DEFAULT '0',
  `size` double NOT NULL DEFAULT '0',
  `so` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4260 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_races_records`
--

CREATE TABLE IF NOT EXISTS `fs_races_records` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `playerid` int(10) unsigned NOT NULL,
  `raceid` smallint(10) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `recordtime` float unsigned NOT NULL,
  `opponents` mediumint(9) NOT NULL DEFAULT '1',
  `finalposition` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `raceid` (`raceid`),
  KEY `i1` (`playerid`,`raceid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_races_sp`
--

CREATE TABLE IF NOT EXISTS `fs_races_sp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `angle` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1477 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_returnpickups`
--

CREATE TABLE IF NOT EXISTS `fs_returnpickups` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `pX` double NOT NULL,
  `pY` double NOT NULL,
  `pZ` double NOT NULL,
  `pi` int(11) NOT NULL,
  `pvw` int(11) NOT NULL,
  `opis` varchar(64) DEFAULT NULL,
  `pickupid` int(10) unsigned NOT NULL DEFAULT '19197',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=152 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_sbans`
--

CREATE TABLE IF NOT EXISTS `fs_sbans` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `serial` varchar(60) NOT NULL DEFAULT 'NONE',
  `ip_class` varchar(16) NOT NULL DEFAULT '.' COMMENT 'Domyslnie: "." - kazdy adres IP',
  `nick_exc` varchar(512) NOT NULL DEFAULT 'none;none;none;none;none;none;none;none;none;none;none;none;none;none;none' COMMENT 'nicki wylaczone z banowania || oddzielaj znakiem ; || max 15',
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `comment` tinytext NOT NULL,
  `exception` int(1) NOT NULL DEFAULT '0' COMMENT 'pomijac banowanie ip po wejsciu gracza z serialem?',
  PRIMARY KEY (`id`),
  UNIQUE KEY `serial` (`serial`,`ip_class`),
  KEY `date_created` (`date_created`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_sluby`
--

CREATE TABLE IF NOT EXISTS `fs_sluby` (
  `id_player1` int(11) NOT NULL,
  `id_player2` int(11) NOT NULL,
  UNIQUE KEY `para` (`id_player1`,`id_player2`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_solo_matches`
--

CREATE TABLE IF NOT EXISTS `fs_solo_matches` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `arenaid` smallint(5) unsigned NOT NULL,
  `weapon1` tinyint(3) unsigned NOT NULL,
  `weapon2` tinyint(3) unsigned NOT NULL,
  `playerid` int(10) unsigned NOT NULL,
  `killerid` int(10) unsigned NOT NULL,
  `fightlen` mediumint(8) unsigned NOT NULL,
  `hp` tinyint(3) unsigned NOT NULL,
  `ar` tinyint(3) unsigned NOT NULL,
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_stats`
--

CREATE TABLE IF NOT EXISTS `fs_stats` (
  `option_name` varchar(32) NOT NULL,
  `value` blob,
  PRIMARY KEY (`option_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_telecheckpoints`
--

CREATE TABLE IF NOT EXISTS `fs_telecheckpoints` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `fPOS` varchar(64) NOT NULL COMMENT 'X,Y,Z,A,interior',
  `fOPIS` varchar(64) DEFAULT NULL COMMENT 'opis na 3dtextlabel i gametext',
  `dPOS` varchar(64) NOT NULL,
  `dOPIS` varchar(64) DEFAULT NULL,
  `rozmiar` smallint(5) unsigned NOT NULL DEFAULT '8',
  `aktywny` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=32 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_telepickups`
--

CREATE TABLE IF NOT EXISTS `fs_telepickups` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `pX` double NOT NULL COMMENT 'Wspolrzedne pickupu',
  `pY` double NOT NULL,
  `pZ` double NOT NULL,
  `pi` int(11) NOT NULL COMMENT 'interior pickupu',
  `pvw` int(11) NOT NULL COMMENT 'vw pickupu',
  `opis` varchar(64) DEFAULT NULL COMMENT 'opis tekstowy',
  `pokazopis` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'czy opis ma sie pojawic przy pickupie',
  `pickupid` int(10) unsigned NOT NULL DEFAULT '19197' COMMENT 'id pickupu, domyslnie 19197',
  `mapicon` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'ikonka na mapie, 0 - brak. lista pod http://wiki.sa-mp.com/wiki/MapIcons',
  `tX` double NOT NULL COMMENT 'dokad ma przenosic - wspolrzedne',
  `tY` double NOT NULL,
  `tZ` double NOT NULL,
  `ti` int(11) NOT NULL COMMENT 'dokad - interior',
  `tvw` mediumint(9) NOT NULL,
  `tA` double NOT NULL COMMENT 'dokad - kierunek patrzenia po przeniesieniu',
  `mapicon_type` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `opis_color` int(10) unsigned NOT NULL DEFAULT '16732697',
  `opis_drawdistance` smallint(5) unsigned NOT NULL DEFAULT '600',
  `restrict_gang` int(10) unsigned NOT NULL DEFAULT '0',
  `restrict_level` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=997 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_vehicles`
--

CREATE TABLE IF NOT EXISTS `fs_vehicles` (
  `vid` smallint(3) unsigned NOT NULL,
  `name` varchar(32) DEFAULT NULL,
  `altnames` varchar(64) DEFAULT NULL,
  `cena` mediumint(8) unsigned NOT NULL DEFAULT '10000',
  `minPoziom` enum('gracz','vip','gm','admin') NOT NULL DEFAULT 'gracz',
  `jezdzi` tinyint(1) NOT NULL DEFAULT '1',
  `lata` tinyint(1) NOT NULL DEFAULT '0',
  `plywa` tinyint(1) NOT NULL DEFAULT '0',
  `wojskowy` tinyint(1) NOT NULL DEFAULT '0',
  `przyczepa` tinyint(1) NOT NULL DEFAULT '0',
  `tablica` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`vid`),
  KEY `minPoziom` (`minPoziom`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Zastąpiona struktura widoku `fs_view_housesandvehicles`
--
CREATE TABLE IF NOT EXISTS `fs_view_housesandvehicles` (
`id` int(10) unsigned
,`X` double
,`Y` double
,`Z` double
,`exitX` float
,`exitY` float
,`exitZ` float
,`exitA` float
,`ownerid` decimal(10,0)
,`koszt` smallint(4) unsigned
,`nick` varchar(24)
,`IFNULL
(h.paidTo,'-')` varchar(10)
,`IFNULL(DATEDIFF(h.paidTo,NOW()),-1)` int(7)
,`homeX` float
,`homeY` float
,`homeZ` float
,`homeA` float
,`homeI` smallint(6)
,`homeVW` mediumint(9)
,`vehicles_allowed` tinyint(4)
,`vehicle_radius` double unsigned
,`IFNULL(hv.model,0)` decimal(5,0)
,`IFNULL(hv.X,0)` double
,`IFNULL(hv.Y,0)` double
,`IFNULL(hv.Z,0)` double
,`IFNULL(hv.A,0)` double
,`IFNULL(hv.color1,0)` decimal(5,0)
,`IFNULL(hv.color2,0)` decimal(5,0)
,`IFNULL(hv.plate,"-
")` varchar(32)
,`IFNULL(audioURL,_latin1'-')` varchar(128)
,`IFNULL(``hv``.``components``,"-")` varchar(80)
,`restrict_gang` tinyint(2) unsigned
);
-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_wg_arena`
--

CREATE TABLE IF NOT EXISTS `fs_wg_arena` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `descr` varchar(64) NOT NULL,
  `interior` smallint(5) unsigned NOT NULL DEFAULT '0',
  `minplayers` smallint(5) unsigned NOT NULL DEFAULT '2',
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT '2',
  `wb_cube` varchar(64) CHARACTER SET ascii COLLATE ascii_bin DEFAULT NULL COMMENT 'prostopadloscian x1,y1,z1,x2,y2,z2',
  `wb_mode` enum('oraz','lub') CHARACTER SET ascii NOT NULL DEFAULT 'lub' COMMENT 'oraz - gracz musi byc w obu polach, lub - musi byc w jednym z nich',
  `wb_sphere` varchar(64) CHARACTER SET ascii DEFAULT NULL COMMENT 'kula/sfera: x,y,z,r',
  PRIMARY KEY (`id`),
  KEY `minplayers` (`minplayers`,`maxplayers`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=86 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_wg_arena_sp`
--

CREATE TABLE IF NOT EXISTS `fs_wg_arena_sp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `aid` int(10) unsigned NOT NULL,
  `team` tinyint(1) unsigned NOT NULL,
  `X` double NOT NULL,
  `Y` double NOT NULL,
  `Z` double NOT NULL,
  `A` double NOT NULL,
  `randomorder` tinyint(3) unsigned NOT NULL COMMENT 'wartosc losowa, aktualizowana automatycznie',
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`,`team`,`randomorder`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3338 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_zones`
--

CREATE TABLE IF NOT EXISTS `fs_zones` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `miny` double NOT NULL,
  `minx` double NOT NULL,
  `maxy` double NOT NULL,
  `maxx` double NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=286 ;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `fs_zones_gangscore`
--

CREATE TABLE IF NOT EXISTS `fs_zones_gangscore` (
  `id_zone` int(10) unsigned NOT NULL,
  `id_gang` smallint(5) unsigned NOT NULL,
  `respect` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_zone`,`id_gang`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura widoku `fs_view_housesandvehicles`
--
DROP TABLE IF EXISTS `fs_view_housesandvehicles`;

CREATE VIEW `fs_view_housesandvehicles` AS select `h`.`id` AS `id`,`h`.`X` AS `X`,`h`.`Y` AS `Y`,`h`.`Z` AS `Z`,`h`.`exitX` AS `exitX`,`h`.`exitY` AS `exitY`,`h`.`exitZ` AS `exitZ`,`h`.`exitA` AS `exitA`,ifnull(`h`.`ownerid`,0) AS `ownerid`,`h`.`koszt` AS `koszt`,ifnull(`p`.`nick`,_ascii'nn') AS `nick`,ifnull(`h`.`paidTo`,_utf8'-') AS `IFNULL
(h.paidTo,'-')`,ifnull((to_days(`h`.`paidTo`) - to_days(now())),-(1)) AS `IFNULL(DATEDIFF(h.paidTo,NOW()),-1)`,`h`.`homeX` AS `homeX`,`h`.`homeY` AS `homeY`,`h`.`homeZ` AS `homeZ`,`h`.`homeA` AS `homeA`,`h`.`homeI` AS `homeI`,`h`.`homeVW` AS `homeVW`,`h`.`vehicles_allowed` AS `vehicles_allowed`,`h`.`vehicle_radius` AS `vehicle_radius`,ifnull(`hv`.`model`,0) AS `IFNULL(hv.model,0)`,ifnull(`hv`.`X`,0) AS `IFNULL(hv.X,0)`,ifnull(`hv`.`Y`,0) AS `IFNULL(hv.Y,0)`,ifnull(`hv`.`Z`,0) AS `IFNULL(hv.Z,0)`,ifnull(`hv`.`A`,0) AS `IFNULL(hv.A,0)`,ifnull(`hv`.`color1`,0) AS `IFNULL(hv.color1,0)`,ifnull(`hv`.`color2`,0) AS `IFNULL(hv.color2,0)`,ifnull(`hv`.`plate`,_latin1'-') AS `IFNULL(hv.plate,"-
")`,ifnull(`h`.`audioURL`,_latin1'-') AS `IFNULL(audioURL,_latin1'-')`,ifnull(`hv`.`components`,_latin1'-') AS `IFNULL(``hv``.``components``,"-")`,`h`.`restrict_gang` AS `restrict_gang` from ((`fs_houses` `h` left join `fs_players` `p` on((`p`.`id` = `h`.`ownerid`))) left join `fs_houses_vehicles` `hv` on(((`hv`.`houseid` = `h`.`id`) and (`h`.`ownerid` > 0) and (`h`.`paidTo` > cast(now() as date)))));

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
