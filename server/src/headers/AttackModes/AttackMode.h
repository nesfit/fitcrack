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
#include <Workunit.h>
#include <Host.h>
#include <Mask.h>
#include <Job.h>
#include <Dictionary.h>
#include <SqlLoader.h>
#include <Config.h>
#include <InputDict.h>
#include <MaskSplitter.h>

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
         * @brief Creates BOINC workunit, adds entry to fc_workunit
         * @return True if a workunit was planned, False otherwise
         */
        virtual bool makeWorkunit() = 0;

        /**
         * @brief Can be used to set workunit to attack mode from retry
         * Then, new workunit is NOT generated when makeWorkunit is called
         * @param workunit [in] Pointer to workunit created from retry
         */
        void setWorkunit(PtrWorkunit &workunit);

        /**
         * @brief Sets default parameters of BOINC workunit
         * @param wu [in/out] Pointer to the workunit
         */
        void setDefaultWorkunitParams(DB_WORKUNIT * wu);

        virtual bool requiresMasks() const {return false;}
        virtual bool requiresDicts() const {return false;}
        virtual bool masksUseRealKeyspace() const {return false;}

protected:
        /**
        * @brief Base constructor for all attacks, saves supplied arguments to member variables
        * @param job [in] Instance of CJob which is parent of this attack instance
        * @param host [in] Instance of CHost which this attack belongs to
        * @param seconds [in] Number of seconds this instance of attack should take
        */
        AttackMode(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
        :   m_job(job),
            m_host(host),
            m_seconds(seconds),
            m_sqlLoader(sqlLoader)
        {

        }

        /**
         * @brief Generates a basic config file for all attack_types
         * @param wuMode Mode of the WU (b/n/a)
         * @param attackMode Attack number
         * @param attackSubmode Attack submode
         * @param name Name of the Job
         * @param hashType Hashcat hash type
         * @param ruleLeft Left rule if present
         * @param ruleRight Right rule if present
         * @param charset1 Charset 1 if being used
         * @param charset2 Charset 2 if being used
         * @param charset3 Charset 3 if being used
         * @param charset4 Charset 4 if being used
         * @return Config string with newline at the end
         */
        virtual std::string generateBasicConfig(
            unsigned attackMode, unsigned attackSubmode, std::string name,
            unsigned hashType, unsigned generateRandomRules = 0,
            unsigned hwTempAbort = 90, bool optimized = true,
            std::string ruleLeft = "", std::string ruleRight = "",
            std::string charset1 = "", std::string charset2 = "",
            std::string charset3 = "", std::string charset4 = "");

        /**
         * @brief Get the Mode Letter. This is 'n' for normal attack modes.
         * 
         * @return char The mode letter
         */
        virtual char getModeLetter() {return 'n';}

        std::string makeConfigLine(const std::string &option, const std::string &type, const std::string &value);

        virtual std::string makeLimitingConfigLine(const std::string &option, const std::string &type, const std::string &value)
        {return makeConfigLine(option, type, value);}

        virtual std::unique_ptr<InputDict> makeInputDict(PtrDictionary dict, uint64_t startIndex, bool isSticky);

        virtual bool hasStickyLeftDict() const {return false;}

        virtual std::unique_ptr<MaskSplitter> makeMaskSplitter(std::vector<std::string> customCharsets = {});

        virtual PtrDictionary FindCurrentDict(std::vector<PtrDictionary> &dicts) const;

        virtual PtrDictionary GetWorkunitDict() const;

        virtual PtrMask GetWorkunitMask() const;

        virtual PtrMask FindCurrentMask(std::vector<PtrMask> &masks, bool useRealKeyspace) const;

        uint64_t getPasswordCountToProcess() const;

        virtual uint64_t getMinPassCount() const {return Config::minPassCount;}

        PtrWorkunit m_workunit; /**< Instance of CWorkunit which is used to create this attack instance */
        PtrJob m_job;           /**< Instance of CJob which is parent of this attack instance */
        PtrHost m_host;         /**< Instance of CHost which this attack belongs to */
        uint64_t m_seconds;     /**< Number of seconds this instance of attack should take */

        CSqlLoader * m_sqlLoader;

    private:

        /**
         * @brief Base function for generating CWorkunit object for certain attack, all attacks implement it
         * @return True if generating was successful, False otherwise
         */
        virtual bool generateWorkunit() = 0;
};

#endif //WORKGENERATOR_ATTACKMODE_H
