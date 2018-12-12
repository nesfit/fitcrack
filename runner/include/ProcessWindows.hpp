/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifdef _WIN32

#ifndef PROCESSWINDOWS_HPP 
#define PROCESSWINDOWS_HPP

#include <windows.h>

#include "PipeWindows.hpp"
#include "ProcessBase.hpp"

/** Class representing sub-process on Windows system */
class ProcessWindows : public ProcessBase {

    private:

	STARTUPINFO startup_info_;			/**< Windows STARTUPINFO structure */
	PROCESS_INFORMATION process_information_;	/**< Windows PROCESS_INFORMATION structure */
    protected:	

        /**
         * @brief   Contains steps leading to execution of executable with appropiate arguments.
         */
        void launchSubprocess();
    
    public:

	/**
	 * @brief   Conctutor which sets name of the executable
	 * @param   exec_name [in] Name of the executable
	 * @param   exec_args [in] Arguments which to pass to executable
	 */
	ProcessWindows(const std::string& exec_name, std::vector<char*>& exec_args);

	/**
	 * @brief Destructor closing the process and thread handles
	 */
	~ProcessWindows();

        /**
         * @brief   Converts status to exit code
         * @return  Exit code of child process 
         */
        int getExitCode() const;

        /**
         * @brief   Checks if process is still running
         *	    
         */
        bool isRunning();

	/**
	 * @brief   Launches the process
	 * @return  Return code of CreateProcess() 
	 */
	int run();

};		// end of class ProcessWindows
#endif // PROCESSWINDOWS_HPP

#endif // _WIN32
