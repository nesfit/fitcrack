/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifdef __linux__

#ifndef PROCESSLINUX_HPP 
#define PROCESSLINUX_HPP

#include "Logging.hpp"
#include "PipeLinux.hpp"
#include "ProcessBase.hpp"

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/** Class representing sub-process on Linux system */
class ProcessLinux : public ProcessBase {

    protected:	
        pid_t process_identifier_;	    /**< ProcessBase identifier */
        int status_;                    /**< Status of the process */

        /**
         * @brief   Function executing another binary in new process, predefined 
         *          to execute saved executable with arguments
         */
        void launchSubprocess();

    public:

        /**
         * @brief   Conctutor which sets name of the executable
         * @param   exec_name [in] Path to binary to execute
         * @param   exec_args [in] Arguments to pass when executing binary
         */
        ProcessLinux(const std::string& exec_name, std::vector<char* >& exec_args);

        /**
         * @brief   Destructor deleting pipes
         */
        ~ProcessLinux();

        /**
         * @brief   Getter on status member
         * @return  status_
         */
        int getStatus() const;

        /**
         * @brief   Converts status to exit code
         * @return  exit code of child process 
         */
        int getExitCode() const;

        /**
         * @brief   Overrides virtual getProcessInformation(), is usefull for determining
         *	        whether code is executed by parrent or child
         * @return  ProcessBase ID
         */
        int getProcessIdentifier() const;

        /**
         * @brief   Checks and saves status of the process
         * @return  True when process is still running, False otherwise
         */
        bool isRunning();

        /**
         * @brief   Spawns new process and execs command saved in executable
         *          with arguments set in constructor
         * @return  0 when proceess spawns correctly 
         */
        int run();
};		
#endif // PROCESSLINUX_HPP

#endif // __linux__
