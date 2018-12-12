/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifdef __linux__

#include "ProcessLinux.hpp"

/* Private */

void ProcessLinux::launchSubprocess() {

    /** Execute application - print it before redirection so that it goes to
     * stderr.txt */
    Logging::debugPrint(Logging::Detail::GeneralInfo, "Executing: " + getReadableArguments());

    /** Child doesn't read from the pipes */
    out_pipe_->closeRead();
    err_pipe_->closeRead();

    /** Redirect childs stdout and stderr to the pipes */
    reinterpret_cast<PipeLinux*>(out_pipe_)->redirectWrite(fileno(stdout));
    reinterpret_cast<PipeLinux*>(err_pipe_)->redirectWrite(fileno(stderr));

    /** Execute application */
    execv(arguments_[0], &arguments_[0]);

    /** This shouldn't be ever executed but if it does, throw error exception */
    RunnerUtils::runtimeException("execv() failed with", errno);
}

/* Public */

ProcessLinux::ProcessLinux(const std::string& exec_name, std::vector<char* >& exec_args) : ProcessBase(exec_name, exec_args) {
    out_pipe_ = new PipeLinux;
    err_pipe_ = new PipeLinux;
}

ProcessLinux::~ProcessLinux() {
    delete out_pipe_;
    delete err_pipe_;
}

int ProcessLinux::getExitCode() const {
    return WEXITSTATUS(status_);
}

int ProcessLinux::getProcessIdentifier() const {
    return process_identifier_;
}

bool ProcessLinux::isRunning() {
    /** Waitpid with WNOHANG returns 0 when process still exists */
    return (waitpid(process_identifier_, &status_, WNOHANG) == 0);
}

int ProcessLinux::run() {
    setStartTime();
    process_identifier_ = fork();

    if (getProcessIdentifier() == -1) {
        RunnerUtils::runtimeException("fork() failed with", errno);
    } else if (process_identifier_ == 0){
        try {

            launchSubprocess();

        } catch (std::exception& e) {
            std::cerr << "Child process failed with std::runtime_error:\n what() : " << e.what() << std::endl;
            /** Exit child process on any exception, don't let child exception to be
             *  caught by the code that should be executed only by parent 
             */
            exit(errno);
        }
    } else {

        /** Parrent doesn't write to pipes */
        out_pipe_->closeWrite();
        err_pipe_->closeWrite();
    }

    return 0;
}

#endif // __linux__
