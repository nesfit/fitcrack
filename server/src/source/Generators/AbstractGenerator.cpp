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


uint64_t CAbstractGenerator::calculateSecondsIcdf2c(PtrPackage & package)
{
    uint64_t minimum = Config::minSeconds;
    uint64_t maximum = package->getMaxSeconds();

    uint64_t seconds = ((package->getHcKeyspace() - package->getCurrentIndex()) / (package->getTotalPower() + 1)) / 10;

    if (seconds < minimum)
        seconds = minimum;
    else if (seconds > maximum)
        seconds = maximum;

    Tools::printDebugPackage(Config::DebugType::Log, package->getId(),
            "Calculating seconds: seconds_per_job = %" PRIu64 ", real seconds = %" PRIu64"\n",
            package->getSecondsPerJob(), seconds);

    return seconds;
}


void CAbstractGenerator::activateJobs()
{
    int retval;
    /** Wait for the transitioner to create instances
     * of the jobs we just created.
     * Otherwise we'll create too many jobs.
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


void CAbstractGenerator::deleteStickyFiles(PtrPackage & package, std::vector<PtrHost> & packageHosts)
{
    std::string stickyName;

    /** Get sticky file name according to attack */
    switch (package->getAttackMode())
    {
        case Config::AttackMode::AttackCombinator:
            stickyName = std::string(Config::appName) + "_combinator_" + std::to_string(package->getId());
            break;

        case Config::AttackMode::AttackMask:
            if (package->getAttackSubmode() == 0)
                break;

            stickyName = std::string(Config::appName) + "_markov_" + std::to_string(package->getId());
            break;

        case Config::AttackMode::AttackDict:
            if (package->getAttackSubmode() == 0)
                break;

            stickyName = std::string(Config::appName) + "_rules_" + std::to_string(package->getId());
            break;

        default:
            break;
    }

    if (stickyName.empty())
        return;

    /** Send message to all hosts in fc_host working on package */
    for (PtrHost & host : packageHosts)
        create_delete_file_msg((int)host->getBoincHostId(), stickyName.c_str());
}
