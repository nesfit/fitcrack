/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKPCFG_HPP
#define ATTACKPCFG_HPP

#include "AttackCrackingBase.hpp"

/** Class representing hashcat's/PFCG manager PCFG attack */
class AttackPCFG: public AttackCrackingBase {

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
        AttackPCFG(const ConfigTask& config, Directory& directory);

};
#endif // ATTACKPCFG_HPP
