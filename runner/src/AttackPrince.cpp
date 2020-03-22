/*
* Author : Dávid Bolvanský
* Licence: MIT, see LICENSE
*/

#include "AttackPrince.hpp"

AttackPrince::AttackPrince(const ConfigTask &config, Directory &directory)
    : AttackExternalGenerator(config, directory)
{}

void AttackPrince::addSpecificArguments() {
  AttackCrackingBase::addSpecificArguments();
  if (attack_submode_ == "1") {
    addArgument("--rules-file");
    addRequiredFile("rules");
  } else if(attack_submode_ != "0") {
    RunnerUtils::runtimeException(
        "Unsupported attack_submode = " + attack_submode_ +
        " attack_mode = " + attack_mode_ + " has no such attack_submode");
  }

  addGeneratorRequiredFile("dict1"); // Add wordlist

  // Add PRINCE specific settings
  std::string value;
  if (config_.find(ConfigTask::CASE_PERMUTE, value) && value == "1")
    addGeneratorArgument("--case-permute");
  if (config_.find(ConfigTask::CHECK_DUPLICATES, value) && value == "0")
    addGeneratorArgument("--dupe-check-disable");
  if (config_.find(ConfigTask::SKIP_FROM_START, value))
    addGeneratorLimitingArgument("--skip=" + value);
  if (config_.find(ConfigTask::DICT_HC_KEYSPACE, value))
    addGeneratorLimitingArgument("--limit=" + value);
  if (config_.find(ConfigTask::MIN_PASSWORD_LEN, value))
    addGeneratorArgument("--pw-min=" + value);
  if (config_.find(ConfigTask::MAX_PASSWORD_LEN, value))
    addGeneratorArgument("--pw-max=" + value);
  if (config_.find(ConfigTask::MIN_ELEM_IN_CHAIN, value))
    addGeneratorArgument("--elem-cnt-min=" + value);
  if (config_.find(ConfigTask::MAX_ELEM_IN_CHAIN, value))
    addGeneratorArgument("--elem-cnt-max=" + value);
}

//
