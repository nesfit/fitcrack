--
-- Spúšťače 
--
DROP TRIGGER IF EXISTS `job_notification`;
DELIMITER //
CREATE TRIGGER `job_notification` AFTER UPDATE ON `fc_job`
 FOR EACH ROW BEGIN
	DECLARE userID int;
	DECLARE done INT DEFAULT FALSE;
	DECLARE usersCursor CURSOR FOR (SELECT id FROM fc_user);
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	-- insert progress info to graph table
	IF NEW.indexes_verified <> OLD.indexes_verified THEN
		INSERT INTO fc_job_graph (progress, job_id) VALUES (
	        IF(NEW.hc_keyspace = 0 OR NEW.status = 1 OR NEW.status = 2,
	           100,
	           ROUND((NEW.indexes_verified / IF( NEW.attack_mode = 1, NEW.keyspace, NEW.hc_keyspace )) * 100, 2)
	          ),

	        NEW.id  );
	END IF;

	-- job status changed. We need to send notifications!
	IF NEW.status <> OLD.status THEN

		IF NEW.status = 10 THEN
			INSERT INTO fc_job_graph (progress, job_id) VALUES ( 0, NEW.id);
		END IF;

		IF NEW.status = 1 OR NEW.status = 2 THEN
			INSERT INTO fc_job_graph (progress, job_id) VALUES ( 100, NEW.id);
		END IF;

		-- send notification to admins that can view this job
		OPEN usersCursor;
			user_loop: LOOP
				FETCH usersCursor INTO userID;
			    IF done THEN
			    	LEAVE user_loop;
			    END IF;
				INSERT INTO fc_notification  VALUES (DEFAULT, userID, DEFAULT,NEW.id,OLD.status,NEW.status,DEFAULT, DEFAULT);
			END LOOP;
		CLOSE usersCursor;
	END IF;

END
//
DELIMITER ;


--
-- Failed benchmark has no meaning, set it to finished automatically
--
DROP TRIGGER IF EXISTS `bench_retry_finish`;
DELIMITER //
CREATE TRIGGER `bench_retry_finish` BEFORE UPDATE ON `fc_workunit`
 FOR EACH ROW BEGIN
	IF (NEW.retry = 1 AND OLD.retry = 0 AND NEW.hc_keyspace = 0)
	THEN
		SET NEW.finished = 1;
	END IF;
END
//
DELIMITER ;

--
-- If BOINC error appears, set host status to benchmark and workunit to retry
--
DROP TRIGGER IF EXISTS `client_error_trigger`;
DELIMITER //
CREATE TRIGGER `client_error_trigger` BEFORE UPDATE ON `result`
 FOR EACH ROW BEGIN
	IF (NEW.outcome = 3 AND OLD.outcome != 3)
	THEN
		UPDATE `fc_workunit` SET retry = 1 WHERE `workunit_id` = NEW.workunitid LIMIT 1;
		UPDATE `fc_host` SET `status` = 0, `power` = 0 WHERE id IN (SELECT `host_id` FROM `fc_workunit` WHERE `workunit_id` = NEW.workunitid) LIMIT 1;
	END IF;
	IF (NEW.outcome = 7 AND OLD.outcome != 7)
	THEN
		UPDATE `fc_workunit` SET retry = 1 WHERE `workunit_id` = NEW.workunitid LIMIT 1;
		UPDATE `fc_host` SET `status` = 0, `power` = 0 WHERE id IN (SELECT `host_id` FROM `fc_workunit` WHERE `workunit_id` = NEW.workunitid) LIMIT 1;
	END IF;
END
//
DELIMITER ;


--
-- Actions after new host appears
--
DROP TRIGGER IF EXISTS `new_host_trigger`;
DELIMITER //
CREATE TRIGGER `new_host_trigger` AFTER INSERT ON `host`
FOR EACH ROW BEGIN

	DECLARE benchAll INT;
	DECLARE currentState INT;

	-- Add host to host_status
	INSERT INTO fc_host_status (`id`, `boinc_host_id`, `last_seen`) VALUES (DEFAULT, NEW.id, DEFAULT);

	-- Run bench_all
	SET benchAll = (SELECT `default_bench_all` FROM `fc_settings` LIMIT 1);

	IF (benchAll)
	THEN		
		SET currentState = (SELECT `status` FROM `fc_job` WHERE `id` = 1 LIMIT 1);

		IF (currentState < 10)
        	THEN
			DELETE FROM `fc_host_activity` WHERE `job_id` = 1 ;
			UPDATE `fc_job` SET `time_start` = NOW() WHERE `id` = 1 LIMIT 1;
		END IF;

		INSERT INTO `fc_host_activity` (`boinc_host_id`, `job_id`) VALUES (NEW.id, 1);
		UPDATE `fc_job` SET `status` = 10, `time_end` = NULL WHERE `id` = 1 LIMIT 1;
	END IF;
END
//
DELIMITER ;

--
-- When timeout is reached, set workunit to retry and host to benchmark
--
DROP TRIGGER IF EXISTS `timeout_v1`;
DELIMITER //
CREATE TRIGGER `timeout_v1` AFTER UPDATE ON `result`
 FOR EACH ROW BEGIN
	IF (NEW.server_state = 5 AND OLD.server_state != 5 AND UNIX_TIMESTAMP() >= NEW.report_deadline)
	THEN
		UPDATE `fc_workunit` SET `retry` = 1 WHERE `workunit_id` = NEW.workunitid LIMIT 1 ;
        UPDATE `fc_host` SET `status` = 0, `power` = 0 WHERE id IN (SELECT `host_id` FROM `fc_workunit` WHERE `workunit_id` = NEW.workunitid) LIMIT 1;
	END IF;
END
//
DELIMITER ;
