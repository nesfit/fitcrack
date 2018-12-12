/**
 * @file SqlLoader.cpp
 * @brief Source file for common MySQL access
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <SqlLoader.h>
#include <Host.h>
#include <Job.h>
#include <Mask.h>
#include <Package.h>
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


std::vector<PtrPackage> CSqlLoader::loadRunningPackages()
{
    return load<CPackage>(formatQuery("WHERE status >= %d AND ( time_start <= now() OR time_start IS NULL) ORDER BY id ASC",
                                      Config::PackageState::PackageRunning));
}


std::vector<PtrHost> CSqlLoader::loadActiveHosts(uint64_t packageId)
{
    /** Add missing hosts */
    addNewHosts(packageId);

    /** Return all active hosts for packageId */
    return load<CHost>(formatQuery("WHERE (status = %d OR status = %d) AND `job_id` = %" PRIu64 "",
                                   Config::HostState::HostBench, Config::HostState::HostNormal, packageId));
}


std::vector<PtrHost> CSqlLoader::loadFinishedHosts()
{
    return load<CHost>(formatQuery("WHERE `job_id` IN (SELECT id FROM `%s` WHERE `status` < %d ) ; ",
                                   CPackage::getTableName().c_str(), Config::PackageState::PackageRunning));
}


void CSqlLoader::removeFinishedHosts()
{
    updateSql(formatQuery("DELETE FROM `%s` WHERE `job_id` IN (SELECT id FROM `%s` WHERE `status` < %d ) ; ",
                          CHost::getTableName().c_str(), CPackage::getTableName().c_str(),
                          Config::PackageState::PackageRunning));
}


void CSqlLoader::finishTimeoutPackages()
{
    /** Set packages with passed time_end to Finishing status */
    std::vector<PtrPackage> packages = load<CPackage>(formatQuery("WHERE status = %d AND time_end <= now() ORDER BY id ASC",
                                                      Config::PackageState::PackageRunning));
    for(PtrPackage & package : packages)
    {
        Tools::printDebugPackage(Config::DebugType::Log, package->getId(),
                "Package has passed timeout deadline. Setting state to finishing.\n");
        package->updateStatusOfRunningPackage(Config::PackageState::PackageFinishing);
    }
}


uint64_t CSqlLoader::getBenchCount(uint64_t packageId, uint64_t hostId)
{
    return getRowCount<CJob>(formatQuery(
            "WHERE `host_id` = %" PRIu64 " AND `job_id` = %" PRIu64 " AND `finished` = 0 AND `retry` = 0 AND `hc_keyspace` = 0",
            hostId, packageId));
}


uint64_t CSqlLoader::getJobCount(uint64_t packageId, uint64_t hostId)
{
    /**
     * @warning This select is used for job generating, retry == 0 is checked (retry == 1 is not and active job)
     */
    return getRowCount<CJob>(formatQuery(
            "WHERE `host_id` = %" PRIu64 " AND `job_id` = %" PRIu64 " AND `finished` = 0 AND `retry` = 0 AND `hc_keyspace` > 0",
            hostId, packageId));
}


uint64_t CSqlLoader::getJobCount(uint64_t packageId)
{
    /**
     * @warning This select is used for finished status checking, retry == 0 is not checked (new jobs will be generated)
     */
    return getRowCount<CJob>(
            formatQuery("WHERE `job_id` = %" PRIu64 " AND `finished` = 0 AND `hc_keyspace` > 0", packageId));
}


void CSqlLoader::setJobFinished(uint64_t id)
{
    return updateSql(formatQuery("UPDATE `%s` SET finished = 1, progress = 100 WHERE id = %" PRIu64 " LIMIT 1;", CJob::getTableName().c_str(), id));
}


Config::Ptr<CJob> CSqlLoader::getEasiestRetry(uint64_t packageId)
{
    std::vector<Config::Ptr<CJob>> jobVec = load<CJob>(formatQuery("WHERE retry != 0 AND finished = 0 AND hc_keyspace != 0 AND job_id = %" PRIu64 " ORDER BY hc_keyspace ASC",
                                                                   packageId));

    if (jobVec.empty())
        return nullptr;

    return jobVec.front();
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


void CSqlLoader::updatePackageIndex(uint64_t packageId, uint64_t newIndex)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_index = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CPackage::getTableName().c_str(), newIndex, packageId));
}


