/**
 * @file job.cpp
 * @brief Source file for fc_job entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Job.h>
#include <Dictionary.h>
#include <SqlLoader.h>


CJob::CJob(DbMap &jobMap, CSqlLoader * sqlLoader)
    :   m_sqlLoader(sqlLoader)
{
    try
    {
        this->m_id = std::stoull(jobMap["id"]);
        this->m_attack = jobMap["attack"];
        this->m_attackMode = std::stoul(jobMap["attack_mode"]);
        this->m_attackSubmode = std::stoul(jobMap["attack_submode"]);
        this->m_distributionMode = std::stoul(jobMap["distribution_mode"]);
        this->m_hashType = std::stoul(jobMap["hash_type"]);
        this->m_status = std::stoul(jobMap["status"]);
        this->m_keyspace = std::stoull(jobMap["keyspace"]);
        this->m_hcKeyspace = std::stoull(jobMap["hc_keyspace"]);
        this->m_currentIndex = std::stoull(jobMap["current_index"]);
        this->m_currentIndex2 = std::stoull(jobMap["current_index_2"]);
        this->m_name = jobMap["name"];
        this->m_secondsPerWorkunit = std::stoull(jobMap["seconds_per_workunit"]);
        this->m_rules = jobMap["rules"];
        if (!jobMap["rules_id"].empty())
            this->m_rules_id = std::stoull(jobMap["rules_id"]);
        this->m_ruleLeft = jobMap["rule_left"];
        this->m_ruleRight = jobMap["rule_right"];
        this->m_charset1 = jobMap["charset1"];
        this->m_charset2 = jobMap["charset2"];
        this->m_charset3 = jobMap["charset3"];
        this->m_charset4 = jobMap["charset4"];
        if (!jobMap["grammar_id"].empty())
            this->m_grammar_id = std::stoull(jobMap["grammar_id"]);
        this->m_markov = jobMap["markov_hcstat"];
        this->m_markovThreshold = std::stoul(jobMap["markov_threshold"]);
        this->m_casePermute = std::stoul(jobMap["case_permute"]);
        this->m_checkDuplicates = std::stoul(jobMap["check_duplicates"]);
        this->m_minPasswordLen = std::stoul(jobMap["min_password_len"]);
        this->m_maxPasswordLen = std::stoul(jobMap["max_password_len"]);
        this->m_minElemInChain = std::stoul(jobMap["min_elem_in_chain"]);
        this->m_maxElemInChain = std::stoul(jobMap["max_elem_in_chain"]);
        this->m_generateRandomRules = std::stoul(jobMap["generate_random_rules"]);
        this->m_splitDictId = std::stoul(jobMap["split_dict_id"]);
        this->m_splitDictIndex = std::stoul(jobMap["split_dict_index"]);
        this->m_splitDictPos = std::stoul(jobMap["split_dict_pos"]);
        this->m_splitRuleIndex = std::stoul(jobMap["split_rule_index"]);
        this->m_optimized = std::stoul(jobMap["optimized"]);
        this->m_slowCandidates = std::stoul(jobMap["slow_candidates"]);
        this->m_killFlag = std::stoul(jobMap["kill"]) != 0;

        uint64_t minSeconds = m_sqlLoader->getAbsoluteMinimumWorkunitSeconds();

        /** Check for valid values */
        if (this->m_secondsPerWorkunit < minSeconds)
        {
            Tools::printDebugJob(Config::DebugType::Warn, this->m_id,
                                 "Found seconds_per_workunit=%" PRIu64 ", falling back to minimum of %" PRIu64"s\n",
                                 this->m_secondsPerWorkunit, minSeconds);
            this->m_secondsPerWorkunit = minSeconds;
        }

        if (this->m_keyspace == 0 && this->m_id != Config::benchAllId)
        {
            Tools::printDebugJob(Config::DebugType::Error, this->m_id,
                                 "Keyspace cannot be 0, setting job to Malformed\n");
            m_sqlLoader->updateJobStatus(this->m_id, Config::JobState::JobMalformed);
        }

        /** Compute second dictionary size */
        if (this->m_attackMode == Config::AttackMode::AttackCombinator &&
            this->m_hcKeyspace != 0)
                m_combSecDictSize = this->m_keyspace / this->m_hcKeyspace;

        /** Load parameters used for adaptive planning */
        m_secondsPassed = m_sqlLoader->getSecondsPassed(this->m_id);
        m_totalPower = m_sqlLoader->getTotalPower(this->m_id);

        m_maxSeconds = m_secondsPassed + minSeconds;

        if(m_maxSeconds > this->m_secondsPerWorkunit)
            m_maxSeconds = this->m_secondsPerWorkunit;

        /** Load workunit timeout */
        m_timeoutFactor = m_sqlLoader->getTimeoutFactor();
        if (this->m_timeoutFactor < Config::minTimeoutFactor)
        {
            Tools::printDebugJob(Config::DebugType::Warn, this->m_id,
                                 "Found timeout_factor=%" PRIu64 ", falling back to minimum of %" PRIu64"\n",
                                 this->m_timeoutFactor, Config::minTimeoutFactor);
            this->m_timeoutFactor = Config::minTimeoutFactor;
        }

        /** Temperature threshold */
        m_hwTempAbort = m_sqlLoader->getHWTempAbort();
    }
    catch(std::logic_error & error)
    {
        Tools::printDebugJob(Config::DebugType::Error, this->m_id,
                             "Error converting jobMap to PtrJob: %s\n", error.what());
        exit(1);
    }
}


