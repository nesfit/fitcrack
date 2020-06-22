/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKMASK_HPP
#define ATTACKMASK_HPP

#include "AttackWithMask.hpp"

#include "File.hpp"

/** Class representing hashcat's mask attack */
class AttackMask: public AttackWithMask {

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
