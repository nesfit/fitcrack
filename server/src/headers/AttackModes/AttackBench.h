/**
 * @file AttackBench.h
 * @brief Header file for creation of Benchmark job
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
         * @brief Constructor for Benchmark Job
         * @param package [in] Instance of CPackage which is parent of this job instance
         * @param host [in] Instance of CHost which this job belongs to
         */
        CAttackBench(PtrPackage & package, PtrHost & host, CSqlLoader * sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackBench() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_job
         * @return True if a job was planned, False otherwise
         */
        bool makeJob() override ;

    private:
        /**
         * @brief Function to generate new CJob for certain host
         * @return True if job was generated successfully, False otherwise
         */
        bool generateJob() override ;
};

#endif //WORKGENERATOR_ATTACKBENCH_H
