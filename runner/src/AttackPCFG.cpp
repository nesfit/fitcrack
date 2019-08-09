/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "AttackPCFG.hpp"

AttackPCFG::AttackPCFG(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory) {
    addSpecificArguments();
}

void AttackPCFG::addSpecificArguments() {

    if (attack_submode_ == "0"){

        // Do nothing, just at the end add grammar and preterminal files

    }
    else if (attack_submode_ == "1"){

        addArgument("--rules-file");
        addRequiredFile("rules");

    }
    else {
        RunnerUtils::runtimeException("Unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
    }


    addPCFGArgument("--preterminal-file"); //Add PCFG-Manager preterminals argument
    addPCFGRequiredFile("preterminals"); //Add file with preterminals to the PCFG-Manager pipeline

    addPCFGArgument("--grammar-file"); //Add PCFG-Manager grammar file
    addPCFGRequiredFile("grammar"); //Add grammar as stickyfile

}
