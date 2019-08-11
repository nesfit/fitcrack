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

#include "ConfigTask.hpp"

#include "Directory.hpp"

#include "File.hpp"

//bool isPCFG = false;

/** Namespace with function for creating new attack */
namespace Attack {
    /**
     * @brief	Creates new config based attack
     * @param   config [in] Structure with sent parameters
     * @param   directory [in] Working directory with found files
     * @return  Specific type of attack object base on config parameters
     */
    AttackBase* create(const ConfigTask& config, Directory& directory, bool& isPCFG);
};

#endif // ATTACK_HPP
