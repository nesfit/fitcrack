/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskBase.hpp"

/* Protected */

void TaskBase::actualizeComputedHashes(const unsigned long long newly_computed_hashes) {
    computed_hashes_ += newly_computed_hashes;
}

/* Public */

TaskBase::TaskBase (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) : directory_(directory), task_config_(task_config), host_config_(host_config), output_file_(output_file), computed_hashes_(0), total_hashes_(0), workunit_name_(workunit_name) {  
}

TaskBase::TaskBase (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name) : directory_(directory), task_config_(task_config), host_config_(host_config), output_file_(output_file), computed_hashes_(0), total_hashes_(0), workunit_name_(workunit_name) {  
}

double TaskBase::fractionDone() {
    if (computed_hashes_ > total_hashes_)
	return 0.0;
        // RunnerUtils::runtimeException("portion_done is bigger than total");
    double fraction_done = computed_hashes_ / (double)total_hashes_;
    return (fraction_done != fraction_done ? 0.0 : fraction_done); // NaN test
}

void TaskBase::writeOutputFile(std::string& output_message){
    std::fstream output_stream;
    File result_file;

    PRINT_POSITION_IN_CODE();

    if (!directory_.find(BoincConstants::ResultFileName, result_file)) {
	result_file.changeTo(BoincConstants::ResultFileName);
    }

    Logging::debugPrint(Logging::Detail::GeneralInfo, "Result_file : " + result_file.getRelativePath());

    File::openWriteStream(output_stream, result_file.getRelativePath(), std::fstream::trunc);
    output_stream.write(output_message.c_str(), output_message.length());

    PRINT_POSITION_IN_CODE();

    output_stream.close();
}

void TaskBase::reportProgress() {
    double fraction_done, percent_done;
    fraction_done = fractionDone();
    percent_done = fraction_done * 100.0;
    
    Logging::debugPrint(Logging::Detail::DevelDebug, "Progress : " + RunnerUtils::toString(fraction_done) + " percent done : " + RunnerUtils::toString(percent_done) + " rinted : " + RunnerUtils::toString(::rint(percent_done)));

    // Add call of the trickler progress message
    if (!boinc_is_standalone()) {
	std::string trickle_message = "<workunit_name>"+ workunit_name_ + "</workunit_name>\n<progress>" + RunnerUtils::toString(percent_done) + "</progress>";

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings" // disable G++ warning about deprecated cast
	boinc_send_trickle_up(const_cast<char*>(BoincConstants::TrickleDeamonName.c_str()), const_cast<char*>(trickle_message.c_str())); // progress to boinc project server
#pragma GCC diagnostic pop
	Logging::debugPrint(Logging::Detail::DevelDebug, " trickle daemon name :" + BoincConstants::TrickleDeamonName);
	Logging::debugPrint(Logging::Detail::ObjectContentRevision, " generated trickle message :" + trickle_message);
	boinc_fraction_done(fraction_done); // progress to BOINC manager
    } 
    std::cout << "Progress : " << 
	    RunnerUtils::toString(computed_hashes_) << "/" << RunnerUtils::toString(total_hashes_) << " : " <<
	    RunnerUtils::toString(percent_done) << "%" << std::endl;
}

int TaskBase::saveAndFinish() {
    PRINT_POSITION_IN_CODE();
    finish();
    PRINT_POSITION_IN_CODE();
    saveResult();
    PRINT_POSITION_IN_CODE();
    return exit_code_;
}

void TaskBase::saveResult() {
    std::string output_message;
    
    PRINT_POSITION_IN_CODE();
    output_message = generateOutputMessage();
    PRINT_POSITION_IN_CODE();

    Logging::debugPrint(Logging::Detail::GeneralInfo, "Result: \n" + output_message);

    writeOutputFile(output_message);
}

