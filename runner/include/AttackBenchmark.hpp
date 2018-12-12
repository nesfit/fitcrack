/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKBENCHMARK_HPP 
#define ATTACKBENCHMARK_HPP

#include "AttackBase.hpp"

/** Class representing hashcat's attack benchmark */
class AttackBenchmark: public AttackBase {

    protected:

        /**
         * @brief   Pure virtual function adding all atttack specific arguments
         */
        void addSpecificArguments();

    public: 

        /**
         * @brief   Basic constructor
         * @param   config [in] Representation of config file
         */
        AttackBenchmark(const ConfigTask& config);

};
#endif // ATTACKBENCHMARK_HPP
