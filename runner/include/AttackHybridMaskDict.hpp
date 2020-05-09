/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKHYBRIDMASKDICT_HPP
#define ATTACKHYBRIDMASKDICT_HPP

#include "AttackWithMask.hpp"

/** Class representing hashcat's combinator attack */
class AttackHybridMaskDict: public AttackWithMask {

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
        AttackHybridMaskDict(const ConfigTask& config, Directory& directory);

};

#endif // ATTACKHYBRIDMASKDICT_HPP
