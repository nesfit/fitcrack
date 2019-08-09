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
      printf("in 9,0\n");
        // Do nothing, just at the end add grammar and preterminal files

    }
    else if (attack_submode_ == "1"){

        addArgument("--rules-file");
        addRequiredFile("rules");

    }
    else {
        RunnerUtils::runtimeException("Unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
    }

    printf("after\n");
    //+manager arguments

    addPCFGArgument("--preterminal-file"); //Add PCFG-Manager preterminals argument
    addPCFGRequiredFile("preterminals"); //Add file with preterminals to the PCFG-Manager pipeline

    addPCFGArgument("--grammar-file"); //Add PCFG-Manager grammar file
    addPCFGRequiredFile("grammar"); //Add grammar as stickyfile

    printf("arguments and files added \n");

}

//
void AttackPCFG::addPCFGArgument(std::string argument){
    std::string *new_PCFG_argument = new std::string(argument);
    PCFG_arguments_.push_back(TOCSTRING(new_PCFG_argument->c_str()));
    Logging::debugPrint(Logging::Detail::ObjectManipulation, "Adding PCFG argument '" + *new_PCFG_argument + "'");
}

void AttackPCFG::addPCFGRequiredFile(const std::string& file_name) {

    Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

    File filePCFG;
    if (!directory_.find(file_name, filePCFG)) {
        RunnerUtils::runtimeException("Missing PCFG-Manager required file " + file_name);
    }

    addPCFGArgument(filePCFG.getRelativePath());
}
//
