/**
 * @file SqlLoader.h
 * @brief Header file for common MySQL access
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @authors Radek Hranicky (hranicky(at)fit.vut.cz)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_SQLLOADER_H
#define WORKGENERATOR_SQLLOADER_H

#include <Config.h>


class CHost;
class CWorkunit;
class CJob;
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
         * @brief Loads running jobs from database
         * @return Vector of PtrJob instances from fc_job (status >= 10)
         */
        std::vector<Config::Ptr<CJob>> loadRunningJobs();

        /**
         * @brief Loads all active hosts
         * @return All rows in fc_host table
         */
        std::vector<Config::Ptr<CHost>> loadActiveHosts(uint64_t jobId);

        /**
         * Loads all hosts whose job is not running
         * @return Vector of finished hosts
         */
        std::vector<Config::Ptr<CHost>> loadFinishedHosts();

        /**
         * @brief Removes finished/exhausted job hosts from fc_host
         */
        void removeFinishedHosts();

        /**
         * @brief Checks if any running jobs have passed time_end and sets them to finishing state
         */
        void finishTimeoutJobs();

        /**
         * @brief Return number of benchmark entries for job-host combo
         * @param jobId [in] Job parent of this workunit
         * @param hostId [in] Benchmark owner
         * @return Number of benchmarks, usually 0 if no benchmark is present, 1 otherwise
         */
        uint64_t getBenchCount(uint64_t jobId, uint64_t hostId);

        /**
         * @brief Return number of unfinished workunits in fc_workunit for supplied job-host combo
         * @param jobId Job ID used to filtering
         * @param hostId Host ID used for filtering
         * @return Number of workunit entries
         */
        uint64_t getWorkunitCount(uint64_t jobId, uint64_t hostId);

        /**
         * @brief Get the number of workunits created for this host-job combo. Counts @em all workunits, regardless of status
         * 
         * @param jobId 
         * @param hostId 
         * @return uint64_t 
         */
        uint64_t getTotalWorkunitCount(uint64_t jobId, uint64_t hostId);

        /**
         * @brief Return number of workunits in fc_workunit for supplied job
         * @param jobId Job ID used for filtering
         * @return Number of workunit entries
         */
        uint64_t getWorkunitCount(uint64_t jobId);

        /**
         * @brief Sets database entry of workunit with supplied ID to finished == 1
         * @param id [in] ID of workunit to modify
         */
        void setWorkunitFinished(uint64_t id);

        /**
         * @brief Gets workunit from database with supplied jobId and retry set to 1
         * @param jobId ID of job of the workunit we are looking for
         * @return Pointer to workunit, created from database entry
         */
        Config::Ptr<CWorkunit> getEasiestRetry(uint64_t jobId);

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
         * @brief Creates new fc_mask entry
         * @param jobId [in] ID of fc_job entry
         * @param newMask [in] New mask string
         * @param newKeyspace [in] New real keyspace
         * @param newHcKeyspace [in] New hashcat keyspace
         * @param incrementMin [in] New increment min size
         * @param incrementMax [in] New increment max size
         * @return ID of the new fc_mask entry
         */
        uint64_t createMask(uint64_t jobId, std::string newMask, uint64_t newKeyspace, uint64_t newHcKeyspace, uint64_t incrementMin, uint64_t incrementMax);

        /**
         * @brief Flag fc_mask entry as merged
         * @param maskID [in] ID of fc_mask entry
         */
        void maskSetMerged(uint64_t maskId);

        /**
         * @brief Get current mask merge setting
         * @return True if mask merging is enabled, False otherwise
         */
        bool getEnableMergeMasks();

        /**
         * @brief Updates current_index of fc_job_dictionary entry
         * @param dictId [in] ID of job_dictionary entry
         * @param newIndex [in] New index value
         */
        void updateDictionaryIndex(uint64_t dictId, uint64_t newIndex);

        /**
         * @brief Updates current_pos (file pos) of fc_job_dictionary entry
         * @param dictId [in] ID of job_dictionary entry
         * @param newPos [in] New position in dictionary file
         */
        void updateDictionaryPos(uint64_t dictId, uint64_t newPos);

        /**
         * @brief Updates current_index of fc_job entry
         * @param jobId [in] ID of the job entry
         * @param newIndex [in] New index value
         */
        void updateJobIndex(uint64_t jobId, uint64_t newIndex);

        /**
         * @brief Updates current_index_2 of fc_job entry
         * @param jobId [in] ID of job entry
         * @param newIndex2 [in] New index value
         */
        void updateJobIndex2(uint64_t jobId, uint64_t newIndex2);

        /**
         * @brief Update status of fc_pacakge entry
         * @param jobId [in] ID of job entry
         * @param newStatus [in] New status value
         */
        void updateJobStatus(uint64_t jobId, uint32_t newStatus);

        /**
         * @brief Update status of RUNNING fc_job entry, otherwise, does nothing
         * @param jobId [in] ID of job entry
         * @param newStatus [in] New status value
         * @note This is used for SQL atomic swap, so stopped job won't be switched to >= 10 by mistake
         */
        void updateRunningJobStatus(uint64_t jobId, uint32_t newStatus);

        /**
         * @brief Returns if job with supplied ID has passed time_end
         * @param jobId ID of job to check
         * @return True if now() is greater than time_end of supplied job, False otherwise
         */
        bool isJobTimeout(uint64_t jobId);

        /**
         * @brief Inserts new workunit to fc_workunit table
         * @param workunit Pointer to workunit appended to database
         */
        void addNewWorkunit(Config::Ptr<CWorkunit> workunit);

        /**
         * @brief Reads default_workunit_timeout_factor from fc_settings
         * @return Number from DB
         */
        unsigned int getTimeoutFactor();

        /**
         * @brief Reads hwmon_temp_abort from fc_settings
         * @return Number from DB
         */
        unsigned int getHWTempAbort();

        /// Gets the distribution coefficient from the database
        double getDistributionCoefficient();

        /// Gets the setting for the absolute minimum time a workunit should take
        unsigned getAbsoluteMinimumWorkunitSeconds();

        /// returns whether there should be a ramp up of WU time
        bool getEnableRampUp();

        /// Gets the ramp-down coefficient from the database
        double getRampDownCoefficient();

        /**
         * Returns fresh host status from DB
         * @param host_id ID of the host
         * @return status column form fc_host
         */
        uint32_t getHostStatus(uint64_t host_id);

        /**
         * @brief Set time_start of job with supplied jobId to now(), only if it is currently NULL
         * Otherwise does nothing
         * @param jobId Job ID to which set time_start
         */
        void updateStartTimeNow(uint64_t jobId);

        /**
         * @brief Set time_end of job with supplied jobId to now()
         * @param jobId Job ID to which set time_end
         */
        void updateEndTimeNow(uint64_t jobId);

        /**
         * @brief Returns vector of masks which are not exhausted and belongs to supplied job
         * @param jobId Job ID which masks we search for
         * @return Vector of smart pointers to found masks
         */
        std::vector<Config::Ptr<CMask>> loadJobMasks(uint64_t jobId);

        /**
         * @brief Returns vector of masks which are not exhausted (taking normal keyspace) and belongs to supplied job
         * @param jobId Job ID which masks we search for
         * @return Vector of smart pointers to found masks
         */
        std::vector<Config::Ptr<CMask>> loadJobMasksWithNormalKeyspace(uint64_t jobId);

        /**
         * @brief Returns vector of dictionaries which are not exhausted and belongs to supplied job
         * @param jobId Job ID which dictionaries we search for
         * @return Vector of smart pointers to found dictionaries
         */
        std::vector<Config::Ptr<CDictionary>> loadJobDictionaries(uint64_t jobId);


        /**
         * @brief Returns ID of the hashlist associated with the job
         * @param jobId Job ID
         * @return Hashlist ID
         */
        uint64_t getHashlistId(uint64_t jobId);

        /**
         * @brief Returns vector of hashes as strings, possibly even binary ones
         * @param jobId Job ID which hashes we search for
         * @return Vector of strings=hashes
         */
        std::vector<std::string> loadJobHashes(uint64_t jobId);

        /**
         * @brief Loads mask object from database with supplied ID
         * @param maskId ID of the mask in DB
         * @return Loaded mask object
         */
        Config::Ptr<CMask> loadMask(uint64_t maskId);

        /**
         * @brief Loads dictionary object from database with supplied ID
         * @param dictId ID of the dictionary in fc_job_dictionary table
         * @return Loaded dictionary object
         */
        Config::Ptr<CDictionary> loadDictionary(uint64_t dictId);

        /**
         * @brief Loads job object from database with supplied ID
         * @param jobId ID of the job in DB
         * @return Loaded job object
         */
        Config::Ptr<CJob> loadJob(uint64_t jobId);

        /**
         * @brief Sets grammar object from database with supplied ID
         * @param grammarId ID of the grammar in DB
         * @return True if grammar was set
         */
        std::string loadGrammarName(uint64_t grammarId);

        /**
         * @brief Returns number of seconds between time_start column and now()
         * @param jobId ID of job from fc_job
         * @return Number of seconds
         */
        uint64_t getSecondsPassed(uint64_t jobId);

        /**
         * @brief Computes sum of power of hosts working on supplied job
         * @param jobId ID of job we work with
         * @return Sum of host power
         */
        uint64_t getTotalPower(uint64_t jobId);

        /**
         * @brief Returns name of dictionary with supplied ID
         * @param dictId [in] ID of dictionary in fc_dictionary
         * @return Path (filename) of dictionary
         */
        std::string getDictFileName(uint64_t dictId);

        /**
         * @brief Checks if any job hashes are cracked
         * @param jobId ID of the job
         * @return True if any hashes with not-null result found, False otherwise
         */
        bool isAnythingCracked(uint64_t jobId);

        /**
        * Stops job, cancels and finishes all workunits
        * @param job Pointer to the job
        */
        void killJob(Config::Ptr<CJob> &job);

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
        void addNewHosts(uint64_t jobId);

        /**
         * Returns a string from database select
         * @param query [in] An SQL query to execute
         * @return A string from select
         */
        std::string getSqlString(const std::string & query);

        /**
         * @brief Get a result of the given type from the database.
         * 
         * @tparam Res 
         * @param query 
         * @return Res 
         */
        template <typename Res>
        Res getSqlConverted(const std::string &query, Res(*conversionFn)(const std::string&));

        /**
         * Returns a number from database select
         * @param query [in] An SQL query to execute
         * @return Number from select
         */
        uint64_t getSqlNumber(const std::string & query);

        ///Returns a floating point number from a database query
        double getSqlDouble(const std::string & query);

        /**
         * @brief This is template function for loading different objects from database
         * @tparam T This can be CHost, CWorkunit, CMask, CJob and CDictionary
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

                /** Create workunit object */
                result.push_back(T::create(dbMap, this));
            }

            mysql_free_result(sqlResult);

            return result;
        }

        /**
         * @brief Similar to load function but supports more difficult queries
         * which qrequire JOINS or selecting non-* columns
         * @tparam T This can be CHost, CWorkunit, CMask, CJob and CDictionary
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

                /** Create workunit object */
                result.push_back(T::create(dbMap, this));
            }

            mysql_free_result(sqlResult);

            return result;
        }

        /**
         * This is template function for loading row count from database
         * @tparam T This can be CHost, CWorkunit, CMask and CPakckage
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
