/**
 * @file Job.h
 * @brief Header file for fc_workunit entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_JOB_H
#define WORKGENERATOR_JOB_H

#include <Config.h>


class CSqlLoader;
class CJob;
using PtrJob = Config::Ptr<CJob>;


class CJob {
    private:
        /**
         * @brief Private constructor, called by create() static function
         * @param jobMap [in] Map representing fc_job table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CJob(DbMap & jobMap);

        /**
         * @brief Private constructor, called by create() static function
         * @param ... [in] Attributes needed by database
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CJob(uint64_t & packageId, uint64_t & hostId, uint64_t & boincHostId, uint64_t & startIndex,
                      uint64_t & startIndex2, uint64_t & hcKeyspace, uint64_t & maskId, uint64_t & dictionaryId,
                      bool & duplicated, uint64_t & duplicate, bool & retry);

    public:
        /** Default destructor */
        ~CJob() = default;

        /**
         * @brief Creating instance as a shared pointer from database select
         * @param jobMap [in] Map representing fc_job table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         * @return Shared pointer to fc_job entry object
         */
        static PtrJob create(DbMap & jobMap, CSqlLoader * sqlLoader);

        /**
         * @brief Creating instance as a shared pointer from generated job data
         * @param ... [in] Attributes needed by database
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        static PtrJob create(uint64_t packageId, uint64_t hostId, uint64_t boincHostId, uint64_t startIndex,
                             uint64_t startIndex2, uint64_t hcKeyspace, uint64_t maskId, uint64_t dictionaryId,
                             bool duplicated, uint64_t duplicate, bool retry);

        /**
         * @brief Get the job SQL table name, usually fc_job
         * @return Job SQL table name
         */
        static std::string getTableName();

    private:

        /**
         * @section Table attributes of fc_job
         */

        uint64_t m_id;
        uint64_t m_packageId;
        uint64_t m_workunitId;
        uint64_t m_hostId;
        uint64_t m_boincHostId;

        uint64_t m_startIndex;
        uint64_t m_startIndex2;
        uint64_t m_hcKeyspace;
        uint64_t m_maskId;
        uint64_t m_dictionaryId;
        bool     m_duplicated;
        uint64_t m_duplicate;
        bool     m_retry;
        bool     m_finished;

        /**
         * @section Other used flags
         */

        bool m_finishJob;  /**< When combinator hc_keyspace reached, set flag -> no stop index sent, nothing ignored */

public:

        /**
         * @section Getters/Setters for table attributes
         */

        uint64_t getId() const;
        uint64_t getPackageId() const;
        uint64_t getWorkunitId() const;
        uint64_t getHostId() const;
        uint64_t getBoincHostId() const;
        uint64_t getStartIndex() const;
        uint64_t getStartIndex2() const;
        uint64_t getHcKeyspace() const;
        uint64_t getMaskId() const;
        uint64_t getDictionaryId() const;
        bool isDuplicated() const;
        uint64_t getDuplicate() const;
        bool isRetry() const;
        bool isFinished() const;
        bool isFinishJob() const;

        void setWorkunitId(uint64_t workunitId);
        void setHcKeyspace(uint64_t hcKeyspace);
        void setRetry(bool retry);
        void setHostId(uint64_t hostId);
        void setBoincHostId(uint64_t boincHostId);
        void setDuplicated(bool duplicated);
        void setDuplicate(uint64_t duplicate);
};

#endif //WORKGENERATOR_JOB_H