void CSqlLoader::updatePackageIndex2(uint64_t packageId, uint64_t newIndex2)
{
    return updateSql(formatQuery("UPDATE `%s` SET current_index_2 = %" PRIu64 " WHERE id = %" PRIu64 " ;",
                                 CPackage::getTableName().c_str(), newIndex2, packageId));
}


void CSqlLoader::updatePackageStatus(uint64_t packageId, uint32_t newStatus)
{
    return updateSql(formatQuery("UPDATE `%s` SET status = %" PRIu32 " WHERE id = %" PRIu64 " LIMIT 1;",
                                 CPackage::getTableName().c_str(), newStatus, packageId));
}


void CSqlLoader::updateRunningPackageStatus(uint64_t packageId, uint32_t newStatus)
{
    return updateSql(formatQuery("UPDATE `%s` SET status = %" PRIu32 " WHERE id = %" PRIu64 " AND status >= 10 LIMIT 1;",
                                 CPackage::getTableName().c_str(), newStatus, packageId));
}


bool CSqlLoader::isPackageTimeout(uint64_t packageId)
{
    uint64_t isTimeout = getRowCount<CPackage>(formatQuery("WHERE id = %" PRIu64 " AND time_end <= now() ;", packageId));
    return (isTimeout > 0);
}


void CSqlLoader::addNewWorkunit(PtrJob job)
{
    return updateSql(formatQuery("INSERT INTO `%s` (`job_id`,`workunit_id`,`host_id`, `boinc_host_id`, \
`start_index`, `start_index_2`, `hc_keyspace`, `mask_id`, `dictionary_id`, `duplicated`, `duplicate`, `retry`) VALUES ('%" PRIu64 "','%" PRIu64 "',\
'%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%" PRIu64 "','%d', '%" PRIu64 "', '%d');",
                                 CJob::getTableName().c_str(), job->getPackageId(), job->getWorkunitId(), job->getHostId(),
                                 job->getBoincHostId(), job->getStartIndex(), job->getStartIndex2(), job->getHcKeyspace(),
                                 job->getMaskId(), job->getDictionaryId(), job->isDuplicated(), job->getDuplicate(),
                                 job->isRetry()));
}


unsigned int CSqlLoader::getTimeoutFactor()
{
    return (unsigned int)(getSqlNumber(formatQuery("SELECT `default_workunit_timeout_factor` FROM `%s` LIMIT 1",
                                                  Config::tableNameSettings.c_str())));
}


uint32_t CSqlLoader::getHostStatus(uint64_t host_id)
{
    return (uint32_t)(getSqlNumber(formatQuery("SELECT `status` FROM `%s` WHERE id = %" PRIu64 " LIMIT 1",
                                               Config::tableNameHost.c_str(), host_id)));
}


void CSqlLoader::updateStartTimeNow(uint64_t packageId)
{
    updateSql(formatQuery("UPDATE `%s` SET `time_start` = now() WHERE `id` = %" PRIu64 " AND `time_start` IS NULL LIMIT 1;",
                          CPackage::getTableName().c_str(), packageId));
}


void CSqlLoader::updateEndTimeNow(uint64_t packageId)
{
    updateSql(formatQuery("UPDATE `%s` SET `time_end` = now() WHERE `id` = %" PRIu64 " LIMIT 1;",
                          CPackage::getTableName().c_str(), packageId));
}


std::vector<Config::Ptr<CMask>> CSqlLoader::loadPackageMasks(uint64_t packageId)
{
    return load<CMask>(formatQuery("WHERE `job_id` = %" PRIu64 " AND `current_index` < `hc_keyspace` ORDER BY id ASC",
                                   packageId));
}


std::vector<Config::Ptr<CDictionary>> CSqlLoader::loadPackageDictionaries(uint64_t packageId) {
    return customLoad<CDictionary>(formatQuery("SELECT `%s`.*, `%s`.`path`, `%s`.`keyspace` FROM `%s` INNER JOIN `%s` ON `%s`.`dictionary_id` = `%s`.`id` WHERE `job_id` = %" PRIu64 " AND `current_index` < `keyspace` ORDER BY `%s`.`id` ASC ;",
                                               Config::tableNamePckgDictionary.c_str(), Config::tableNameDictionary.c_str(),
                                               Config::tableNameDictionary.c_str(), Config::tableNamePckgDictionary.c_str(),
                                               Config::tableNameDictionary.c_str(), Config::tableNamePckgDictionary.c_str(),
                                               Config::tableNameDictionary.c_str(), packageId, Config::tableNamePckgDictionary.c_str()));
}


