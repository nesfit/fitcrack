/**
 * @file AbstractGenerator.cpp
 * @brief Source file for abstract generator
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <sched_util_basic.h>
#include <sched_util.h>
#include "AbstractGenerator.h"


uint64_t CAbstractGenerator::calculateSecondsIcdf2c(PtrJob &job)
{
    uint64_t minimum = Config::minSeconds;
    uint64_t maximum = job->getMaxSeconds();

    uint64_t seconds = ((job->getHcKeyspace() - job->getCurrentIndex()) / (job->getTotalPower() + 1)) / 10;

    if (seconds < minimum)
        seconds = minimum;
    else if (seconds > maximum)
        seconds = maximum;

    Tools::printDebugJob(Config::DebugType::Log, job->getId(),
                         "Calculating seconds: seconds_per_workunit = %" PRIu64 ", real seconds = %" PRIu64"\n",
                         job->getSecondsPerWorkunit(), seconds);

    return seconds;
}


void CAbstractGenerator::activateJobs()
{
    int retval;
    /** Wait for the transitioner to create instances
     * of the workunits we just created.
     * Otherwise we'll create too many workunits.
     */
    double now = dtime();
    while (1)
    {
        daemon_sleep(1);
        double x;

        retval = min_transition_time(x);
        if (retval)
        {
            log_messages.printf(MSG_DEBUG, "min_transition_time failed: %s\n", boincerror(retval));
            break;
        }

        if (x > now)
            break;
    }
}


void CAbstractGenerator::deleteStickyFiles(PtrJob &job, std::vector<PtrHost> &jobHosts)
{
    std::string stickyName;

    /** Get sticky file name according to attack */
    switch (job->getAttackMode())
    {
        case Config::AttackMode::AttackCombinator:
            stickyName = std::string(Config::appName) + "_combinator_" + std::to_string(job->getId());
            break;

        case Config::AttackMode::AttackMask:
            if (job->getAttackSubmode() == 0)
                break;

            stickyName = std::string(Config::appName) + "_markov_" + std::to_string(job->getId());
            break;

        case Config::AttackMode::AttackDict:
            if (job->getAttackSubmode() == 0)
                break;

            stickyName = std::string(Config::appName) + "_rules_" + std::to_string(job->getId());
            break;

        default:
            break;
    }

    if (stickyName.empty())
        return;

    /** Send message to all hosts in fc_host working on job */
    for (PtrHost & host : jobHosts)
        create_delete_file_msg((int)host->getBoincHostId(), stickyName.c_str());
}
