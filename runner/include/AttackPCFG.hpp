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
        void addPCFGArgument(const std::string &argument);
        void addPCFGRequiredFile(const std::string& file_name);

    public:

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         * @param   directory [in] Working directory
         */
      AttackPCFG(const ConfigTask &config, Directory &directory);
      const std::vector<std::string> &getPCFGArguments();
};
#endif // ATTACKPCFG_HPP
