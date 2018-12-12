/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef TASKNORMAL_HPP 
#define TASKNORMAL_HPP

#include "TaskComputeBase.hpp"

/** Class representing cracking task */
class TaskNormal: public TaskComputeBase {

    private:

	bool parse_curku_;	    /**< Flag signaling whether to parse CURKU or PROGRESS */

	std::string passwords_;	    /**< Found password/s */

	unsigned long long start_index_;	    /**< Index from which start computation */
	unsigned long long start_computed_index_;   /**< Index from which start computation */

    protected: 

        /**
         * @brief   Getter of the password/s
         * @return  Found password/s
         */
        std::string getPasswords();

	/**
	 * @brief   Parses Hashcat's progress output line
	 * @param   progress_line [in] Line of the hashcat's output
	 */
	void parseHashcatProgress(std::string& progress_line);

	/**
	 * @brief   Reads password/s from the file
	 */
	void readPasswordsFromFile();

	/**
	 * @brief   Parses CURKU value from hashcat progress line
	 * @param   progress_line [] Hashcat's progress line
	 * @return  Parsed CURKU value
	 */
	std::string parseCurku(const std::string& progress_line);

	/**
	 * @brief   Parses PROGRESS value from hashcat progress line
	 * @param   progress_line [] Hashcat's progress line
	 * @return  Parsed PROGRESS value
	 */
	std::string parseProgress(const std::string& progress_line);

	/**
	 * @brief   Saves value from parameter into computed_hashes_
	 * @param   computed_hashes_index [in] Values parsed from Hashcat's
	 *	    progress line
	 */
	void saveParsedProgress(const std::string& computed_hashes_index);

	/**
	 * @brief   Sets total_hashes_ to the second value after PROGRESS
	 * @param   progress_line [] Hashcat's progress line
	 */
	void setTotalHahsesFromProgressLine(const std::string& progress_line);

    public:

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Path to the host based configuration file 
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
         */
        TaskNormal (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name);

        /**
         * @brief   Constructor 
	 * @param   directory [in] Working directory object
         * @param   task_config [in] Task configuration file object
         * @param   host_config [in] Host configuration file object
         * @param   output_file [in] Name of the hashcat output file
         * @param   workunit_name [in] Name of the BOINC project workunit
         */
        TaskNormal (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name);

        /**
         * @brief   Generates task result information
         * @return  Information as string 
         */
        std::string generateOutputMessage(); 

        /**
         * @brief   Sets total_keyspace_ value to 1 since each benchmark task
         *          is only for single hash_type
         */
        void initializeTotalHashes();

	/**
	 * @brief Parse hashcat benchmark line  
	 * @param output_line
	 */
	void parseHashcatBenchmark(std::string output_line);

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

#endif // TASKNORMAL_HPP
