/**
 * @file AttackMask.cpp
 * @brief Source file for creation of Mask Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackMask.h>


CAttackMask::CAttackMask(PtrPackage & package, PtrHost & host, uint64_t seconds, CSqlLoader * sqlLoader)
    :   AttackMode(package, host, seconds, sqlLoader)
{

}


bool CAttackMask::makeJob()
{
    /** Create the job instance first */
    if (!m_job && !generateJob())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE],name2[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];
    const char* infiles[2];
    int retval;

    /** Make a unique name for the job and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);

    /** Load the job mask to object */
    PtrMask jobMask = m_sqlLoader->loadMask(m_job->getMaskId());

    /** Append mode, mask, start_index, stop_index to config */
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

    /** Output mask */
    f << "|||mask|String|" << jobMask->getMask().length() << "|" << jobMask->getMask() << "|||\n";
    Tools::printDebug("|||mask|String|%d|%s|||\n", jobMask->getMask().length(), jobMask->getMask().c_str());

    /** Output start_index */
    int digits = 0;
    uint64_t num = m_job->getStartIndex();
    do { num /= 10; ++digits; } while (num != 0);    // Count digits
    f << "|||start_index|BigUInt|" << digits << "|" << m_job->getStartIndex() << "|||\n";
    Tools::printDebug("|||start_index|BigUInt|%d|%" PRIu64 "|||\n", digits, m_job->getStartIndex());

    uint64_t maskHcKeyspace = jobMask->getHcKeyspace();
    uint64_t jobHcKeyspace = m_job->getHcKeyspace();

    /** Output stop_index - only if it is not the last job in the current mask */
    if (m_job->getStartIndex() + jobHcKeyspace < maskHcKeyspace)
    {
        digits = 0;
        num = jobHcKeyspace;
        do { num /= 10; ++digits; } while (num != 0);    // Count digits
        f << "|||hc_keyspace|BigUInt|" << digits << "|" << jobHcKeyspace << "|||\n";
        Tools::printDebug("|||hc_keyspace|BigUInt|%d|%" PRIu64 "|||\n", digits, jobHcKeyspace);
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


    /** Fill in the job parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_package->getTimeoutFactor() * (uint32_t)(m_package->getSecondsPerJob());
    infiles[0] = name1;
    infiles[1] = name2;

    setDefaultWorkunitParams(&wu);

    /** Register the job with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            Config::inTemplatePathMask,
            path,
            config.project_path(path),
            infiles,
            2,
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


bool CAttackMask::generateJob()
{
    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
            "Generating mask workunit ...\n");

    /** Compute password count */
    uint64_t passCount = m_host->getPower() * m_seconds;
    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
            "Number of real passwords host could compute: %" PRIu64 "\n", passCount);

    if (passCount < Config::minPassCount)
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_package->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = Config::minPassCount;
    }

    std::vector<PtrMask> maskVec = m_package->getMasks();
    Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
            "Masks left for this package: %" PRIu64 "\n", maskVec.size());

    /** Find the following mask */
    PtrMask currentMask;
    for (PtrMask & mask : maskVec)
    {
        if (mask->getCurrentIndex() < mask->getHcKeyspace())
        {
            /** Mask for a new job found */
            Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                    "Mask found: %s, current index: %" PRIu64 "/%" PRIu64 "\n",
                    mask->getMask().c_str(), mask->getCurrentIndex(), mask->getHcKeyspace());
            currentMask = mask;
            break;
        }
    }

    if (!currentMask)
    {
        /** No masks found, no job could be generated */
        Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                "No masks found for this package\n");
        return false;
    }

    uint64_t maskIndex = currentMask->getCurrentIndex();
    uint64_t maskHcKeyspace = currentMask->getHcKeyspace();
    if (maskIndex + passCount > maskHcKeyspace)
    {
        /** Host is too powerful for this mask, it will finish it */
        passCount = maskHcKeyspace - maskIndex;
        Tools::printDebugHost(Config::DebugType::Log, m_package->getId(), m_host->getBoincHostId(),
                "Adjusting #passwords, mask too small, new #: %" PRIu64 "\n", passCount);
    }

    /** Create new mask job */
    m_job = CJob::create(m_package->getId(), m_host->getId(), m_host->getBoincHostId(), maskIndex, 0, passCount,
                         currentMask->getId(), 0, false, 0, false);

    /** Update indexes for package and mask*/
    m_package->updateIndex(m_package->getCurrentIndex() + passCount);
    currentMask->updateIndex(maskIndex + passCount);

    return true;
}
