-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Hostiteľ: localhost
-- Vygenerované: Št 05.Apr 2018, 20:48
-- Verzia serveru: 5.5.59-0ubuntu0.14.04.1
-- Verzia PHP: 5.5.9-1ubuntu4.24

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Databáza: `fitcrack`
--

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_benchmark`
--

CREATE TABLE IF NOT EXISTS `fc_benchmark` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `hash_type` int(11) unsigned DEFAULT NULL,
  `power` bigint(20) unsigned NOT NULL DEFAULT '0',
  `last_update` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_charset`
--

CREATE TABLE IF NOT EXISTS `fc_charset` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `path` varchar(400) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_dictionary`
--

CREATE TABLE IF NOT EXISTS `fc_dictionary` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `path` varchar(400) NOT NULL,
  `keyspace` bigint(20) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  `modification_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_hashcache`
--

CREATE TABLE IF NOT EXISTS `fc_hashcache` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `hash_type` int(11) unsigned DEFAULT NULL,
  `hash` longtext COLLATE utf8_bin,
  `result` text COLLATE utf8_bin,
  `added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_hash`
--

CREATE TABLE IF NOT EXISTS`fc_hash` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `hash_type` int(11) unsigned NOT NULL,
  `hash` blob NOT NULL,
  `result` longtext DEFAULT NULL,
  `added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `time_cracked` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_hcstats`
--

CREATE TABLE IF NOT EXISTS `fc_hcstats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `path` varchar(400) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_host`
--

CREATE TABLE IF NOT EXISTS `fc_host` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `power` bigint(20) unsigned NOT NULL DEFAULT '0',
  `job_id` bigint(20) unsigned NOT NULL,
  `status` smallint(1) unsigned NOT NULL DEFAULT '0',
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_host_activity`
--

CREATE TABLE IF NOT EXISTS `fc_host_activity` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `job_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_workunit`
--

CREATE TABLE IF NOT EXISTS `fc_workunit` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) NOT NULL,
  `workunit_id` bigint(20) unsigned NOT NULL,
  `host_id` bigint(20) unsigned NOT NULL,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `start_index` bigint(20) unsigned NOT NULL,
  `start_index_2` bigint(20) unsigned NOT NULL,
  `hc_keyspace` bigint(20) unsigned NOT NULL,
  `progress` double NOT NULL DEFAULT '0',
  `mask_id` bigint(20) unsigned NOT NULL,
  `dictionary_id` bigint(20) unsigned NOT NULL,
  `duplicated` tinyint(1) NOT NULL DEFAULT '0',
  `duplicate` bigint(20) unsigned NOT NULL DEFAULT '0',
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cracking_time` double unsigned NOT NULL DEFAULT '0',
  `retry` int(1) NOT NULL DEFAULT '0',
  `finished` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_mask`
--

CREATE TABLE IF NOT EXISTS `fc_mask` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `mask` varchar(30) COLLATE utf8_bin DEFAULT NULL,
  `current_index` bigint(20) unsigned NOT NULL,
  `keyspace` bigint(20) unsigned NOT NULL,
  `hc_keyspace` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_masks_set`
--

CREATE TABLE IF NOT EXISTS `fc_masks_set` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `path` varchar(400) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_notification`
--

CREATE TABLE IF NOT EXISTS `fc_notification` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) DEFAULT NULL,
  `source_type` int(11) DEFAULT '0',
  `source_id` bigint(20) unsigned DEFAULT NULL,
  `old_value` smallint(6) DEFAULT NULL,
  `new_value` smallint(6) DEFAULT NULL,
  `seen` tinyint(1) DEFAULT '0',
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`),
  KEY `source_id` (`source_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_protected_file`
--

CREATE TABLE IF NOT EXISTS `fc_protected_file` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `path` varchar(400) NOT NULL,
  `hash` varchar(4000) NOT NULL,
  `hash_type` varchar(50) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_job`
--

