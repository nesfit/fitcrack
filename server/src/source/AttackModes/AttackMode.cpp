/**
 * @file AttackMode.cpp
 * @brief Abstract base class for all Attack Modes
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include "AttackMode.h"


void AttackMode::setJob(PtrJob & job)
{
    m_job = job;
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
