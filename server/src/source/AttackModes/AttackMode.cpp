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

#include <algorithm>
#include <sstream>

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


std::string AttackMode::generateBasicConfig(unsigned attackMode, unsigned attackSubmode, std::string name, unsigned hashType, unsigned generateRandomRules,
                                            unsigned hwTempAbort, bool optimized, std::string ruleLeft, std::string ruleRight, std::string charset1, std::string charset2,
                                            std::string charset3, std::string charset4)
{
    std::string result = "|||mode|String|1|" + std::string(1, getModeLetter()) + "|||\n";
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
        result += "|||charset2|String|" + std::to_string(charset2.length()) + "|" + charset2 + "|||\n";

    if (!charset3.empty())
        result += "|||charset3|String|" + std::to_string(charset3.length()) + "|" + charset3 + "|||\n";

    if (!charset4.empty())
        result += "|||charset4|String|" + std::to_string(charset4.length()) + "|" + charset4 + "|||\n";

    if (generateRandomRules)
      result += "|||generate_random_rules|UInt|" +
                std::to_string(std::to_string(generateRandomRules).length()) +
                "|" + std::to_string(generateRandomRules) + "|||\n";

    if (hwTempAbort)
      result += "|||hwmon_temp_abort|UInt|" +
                std::to_string(std::to_string(hwTempAbort).length()) + "|" +
                std::to_string(hwTempAbort) + "|||\n";

    result += "|||optimized|UInt|1|" << std::to_string(optimized) << "|||\n";

    Tools::printDebug("%s", result.c_str());
    return result;
}

std::string AttackMode::makeConfigLine(const std::string &option, const std::string &type, const std::string &value)
{
    std::ostringstream stream;
    stream << "|||" << option << '|' << type << '|' << value.length() << '|' << value << "|||\n";
    return stream.str();
}

std::unique_ptr<InputDict> AttackMode::makeInputDict(PtrDictionary dict, uint64_t startIndex, bool)
{
    return std::unique_ptr<InputDict>(new InputDict(dict, startIndex));
}

std::unique_ptr<MaskSplitter> AttackMode::makeMaskSplitter(std::vector<std::string> customCharsets)
{
    return std::unique_ptr<MaskSplitter>(new MaskSplitter(std::move(customCharsets)));
}

PtrDictionary AttackMode::FindCurrentDict(std::vector<PtrDictionary> &dicts) const
{
    for (PtrDictionary & dict : dicts)
    {
        if (dict->getCurrentIndex() < dict->getHcKeyspace())
        {
            /** Dictionary for a new workunit found */
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Dict found: %s, current index: %" PRIu64 "/%" PRIu64 "\n",
                    dict->getDictFileName().c_str(), dict->getCurrentIndex(), dict->getHcKeyspace());
            return dict;
        }
    }
    return nullptr;
}

PtrDictionary AttackMode::GetWorkunitDict() const
{
    return m_sqlLoader->loadDictionary(m_workunit->getDictionaryId());
}

PtrMask AttackMode::GetWorkunitMask() const
{
    return m_sqlLoader->loadMask(m_workunit->getMaskId());
}

PtrMask AttackMode::FindCurrentMask(std::vector<PtrMask> &masks, bool useRealKeyspace) const
{
    for (PtrMask & mask : masks)
    {
        auto maskKeyspace = useRealKeyspace ? mask->getKeyspace() : mask->getHcKeyspace();
        if (mask->getCurrentIndex() < maskKeyspace)
        {
            /** Mask for a new workunit found */
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Mask found: %s, current index: %" PRIu64 "/%" PRIu64 "\n",
                    mask->getMask().c_str(), mask->getCurrentIndex(), maskKeyspace);
            return mask;
        }
    }
    return nullptr;
}

uint64_t AttackMode::getPasswordCountToProcess() const
{
    return m_host->getPower()*m_seconds;
}
