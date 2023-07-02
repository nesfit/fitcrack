/**
 * @file SqlLoader.cpp
 * @brief Source file for common MySQL access
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <SqlLoader.h>
#include <Host.h>
#include <Workunit.h>
#include <Mask.h>
#include <Job.h>
#include <Dictionary.h>

#include <cstdarg>


CSqlLoader::CSqlLoader()
    :   m_sqlBuffer(new char[Config::SQL_BUF_SIZE])
{

}


CSqlLoader::~CSqlLoader()
{
    delete[] m_sqlBuffer;
}


std::vector<PtrJob> CSqlLoader::loadRunningJobs()
{
    return load<CJob>(formatQuery("WHERE status >= %d AND ( time_start <= now() OR time_start IS NULL) ORDER BY priority ASC",
                                      Config::JobState::JobRunning));
}


std::vector<PtrHost> CSqlLoader::loadActiveHosts(uint64_t jobId)
{
    /** Add missing hosts */
    addNewHosts(jobId);

    /** Return all active hosts for jobId */
    return customLoad<CHost>(formatQuery(
        R"';..;'(
            SELECT host_table.*, TIMESTAMPDIFF(SECOND, last_seen, UTC_TIMESTAMP()) as seconds_since_last_seen
                FROM `%s` host_table INNER JOIN `%s_status` status_table ON host_table.`boinc_host_id` = status_table.`boinc_host_id`
                WHERE (status = %d OR status = %d) AND `job_id` = %)';..;'" PRIu64 ";"
        ,
        CHost::getTableName().c_str(),
        CHost::getTableName().c_str(),
        Config::HostState::HostBench,
        Config::HostState::HostNormal,
        jobId
    ));
}


std::vector<PtrHost> CSqlLoader::loadFinishedHosts()
{
    return load<CHost>(formatQuery("WHERE `job_id` IN (SELECT id FROM `%s` WHERE `status` < %d ) ; ",
                                   CJob::getTableName().c_str(), Config::JobState::JobRunning));
}


void CSqlLoader::removeFinishedHosts()
{
    updateSql(formatQuery("DELETE FROM `%s` WHERE `job_id` IN (SELECT id FROM `%s` WHERE `status` < %d ) ; ",
                          CHost::getTableName().c_str(), CJob::getTableName().c_str(),
                          Config::JobState::JobRunning));
}


void CSqlLoader::finishTimeoutJobs()
{
    /** Set jobs with passed time_end to Finishing status */
    std::vector<PtrJob> jobs = load<CJob>(formatQuery("WHERE status = %d AND time_end <= now() ORDER BY id ASC",
                                                      Config::JobState::JobRunning));
    for(PtrJob & job : jobs)
    {
        Tools::printDebugJob(Config::DebugType::Log, job->getId(),
                             "Job has passed timeout deadline. Setting state to finishing.\n");
        job->updateStatusOfRunningJob(Config::JobState::JobFinishing);
    }
}


uint64_t CSqlLoader::getBenchCount(uint64_t jobId, uint64_t hostId)
{
    return getRowCount<CWorkunit>(formatQuery(
            "WHERE `host_id` = %" PRIu64 " AND `job_id` = %" PRIu64 " AND `finished` = 0 AND `retry` = 0 AND `hc_keyspace` = 0",
            hostId, jobId));
}


uint64_t CSqlLoader::getWorkunitCount(uint64_t jobId, uint64_t hostId)
{
    /**
     * @warning This select is used for workunit generating, retry == 0 is checked (retry == 1 is not and active workunit)
     */
    return getRowCount<CWorkunit>(formatQuery(
            "WHERE `host_id` = %" PRIu64 " AND `job_id` = %" PRIu64 " AND `finished` = 0 AND `retry` = 0 AND `hc_keyspace` > 0",
            hostId, jobId));
}

uint64_t CSqlLoader::getTotalWorkunitCount(uint64_t jobId, uint64_t hostId)
{
    return getRowCount<CWorkunit>(formatQuery(
            "WHERE `host_id` = %" PRIu64 " AND `job_id` = %" PRIu64,
            hostId, jobId));
}


uint64_t CSqlLoader::getWorkunitCount(uint64_t jobId)
{
    /**
     * @warning This select is used for finished status checking, retry == 0 is not checked (new workunits will be generated)
     */
    return getRowCount<CWorkunit>(
            formatQuery("WHERE `job_id` = %" PRIu64 " AND `finished` = 0 AND `hc_keyspace` > 0", jobId));
}


