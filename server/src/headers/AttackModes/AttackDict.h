/**
 * @file AttackDict.h
 * @brief Header file for creation of Dictionary Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKDICT_H
#define WORKGENERATOR_ATTACKDICT_H

#include <AttackMode.h>


class CAttackDict : public AttackMode {
    public:
        /**
         * @brief Constructor for Combinator Attack
         * @param package [in] Instance of CPackage which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackDict(PtrPackage & package, PtrHost & host, uint64_t seconds, CSqlLoader * sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackDict() override = default;

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

#endif //WORKGENERATOR_ATTACKDICT_H
