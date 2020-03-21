/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKPCFG_HPP
#define ATTACKPCFG_HPP

#include "AttackExternalGenerator.hpp"

/** Class representing hashcat's/PFCG manager PCFG attack */
class AttackPCFG: public AttackExternalGenerator {
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
        AttackPCFG(const ConfigTask &config, Directory &directory);

        virtual std::string getExternalGeneratorName() {return "pcfg";}
};
#endif // ATTACKPCFG_HPP
