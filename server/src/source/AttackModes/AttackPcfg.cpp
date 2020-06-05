/**
 * @file AttackPcfg.cpp
 * @brief Source file for creation of Pcfg Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 10. 7. 2019
 * @license MIT, see LICENSE
 */

#include <AttackPcfg.h>
#include <AttackPcfgClient.h>


CAttackPcfg::CAttackPcfg(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
        :   AttackMode(std::move(job), host, seconds, sqlLoader)
{
    m_client = PretermClient(m_job->getId());
}


bool CAttackPcfg::makeWorkunit()
{
    /** Create the workunit instance first */
    if (!m_workunit && !generateWorkunit())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE],name2[Config::SQL_BUF_SIZE],name3[Config::SQL_BUF_SIZE],name4[Config::SQL_BUF_SIZE],path[Config::SQL_BUF_SIZE];
    const char* infiles[4];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_%d_%d.dict", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name4, Config::SQL_BUF_SIZE, "%s_grammar_%" PRIu64 "", Config::appName, m_job->getId());


    /** Create data file */
    std::ofstream f;
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

    /** Create preterminals file. */
    retval = config.download_path(name3, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - dict1. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream outfile(path, std::ofstream::binary);
    if (!outfile)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                              "Failed to open dict1 BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                          "Asking for preterminals fragment ...\n");

    /** gRPC call to collect preterminals and update current index + keyspace */
    std::string preterminals;
    uint64_t newKeyspace = m_workunit->getHcKeyspace();

    /** @workaround Limit keyspace because of client memory problems */
    if (newKeyspace > Config::MAX_PCFG_KEYSPACE)
        newKeyspace = Config::MAX_PCFG_KEYSPACE;

    if(!loadNextPreterminals(preterminals, newKeyspace))
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                              "Failed to fetch response from PCFG Manager!\n");
        return true;
    }

    outfile.write(preterminals.c_str(), sizeof(char) * preterminals.size());
    outfile.close();


    /** Append mode to config */
    retval = config.download_path(name1, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - config. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }


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

    /** Output hc_keyspace */
    auto limitLine = makeLimitingConfigLine("hc_keyspace", "BigUInt", std::to_string(newKeyspace));
    f << limitLine;
    Tools::printDebug(limitLine.c_str());

    f.close();


    /** Create grammar sticky file. */
    retval = config.download_path(name4, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - grammar. Skipping workunit.\n");
        return true;
    }

    if(!std::ifstream(path))
    {
        f.open(path);
        if (!f.is_open())
        {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                                "Failed to open grammar BOINC input file! Skipping workunit.\n");
            return true;
        }

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                            "Creating grammar file %s\n", (Config::pcfgDir + m_job->getGrammar() + "/grammar.bin").c_str());


        /** Load grammar path from DB and dump it to BOINC input file  */
        std::ifstream grammarFile;
        grammarFile.open((Config::pcfgDir + m_job->getGrammar() + "/grammar.bin").c_str());
        if (!grammarFile) {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                                "Failed to open grammar file! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }

        f << grammarFile.rdbuf();
        grammarFile.close();
        f.close();
    }


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
            Config::inTemplatePathPcfg,
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

    /** Update keyspaces */
    m_job->updateIndex(m_job->getCurrentIndex() + newKeyspace);
    m_workunit->setWorkunitId(uint64_t(wu.id));
    m_workunit->setHcKeyspace(newKeyspace);
    m_sqlLoader->addNewWorkunit(m_workunit);

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                          "Workunit succesfully created\n");

    /** Check if we reached end of PCFG keyspace */
    if (m_job->getCurrentIndex() >= m_job->getKeyspace())
    {
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                              "Reached end of keyspace. Setting job to finishing!\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobFinishing);
        m_client.Disconnect();
        m_client.Kill();
    }

    return true;
}


bool CAttackPcfg::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                          "Generating PCFG workunit ...\n");

    /** Compute password count */
    uint64_t passCount = getPasswordCountToProcess();

    if (passCount < getMinPassCount())
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                              "Passcount is too small! Falling back to minimum passwords\n");
        passCount = getMinPassCount();
    }

    if (passCount + m_job->getCurrentIndex() > m_job->getHcKeyspace())
        passCount = m_job->getHcKeyspace() - m_job->getCurrentIndex();

    /** Create the workunit */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), m_job->getCurrentIndex(), 0, passCount, 0, 0,
                         false, 0, false);

    /**
     * @warning Index and WU-Keyspace updating must be done later, after the response from PCFG Manager
     */

    return true;
}


bool CAttackPcfg::loadNextPreterminals(std::string & preterminals, uint64_t & realKeyspace)
{
    /** Run gRPC query to get preterminals */
    if (!m_client.Connect())
        return false;

    preterminals = m_client.GetNextItems(realKeyspace);
    m_client.Acknowledge();
    return !preterminals.empty() && realKeyspace > 0;
}
