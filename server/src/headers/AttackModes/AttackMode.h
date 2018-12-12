/**
 * @file AttackMode.h
 * @brief Abstract base class for all Attack Modes
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKMODE_H
#define WORKGENERATOR_ATTACKMODE_H

/** Project headers */
#include <Job.h>
#include <Host.h>
#include <Mask.h>
#include <Package.h>
#include <Dictionary.h>
#include <SqlLoader.h>
#include <Config.h>

/** BOINC headers */
#include <backend_lib.h>
#include <sched_util.h>
#include <str_util.h>
#include <str_replace.h>

/** System headers*/
#include <fstream>


class AttackMode {
    public:

        /**
         * @brief Default destructor
         */
        virtual ~AttackMode() = default;

        /**
         * @brief Creates BOINC workunit, adds entry to fc_job
         * @return True if a job was planned, False otherwise
         */
        virtual bool makeJob() = 0;

        /**
         * @brief Can be used to set job to attack mode from retry
         * Then, new job is NOT generated when makeJob is called
         * @param job [in] Pointer to job created from retry
         */
        void setJob(PtrJob & job);

        /**
         * @brief Sets default parameters of BOINC workunit
         * @param wu [in/out] Pointer to the workunit
         */
        void setDefaultWorkunitParams(DB_WORKUNIT * wu);

protected:
        /**
        * @brief Base constructor for all attacks, saves supplied arguments to member variables
        * @param package [in] Instance of CPackage which is parent of this attack instance
        * @param host [in] Instance of CHost which this attack belongs to
        * @param seconds [in] Number of seconds this instance of attack should take
        */
        AttackMode(PtrPackage & package, PtrHost & host, uint64_t seconds, CSqlLoader * sqlLoader)
        :   m_package(package),
            m_host(host),
            m_seconds(seconds),
            m_sqlLoader(sqlLoader)
        {

        }


        PtrJob m_job;           /**< Instance of CJob which is used to create this attack instance */
        PtrPackage m_package;   /**< Instance of CPackage which is parent of this attack instance */
        PtrHost m_host;         /**< Instance of CHost which this attack belongs to */
        uint64_t m_seconds;     /**< Number of seconds this instance of attack should take */

        CSqlLoader * m_sqlLoader;

    private:

        /**
         * @brief Base function for generating CJob object for certain attack, all attacks implement it
         * @return True if generating was successful, False otherwise
         */
        virtual bool generateJob() = 0;
};

#endif //WORKGENERATOR_ATTACKMODE_H
