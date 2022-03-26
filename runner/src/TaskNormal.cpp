/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskNormal.hpp"

#include <algorithm>
#include <cctype>

/* Protected */

std::string TaskNormal::getPasswords() {
  readPasswordsFromFile();
  return passwords_;
}

bool TaskNormal::parseHashcatStatus(const std::string &progress_line) {
  using namespace nlohmann;
  status_info_ = json::parse(progress_line, nullptr, false);
  if (status_info_.is_discarded()) {
    // JSON parse error
    return false;
  }

  uint64_t current_progress = status_info_.at("progress").at(0);
  uint64_t total_hashes = status_info_.at("progress").at(1);

  uint64_t salt_count = status_info_.at("recovered_salts").at(1);
  salt_count = std::max<uint64_t>(salt_count, 1);

  if (current_progress == 0 && total_hashes == 0) {
    return false;
  }

  /** When this is the first parsed line */
  if (total_hashes_ == 0) {
    setTotalHashesFromProgress(current_progress, total_hashes, salt_count);
  }

  saveParsedProgress(current_progress);
  return true;
}

void TaskNormal::saveParsedProgress(uint64_t currentProgress) {

  uint64_t difference_since_last_actualization =
      currentProgress - start_index_ - computed_hashes_;
  Logging::debugPrint(Logging::Detail::CustomOutput,
                      "Computed_hashes_index: " +
                          RunnerUtils::toString(currentProgress));
  Logging::debugPrint(
      Logging::Detail::CustomOutput,
      "Difference_since_last_actualization: " +
          RunnerUtils::toString(difference_since_last_actualization));

  actualizeComputedHashes(difference_since_last_actualization);
}

void TaskNormal::setTotalHashesFromProgress(uint64_t current, uint64_t max,
                                            uint64_t salt_count) {

  std::string keyspace_val;
  uint64_t limit = 0;

  bool has_keyspace =
      task_config_.find(ConfigTask::HC_KEYSPACE, keyspace_val) ||
      task_config_.find(ConfigTask::DICT_HC_KEYSPACE, keyspace_val) ||
      task_config_.find(ConfigTask::MASK_HC_KEYSPACE, keyspace_val);
  if (has_keyspace) {
    limit = RunnerUtils::stoull(keyspace_val);
  }

  if (max) {
    if (limit) {
      std::string start_index_val;
      uint64_t skip = 0;
      if (task_config_.find("start_index", start_index_val)) {
        skip = RunnerUtils::stoull(start_index_val);
      }
      uint64_t total_limit = skip + limit;
      start_index_ = (max / total_limit) * skip;
    } else {
      // we have no way of finding out where it started, so just say that it was
      // here ¯\_(ツ)_/¯
      start_index_ = current;
    }
    total_hashes_ = max - start_index_;
  } else {
    // fed by pipe, no skip and no limit
    start_index_ = 0;
    total_hashes_ = limit;
    if (total_hashes_ == 0) {
      // well, if we don't have any info about password count, we're screwed
      return;
    }
    File rule_file;
    if (directory_.find("rules", rule_file)) {
      std::ifstream rulesStream(rule_file.getRelativePath().c_str());
      size_t rules_count = 0;
      std::string line;
      while (std::getline(rulesStream, line)) {
        if (!line.empty()) {
          ++rules_count;
        }
      }
      // subtract invalid rule count, as those are not applied
      rules_count -= invalid_rules_count;
      total_hashes_ *= rules_count;
    }
    total_hashes_ *= salt_count;
  }
}

void TaskNormal::readPasswordsFromFile() {
  std::fstream passwords_stream;
  File passwords_file;

  Logging::debugPrint(Logging::Detail::GeneralInfo,
                      "Looking for hashcat output file: " +
                          HashcatConstant::OutputFile);

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
  }
}

/* Public */

TaskNormal::TaskNormal(Directory &directory, ConfigTask &task_config,
                       const std::string &host_config,
                       const std::string &output_file,
                       const std::string &workunit_name)
    : TaskComputeBase(directory, task_config, host_config, output_file,
                      workunit_name),
      start_index_(0), invalid_rules_count(0) {
  mode_ = "n";
  initializeTotalHashes();
}

TaskNormal::TaskNormal(Directory &directory, ConfigTask &task_config,
                       ConfigHost &host_config, const std::string &output_file,
                       const std::string &workunit_name)
    : TaskComputeBase(directory, task_config, host_config, output_file,
                      workunit_name),
      start_index_(0), invalid_rules_count(0) {
  mode_ = "n";
  initializeTotalHashes();
}

std::string TaskNormal::generateOutputMessage() {

  std::string output_info = "";

  exit_code_ = process_hashcat_->getExitCode();
  readPasswordsFromFile();

  output_info += mode_ + "\n";

  /* Succeded is returned when passwords to all hashes are found otherwise
   * Exhausted is retuned and we have to check what passwords were found and
   * report them to server
   */
  if (exit_code_ == HashcatConstant::Succeded || !passwords_.empty()) {

    output_info += ProjectConstants::TaskFinalStatus::Succeded + "\n";
    output_info +=
        RunnerUtils::toString(process_hashcat_->getExecutionTime()) + "\n";
    output_info += passwords_;

  } else if (exit_code_ == HashcatConstant::Exhausted) {

    output_info += ProjectConstants::TaskFinalStatus::Exhausted + "\n";
    output_info +=
        RunnerUtils::toString(process_hashcat_->getExecutionTime()) + "\n";

  } else {

    output_info += ProjectConstants::TaskFinalStatus::Error + "\n";
    output_info += RunnerUtils::toString(exit_code_) + "\n";
    output_info += process_hashcat_->readErrPipeAvailableLines() + "\n";

    /*
  } else if (exit_code == 4) {
  // TODO: similar to 3 but different errors
  */
  }

  return output_info;
}

bool TaskNormal::parseHashcatOutputLine(std::string &output_line) {
  if (output_line.empty())
    return false;

  if (output_line.find(
          "Cannot convert rule for use on OpenCL device in file") !=
      std::string::npos) {
    invalid_rules_count += 1;
    return false;
  }

  Logging::debugPrint(Logging::Detail::CustomOutput,
                      "Hashcat line: " + output_line);

  bool parsed = parseHashcatStatus(output_line);
  if (!parsed) {
    return false;
  }

  return true;
}

void TaskNormal::progress() {
  std::string line;

  PRINT_POSITION_IN_CODE();

  while (process_hashcat_->isRunning()) {

    PRINT_POSITION_IN_CODE();

    line = process_hashcat_->readOutPipeLine();

    PRINT_POSITION_IN_CODE();

    if (parseHashcatOutputLine(line)) {
      reportProgress();
    }
  }
  PRINT_POSITION_IN_CODE();
}
