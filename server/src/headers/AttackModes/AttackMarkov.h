/**
 * @file AttackMarkov.h
 * @brief Header file for creation of Markov Attack
 * In hashcat terminology, a mask attack with .hcstat file
 *
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKMARKOV_H
#define WORKGENERATOR_ATTACKMARKOV_H

#include <AttackMode.h>


class CAttackMarkov : public AttackMode {
    public:
        /**
         * @brief Constructor for Combinator Attack
         * @param job [in] Instance of CJob which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackMarkov(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackMarkov() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_workunit
         * @return True if a workunit was planned, False otherwise
         */
        bool makeWorkunit() override ;

        virtual bool requiresMasks() const override {return true;}

    private:
        /**
         * @brief Function to generate new CWorkunit for certain host for given time
         * @return True if workunit was generated successfully, False otherwise
         */
        bool generateWorkunit() override ;
};

#endif //WORKGENERATOR_ATTACKMARKOV_H
