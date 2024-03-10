/**
 * @file Job.h
 * @brief Header file for fc_job entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_JOB_H
#define WORKGENERATOR_JOB_H

#include <Config.h>


class CSqlLoader;
class CJob;
class CMask;
class CDictionary;
using PtrJob = Config::Ptr<CJob>;


class CJob {
    private:
        /**
         * @brief Private constructor, called by create() static function
         * @param jobMap [in] Map representing fc_job table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CJob(DbMap & jobMap, CSqlLoader * sqlLoader);

    public:
        /** Default destructor */
        virtual ~CJob() = default;

        /**
         * @brief Creating instance as a shared pointer from database select
         * @param hostMap [in] Map representing fc_job table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         * @return Shared pointer to fc_job entry object
         */
        static PtrJob create(DbMap &jobMap, CSqlLoader * sqlLoader);

        /**
         * @brief Get the job SQL table name, usually fc_job
         * @return Job SQL table name
         */
        static std::string getTableName();

        /**
         * @brief Updates index of this job object and database entry
         * @param newIndex [in] New mask current index
         */
        virtual void updateIndex(uint64_t newIndex);

        /**
         * @brief Updates index 2 of this job object and database entry
         * @param newIndex [in] New mask current index 2
         */
        virtual void updateIndex2(uint64_t newIndex2);

        /**
         * @brief Updates status of this job object and database entry
         * @param newIndex [in] New job status
         */
        void updateStatus(Config::JobState newStatus);

        /**
         * @brief Updates status of running job, otherwise, does nothing
         * @param newIndex [in] New job status, original status must be >= 10
         */
        void updateStatusOfRunningJob(Config::JobState newStatus);

        /**
         * @brief Updates start time of job database entry to now, only if it is NULL
         */
        void updateStartTime();

        /**
         * @brief Loads all non-exhausted mask database entries for this job to job vector m_masks
         * @param useNormalKeyspace [in] If true, will use keyspace instead of hc keyspace to determine exhaustion
         */
        void loadMasks(bool useNormalKeyspace);

        /**
         * @brief Loads all non-exhausted dictionary database entries for this job to job vector m_dictionaries
         */
        void loadDictionaries();

        /**
         * @brief Loads all non-cracked hash database entries fot his job to job string m_hashes
         * @return True if any hashes were loaded, False otherwise
         */
        bool loadHashes();

        /**
         * @brief Get the index at which the job is finished
         */
        uint64_t getEndIndex() const;

    private:

        CSqlLoader * m_sqlLoader;         /**< SqlLoader for database updating */

        /**
         * @section Table attributes of fc_job
         */

        uint64_t    m_id;
        std::string m_attack;
        uint32_t    m_attackMode;
        uint32_t    m_attackSubmode;
        uint32_t    m_distributionMode;
        uint32_t    m_hashType;
        uint32_t    m_status;
        uint64_t    m_keyspace;
        uint64_t    m_hcKeyspace;
        uint64_t    m_currentIndex;
        uint64_t    m_currentIndex2;
        std::string m_name;
        uint64_t    m_secondsPerWorkunit;
        std::string m_rules;
        uint64_t    m_rules_id;
        std::string m_ruleLeft;
        std::string m_ruleRight;
        std::string m_charset1;
        std::string m_charset2;
        std::string m_charset3;
        std::string m_charset4;
        std::string m_grammar;
        uint64_t    m_grammar_id;
        std::string m_markov;
        uint32_t m_markovThreshold;
        bool m_casePermute;
        bool m_checkDuplicates;
        uint32_t m_minPasswordLen;
        uint32_t m_maxPasswordLen;
        uint32_t m_minElemInChain;
        uint32_t m_maxElemInChain;
        uint32_t m_generateRandomRules;
        uint64_t m_splitDictId;
        uint64_t m_splitDictIndex;
        uint64_t m_splitDictPos;
        uint64_t m_splitRuleIndex;
        bool m_optimized;
        bool m_killFlag;

        /**
         * @section Other member variables
         */

        uint64_t m_combSecDictSize;     /**< Size of second dictionary in Combinator attack */
    protected:
        std::vector<Config::Ptr<CMask>> m_masks;    /**< Vector of non-exhausted masks for mask attack */
        std::vector<Config::Ptr<CDictionary>> m_dictionaries;  /**< Vector of non-exhausted dictionaries */
    private:
        std::string m_hashes;           /**< job hash(es) denoted by newlines */

        uint64_t m_totalPower;          /**< Sum of host power for this job */
        uint64_t m_secondsPassed;       /**< Seconds from time_start to now() */
        uint64_t m_maxSeconds;          /**< Maximum time of workunit */
        unsigned int m_timeoutFactor;   /**< Timeout for workunits, factor of time for a single workunit */
        unsigned int m_hwTempAbort;     /**< Temperature threshold to stop cracking */

    public:

        /**
         * @section Getters for table attributes
         */

        uint64_t getId() const;
        const std::string &getAttack() const;
        uint32_t getAttackMode() const;
        uint32_t getAttackSubmode() const;
        uint32_t getDistributionMode() const;
        uint32_t getHashType() const;
        uint32_t getStatus() const;
        uint64_t getKeyspace() const;
        uint64_t getHcKeyspace() const;
        uint64_t getCurrentIndex() const;
        uint64_t getCurrentIndex2() const;
        const std::string & getName() const;
        uint64_t getSecondsPerWorkunit() const;
        const std::string & getRules() const;
        uint64_t getRulesId() const;
        const std::string & getRuleLeft() const;
        const std::string & getRuleRight() const;
        const std::string & getCharset1() const;
        const std::string & getCharset2() const;
        const std::string & getCharset3() const;
        const std::string & getCharset4() const;
        uint64_t getGrammarId() const;
        const std::string & getMarkov() const;
        uint32_t getMarkovThreshold() const;
        bool getCasePermuteFlag() const;
        bool getCheckDuplicatesFlag() const;
        uint32_t getMinPasswordLen() const;
        uint32_t getMaxPasswordLen() const;
        uint32_t getMinElemInChain() const;
        uint32_t getMaxElemInChain() const;
        uint32_t getRandomRulesCount() const;

        void createRuleSplit(uint64_t dictId, uint64_t dictIndex, uint64_t dictPos, uint64_t ruleIndex);
        void removeRuleSplit();
        void updateRuleIndex(uint64_t newRuleIndex);
        uint64_t getSplitDictId() const;
        uint64_t getSplitDictIndex() const;
        uint64_t getSplitDictPos() const;
        uint64_t getSplitRuleIndex() const;

        bool getOptimizedFlag() const;
        bool getKillFlag() const;

        void setGrammar(const std::string & grammar);

    /**
     * @section Getters/Setters for other member variables
     */

        virtual std::vector<Config::Ptr<CMask>> getMasks() const;
        void addMask(Config::Ptr<CMask> mask);

        virtual std::vector<Config::Ptr<CDictionary>> getDictionaries() const;
        virtual std::vector<Config::Ptr<CDictionary>> getRightDictionaries() const;
        void addDictionary(Config::Ptr<CDictionary> dictionary);

        std::string getHashes() const;

        uint64_t getTotalPower() const;
        uint64_t getSecondsPassed() const;
        uint64_t getMaxSeconds() const;
        uint64_t getCombSecDictSize() const;
        unsigned int getTimeoutFactor() const;
        unsigned int getHWTempAbort() const;

        const std::string & getGrammar() const;
};

#endif //WORKGENERATOR_JOB_H
