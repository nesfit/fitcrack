/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef NODEBASE_HPP 
#define NODEBASE_HPP

#include "Logging.hpp"
#include "RunnerUtils.hpp"

#include <string>

#include <limits.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
 #define stat _stat
 #define S_IFDIR _S_IFDIR 
 #define S_IFREG _S_IFREG 
 #define PATH_DELIMITER "\\"
#else
 #define PATH_DELIMITER "/"
#endif // _WIN32



/** Class representing common layer for directories and files as nodes in filesystem*/
class FilesysBase {
    private:
	/**
	 * @brief   Initializes base object, is called by constructor
	 */
	void initialize();

        /**
         * @brief   Removes "./" from absolute path
         * @param   absolute_path [in/out] Absolute path
         */
        void simplifyPath(std::string& absolute_path);

    protected:
	
	/* Constants */
	std::string current_directory_;	/**< Current directory symbol in relative path */
	std::string parent_directory_;	/**< Parent directory symbol in relative path */
	std::string path_delimiter_;	/**< Path delimiter */
	/******************************************************************************/

        std::string absolute_path_; /**< Absolute path to the node, if resolved path if softlink */
        std::string name_;          /**< Name of the node */
        std::string relative_path_; /**< Relative path of the node, might be same as name and as absolute, may be path to softlink */

	/**
	 * @brief   Initializes paths
	 */
	virtual void init();

        /**
         * @brief   Parses file name from the path
         * @param   path [in] Path to the file
         */
        std::string parseName(const std::string& path);

    public:

	/**
	 * @brief   Basic constructor
	 */
	FilesysBase();

	/**
	 * @brief   Constructor with path specified
	 * @param   path [in] Path to the filesystem node
	 */
	FilesysBase(std::string path);

	/**
	 * @brief   Contructor with path and alias name specified
	 * @param   path [in] Path to the filesystem node
	 * @param   name [in] Alias name of the node
	 */
	FilesysBase(std::string path, std::string name);

        /**
         * @brief   Changes the node it represents
         * @param   relative_path [in] Relative path to new file or symlink
         */
        void changeTo(const std::string& relative_path);

        /**
         * @brief   Getter of the name_
         * @return  Name of the node 
         */
        std::string getName() const;

        /**
         * @brief   Getter of the absolute_path_
         * @return  Absolute path to the directory
         */
        std::string getAbsolutePath() const;

        /**
         * @brief   Getter of the relative_path_
         * @return  Relative path to the directory
         */
        std::string getRelativePath() const;

	/**
	 * @brief   TODO
	 * @param path
	 * @return  
	 */
	static bool isDirectory(const std::string& path);

	/**
	 * @brief   TODO
	 * @param path
	 * @return  
	 */
	static bool isFile(const std::string& path);

	/**
	 * @brief   Unifies path delimiters
	 * @param   path [in/out] Path to be normalized
	 */
	static void normalizePath(std::string& path);

        /**
         * @brief   Converts relative path to absolute path
         * @param   relative_path [in] Relative path to the file
         * @param   absolute_path [out] Absolute path to the file
         */
        void relativeToAbsolute(const std::string& relative_path, std::string& absolute_path);

        /**
         * @brief   Sets name_ out of the path_
         */
        void setName();

        /**
         * @brief   Sets name_ by parsing directory name from the path
         * @param   path [in] Path to the directory
         */
        void setName(const std::string& path);

	/**
	 * @brief   TODO
	 * @param path
	 */
	void setRelativePath(std::string& path);

};

#endif // NODEBASE_HPP 