std::vector<std::string> CSqlLoader::loadPackageHashes(uint64_t packageId)
{
    std::vector<std::string> result;
    updateSql(formatQuery("SELECT REPLACE(TO_BASE64(`hash`), '\n', '') FROM `%s` WHERE `job_id` = %" PRIu64 " AND `result` IS NULL ;",
                          Config::tableNameHash.c_str(), packageId));

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
            Tools::printDebugPackage(Config::DebugType::Log, packageId,
                                     "Decoded hash: %s\n", output.c_str());
        }
    }

    mysql_free_result(sqlResult);
    Tools::printDebugPackage(Config::DebugType::Log, packageId,
                             "Number of loaded hashes: %d\n", result.size());

    return result;
}


Config::Ptr<CMask> CSqlLoader::loadMask(uint64_t maskId)
{
    return load<CMask>(formatQuery("WHERE `id` = %" PRIu64 " LIMIT 1", maskId)).front();
}


Config::Ptr<CDictionary> CSqlLoader::loadDictionary(uint64_t dictId)
{
    return customLoad<CDictionary>(formatQuery("SELECT `%s`.*, `%s`.`path`, `%s`.`keyspace` FROM `%s` INNER JOIN `%s` ON `%s`.`dictionary_id` = `%s`.`id` WHERE `%s`.`id` = %" PRIu64 " LIMIT 1 ;",
                                               Config::tableNamePckgDictionary.c_str(), Config::tableNameDictionary.c_str(),
                                               Config::tableNameDictionary.c_str(), Config::tableNamePckgDictionary.c_str(),
                                               Config::tableNameDictionary.c_str(), Config::tableNamePckgDictionary.c_str(),
                                               Config::tableNameDictionary.c_str(), Config::tableNamePckgDictionary.c_str(),
                                               dictId)).front();
}


Config::Ptr<CPackage> CSqlLoader::loadPackage(uint64_t packageId)
{
    return load<CPackage>(formatQuery("WHERE `id` = %" PRIu64 " LIMIT 1", packageId)).front();
}


uint64_t CSqlLoader::getTotalPower(uint64_t packageId)
{
    return getSqlNumber(formatQuery(
            "SELECT SUM(power) FROM `%s` WHERE `job_id` = %" PRIu64 ";",
            CHost::getTableName().c_str(), packageId));
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


bool CSqlLoader::isAnythingCracked(uint64_t packageId)
{
    return getSqlNumber(formatQuery("SELECT COUNT(*) FROM `%s` WHERE `job_id` = %" PRIu64 " AND `result` IS NOT NULL ;",
            Config::tableNameHash.c_str(), packageId)) != 0;
}


uint64_t CSqlLoader::getSecondsPassed(uint64_t packageId)
{
    return getSqlNumber(formatQuery("SELECT TIMESTAMPDIFF(SECOND, time_start, now()) FROM `%s` WHERE id = %" PRIu64 ";",
                                    CPackage::getTableName().c_str(), packageId));
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


void CSqlLoader::addNewHosts(uint64_t packageId)
{
    /** Find missing hosts */
    updateSql(formatQuery("SELECT id FROM host WHERE id IN (SELECT `boinc_host_id` FROM `%s` WHERE `job_id` = %" PRIu64 ") AND id NOT IN (SELECT `boinc_host_id` FROM `%s` WHERE `job_id` = %" PRIu64 ");",
                          Config::tableNameHostActivity.c_str(), packageId, CHost::getTableName().c_str(), packageId));

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
                                  CHost::getTableName().c_str(), newHostId, packageId));

            Tools::printDebugHost(Config::DebugType::Log, packageId, newHostId,
                    "New host #%" PRIu64 " added to package #%" PRIu64 "\n", newHostId, packageId);
        }
        catch (std::logic_error & error)
        {
            Tools::printDebugPackage(Config::DebugType::Log, packageId,
                    "Error converting boinc_host_id to uint64_t: %s\n", error.what());
            continue;
        }
    }

    mysql_free_result(sqlResult);
}


uint64_t CSqlLoader::getSqlNumber(const std::string & query)
{
    uint64_t result = 0;
    updateSql(query.c_str());

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
        if (row[0] == nullptr)
            continue;

        try
        {
            result = std::stoull(row[0]);
        }
        catch (std::logic_error & error)
        {
            Tools::printDebugTimestamp("Error converting row count to uint64_t: %s\n", error.what());
            continue;
        }
    }

    mysql_free_result(sqlResult);
    return result;
}
