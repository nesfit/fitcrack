/*
 * Author : Dávid Bolvanský
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKPRINCE_HPP
#define ATTACKPRINCE_HPP

#include "AttackCrackingBase.hpp"

/** Class representing hashcat's/Prince attack */
class AttackPrince: public AttackCrackingBase {
    std::vector<std::string> Prince_arguments_; /**< Prince preprocessor arguments based on the config */
    protected:

        /**
         * @brief   Adds all attack specific arguments
         */
        void addSpecificArguments();
        void addPrinceArgument(const std::string &argument);
        void addPrinceRequiredFile(const std::string& file_name);

    public:

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         * @param   directory [in] Working directory
         */
      AttackPrince(const ConfigTask &config, Directory &directory);
      const std::vector<std::string> &getPrinceArguments();
};
#endif // ATTACKPRINCE_HPP
