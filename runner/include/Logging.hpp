/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "RunnerUtils.hpp"
#include <iostream>
#include <string>

#define PRINT_POSITION_IN_CODE() Logging::positionInCode(__FILE__, __LINE__)

#define POSITION_IN_CODE std::string(__FILE__) + " : " + RunnerUtils::toString(__LINE__) + " : " + __FUNCTION__ 

/**
 * @brief   Prints debugging output on stderr
 * @param   debug_level [in] Sets level of given message
 * @param   message [in] Message to output
 */
#define debugPrint(detail, str) log(detail, std::string(__PRETTY_FUNCTION__), str)

/** Namespace with functions for logging and priority list */
namespace Logging {

    /** Namespace which hacks C++98 lacking support for scoped enums */
    /** @brief Enum items are place directly into the Detail namespace, the
     * namespace provides the needed scope which enums lack in C++ till C++0x
     */

    /** Adds scope to level and tag */
    namespace Detail {
	/** Enum with logging levels */
	enum level {		    
	    Important = 0,		/**< Exception message */
	    GeneralInfo = 1,		/**< BOINC /standalone, result, exit message etc. */
	    ObjectContentRevision = 2,  /**< Contents of objects with collections */
	    ObjectManipulation = 3,     /**< Adding to collections, setting paths etc */	
	    CustomOutput = 4,		/**< Any output in the code */
	    DevelDebug = 5		/**< Development debug msg like entering of functions, lengths and etc */
	};  

	const std::string tag[] = {
	    "IMPORTANT",	    // level::Important
	    "  INFO   ",   	    // level::GeneralInfo
	    " CONTENT ",   	    // level::ObjectContentRevision
	    " CHANGE  ",   	    // level::ObjectManipulation
	    " CUSTOM  ",   	    // level::CustomOutput
	    " DEVEL   "    	    // level::DevelDebug
	};  /**< Logging prefix tag corresponding to Detail::level */
    };

    extern unsigned char debug_detail;	    /**< Sets logging depth */
    extern bool position_in_code_enabled;   /**< Enables logging of positionInCode */

    // taken from https://stackoverflow.com/questions/1666802/is-there-a-class-macro-in-c/15775519#15775519
    /**
     * @brief   Takes __PRETTY_FUNCTION__ and parses class name from it
     * @param	pretty_function [in] __PRETTY_FUCTION__ can't be set as default!
     * @return  Name of the clast
     */
    std::string parseClassName(const std::string& pretty_function);

    /**
     * @brief   Prints debugging output on stderr
     * @param	debug_level [in] Sets level of given message
     * @param	class_name [in] Name of the class from which call made
     * @param	message [in] Message to output
     */
    void log(const int debug_level, const std::string& class_name, const std::string message);

    /**
     * @brief   Prints __FILE__, __LINE__ in one format in whole project
     * @param	file [in] Name of the file with the code
     * @param 	line [in] Line in the file 
     */
    void positionInCode(const std::string file, const int line);

//    /**
//     * @brief	Sets std::cerr to be in buffered mode to increase performance
//     */
//    void setBuffered();
//
//    /**
//     * @brief   Sets std::cerr to be in unbuffed mode with flushing in write
//     */
//    void setUnbuffered();

}
#endif // LOGGING_HPP
