/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "Attack.hpp"

AttackBase *Attack::create(const ConfigTask &task_config, Directory &directory, enum AttackType attack_type) {
  switch (attack_type) {
  case AT_Dictionary:
    return new AttackDictionary(task_config, directory);
    break;
  case AT_Combinator:
    return new AttackCombinator(task_config, directory);
    break;
  case AT_Mask:
    return new AttackMask(task_config, directory);
    break;
  case AT_Prince:
    return new AttackPrince(task_config, directory);
  case AT_PCFG:
    return new AttackPCFG(task_config, directory);
    break;
  default:
    RunnerUtils::runtimeException("Invalid attack mode");
    return nullptr;
  }
}

enum AttackType Attack::detectAttackType(const ConfigTask &task_config) {
  std::string mode, attack_mode;

  task_config.find("mode", mode);
  task_config.find("attack_mode", attack_mode);
  if (mode == "n") {
    switch (attack_mode[0]) {
    case '0':
      return AT_Dictionary;
      break;
    case '1':
      return AT_Combinator;
      break;
    case '3':
      return AT_Mask;
      break;
    case '8':
      return AT_Prince;
    case '9':
      return AT_PCFG;
      break;
    default:
      return AT_Unknown;
    }
  } else if (mode == "a" || mode == "b") {
    return AT_Benchmark;
  }
  return AT_Unknown;
}