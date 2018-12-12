/**
 * @file SqlLoader.h
 * @brief Header file for common MySQL access
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_SQLLOADER_H
#define WORKGENERATOR_SQLLOADER_H

#include <Config.h>


class CHost;
class CJob;
class CPackage;
class CMask;
class CDictionary;


class CSqlLoader {
    public:

        /**
         * @brief Constructor
         */
        CSqlLoader();

        /**
         * @brief Destructor
         */
        ~CSqlLoader();

        /**
         * @brief Loads running packages from database
         * @return Vector of PtrPackage instances from fc_package (status >= 10)
         */
        std::vector<Config::Ptr<CPackage>> loadRunningPackages();

        /**
         * @brief Loads all active hosts
         * @return All rows in fc_host table
         */
        std::vector<Config::Ptr<CHost>> loadActiveHosts(uint64_t packageId);

        /**
         * Loads all hosts which package is not running
         * @return Vector of finished hosts
         */
        std::vector<Config::Ptr<CHost>> loadFinishedHosts();

        /**
         * @brief Removes finished/exhausted package hosts from fc_host
         */
        void removeFinishedHosts();

        /**
         * @brief Checks if any running packages have passed time_end and sets them to finishing state
         */
        void finishTimeoutPackages();

        /**
         * @brief Return number of benchmark entries for package-host combo
         * @param packageId [in] Package parent of this job
         * @param hostId [in] Benchmark owner
         * @return Number of benchmarks, usually 0 if no benchmark is present, 1 otherwise
         */
        uint64_t getBenchCount(uint64_t packageId, uint64_t hostId);

        /**
         * @brief Return number of jobs in fc_job for supplied package-host combo
         * @param packageId Package ID used to filtering
         * @param hostId Host ID used for filtering
         * @return Number of job entries
         */
        uint64_t getJobCount(uint64_t packageId, uint64_t hostId);

        /**
         * @brief Return number of jobs in fc_job for supplied package
         * @param packageId Package ID used for filtering
         * @return Number of job entries
         */
        uint64_t getJobCount(uint64_t packageId);

        /**
         * @brief Sets database entry of job with supplied ID to finished == 1
         * @param id [in] ID of job to modify
         */
        void setJobFinished(uint64_t id);

        /**
         * @brief Gets job from database with supplied packageId and retry set to 1
         * @param packageId ID of package of the job we are looking for
         * @return Pointer to job, created from database entry
         */
        Config::Ptr<CJob> getEasiestRetry(uint64_t packageId);

        /**
         * @brief Updates status of a host in fc_host table
         * @param hostId [in] Host ID which status we are updating
         * @param newStatus [in] New status of this host
         */
        void updateHostStatus(uint64_t hostId, uint32_t newStatus);

        /**
         * @brief Updates current_index of fc_mask entry
         * @param maskId [in] ID of mask entry
         * @param newIndex [in] New index value
         */
        void updateMaskIndex(uint64_t maskId, uint64_t newIndex);

        /**
         * @brief Updates current_index of fc_package_dictionary entry
         * @param dictId [in] ID of package_dictionary entry
         * @param newIndex [in] New index value
         */
        void updateDictionaryIndex(uint64_t dictId, uint64_t newIndex);

        /**
         * @brief Updates current_index of fc_package entry
         * @param packageId [in] ID of the package entry
         * @param newIndex [in] New index value
         */
        void updatePackageIndex(uint64_t packageId, uint64_t newIndex);

        /**
         * @brief Updates current_index_2 of fc_package entry
         * @param packageId [in] ID of package entry
         * @param newIndex2 [in] New index value
         */
        void updatePackageIndex2(uint64_t packageId, uint64_t newIndex2);

        /**
         * @brief Update status of fc_pacakge entry
         * @param packageId [in] ID of package entry
         * @param newStatus [in] New status value
         */
        void updatePackageStatus(uint64_t packageId, uint32_t newStatus);

        /**
         * @brief Update status of RUNNING fc_package entry, otherwise, does nothing
         * @param packageId [in] ID of package entry
         * @param newStatus [in] New status value
         * @note This is used for SQL atomic swap, so stopped package won't be switched to >= 10 by mistake
         */
        void updateRunningPackageStatus(uint64_t packageId, uint32_t newStatus);

        /**
         * @brief Returns if package with supplied ID has passed time_end
         * @param packageId ID of package to check
         * @return True if now() is greater than time_end of supplied package, False otherwise
         */
        bool isPackageTimeout(uint64_t packageId);

        /**
         * @brief Inserts new job to fc_job table
         * @param job Pointer to job appended to database
         */
        void addNewWorkunit(Config::Ptr<CJob> job);

        /**
         * @brief Reads default_job_timeout_factor from fc_settings
         * @return Number from DB
         */
        unsigned int getTimeoutFactor();

        /**
         * Returns fresh host status from DB
         * @param host_id ID of the host
         * @param package_id ID of the package
         * @return status column form fc_host
         */
        uint32_t getHostStatus(uint64_t host_id);

        /**
         * @brief Set time_start of package with supplied packageId to now(), only if it is currently NULL
         * Otherwise does nothing
         * @param packageId Package ID to which set time_start
         */
        void updateStartTimeNow(uint64_t packageId);

        /**
         * @brief Set time_end of package with supplied packageId to now()
         * @param packageId Package ID to which set time_end
         */
        void updateEndTimeNow(uint64_t packageId);

        /**
         * @brief Returns vector of masks which are not exhausted and belongs to supplied package
         * @param packageId Package ID which masks we search for
         * @return Vector of smart pointers to found masks
         */
        std::vector<Config::Ptr<CMask>> loadPackageMasks(uint64_t packageId);

        /**
         * @brief Returns vector of dictionaries which are not exhausted and belongs to supplied package
         * @param packageId Package ID which dictionaries we search for
         * @return Vector of smart pointers to found dictionaries
         */
        std::vector<Config::Ptr<CDictionary>> loadPackageDictionaries(uint64_t packageId);

        /**
         * @brief Returns vector of hashes as strings, possibly even binary ones
         * @param packageId Package ID which hashes we search for
         * @return Vector of strings=hashes
         */
        std::vector<std::string> loadPackageHashes(uint64_t packageId);

        /**
         * @brief Loads mask object from database with supplied ID
         * @param maskId ID of the mask in DB
         * @return Loaded mask object
         */
        Config::Ptr<CMask> loadMask(uint64_t maskId);

        /**
         * @brief Loads dictionary object from database with supplied ID
         * @param dictId ID of the dictionary in fc_package_dictionary table
         * @return Loaded dictionary object
         */
        Config::Ptr<CDictionary> loadDictionary(uint64_t dictId);

        /**
         * @brief Loads package object from database with supplied ID
         * @param packageId ID of the package in DB
         * @return Loaded package object
         */
        Config::Ptr<CPackage> loadPackage(uint64_t packageId);

        /**
         * @brief Returns number of seconds between time_start column and now()
         * @param packageId ID of package from fc_package
         * @return Number of seconds
         */
        uint64_t getSecondsPassed(uint64_t packageId);

        /**
         * @brief Computes sum of power of hosts working on supplied package
         * @param packageId ID of package we work with
         * @return Sum of host power
         */
        uint64_t getTotalPower(uint64_t packageId);

        /**
         * @brief Returns name of dictionary with supplied ID
         * @param dictId [in] ID of dictionary in fc_dictionary
         * @return Path (filename) of dictionary
         */
        std::string getDictFileName(uint64_t dictId);

        /**
         * @brief Checks if any package hashes are cracked
         * @param packageId ID of the package
         * @return True if any hashes with not-null result found, False otherwise
         */
        bool isAnythingCracked(uint64_t packageId);

