/**
 * @file AttackMode.cpp
 * @brief Abstract base class for all Attack Modes
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include "AttackMode.h"


void AttackMode::setWorkunit(PtrWorkunit &workunit)
{
    m_workunit = workunit;
}


void AttackMode::setDefaultWorkunitParams(DB_WORKUNIT * wu)
{
    wu->rsc_fpops_est = 1e12;
    wu->rsc_fpops_bound = 1e16;
    wu->rsc_memory_bound = 1e8;
    wu->rsc_disk_bound = 1e9;
    wu->min_quorum = Config::replicationFactor;
    wu->target_nresults = Config::replicationFactor;
    wu->max_error_results = Config::replicationFactor * 4;
    wu->max_total_results = Config::replicationFactor * 8;
    wu->max_success_results = Config::replicationFactor * 4;
}


std::string AttackMode::generateBasicConfig(char wuMode, unsigned attackMode, unsigned attackSubmode, std::string name, unsigned hashType,
                                            std::string ruleLeft, std::string ruleRight, std::string charset1, std::string charset2,
                                            std::string charset3, std::string charset4)
{
    return std::string();
}

