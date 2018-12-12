/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "AttackDictionary.hpp"

AttackDictionary::AttackDictionary(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory) {
    addSpecificArguments();
}

void AttackDictionary::addSpecificArguments() {

    if (attack_submode_ == "0") {

        // Do nothing just at the end add dictionaries

    } else if (attack_submode_ == "1") {

        addArgument("--rules-file");
        addRequiredFile("rules");

    } else {
        RunnerUtils::runtimeException("Unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
    }

    addRequiredFile("dict1");

}
