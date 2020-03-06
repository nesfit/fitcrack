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
        /**
         * @brief   Adds prince specific argument
         * @param   argument [in] Argument
         */
        void addPrinceArgument(const std::string &argument);
        /**
         * @brief   Adds prince specific file
         * @param   file_name [in] Filename
         */
        void addPrinceRequiredFile(const std::string& file_name);

    public:

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         * @param   directory [in] Working directory
         */
      AttackPrince(const ConfigTask &config, Directory &directory);
        /**
         * @brief   Get prince attack arguments
         * @return  Arguments
         */
      const std::vector<std::string> &getPrinceArguments();
};
#endif // ATTACKPRINCE_HPP
