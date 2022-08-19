/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef DIRECTORY_HPP 
#define DIRECTORY_HPP

#include "FilesysBase.hpp"
#include "File.hpp"
#include "Logging.hpp"
#include "RunnerUtils.hpp"

#include <map>
#include <string>
#include <vector>

#include <dirent.h>
#include <fcntl.h>


#ifdef _WIN32

 #include <direct.h>

 #define mkdir _mkdir

 #define DIR_CREATE_FLAGS 
 #define IFDIR_FLAG _S_IFDIR

#else

 #define DIR_CREATE_FLAGS , S_IRWXG | S_IRWXU | S_IROTH | S_IXOTH
 #define IFDIR_FLAG S_IFDIR 

#endif // _WIN32, __linux__

/** Class representing Directory in file-system */
/** @brief Functions makePath and exists are made based on https://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux/29828907#29828907 */
class Directory : public FilesysBase {
    private:

	/**
	 * @brief   Initialize directory
	 */
	void init();

        /** Holds all the files in the folder in one collection */
        std::map<std::string, Directory> directories_;  /**< Directories placed in the directory on file-system */
        std::map<std::string, File> files_;             /**< Files placed in the directory on file-system */

    public:

	/**
	 * @brief   Empty construtor
	 */
	Directory();

        /**
         * @brief   Basic constructor
         */
        Directory(const std::string& path);

        /**
         * @brief   Construtor 
         */
        Directory(const std::string& path, const std::string& name);
		
        /**
         * @brief   Checks wether the path points to existing directory
         * @param   path [in] Path to the directory
         * @return  True when exists, False when doesn't
         */
	static bool exists(const std::string& path);

	/**
	 * @brief   Finds sub directory in directory by name 
	 * @param   key [in] Name of the directory
	 * @param   value [out] Subdirectory object to save copy
	 * @return  True when found, False otherwise 
	 */
        bool find(const std::string& key, Directory& value) const; 

        /**
         * @brief   Finds file in directory by name 
         * @param   key [in] Name of the directory
         * @param   value [out] File object to save to
         * @return  True when found, False otherwise 
         */
        bool find(const std::string& key, File& value) const;

        /**
         * @brief   Finds file with part of the name with dynamic name, like
         *          with version which can change over time
         * @param   prefix [in] Fixed part of the name of the file at start
         * @param   suffix [in] Fixed part of the name of the file at end
         * @param   file [out] File object to save to
         * @return  True when found, False otherwise 
         */
        bool findVersionedFile(const std::string& prefix, const std::string& suffix, File& file);

        /**
         * @brief   Checks wether directory has some subdirectories
         * @return  True when > 0 subdirectories, False otherwise 
         */
        bool hasDirectories() const;

        /**
         * @brief   Checks wether directory has some subfiles
         * @return  True when > 0 subfiles, False otherwise 
         */
        bool hasFiles() const;

        // based on https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c/612176#612176
        /**
         * @brief   Scan all entities in specified folder
         */
        void scanForEntities();

        /**
         * @brief   Makes directory chain out of the given path
         * @param   path [in] Path to targeted directory or file
         */
        static void makePath(const std::string& path);

        /**
         * @brief   Prints all directories in the directory as "name -> path" 
         */
        void printDirectories();

        /**
         * @brief   Prints all files in the directory as "name -> path" 
         */
        void printFiles();


};
#endif //DIRECTORY_HPP
