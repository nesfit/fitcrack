/**
 * @file AttackCombinator.cpp
 * @brief Source file for creation of Combinator Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackCombinator.h>

#include <cmath>    /**< std::round */


CAttackCombinator::CAttackCombinator(PtrJob &job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(job, host, seconds, sqlLoader)
{

}


bool CAttackCombinator::makeWorkunit()
{
    /** Create the workunit first */
    if (!m_workunit && !generateWorkunit())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE], name2[Config::SQL_BUF_SIZE], name3[Config::SQL_BUF_SIZE],
            name4[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];

    const char* infiles[4];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    /** Same name of first dictionary - for sticky flag to work */
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_combinator_%" PRIu64 ".dict", Config::appName, m_job->getId());
    std::snprintf(name4, Config::SQL_BUF_SIZE, "%s_%d_%d.dict", Config::appName, Config::startTime, Config::seqNo++);

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

    /** Load current workunit dictionary */
    PtrDictionary workunitDict = m_sqlLoader->loadDictionary(m_workunit->getDictionaryId());

    // Debug
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "In first dicts, there are %" PRIu64 " total passwords\n", m_job->getHcKeyspace());
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "In second dict, there are %" PRIu64 " passwords\n", workunitDict->getHcKeyspace());
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Workunit can compute %" PRIu64 " passwords\n", m_workunit->getHcKeyspace());

    /** Create dict2 file */
    retval = config.download_path(name4, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - dict2. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream f2;
    f2.open(path);
    if (!f2)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open second dictionary BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ifstream dict2_file;
    dict2_file.open((Config::dictDir + workunitDict->getDictFileName()).c_str());
    if (!dict2_file)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open second dictionary input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    /** Check combinator workunit type */
    /** Dictionary is already fragmented, continue fragmenting */
    if (m_workunit->getStartIndex2() != 0)
    {
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Dictionary is already fragmented, we need to continue\n");

        /** Ignore 'start_index' passwords */
        uint64_t workunitStartIndex = m_workunit->getStartIndex();
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Skipping %" PRIu64 " passwords\n", workunitStartIndex);

        std::string passwd;
        uint64_t currentIndex;
        for (currentIndex = 0; currentIndex < workunitStartIndex; ++currentIndex)
            std::getline(dict2_file, passwd);

        /** No more passwords in current dictionary */
        if (dict2_file.eof())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "'start_index' parameter is too far away\n");
            if (!m_workunit->isDuplicated())
                workunitDict->updateIndex(workunitDict->getHcKeyspace());

            return true;
        }

        /** Append a single password to second dict */
        std::getline(dict2_file, passwd);
        if (passwd.empty() || dict2_file.eof())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Empty password or just reached EOF, skipping workunit\n");
            if (!m_workunit->isDuplicated())
            {
                workunitDict->updateIndex(workunitDict->getCurrentIndex() + 1);
                m_job->updateIndex(m_job->getCurrentIndex() + 1);
            }
            return true;
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adding a single password to host dict2 file: %s", passwd.c_str());
        f2 << passwd << '\n';

        /** Append skip and limit to config */
        Tools::printDebug("Adding additional info to CONFIG:\n");

        uint64_t workunitStartIndex2 = m_workunit->getStartIndex2();
        int digits = 0;
        uint64_t num = workunitStartIndex2;
        do { num /= 10; ++digits; } while (num != 0);    // Count digits
        f << "|||start_index|BigUInt|" << digits << "|" << workunitStartIndex2 << "|||\n";
        Tools::printDebug("|||start_index|BigUInt|%d|%" PRIu64 "|||\n", digits, workunitStartIndex2);

        /** Check if we reach the end of password keyspace in 1st dict */
        if (m_workunit->getHcKeyspace() + workunitStartIndex2 >= m_job->getHcKeyspace())
        {
            m_workunit->setHcKeyspace(m_job->getHcKeyspace() - workunitStartIndex2);

            if (!m_workunit->isDuplicated())
            {
                workunitDict->updateIndex(workunitDict->getCurrentIndex() + 1);
                m_job->updateIndex(m_job->getCurrentIndex() + 1);
                m_job->updateIndex2(0);
            }
        }
        else
        {
            uint64_t workunitHcKeyspace = m_workunit->getHcKeyspace();
            if (!m_workunit->isDuplicated())
                m_job->updateIndex2(workunitStartIndex2 + workunitHcKeyspace);

            digits = 0;
            num = workunitHcKeyspace;
            do { num /= 10; ++digits; } while (num != 0);    // Count digits
            f << "|||hc_keyspace|BigUInt|" << digits << "|" << workunitHcKeyspace << "|||\n";
            Tools::printDebug("|||hc_keyspace|BigUInt|%d|%" PRIu64 "|||\n", digits, workunitHcKeyspace);
        }
    }
    /** Host has enough power, no fragments */
    else if (m_workunit->getHcKeyspace() > (0.5f * m_job->getHcKeyspace()))
    {
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Host has enough power, no fragments\n");

        /** # of real passwords / # of passwords in first dictionaries = # of passwords to check from dict2*/
        uint64_t secDictKeyspace = (uint64_t)(std::round(m_workunit->getHcKeyspace() / (float)(m_job->getHcKeyspace())));
        uint64_t newCurrentIndex = m_workunit->getStartIndex() + secDictKeyspace;

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Rounded # of passwords from dict2: %" PRIu64 " passwords\n", secDictKeyspace);
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "New current index of current dict2: %" PRIu64 "\n", newCurrentIndex);

        /** Check if we reached end of keyspace of current dict2 */
        if (newCurrentIndex >= workunitDict->getHcKeyspace())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "We reached the end of current dict2, modifiyng workunit keyspace\n");

            secDictKeyspace = workunitDict->getHcKeyspace() - m_workunit->getStartIndex();
            newCurrentIndex = workunitDict->getHcKeyspace();
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "The #passwords from second dict is therefore: %" PRIu64 "\n", secDictKeyspace);
        if (!m_workunit->isDuplicated())
        {
            workunitDict->updateIndex(newCurrentIndex);
            m_job->updateIndex(m_job->getCurrentIndex() + secDictKeyspace);
        }

        /** Exact number of passwords in workunit
         * @warning Combinator sets real keyspace to hc_keyspace, as there is no such thing as hc_keyspace here
         **/
        m_workunit->setHcKeyspace(secDictKeyspace * m_job->getHcKeyspace());

        /** Computation done, start creating dictionary dict2 */
        /** Ignore 'start_index' passwords */
        uint64_t workunitStartIndex = m_workunit->getStartIndex();
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Skipping %" PRIu64 " passwords\n", workunitStartIndex);

        std::string passwd;
        uint64_t currentIndex;
        for (currentIndex = 0; currentIndex < workunitStartIndex; ++currentIndex)
            std::getline(dict2_file, passwd);

        /** No more passwords in dictionary */
        if (dict2_file.eof())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "'start_index' parameter is too far away\n");
            if (!m_workunit->isDuplicated())
                workunitDict->updateIndex(workunitDict->getHcKeyspace());

            return true;
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adding %" PRIu64 " passwords to host dict2 file\n", secDictKeyspace);

        /** Add 'keyspace' passwords to dict2 file */
        for (currentIndex = 0; currentIndex < secDictKeyspace; ++currentIndex)
        {
            std::getline(dict2_file, passwd);
            if (!passwd.empty())
                f2 << passwd << '\n';

            /** End of current dictionary */
            if (dict2_file.eof())
            {
                Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                        "Ate all passwords from current dictionary\n");
                m_workunit->setHcKeyspace(currentIndex * m_job->getHcKeyspace());

                if (!m_workunit->isDuplicated())
                {
                    workunitDict->updateIndex(workunitDict->getHcKeyspace());
                    m_job->updateIndex(m_job->getCurrentIndex() + currentIndex);
                }

                break;
            }
        }
    }
    /** Host doesn't have enough power, start fragmenting */
    else
    {
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Host doesn't have the power, start fragmenting\n");

        /** Ignore 'start_index' passwords */
        uint64_t workunitStartIndex = m_workunit->getStartIndex();
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Skipping %" PRIu64 " passwords\n", workunitStartIndex);
        std::string passwd;
        uint64_t currentIndex;
        for (currentIndex = 0; currentIndex < workunitStartIndex; ++currentIndex)
            std::getline(dict2_file, passwd);

        /** No more passwords in current dictionary */
        if (dict2_file.eof())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "'start_index' parameter is too far away\n");
            if (!m_workunit->isDuplicated())
                workunitDict->updateIndex(workunitDict->getHcKeyspace());

            return true;
        }

        std::getline(dict2_file, passwd);
        if (passwd.empty() || dict2_file.eof())
        {
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Empty password or just reached EOF, skipping workunit\n");
            if (!m_workunit->isDuplicated())
            {
                workunitDict->updateIndex(workunitDict->getCurrentIndex() + 1);
                m_job->updateIndex(m_job->getCurrentIndex() + 1);
            }
            return true;
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adding a single password to host dict2 file: %s\n", passwd.c_str());
        f2 << passwd << '\n';

        /** Append skip and limit to config */
        Tools::printDebug("Adding additional info to CONFIG:\n");

        f << "|||start_index|BigUInt|1|0|||\n";
        Tools::printDebug("|||start_index|BigUInt|1|0|||\n");

        uint64_t workunitHcKeyspace = m_workunit->getHcKeyspace();
        int digits = 0;
        uint64_t num = workunitHcKeyspace;
        do { num /= 10; ++digits; } while (num != 0);    // Count digits
        f << "|||hc_keyspace|BigUInt|" << digits << "|" << workunitHcKeyspace << "|||\n";
        Tools::printDebug("|||hc_keyspace|BigUInt|%d|%" PRIu64 "|||\n", digits, workunitHcKeyspace);

        /** Update current_index_2 of the first fragmented dict */
        if (!m_workunit->isDuplicated())
            m_job->updateIndex2(workunitHcKeyspace);
    }

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Done. Closing files\n");

    dict2_file.close();
    f2.close();
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

    /** Create dict1 file */
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

    auto dictVec = m_job->getDictionaries();
    std::ifstream dictFile;
    for (auto & dict : dictVec)
    {
        if (!dict->isLeft())
            continue;

        dictFile.open((Config::dictDir + dict->getDictFileName()).c_str());
        if (!dictFile) {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                    "Failed to open dictionary file! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }

        f << dictFile.rdbuf();
        dictFile.close();
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
    infiles[3] = name4;

    setDefaultWorkunitParams(&wu);

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            Config::inTemplatePathCombinator,
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


bool CAttackCombinator::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Generating combinator workunit ...\n");

    /** Compute password count */
    uint64_t passCount = m_host->getPower() * m_seconds;

    if (passCount < Config::minPassCount)
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = Config::minPassCount;
    }

    /** Load job RIGHT-side dictionaries */
    std::vector<PtrDictionary> dictVec = m_job->getRightDictionaries();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Right-dictionaries remaining for this job: %" PRIu64 "\n", dictVec.size());

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

    /**
     * Create the workunit
     * @warning We save number of real passwords to hc_keyspace as we don't work with hc_indexes in combinator attack
     */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), currentDict->getCurrentIndex(),
                         m_job->getCurrentIndex2(), passCount, 0, currentDict->getId(), false, 0, false);
    /**
     * @warning Index updating (current_index(_2)) must be done later
     */
    return true;
}
