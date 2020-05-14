/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackHybridDictMask.hpp"

AttackHybridDictMask::AttackHybridDictMask(const ConfigTask& config, Directory& directory) : AttackWithMask(config, directory) {
  
}

void AttackHybridDictMask::addSpecificArguments() {

  AttackCrackingBase::addSpecificArguments();

  addRequiredFile("dict1");

  createMaskAndCharsets();

  findAndAddOptional("rule_left", "--rule-left");
}
