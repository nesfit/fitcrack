/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef RUNNERUTILS_HPP 
#define RUNNERUTILS_HPP

#include "nullptr.hpp"

#include "boinc_api.h"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <sys/time.h>

#ifdef _WIN32
  #include <windows.h>
#endif

#define TOCSTRING(x) (const_cast<char*>(x))

/** Namespace with various usefull functions */
namespace RunnerUtils {

    /**
     * @brief   Set standalone flag, process priority and etc.
     */
    void initializeBoinc(BOINC_OPTIONS& options);

    /**
     * @brief   Checks whether the application runs as standalone or is
     *          launched by BOINC-client
     * @return  True on standalone, False when launched 
     */
    bool isStandalone();

    /**
     * @brief   String to unsigned long conversion, C++98 doesn't have it in the std namespace
     * @param   number [in] String number to be converted
     * @param   idx [in] Size of the string in C way, nullptr by default
     * @param   base [in] Numeric base for convertion, 10 by default
     * @return  Converted unsigned long number 
     */
    unsigned long stoul(const std::string& number, size_t *idx = nullptr, int base = 10);

    /**
     * @brief   String to unsigned long long conversion, C++98 doesn't have it in the std namespace
     * @param   number [in] String number to be converted
     * @param   idx [in] Size of the string in C way, nullptr by default
     * @param   base [in] Numeric base for convertion, 10 by default
     * @return  Converted unsigned long number 
     */
    unsigned long long stoull(const std::string& number, size_t *idx = nullptr, int base = 10);

    /**
     * @brief   Fills struct timeval and converts it to milliseconds
     * @return  Current time as milliseconds
     */
    unsigned long long timeInMs();

    /**
     * @brief   Number to string convertion
     * @param   number [in] Number to convert in some supported type
     * @return  String value of converted number
     */
    template<typename T>
	std::string toString(const T& number)
	{
	    std::stringstream out;
	    out << number;
	    return out.str();
	}


    // Has to be in header file because the inline specification, so that it
    // is present at compilation of every that uses this header
    /**
     * @brief   Throws runtime exception with given string as exception message
     * @param   message [in] String saved to exception
     */
    inline void runtimeException(const std::string& message) {
	throw std::runtime_error(message);
    }

    // Has to be in header file because the inline specification, so that it
    // is present at compilation of every that uses this header
    /**
     * @brief   Inline function to place valid __FUNCTION__ and __LINE__ to exception without call/jump to another function with would change tho macros
     * @param   message [in] String saved to exception
     * @param   error_number [in] Error number (C errno)
     */
    inline void runtimeException(const std::string& message, const int error_number) {
	throw std::runtime_error(message + " : error #." + toString(error_number) + " => " + std::strerror(error_number));
    }

#ifdef _WIN32
    /**
     * @brief   Inline function to place valid __FUNCTION__ and __LINE__ to exception without call/jump to another function with would change tho macros
     * @param   message [in] String saved to exception
     * @param   error_number [in] Error number (C errno)
     */
    inline void runtimeException(const std::string& message, const DWORD error_number) {
	char error_message_buffer[1024];
	std::ostringstream error_sstream;
	
	/* 1033 constant sets US english error message */
	if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error_number, 1033, error_message_buffer, 1024, 0)) {
	    error_sstream << "FormatMessage() failed with error #." << GetLastError();
	} else {
	    error_sstream << message + " : error #." << error_number << " => " << toString(error_message_buffer);
	}

	throw std::runtime_error(error_sstream.str());
    }
#endif

};

#endif // RUNNERUILS_HPP
