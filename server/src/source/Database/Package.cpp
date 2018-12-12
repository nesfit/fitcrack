/**
 * @file Package.cpp
 * @brief Source file for fc_job entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Package.h>
#include <Dictionary.h>
#include <SqlLoader.h>


CPackage::CPackage(DbMap &packageMap, CSqlLoader * sqlLoader)
    :   m_sqlLoader(sqlLoader)
{
    try
    {
        this->m_id = std::stoull(packageMap["id"]);
        this->m_attack = packageMap["attack"];
        this->m_attackMode = std::stoul(packageMap["attack_mode"]);
        this->m_attackSubmode = std::stoul(packageMap["attack_submode"]);
        this->m_hashType = std::stoul(packageMap["hash_type"]);
        this->m_hash = packageMap["hash"];
        this->m_status = std::stoul(packageMap["status"]);
        this->m_keyspace = std::stoull(packageMap["keyspace"]);
        this->m_hcKeyspace = std::stoull(packageMap["hc_keyspace"]);
        this->m_currentIndex = std::stoull(packageMap["current_index"]);
        this->m_currentIndex2 = std::stoull(packageMap["current_index_2"]);
        this->m_name = packageMap["name"];
        this->m_secondsPerWorkunit = std::stoull(packageMap["seconds_per_workunit"]);
        this->m_config = packageMap["config"];
        this->m_dict1 = packageMap["dict1"];
        this->m_dict2 = packageMap["dict2"];
        this->m_rules = packageMap["rules"];
        this->m_markov = packageMap["markov_hcstat"];
        this->m_markovThreshold = std::stoul(packageMap["markov_threshold"]);
        this->m_replicateFactor = std::stoul(packageMap["replicate_factor"]);

        /** Check for valid values */
        if (this->m_secondsPerWorkunit < Config::minSeconds)
        {
            Tools::printDebugPackage(Config::DebugType::Warn, this->m_id,
                    "Found seconds_per_job=%" PRIu64 ", falling back to minimum of %" PRIu64"s\n",
                    this->m_secondsPerWorkunit, Config::minSeconds);
            this->m_secondsPerWorkunit = Config::minSeconds;
        }

        if (this->m_keyspace == 0 && this->m_id != Config::benchAllId)
        {
            Tools::printDebugPackage(Config::DebugType::Error, this->m_id,
                    "Keyspace cannot be 0, setting package to Malformed\n");
            m_sqlLoader->updatePackageStatus(this->m_id, Config::PackageState::PackageMalformed);
        }

        /** Compute second dictionary size */
        if ((this->m_attackMode == Config::AttackMode::AttackCombinator ||             /**< For combinator attacks*/
            (this->m_attackMode == Config::AttackMode::AttackDict && this->m_attackSubmode > 0)) && /**< and rules*/
            this->m_hcKeyspace != 0)
                m_combSecDictSize = this->m_keyspace / this->m_hcKeyspace;

        /** Load parameters used for adaptive planning */
        m_secondsPassed = m_sqlLoader->getSecondsPassed(this->m_id);
        m_totalPower = m_sqlLoader->getTotalPower(this->m_id);

        m_maxSeconds = m_secondsPassed + Config::minSeconds;

        if(m_maxSeconds > this->m_secondsPerWorkunit)
            m_maxSeconds = this->m_secondsPerWorkunit;

        /** Load job timeout */
        m_timeoutFactor = m_sqlLoader->getTimeoutFactor();
        if (this->m_timeoutFactor < Config::minTimeoutFactor)
        {
            Tools::printDebugPackage(Config::DebugType::Warn, this->m_id,
                    "Found timeout_factor=%" PRIu64 ", falling back to minimum of %" PRIu64"\n",
                    this->m_timeoutFactor, Config::minTimeoutFactor);
            this->m_timeoutFactor = Config::minTimeoutFactor;
        }
    }
    catch(std::logic_error & error)
    {
        Tools::printDebugPackage(Config::DebugType::Error, this->m_id,
                "Error converting packageMap to PtrPackage: %s\n", error.what());
        exit(1);
    }
}


PtrPackage CPackage::create(DbMap &packageMap, CSqlLoader * sqlLoader)
{
    return PtrPackage(new CPackage(packageMap, sqlLoader));
}


std::string CPackage::getTableName()
{
    return Config::tableNamePackage;
}


void CPackage::updateIndex(uint64_t newIndex)
{
    /** Local update */
    this->m_currentIndex = newIndex;

    /** Database update */
    this->m_sqlLoader->updatePackageIndex(this->m_id, newIndex);
}


void CPackage::updateIndex2(uint64_t newIndex2)
{
    /** Local update */
    this->m_currentIndex2 = newIndex2;

    /** Database update */
    this->m_sqlLoader->updatePackageIndex2(this->m_id, newIndex2);
}


