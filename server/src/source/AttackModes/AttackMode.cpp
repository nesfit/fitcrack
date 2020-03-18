/**
 * @file AttackMode.cpp
 * @brief Abstract base class for all Attack Modes
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include "AttackMode.h"
#include "Config.h"

#include <fstream>


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
    std::string result = "|||mode|String|1|" + std::string(1, wuMode) + "|||\n";
    result += "|||attack_mode|UInt|" + std::to_string(std::to_string(attackMode).length()) + "|" + std::to_string(attackMode) + "|||\n";
    result += "|||attack_submode|UInt|" + std::to_string(std::to_string(attackSubmode).length()) + "|" + std::to_string(attackSubmode) + "|||\n";
    result += "|||name|String|" + std::to_string(name.length()) + "|" + name + "|||\n";
    result += "|||hash_type|UInt|" + std::to_string(std::to_string(hashType).length()) + "|" + std::to_string(hashType) + "|||\n";

    if (!ruleLeft.empty())
        result += "|||rule_left|String|" + std::to_string(ruleLeft.length()) + "|" + ruleLeft + "|||\n";

    if (!ruleRight.empty())
        result += "|||rule_right|String|" + std::to_string(ruleRight.length()) + "|" + ruleRight + "|||\n";

    if (!ruleRight.empty())
        result += "|||rule_right|String|" + std::to_string(ruleRight.length()) + "|" + ruleRight + "|||\n";

    if (!charset1.empty())
        result += "|||charset1|String|" + std::to_string(charset1.length()) + "|" + charset1 + "|||\n";

    if (!charset2.empty())
        result += "|||charset1|String|" + std::to_string(charset2.length()) + "|" + charset2 + "|||\n";

    if (!charset3.empty())
        result += "|||charset1|String|" + std::to_string(charset3.length()) + "|" + charset3 + "|||\n";

    if (!charset4.empty())
        result += "|||charset1|String|" + std::to_string(charset4.length()) + "|" + charset4 + "|||\n";

    return result;
}

