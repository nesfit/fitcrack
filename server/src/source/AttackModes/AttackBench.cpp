/**
 * @file AttackBench.cpp
 * @brief Source file for creation of Benchmark workunit
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackBench.h>


CAttackBench::CAttackBench(PtrPackage & package, PtrHost & host, CSqlLoader * sqlLoader)
    :   AttackMode(package, host, 0, sqlLoader)
{

}


bool CAttackBench::makeWorkunit()
{
    /** Create the workunit first */
    generateWorkunit();

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];
    const char* infiles[1];
    int retval;

    /** Make a unique name for the workunit and its config file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);

    /** Append bech mode to the config file */
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

    Tools::printDebug("CONFIG for new workunit:\n");

    /** Output original config from DB */
    f << m_package->getConfig();
    Tools::printDebug(m_package->getConfig().c_str());

    /** Output mode */
    if (m_package->getId() == Config::benchAllId)
    {
        /** Benchmark all hash types */
        f << "|||mode|String|1|a|||\n";
        Tools::printDebug("|||mode|String|1|a|||\n");
    }
    else
    {
        f << "|||mode|String|1|b|||\n";
        Tools::printDebug("|||mode|String|1|b|||\n");
    }
    f.close();

    /** Fill in the workunit parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_package->getTimeoutFactor() * (uint32_t)(m_package->getSecondsPerWorkunit());
    infiles[0] = name1;

    setDefaultWorkunitParams(&wu);

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
        wu,
        Config::inTemplatePathBench,
        path,
        config.project_path(path),
        infiles,
        1,
        config
    );

    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_package->getId(), m_host->getBoincHostId(),
                "Failed to create BOINC workunit. Setting package to malformed.\n");
        m_sqlLoader->updateRunningPackageStatus(m_package->getId(), Config::PackageState::PackageMalformed);
        return false;
    }

    restrict_wu_to_host(wu, m_workunit->getBoincHostId());

    m_workunit->setWorkunitId(uint64_t(wu.id));
    m_sqlLoader->addNewWorkunit(m_workunit);

    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                          "Workunit succesfully created\n");
    return true;
}


bool CAttackBench::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
            "Generating benchmark workunit ...\n");

    /** Create benchmark workunit */
    m_workunit = CWorkunit::create(m_package->getId(), m_host->getId(), m_host->getBoincHostId(), 0, 0, 0, 0, 0, false, 0, false);
    return true;
}
