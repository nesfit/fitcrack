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
#include "AttackHybridMaskDict.hpp"
#include "AttackMask.hpp"
#include "AttackPCFG.hpp"
#include "AttackPrince.hpp"
#include "AttackAssociation.hpp"

AttackType AttackModeToType(std::string attack_mode)
{
  // opted for readablility but tree would be faster
  if(attack_mode == "0")
    return AT_Dictionary;
  if(attack_mode == "1")
    return AT_Combinator;
  if(attack_mode == "3")
    return AT_Mask;
  if(attack_mode == "6")
    return AT_HybridDictMask;
  if(attack_mode == "7")
    return AT_HybridMaskDict;
  if(attack_mode == "8")
    return AT_Prince;
  if(attack_mode == "9")
    return AT_PCFG;
  if(attack_mode == "10")
    return AT_Association;
  return AT_Unknown;
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
  case AT_HybridMaskDict:
    attack = new AttackHybridMaskDict(task_config, directory);
    break;
  case AT_Prince:
    attack = new AttackPrince(task_config, directory);
    break;
  case AT_PCFG:
    attack = new AttackPCFG(task_config, directory);
    break;
  case AT_Association:
    attack = new AttackAssociation(task_config, directory);
    break;
  case AT_Benchmark:
  {
    std::string attack_mode;
    task_config.find("attack_mode", attack_mode);
    switch(AttackModeToType(attack_mode))
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
    case AT_HybridMaskDict:
      attack = new AttackBenchmark<AttackHybridMaskDict>(task_config, directory);
      break;
    case AT_Prince:
      attack = new AttackBenchmark<AttackPrince>(task_config, directory);
      break;
    case AT_PCFG:
      attack = new AttackBenchmark<AttackPCFG>(task_config, directory);
      break;
    case AT_Association:
      attack = new AttackBenchmark<AttackAssociation>(task_config, directory);
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
    return AttackModeToType(attack_mode);
  } else if (mode == "a" || mode == "b") {
    return AT_Benchmark;
  }
  return AT_Unknown;
}