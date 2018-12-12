/**
 * @file Host.h
 * @brief Header file for fc_host entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_HOST_H
#define WORKGENERATOR_HOST_H

#include <Config.h>


class CSqlLoader;
class CHost;
using PtrHost = Config::Ptr<CHost>;


class CHost {
    private:
        /**
         * @brief Private constructor, called by create() static function
         * @param hostMap [in] Map representing fc_host table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         */
        explicit CHost(DbMap & hostMap, CSqlLoader * sqlLoader);

    public:
        /** Default destructor */
        ~CHost() = default;

        /**
         * @brief Creating instance as a shared pointer from database select
         * @param hostMap [in] Map representing fc_host table entry
         * @param sqlLoader [in] Pointer to SqlLoader for database updates
         * @return Shared pointer to fc_host entry object
         */
        static PtrHost create(DbMap & hostMap, CSqlLoader * sqlLoader);

        /**
         * @brief Get the host SQL table name, usually fc_host
         * @return Host SQL table name
         */
        static std::string getTableName();

        /**
         * @brief Updates member variable and fc_host table
         * @param newStatus [in] New status for this host
         */
        void updateStatus(Config::HostState newStatus);

    private:

        CSqlLoader * m_sqlLoader;       /**< SqlLoader for database updating */

        /**
         * @section Table attributes of fc_host
         */

        uint64_t m_id;
        uint64_t m_boincHostId;
        uint64_t m_power;
        uint64_t m_packageId;
        uint32_t m_status;

    public:

        /**
         * @section Getters for table attributes
         */

        uint64_t getId() const;
        uint64_t getBoincHostId() const;
        uint64_t getPower() const;
        uint64_t getPackageId() const;
        uint32_t getStatus() const;
};

#endif //WORKGENERATOR_HOST_H