PtrJob CJob::create(DbMap &jobMap, CSqlLoader * sqlLoader)
{
    return PtrJob(new CJob(jobMap, sqlLoader));
}


std::string CJob::getTableName()
{
    return Config::tableNameJob;
}


void CJob::updateIndex(uint64_t newIndex)
{
    /** Local update */
    this->m_currentIndex = newIndex;

    /** Database update */
    this->m_sqlLoader->updateJobIndex(this->m_id, newIndex);
}


void CJob::updateIndex2(uint64_t newIndex2)
{
    /** Local update */
    this->m_currentIndex2 = newIndex2;

    /** Database update */
    this->m_sqlLoader->updateJobIndex2(this->m_id, newIndex2);
}


void CJob::updateStatus(Config::JobState newStatus)
{
    /** Local update */
    this->m_status = newStatus;

    /** Database update */
    this->m_sqlLoader->updateJobStatus(this->m_id, newStatus);
}


void CJob::updateStatusOfRunningJob(Config::JobState newStatus)
{
    /** Local update */
    if (this->m_status >= 10)
        this->m_status = newStatus;

    /** Database update */
    this->m_sqlLoader->updateRunningJobStatus(this->m_id, newStatus);
}


void CJob::updateStartTime()
{
    /** Database update only */
    m_sqlLoader->updateStartTimeNow(this->m_id);
}


void CJob::loadMasks(bool useNormalKeyspace)
{
    m_masks.clear();
    auto maskVec = useNormalKeyspace ? m_sqlLoader->loadJobMasksWithNormalKeyspace(this->m_id) : m_sqlLoader->loadJobMasks(this->m_id);
    for (auto & mask : maskVec)
        this->addMask(mask);
}


void CJob::loadDictionaries()
{
    m_dictionaries.clear();
    auto dictVec = m_sqlLoader->loadJobDictionaries(this->m_id);
    for (auto & dict : dictVec)
        this->addDictionary(dict);
}


bool CJob::loadHashes()
{
    if (m_all_hashes.empty()){
        std::vector<std::string> allHashVec = m_sqlLoader->loadJobAllHashes(this->m_id);

        auto it1 = std::begin(allHashVec);
        m_all_hashes.append(*it1);
        ++it1;
        for (auto end = std::end(allHashVec); it1 != end; ++it1)
        {
            m_all_hashes.append("\n");
            m_all_hashes.append(*it1);
        }
    }

    m_hashes.clear();
    std::vector<std::string> hashVec = m_sqlLoader->loadJobHashes(this->m_id);

    if (hashVec.empty())
        return false;

    auto it = std::begin(hashVec);
    m_hashes.append(*it);
    ++it;

    for (auto end = std::end(hashVec); it != end; ++it)
    {
        m_hashes.append("\n");
        m_hashes.append(*it);
    }

    /** Debug output */
    Tools::printDebugJob(Config::DebugType::Log, this->m_id,
                         "Loaded hashes #: %d\n", hashVec.size());
    /** @warning c_str() wont work for binary hashes, but we don't care in debug for now */
    //Tools::printDebug("%s\n", m_hashes.c_str());

    return true;
}

uint64_t CJob::getEndIndex() const
{
    switch(getAttackMode())
    {
    case Config::AttackMode::AttackCombinator:
    case Config::AttackMode::AttackHybridDictMask:
    case Config::AttackMode::AttackHybridMaskDict:
        return getKeyspace()/getHcKeyspace();
    default:
        return getHcKeyspace();
    }
}

uint64_t CJob::createMask(std::string newMask, uint64_t newKeyspace, uint64_t newHcKeyspace, uint64_t incrementMin, uint64_t incrementMax)
{
    return(m_sqlLoader->createMask(this->m_id, newMask, newKeyspace, newHcKeyspace, incrementMin, incrementMax));
}

/**
 * @section Table attributes getters
 */

uint64_t CJob::getId() const
{
    return this->m_id;
}


const std::string & CJob::getAttack() const
{
    return m_attack;
}


uint32_t CJob::getAttackMode() const
{
    return m_attackMode;
}


uint32_t CJob::getAttackSubmode() const
{
    return m_attackSubmode;
}

uint32_t CJob::getDistributionMode() const
{
    return m_distributionMode;
}

uint32_t CJob::getHashType() const
{
    return m_hashType;
}


uint32_t CJob::getStatus() const
{
    return m_status;
}


uint64_t CJob::getKeyspace() const
{
    return m_keyspace;
}


uint64_t CJob::getHcKeyspace() const
{
    return m_hcKeyspace;
}


uint64_t CJob::getCurrentIndex() const
{
    return m_currentIndex;
}


