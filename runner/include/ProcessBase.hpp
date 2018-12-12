/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef PROCESSBASE_HPP 
#define PROCESSBASE_HPP

#include "PipeBase.hpp"
#include "RunnerUtils.hpp"

#include <iostream>
#include <vector>

/**
 * This is an abstract class to be inhereted from for system specific
 * process implementation ie. Linux, Windows and other
 */
class ProcessBase {

    protected:	

        PipeBase *out_pipe_; /**< Stdout pipe */
        PipeBase *err_pipe_; /**< Stderr pipe */

        unsigned long long start_time_;      /**< Start time of process */
        unsigned long long stop_time_;       /**< End time of process */

        std::string executable_;    /**< Path to executable or shell command */

        std::vector<char*> arguments_; /**< Arguments to pass to executable */

        /**
         * @brief   Pure virtual function containing steps leading to execution
         *	        of executable with appropiate arguments.
         */
        virtual void launchSubprocess() = 0;

        /**
         * @brief   Sets start_time_ with now timestamp
         */
        void setStartTime();

        /**
         * @brief   Sets stop_time_ with now timestamp
         */
        void setStopTime();

        /**
         * @brief   Reads all lines from the pipe
         * @param   pipe [in] Pipe to read from
         * @return  All lines written to the pipe 
         */
        std::string readPipeAvailableLines(PipeBase* pipe);

    public:

        /**
         * @brief   Conctutor which sets name of the executable
         * @param   exec_name [in] Path to the executable / shell command
         * @param   exec_args [in] Arguments of the executable / shell command
         *          to be passed to it
         */
        ProcessBase(const std::string& exec_name, std::vector<char* >& exec_args);

        /**
         * @brief   Pure virtual function converting status to exit code
         * @return  Exit code of child process 
         */
        virtual int getExitCode() const = 0;

        /**
         * @brief   Makes readable string of command to be executed out of arguments_ vector
         * @return  Excutable command 
         */
        std::string getReadableArguments();

        /**
         * @brief   Reads all lines unread available and from the stdout pipe of the process
         * @return  All unread lines written to stdout 
         */
        std::string readErrPipeAvailableLines();

        /**
         * @brief   Reads a line from stderr of process
         * @return  Line from the stderr 
         */
        std::string readErrPipeLine();

        /**
         * @brief   Reads all lines unread available and from the stdout pipe of the process
         * @return  All unread lines written to stdout 
         */
        std::string readOutPipeAvailableLines();

        /**
         * @brief   Reads a line from stdout of process
         * @return  Line from the stdout 
         */
        std::string readOutPipeLine();

        /**
         * @brief   Computes run time of the launched process after it has
         *	    finished
         * @return  Seconds elapsed 
         */
        double getExecutionTime() const;

        /**
         * @brief   Sets end clock and retrievs exit code
         * @return  Exit code
         */
        int finish();

        /**
         * @brief   Pure virtual function checking if process is still running
         *	    
         */
        virtual bool isRunning() = 0;

        /**
         * @brief   Sets executable which will be executed by the process 
         * @param   exec_name [in] Path to the executable / shell command
         */
        void setExecutable(const std::string& exec_name);

        /**
         * @brief   Pure virtual function launching process and filling star time
         */
        virtual int run() = 0;

};  // end of class ProcessBase

#endif // PROCESSBASE_HPP
