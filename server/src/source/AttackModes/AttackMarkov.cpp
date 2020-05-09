/**
 * @file AttackMarkov.cpp
 * @brief Source file for creation of Markov Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackMarkov.h>


CAttackMarkov::CAttackMarkov(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(std::move(job), host, seconds, sqlLoader)
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

    f << generateBasicConfig(m_job->getAttackMode(), m_job->getAttackSubmode(),
                             m_job->getName(), m_job->getHashType(), 0, m_job->getHWTempAbort(), "", "",
                             m_job->getCharset1(), m_job->getCharset2(), m_job->getCharset3(),
                             m_job->getCharset4());

    /** Output mask */
    f << "|||mask|String|" << workunitMask->getMask().length() << "|" << workunitMask->getMask() << "|||\n";
    Tools::printDebug("|||mask|String|%d|%s|||\n", workunitMask->getMask().length(), workunitMask->getMask().c_str());

    /** Output start_index */
    auto skipLine = makeLimitingConfigLine("start_index", "BigUInt", std::to_string(m_workunit->getStartIndex()));
    f << skipLine;
    Tools::printDebug(skipLine.c_str());

    /** Output markov_threshold */
    uint32_t markov_threshold = m_job->getMarkovThreshold();
    if (markov_threshold != 0) {
      std::string markovThresholdStr = std::to_string(markov_threshold);
      f << "|||markov_threshold|UInt|" << markovThresholdStr.size() << "|"
        << markovThresholdStr << "|||\n";
      Tools::printDebug("|||markov_threshold|UInt|%d|%s|||\n",
                        markovThresholdStr.size(), markovThresholdStr.c_str());
    }

    uint64_t maskHcKeyspace = workunitMask->getHcKeyspace();
    uint64_t workunitHcKeyspace = m_workunit->getHcKeyspace();

    /** Output stop_index - only if it is not the last workunit in the current mask */
    if (m_workunit->getStartIndex() + workunitHcKeyspace < maskHcKeyspace)
    {
        auto limitLine = makeLimitingConfigLine("hc_keyspace", "BigUInt", std::to_string(workunitHcKeyspace));
        f << limitLine;
        Tools::printDebug(limitLine.c_str());
    }
    else
    {
      /** Otherwise, send whole mask_hc_keyspace for correct progress
       * calculation, --limit is omitted */
      std::string maskHcKeyspaceStr = std::to_string(maskHcKeyspace);
      f << "|||mask_hc_keyspace|BigUInt|" << maskHcKeyspaceStr.size() << "|"
        << maskHcKeyspaceStr << "|||\n";
      Tools::printDebug("|||mask_hc_keyspace|BigUInt|%d|%s|||\n",
                        maskHcKeyspaceStr.size(), maskHcKeyspaceStr.c_str());
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
    uint64_t passCount = getPasswordCountToProcess();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Number of real passwords host could compute: %" PRIu64 "\n", passCount);

    if (passCount < getMinPassCount())
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = getMinPassCount();
    }

    std::vector<PtrMask> maskVec = m_job->getMasks();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Masks left for this job: %" PRIu64 "\n", maskVec.size());

    /** Find the following mask */
    PtrMask currentMask = FindCurrentMask(maskVec, false);

    if (!currentMask)
    {
        /** No masks found, no workunit could be generated */
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "No masks found for this job\n");
        return false;
    }
    uint64_t maskHcKeyspace = currentMask->getHcKeyspace();
    uint64_t maskKeyspace = currentMask->getKeyspace();

    uint64_t hcDivisionFactor = maskKeyspace/maskHcKeyspace;
    //round up
    uint64_t hcKeyspace = (passCount+hcDivisionFactor-1)/hcDivisionFactor;

    uint64_t maskIndex = currentMask->getCurrentIndex();
    if (maskIndex + hcKeyspace > maskHcKeyspace)
    {
        /** Host is too powerful for this mask, it will finish it */
        hcKeyspace = maskHcKeyspace - maskIndex;
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adjusting #passwords, mask too small, new #: %" PRIu64 "\n", hcKeyspace);
    }

    /** Create new mask workunit */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), maskIndex, 0, hcKeyspace,
                         currentMask->getId(), 0, false, 0, false);

    /** Update indexes for job and mask*/
    m_job->updateIndex(m_job->getCurrentIndex() + hcKeyspace);
    currentMask->updateIndex(maskIndex + hcKeyspace);

    return true;
}
