/**
 * @file SimpleGenerator.cpp
 * @brief Source file for simple generator
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <SimpleGenerator.h>
#include <Config.h>
#include <SqlLoader.h>

#include <AttackBench.h>
#include <AttackCombinator.h>
#include <AttackDict.h>
#include <AttackMask.h>
#include <AttackMarkov.h>
#include <AttackRules.h>



CSimpleGenerator::CSimpleGenerator()
    :   m_sqlLoader(new CSqlLoader)
{

}


CSimpleGenerator::~CSimpleGenerator()
{
    delete m_sqlLoader;

    /** Release the allocated file descriptors */
    Tools::releaseFdMemory();
}


void CSimpleGenerator::run()
{
    Tools::printDebugTimestamp("SimpleGenerator has started.\n");

    while(true)
    {
        /** Set packages with passed time_out to finishing status*/
        m_sqlLoader->finishTimeoutPackages();

        /** Load running packages */
        std::vector<PtrPackage> runningPackages = m_sqlLoader->loadRunningPackages();

        /** Remove finished package hosts */
        removeFinishedHosts();

        /** Generate jobs for each package */
        for (PtrPackage & package : runningPackages)
        {
            uint64_t packageId = package->getId();

            /** Update start_time of package */
            package->updateStartTime();

            /** Generate job for each host */
            std::vector<PtrHost> activeHosts = m_sqlLoader->loadActiveHosts(packageId);
            for (PtrHost & host : activeHosts)
            {
                uint32_t hostStatus = host->getStatus();

                if (hostStatus == Config::HostState::HostBench)
                    createBenchmark(package, host);
                else if (hostStatus == Config::HostState::HostNormal)
                    createRegularJob(package, host);
            }

            /** Check timeout/pause/exhausted status */
            if (package->getStatus() == Config::PackageState::PackageFinishing &&
                    m_sqlLoader->getJobCount(packageId) == 0)
                finishPackage(package);
        }

        /** Wait for BOINC Transitioner */
        activateJobs();
    }
}


void CSimpleGenerator::createBenchmark(PtrPackage & package, PtrHost & host)
{
    if (m_sqlLoader->getBenchCount(package->getId(), host->getId()) == 0 &&
        package->getStatus() == Config::PackageState::PackageRunning)   /**< Do not create bench when Finishing,
                                                                          * causes cycles when bench error occurs */
    {
        /** Check if status is still bench in DB */
        if (m_sqlLoader->getHostStatus(host->getId()) != Config::HostState::HostBench)
            return;

        /** Create Benchmark job */
        CAttackBench bench(package, host, m_sqlLoader);
        bench.makeJob();
    }
}


void CSimpleGenerator::createRegularJob(PtrPackage & package, PtrHost & host)
{
    uint64_t packageId = package->getId();
    uint64_t hostBoincId = host->getBoincHostId();

    if (m_sqlLoader->getJobCount(packageId, host->getId()) >= 2)
    {
        Tools::printDebugHost(Config::DebugType::Log, packageId, hostBoincId,
                "Host has enough workunits\n");
        return;
    }

    /** Load package uncracked hashes */
    if (!package->loadHashes())
    {
        /** All hashes cracked, wait for assimilator to Finish package */
        Tools::printDebugHost(Config::DebugType::Warn, packageId, hostBoincId,
                                 "No hashes found for the package, Assimilator should have already Finished the package\n");

        package->updateStatusOfRunningPackage(Config::PackageState::PackageFinished);

        return;
    }

    /** Load non-exhausted masks/dictionaries */
    if (package->getAttackMode() == Config::AttackMode::AttackMask)
        package->loadMasks();
    else
        package->loadDictionaries();

    /** Calculate workunit duration */
    uint64_t duration = calculateSecondsIcdf2c(package);

    /** Create the workunit */
    AttackMode * attack = nullptr;

    switch (package->getAttackMode())
    {
        case Config::AttackMode::AttackDict:
            if (package->getAttackSubmode() == 0)
                attack = new CAttackDict(package, host, duration, m_sqlLoader);
            else
                attack = new CAttackRules(package, host, duration, m_sqlLoader);
            break;

        case Config::AttackMode::AttackCombinator:
            attack = new CAttackCombinator(package, host, duration, m_sqlLoader);
            break;

        case Config::AttackMode::AttackMask:
            if (package->getAttackSubmode() == 0)
                attack = new CAttackMask(package, host, duration, m_sqlLoader);
            else
                attack = new CAttackMarkov(package, host, duration, m_sqlLoader);
            break;

        default:
            Tools::printDebugHost(Config::DebugType::Error, packageId, hostBoincId,
                    "Attack mode not recognized (%d). Setting package to malformed.\n", package->getAttackMode());
            package->updateStatusOfRunningPackage(Config::PackageState::PackageMalformed);
            return;
    }

    /** Try to set a job from retry */
    bool retryFlag = setEasiestRetry(package, host, attack);

    if (package->getStatus() == Config::PackageState::PackageFinishing && !retryFlag)
    {
        Tools::printDebugHost(Config::DebugType::Log, packageId, hostBoincId,
                              "No more retry jobs available. Job will end soon.\n");
    }
    else if (!attack->makeJob() && package->getStatus() != Config::PackageState::PackageFinishing)
    {
        /** No job could be generated, setting package to finishing */
        Tools::printDebugPackage(Config::DebugType::Log, packageId,
                "No more jobs available. Setting package to finishing\n");
        package->updateStatusOfRunningPackage(Config::PackageState::PackageFinishing);
    }

    delete attack;
}


