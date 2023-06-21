SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;


-- --------------------------------------------------------

--
-- Table definition for `fc_benchmark`
--

CREATE TABLE IF NOT EXISTS `fc_benchmark` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `hash_type` int(11) unsigned DEFAULT NULL,
  `attack_mode` tinyint(3) unsigned DEFAULT NULL,
  `power` bigint(20) unsigned NOT NULL DEFAULT '0',
  `last_update` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_charset`
--

CREATE TABLE IF NOT EXISTS `fc_charset` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `path` varchar(400) NOT NULL,
  `keyspace` bigint(20) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_dictionary`
--

CREATE TABLE IF NOT EXISTS `fc_dictionary` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `path` varchar(400) NOT NULL,
  `password_distribution` text COLLATE utf8_bin NOT NULL,
  `keyspace` bigint(20) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `hex_dict` tinyint(1) NOT NULL DEFAULT '0',
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_hash`
--

CREATE TABLE IF NOT EXISTS`fc_hash` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `hash_type` int(11) unsigned NOT NULL,
  `hash` longblob NOT NULL,
  `result` longtext DEFAULT NULL,
  `added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `time_cracked` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_hcstats`
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
-- Table definition for `fc_host`
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
-- Table definition for `fc_device`
--

CREATE TABLE IF NOT EXISTS `fc_device` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `hc_id` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_device_info`
--

CREATE TABLE IF NOT EXISTS `fc_device_info` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `device_id` bigint(20) unsigned NOT NULL,   -- fc_device
  `workunit_id` bigint(20) unsigned NOT NULL, -- fc_workunit
  `speed` bigint(20) NOT NULL,
  `temperature` int(11) NOT NULL,
  `utilization` int(11) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_host_activity`
--

CREATE TABLE IF NOT EXISTS `fc_host_activity` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `boinc_host_id` bigint(20) unsigned NOT NULL,
  `job_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_workunit`
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
  `speed` bigint(20) unsigned NOT NULL DEFAULT '0',
  `remaining_time` bigint(20) unsigned NOT NULL DEFAULT '0',
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
-- Table definition for `fc_mask`
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
-- Table definition for `fc_masks_set`
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
-- Table definition for `fc_notification`
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
-- Table definition for `fc_protected_file`
--

CREATE TABLE IF NOT EXISTS `fc_protected_file` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `path` varchar(400) NOT NULL,
  `hash` longblob NOT NULL,
  `hash_type` varchar(50) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;


-- --------------------------------------------------------

--
-- Table definition for `fc_bin`
--

CREATE TABLE IF NOT EXISTS `fc_bin` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `position` int(11),
  primary key (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;


-- --------------------------------------------------------

--
-- Table definition for `fc_batch`
--

CREATE TABLE IF NOT EXISTS `fc_batch` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `creator_id` int(11),
  primary key (`id`),
  key `creator_id` (`creator_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Table definition for `fc_job`
--

