/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "AttackBenchmark.hpp"

AttackBenchmark::AttackBenchmark(const ConfigTask& config) : AttackBase(config) {
    addSpecificArguments();
}

void AttackBenchmark::addSpecificArguments() {
    addArgument("--benchmark");
}
