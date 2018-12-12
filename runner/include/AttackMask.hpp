/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKMASK_HPP 
#define ATTACKMASK_HPP

#include "AttackCrackingBase.hpp"

#include "File.hpp"

/** Class representing hashcat's mask attack */
class AttackMask: public AttackCrackingBase {

    private:

	bool has_charsets_;		    /**< Indicates whether attack has some charsets */

	/**
	 * @brief   Adds argument and charset file to hashcat, calls saveCharset
	 * @param   mask_file [in] Opened file which contains charsets and masks
	 * @param   charset_name [in] Charset name (charsetX)
	 */
	void addCharset(std::fstream& mask_file, const std::string& charset_name);

	/**
	 * @brief   Adds mask to the file which is then passed to hashcat
	 * @param   mask_file [in] File to write to as a stream
	 */
	void addMask(std::fstream& mask_file);

	/**
	 * @brief   Creates mask file which is than passed to hashcat. It allows
	 *	    us to use '/' (slash) in mask without hashcat thinking it is
	 *	    a path to some file as proposed at
	 *	    https://hashcat.net/forum/thread-7738-post-41579.html#pid41579
	 *	    The file also contains charsets when some are specified
	 */
	void createMaskFile();

	/**
	 * @brief   Takes charset from ConfigTask and saves it to the file with
	 *	    \<charset_name\>.hcchr
	 * @param   charset_name [in] Charset name (charsetX)
	 * @param   content [in] Value of the charset saved in config
	*/
	std::string saveCharset(const std::string& charset_name, const std::string& content);

	/**
	 * @brief   Sets has_charsets_ flag
	 */
	void setHasCharsets();

    protected:

        /**
         * @brief   Adds all attack specific arguments
         */
        void addSpecificArguments();

    public: 

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         * @param   directory [in] Working directory
         */
        AttackMask(const ConfigTask& config, Directory& directory);
};
#endif // ATTACKMASK_HPP
