/**
 * @file AttackPcfg.h
 * @brief Header file for creation of Pcfg Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 10. 7. 2019
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKPCFG_H
#define WORKGENERATOR_ATTACKPCFG_H

#include <vector>

#include <AttackMode.h>
#include <AttackPcfgClient.h>


class CAttackPcfg : public AttackMode {
    public:
        /**
         * @brief Constructor for PCFG Attack
         * @param package [in] Instance of CPackage which is parent of this attack instance
         * @param host [in] Instance of CHost which this attack belongs to
         * @param seconds [in] Number of seconds this instance of attack should take
         */
        CAttackPcfg(PtrPackage & package, PtrHost & host, uint64_t seconds, CSqlLoader * sqlLoader);

        /**
         * @brief Default destructor
         */
        ~CAttackPcfg() override = default;

        /**
         * @brief Creates BOINC workunit, adds entry to en_workunit
         * @return True if a workunit was planned, False otherwise
         */
        bool makeJob() override ;

    private:
        /**
         * @brief Function to generate new CWorkunit for certain host for given time
         * @return True if workunit was generated successfully, False otherwise
         */
        bool generateJob() override ;

        /**
         * @brief Calls gRPC request to PCFG Manager which asks for new preterminals
         * @param preterminals List of preterminals as a single string
         * @param realKeyspace Number of passwords that are represented by those preterminals
         */
        void loadNextPreterminals(std::string & preterminals, uint64_t & realKeyspace);


        /** gRPC channel */
        PretermClient m_client;
};

#endif //WORKGENERATOR_ATTACKPCFG_H