void CSqlLoader::setWorkunitFinished(uint64_t id)
{
    return updateSql(formatQuery("UPDATE `%s` SET finished = 1, progress = 100 WHERE id = %" PRIu64 " LIMIT 1;", CWorkunit::getTableName().c_str(), id));
}


Config::Ptr<CWorkunit> CSqlLoader::getEasiestRetry(uint64_t jobId)
{
    std::vector<Config::Ptr<CWorkunit>> workunitVec = load<CWorkunit>(formatQuery("WHERE retry != 0 AND finished = 0 AND hc_keyspace != 0 AND job_id = %" PRIu64 " ORDER BY hc_keyspace ASC",
                                                                   jobId));

    if (workunitVec.empty())
        return nullptr;

    return workunitVec.front();
}


void CSqlLoader::updateHostStatus(uint64_t hostId, uint32_t newStatus)
{
    return updateSql(formatQuery("UPDATE `%s` SET status = %" PRIu32 " WHERE id = %" PRIu64 " LIMIT 1;",
                                 CHost::getTableName().c_str(), newStatus, hostId));
}


void CSqlLoader::updateMaskIndex(uint64_t maskId, uint64_t newIndex)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_index = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CMask::getTableName().c_str(), newIndex, maskId));
}


void CSqlLoader::updateDictionaryIndex(uint64_t dictId, uint64_t newIndex)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_index = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CDictionary::getTableName().c_str(), newIndex, dictId));
}

void CSqlLoader::updateDictionaryPos(uint64_t dictId, uint64_t newPos)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_pos = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CDictionary::getTableName().c_str(), newPos, dictId));
}

void CSqlLoader::updateJobIndex(uint64_t jobId, uint64_t newIndex)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_index = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CJob::getTableName().c_str(), newIndex, jobId));
}


void CSqlLoader::updateJobIndex2(uint64_t jobId, uint64_t newIndex2)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_index_2 = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CJob::getTableName().c_str(), newIndex2, jobId));
}


void CSqlLoader::updateJobStatus(uint64_t jobId, uint32_t newStatus)
{
    return updateSql(formatQuery("UPDATE `%s` SET status = %" PRIu32 " WHERE id = %" PRIu64 " LIMIT 1;",
                                 CJob::getTableName().c_str(), newStatus, jobId));
}


void CSqlLoader::updateRunningJobStatus(uint64_t jobId, uint32_t newStatus)
{
    return updateSql(formatQuery("CALL set_running_job_status(%" PRIu64 ", %" PRIu32 ");",
                                 jobId, newStatus));
}


bool CSqlLoader::isJobTimeout(uint64_t jobId)
{
    uint64_t isTimeout = getRowCount<CJob>(formatQuery("WHERE id = %" PRIu64 " AND time_end <= now() ;", jobId));
    return (isTimeout > 0);
}


void CSqlLoader::addNewWorkunit(PtrWorkunit workunit)
{
    return updateSql(formatQuery("INSERT INTO `%s` (`job_id`,`workunit_id`,`host_id`, `boinc_host_id`, \
`start_index`, `start_index_2`, `hc_keyspace`, `mask_id`, `dictionary_id`, `duplicated`, `duplicate`, `retry`) VALUES ('%" PRIu64 "','%" PRIu64 "',\
'%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%d', '%" PRIu64 "', '%d');",
                                 CWorkunit::getTableName().c_str(), workunit->getJobId(), workunit->getWorkunitId(), workunit->getHostId(),
                                 workunit->getBoincHostId(), workunit->getStartIndex(), workunit->getStartIndex2(), workunit->getHcKeyspace(),
                                 workunit->getMaskId(), workunit->getDictionaryId(), workunit->isDuplicated(), workunit->getDuplicate(),
                                 workunit->isRetry()));
}


unsigned int CSqlLoader::getTimeoutFactor()
{
    return (unsigned int)(getSqlNumber(formatQuery("SELECT `workunit_timeout_factor` FROM `%s` LIMIT 1",
                                                  Config::tableNameSettings.c_str())));
}

unsigned int CSqlLoader::getHWTempAbort()
{
    return getSqlNumber(formatQuery("SELECT `hwmon_temp_abort` FROM `%s` LIMIT 1",
                                                  Config::tableNameSettings.c_str()));
}

/// Gets the distribution coefficient from the database
double CSqlLoader::getDistributionCoefficient()
{
    return getSqlDouble(formatQuery("SELECT `distribution_coefficient_alpha` FROM `%s` LIMIT 1",
                                                Config::tableNameSettings.c_str()));
}

