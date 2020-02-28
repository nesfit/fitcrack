/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackPCFG.hpp"

AttackPCFG::AttackPCFG(const ConfigTask &config, Directory &directory)
    : AttackCrackingBase(
          config, directory,
          /*attack_mode=*/"0") { // Change attack mode from 9 to 0
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

  addPCFGArgument("--preterminals-file"); //Add PCFG-Manager preterminals argument
  addPCFGRequiredFile("preterminals"); //Add file with preterminals to the PCFG-Manager pipeline

  addPCFGArgument("--grammar-file"); //Add PCFG-Manager grammar file
  addPCFGRequiredFile("grammar"); //Add grammar as stickyfile

}

//
void AttackPCFG::addPCFGArgument(const std::string &argument){
  Logging::debugPrint(Logging::Detail::ObjectManipulation, "Adding PCFG argument '" + argument + "'");
  PCFG_arguments_.push_back(argument);
}

void AttackPCFG::addPCFGRequiredFile(const std::string& file_name) {

  Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

  File filePCFG;
  if (!directory_.find(file_name, filePCFG)) {
    RunnerUtils::runtimeException("Missing PCFG-Manager required file " + file_name);
  }

  addPCFGArgument(filePCFG.getRelativePath());
}

const std::vector<std::string>& AttackPCFG::getPCFGArguments() {
  return PCFG_arguments_;
}

//