CREATE TABLE IF NOT EXISTS `fc_job` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `token` varchar(64) COLLATE utf8_bin DEFAULT NULL,
  `attack` varchar(40) COLLATE utf8_bin NOT NULL,
  `attack_mode` tinyint(3) unsigned NOT NULL,
  `attack_submode` tinyint(3) NOT NULL DEFAULT '0',
  `hash_type` int(10) unsigned NOT NULL,
  `hash` longtext COLLATE utf8_bin NOT NULL,
  `status` smallint(1) unsigned NOT NULL DEFAULT '0',
  `result` text COLLATE utf8_bin,
  `keyspace` bigint(20) unsigned NOT NULL,
  `hc_keyspace` bigint(20) unsigned NOT NULL,
  `indexes_verified` bigint(20) unsigned NOT NULL,
  `current_index` bigint(20) unsigned NOT NULL DEFAULT '0',
  `current_index_2` bigint(20) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `name` text COLLATE utf8_bin NOT NULL,
  `comment` text COLLATE utf8_bin NOT NULL,
  `time_start` timestamp NULL DEFAULT NULL,
  `time_end` timestamp NULL DEFAULT NULL,
  `cracking_time` double NOT NULL DEFAULT '0',
  `seconds_per_workunit` bigint(20) unsigned NOT NULL DEFAULT '3600',
  `config` longtext COLLATE utf8_bin NOT NULL,
  `dict1` varchar(255) COLLATE utf8_bin NOT NULL,
  `dict2` varchar(255) COLLATE utf8_bin NOT NULL,
  `charset1` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `charset2` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `charset3` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `charset4` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `rules` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `rule_left` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `rule_right` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `markov_hcstat` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `markov_threshold` int(20) NOT NULL DEFAULT '0',
  `replicate_factor` int(10) unsigned NOT NULL DEFAULT '1',
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_job_dictionary`
--

CREATE TABLE `fc_job_dictionary` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `dictionary_id` bigint(20) unsigned NOT NULL,
  `current_index` bigint(20) unsigned NOT NULL DEFAULT '0',
  `is_left` tinyint(1) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_job_graph`
--

CREATE TABLE IF NOT EXISTS `fc_job_graph` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `progress` decimal(5,2) NOT NULL,
  `job_id` bigint(20) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `job_id` (`job_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_role`
--

CREATE TABLE IF NOT EXISTS `fc_role` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `MANAGE_USERS` tinyint(1) NOT NULL DEFAULT '0',
  `ADD_NEW_JOB` tinyint(1) NOT NULL DEFAULT '0',
  `UPLOAD_DICTIONARIES` tinyint(1) NOT NULL DEFAULT '0',
  `VIEW_ALL_JOBS` tinyint(1) NOT NULL DEFAULT '0',
  `EDIT_ALL_JOBS` tinyint(1) NOT NULL DEFAULT '0',
  `OPERATE_ALL_JOBS` tinyint(1) NOT NULL DEFAULT '0',
  `ADD_USER_PERMISSIONS_TO_JOB` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_rule`
--

CREATE TABLE IF NOT EXISTS `fc_rule` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `path` varchar(400) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_settings`
--

CREATE TABLE IF NOT EXISTS `fc_settings` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `delete_finished_workunits` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `default_seconds_per_workunit` int(10) unsigned NOT NULL DEFAULT '3600',
  `default_replicate_factor` int(10) unsigned NOT NULL DEFAULT '1',
  `default_verify_hash_format` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `default_check_hashcache` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `default_workunit_timeout_factor` int(10) unsigned NOT NULL DEFAULT '6',
  `default_bench_all` tinyint(1) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_user`
--

CREATE TABLE IF NOT EXISTS `fc_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(64) NOT NULL,
  `password` varchar(128) NOT NULL,
  `mail` varchar(200) NOT NULL,
  `role_id` int(11) NOT NULL,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_user_permissions`
--

CREATE TABLE IF NOT EXISTS `fc_user_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `user_id` int(11) NOT NULL,
  `view` tinyint(1) NOT NULL DEFAULT '0',
  `modify` tinyint(1) NOT NULL DEFAULT '0',
  `operate` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `job_id` (`job_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Štruktúra tabuľky pre tabuľku `fc_host_status`
--

CREATE TABLE IF NOT EXISTS `fc_host_status` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `last_seen` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Obmedzenie pre exportované tabuľky
--

--
-- Obmedzenie pre tabuľku `fc_notification`
--
ALTER TABLE `fc_notification`
  ADD CONSTRAINT `fc_notification_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `fc_user` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fc_notification_ibfk_2` FOREIGN KEY (`source_id`) REFERENCES `fc_job` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Obmedzenie pre tabuľku `fc_job_graph`
--
ALTER TABLE `fc_job_graph`
  ADD CONSTRAINT `fc_job_graph_ibfk_1` FOREIGN KEY (`job_id`) REFERENCES `fc_job` (`id`);

--
-- Obmedzenie pre tabuľku `fc_user`
--
ALTER TABLE `fc_user`
  ADD CONSTRAINT `fc_user_ibfk_1` FOREIGN KEY (`role_id`) REFERENCES `fc_role` (`id`);

--
-- Obmedzenie pre tabuľku `fc_user_permissions`
--
ALTER TABLE `fc_user_permissions`
  ADD CONSTRAINT `fc_user_permissions_ibfk_1` FOREIGN KEY (`job_id`) REFERENCES `fc_job` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fc_user_permissions_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `fc_user` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;


