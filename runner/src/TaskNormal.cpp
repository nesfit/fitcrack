/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskNormal.hpp"

/* Protected */

std::string TaskNormal::getPasswords() {
    readPasswordsFromFile();
    return passwords_;
}

void TaskNormal::parseHashcatProgress(std::string& progress_line) {
    /* example for parsing
     * STATUS	2	SPEED	0	1	EXEC_RUNTIME	0.182546	CURKU	0	PROGRESS	2028	17675	RECHASH 0	1	RECSALT 0	1	REJECTED	0
     * STATUS\t	%d\tSPEED\t<N x %PRIu64\t>1000\tEXEC_RUNTIME\t<N x %f\t>CURKU\t%PRIu64\tPROGRESS\t%PRIu64\t%PRIu64\tRECHASH\t%d\t%d\tRECSALT\t%d\t%d\tTEMP\t<N x %d\t>REJECTED\t%PRIu64\tUTIL\t<N x %d\t>
     * Above line is taken from
     * https://github.com/hashcat/hashcat/blob/8903adb164ce6e6c554dc9cb9634a725dabcceb0/src/terminal.c#L781
     */

    //Logging::debugPrint(Logging::Detail::CustomOutput, "Progress line : " + progress_line);
    if (progress_line.find("STATUS", 0) != 0) {
	return;
    }

    /** When total_hashes_ wasn't specified in config file save hashcat's keyspace */
    if (total_hashes_ == 0) {
	setTotalHahsesFromProgressLine(progress_line);
    }

    std::string total_computed_hashes;
    if (parse_curku_) {
	total_computed_hashes = parseCurku(progress_line);
    } else {
	total_computed_hashes = parseProgress(progress_line);
    }

    if (!total_computed_hashes.empty()) {
	saveParsedProgress(total_computed_hashes);
    }
}

std::string TaskNormal::parseCurku(const std::string& progress_line) {
    size_t last = 0;
    size_t found_at;
    std::string curku;

    found_at = progress_line.find("CURKU", last);    // Find position of PROGRESS

    if (found_at == std::string::npos) {
	return std::string();
    }

    last = found_at+1;
    found_at = progress_line.find("\t", last);	    // Skip CURKU word

    last = found_at+1;
    found_at = progress_line.find("\t", last);	    // Get position after computed hashes

    curku = progress_line.substr(last, found_at - last);	// Cut out the number
    Logging::debugPrint(Logging::Detail::DevelDebug, "Parsed curku: " + curku);

    return curku;
}

std::string TaskNormal::parseProgress(const std::string& progress_line) {
    size_t last = 0;
    size_t found_at;
    std::string computed_hashes;

    found_at = progress_line.find("PROGRESS", last);    // Find position of PROGRESS

    if (last == std::string::npos) {
	return std::string();
    }

    last = found_at +1;
    found_at = progress_line.find("\t", last);	    // Get position of \t after PROGRESS

    last = found_at +1;
    found_at = progress_line.find("\t", last);	    // Get position of \t after computed hashes

    computed_hashes = progress_line.substr(last, found_at - last);	// Cut out the number
    Logging::debugPrint(Logging::Detail::DevelDebug, "Parsed computed_hashes: " + computed_hashes);
    return computed_hashes;

}

void TaskNormal::saveParsedProgress(const std::string& computed_hashes_index) {
    unsigned long long total_computed_hashes = RunnerUtils::stoull(computed_hashes_index) - start_index_;
    unsigned long long difference_since_last_actualization = total_computed_hashes - computed_hashes_;

    Logging::debugPrint(Logging::Detail::CustomOutput, "Computed_hashes_index: " + RunnerUtils::toString(computed_hashes_index));
    Logging::debugPrint(Logging::Detail::CustomOutput, "Difference_since_last_actualization: " + RunnerUtils::toString(difference_since_last_actualization));

    actualizeComputedHashes(difference_since_last_actualization);
}

void TaskNormal::setTotalHahsesFromProgressLine(const std::string& progress_line) {
    size_t last = 0;
    size_t found_at;
    std::string total_hashes;

    found_at = progress_line.find("PROGRESS", last);    // Find position of PROGRESS

    if (last == std::string::npos) {
	return; 
    }

    last = found_at +1;
    found_at = progress_line.find("\t", last);	    // Get position of \t after PROGRESS

    last = found_at +1;
    found_at = progress_line.find("\t", last);	    // Get position of \t after computed hashes

    last = found_at +1;
    found_at = progress_line.find("\t", last);	    // Get position of \t after total hashes

    Logging::debugPrint(Logging::Detail::DevelDebug, "Parsed total_hashes " + total_hashes);
    total_hashes = progress_line.substr(last, found_at - last);	// Cut out the number

    total_hashes_ = RunnerUtils::stoull(total_hashes) - start_index_;

}

