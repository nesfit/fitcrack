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
#include <AttackHybridDictMask.h>
#include <AttackHybridMaskDict.h>
#include <AttackDict.h>
#include <AttackMask.h>
#include <AttackMarkov.h>
#include <AttackRules.h>
#include <AttackPcfg.h>
#include <AttackPcfgRules.h>
#include <AttackPrince.h>
#include <AttackBenchAll.h>


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
        /** Set jobs with passed time_out to finishing status*/
        m_sqlLoader->finishTimeoutJobs();

        /** Load running jobs */
        std::vector<PtrJob> runningJobs = m_sqlLoader->loadRunningJobs();

        /** Remove finished job hosts */
        removeFinishedHosts();

        /** Generate workunits for each job */
        for (PtrJob & job : runningJobs)
        {
            /** Process kill requests */
            if (job->getKillFlag())
            {
                m_sqlLoader->killJob(job);
                continue;
            }

            uint64_t jobId = job->getId();

            /** Update start_time of job */
            job->updateStartTime();

            /** Load separate grammar name */
            if (job->getAttackMode() == Config::AttackMode::AttackPcfg)
                job->setGrammar(m_sqlLoader->loadGrammarName(job->getGrammarId()));

            /** Generate workunit for each host */
            std::vector<PtrHost> activeHosts = m_sqlLoader->loadActiveHosts(jobId);
            for (PtrHost & host : activeHosts)
            {
                uint32_t hostStatus = host->getStatus();

                if (hostStatus == Config::HostState::HostBench)
                    createWorkunit(job, host, true);
                else if (hostStatus == Config::HostState::HostNormal)
                    createWorkunit(job, host, false);
            }

            /** Check timeout/pause/exhausted status */
            if (job->getStatus() == Config::JobState::JobFinishing &&
                    m_sqlLoader->getWorkunitCount(jobId) == 0)
                finishJob(job);
        }

        /** Wait for BOINC Transitioner */
        activateJobs();
    }
}

template <template <typename AttackType> class AttackTypeMaker>
AttackMode *CreateAttack(PtrJob &job, PtrHost &host, uint64_t duration, CSqlLoader *sqlLoader)
{
    switch (job->getAttackMode())
    {
        case Config::AttackMode::AttackDict:
            if (job->getAttackSubmode() == 0)
                return AttackTypeMaker<CAttackDict>::CreateAttack(job, host, duration, sqlLoader);
            else
                return AttackTypeMaker<CAttackRules>::CreateAttack(job, host, duration, sqlLoader);
            break;

        case Config::AttackMode::AttackCombinator:
            return AttackTypeMaker<CAttackCombinator>::CreateAttack(job, host, duration, sqlLoader);
            break;

        case Config::AttackMode::AttackMask:
            if (job->getAttackSubmode() == 0)
                return AttackTypeMaker<CAttackMask>::CreateAttack(job, host, duration, sqlLoader);
            else
                return AttackTypeMaker<CAttackMarkov>::CreateAttack(job, host, duration, sqlLoader);
            break;

        case Config::AttackMode::AttackPcfg:
            if (job->getAttackSubmode() == 0)
                return AttackTypeMaker<CAttackPcfg>::CreateAttack(job, host, duration, sqlLoader);
            else
                return AttackTypeMaker<CAttackPcfgRules>::CreateAttack(job, host, duration, sqlLoader);
            break;

        case Config::AttackMode::AttackPrince:
            return AttackTypeMaker<CAttackPrince>::CreateAttack(job, host, duration, sqlLoader);

        case Config::AttackMode::AttackHybridDictMask:
            return AttackTypeMaker<CAttackHybridDictMask>::CreateAttack(job, host, duration, sqlLoader);

        case Config::AttackMode::AttackHybridMaskDict:
            return AttackTypeMaker<CAttackHybridMaskDict>::CreateAttack(job, host, duration, sqlLoader);

        default:
            Tools::printDebugHost(Config::DebugType::Error, job->getId(), host->getId(),
                    "Attack mode not recognized (%d). Setting job to malformed.\n", job->getAttackMode());
            job->updateStatusOfRunningJob(Config::JobState::JobMalformed);
            return nullptr;
    }
}

template <typename AttackType>
struct BenchmarkAttackMaker
{
    static AttackMode *CreateAttack(PtrJob &job, PtrHost &host, uint64_t, CSqlLoader *sqlLoader)
    {
        return new CAttackBench<AttackType>(job, host, sqlLoader);
    }
};

template <typename AttackType>
struct NormalAttackMaker
{
    static AttackMode *CreateAttack(PtrJob &job, PtrHost &host, uint64_t duration, CSqlLoader *sqlLoader)
    {
        return new AttackType(job, host, duration, sqlLoader);
    }
};

