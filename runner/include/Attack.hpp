/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACK_HPP
#define ATTACK_HPP

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
  AT_HybridDictMask,
  AT_HybridMaskDict,
  AT_Unknown
};

class AttackBase;

/** Namespace with function for creating new attack */
namespace Attack {
    /**
     * @brief	Creates new config based attack
     * @param   task_config [in] Structure with sent parameters
     * @param   directory [in] Working directory with found files
     * @param   attack_type [in] Attack type
     * @return  Specific type of attack object base on config parameters
     */
AttackBase *create(const ConfigTask &task_config, Directory &directory);
    /**
     * @brief	Detects attack type from task configuration
     * @param   task_config [in] Structure with sent parameters
     * @return  Attack type
     */
enum AttackType detectAttackType(const ConfigTask &task_config);
};

#endif // ATTACK_HPP
