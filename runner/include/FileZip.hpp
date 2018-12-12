/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ZIPFILE_HPP 
#define ZIPFILE_HPP

#include "File.hpp"
#include "Directory.hpp"


#ifdef _WIN32
  #define ZIP_STATIC
#endif

#include "zip.h"

#define ZIP_READ_SIZE 100

/** Class represeting ZIP archive on file-system */
class FileZip : public File {
    
    private:

        bool flag_opened_;      /**< Flag signifying wether the archive is open */

        int error_;             /**< Member for the saving the zip errors */

        zip_t *handle_;         /**< Handle for the archive file to be open to */

        /**
         * @brief   Decodes entry of the archive and saves it on the file system
         * @param   entry_number [in] Identifier of the entry inside the archive
         * @param   entry_stats [in] Structure with information about entry
         */
        void decodeEntry(size_t entry_number, zip_stat_t *entry_stats);

        /**
         * @brief   Extracts entry from the archive 
         * @param   entry_number [in] Identifier of the entry inside the archive
         */
        void extractEntry(size_t entry_number);

    public: 

        /**
         * @brief   Basic constructor
         * @param   relative_path [in] Relative path to the file or symlink
         */
        FileZip(const std::string& relative_path);

        /**
         * @brief   Basic constructor with custom file name
         * @param   relative_path [in] Relative path to the file or symlink
         * @param   name [in] Custom name of the file
         */
        FileZip(const std::string& relative_path, const std::string& name);

        /**
         * @brief   Construtor using copy constructor of base class
         * @param   file [in] Existing File object 
         */
        FileZip(const File& file);

        /**
         * @brief   Closes the archive file and frees the handle_
         */
        void close();

        /**
         * @brief   Extracts Zip with all the directories 
         */
        void extract();

        /**
         * @brief   Getter of flag_opened_
         * @return  True when open, False otherwise 
         */
        bool isOpen() const;

        /**
         * @brief   Opens archive and saves descriptor to handle_
         */
        void open();
};
#endif // ZIPFILE_HPP