void CSimpleGenerator::createWorkunit(PtrJob &job, PtrHost &host, bool isBenchmark)
{
    uint64_t jobId = job->getId();
    uint64_t hostBoincId = host->getBoincHostId();

    AttackMode * attack;
    
    if(job->getId() == Config::benchAllId)
    {
        if(m_sqlLoader->getTotalWorkunitCount(jobId, host->getId()) > 0)
        {
            Tools::printDebugHost(Config::DebugType::Log, jobId, hostBoincId,
                    "Host has already been benchmarked or is being benchmarked\n");
            return;
        }
        attack = new CAttackBenchAll(job, host, m_sqlLoader);
    }
    else
    {
        if (
            (!isBenchmark && m_sqlLoader->getWorkunitCount(jobId, host->getId()) >= 2) ||
            (isBenchmark && m_sqlLoader->getBenchCount(jobId, host->getId()) >= 1)
        )
        {
            Tools::printDebugHost(Config::DebugType::Log, jobId, hostBoincId,
                    "Host has enough workunits\n");
            return;
        }

        /** Load job uncracked hashes */
        if (!job->loadHashes())
        {
            /** All hashes cracked, wait for assimilator to Finish job */
            Tools::printDebugHost(Config::DebugType::Warn, jobId, hostBoincId,
                                    "No hashes found for the job, Assimilator should have already Finished the job\n");

            job->updateStatusOfRunningJob(Config::JobState::JobFinished);

            return;
        }

        /** Calculate workunit duration */
        uint64_t duration = calculateSecondsIcdf2c(job);

        /** Create the workunit */
        if(isBenchmark)
        {
            attack = CreateAttack<BenchmarkAttackMaker>(job, host, duration, m_sqlLoader);
        }
        else
        {
            attack = CreateAttack<NormalAttackMaker>(job, host, duration, m_sqlLoader);
        }

        /** Load non-exhausted masks/dictionaries */
        if(attack->requiresMasks())
            job->loadMasks(attack->masksUseRealKeyspace());
        if(attack->requiresDicts())
            job->loadDictionaries();
    }
    /** Try to set a workunit from retry */
    bool retryFlag = false;
    if(!isBenchmark)
    {
        retryFlag = setEasiestRetry(job, host, attack);
    }

    if (job->getStatus() == Config::JobState::JobFinishing && !retryFlag)
    {
        Tools::printDebugHost(Config::DebugType::Log, jobId, hostBoincId,
                              "No more retry workunits available. Job will end soon.\n");
    }
    else if (!attack->makeWorkunit() && job->getStatus() != Config::JobState::JobFinishing)
    {
        /** No workunit could be generated, setting job to finishing */
        Tools::printDebugJob(Config::DebugType::Log, jobId,
                             "No more workunits available. Setting job to finishing\n");
        job->updateStatusOfRunningJob(Config::JobState::JobFinishing);
    }

    delete attack;
}


void CSimpleGenerator::finishJob(PtrJob &job)
{
    uint64_t jobId = job->getId();

    if (m_sqlLoader->isJobTimeout(jobId))
    {
        Tools::printDebugJob(Config::DebugType::Log, jobId,
                             "Timeouted job has finished all workunits. Setting state to timeout.\n", jobId);
        job->updateStatusOfRunningJob(Config::JobState::JobTimeout);
    }
    else if (job->isKeyspaceExhausted())
    {
        if (m_sqlLoader->isAnythingCracked(jobId))
        {
            Tools::printDebugJob(Config::DebugType::Log, jobId,
                                 "Job is exhausted but found some passwords. Setting state to finished.\n",
                                 jobId);
            job->updateStatusOfRunningJob(Config::JobState::JobFinished);
            m_sqlLoader->updateEndTimeNow(jobId);
        }
        else
        {
            Tools::printDebugJob(Config::DebugType::Log, jobId,
                                 "Job is exhausted. Setting state to exhausted.\n", jobId);
            job->updateStatusOfRunningJob(Config::JobState::JobExhausted);
            m_sqlLoader->updateEndTimeNow(jobId);
        }
    }
    else
    {
        Tools::printDebugJob(Config::DebugType::Log, jobId,
                             "Job finished all workunits and was paused.\n", jobId);
        job->updateStatusOfRunningJob(Config::JobState::JobReady);
    }
}


bool CSimpleGenerator::setEasiestRetry(PtrJob &job, PtrHost &host, AttackMode *attack)
{
    PtrWorkunit retryWorkunit = m_sqlLoader->getEasiestRetry(job->getId());

    if (!retryWorkunit || retryWorkunit->getHcKeyspace() == 0)
        return false;

    Tools::printDebugHost(Config::DebugType::Log, job->getId(), host->getBoincHostId(),
            "Setting attack workunit from retry.\n");

    /** Modify attributes of new workunit */
    retryWorkunit->setHostId(host->getId());
    retryWorkunit->setBoincHostId(host->getBoincHostId());

    /** For rules attack, multiply keyspace by rules size */
    if (job->getAttackMode() == Config::AttackMode::AttackDict && job->getAttackSubmode() != 0)
        retryWorkunit->setHcKeyspace(retryWorkunit->getHcKeyspace() * job->getCombSecDictSize());

    /** Set the old retry workunit to finished */
    m_sqlLoader->setWorkunitFinished(retryWorkunit->getId());

    // TODO: Use some intelligent thresholds to split retry workunit (if it is too large)
    attack->setWorkunit(retryWorkunit);
    return true;
}


void CSimpleGenerator::removeFinishedHosts()
{
    /** Send them message to delete sticky files */
    std::vector<PtrHost > finishedHosts = m_sqlLoader->loadFinishedHosts();

    PtrJob tmpJob;
    std::vector<PtrHost> hostVec;
    for (PtrHost & host : finishedHosts)
    {
        hostVec.push_back(host);
        tmpJob = m_sqlLoader->loadJob(host->getJobId());
        deleteStickyFiles(tmpJob, hostVec);
    }

    /** Remove them from database */
    m_sqlLoader->removeFinishedHosts();
}
