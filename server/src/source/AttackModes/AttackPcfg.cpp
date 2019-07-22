/**
 * @file AttackPcfg.cpp
 * @brief Source file for creation of Pcfg Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 10. 7. 2019
 * @license MIT, see LICENSE
 */

#include <AttackPcfg.h>



CAttackPcfg::CAttackPcfg(PtrPackage & package, PtrHost & host, uint64_t seconds, CSqlLoader * sqlLoader)
        :   AttackMode(package, host, seconds, sqlLoader)
{

}


bool CAttackPcfg::makeJob()
{
    /** Create the job instance first */
    if (!m_job && !generateJob())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE],name2[Config::SQL_BUF_SIZE],name3[Config::SQL_BUF_SIZE],name4[Config::SQL_BUF_SIZE],path[Config::SQL_BUF_SIZE];
    const char* infiles[4];
    int retval;

    /** Make a unique name for the job and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_%d_%d.dict", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name4, Config::SQL_BUF_SIZE, "%s_grammar_%" PRIu64 "", Config::appName, m_package->getId());


    /** Append mode to config */
    retval = config.download_path(name1, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - config. Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    std::ofstream f;
    f.open(path);
    if (!f)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to open config BOINC input file! Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    Tools::printDebug("CONFIG for new job:\n");

    /** Output original config from DB */
    f << m_package->getConfig();
    Tools::printDebug(m_package->getConfig().c_str());

    /** Output mode */
    f << "|||mode|String|1|n|||\n";
    Tools::printDebug("|||mode|String|1|n|||\n");

    f.close();

    /** Create data file */
    retval = config.download_path(name2, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - data. Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    f.open(path);
    if (!f)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to open data BOINC input file! Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    f << m_package->getHashes();
    f.close();

    /** Create preterminals file. */
    retval = config.download_path(name3, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - dict1. Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    f.open(path);
    if (!f)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to open dict1 BOINC input file! Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Asking for preterminals fragment\n");

    /*******/
    /** TODO: gRPC call to collect preterminals and update current index + keyspace */
    /*******/

    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Done\n");
    f.close();


    /** Create grammar sticky file. */
    retval = config.download_path(name4, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - grammar. Skipping workunit.\n");
        return true;
    }

    f.open(path);
    if (!f)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to open grammar BOINC input file! Skipping workunit.\n");
        return true;
    }

    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Creating grammar file\n");

    /********/
    /** TODO: Load grammar path from DB and dump it to BOINC input file  */
    /********/
    /*
    std::ifstream grammarFile;
    grammarFile.open((Config::pcfgDir + grammar->getGrammarFileName() + "/grammar.bin").c_str());
    if (!grammarFile) {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to open dictionary file! Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    f << grammarFile.rdbuf();
    dictFile.close();

    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Done\n");
    f.close();
    */

    /** Fill in the job parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_package->getTimeoutFactor() * (uint32_t)(m_package->getSecondsPerJob());
    infiles[0] = name1;
    infiles[1] = name2;
    infiles[2] = name3;
    infiles[3] = name4;

    setDefaultWorkunitParams(&wu);

    /** Register the job with BOINC */
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
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                              "Failed to create BOINC workunit. Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    restrict_wu_to_host(wu, m_job->getBoincHostId());

    m_job->setWorkunitId(uint64_t(wu.id));
    m_sqlLoader->addNewWorkunit(m_job);

    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Workunit succesfully created\n");
    return true;
}


bool CAttackPcfg::generateJob()
{
    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Generating PCFG workunit ...\n");

    /** Compute password count */
    uint64_t passCount = m_host->getPower() * m_seconds;

    if (passCount < Config::minPassCount)
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_package->getId(), m_host->getBoincHostId(),
                              "Passcount is too small! Falling back to minimum passwords\n");
        passCount = Config::minPassCount;
    }

    /*********/
    /** TODO: Send grammar index to makeJob (in CJob) */
    /*********/

    /** Create the job */
    m_job = CJob::create(m_package->getId(), m_host->getId(), m_host->getBoincHostId(), dictIndex, 0, passCount, 0,
                         currentDict->getId(), false, 0, false);

    /**
     * @warning Index and WU-Keyspace updating must be done later, after reply from PCFG Manager
     */

    return true;
}


void CAttackPcfg::loadNextPreterminals(std::vector<std::string> & preterminals, uint64_t & realKeyspace)
{
    /********/
    /** TODO: Call and parse the gRPC reply */
    /********/

    preterminals.push_down("0,0,[[3,1,[4,1,[]],[1,0,[]]]]");
    realKeyspace = 42;
}

