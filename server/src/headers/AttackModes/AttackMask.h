/**
 * @file AttackMask.h
 * @brief Header file for creation of Mask Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKMASK_H
#define WORKGENERATOR_ATTACKMASK_H

#include <AttackMode.h>


class CAttackMask : public AttackMode {
    public:
        /**
         * @brief Constructor for Combinator Attack
         * @param package [in] Instance of CPackage which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackMask(PtrPackage & package, PtrHost & host, uint64_t seconds, CSqlLoader * sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackMask() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_job
         * @return True if a job was planned, False otherwise
         */
        bool makeJob() override ;

    private:
        /**
         * @brief Function to generate new CJob for certain host for given time
         * @return True if job was generated successfully, False otherwise
         */
        bool generateJob() override ;
};

#endif //WORKGENERATOR_ATTACKMASK_H