void CPackage::updateStatus(Config::PackageState newStatus)
{
    /** Local update */
    this->m_status = newStatus;

    /** Database update */
    this->m_sqlLoader->updatePackageStatus(this->m_id, newStatus);
}


void CPackage::updateStatusOfRunningPackage(Config::PackageState newStatus)
{
    /** Local update */
    if (this->m_status >= 10)
        this->m_status = newStatus;

    /** Database update */
    this->m_sqlLoader->updateRunningPackageStatus(this->m_id, newStatus);
}


void CPackage::updateStartTime()
{
    /** Database update only */
    m_sqlLoader->updateStartTimeNow(this->m_id);
}


void CPackage::loadMasks()
{
    m_masks.clear();
    auto maskVec = m_sqlLoader->loadPackageMasks(this->m_id);
    for (auto & mask : maskVec)
        this->addMask(mask);
}


void CPackage::loadDictionaries()
{
    m_dictionaries.clear();
    auto dictVec = m_sqlLoader->loadPackageDictionaries(this->m_id);
    for (auto & dict : dictVec)
        this->addDictionary(dict);
}


bool CPackage::loadHashes()
{
    m_hashes.clear();
    std::vector<std::string> hashVec = m_sqlLoader->loadPackageHashes(this->m_id);

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
    Tools::printDebugPackage(Config::DebugType::Log, this->m_id,
            "Loaded hashes #: %d\n", hashVec.size());
    /** @warning c_str() wont work for binary hashes, but we don't care in debug for now */
    //Tools::printDebug("%s\n", m_hashes.c_str());

    return true;
}


/**
 * @section Table attributes getters
 */

uint64_t CPackage::getId() const
{
    return this->m_id;
}


const std::string & CPackage::getAttack() const
{
    return m_attack;
}


uint32_t CPackage::getAttackMode() const
{
    return m_attackMode;
}


uint32_t CPackage::getAttackSubmode() const
{
    return m_attackSubmode;
}


uint32_t CPackage::getHashType() const
{
    return m_hashType;
}


const std::string &CPackage::getHash() const
{
    return m_hash;
}


uint32_t CPackage::getStatus() const
{
    return m_status;
}


uint64_t CPackage::getKeyspace() const
{
    return m_keyspace;
}


uint64_t CPackage::getHcKeyspace() const
{
    return m_hcKeyspace;
}


uint64_t CPackage::getCurrentIndex() const
{
    return m_currentIndex;
}


uint64_t CPackage::getCurrentIndex2() const
{
    return m_currentIndex2;
}


const std::string & CPackage::getName() const
{
    return m_name;
}


uint64_t CPackage::getSecondsPerJob() const
{
    return m_secondsPerWorkunit;
}


const std::string & CPackage::getConfig() const
{
    return m_config;
}


const std::string & CPackage::getDict1() const
{
    return m_dict1;
}


const std::string & CPackage::getDict2() const
{
    return m_dict2;
}


const std::string & CPackage::getRules() const
{
    return m_rules;
}


const std::string & CPackage::getMarkov() const
{
    return m_markov;
}


uint32_t CPackage::getReplicateFactor() const
{
    return m_replicateFactor;
}


/**
 * @section Getters/Setters for other member variables
 */

std::vector<Config::Ptr<CMask>> CPackage::getMasks() const
{
    return m_masks;
}


void CPackage::addMask(Config::Ptr<CMask> mask)
{
    m_masks.push_back(mask);
}


std::vector<Config::Ptr<CDictionary>> CPackage::getDictionaries() const
{
    return m_dictionaries;
}


std::vector<Config::Ptr<CDictionary>> CPackage::getRightDictionaries() const
{
    std::vector<Config::Ptr<CDictionary>> result;

    for (auto & dict : m_dictionaries)
    {
        if (!dict->isLeft())
            result.push_back(dict);
    }

    return result;
}


void CPackage::addDictionary(Config::Ptr<CDictionary> dictionary)
{
    m_dictionaries.push_back(dictionary);
}


std::string CPackage::getHashes() const
{
    return m_hashes;
}


uint64_t CPackage::getTotalPower() const
{
    return m_totalPower;
}


uint64_t CPackage::getSecondsPassed() const
{
    return m_secondsPassed;
}


uint64_t CPackage::getMaxSeconds() const
{
    return m_maxSeconds;
}


uint64_t CPackage::getCombSecDictSize() const
{
    return m_combSecDictSize;
}


unsigned int CPackage::getTimeoutFactor() const
{
    return m_timeoutFactor;
}


uint32_t CPackage::getMarkovThreshold() const
{
    return m_markovThreshold;
}
