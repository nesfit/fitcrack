/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef TASKBENCHMARK_HPP 
#define TASKBENCHMARK_HPP

#include "TaskComputeBase.hpp"

/** Class representing benchmark task */
class TaskBenchmark: public TaskComputeBase {
    
    protected:

        std::vector<unsigned long long> speeds_;    /**< Storage of speeds for all OCL deviced used by hashcat */

	/**
	 * @brief Parse hashcat benchmark line  
	 * @param output_line
	 */
	void parseHashcatBenchmark(std::string& output_line);

    public:

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Path to the host based configuration file 
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
         */
        TaskBenchmark (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name);

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Host configuration file object
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
	 */
 
        TaskBenchmark (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name);

        /**
         * @brief   Generates task result information
         * @return  Information as string 
         */
        std::string generateOutputMessage(); 

        /**
         * @brief   Computes total speed of node
         * @return  Total speed
         */
        unsigned long long getTotalSpeed();
	
        /**
         * @brief   Sets total_keyspace_ value to 1 since each benchmark task
         *          is only for single hash_type
         */
        void initializeTotalHashes();

        /**
         * @brief   Parses hashcat output line
         * @param   output_line [in] Output line from the hashcat process
         */
        bool parseHashcatOutputLine(std::string& output_line);

	/**
	 * @brief  Handles monitoring of prgress with nofitication
	 */
	void progress();

};

#endif // TASKBENCHMARK_HPP
