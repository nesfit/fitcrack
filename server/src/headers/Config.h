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
    extern std::string tableNamePackage;
    extern std::string tableNameHost;
    extern std::string tableNameHostActivity;
    extern std::string tableNameMask;
    extern std::string tableNameSettings;
    extern std::string tableNameDictionary;
    extern std::string tableNamePckgDictionary;
    extern std::string tableNameHash;

    /** Path to dictionaries/markov/rules */
    extern std::string dictDir;
    extern std::string markovDir;
    extern std::string rulesDir;
    extern std::string projectDir;

    /** Template names */
    extern std::string inTemplateFileBench;
    extern std::string inTemplateFileMask;
    extern std::string inTemplateFileMarkov;
    extern std::string inTemplateFileDict;
    extern std::string inTemplateFileCombinator;
    extern std::string inTemplateFileRule;
    extern std::string outTemplateFile;

    /** Other parameters filled by BOINC arg parsing */
    extern char * inTemplatePathBench;
    extern char * inTemplatePathMask;
    extern char * inTemplatePathMarkov;
    extern char * inTemplatePathDict;
    extern char * inTemplatePathCombinator;
    extern char * inTemplatePathRule;
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

    /** Package states */
    enum PackageState {
        PackageReady      =  0,   /**< Package is not running */
        PackageFinished   =  1,   /**< Password was found */
        PackageExhausted  =  2,   /**< Keyspace was exhausted, pass not found */
        PackageMalformed  =  3,   /**< Package has incorrect input */
        PackageTimeout    =  4,   /**< time_end of the package was reached, cracking stopped */
        PackageRunning    = 10,   /**< Package is running */
        PackageFinishing  = 12    /**< Package is running, but all jobs has been generated, ending soon */
    };

    /** Attack modes in haschat*/
    enum AttackMode {
        AttackDict = 0,
        AttackCombinator = 1,
        AttackMask = 3
    };

    /** Types of debug output */
    enum DebugType {
        Log,
        Warn,
        Error
    };

    /** Buffer size for SQL queries */
    const unsigned int SQL_BUF_SIZE = 4096;

    /** Minimum job time in seconds */
    const uint64_t minSeconds = 60;

    /** Minimum timeout factor of seconds_per_job */
    const uint64_t minTimeoutFactor = 5;

    /** ID of bench_all package */
    const uint64_t benchAllId = 1;

    /** Job replication factor, must be 1 */
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
         * @brief Prints formatted string to stderr, with web-admin package tags
         * @param type [in] Type of message
         * @param packageId [in] ID of package
         * @param format [in] List of arguments
         */
        static void printDebugPackage(Config::DebugType type, uint64_t packageId, const char* format, ...);

        /**
         * @brief Prints formatted string to stderr, with web-admin package+host tags
         * @param type [in] Type of message
         * @param packageId [in] ID of package
         * @param hostId [in] ID of boinc host
         * @param format [in] List of arguments
         */
        static void printDebugHost(Config::DebugType type, uint64_t packageId, uint64_t hostId, const char* format, ...);

        /**
         * Prints out contents of database map
         * @param map [in] Map to output
         */
        static void printDbMap(DbMap & map);

        /**
         * Opens the input stream or uses an opened one
         * @param packageId ID of the package
         * @param dictId ID of the dictionary
         * @return Input stream to read dictionary
         */
        static std::ifstream * getStream(uint64_t packageId, uint64_t dictId, std::string dictFileName);

        /**
         * @brief Releases the memory allocated for file descriptors
         */
        static void releaseFdMemory();

    private:
        static std::map<std::pair<uint64_t, uint64_t>, std::ifstream* > m_fd_map;
};

#endif //WORKGENERATOR_CONFIG_H