void TaskNormal::readPasswordsFromFile() {
    std::fstream passwords_stream;
    File passwords_file;

    PRINT_POSITION_IN_CODE();

    Logging::debugPrint(Logging::Detail::GeneralInfo, "Looking for hashcat output file: " + HashcatConstant::OutputFile);

    directory_.scanForEntities();
    if (directory_.find(HashcatConstant::OutputFile, passwords_file)) {

	File::openReadStream(passwords_stream, passwords_file.getRelativePath());

	/** Preallocates size of the string (performance) and saves whole file into
	 * string */
	passwords_stream.seekg(0, std::ios::end);   
	passwords_.reserve(passwords_stream.tellg());
	passwords_stream.seekg(0, std::ios::beg);

	passwords_.assign((std::istreambuf_iterator<char>(passwords_stream)),
		std::istreambuf_iterator<char>());

	PRINT_POSITION_IN_CODE();
    }
    PRINT_POSITION_IN_CODE();
}

/* Public */

TaskNormal::TaskNormal (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) : TaskComputeBase (directory, task_config, host_config, output_file, workunit_name), start_index_(0), parse_curku_(false) {
    mode_ = "n";
    initializeTotalHashes();
}

TaskNormal::TaskNormal (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name) : TaskComputeBase (directory, task_config, host_config, output_file, workunit_name), start_index_(0), parse_curku_(false)  {
    mode_ = "n";
    initializeTotalHashes();
}

std::string TaskNormal::generateOutputMessage() {

    std::string output_info = "";

    exit_code_ = process_->getExitCode();
    readPasswordsFromFile();

    output_info += mode_ + "\n"; 

    /* Succeded is returned when passwords to all hashes are found otherwise
     * Exhausted is retuned and we have to check what passwords were found and
     * report them to server 
     */
    if (exit_code_ == HashcatConstant::Succeded || !passwords_.empty()) {

	output_info += ProjectConstants::TaskFinalStatus::Succeded + "\n";
	output_info += RunnerUtils::toString(process_->getExecutionTime()) + "\n";
	output_info += passwords_;

    } else if (exit_code_ == HashcatConstant::Exhausted) {

	output_info += ProjectConstants::TaskFinalStatus::Exhausted + "\n";
	output_info += RunnerUtils::toString(process_->getExecutionTime()) + "\n";

    } else {

	output_info += ProjectConstants::TaskFinalStatus::Error + "\n"; 
	output_info += RunnerUtils::toString(exit_code_) + "\n";
	output_info += process_->readErrPipeAvailableLines() + "\n"; 

    /* 
       } else if (exit_code == 4) {
    // TODO: similar to 3 but different errors
    */
    }

    return output_info;
}

void TaskNormal::initializeTotalHashes() {

    std::string total_hc_keyspace;
    std::string start_index;

    // --limit or total keyspace of mask
    if (task_config_.find("hc_keyspace", total_hc_keyspace) ||
	    task_config_.find("dict_hc_keyspace", total_hc_keyspace) ||
	    task_config_.find("maks_hc_keyspace", total_hc_keyspace)) { 

	parse_curku_ = true;
	total_hashes_ = RunnerUtils::stoull(total_hc_keyspace);

    } // else { when not specified in config parse from Hashcat progress_line }

    if (task_config_.find("start_index", start_index)) {
	start_index_ = RunnerUtils::stoull(start_index);
    }

    if (parse_curku_) { // --skip
	start_computed_index_ = start_index_;
    } else {
	std::string start_computed_index;

	if (task_config_.find("start_hash_index", start_computed_index)) {
	    start_computed_index_ = RunnerUtils::stoull(start_computed_index);
	}
    }
}

bool TaskNormal::parseHashcatOutputLine(std::string& output_line) {

    if (!output_line.empty()) {
	Logging::debugPrint(Logging::Detail::CustomOutput, "Hashcat line: " + output_line);
    } else {
	return false;
    }

    parseHashcatProgress(output_line);

    return true;
}

void TaskNormal::progress() {
    std::string line;

    PRINT_POSITION_IN_CODE();

    while (process_->isRunning()) {

	PRINT_POSITION_IN_CODE();

	line = process_->readOutPipeLine();

	PRINT_POSITION_IN_CODE();
	if (parseHashcatOutputLine(line)) {
	    reportProgress();
	}
    }

    PRINT_POSITION_IN_CODE();
}