private:

        /**
         * @brief Updates SQL database with supplied query
         * @param query [in] SQL query to execute
         */
        void updateSql(const char * query);

        /**
         * @brief Converts formatted input to a single string
         * @param format [in] Formatted strings
         * @param ... [in] Optional variable length arguments
         * @return Single string created from formatted input
         */
        char * formatQuery(const char* format, ...);

        /**
         * @brief Adds missing active hosts to fc_host
         */
        void addNewHosts(uint64_t packageId);

        /**
         * Returns number from database select
         * @param query [in] An SQL query to execute
         * @return Number from select
         */
        uint64_t getSqlNumber(const std::string & query);

        /**
         * @brief This is template function for loading different objects from database
         * @tparam T This can be CHost, CJob, CMask, CPackage and CDictionary
         * @param filter [in] An SQL filter which follows select
         * @return Vector of smart pointers to instantiated objects
         * @note Implementation must be in header file, see https://stackoverflow.com/a/8752879
         */
        template <typename T>
        std::vector<std::shared_ptr<T>> load(std::string filter)
        {
            std::string tableName = T::getTableName();
            std::vector<std::shared_ptr<T>> result;

            updateSql(formatQuery("SELECT * FROM `%s` %s ;", tableName.c_str(), filter.c_str()));

            MYSQL_RES* sqlResult;
            sqlResult = mysql_store_result(boinc_db.mysql);
            if (!sqlResult)
            {
                Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.\n");
                boinc_db.close();
                exit(1);
            }

            /** Get DB scheme */
            MYSQL_FIELD *field;
            std::vector<std::string> tableScheme;
            while((field = mysql_fetch_field(sqlResult)))
            {
                tableScheme.emplace_back(field->name);
            }

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(sqlResult)))
            {
                /** Make sure black magic does not happen */
                unsigned int col_num = mysql_num_fields(sqlResult);
                if (col_num != tableScheme.size())
                {
                    Tools::printDebug("ERROR: Table scheme size doesn't fit!\n");
                    return result;
                }

                /** Create the database objects map */
                DbMap dbMap;

                for(unsigned int i = 0; i < col_num; i++)
                {
                    if (row[i] == nullptr)
                        dbMap.insert(std::make_pair(tableScheme[i], ""));
                    else
                        dbMap.insert(std::make_pair(tableScheme[i], row[i]));
                }

                /** Debug output */
                //Tools::printDbMap(dbMap);

                /** Create job object */
                result.push_back(T::create(dbMap, this));
            }

            mysql_free_result(sqlResult);

            return result;
        }

        /**
         * @brief Similar to load function but supports more difficult queries
         * which qrequire JOINS or selecting non-* columns
         * @tparam T This can be CHost, CJob, CMask, CPackage and CDictionary
         * @param filter [in] A complete SQL query
         * @return Vector of smart pointers to instantiated objects
         * @note Implementation must be in header file, see https://stackoverflow.com/a/8752879
         */
        template <typename T>
        std::vector<std::shared_ptr<T>> customLoad(std::string query)
        {
            std::vector<std::shared_ptr<T>> result;
            updateSql(query.c_str());

            MYSQL_RES* sqlResult;
            sqlResult = mysql_store_result(boinc_db.mysql);
            if (!sqlResult)
            {
                Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.\n");
                boinc_db.close();
                exit(1);
            }

            /** Get DB scheme */
            MYSQL_FIELD *field;
            std::vector<std::string> tableScheme;
            while((field = mysql_fetch_field(sqlResult)))
            {
                tableScheme.emplace_back(field->name);
            }

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(sqlResult)))
            {
                /** Make sure black magic does not happen */
                unsigned int col_num = mysql_num_fields(sqlResult);
                if (col_num != tableScheme.size())
                {
                    Tools::printDebug("ERROR: Table scheme size doesn't fit!\n");
                    return result;
                }

                /** Create the database objects map */
                DbMap dbMap;

                for(unsigned int i = 0; i < col_num; i++)
                {
                    if (row[i] == nullptr)
                        dbMap.insert(std::make_pair(tableScheme[i], ""));
                    else
                        dbMap.insert(std::make_pair(tableScheme[i], row[i]));
                }

                /** Debug output */
                //Tools::printDbMap(dbMap);

                /** Create job object */
                result.push_back(T::create(dbMap, this));
            }

            mysql_free_result(sqlResult);

            return result;
        }

        /**
         * This is template function for loading row count from database
         * @tparam T This can be CHost, CJob, CMask and CPakckage
         * @param filter [in] An SQL filter which follows select
         * @return Number from result select in database
         * @note Implementation must be in header file, see https://stackoverflow.com/a/8752879
         */
        template <typename T>
        uint64_t getRowCount(std::string filter)
        {
            uint64_t result = 0;
            std::string tableName = T::getTableName();

            updateSql(formatQuery("SELECT COUNT(*) FROM `%s` %s ;\n", tableName.c_str(), filter.c_str()));

            MYSQL_RES* sqlResult;
            sqlResult = mysql_store_result(boinc_db.mysql);
            if (!sqlResult)
            {
                Tools::printDebugTimestamp("Problem with DB query.\nShutting down now.");
                boinc_db.close();
                exit(1);
            }

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(sqlResult)))
            {
                try
                {
                    result = std::stoull(row[0]);
                }
                catch (std::logic_error & error)
                {
                    Tools::printDebugTimestamp("Error converting row count to uint64_t: %s\n", error.what());
                    continue;
                }
            }

            mysql_free_result(sqlResult);
            return result;
        }

    private:

        char * m_sqlBuffer;     /**< Buffer for SQL queries */
};

#endif //WORKGENERATOR_SQLLOADER_H
