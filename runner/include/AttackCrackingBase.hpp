/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKCRACKINGBASE_HPP 
#define ATTACKCRACKINGBASE_HPP

#include "AttackBase.hpp"

#include "Directory.hpp"

/** Class representing base class for all cracking attack */
class AttackCrackingBase: public AttackBase {

    protected:

        Directory& directory_;          /**< Working directory to look for files in */

        std::string attack_submode_;    /**< Submode from TLV */

        /**
         * @brief   Adds all attack specific arguments
         */
        void addSpecificArguments();

        /**
         * @brief   Adds requred file form directory to arguments
         * @param   file_name [in] Name of the file to add
         */
        void addRequiredFile(const std::string& file_name);

        /**
         * @brief   Adds requred file form directory to arguments
         * @param   file_name [in] Name of the file to add
         * @param   argument [in] Name of the argument to add as predecesor of
	 *	    file_name
         */
        void addOptionalFile(const std::string& file_name, const std::string& argument);


    public: 

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         * @param   directory [in] Working directory
         */
        AttackCrackingBase(const ConfigTask& config, Directory& directory);

};

#endif // ATTACKCRACKINGBASE_HPP