void CSimpleGenerator::finishPackage(PtrPackage & package)
{
    uint64_t packageId = package->getId();
    uint32_t attackMode = package->getAttackMode();

    if (m_sqlLoader->isPackageTimeout(packageId))
    {
        Tools::printDebugPackage(Config::DebugType::Log, packageId,
                "Timeouted package has finished all jobs. Setting state to timeout.\n", packageId);
        package->updateStatusOfRunningPackage(Config::PackageState::PackageTimeout);
    }
    else if ((attackMode == Config::AttackMode::AttackCombinator && (package->getCurrentIndex() * package->getHcKeyspace() >= package->getKeyspace())) ||
             (attackMode != Config::AttackMode::AttackCombinator && (package->getCurrentIndex() >= package->getHcKeyspace())))
    {
        if (m_sqlLoader->isAnythingCracked(packageId))
        {
            Tools::printDebugPackage(Config::DebugType::Log, packageId,
                                     "Package is exhausted but found some passwords. Setting state to finished.\n", packageId);
            package->updateStatusOfRunningPackage(Config::PackageState::PackageFinished);
            m_sqlLoader->updateEndTimeNow(packageId);
        }
        else
        {
            Tools::printDebugPackage(Config::DebugType::Log, packageId,
                                     "Package is exhausted. Setting state to exhausted.\n", packageId);
            package->updateStatusOfRunningPackage(Config::PackageState::PackageExhausted);
            m_sqlLoader->updateEndTimeNow(packageId);
        }
    }
    else
    {
        Tools::printDebugPackage(Config::DebugType::Log, packageId,
                "Package finished all workunits and was paused.\n", packageId);
        package->updateStatusOfRunningPackage(Config::PackageState::PackageReady);
    }
}


bool CSimpleGenerator::setEasiestRetry(PtrPackage & package, PtrHost & host, AttackMode * attack)
{
    PtrJob retryJob = m_sqlLoader->getEasiestRetry(package->getId());

    if (!retryJob || retryJob->getHcKeyspace() == 0)
        return false;

    Tools::printDebugHost(Config::DebugType::Log, package->getId(), host->getBoincHostId(),
            "Setting attack job from retry.\n");

    /** Modify attributes of new job */
    retryJob->setHostId(host->getId());
    retryJob->setBoincHostId(host->getBoincHostId());

    /** For rules attack, multiply keyspace by rules size */
    if (package->getAttackMode() == Config::AttackMode::AttackDict && package->getAttackSubmode() != 0)
        retryJob->setHcKeyspace(retryJob->getHcKeyspace() * package->getCombSecDictSize());

    /** Set the old retry job to finished */
    m_sqlLoader->setJobFinished(retryJob->getId());

    // TODO: Use some intelligent thresholds to split retry job (if it is too large)
    attack->setJob(retryJob);
    return true;
}


void CSimpleGenerator::removeFinishedHosts()
{
    /** Send them message to delete sticky files */
    std::vector<PtrHost > finishedHosts = m_sqlLoader->loadFinishedHosts();

    PtrPackage tmpPackage;
    std::vector<PtrHost> hostVec;
    for (PtrHost & host : finishedHosts)
    {
        hostVec.push_back(host);
        tmpPackage = m_sqlLoader->loadPackage(host->getPackageId());
        deleteStickyFiles(tmpPackage, hostVec);
    }

    /** Remove them from database */
    m_sqlLoader->removeFinishedHosts();
}
