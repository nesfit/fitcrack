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
        this->m_hashType = std::stoul(jobMap["hash_type"]);
        this->m_hash = jobMap["hash"];
        this->m_status = std::stoul(jobMap["status"]);
        this->m_keyspace = std::stoull(jobMap["keyspace"]);
        this->m_hcKeyspace = std::stoull(jobMap["hc_keyspace"]);
        this->m_currentIndex = std::stoull(jobMap["current_index"]);
        this->m_currentIndex2 = std::stoull(jobMap["current_index_2"]);
        this->m_name = jobMap["name"];
        this->m_secondsPerWorkunit = std::stoull(jobMap["seconds_per_workunit"]);
        this->m_config = jobMap["config"];
        this->m_dict1 = jobMap["dict1"];
        this->m_dict2 = jobMap["dict2"];
        this->m_rules = jobMap["rules"];
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
        this->m_replicateFactor = std::stoul(jobMap["replicate_factor"]);
        this->m_killFlag = std::stoul(jobMap["kill"]) != 0;

        /** Check for valid values */
        if (this->m_secondsPerWorkunit < Config::minSeconds)
        {
            Tools::printDebugJob(Config::DebugType::Warn, this->m_id,
                                 "Found seconds_per_workunit=%" PRIu64 ", falling back to minimum of %" PRIu64"s\n",
                                 this->m_secondsPerWorkunit, Config::minSeconds);
            this->m_secondsPerWorkunit = Config::minSeconds;
        }

        if (this->m_keyspace == 0 && this->m_id != Config::benchAllId)
        {
            Tools::printDebugJob(Config::DebugType::Error, this->m_id,
                                 "Keyspace cannot be 0, setting job to Malformed\n");
            m_sqlLoader->updateJobStatus(this->m_id, Config::JobState::JobMalformed);
        }

        /** Compute second dictionary size */
        if ((this->m_attackMode == Config::AttackMode::AttackCombinator ||             /**< For combinator attacks*/
            (this->m_attackMode == Config::AttackMode::AttackDict && this->m_attackSubmode > 0) ||    /**< and rules*/
            (this->m_attackMode == Config::AttackMode::AttackPcfg && this->m_attackSubmode > 0)) &&   /**< and PCFG rules*/
            this->m_hcKeyspace != 0)
                m_combSecDictSize = this->m_keyspace / this->m_hcKeyspace;

        /** Load parameters used for adaptive planning */
        m_secondsPassed = m_sqlLoader->getSecondsPassed(this->m_id);
        m_totalPower = m_sqlLoader->getTotalPower(this->m_id);

        m_maxSeconds = m_secondsPassed + Config::minSeconds;

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


void CJob::loadMasks()
{
    m_masks.clear();
    auto maskVec = m_sqlLoader->loadJobMasks(this->m_id);
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


uint32_t CJob::getHashType() const
{
    return m_hashType;
}


const std::string &CJob::getHash() const
{
    return m_hash;
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


const std::string & CJob::getConfig() const
{
    return m_config;
}


const std::string & CJob::getDict1() const
{
    return m_dict1;
}


const std::string & CJob::getDict2() const
{
    return m_dict2;
}


const std::string & CJob::getRules() const
{
    return m_rules;
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


uint32_t CJob::getReplicateFactor() const
{
    return m_replicateFactor;
}


bool CJob::getKillFlag() const
{
    return m_killFlag;
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


uint32_t CJob::getMarkovThreshold() const
{
    return m_markovThreshold;
}


void CJob::setGrammar(const std::string & grammar)
{
    m_grammar = grammar;
}
