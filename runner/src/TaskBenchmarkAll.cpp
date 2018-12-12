/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskBenchmarkAll.hpp" 
/* Public */ 
TaskBenchmarkAll::TaskBenchmarkAll (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) : 
    TaskBase(directory, task_config, host_config, output_file, workunit_name) { 
    mode_ = "a";
    }

std::string TaskBenchmarkAll::generateOutputMessage() {

    std::string output_message = "";
    std::string hashtypes_speeds = "";

    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "last benchmark exit code is " + RunnerUtils::toString(exit_code_));

    output_message += mode_ + "\n"; 

    double time_sum = 0;

    if (exit_code_ == HashcatConstant::Succeded) {

	output_message += ProjectConstants::TaskFinalStatus::Succeded + "\n";

	for (size_t i = 0; i < hashtypes_.size(); i++) {
	    hashtypes_speeds += hashtypes_[i] + ":" + RunnerUtils::toString(benchmark_tasks_[i].getTotalSpeed()) + "\n";
	    time_sum += benchmark_tasks_[i].getRunTime();
	}

	output_message += RunnerUtils::toString(time_sum) + "\n";
	output_message += hashtypes_speeds;

    } else {

	output_message += ProjectConstants::TaskFinalStatus::Error + "\n";
	output_message += RunnerUtils::toString(exit_code_) + "\n";
	output_message += benchmark_tasks_.back().getErrorMessage() + "\n"; 
	output_message += ProjectConstants::TaskFinalStatus::Error + "\n";

	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "last benchmark exit code is " + RunnerUtils::toString(exit_code_));

    }

   return output_message;
}

void TaskBenchmarkAll::initializeTotalHashes() {
    total_hashes_ = benchmark_tasks_.size();
}

void TaskBenchmarkAll::initialize() {

    std::fstream hashtypes_list_stream;
    File hashtypes_list_file;
    std::string hashtype;

    directory_.findVersionedFile("formats", "list", hashtypes_list_file);
    File::openReadStream(hashtypes_list_stream, hashtypes_list_file.getRelativePath());

    while (!hashtypes_list_stream.eof()) {
	File::readLine(hashtypes_list_stream, hashtype);

	if (!hashtype.empty()) {

	    hashtypes_.push_back(std::string(hashtype));

	    ConfigTask benchmark_config = task_config_;
	    benchmark_config.add("hash_type", hashtype);

	    benchmark_tasks_.push_back(TaskBenchmark(directory_, benchmark_config, host_config_, output_file_, workunit_name_));
	}
    }

    initializeTotalHashes();
}

void TaskBenchmarkAll::startComputation() {
    for (std::vector<TaskBenchmark>::iterator it = benchmark_tasks_.begin(); it != benchmark_tasks_.end(); it++) {
	it->initialize();
	it->startComputation();
	it->progress();
	it->finish();
	Logging::debugPrint(Logging::Detail::GeneralInfo, 
		"TaskBenchmarkAll : sub-result : " + RunnerUtils::toString(hashtypes_[computed_hashes_])
		+ " : " + RunnerUtils::toString(it->getTotalSpeed()));
	actualizeComputedHashes(1);
	reportProgress();
    }
}

int TaskBenchmarkAll::finish() {
    exit_code_ = benchmark_tasks_.back().finish();
    return exit_code_;
}

void TaskBenchmarkAll::progress() {
// TODO: implement and use or think how to report progress on benchmark all now
// it is in the starComputation

}

void TaskBenchmarkAll::printHashTypes() {
    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "hashes for benchmarking :");

    for (std::vector<std::string>::iterator it = hashtypes_.begin(); it != hashtypes_.end(); it++) {
	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "\t'" + *it + "'");
    }

    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "------------------------------------");
}

