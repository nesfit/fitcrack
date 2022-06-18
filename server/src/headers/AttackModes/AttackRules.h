/**
 * @file AttackRules.h
 * @brief Header file for creation of Rules Attack
 * In hashcat terminology, a dictionary attack with rules
 *
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKRULES_H
#define WORKGENERATOR_ATTACKRULES_H

#include <AttackMode.h>


class CAttackRules : public AttackMode {
    public:
        /**
         * @brief Constructor for Combinator Attack
         * @param job [in] Instance of CJob which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackRules(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackRules() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_workunit
         * @return True if a workunit was planned, False otherwise
         */
        bool makeWorkunit() override ;

        virtual bool requiresDicts() const override {return true;}

        virtual bool hasStickyLeftDict() const override {
          return m_job->getDistributionMode() == 1;
        }

    private:
        /**
         * @brief Function to generate new CWorkunit for certain host for given time
         * @return True if workunit was generated successfully, False otherwise
         */
        bool generateWorkunit() override ;
};

#endif //WORKGENERATOR_ATTACKRULES_H
