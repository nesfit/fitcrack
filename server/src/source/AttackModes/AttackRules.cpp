/**
 * @file AttackRules.cpp
 * @brief Source file for creation of Rules Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackRules.h>

#include <cmath>    /**< std::round */


CAttackRules::CAttackRules(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(std::move(job), host, seconds, sqlLoader)
{

}


bool CAttackRules::makeWorkunit() {
    /** Create the workunit first */
    if (!m_workunit && !generateWorkunit())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE], name2[Config::SQL_BUF_SIZE], name3[Config::SQL_BUF_SIZE],
            name4[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];

    const char *infiles[4];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_%d_%d.dict", Config::appName, Config::startTime, Config::seqNo++);
    /** Same name of rules file - for sticky flag to work */
    std::snprintf(name4, Config::SQL_BUF_SIZE, "%s_rules_%" PRIu64 "", Config::appName, m_job->getId());

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

    /** Load current workunit dictionary */
    PtrDictionary workunitDict = GetWorkunitDict();

    /** Debug */
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "In first dict, there are %" PRIu64 " passwords\n", workunitDict->getHcKeyspace());
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "In rule file, there are %" PRIu64 " rules\n", m_job->getCombSecDictSize());
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Workunit can compute %" PRIu64 " passwords\n", m_workunit->getHcKeyspace());


    /** Create dict1 file */
    retval = config.download_path(name3, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - dict1. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    try
    {

        auto inputDict = makeInputDict(workunitDict, m_workunit->getStartIndex());

        uint64_t dictKeyspace = m_workunit->getHcKeyspace();

        uint64_t newCurrentIndex = m_workunit->getStartIndex() + dictKeyspace;
        if (!m_workunit->isDuplicated())
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "New dictionary current index: %" PRIu64 "\n", newCurrentIndex);

        /** Check if we reached end of keyspace */
        if (newCurrentIndex >= workunitDict->getHcKeyspace())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "We reached the end of current dictionary, modifiyng workunit keyspace\n");

            dictKeyspace = workunitDict->getHcKeyspace() - m_workunit->getStartIndex();
            newCurrentIndex = workunitDict->getHcKeyspace();
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "The #passwords from dict is therefore: %" PRIu64 "\n", dictKeyspace);

        if (!m_workunit->isDuplicated())
        {
            workunitDict->updateIndex(newCurrentIndex);
            m_job->updateIndex(m_job->getCurrentIndex() + dictKeyspace);
        }

        m_workunit->setHcKeyspace(dictKeyspace);

        /** Computation done, start creating dictionary */
        /** Ignore 'start_index' passwords */
        uint64_t workunitStartIndex = m_workunit->getStartIndex();
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Skipping %" PRIu64 " passwords\n", workunitStartIndex);

        /** Add 'keyspace' passwords to dict2 file */
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adding %" PRIu64 " passwords to host dict file\n", dictKeyspace);

        auto writtenPasswords = inputDict->WritePasswordsTo(dictKeyspace, path);
        if(writtenPasswords == 0)
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "'start_index' parameter is too far away\n");
            workunitDict->updateIndex(workunitDict->getHcKeyspace());
            m_job->updateIndex(m_job->getCurrentIndex() - dictKeyspace);
            return true;
        }
        else if(writtenPasswords < dictKeyspace)
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Ate all passwords from current dictionary\n");
            m_workunit->setHcKeyspace(writtenPasswords);

            if (!m_workunit->isDuplicated())
            {
                workunitDict->updateIndex(workunitDict->getHcKeyspace());
                m_job->updateIndex(m_job->getCurrentIndex() - dictKeyspace + writtenPasswords);
            }
        }
    }
    catch(const InputDict::Exception &e)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "A dictionary operation failed. Setting job to malformed. Message: %s\n", e.what());
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Done. Closing files\n");


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


    /** Create rules file */
    retval = config.download_path(name4, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - rules. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f.open(path);
    if (!f.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open rules BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    if(m_job->getRules().empty())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Rules is not set in database! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ifstream rulesFile;
    rulesFile.open((Config::rulesDir + m_job->getRules()).c_str());
    if (!rulesFile)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open rules file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f << rulesFile.rdbuf();
    f.close();


    /** Fill in the workunit parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_job->getTimeoutFactor() * (uint32_t)(m_job->getSecondsPerWorkunit());
    infiles[0] = name1;
    infiles[1] = name2;
    infiles[2] = name3;
    infiles[3] = name4;

    setDefaultWorkunitParams(&wu);

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            Config::inTemplatePathRule,
            path,
            config.project_path(path),
            infiles,
            4,
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


bool CAttackRules::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Generating rules workunit ...\n");

    /** Compute password count */
    uint64_t passCount = getPasswordCountToProcess();

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
    PtrDictionary currentDict = FindCurrentDict(dictVec);

    if (!currentDict)
    {
        /** No dictionaries found, no workunit could be generated */
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "No dictionaries found for this job\n");
        return false;
    }

    /**
     * Create the workunit
     * @warning We save number of real passwords to hc_keyspace and modify it later
     */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), currentDict->getCurrentIndex(),
                         0, passCount, 0, currentDict->getId(), false, 0, false);
    /**
     * @warning Index updating (current_index) must be done later
     */
    return true;
}
