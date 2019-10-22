/**
 * @file Job.cpp
 * @brief Source file for fc_workunit entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Job.h>
#include <SqlLoader.h>


CJob::CJob(DbMap & jobMap)
{
    try
    {
        this->m_id = std::stoull(jobMap["id"]);
        this->m_packageId = std::stoull(jobMap["job_id"]);
        this->m_workunitId = std::stoull(jobMap["workunit_id"]);
        this->m_hostId = std::stoull(jobMap["host_id"]);
        this->m_boincHostId = std::stoull(jobMap["boinc_host_id"]);
        this->m_startIndex = std::stoull(jobMap["start_index"]);
        this->m_startIndex2 = std::stoull(jobMap["start_index_2"]);
        this->m_hcKeyspace = std::stoull(jobMap["hc_keyspace"]);
        this->m_maskId = std::stoull(jobMap["mask_id"]);
        this->m_dictionaryId = std::stoull(jobMap["dictionary_id"]);
        this->m_duplicated = true;
        this->m_duplicate = std::stoull(jobMap["id"]);   /**< We use this only for duplicate job creation */
        this->m_retry = false;
        this->m_finished = false;
        
        this->m_finishJob = false;
    }
    catch(std::logic_error & error)
    {
        Tools::printDebugTimestamp("Error converting jobMap to PtrJob: %s", error.what());
        exit(1);
    }
}


CJob::CJob(uint64_t & packageId, uint64_t & hostId, uint64_t & boincHostId, uint64_t & startIndex,
           uint64_t & startIndex2, uint64_t & hcKeyspace, uint64_t & maskId, uint64_t & dictionaryId,
           bool & duplicated, uint64_t & duplicate, bool & retry)
{
    this->m_id = 0;     /**< ID will be filled by BOINC*/
    this->m_packageId = packageId;
    this->m_hostId = hostId;
    this->m_boincHostId = boincHostId;
    this->m_workunitId = 0;     /**< WUID will be filled by BOINC */
    this->m_startIndex = startIndex;
    this->m_startIndex2 = startIndex2;
    this->m_hcKeyspace = hcKeyspace;
    this->m_maskId = maskId;
    this->m_dictionaryId = dictionaryId;
    this->m_duplicated = duplicated;
    this->m_duplicate = duplicate;
    this->m_retry = retry;
    this->m_finished = false;

    this->m_finishJob = false;
}


PtrJob CJob::create(uint64_t packageId, uint64_t hostId, uint64_t boincHostId, uint64_t startIndex,
                    uint64_t startIndex2, uint64_t hcKeyspace, uint64_t maskId, uint64_t dictionaryId,
                    bool duplicated, uint64_t duplicate, bool retry)
{
    return PtrJob(new CJob(packageId, hostId, boincHostId, startIndex, startIndex2, hcKeyspace, maskId, dictionaryId,
                           duplicated, duplicate, retry));
}


PtrJob CJob::create(DbMap & jobMap, CSqlLoader * sqlLoader)
{
    /** sqlLoader is here just for common interface in load() function */
    UNUSED(sqlLoader);
    return PtrJob(new CJob(jobMap));
}


std::string CJob::getTableName()
{
    return Config::tableNameWorkunit;
}


/**
 * @section Table attributes getters/setters
 */

uint64_t CJob::getId() const
{
    return m_id;
}


uint64_t CJob::getPackageId() const
{
    return m_packageId;
}


uint64_t CJob::getWorkunitId() const
{
    return m_workunitId;
}


uint64_t CJob::getHostId() const
{
    return m_hostId;
}


uint64_t CJob::getBoincHostId() const
{
    return m_boincHostId;
}


uint64_t CJob::getStartIndex() const
{
    return m_startIndex;
}


uint64_t CJob::getStartIndex2() const
{
    return m_startIndex2;
}


uint64_t CJob::getHcKeyspace() const
{
    return m_hcKeyspace;
}


uint64_t CJob::getMaskId() const
{
    return m_maskId;
}


uint64_t CJob::getDictionaryId() const
{
    return m_dictionaryId;
}


bool CJob::isDuplicated() const
{
    return m_duplicated;
}


uint64_t CJob::getDuplicate() const
{
    return m_duplicate;
}


bool CJob::isRetry() const
{
    return m_retry;
}


bool CJob::isFinished() const
{
    return m_finished;
}


bool CJob::isFinishJob() const
{
    return m_finishJob;
}


void CJob::setWorkunitId(uint64_t workunitId)
{
    m_workunitId = workunitId;
}


void CJob::setHcKeyspace(uint64_t hcKeyspace)
{
    m_hcKeyspace = hcKeyspace;
}


void CJob::setRetry(bool retry)
{
    m_retry = retry;
}


void CJob::setHostId(uint64_t hostId)
{
    m_hostId = hostId;
}


void CJob::setBoincHostId(uint64_t boincHostId)
{
    m_boincHostId = boincHostId;
}


void CJob::setDuplicated(bool duplicated)
{
    m_duplicated = duplicated;
}


void CJob::setDuplicate(uint64_t duplicate)
{
    m_duplicate = duplicate;
}
