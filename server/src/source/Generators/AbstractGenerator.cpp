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
#include "SqlLoader.h"

#include <cmath>

uint64_t CAbstractGenerator::calculateSecondsIcdf2c(PtrJob &job, CSqlLoader &loader)
{
    uint64_t desiredSeconds = job->getSecondsPerWorkunit();
    uint64_t maximum = loader.getEnableRampUp() ? job->getMaxSeconds() : desiredSeconds;
    uint64_t minimum = std::max<uint64_t>(loader.getAbsoluteMinimumWorkunitSeconds(), desiredSeconds*loader.getRampDownCoefficient());

    uint64_t curIndex = job->getCurrentIndex();
    uint64_t passCount = job->getKeyspace();
    //power is in passwords/second, make sure index is counted in passwords
    curIndex *= passCount/job->getEndIndex();
    //in most jobs it is 0, so it won't hurt. When it is not 0, it is always in passwords
    curIndex += job->getCurrentIndex2();

    uint64_t seconds = std::round(((passCount - curIndex) / (job->getTotalPower() + 1))*loader.getDistributionCoefficient());

    if (seconds < minimum)
        seconds = minimum;
    else if (seconds > maximum)
        seconds = maximum;

    Tools::printDebugJob(Config::DebugType::Log, job->getId(),
                         "Calculating seconds: seconds_per_workunit = %" PRIu64 ", real seconds = %" PRIu64"\n",
                         desiredSeconds, seconds);

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

std::vector<std::string> CAbstractGenerator::getStickyFiles(PtrJob &job) {
    std::vector<std::string> stickyFiles;
    /** Get sticky file names according to attack */
    switch (job->getAttackMode())
    {
        case Config::AttackMode::AttackCombinator:
            stickyFiles.push_back(std::string(Config::appName) + "_combinator_" + std::to_string(job->getId()));
            break;

        case Config::AttackMode::AttackMask:
            if (job->getAttackSubmode() == 0)
                break;

            stickyFiles.push_back(std::string(Config::appName) + "_markov_" + std::to_string(job->getId()));
            break;

        case Config::AttackMode::AttackDict:
          if (job->getAttackSubmode() == 1)
            stickyFiles.push_back(std::string(Config::appName) + "_rules_" +
                                  std::to_string(job->getId()));
          break;
        case Config::AttackMode::AttackPrince:
          stickyFiles.push_back(std::string(Config::appName) + "_dict_" +
                                std::to_string(job->getId()));
          if (job->getAttackSubmode() == 1)
            stickyFiles.push_back(std::string(Config::appName) + "_rules_" +
                                  std::to_string(job->getId()));
          break;
        case Config::AttackMode::AttackPcfg:
          stickyFiles.push_back(std::string(Config::appName) + "_grammar_" +
                                std::to_string(job->getId()));
          if (job->getAttackSubmode() == 1)
            stickyFiles.push_back(std::string(Config::appName) + "_rules_" +
                                  std::to_string(job->getId()));
          break;
        
        case Config::AttackMode::AttackHybridDictMask:
          stickyFiles.push_back(std::string(Config::appName) + "_hybrid_" +
                                std::to_string(job->getId())+".dict");
          break;

        default:
          break;
    }

    return stickyFiles;
}

void CAbstractGenerator::deleteStickyFiles(PtrJob &job, std::vector<PtrHost> &jobHosts)
{
    std::vector<std::string> stickyFiles = getStickyFiles(job);
    if (stickyFiles.empty())
        return;

    /** Send message to all hosts in fc_host working on job */
    for (PtrHost &host : jobHosts)
      for (const std::string &stickyFile : stickyFiles)
        create_delete_file_msg((int)host->getBoincHostId(), stickyFile.c_str());
}
