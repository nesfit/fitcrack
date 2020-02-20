/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKPCFG_HPP
#define ATTACKPCFG_HPP

#include "AttackCrackingBase.hpp"

/** Class representing hashcat's/PFCG manager PCFG attack */
class AttackPCFG: public AttackCrackingBase {
    std::vector<std::string> PCFG_arguments_; /**< PCFG-manager arguments based on the config */
    protected:

        /**
         * @brief   Adds all attack specific arguments
         */
        void addSpecificArguments();
        /**
         * @brief   Adds pcfg specific argument
         * @param   argument [in] Argument
         */
        void addPCFGArgument(const std::string &argument);
        /**
         * @brief   Adds pcfg specific file
         * @param   file_name [in] Filename
         */
        void addPCFGRequiredFile(const std::string& file_name);

    public:

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         * @param   directory [in] Working directory
         */
      AttackPCFG(const ConfigTask &config, Directory &directory);
        /**
         * @brief   Get pcfg attack arguments
         * @return  Arguments
         */
      const std::vector<std::string> &getPCFGArguments();
};
#endif // ATTACKPCFG_HPP
