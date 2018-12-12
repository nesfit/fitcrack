/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef CONFIGTASK_HPP
#define CONFIGTASK_HPP

#include "File.hpp"

#include "RunnerUtils.hpp"

#include <map>

/** Class representing config file of the task in TLV format */
class ConfigTask {

    private:

        std::map<std::string, std::string> options_;    /**< Parsed config options as key => value */

        std::string path_;                              /**< Path to the config file */

        std::vector<std::string> supported_;            /**< Set of supported config options */

	/**
	 * @brief   Converts the config line to the option pair
	 * @param   line [in] parsed line of config
	 */
	void convertLineToOption(std::string& line);

        /**
         * @brief   Sets supported_ to contain all supported options by the
         *          runner
         */
        void initSupported();

        /**
         * @brief   Checks whether the key is in the vector of supported config
         *          options
         * @param   key [in] Selector of the option
         * @return  True when key is in vector, False otherwise 
         */
        bool isSupported(const std::string& key);

        /**
         * @brief   Parses all the fields from the line of config
         * @param   line [in] Line to be parsed
         * @param   fields [out] Parsed fields from the line
         */
        void parseLine(const std::string& line, std::vector<std::string>& fields);

        /**
         * @brief   Takes the fields, calls verification and then saves the
         *          important fields to options_
         * @param   fields [in] Parsed fields from the line
         */
        void saveFieldsToOptions(std::vector<std::string>& fields);

        /**
         * @brief   Verifies if key is supported and length and types values match
         * @param   fields [in] Parsed fields from the line
         * @return  True when verification passes, False otherwise 
         */
        bool verifyFields(std::vector<std::string>& fields);

    public:

	/**
	 * @brief  Basic constructor 
	 */
        ConfigTask();

        /**
         * @brief   Constructor that saves config_path and opens fstream on it
         * @param   config_path [in] Path to config file
         */
        ConfigTask(std::string config_path);

	/**
	 * @brief   Add new parameter to the collection
	 * @param   key [in] Key to be added
	 * @param   values [in] Value saved under the key
	 */
	void add(const std::string& key, const std::string& values);

        /**
         * @brief   Wrapper arround std::map::find()
         * @param   key [in] Selector of associative array
         * @param   value [out] Value of element selected of selector
         * @return  True when found, False otherwise
         */
        bool find(const std::string& key, std::string& value) const;

        /**
         * @brief   Wrapper arround std::map::find()
         * @param   key [in] Selector of associative array
         * @return  Found value 
         */
        std::string find(const std::string& key) const;

        /** 
         * @brief   Checks if options_ is empty 
         * @return  True when empty, False otherwise
         */
        bool isEmpty() const;

        /**
         * @brief   Parses TVL dictionary file and fills options_
         */
        void parse();

        /**
         * @brief   Prints the pairs from the options key <= value
         */
        void print();

        /**
         * @brief   Setter for path to TLV file out of constructor and open
         *          std::istream using provided path
         * @param   config_path [in] Path to the config file
         */
        void setPath(const std::string& config_path);

        /**
         * @brief   Getter of path to TLV file
         * @return  Path to config file as string 
         */
        std::string getPath() const;
};

#endif // CONFIGTASK_HPP
