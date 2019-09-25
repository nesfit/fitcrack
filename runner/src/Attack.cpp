/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "Attack.hpp"

AttackBase* Attack::create(const ConfigTask& task_config, Directory& directory, bool &isPCFG) {

  std::string mode, attack_mode;

  task_config.find("mode", mode);
  task_config.find("attack_mode", attack_mode);

  if (mode == "n") {
    switch (attack_mode[0]) {
      case '0':
      return new AttackDictionary(task_config, directory, isPCFG);
      break;
      case '1':
      return new AttackCombinator(task_config, directory, isPCFG);
      break;
      case '3':
      return new AttackMask(task_config, directory, isPCFG);
      break;
      case '9':
      isPCFG = true;
      return new AttackPCFG(task_config, directory, isPCFG);
      break;
      default:
      RunnerUtils::runtimeException("Invalid attack mode");
    }
  } else if (mode == "a" || mode == "b") {
    return new AttackBenchmark(task_config);
  } else {
    RunnerUtils::runtimeException("Invalid task mode");
  }
}
