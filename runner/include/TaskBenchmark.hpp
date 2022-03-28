/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef TASKBENCHMARK_HPP 
#define TASKBENCHMARK_HPP

#include "TaskComputeBase.hpp"
#include "json.hpp"
#include <set>

/** Class representing benchmark task */
class TaskBenchmark: public TaskComputeBase {
	//! No copying
	TaskBenchmark(const TaskBenchmark&);
protected:
	std::set<uint64_t> benchmarked_speeds_;

	uint64_t salt_count_;

	/**
	 * @brief Parse hashcat benchmark line  
	 * @param output_line
	 */
	bool parseHashcatBenchmark(std::string& output_line);

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

	uint64_t getBenchmarkedSpeed();

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
