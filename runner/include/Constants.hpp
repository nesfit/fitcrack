/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef CONSTANTS_HPP 
#define CONSTANTS_HPP

#include "boinc_api.h"

/** Namespace representing constants for hashcat arguments */
namespace HashcatConstant {
    const int OutputFormat	= 5;		    /**< Hashcat's output format (5 is HASH:SALT:HEX_PASSWORD) */
    const int ProgressPeriod	= 10;		    /**< Period of Hashcat's --status-timer */

    const std::string MaskFileName  = "mask.hcmask";	/**< Name of the file with charsets and mask passed to hashcat */
    const std::string OutputFile    = "passwords.txt";	/**< Name of the Hashcat's file with the found passwords */

    // Created from
    // https://github.com/hashcat/hashcat/blob/master/docs/status_codes.txt
    /** Enum of all hashcat exit/status codes */
    enum status_codes {
	GpuAlarm	= -2,	    /**< Gpu-watchdog alarm */
	Error	    	= -1, 	    /**< Error */
	Succeded    	=  0, 	    /**< OK/cracked */
	Exhausted   	=  1, 	    /**< Exhausted */
	Aborted	    	=  2, 	    /**< Aborted */
	CheckpointAbort	=  3, 	    /**< Aborted by checkpoint */
	RuntimeAbort	=  4 	    /**< Aborted by runtime */
    };
};

/** Namespace representing Boinc constants*/
namespace BoincConstants {
    extern std::string ProjectName;			/**< Name of the BOINC project, set at run-time */
    extern std::string TrickleDeamonName;	    	/**< Name of the BOINC trickler, set at run-time */ 
    const  std::string ResultFileName = "out";	    	/**< Name of the BOINC result file */
    const  std::string InitDataFile = INIT_DATA_FILE;	/**< Name of the BOINC init data XML created by BOINC client */

    /**
     * @brief   Parses project name from workunit name
     * @param	workunit_name [in] Name of the BOINC workunit
     */
    void setProjectName(const std::string& workunit_name);
};

/** Namespace representing ProjectConstants */
namespace ProjectConstants {

    /** Sub-Namespace for additional scope */
    namespace TaskFinalStatus {
	const std::string Succeded	= "0";	/**< Password found or benchmark successful */
	const std::string Exhausted	= "1";	/**< Whole keyspace exhausted and password not found */
	const std::string Error		= "3";	/**< Hashcat or something else failed */
    };
};
#endif // CONSTANTS_HPP
