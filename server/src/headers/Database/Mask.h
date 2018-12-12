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
        ~CMask() = default;


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
        void updateIndex(uint64_t newIndex);


    private:

        CSqlLoader * m_sqlLoader;         /**< SqlLoader for database updating */

        /**
         * @section Table attributes of fc_mask
         */

        uint64_t    m_id;
        uint64_t    m_packageId;
        std::string m_mask;
        uint64_t    m_currentIndex;
        uint64_t    m_hcKeyspace;

    public:

        /**
         * @section Getters for table attributes
         */

        uint64_t getId() const;
        uint64_t getPackageId() const;
        const std::string & getMask() const;
        uint64_t getCurrentIndex() const;
        uint64_t getHcKeyspace() const;
};

#endif //WORKGENERATOR_MASK_H
