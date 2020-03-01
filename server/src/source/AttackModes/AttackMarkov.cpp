/**
 * @file AttackMarkov.cpp
 * @brief Source file for creation of Markov Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackMarkov.h>


CAttackMarkov::CAttackMarkov(PtrJob &job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(job, host, seconds, sqlLoader)
{

}


bool CAttackMarkov::makeWorkunit()
{
    /** Create the workunit instance first */
    if (!m_workunit && !generateWorkunit())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE],name2[Config::SQL_BUF_SIZE],name3[Config::SQL_BUF_SIZE],path[Config::SQL_BUF_SIZE];
    const char* infiles[3];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_markov_%" PRIu64 "", Config::appName, m_job->getId());

    /** Load the workunit mask to object */
    PtrMask workunitMask = m_sqlLoader->loadMask(m_workunit->getMaskId());

    /** Append mode, mask, start_index, stop_index to config */
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

    f << generateBasicConfig('n', m_job->getAttackMode(), m_job->getAttackSubmode(),
                             m_job->getName(), m_job->getHashType(), "", "",
                             m_job->getCharset1(), m_job->getCharset2(), m_job->getCharset3(),
                             m_job->getCharset4());

    /** Output mask */
    f << "|||mask|String|" << workunitMask->getMask().length() << "|" << workunitMask->getMask() << "|||\n";
    Tools::printDebug("|||mask|String|%d|%s|||\n", workunitMask->getMask().length(), workunitMask->getMask().c_str());

    /** Output start_index */
    int digits = 0;
    uint64_t num = m_workunit->getStartIndex();
    do { num /= 10; ++digits; } while (num != 0);    // Count digits
    f << "|||start_index|BigUInt|" << digits << "|" << m_workunit->getStartIndex() << "|||\n";
    Tools::printDebug("|||start_index|BigUInt|%d|%" PRIu64 "|||\n", digits, m_workunit->getStartIndex());

    /** Output markov_threshold */
    uint32_t markov_threshold = m_job->getMarkovThreshold();
    if (markov_threshold != 0)
    {
        digits = 0;
        num = markov_threshold;
        do { num /= 10; ++digits; } while (num != 0);    // Count digits
        f << "|||markov_threshold|UInt|" << digits << "|" << markov_threshold << "|||\n";
        Tools::printDebug("|||markov_threshold|UInt|%d|%" PRIu32 "|||\n", digits, markov_threshold);
    }

    uint64_t maskHcKeyspace = workunitMask->getHcKeyspace();
    uint64_t workunitHcKeyspace = m_workunit->getHcKeyspace();

    /** Output stop_index - only if it is not the last workunit in the current mask */
    if (m_workunit->getStartIndex() + workunitHcKeyspace < maskHcKeyspace)
    {
        digits = 0;
        num = workunitHcKeyspace;
        do { num /= 10; ++digits; } while (num != 0);    // Count digits
        f << "|||hc_keyspace|BigUInt|" << digits << "|" << workunitHcKeyspace << "|||\n";
        Tools::printDebug("|||hc_keyspace|BigUInt|%d|%" PRIu64 "|||\n", digits, workunitHcKeyspace);
    }
    else
    {
        /** Otherwise, send whole mask_hc_keyspace for correct progress calculation, --limit is omitted */
        digits = 0;
        num = maskHcKeyspace;
        do { num /= 10; ++digits; } while (num != 0);    // Count digits
        f << "|||mask_hc_keyspace|BigUInt|" << digits << "|" << maskHcKeyspace << "|||\n";
        Tools::printDebug("|||mask_hc_keyspace|BigUInt|%d|%" PRIu64 "|||\n", digits, maskHcKeyspace);
    }

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


    /** Create markov file */
    retval = config.download_path(name3, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - markov. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f.open(path);
    if (!f.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open markov BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    if(m_job->getMarkov().empty())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Job markov is empty! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ifstream markovFile;
    markovFile.open((Config::markovDir + m_job->getMarkov()).c_str());
    if (!markovFile.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open markov file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f << markovFile.rdbuf();
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
            Config::inTemplatePathMarkov,
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


bool CAttackMarkov::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Generating markov workunit ...\n");

    /** Compute password count */
    uint64_t passCount = m_host->getPower() * m_seconds;
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Number of real passwords host could compute: %" PRIu64 "\n", passCount);

    if (passCount < Config::minPassCount)
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = Config::minPassCount;
    }

    std::vector<PtrMask> maskVec = m_job->getMasks();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Masks left for this job: %" PRIu64 "\n", maskVec.size());

    /** Find the following mask */
    PtrMask currentMask;
    for (PtrMask & mask : maskVec)
    {
        if (mask->getCurrentIndex() < mask->getHcKeyspace())
        {
            /** Mask for a new workunit found */
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Mask found: %s, current index: %" PRIu64 "/%" PRIu64 "\n",
                    mask->getMask().c_str(), mask->getCurrentIndex(), mask->getHcKeyspace());
            currentMask = mask;
            break;
        }
    }

    if (!currentMask)
    {
        /** No masks found, no workunit could be generated */
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "No masks found for this job\n");
        return false;
    }

    uint64_t maskIndex = currentMask->getCurrentIndex();
    uint64_t maskHcKeyspace = currentMask->getHcKeyspace();
    if (maskIndex + passCount > maskHcKeyspace)
    {
        /** Host is too powerful for this mask, it will finish it */
        passCount = maskHcKeyspace - maskIndex;
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adjusting #passwords, mask too small, new keyspace: %" PRIu64 "\n", passCount);
    }

    /** Create new mask workunit */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), maskIndex, 0, passCount,
                         currentMask->getId(), 0, false, 0, false);

    /** Update indexes for job and mask*/
    m_job->updateIndex(m_job->getCurrentIndex() + passCount);
    currentMask->updateIndex(maskIndex + passCount);

    return true;
}
