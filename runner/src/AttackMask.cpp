/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackMask.hpp"

/* Protected */

void AttackMask::addSpecificArguments() {
  AttackCrackingBase::addSpecificArguments();
  if (attack_submode_ == "0") {

    addArgument("--markov-disable");

  } else if (attack_submode_ == "1") {
    // This and next else-if could be rewritten but from this is easier to
    // read and say what are parameters for which attack

    addArgument("--markov-hcstat");
    addRequiredFile("markov");
    addArgument("--markov-classic");
    findAndAddOptional("markov_threshold", "--markov-threshold");

  } else if (attack_submode_ == "2") {

    addArgument("--markov-hcstat");
    addRequiredFile("markov");
    findAndAddOptional("markov_threshold", "--markov-threshold");

  } else {
    RunnerUtils::runtimeException("AttackMask: unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
  }

  createMaskAndCharsets();

}


/* Public */
AttackMask::AttackMask(const ConfigTask& config, Directory& directory) : AttackWithMask(config, directory) {
  
}