uint64_t CJob::getCurrentIndex2() const
{
    return m_currentIndex2;
}


const std::string & CJob::getName() const
{
    return m_name;
}


uint64_t CJob::getSecondsPerWorkunit() const
{
    return m_secondsPerWorkunit;
}


const std::string & CJob::getRules() const
{
    return m_rules;
}


uint64_t CJob::getRulesId() const
{
    return m_rules_id;
}


const std::string & CJob::getRuleLeft() const
{
    return m_ruleLeft;
}


const std::string & CJob::getRuleRight() const
{
    return m_ruleRight;
}


const std::string & CJob::getGrammar() const
{
    return m_grammar;
}

uint64_t CJob::getGrammarId() const
{
    return m_grammar_id;
}

const std::string & CJob::getMarkov() const
{
    return m_markov;
}


uint32_t CJob::getRandomRulesCount() const
{
    return m_generateRandomRules;
}

bool CJob::getKillFlag() const
{
    return m_killFlag;
}

bool CJob::getCasePermuteFlag() const
{
    return m_casePermute;
}

bool CJob::getCheckDuplicatesFlag() const
{
    return m_checkDuplicates;
}

bool CJob::getOptimizedFlag() const
{
    return m_optimized;
}

bool CJob::getSlowCandidatesFlag() const
{
    return m_slowCandidates;
}


/**
 * @section Getters/Setters for other member variables
 */

std::vector<Config::Ptr<CMask>> CJob::getMasks() const
{
    return m_masks;
}


void CJob::addMask(Config::Ptr<CMask> mask)
{
    m_masks.push_back(mask);
}


std::vector<Config::Ptr<CDictionary>> CJob::getDictionaries() const
{
    return m_dictionaries;
}


std::vector<Config::Ptr<CDictionary>> CJob::getRightDictionaries() const
{
    std::vector<Config::Ptr<CDictionary>> result;

    for (auto & dict : m_dictionaries)
    {
        if (!dict->isLeft())
            result.push_back(dict);
    }

    return result;
}


void CJob::addDictionary(Config::Ptr<CDictionary> dictionary)
{
    m_dictionaries.push_back(dictionary);
}


std::string CJob::getHashes() const
{
    return m_hashes;
}

std::string CJob::getAllHashes() const
{
    return m_all_hashes;
}


uint64_t CJob::getTotalPower() const
{
    return m_totalPower;
}


uint64_t CJob::getSecondsPassed() const
{
    return m_secondsPassed;
}


uint64_t CJob::getMaxSeconds() const
{
    return m_maxSeconds;
}


uint64_t CJob::getCombSecDictSize() const
{
    return m_combSecDictSize;
}


unsigned int CJob::getTimeoutFactor() const
{
    return m_timeoutFactor;
}

unsigned int CJob::getHWTempAbort() const
{
    return m_hwTempAbort;
}


uint32_t CJob::getMarkovThreshold() const
{
    return m_markovThreshold;
}


void CJob::setGrammar(const std::string & grammar)
{
    m_grammar = grammar;
}


const std::string &CJob::getCharset1() const
{
    return m_charset1;
}


const std::string &CJob::getCharset2() const
{
    return m_charset2;
}


const std::string &CJob::getCharset3() const
{
    return m_charset3;
}


const std::string &CJob::getCharset4() const
{
    return m_charset4;
}


uint32_t CJob::getMinPasswordLen() const
{
    return m_minPasswordLen;
}


uint32_t CJob::getMaxPasswordLen() const
{
    return m_maxPasswordLen;
}


uint32_t CJob::getMinElemInChain() const
{
    return m_minElemInChain;
}


uint32_t CJob::getMaxElemInChain() const
{
    return m_maxElemInChain;
}


void CJob::createRuleSplit(uint64_t dictId, uint64_t dictIndex, uint64_t dictPos, uint64_t ruleIndex)
{
    this->m_splitDictId = dictId;
    this->m_splitDictIndex = dictIndex;
    this->m_splitDictPos = dictPos;
    this->m_splitRuleIndex = ruleIndex;

    m_sqlLoader->createRuleSplit(m_id, dictId, dictIndex, dictPos, ruleIndex);
}


void CJob::removeRuleSplit()
{
    this->m_splitDictId = 0;
    this->m_splitDictIndex = 0;
    this->m_splitDictPos = 0;
    this->m_splitRuleIndex = 0;

    m_sqlLoader->removeRuleSplit(m_id);
}


void CJob::updateRuleIndex(uint64_t newRuleIndex)
{
    this->m_splitRuleIndex = newRuleIndex;

    m_sqlLoader->updateRuleIndex(m_id, newRuleIndex);
}


uint64_t CJob::getSplitDictId() const
{
    return m_splitDictId;
}

uint64_t CJob::getSplitDictIndex() const
{
    return m_splitDictIndex;
}


uint64_t CJob::getSplitDictPos() const
{
    return m_splitDictPos;
}


uint64_t CJob::getSplitRuleIndex() const
{
    return m_splitRuleIndex;
}
