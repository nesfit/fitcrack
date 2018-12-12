/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef TASKBENCHMARKALL_HPP 
#define TASKBENCHMARKALL_HPP

#include "TaskBase.hpp"
#include "TaskBenchmark.hpp"

/** Class representing benchmark all class, runs multiple TaskBenchmark*/
class TaskBenchmarkAll: public TaskBase {
    private:

	std::vector<TaskBenchmark> benchmark_tasks_; /**< Collection with prepared benchmark task for all supported hash_types */
	std::vector<std::string> hashtypes_; /**< Collection of hashtypes indexes correspond with benchmark_tasks_ */

    public:

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Path to the host based configuration file 
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
         */
        TaskBenchmarkAll(Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name);

	/**
	 * @brief   Generate task mode based information
	 * @return  Output message 
	 */
	std::string generateOutputMessage();

        /**
         * @brief   Initializes hashtypes and benchmark_task_ collection
         */
        void initialize();

        /**
         * @brief   Sets total_keyspace_ value base on ConfigTask object
         */
        void initializeTotalHashes();

	/**
	 * @brief   Finishes the task by finishing all benchmark tasks
	 * @return  Task exit code
	 */
	int finish();

	/**
	 * @brief   Runs startComputation() on whole benchmark_tasks_ collection
	 *	    and reports progress
	 */
	void startComputation();

	/**
	 * @brief   Does nothing
	 */
	void progress();

	/**
	 * @brief   Prints all hashtypes read from the file
	 */
	void printHashTypes();
};

#endif // TASKBENCHMARKALL_HPP
