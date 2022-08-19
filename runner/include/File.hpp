/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef FILE_HPP 
#define FILE_HPP

#include "FilesysBase.hpp"
#include "Logging.hpp"
#include "RunnerUtils.hpp"

#include <cerrno>

#include <fstream>
#include <iomanip>
#include <iostream>


/** Class represeting file on file-system, by symlinks are meant BOINC symlinks */
class File : public FilesysBase {
    
    private:

        std::string extension_;	    /**< Extension of the file */

        /**
         * @brief   Initializes object based on relative_path_
         */
        void init();

        /**
         * @brief   Opens fstream with given parameters
         * @param   file [in,out] File stream to take open file
         * @param   mode [in] Mode how to open file read/write
         * @param   file_name [in] Name of the file to open
         */
        static int openStream(std::fstream &file, std::ios_base::openmode mode, const std::string &file_name);

	/**
	 * @brief   TODO
	 * @param path
	 */
	void parseSoftlinkFileContent(std::string& path);

    public: 

        /**
         * @brief   Basic constructor
         */
        File();

        /**
         * @brief   Constructor
         * @param   relative_path [in] Relative path to the file or symlink
         */
        File(const std::string& relative_path);

        /**
         * @brief   Basic constructor with custom file name
         * @param   relative_path [in] Relative path to the file or symlink
         * @param   name [in] Custom name of the file
         */
        File(const std::string& relative_path, const std::string& name);

	/**
	 * @brief   Creates new file
	 * @param   path [in] path to the new file
	 */
	static void createNew(const std::string& path);

        /**
         * @brief   Checks wether the file at specified path exists
         * @param   path [in] Path to the file
         * @return  True when file exists, False when it doesn't
         */
        static bool exists(const std::string& path);

        /**
         * @brief   Opens file_name as read stream
         * @param   file [out] Reading fstream with opened file_name
         * @param   file_name [in] Path to the file to be opened
         * @param   mode [in] Mode how to open file binary/text
         */
        static int openReadStream(std::fstream &file, const std::string &file_name, std::ios_base::openmode mode = std::ios_base::in);

        /**
         * @brief   Opens file_name as write stream
         * @param   file [out] Writing fstream with opened file_name
         * @param   file_name [in] Path to the file to be opened
         * @param   mode [in] Mode how to open file binary/text, trunc/append
         */
        static int openWriteStream(std::fstream &file, const std::string &file_name, std::ios_base::openmode mode = std::ios_base::app);

        /**
         * @brief   Parses file extension form the path, works with filename too
         * @param   path [in] Path to the file
         */
        static std::string parseExtension(const std::string& path);

        /**
         * @brief   Fastest safe read of the line from the stream
         * @param   input_stream [in] File stream
         * @param   line [out] Read line as a string
         * @return  Input_stream, allows to call .eof() on function call 
         */
        static std::fstream& readLine(std::fstream& input_stream, std::string& line);

	/**
	 * @brief   Resolves BOINC symlink file to path
	 * @return  Resolved relative path
	 */
	std::string resolveSoftlink();

	/**
	 * @brief   Set file to be executable (chmod +x \<file\>)
	 */
	void setExecutableMode();
};
#endif // FILE_HPP
