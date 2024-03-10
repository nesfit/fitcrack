/**
 * @file Mask.h
 * @brief Header file for fc_mask entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_MASK_H
#define WORKGENERATOR_MASK_H

#include <Config.h>


class CSqlLoader;
class CMask;
using PtrMask = Config::Ptr<CMask>;


class CMask {
    private:
        /**
         * @brief Private constructor, called by create() static function
         * @param maskMap [in] Map representing fc_mask table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CMask(DbMap & maskMap, CSqlLoader * sqlLoader);

    public:
        /** Default destructor */
        virtual ~CMask() = default;


        /**
         * @brief Creating instance as a shared pointer from database select
         * @param maskMap [in] Map representing fc_mask table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         * @return Shared pointer to fc_mask entry object
         */
        static PtrMask create(DbMap & maskMap, CSqlLoader * sqlLoader);

        /**
         * @brief Get the mask SQL table name, usually fc_mask
         * @return Mask SQL table name
         */
        static std::string getTableName();

        /**
         * @brief Updates index of this mask object and database entry
         * @param newIndex [in] New mask current index
         */
        virtual void updateIndex(uint64_t newIndex);

        /**
         * @brief Updates mask
         * @param newMask [in] New mask string
         * @param newKeyspace [in] New real keyspace
         * @param newHcKeyspace [in] New hashcat keyspace
         * @param incrementMin [in] New increment min size
         * @param incrementMax [in] New increment max size
         */
        virtual void updateMask(std::string newMask, uint64_t newKeyspace, uint64_t newHcKeyspace,
                                 uint64_t incrementMin, uint64_t incrementMax);


    private:

        CSqlLoader * m_sqlLoader;         /**< SqlLoader for database updating */

        /**
         * @section Table attributes of fc_mask
         */

        uint64_t    m_id;
        uint64_t    m_jobId;
        std::string m_mask;
        uint64_t    m_currentIndex;
        uint64_t    m_hcKeyspace;
        uint64_t    m_keyspace;
        uint64_t    m_incrementMin;
        uint64_t    m_incrementMax;

    public:

        /**
         * @section Getters for table attributes
         */

        uint64_t getId() const;
        uint64_t getJobId() const;
        const std::string & getMask() const;
        uint64_t getCurrentIndex() const;
        uint64_t getHcKeyspace() const;
        uint64_t getKeyspace() const;
        uint64_t getIncrementMin() const;
        uint64_t getIncrementMax() const;

        /**
         * @brief Get mask length (number of characters in candidate passwords created from the mask)
         * @return Mask length - '?d?d' has length 2, 'X?d?l' length 3 etc.
         */
        uint64_t getLength() const;

        /**
         * @brief Compare 2 masks
         * @param otherMask [in] Other mask to compare to
         * @param length [in] Length from the start that should be compared
         * @return True if masks are identical in specified range, False otherwise
         */
        bool compare(PtrMask otherMask, uint64_t length) const;
};

#endif //WORKGENERATOR_MASK_H
