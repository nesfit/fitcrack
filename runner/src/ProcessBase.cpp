/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "ProcessBase.hpp"

ProcessBase::ProcessBase(const std::string& exec_name, std::vector<char*>& exec_args) :
    start_time_(0), stop_time_(0) { 

	setExecutable(exec_name);

        /** Push name of the executable to the start of the vector and as starting value of string with the arguments of executable */
        arguments_.push_back(TOCSTRING(executable_.c_str()));

	Logging::debugPrint(Logging::Detail::ObjectManipulation, "Process arguments: ");
        for (std::vector<char*>::iterator it = exec_args.begin(); it != exec_args.end(); it++) {
            arguments_.push_back(*it);

	    Logging::debugPrint(Logging::Detail::ObjectManipulation,  + "\t argument '" + std::string(*it) + "'");
        }
	Logging::debugPrint(Logging::Detail::ObjectManipulation, "-------------------------------");

        // WARNING: you can't construct std::string with NULL
        arguments_.push_back(NULL);
    }

std::string ProcessBase::getReadableArguments() {
    std::string readable_arguments;

    readable_arguments = std::string(*arguments_.begin());

    for (std::vector<char*>::iterator it = arguments_.begin() + 1; it != arguments_.end(); it++) {
        if (*it != NULL) {
            std::string s(*it);
            readable_arguments += " " + s;
        }
    }
    return readable_arguments;

}

void ProcessBase::setStartTime() {
    start_time_ = RunnerUtils::timeInMs();

    Logging::debugPrint(Logging::Detail::ObjectManipulation, "Process set start time : " + RunnerUtils::toString(start_time_));
}

void ProcessBase::setStopTime() {
    stop_time_ = RunnerUtils::timeInMs();

    Logging::debugPrint(Logging::Detail::ObjectManipulation, "Process set stop time : " + RunnerUtils::toString(stop_time_));
}

std::string ProcessBase::readPipeAvailableLines(PipeBase* pipe) {
    std::string content = "";
    while (pipe->canRead()) {
        content += pipe->readLine();
	Logging::debugPrint(Logging::Detail::DevelDebug, " Temp Available content: " + content);
    }
    Logging::debugPrint(Logging::Detail::DevelDebug, " Final Available content: " + content);

    return content;
}

/* Public */

int ProcessBase::finish() {
    while (isRunning());
    setStopTime(); 
    return getExitCode();
}

double ProcessBase::getExecutionTime() const {
    /** When stop_time_ is set use step time else use current_time */
    unsigned long long end_time = (stop_time_ > 0 ? stop_time_ : RunnerUtils::timeInMs());

    /** Both are of type unsigned long long and in milliseconds so we can just substract */
    unsigned long long elapsed_time = end_time - start_time_;

    double elapsed_time_seconds = elapsed_time / 1000.0;

    Logging::debugPrint(Logging::Detail::GeneralInfo, "Process execution time : " + RunnerUtils::toString(elapsed_time_seconds));

    return elapsed_time_seconds;
}

std::string ProcessBase::readErrPipeAvailableLines() {
    return readPipeAvailableLines(err_pipe_);
}

std::string ProcessBase::readErrPipeLine() {
    if (err_pipe_ != nullptr)
        return err_pipe_->readLine();
    RunnerUtils::runtimeException("err_pipe_ is nullptr. Can't read.");
    return "";
}

std::string ProcessBase::readOutPipeAvailableLines() {
    return readPipeAvailableLines(out_pipe_);
}

std::string ProcessBase::readOutPipeLine() {
    if (err_pipe_ != nullptr)
        return out_pipe_->readLine();
    RunnerUtils::runtimeException("out_pipe_ is nullptr. Can't read.");
    return "";
}

void ProcessBase::setExecutable(const std::string& exec_name) {
    executable_ = exec_name;

    Logging::debugPrint(Logging::Detail::ObjectManipulation, "Changed Process executable to: " + executable_);
}
