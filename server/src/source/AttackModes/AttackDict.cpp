/**
 * @file AttackDict.cpp
 * @brief Source file for creation of Dictionary Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackDict.h>


CAttackDict::CAttackDict(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(std::move(job), host, seconds, sqlLoader)
{

}


bool CAttackDict::makeWorkunit()
{
    /** Create the workunit instance first */
    if (!m_workunit && !generateWorkunit())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE],name2[Config::SQL_BUF_SIZE],name3[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];
    const char* infiles[3];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_%d_%d.dict", Config::appName, Config::startTime, Config::seqNo++);

    /** Create the config file */
    retval = config.download_path(name1, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - config. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream f;
    f.open(path);
    if (!f.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open config BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f << generateBasicConfig(m_job->getAttackMode(), m_job->getAttackSubmode(),
            m_job->getName(), m_job->getHashType(), 0, m_job->getHWTempAbort());
    f.close();

    /** Create data file */
    retval = config.download_path(name2, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - data. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f.open(path);
    if (!f.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open data BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f << m_job->getHashes();
    f.close();

    /** Create dictionary file. */
    retval = config.download_path(name3, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - dict1. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }


    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Creating dictionary fragment\n");

    /** Load current workunit dictionary */
    PtrDictionary workunitDict = GetWorkunitDict();

    try
    {
        auto inputDict = makeInputDict(workunitDict, m_workunit->getStartIndex(), false);

        /** Add 'keyspace' passwords to dict file */

        uint64_t workunitKeyspace = m_workunit->getHcKeyspace();

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adding %" PRIu64 " passwords to host dictionary file\n", workunitKeyspace);
        auto writtenPasswords = inputDict->WritePasswordsTo(workunitKeyspace, path);
        if(writtenPasswords == 0 && workunitKeyspace > 0)
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                                "'start_index' parameter is too far away\n");
            if (!m_workunit->isDuplicated())
            {
                workunitDict->updateIndex(workunitDict->getHcKeyspace());
                m_job->updateIndex(m_job->getCurrentIndex() - m_workunit->getHcKeyspace());
            }
            return true;
        }
        else if(writtenPasswords < workunitKeyspace)
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Ate all passwords, new workunit keyspace: %" PRIu64 "\nSetting job to Finishing\n",
                                writtenPasswords);

            if (!m_workunit->isDuplicated())
            {
                m_job->updateIndex(m_job->getCurrentIndex() - m_workunit->getHcKeyspace() + writtenPasswords);
                workunitDict->updateIndex(workunitDict->getHcKeyspace());
            }

            m_workunit->setHcKeyspace(writtenPasswords);
        }
    }
    catch(const InputDict::Exception &e)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "A dictionary operation failed. Setting job to malformed. Message: %s\n", e.what());
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(), "Done.\n");

    /** Fill in the workunit parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_job->getTimeoutFactor() * (uint32_t)(m_job->getSecondsPerWorkunit());
    infiles[0] = name1;
    infiles[1] = name2;
    infiles[2] = name3;

    setDefaultWorkunitParams(&wu);

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            Config::inTemplatePathDict,
            path,
            config.project_path(path),
            infiles,
            3,
            config
    );

    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to create BOINC workunit. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    restrict_wu_to_host(wu, m_workunit->getBoincHostId());

    m_workunit->setWorkunitId(uint64_t(wu.id));
    m_sqlLoader->addNewWorkunit(m_workunit);

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                          "Workunit succesfully created\n");
    return true;
}


bool CAttackDict::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Generating dictionary workunit ...\n");

    /** Compute password count */
    uint64_t passCount = getPasswordCountToProcess();

    if (passCount < getMinPassCount())
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = getMinPassCount();
    }

    /** Load job dictionaries */
    std::vector<PtrDictionary> dictVec = m_job->getDictionaries();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Dictionaries left for this job: %" PRIu64 "\n", dictVec.size());

    /** Find the following dictionary */
    PtrDictionary currentDict = FindCurrentDict(dictVec);

    if (!currentDict)
    {
        /** No dictionaries found, no workunit could be generated */
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "No dictionaries found for this job\n");
        return false;
    }

    uint64_t dictIndex = currentDict->getCurrentIndex();
    uint64_t dictHcKeyspace = currentDict->getHcKeyspace();
    if (dictIndex + passCount > dictHcKeyspace)
    {
        /** Host is too powerful for this mask, it will finish it */
        passCount = dictHcKeyspace - dictIndex;
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adjusting #passwords, dictionary too small, new #: %" PRIu64 "\n", passCount);
    }

    /** Create the workunit */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), dictIndex, 0, passCount, 0,
                         currentDict->getId(), false, 0, false);

    /** Update the job/dictionary index */
    uint64_t currentIndex = m_job->getCurrentIndex();

    m_job->updateIndex(currentIndex + passCount);
    currentDict->updateIndex(dictIndex + passCount);

    return true;
}
