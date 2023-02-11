/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackAssociation.hpp"
#include "Dictstat.hpp"


AttackAssociation::AttackAssociation(const ConfigTask& config, Directory& directory)
  : AttackCrackingBase(config, directory, "9") { // change attack mode for hashcat
  std::string mode;

  config.find("mode", mode);

  // This is abusing an undefined behavior in hascat (multiple definitions of attack mode argument)
  // The last defined one will be considered the correct
  if (mode == "b" || mode == "a"){
    // Association mode doen't like 0 keyspace so testing is done on dictionary
    addArgument("-a");
    addArgument("0");
  }
}

void AttackAssociation::addSpecificArguments() {
  AttackCrackingBase::addSpecificArguments();

  if (attack_submode_ == "0") {

    // Do nothing just at the end add dictionaries

  } else if (attack_submode_ == "1") {

    addArgument("--rules-file");
    addRequiredFile("rules");

  } else {
    RunnerUtils::runtimeException("Unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
  }

  std::string relativePath = addRequiredFile("dict1");

  std::string dict1Keyspace;
  if (config_.find(ConfigTask::DICT1_KEYSPACE, dict1Keyspace)) {
    // Build hashcat.dictstat2 so hashcat does not have to recompute
    // number of passwords in this dictionary - could be a bottleneck for huge
    // dictionaries.
    DictStatBuilder dsBuilder;
    bool dict1StatAdded =
        dsBuilder.addStatForDict(relativePath.c_str(), stoull(dict1Keyspace));
    if (dict1StatAdded) {
      Logging::debugPrint(Logging::Detail::GeneralInfo,
                          "dictstat2 created for " + relativePath);
    }
  }
}
