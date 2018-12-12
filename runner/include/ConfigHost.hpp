/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */


#ifndef CONFIGHOST_HPP
#define CONFIGHOST_HPP

#include "File.hpp"

#include "RunnerUtils.hpp"

/** Class representing host configuration file */
class ConfigHost {

    private:

        std::string path_;      /**< Path to the config file */
		std::string content_;   /**< Content of the host config file (only 1 line expected)*/

    public:

        /**
         * @brief   Constructor that saves config_path and opens fstream on it
         * @param   config_path [in] Path to config file
         */
        ConfigHost(std::string config_path);

        /**
         * @brief   Constructor that saves config_path and opens fstream on it
         * @param   config_path [in] Path to config file
         */
        ConfigHost(std::string& config_path);

	/**
	 * @brief   Getter of content_
	 */
	std::string getContent() const;

        /**
         * @brief   Getter of path to host configuration file
         * @return  Path to host configuration file as string 
         */
        std::string getPath() const;

        /**
         * @brief   Parses the content_ to separated arguments of hashcat
         * @param   arguments [out] Collection of hashcat arguments
         */
        void parseArguments(std::vector<char*>& arguments);


        /**
         * @brief   Prints content of the host configuration file
         */
        void print();

        /**
         * @brief   Reads host configuration file and save it to
         *          content_
         */
        void read();


        /**
         * @brief   Setter for path to host configuration file out of 
         *          constructor and also opens std::istream using provided path
         * @param   config_path [in] Path to the config file
         */
        void setPath(const std::string& config_path);
};

#endif // CONFIGHOST_HPP

