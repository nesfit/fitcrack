/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackPCFG.hpp"

AttackPCFG::AttackPCFG(const ConfigTask &config, Directory &directory)
    : AttackExternalGenerator(
          config, directory) { // Change attack mode from 9 to 0
  
}

void AttackPCFG::addSpecificArguments() {
  AttackCrackingBase::addSpecificArguments();

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

  addGeneratorLimitingArgument("--preterminals-file"); //Add PCFG-Manager preterminals argument
  addGeneratorLimitingRequiredFile("preterminals"); //Add file with preterminals to the PCFG-Manager pipeline

  addGeneratorArgument("--grammar-file"); //Add PCFG-Manager grammar file
  addGeneratorRequiredFile("grammar"); //Add grammar as stickyfile

}

//
