/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskBenchmark.hpp"
/* Protected */

void TaskBenchmark::parseHashcatBenchmark(std::string& output_line) {
    size_t found_at;

    /** READ THIS
     * Detection of output lines with speeds since we cant get rid of lines
     * with times in hashcat output. For more more see example output bellow
     *  ------------------------------------------
     *  |1|   # hashcat (v3.6.0)                 |
     *  |2|   1:0:-1:-1:82.80:118271656          | <- line with the speed
     *  |3|   Started: Thu Jul 19 07:55:14 2018  |
     *  |4|   Stopped: Thu Jul 19 07:55:21 2018  |
     *  ------------------------------------------
     *
     * 2nd line is the speed of the device
     * Number before first double-dot represents ID of OCL device number and
     * theoretically it can be 1 - 12ish devices when we take in account
     * 8 GPUs and 4 CPUs which all can be the benchmarked devices. So we
     * need up to 2 characters to represent such number. 
     * 
     * Number after the last double-dot represents the speed of the device
     *
     * Other part represent hash_type, [temperature, fan_speed?], runtime
     * but we can ignore those.
     */

    Logging::debugPrint(Logging::Detail::CustomOutput, "Hashcat output line for parsing: " + output_line);

    found_at = output_line.find(":");
    if (found_at > 0 && found_at <= 2) {
        found_at = output_line.rfind(":") + 1;

        std::string speed = output_line.substr(found_at, output_line.length() - found_at);
	Logging::debugPrint(Logging::Detail::DevelDebug, "parsed speed: " + speed + " from line: " + output_line);
        speeds_.push_back(RunnerUtils::stoull(speed));
    }

}

/* Public */ 

TaskBenchmark::TaskBenchmark (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) : TaskComputeBase (directory, task_config, host_config, output_file, workunit_name)  {
    mode_ = "b";  
    initializeTotalHashes();
}

TaskBenchmark::TaskBenchmark (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name) : TaskComputeBase (directory, task_config, host_config, output_file, workunit_name)  {
    mode_ = "b";  
    initializeTotalHashes();
}

std::string TaskBenchmark::generateOutputMessage() {

    std::string output_message = "";

    exit_code_ = process_->getExitCode();

    output_message += mode_ + "\n"; 

    if (exit_code_ == HashcatConstant::Succeded) {

	output_message += ProjectConstants::TaskFinalStatus::Succeded + "\n";
	output_message += RunnerUtils::toString(getTotalSpeed()) + "\n";
	output_message += RunnerUtils::toString(process_->getExecutionTime()) + "\n";

    } else if (exit_code_ != HashcatConstant::Succeded) {

	output_message += ProjectConstants::TaskFinalStatus::Error + "\n";
	output_message += RunnerUtils::toString(process_->getExitCode()) + "\n";
	output_message += getErrorMessage() + "\n"; 

    }

    return output_message;
}

unsigned long long TaskBenchmark::getTotalSpeed() {
    unsigned long long speed_sum = 0;

    /** Sum speeds of all OCL devices used by hashcat on given node */
    for (std::vector<unsigned long long>::iterator it = speeds_.begin(); it != speeds_.end(); it++) {
        speed_sum += *it;
    }

    return speed_sum;
}

void TaskBenchmark::initializeTotalHashes() {
    total_hashes_ = 1;
}

bool TaskBenchmark::parseHashcatOutputLine(std::string& output_line) {
    parseHashcatBenchmark(output_line);
}

void TaskBenchmark::progress() {
    std::string lines;
    std::string line;
    while (!(lines = process_->readOutPipeAvailableLines()).empty()) {
	size_t found_at = 0, last = 0;
	for (found_at = 0; found_at != std::string::npos; found_at = lines.find("\n", last)) {
	    line = lines.substr(last, found_at - last);

	    parseHashcatOutputLine(line);

	    last = found_at +1;
	}
	actualizeComputedHashes(1);
    }
}
