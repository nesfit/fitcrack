/**
 * @file AttackPrince.h
 * @brief Header file for creation of Prince Attack
 * @authors David Bolvansky
 * @date 22. 2. 2020
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKPRINCE_H
#define WORKGENERATOR_ATTACKPRINCE_H

#include <AttackMode.h>


class CAttackPrince : public AttackMode {
    public:
        /**
         * @brief Constructor for Prince Attack
         * @param job [in] Instance of CJob which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackPrince(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackPrince() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_workunit
         * @return True if a workunit was planned, False otherwise
         */
        bool makeWorkunit() override ;

    private:
        /**
         * @brief Function to generate new CWorkunit for certain host for given time
         * @return True if workunit was generated successfully, False otherwise
         */
        bool generateWorkunit() override ;
};

#endif //WORKGENERATOR_ATTACKPRINCE_H
