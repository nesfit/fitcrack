/**
 * @file SimpleGenerator.h
 * @brief Header file for simple generator
 * Keeps 2 generated workunits for each active host
 *
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_SIMPLEGENERATOR_H
#define WORKGENERATOR_SIMPLEGENERATOR_H

#include <AbstractGenerator.h>
#include <AttackMode.h>


class CSimpleGenerator : public CAbstractGenerator {
    public:
        /**
         * @brief Default constructor
         */
        CSimpleGenerator();

        /**
         * @brief Default destructor
         */
        ~CSimpleGenerator();

        /**
         * @brief Runs the SimpleGenerator infinite loop
         */
        void run() override ;

    private:

        CSqlLoader * m_sqlLoader;   /**< SqlLoader for database access */

        /**
         * @brief Creates Benchamrk workunit for supplied host
         * @param job [in] Parent job of this benchmark
         * @param host [in] Host to whom the benchmark is sent
         */
        void createBenchmark(PtrJob &job, PtrHost &host);

        /**
         * @brief Creates Regular cracking workunit for supplied host
         * @param job [in] Parent job of this workunit
         * @param host [in] Host to whom the cracking workunit is sent
         */
        void createRegularWorkunit(PtrJob &job, PtrHost &host);

        /**
         * @brief Set job to timeout/exhausted/pause status
         */
        void finishJob(PtrJob &job);

        /**
         * Set workunit to attack, copied from smallest corresponding workunit with retry set to 1
         * @param job [in] Job for which we are generating
         * @param host [in] Host for whom we are generating
         * @param attack [in/out] Attack to which we set the new workunit
         * @return True if workunit was copied from retry, False otherwise
         */
        bool setEasiestRetry(PtrJob &job, PtrHost &host, AttackMode *attack);

        /**
         * @brief Removes hosts of finished job and send them message to delete sticky files
         */
        void removeFinishedHosts();
};

#endif //WORKGENERATOR_SIMPLEGENERATOR_H
