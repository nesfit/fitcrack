/**
 * @file Dictionary.h
 * @brief Header file for fc_[job_]dictionary entries
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_DICTIONARY_H
#define WORKGENERATOR_DICTIONARY_H

#include <Config.h>


class CSqlLoader;
class CDictionary;
using PtrDictionary = Config::Ptr<CDictionary>;


class CDictionary {
    private:
        /**
         * @brief Private constructor, called by create() static function
         * @param dictMap [in] Map representing fc_[job_]dictionary table entries
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CDictionary(DbMap & dictMap, CSqlLoader * sqlLoader);

    public:
        /** Default destructor */
        virtual ~CDictionary() = default;


        /**
         * @brief Creating instance as a shared pointer from database select
         * @param dictMap [in] Map representing fc_[job_]dictionary table entries
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         * @return Shared pointer to fc_[job_]dictionary entries object
         */
        static PtrDictionary create(DbMap & dictMap, CSqlLoader * sqlLoader);

        /**
        * @brief Get the mask SQL table name, usually fc_job_dictionary
        * @return Mask SQL table name
        */
        static std::string getTableName();

        /**
         * @brief Updates index of this dictionary object and database entry
         * @param newIndex [in] New dictionary current index
         */
        virtual void updateIndex(uint64_t newIndex);

        /**
         * @brief Updates file position of this dictionary object and database entry
         * @param newIndex [in] New dictionary current index
         */
        virtual void updatePos(uint64_t newPos);


    private:

        CSqlLoader * m_sqlLoader;         /**< SqlLoader for database updating */

        /**
         * @section Table attributes of fc_[job_]dictionary
         */

        uint64_t    m_id;
        uint64_t    m_jobId;
        uint64_t    m_dictionaryId;
        uint64_t    m_currentIndex;
        uint64_t    m_currentPos;
        uint64_t    m_hcKeyspace;
        std::string m_dictFileName;
        std::string m_passwordDistribution;
        bool        m_isLeft;
        bool        m_hexDict;

    public:

        /**
         * @section Getters for table attributes
         */

        uint64_t getId() const;
        uint64_t getJobId() const;
        uint64_t getDictionaryId() const;
        uint64_t getCurrentIndex() const;
        uint64_t getCurrentPos() const;
        uint64_t getHcKeyspace() const;
        const std::string & getDictFileName() const;
        const std::string & getPasswordDistribution() const;
        bool isHexDict() const;
        bool isLeft() const;
};

#endif //WORKGENERATOR_DICTIONARY_H
