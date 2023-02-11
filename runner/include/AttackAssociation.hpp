/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKASSOCIATION_HPP
#define ATTACKASSOCIATION_HPP

#include "AttackCrackingBase.hpp"

/** Class representing hashcat's association attack */
class AttackAssociation: public AttackCrackingBase {

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
        AttackAssociation(const ConfigTask& config, Directory& directory);

};
#endif // ATTACKASSOCIATION_HPP
