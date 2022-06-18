/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackDictionary.hpp"

AttackDictionary::AttackDictionary(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory) {
  
}

void AttackDictionary::addSpecificArguments() {
  AttackCrackingBase::addSpecificArguments();

  if (attack_submode_ == "0") {

    // Do nothing just at the end add dictionaries

  } else if (attack_submode_ == "1") {

    addArgument("--rules-file");
    addRequiredFile("rules");

  } else {
    RunnerUtils::runtimeException("Unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
  }

  std::string distribution_mode = "0";
  config_.find(ConfigTask::DISTRIBUTION_MODE, distribution_mode);

  if (distribution_mode == "0") { // fragmenting on server
    addRequiredFile("dict1");
  } else if (distribution_mode == "1") { // fragmenting on client
    std::string skip, limit;
    if (config_.find(ConfigTask::SKIP_FROM_START, skip) && skip != "0")
      addArgument("--skip=" + skip);
    if (config_.find(ConfigTask::DICT_HC_KEYSPACE, limit) && limit != "0")
      addArgument("--limit=" + limit);

    addRequiredFile("dict1");
  }
}
