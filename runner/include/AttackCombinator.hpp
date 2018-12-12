/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKCOMBINATOR_HPP 
#define ATTACKCOMBINATOR_HPP

#include "AttackCrackingBase.hpp"

/** Class representing hashcat's combinator attack */
class AttackCombinator: public AttackCrackingBase {

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
        AttackCombinator(const ConfigTask& config, Directory& directory);

};

#endif // ATTACKCOMBINATOR_HPP
