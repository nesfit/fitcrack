/**
 * @file Package.h
 * @brief Header file for fc_job entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_PACKAGE_H
#define WORKGENERATOR_PACKAGE_H

#include <Config.h>


class CSqlLoader;
class CPackage;
class CMask;
class CDictionary;
using PtrPackage = Config::Ptr<CPackage>;


class CPackage {
    private:
        /**
         * @brief Private constructor, called by create() static function
         * @param packageMap [in] Map representing fc_package table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CPackage(DbMap & packageMap, CSqlLoader * sqlLoader);

    public:
        /** Default destructor */
        ~CPackage() = default;

        /**
         * @brief Creating instance as a shared pointer from database select
         * @param hostMap [in] Map representing fc_package table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         * @return Shared pointer to fc_package entry object
         */
        static PtrPackage create(DbMap &packageMap, CSqlLoader * sqlLoader);

        /**
         * @brief Get the package SQL table name, usually fc_package
         * @return Package SQL table name
         */
        static std::string getTableName();

        /**
         * @brief Updates index of this package object and database entry
         * @param newIndex [in] New mask current index
         */
        void updateIndex(uint64_t newIndex);

        /**
         * @brief Updates index 2 of this package object and database entry
         * @param newIndex [in] New mask current index 2
         */
        void updateIndex2(uint64_t newIndex2);

        /**
         * @brief Updates status of this package object and database entry
         * @param newIndex [in] New package status
         */
        void updateStatus(Config::PackageState newStatus);

        /**
         * @brief Updates status of running package, otherwise, does nothing
         * @param newIndex [in] New package status, original status must be >= 10
         */
        void updateStatusOfRunningPackage(Config::PackageState newStatus);

        /**
         * @brief Updates start time of package database entry to now, only if it is NULL
         */
        void updateStartTime();

        /**
         * @brief Loads all non-exhausted mask database entries for this package to package vector m_masks
         */
        void loadMasks();

        /**
         * @brief Loads all non-exhausted dictionary database entries for this package to package vector m_dictionaries
         */
        void loadDictionaries();

        /**
         * @brief Loads all non-cracked hash database entries fot his package to package string m_hashes
         * @return True if any hashes were loaded, False otherwise
         */
        bool loadHashes();

    private:

        CSqlLoader * m_sqlLoader;         /**< SqlLoader for database updating */

        /**
         * @section Table attributes of fc_package
         */

        uint64_t    m_id;
        std::string m_attack;
        uint32_t    m_attackMode;
        uint32_t    m_attackSubmode;
        uint32_t    m_hashType;
        std::string m_hash;
        uint32_t    m_status;
        uint64_t    m_keyspace;
        uint64_t    m_hcKeyspace;
        uint64_t    m_currentIndex;
        uint64_t    m_currentIndex2;
        std::string m_name;
        uint64_t    m_secondsPerWorkunit;
        std::string m_config;
        std::string m_dict1;
        std::string m_dict2;
        std::string m_rules;
        std::string m_markov;
        uint32_t    m_markovThreshold;
        uint32_t    m_replicateFactor;

        /**
         * @section Other member variables
         */

        uint64_t m_combSecDictSize;     /**< Size of second dictionary in Combinator attack */
        std::vector<Config::Ptr<CMask>> m_masks;    /**< Vector of non-exhausted masks for mask attack */
        std::vector<Config::Ptr<CDictionary>> m_dictionaries;  /**< Vector of non-exhausted dictionaries */
        std::string m_hashes;           /**< Package hash(es) denoted by newlines */

        uint64_t m_totalPower;          /**< Sum of host power for this package */
        uint64_t m_secondsPassed;       /**< Seconds from time_start to now() */
        uint64_t m_maxSeconds;          /**< Maximum time of job */
        unsigned int m_timeoutFactor;   /**< Timeout for jobs, factor of time for a single job */

    public:

        /**
         * @section Getters for table attributes
         */

        uint64_t getId() const;
        const std::string &getAttack() const;
        uint32_t getAttackMode() const;
        uint32_t getAttackSubmode() const;
        uint32_t getHashType() const;
        const std::string & getHash() const;
        uint32_t getStatus() const;
        uint64_t getKeyspace() const;
        uint64_t getHcKeyspace() const;
        uint64_t getCurrentIndex() const;
        uint64_t getCurrentIndex2() const;
        const std::string & getName() const;
        uint64_t getSecondsPerJob() const;
        const std::string & getConfig() const;
        const std::string & getDict1() const;
        const std::string & getDict2() const;
        const std::string & getRules() const;
        const std::string & getMarkov() const;
        uint32_t getMarkovThreshold() const;
        uint32_t getReplicateFactor() const;

    /**
     * @section Getters/Setters for other member variables
     */

        std::vector<Config::Ptr<CMask>> getMasks() const;
        void addMask(Config::Ptr<CMask> mask);

        std::vector<Config::Ptr<CDictionary>> getDictionaries() const;
        std::vector<Config::Ptr<CDictionary>> getRightDictionaries() const;
        void addDictionary(Config::Ptr<CDictionary> dictionary);

        std::string getHashes() const;

        uint64_t getTotalPower() const;
        uint64_t getSecondsPassed() const;
        uint64_t getMaxSeconds() const;
        uint64_t getCombSecDictSize() const;
        unsigned int getTimeoutFactor() const;
};

#endif //WORKGENERATOR_PACKAGE_H
