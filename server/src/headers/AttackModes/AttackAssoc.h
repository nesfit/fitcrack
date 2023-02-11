/**
 * @file AttackDict.h
 * @brief Header file for creation of Dictionary Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKASSOC_H
#define WORKGENERATOR_ATTACKASSOC_H

#include <AttackMode.h>


class CAttackAssoc : public AttackMode {
    public:
        /**
         * @brief Constructor for Association Attack
         * @param job [in] Instance of CJob which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackAssoc(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackAssoc() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_workunit
         * @return True if a workunit was planned, False otherwise
         */
        bool makeWorkunit() override ;

        virtual bool requiresDicts() const override {return true;}

        virtual bool hasStickyLeftDict() const override {
          return m_job->getDistributionMode() == 1 || m_job->getDistributionMode() == 2;
        }

        /**
         * @brief enum for distribution mode options readability
        */
        enum DistributionMode {
          FragmentOnServer = 0,
          FragmentOnHosts  = 1,
          FragmentByRules  = 2
        };

    private:
        /**
         * @brief Function to generate new CWorkunit for certain host for given time
         * @return True if workunit was generated successfully, False otherwise
         */
        bool generateWorkunit() override ;
};

#endif //WORKGENERATOR_ATTACKASSOC_H
