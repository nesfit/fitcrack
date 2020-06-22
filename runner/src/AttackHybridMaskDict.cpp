/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackHybridMaskDict.hpp"

AttackHybridMaskDict::AttackHybridMaskDict(const ConfigTask& config, Directory& directory) : AttackWithMask(config, directory) {
  
}

void AttackHybridMaskDict::addSpecificArguments() {

  AttackCrackingBase::addSpecificArguments();

  createMaskAndCharsets();

  addRequiredFile("dict2");

  findAndAddOptional("rule_right", "--rule-right");
}
