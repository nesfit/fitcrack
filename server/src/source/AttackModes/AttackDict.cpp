/**
 * @file AttackDict.cpp
 * @brief Source file for creation of Dictionary Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackDict.h>


CAttackDict::CAttackDict(PtrJob &job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(job, host, seconds, sqlLoader)
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

    /** Append mode to config */
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
    if (!f)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open config BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    Tools::printDebug("CONFIG for new workunit:\n");

    /** Output original config from DB */
    f << m_job->getConfig();
    Tools::printDebug(m_job->getConfig().c_str());

    /** Output mode */
    f << "|||mode|String|1|n|||\n";
    Tools::printDebug("|||mode|String|1|n|||\n");

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
    if (!f)
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

    f.open(path);
    if (!f)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open dict1 BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Creating dictionary fragment\n");

    /** Load current workunit dictionary */
    PtrDictionary workunitDict = m_sqlLoader->loadDictionary(m_workunit->getDictionaryId());
    std::ifstream* dictFile;
    std::string passwd;
    uint64_t currentIndex;

    if(m_workunit->isDuplicated())
    {
        /** Open new FD for retry workunits */
        uint64_t startIndex = m_workunit->getStartIndex();
        dictFile = new std::ifstream((Config::dictDir + workunitDict->getDictFileName()).c_str());
        if (!(*dictFile))
        {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                                  "Cannot find dictionary file! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                              "Skipping %" PRIu64 " passwords\n", startIndex);

        /** Skip passwords in dictionary */
        for (currentIndex = 0; currentIndex < startIndex; ++currentIndex)
            std::getline(*dictFile, passwd);

        /** No more passwords in dictionary */
        if ((*dictFile).eof())
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
    }
    else {

        dictFile = Tools::getStream(m_job->getId(), workunitDict->getDictionaryId(), workunitDict->getDictFileName());
        if (!(*dictFile))
        {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                                  "Cannot find dictionary file! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }
    }
    /** Add 'keyspace' passwords to dict file */

    uint64_t workunitKeyspace = m_workunit->getHcKeyspace();

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Adding %" PRIu64 " passwords to host dictionary file\n", workunitKeyspace);
    for (currentIndex = 0; currentIndex < workunitKeyspace; ++currentIndex)
    {
        std::getline((*dictFile), passwd);
        if (!passwd.empty())
            f << passwd << '\n';

        /** End of current dictionary */
        if ((*dictFile).eof())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Ate all passwords, new workunit keyspace: %" PRIu64 "\nSetting job to Finishing\n",
                              currentIndex);

            if (!m_workunit->isDuplicated())
            {
                m_job->updateIndex(m_job->getCurrentIndex() - m_workunit->getHcKeyspace() + currentIndex);
                workunitDict->updateIndex(workunitDict->getHcKeyspace());
            }

            m_workunit->setHcKeyspace(currentIndex);
            break;
        }
    }

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(), "Done.\n");

    if (m_workunit->isDuplicated())
    {
        (*dictFile).close();
        delete dictFile;
    }

    f.close();

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
    uint64_t passCount = m_host->getPower() * m_seconds;

    if (passCount < Config::minPassCount)
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = Config::minPassCount;
    }

    /** Load job dictionaries */
    std::vector<PtrDictionary> dictVec = m_job->getDictionaries();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Dictionaries left for this job: %" PRIu64 "\n", dictVec.size());

    /** Find the following dictionary */
    PtrDictionary currentDict;
    for (PtrDictionary & dict : dictVec)
    {
        if (dict->getCurrentIndex() < dict->getHcKeyspace())
        {
            /** Dictionary for a new workunit found */
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Dict found: %s, current index: %" PRIu64 "/%" PRIu64 "\n",
                    dict->getDictFileName().c_str(), dict->getCurrentIndex(), dict->getHcKeyspace());
            currentDict = dict;
            break;
        }
    }

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