/// Gets the setting for the absolute minimum time a workunit should take
unsigned CSqlLoader::getAbsoluteMinimumWorkunitSeconds()
{
    return getSqlNumber(formatQuery("SELECT `t_pmin` FROM `%s` LIMIT 1",
                                                  Config::tableNameSettings.c_str()));
}

/// returns whether there should be a ramp up of WU time
bool CSqlLoader::getEnableRampUp()
{
    return getSqlNumber(formatQuery("SELECT `ramp_up_workunits` FROM `%s` LIMIT 1",
                                                  Config::tableNameSettings.c_str()));
}

/// Gets the ramp-down coefficient from the database
double CSqlLoader::getRampDownCoefficient()
{
    return getSqlDouble(formatQuery("SELECT `ramp_down_coefficient` FROM `%s` LIMIT 1",
                                                Config::tableNameSettings.c_str()));
}


uint32_t CSqlLoader::getHostStatus(uint64_t host_id)
{
    return (uint32_t)(getSqlNumber(formatQuery("SELECT `status` FROM `%s` WHERE id = %" PRIu64 " LIMIT 1",
                                               Config::tableNameHost.c_str(), host_id)));
}


void CSqlLoader::updateStartTimeNow(uint64_t jobId)
{
    updateSql(formatQuery("UPDATE `%s` SET `time_start` = now() WHERE `id` = %" PRIu64 " AND `time_start` IS NULL LIMIT 1;",
                          CJob::getTableName().c_str(), jobId));
}


void CSqlLoader::updateEndTimeNow(uint64_t jobId)
{
    updateSql(formatQuery("UPDATE `%s` SET `time_end` = now() WHERE `id` = %" PRIu64 " LIMIT 1;",
                          CJob::getTableName().c_str(), jobId));
}


std::vector<Config::Ptr<CMask>> CSqlLoader::loadJobMasks(uint64_t jobId)
{
    return load<CMask>(formatQuery("WHERE `job_id` = %" PRIu64 " AND `current_index` < `hc_keyspace` ORDER BY id ASC",
                                   jobId));
}

std::vector<Config::Ptr<CMask>> CSqlLoader::loadJobMasksWithNormalKeyspace(uint64_t jobId)
{
    return load<CMask>(formatQuery("WHERE `job_id` = %" PRIu64 " AND `current_index` < `keyspace` ORDER BY id ASC",
                                   jobId));
}

std::vector<Config::Ptr<CDictionary>>
CSqlLoader::loadJobDictionaries(uint64_t jobId) {
  return customLoad<CDictionary>(formatQuery(
      "SELECT `%s`.*, `%s`.`name`, `%s`.`path`, `%s`.`password_distribution`, `%s`.`hex_dict`, "
      "`%s`.`keyspace` FROM `%s` INNER JOIN `%s` ON `%s`.`dictionary_id` = "
      "`%s`.`id` WHERE `job_id` = %" PRIu64
      " AND `current_index` < `keyspace` ORDER BY `%s`.`id` ASC ;",
      Config::tableNameJobDictionary.c_str(), Config::tableNameDictionary.c_str(),
      Config::tableNameDictionary.c_str(), Config::tableNameDictionary.c_str(),
      Config::tableNameDictionary.c_str(), Config::tableNameDictionary.c_str(),
      Config::tableNameJobDictionary.c_str(),
      Config::tableNameDictionary.c_str(),
      Config::tableNameJobDictionary.c_str(),
      Config::tableNameDictionary.c_str(), jobId,
      Config::tableNameJobDictionary.c_str()));
}

std::vector<std::string> CSqlLoader::loadJobHashes(uint64_t jobId)
{
    std::vector<std::string> result;
    updateSql(formatQuery("SELECT REPLACE(TO_BASE64(`hash`), '\n', '') FROM `%s` WHERE `job_id` = %" PRIu64 " AND `result` IS NULL ;",
                          Config::tableNameHash.c_str(), jobId));

    MYSQL_RES* sqlResult;
    sqlResult = mysql_store_result(boinc_db.mysql);
    if (!sqlResult)
    {
        Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.\n");
        boinc_db.close();
        exit(1);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(sqlResult)))
    {
        std::string output;
        if (row[0])
        {
            std::string input(row[0]);
            CodeTools::Base64::Decode(input, output);
            result.emplace_back(output);
        }
    }

    mysql_free_result(sqlResult);
    Tools::printDebugJob(Config::DebugType::Log, jobId,
                         "Number of loaded hashes: %d\n", result.size());

    return result;
}


