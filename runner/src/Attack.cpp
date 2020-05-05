/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "Attack.hpp"

#include "AttackBase.hpp"
#include "AttackBenchmark.hpp"
#include "AttackCombinator.hpp"
#include "AttackDictionary.hpp"
#include "AttackHybridDictMask.hpp"
#include "AttackMask.hpp"
#include "AttackPCFG.hpp"
#include "AttackPrince.hpp"

AttackType AttackModeToType(char attack_mode)
{
  switch (attack_mode) {
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
}

AttackBase *Attack::create(const ConfigTask &task_config, Directory &directory) {
  
  AttackBase *attack = nullptr;

  switch (detectAttackType(task_config)) {
  case AT_Dictionary:
    attack = new AttackDictionary(task_config, directory);
    break;
  case AT_Combinator:
    attack = new AttackCombinator(task_config, directory);
    break;
  case AT_Mask:
    attack = new AttackMask(task_config, directory);
    break;
  case AT_HybridDictMask:
    attack = new AttackHybridDictMask(task_config, directory);
    break;
  case AT_Prince:
    attack = new AttackPrince(task_config, directory);
    break;
  case AT_PCFG:
    attack = new AttackPCFG(task_config, directory);
    break;
  case AT_Benchmark:
  {
    std::string attack_mode;
    task_config.find("attack_mode", attack_mode);
    switch(AttackModeToType(attack_mode[0]))
    {
    case AT_Dictionary:
      attack = new AttackBenchmark<AttackDictionary>(task_config, directory);
      break;
    case AT_Combinator:
      attack = new AttackBenchmark<AttackCombinator>(task_config, directory);
      break;
    case AT_Mask:
      attack = new AttackBenchmark<AttackMask>(task_config, directory);
      break;
    case AT_HybridDictMask:
      attack = new AttackBenchmark<AttackHybridDictMask>(task_config, directory);
      break;
    case AT_Prince:
      attack = new AttackBenchmark<AttackPrince>(task_config, directory);
      break;
    case AT_PCFG:
      attack = new AttackBenchmark<AttackPCFG>(task_config, directory);
      break;
    default:
      RunnerUtils::runtimeException("invalid attack mode for benchmark");
      return nullptr;
    }
    break;
  }
  default:
    RunnerUtils::runtimeException("Invalid attack mode");
    return nullptr;
  }
  attack->addSpecificArguments();
  return attack;
}

enum AttackType Attack::detectAttackType(const ConfigTask &task_config) {
  std::string mode, attack_mode;

  task_config.find("mode", mode);
  task_config.find("attack_mode", attack_mode);
  if (mode == "n") {
    return AttackModeToType(attack_mode[0]);
  } else if (mode == "a" || mode == "b") {
    return AT_Benchmark;
  }
  return AT_Unknown;
}