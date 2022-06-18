/**
 * @file Config.h
 * @brief Header file containing global settings
 * and support functions
 *
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_CONFIG_H
#define WORKGENERATOR_CONFIG_H


#include <Base64.h>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <cinttypes>
#include <fstream>

#include <boinc_db.h>
#include <backend_lib.h>

/** Ignore unused variables */
#ifndef UNUSED
#define UNUSED(expr) do { (void)(expr); } while (0)
#endif

using DbMap = std::map<std::string, std::string>;


/**
 * @brief Contains global settings
 */
namespace Config {
    /** Interface for creating safe dynamic objects*/
    template <typename T>
    using Ptr = std::shared_ptr<T>;

    /** Table names, for easy configuration */
    extern std::string tableNameBenchmark;
    extern std::string tableNameWorkunit;
    extern std::string tableNameJob;
    extern std::string tableNameHost;
    extern std::string tableNameHostActivity;
    extern std::string tableNameMask;
    extern std::string tableNameSettings;
    extern std::string tableNameDictionary;
    extern std::string tableNameJobDictionary;
    extern std::string tableNameHash;
    extern std::string tableNamePcfgGrammar;

    /** Path to dictionaries/markov/rules */
    extern std::string dictDir;
    extern std::string markovDir;
    extern std::string rulesDir;
    extern std::string pcfgDir;
    extern std::string projectDir;
    extern std::string charsetDir;

    /** Template names */
    extern std::string inTemplateFileBench;
    extern std::string inTemplateFileMask;
    extern std::string inTemplateFileMarkov;
    extern std::string inTemplateFileDict;
    extern std::string inTemplateFileDictAlt;
    extern std::string inTemplateFileCombinator;
    extern std::string inTemplateFileHybridDictMask;
    extern std::string inTemplateFileHybridMaskDict;
    extern std::string inTemplateFileRule;
    extern std::string inTemplateFileRuleAlt;
    extern std::string inTemplateFilePrince;
    extern std::string inTemplateFilePrinceRules;
    extern std::string inTemplateFilePcfg;
    extern std::string inTemplateFilePcfgRules;
    extern std::string outTemplateFile;

    /** Other parameters filled by BOINC arg parsing */
    extern char * inTemplatePathBench;
    extern char * inTemplatePathMask;
    extern char * inTemplatePathMarkov;
    extern char * inTemplatePathDict;
    extern char * inTemplatePathDictAlt;
    extern char * inTemplatePathCombinator;
    extern char * inTemplatePathHybridDictMask;
    extern char * inTemplatePathHybridMaskDict;
    extern char * inTemplatePathRule;
    extern char * inTemplatePathRuleAlt;
    extern char * inTemplatePathPrince;
    extern char * inTemplatePathPrinceRules;
    extern char * inTemplatePathPcfg;
    extern char * inTemplatePathPcfgRules;
    extern DB_APP * app;
    extern int startTime;
    extern int seqNo;
    extern char * appName;

    /** Host states */
    enum HostState {
        HostBench =  0,     /**< Host is performing benchmark */
        HostNormal = 1,     /**< Host is performing normal computation */
        HostDone =   3,     /**< Host computation is done */
        HostError =  4      /**< Host is not able to work */
    };

    /** Job states */
    enum JobState {
        JobReady      =  0,   /**< Job is not running */
        JobFinished   =  1,   /**< Password was found */
        JobExhausted  =  2,   /**< Keyspace was exhausted, pass not found */
        JobMalformed  =  3,   /**< Job has incorrect input */
        JobTimeout    =  4,   /**< time_end of the job was reached, cracking stopped */
        JobRunning    = 10,   /**< Job is running */
        JobFinishing  = 12    /**< Job is running, but all workunits has been generated, ending soon */
    };

    /** Attack modes in haschat*/
    enum AttackMode {
        AttackDict = 0,
        AttackCombinator = 1,
        AttackMask = 3,
        AttackHybridDictMask = 6,
        AttackHybridMaskDict = 7,
        AttackPrince = 8,
        AttackPcfg = 9
    };

    /** Types of debug output */
    enum DebugType {
        Log,
        Warn,
        Error
    };

    /** Quick binary2hex conversion */
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    /** Buffer size for SQL queries */
    const unsigned int SQL_BUF_SIZE = 4096;

    /** Maximum keyspace in PCFG Attack */
    const uint64_t MAX_PCFG_KEYSPACE = 60000000;

    /** Minimum timeout factor of seconds_per_workunit */
    const uint64_t minTimeoutFactor = 5;

    /** ID of bench_all job */
    const uint64_t benchAllId = 1;

    /** Workunit replication factor, must be 1 */
    const int replicationFactor = 1;

    /** Minimum passwords in attack */
    const uint64_t minPassCount = 10;
}


class Tools {
    public:
        /**
         * @brief Prints formatted string to stderr
         * @param format [in] List of arguments
         */
        static void printDebug(const char* format, ...);

        /**
         * @brief Prints formatted string to stderr, preceded by timestamp
         * @param format [in] List of arguments
         */
        static void printDebugTimestamp(const char* format, ...);

        /**
         * @brief Prints formatted string to stderr, with web-admin job tags
         * @param type [in] Type of message
         * @param jobId [in] ID of job
         * @param format [in] List of arguments
         */
        static void printDebugJob(Config::DebugType type, uint64_t jobId, const char *format, ...);

        /**
         * @brief Prints formatted string to stderr, with web-admin job+host tags
         * @param type [in] Type of message
         * @param jobId [in] ID of job
         * @param hostId [in] ID of boinc host
         * @param format [in] List of arguments
         */
        static void printDebugHost(Config::DebugType type, uint64_t jobId, uint64_t hostId, const char* format, ...);

        /**
         * Prints out contents of database map
         * @param map [in] Map to output
         */
        static void printDbMap(DbMap & map);

        /**
         * Opens the input stream or uses an opened one
         * @param jobId ID of the job
         * @param dictId ID of the dictionary
         * @return Input stream to read dictionary
         */
        static std::ifstream * getStream(uint64_t jobId, uint64_t dictId, std::string dictFileName);

        /**
         * @brief Releases the memory allocated for file descriptors
         */
        static void releaseFdMemory();

        /**
         * @brief Converts binary data to hex string
         * @param binaryData Binary data
         * @return Hex string
         */
        static std::string toHex(char *binaryData, int len);

    private:
        static std::map<std::pair<uint64_t, uint64_t>, std::ifstream* > m_fd_map;
};

#endif //WORKGENERATOR_CONFIG_H