Config::Ptr<CMask> CSqlLoader::loadMask(uint64_t maskId)
{
    return load<CMask>(formatQuery("WHERE `id` = %" PRIu64 " LIMIT 1", maskId)).front();
}

Config::Ptr<CDictionary> CSqlLoader::loadDictionary(uint64_t dictId) {
  return customLoad<CDictionary>(
             formatQuery(
                 "SELECT `%s`.*, `%s`.`path`, `%s`.`password_distribution`, `%s`.`hex_dict`, "
                 "`%s`.`keyspace` FROM `%s` INNER JOIN `%s` ON "
                 "`%s`.`dictionary_id` = `%s`.`id` WHERE `%s`.`id` = %" PRIu64
                 " LIMIT 1 ;",
                 Config::tableNameJobDictionary.c_str(),
                 Config::tableNameDictionary.c_str(),
                 Config::tableNameDictionary.c_str(),
                 Config::tableNameDictionary.c_str(),
                 Config::tableNameDictionary.c_str(),
                 Config::tableNameJobDictionary.c_str(),
                 Config::tableNameDictionary.c_str(),
                 Config::tableNameJobDictionary.c_str(),
                 Config::tableNameDictionary.c_str(),
                 Config::tableNameJobDictionary.c_str(), dictId))
      .front();
}


Config::Ptr<CJob> CSqlLoader::loadJob(uint64_t jobId)
{
    return load<CJob>(formatQuery("WHERE `id` = %" PRIu64 " LIMIT 1", jobId)).front();
}


std::string CSqlLoader::loadGrammarName(uint64_t grammarId)
{
    std::string result;
    updateSql(formatQuery("SELECT path FROM `%s` WHERE `id` = %" PRIu64 " LIMIT 1 ;",
                          Config::tableNamePcfgGrammar.c_str(), grammarId));

    MYSQL_RES* sqlResult;
    sqlResult = mysql_store_result(boinc_db.mysql);
    if (!sqlResult)
    {
        Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.\n");
        boinc_db.close();
        exit(1);
    }

    MYSQL_ROW row;
    if ((row = mysql_fetch_row(sqlResult)))
    {
        if (row[0])
        {
            std::string grammar_name(row[0]);
            result = grammar_name;
            // Tools::printDebug("LOG: Loaded grammar name: %s\n", result.c_str());
        }
    }

    mysql_free_result(sqlResult);
    return result;
}


uint64_t CSqlLoader::getTotalPower(uint64_t jobId)
{
    return getSqlNumber(formatQuery(
            "SELECT SUM(power) FROM `%s` WHERE `job_id` = %" PRIu64 ";",
            CHost::getTableName().c_str(), jobId));
}


std::string CSqlLoader::getDictFileName(uint64_t dictId)
{
    std::string result;
    updateSql(formatQuery("SELECT path FROM `%s` WHERE `id` = %" PRIu64 " LIMIT 1;",
              Config::tableNameDictionary.c_str(), dictId));

    MYSQL_RES* sqlResult;
    sqlResult = mysql_store_result(boinc_db.mysql);
    if (!sqlResult)
    {
        Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.");
        boinc_db.close();
        exit(1);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(sqlResult)))
    {
        if (row[0])
            result = row[0];
    }

    mysql_free_result(sqlResult);
    return result;
}


bool CSqlLoader::isAnythingCracked(uint64_t jobId)
{
    return getSqlNumber(formatQuery("SELECT COUNT(*) FROM `%s` WHERE `job_id` = %" PRIu64 " AND `result` IS NOT NULL ;",
            Config::tableNameHash.c_str(), jobId)) != 0;
}


void CSqlLoader::killJob(PtrJob &job)
{
    uint64_t jobId = job->getId();
    Tools::printDebugJob(Config::DebugType::Log, jobId,
                         "JOB KILLED, sending BOINC messages ...\n");

    /** Find corresponding workunits */
    std::vector<PtrWorkunit > jobWorkunits = load<CWorkunit>(formatQuery("WHERE job_id = %" PRIu64 ";", jobId));

    for (PtrWorkunit & wu : jobWorkunits)
        cancel_jobs(wu->getWorkunitId(), wu->getWorkunitId());

    /** Wipe the workunits */
    updateSql(formatQuery("DELETE FROM `%s` WHERE job_id = %" PRIu64 " ;",
                          CWorkunit::getTableName().c_str(), jobId));

    /** Put down the kill flag */
    updateSql(formatQuery("UPDATE `%s` SET `kill` = 0 WHERE `id` = %" PRIu64 " LIMIT 1;",
                          CJob::getTableName().c_str(), jobId));

    /** Stop the job */
    updateRunningJobStatus(jobId, Config::JobState::JobReady);

    Tools::printDebugJob(Config::DebugType::Log, jobId,
                         "Job killed successfully!\n");
}


