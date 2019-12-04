/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "AttackBase.hpp"
#include "AttackBenchmark.hpp"
#include "AttackCombinator.hpp"
#include "AttackDictionary.hpp"
#include "AttackMask.hpp"
#include "AttackPCFG.hpp"
#include "AttackPrince.hpp"

#include "ConfigTask.hpp"

#include "Directory.hpp"

#include "File.hpp"

enum AttackType {
  AT_Dictionary,
  AT_Combinator,
  AT_Mask,
  AT_Prince,
  AT_PCFG,
  AT_Benchmark,
  AT_Unknown
};

/** Namespace with function for creating new attack */
namespace Attack {
    /**
     * @brief	Creates new config based attack
     * @param   task_config [in] Structure with sent parameters
     * @param   directory [in] Working directory with found files
     * @return  Specific type of attack object base on config parameters
     */
AttackBase *create(const ConfigTask &task_config, Directory &directory, enum AttackType attack_type);
enum AttackType detectAttackType(const ConfigTask &task_config);
};

#endif // ATTACK_HPP
