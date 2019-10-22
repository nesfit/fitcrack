/**
 * @file AttackBench.h
 * @brief Header file for creation of Benchmark workunit
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKBENCH_H
#define WORKGENERATOR_ATTACKBENCH_H

#include <AttackMode.h>


class CAttackBench : public AttackMode {
    public:
        /**
         * @brief Constructor for Benchmark Workunit
         * @param package [in] Instance of CPackage which is parent of this workunit instance
         * @param host [in] Instance of CHost which this workunit belongs to
         */
        CAttackBench(PtrPackage & package, PtrHost & host, CSqlLoader * sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackBench() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_workunit
         * @return True if a workunit was planned, False otherwise
         */
        bool makeWorkunit() override ;

    private:
        /**
         * @brief Function to generate new CWorkunit for certain host
         * @return True if workunit was generated successfully, False otherwise
         */
        bool generateWorkunit() override ;
};

#endif //WORKGENERATOR_ATTACKBENCH_H
