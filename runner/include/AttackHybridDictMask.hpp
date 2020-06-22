/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKHYBRIDDICTMASK_HPP
#define ATTACKHYBRIDDICTMASK_HPP

#include "AttackWithMask.hpp"

/** Class representing hashcat's combinator attack */
class AttackHybridDictMask: public AttackWithMask {

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
        AttackHybridDictMask(const ConfigTask& config, Directory& directory);

};

#endif // ATTACKHYBRIDDICTMASK_HPP