CREATE TABLE IF NOT EXISTS `fc_job` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `attack` varchar(40) COLLATE utf8_bin NOT NULL,
  `attack_mode` tinyint(3) unsigned NOT NULL,
  `attack_submode` tinyint(3) NOT NULL DEFAULT '0',
  `distribution_mode` tinyint(3) NOT NULL DEFAULT '0',
  `hash_type` int(10) unsigned NOT NULL,
  `status` smallint(1) unsigned NOT NULL DEFAULT '0',
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
  `workunit_sum_time` double NOT NULL DEFAULT '0',
  `seconds_per_workunit` bigint(20) unsigned NOT NULL DEFAULT '3600',
  `charset1` varchar(4096) COLLATE utf8_bin DEFAULT NULL,
  `charset2` varchar(4096) COLLATE utf8_bin DEFAULT NULL,
  `charset3` varchar(4096) COLLATE utf8_bin DEFAULT NULL,
  `charset4` varchar(4096) COLLATE utf8_bin DEFAULT NULL,
  `rules` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `rule_left` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `rule_right` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `markov_hcstat` varchar(255) COLLATE utf8_bin DEFAULT NULL,
  `markov_threshold` int(20) NOT NULL DEFAULT '0',
  `grammar_id` bigint(20) unsigned DEFAULT NULL,
  `case_permute` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `check_duplicates` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `min_password_len` int(10) unsigned NOT NULL DEFAULT '1',
  `max_password_len` int(10) unsigned NOT NULL DEFAULT '8',
  `min_elem_in_chain` int(10) unsigned NOT NULL DEFAULT '1',
  `max_elem_in_chain` int(10) unsigned NOT NULL DEFAULT '8',
  `generate_random_rules` int(10) unsigned NOT NULL DEFAULT '0',
  `dict_deployment_mode` tinyint(3) NOT NULL DEFAULT '0',
  `optimized` tinyint(1) NOT NULL DEFAULT '1',
  `device_types` int(10) unsigned NOT NULL DEFAULT '1',
  `workload_profile` tinyint(3) NOT NULL DEFAULT '2',
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  `kill` int(11) NOT NULL DEFAULT '0',
  `batch_id` int(11),
  `queue_position` int(11),
  PRIMARY KEY (`id`),
  KEY `batch_id` (`batch_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

--
-- Table definition for `fc_bin_job` (M2M junction)
--

CREATE TABLE IF NOT EXISTS `fc_bin_job` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `bin_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `job_id` (`job_id`),
  KEY `bin_id` (`bin_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;



-- --------------------------------------------------------

--
-- Table definition for `fc_job_dictionary`
--

CREATE TABLE IF NOT EXISTS `fc_job_dictionary` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `dictionary_id` bigint(20) unsigned NOT NULL,
  `current_index` bigint(20) unsigned NOT NULL DEFAULT '0',
  `current_pos` bigint(20) unsigned NOT NULL DEFAULT '0',
  `is_left` tinyint(1) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_job_graph`
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
-- Table definition for `fc_template`
--

CREATE TABLE IF NOT EXISTS `fc_template` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `template` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_role`
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
-- Table definition for `fc_rule`
--

CREATE TABLE IF NOT EXISTS `fc_rule` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `path` varchar(400) NOT NULL,
  `count` int(11) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_pcfg_preterminals`
--

CREATE TABLE `fc_pcfg_preterminals` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `workunit_id` bigint(20) unsigned NOT NULL,
  `preterminals` blob DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_pcfg_grammar`
--

CREATE TABLE `fc_pcfg_grammar` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `path` varchar(400) NOT NULL,
  `keyspace` bigint(20) unsigned NOT NULL,
  `time_added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;


-- --------------------------------------------------------

--
-- Table definition for `fc_settings`
--

CREATE TABLE IF NOT EXISTS `fc_settings` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `default_seconds_per_workunit` int(10) unsigned NOT NULL DEFAULT '3600',
  `workunit_timeout_factor` int(10) unsigned NOT NULL DEFAULT '6',
  `hwmon_temp_abort` int(10) unsigned NOT NULL DEFAULT '90',
  `bench_all` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `distribution_coefficient_alpha` decimal(5,2) NOT NULL DEFAULT '0.1',
  `t_pmin` int(10) unsigned NOT NULL DEFAULT '20',
  `ramp_up_workunits` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `ramp_down_coefficient` decimal(5,2) NOT NULL DEFAULT '0.25',
  `verify_hash_format` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `auto_add_hosts_to_running_jobs` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Table definition for `fc_user`
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
-- Table definition for `fc_user_permissions`
--

CREATE TABLE IF NOT EXISTS `fc_user_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `user_id` int(11) NOT NULL,
  `view` tinyint(1) NOT NULL DEFAULT '0',
  `modify` tinyint(1) NOT NULL DEFAULT '0',
  `operate` tinyint(1) NOT NULL DEFAULT '0',
  `owner` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `job_id` (`job_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_host_status`
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
-- Table definition for `fc_job_status`
--

CREATE TABLE IF NOT EXISTS `fc_job_status` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `job_id` bigint(20) unsigned NOT NULL,
  `status` smallint(1) unsigned NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table definition for `fc_server_usage`
--

CREATE TABLE IF NOT EXISTS `fc_server_usage` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `cpu` decimal(10,0) NOT NULL,
  `ram` decimal(10,0) NOT NULL,
  `net_recv` int(11) NOT NULL,
  `net_sent` int(11) NOT NULL,
  `hdd_read` int(11) NOT NULL,
  `hdd_write` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Constraints for `fc_job`
--
ALTER TABLE `fc_job`
  ADD CONSTRAINT `batch_link` FOREIGN KEY (`batch_id`) REFERENCES `fc_batch` (`id`) ON DELETE SET NULL;

--
-- Constraints for `fc_batch`
--
ALTER TABLE `fc_batch`
  ADD CONSTRAINT `user_link` FOREIGN KEY (`creator_id`) REFERENCES `fc_user` (`id`) ON DELETE SET NULL;

--
-- Constraints for `fc_job_status`
--
ALTER TABLE `fc_job_status`
  ADD CONSTRAINT `fc_job_status_ibfk_1` FOREIGN KEY (`job_id`) REFERENCES `fc_job` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;


--
-- Constraints for `fc_notification`
--
ALTER TABLE `fc_notification`
  ADD CONSTRAINT `fc_notification_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `fc_user` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fc_notification_ibfk_2` FOREIGN KEY (`source_id`) REFERENCES `fc_job` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;


--
-- Constraints for `fc_bin_job`
--
ALTER TABLE `fc_bin_job`
  ADD CONSTRAINT `fc_bin_job_jobfk` FOREIGN KEY (`job_id`) REFERENCES `fc_job` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fc_bin_job_binfk` FOREIGN KEY (`bin_id`) REFERENCES `fc_bin` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;


--
-- Constraints for `fc_job_graph`
--
ALTER TABLE `fc_job_graph`
  ADD CONSTRAINT `fc_job_graph_ibfk_1` FOREIGN KEY (`job_id`) REFERENCES `fc_job` (`id`);

--
-- Constraints for `fc_user`
--
ALTER TABLE `fc_user`
  ADD CONSTRAINT `fc_user_ibfk_1` FOREIGN KEY (`role_id`) REFERENCES `fc_role` (`id`);

--
-- Constraints for `fc_user_permissions`
--
ALTER TABLE `fc_user_permissions`
  ADD CONSTRAINT `fc_user_permissions_ibfk_1` FOREIGN KEY (`job_id`) REFERENCES `fc_job` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fc_user_permissions_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `fc_user` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;