uint64_t CSqlLoader::getSecondsPassed(uint64_t jobId)
{
    return getSqlNumber(formatQuery("SELECT TIMESTAMPDIFF(SECOND, time_start, now()) FROM `%s` WHERE id = %" PRIu64 ";",
                                    CJob::getTableName().c_str(), jobId));
}


void CSqlLoader::updateSql(const char * query)
{
    /** Debug output */
    //Tools::printDebug("Executing SQL command: %s\n", query);

    int retval = boinc_db.do_query(query);
    if (retval)
    {
        Tools::printDebugTimestamp("Problem with DB query: %s\n Shutting down now.\n", query);
        boinc_db.close();
        exit(1);
    }
}


char * CSqlLoader::formatQuery(const char * format, ...)
{
    va_list argPtr;
    va_start(argPtr, format);
    vsnprintf(m_sqlBuffer, Config::SQL_BUF_SIZE, format, argPtr);
    va_end(argPtr);

    return m_sqlBuffer;
}


void CSqlLoader::addNewHosts(uint64_t jobId)
{
    /** Find missing hosts */
    updateSql(formatQuery("SELECT id FROM host WHERE id IN (SELECT `boinc_host_id` FROM `%s` WHERE `job_id` = %" PRIu64 ") AND id NOT IN (SELECT `boinc_host_id` FROM `%s` WHERE `job_id` = %" PRIu64 ");",
                          Config::tableNameHostActivity.c_str(), jobId, CHost::getTableName().c_str(), jobId));

    MYSQL_RES* sqlResult;
    sqlResult = mysql_store_result(boinc_db.mysql);
    if (!sqlResult)
    {
        Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.\n");
        boinc_db.close();
        exit(1);
    }

    MYSQL_ROW row;

    uint64_t newHostId;
    while ((row = mysql_fetch_row(sqlResult)))
    {
        try
        {
            newHostId = std::stoull(row[0]);

            /** Insert new hosts to fc_host */
            updateSql(formatQuery("INSERT INTO `%s` (`boinc_host_id`, `job_id`) VALUES (%" PRIu64 ", %" PRIu64 ");",
                                  CHost::getTableName().c_str(), newHostId, jobId));

            Tools::printDebugHost(Config::DebugType::Log, jobId, newHostId,
                    "New host #%" PRIu64 " added to job #%" PRIu64 "\n", newHostId, jobId);
        }
        catch (std::logic_error & error)
        {
            Tools::printDebugJob(Config::DebugType::Log, jobId,
                                 "Error converting boinc_host_id to uint64_t: %s\n", error.what());
            continue;
        }
    }

    mysql_free_result(sqlResult);
}


std::string CSqlLoader::getSqlString(const std::string & query)
{
    updateSql(query.c_str());

    std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> sqlResult(nullptr, &mysql_free_result);
    sqlResult.reset(mysql_store_result(boinc_db.mysql));
    if (!sqlResult)
    {
        Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.");
        boinc_db.close();
        exit(1);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(sqlResult.get())))
    {
        if (row[0] == nullptr)
            continue;

        return row[0];
    }

    return "";
}

template <typename Res>
Res CSqlLoader::getSqlConverted(const std::string &query, Res(*conversionFn)(const std::string&))
{
    std::string sqlQuery = getSqlString(query);
    try
    {
        return conversionFn(sqlQuery);
    }
    catch(const std::exception &e)
    {
        Tools::printDebugTimestamp("SQL query '%s' failed.\n", query.c_str());
        Tools::printDebugTimestamp("Error converting db result to target type: %s\n", e.what());
    }
    catch(...)
    {
        Tools::printDebugTimestamp("SQL query '%s' failed.\n", query.c_str());
        Tools::printDebugTimestamp("Unknown error converting db result to target type\n");
    }
    return {};
}

uint64_t CSqlLoader::getSqlNumber(const std::string & query)
{
    return getSqlConverted(
        query,
        static_cast<
            unsigned long long(*)(const std::string &)
        >(
            [](const std::string &res){return std::stoull(res);}
        )
    );
}

double CSqlLoader::getSqlDouble(const std::string & query)
{
    return getSqlConverted(
        query,
        static_cast<
            double(*)(const std::string &)
        >(
            [](const std::string &res){return std::stod(res);}
        )
    );
}
