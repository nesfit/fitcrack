/*
* Author : Dávid Bolvanský
* Licence: MIT, see LICENSE
*/

#include "AttackPrince.hpp"

AttackPrince::AttackPrince(const ConfigTask &config, Directory &directory)
    : AttackCrackingBase(
          config, directory,
          /*attack_mode=*/"0") { // Change attack mode from 8 to 0
  addSpecificArguments();
}

void AttackPrince::addSpecificArguments() {
  if (attack_submode_ == "0") {
    // Do nothing.
  } else if (attack_submode_ == "1") {
    addArgument("--rules-file");
    addRequiredFile("rules");
  } else {
    RunnerUtils::runtimeException(
        "Unsupported attack_submode = " + attack_submode_ +
        " attack_mode = " + attack_mode_ + " has no such attack_submode");
  }

  addPrinceRequiredFile("dict1"); // Add wordlist

  // Add PRINCE specific settings
  std::string value;
  if (config_.find(ConfigTask::CASE_PERMUTE, value) && value == "1")
    addPrinceArgument("--case-permute");
  if (config_.find(ConfigTask::CHECK_DUPLICATES, value) && value == "0")
    addPrinceArgument("--dupe-check-disable");
  if (config_.find(ConfigTask::MIN_PASSWORD_LEN, value))
    addPrinceArgument("--pw-min=" + value);
  if (config_.find(ConfigTask::MAX_PASSWORD_LEN, value))
    addPrinceArgument("--pw-max=" + value);
  if (config_.find(ConfigTask::MIN_ELEM_IN_CHAIN, value))
    addPrinceArgument("--elem-cnt-min=" + value);
  if (config_.find(ConfigTask::MAX_ELEM_IN_CHAIN, value))
    addPrinceArgument("--elem-cnt-max=" + value);
}

void AttackPrince::addPrinceArgument(const std::string &argument){
  Logging::debugPrint(Logging::Detail::ObjectManipulation, "Adding Prince preprocessor argument '" + argument + "'");
  Prince_arguments_.push_back(argument);
}

void AttackPrince::addPrinceRequiredFile(const std::string& file_name) {

  Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

  File filePrince;
  if (!directory_.find(file_name, filePrince)) {
    RunnerUtils::runtimeException("Missing Prince preprocessor required file " + file_name);
  }

  addPrinceArgument(filePrince.getRelativePath());
}

const std::vector<std::string>& AttackPrince::getPrinceArguments() {
  return Prince_arguments_;
}

//
