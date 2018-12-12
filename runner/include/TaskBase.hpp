/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef TASKBASE_HPP 
#define TASKBASE_HPP


#include "ConfigHost.hpp"
#include "ConfigTask.hpp"

#include "Constants.hpp"

#include "Directory.hpp"

#include "File.hpp"

#include "RunnerUtils.hpp"

#include <cmath>

#include <string.h>

#include "app_ipc.h"
#include "boinc_api.h"


/** Class representing task created by the server */
class TaskBase {

    protected:

        ConfigTask task_config_;         /**< Representant of task config file received from server */

        ConfigHost host_config_;    /**< Representant of host based config file set by host administrator */
        
        Directory directory_;       /**< Representatn of directory where is program executed from */

        int exit_code_;             /**< Exit code of the task, DIFFERS from the process exit code */

        unsigned long long computed_hashes_;    /**< Already computed hashes */
        unsigned long long total_hashes_;     /**< Total number or hashes to compute in the task */ 

        std::string mode_;           /**< Mode of the task a-benchmark_all, b-benchmark, n-normal */
        std::string output_file_;   /**< Name of the hashcat output file */
	std::string workunit_name_; /**< Name of the workunit assigned by boinc server */

	/**
         * @brief   Actualizes member variables represeting progress
	 * @param   newly_computed_hashes [in] Number of computed hashes since
	 *	    last actualization 
	 */
        void actualizeComputedHashes(const unsigned long long newly_computed_hashes);

        /**
         * @brief   Computes fraction done 
         * @return  <0, 1> decimal number represeting percentages done (0 - 100%)
         */
        double fractionDone();

	/**
         * @brief   Generates task result information
         * @return  Information as string 
         */
        virtual std::string generateOutputMessage() = 0;

    public:

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Path to the host based configuration file 
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
         */
        TaskBase(Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name);

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Host configuration file object
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
         */
        TaskBase(Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name);

        /**
         * @brief   Reads hashcat output file 
         * @return  Exit code of the process
         */
        virtual int finish() = 0;

        /**
         * @brief   Reads hashcat output file and passes information to server
         * @return  Exit code of the process
         */
        int saveAndFinish();
        
        /**
         * @brief   Writes result information into output file
         * @param   output_message [in] Output file as stream
         */
        void writeOutputFile(std::string& output_message);

        /**
         * @brief   Pure virtual function for Initializing attack and process
         */
        virtual void initialize() = 0;

        /**
         * @brief   Pure virtual function setting total_keyspace_ value base on ConfigTask object
         */
        virtual void initializeTotalHashes() = 0;

	/**
	 * @brief   Pure virtual function handling progress notifications
	 */
	virtual void progress() = 0;
        
        /**
         * @brief   Reports current progress to boinc-client and project server
         * @return  Progress of the task as percentage
         */
        void reportProgress();

	/**
	 * @brief   Spawns the computation process
	 */
	virtual void startComputation() = 0;

	/**
	 * @brief   Saves generated result message into the result file
	 */
	void saveResult();

};

#endif // TASKBASE_HPP
