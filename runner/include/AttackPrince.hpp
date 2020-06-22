/*
 * Author : Dávid Bolvanský
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKPRINCE_HPP
#define ATTACKPRINCE_HPP

#include "AttackExternalGenerator.hpp"

/** Class representing hashcat's/Prince attack */
class AttackPrince: public AttackExternalGenerator {
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
        AttackPrince(const ConfigTask &config, Directory &directory);

        virtual std::string getExternalGeneratorName() {return "prince";}
};
#endif // ATTACKPRINCE_HPP
