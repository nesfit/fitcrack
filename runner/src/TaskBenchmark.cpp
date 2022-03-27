/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskBenchmark.hpp"
/* Protected */

bool TaskBenchmark::parseHashcatBenchmark(std::string &output_line) {
  using namespace nlohmann;
  status_info_ = json::parse(output_line, nullptr, false);
  if (status_info_.is_discarded()) {
    // JSON parse error
    return false;
  }

  for (const auto &device : status_info_.at("devices")) {
    uint64_t device_speed = device.at("speed");
    device_speeds_.push_back(device_speed);
  }

  benchmarked_speeds_.insert(getTotalSpeed());

  if (salt_count_ <= 1) {
    uint64_t salt_count = status_info_.at("recovered_salts").at(1);
    salt_count_ = std::max<uint64_t>(salt_count, 1);
  }

  return true;
}

/* Public */

TaskBenchmark::TaskBenchmark(Directory &directory, ConfigTask &task_config,
                             const std::string &host_config,
                             const std::string &output_file,
                             const std::string &workunit_name)
    : TaskComputeBase(directory, task_config, host_config, output_file,
                      workunit_name),
      salt_count_(1) {
  mode_ = "b";
  initializeTotalHashes();
}

TaskBenchmark::TaskBenchmark(Directory &directory, ConfigTask &task_config,
                             ConfigHost &host_config,
                             const std::string &output_file,
                             const std::string &workunit_name)
    : TaskComputeBase(directory, task_config, host_config, output_file,
                      workunit_name),
      salt_count_(1) {
  mode_ = "b";
  initializeTotalHashes();
}

std::string TaskBenchmark::generateOutputMessage() {

  std::string output_message = "";

  exit_code_ = process_hashcat_->getExitCode();

  output_message += mode_ + "\n";

  if (exit_code_ == HashcatConstant::Succeded ||
      exit_code_ == HashcatConstant::Exhausted ||
      exit_code_ == HashcatConstant::RuntimeAbort) {

    output_message += ProjectConstants::TaskFinalStatus::Succeded + "\n";
    // divide by salt count. This is done since hashcat prints it and finding it
    // out on the server would be a PITA
    uint64_t speed = getBenchmarkedSpeed() / salt_count_;
    output_message += RunnerUtils::toString(speed) + "\n";
    output_message +=
        RunnerUtils::toString(process_hashcat_->getExecutionTime()) + "\n";
    // communicate success to the outside world
    exit_code_ = HashcatConstant::Succeded;

  } else {

    output_message += ProjectConstants::TaskFinalStatus::Error + "\n";
    output_message +=
        RunnerUtils::toString(process_hashcat_->getExitCode()) + "\n";
    output_message += getErrorMessage() + "\n";
  }

  return output_message;
}

unsigned long long TaskBenchmark::getTotalSpeed() {
  unsigned long long speed_sum = 0;

  /** Sum speeds of all OCL devices used by hashcat on given node */
  for (unsigned long long speed : device_speeds_) {
    speed_sum += speed;
  }

  return speed_sum;
}

uint64_t TaskBenchmark::getBenchmarkedSpeed() {
  if (attack_->getExternalGeneratorName().empty()) {
    return getTotalSpeed();
  }
  // if this is generator process, some magic is required, since hashcat 5+ can
  // update status sporadically when fed by pipe therefore we remove the extreme
  // values and take the average of the rest. zero speeds are worthless
  benchmarked_speeds_.erase(0);
  std::set<uint64_t>::const_iterator iter = benchmarked_speeds_.begin();
  if (benchmarked_speeds_.size() == 0) {
    return 0;
  }
  size_t ignore_start = 0;
  size_t ignore_end = 0;
  if (benchmarked_speeds_.size() > 1) {
    // there tends to be a huge number at first, we don't want that
    ignore_end += 1;
  }
  if (benchmarked_speeds_.size() > 2) {
    // also ignore smallest value
    ++iter;
    ignore_start += 1;
  }
  // if we have enough, discard more of the extreme values
  if (benchmarked_speeds_.size() > 7) {
    ignore_end += 1;
  }
  if (benchmarked_speeds_.size() > 8) {
    ++iter;
    ignore_start += 1;
  }
  size_t total = 0;
  for (size_t i = ignore_start; i < benchmarked_speeds_.size() - ignore_end;
       ++i) {
    total += *(iter++);
  }
  return total / (benchmarked_speeds_.size() - ignore_end - ignore_start);
}

void TaskBenchmark::initializeTotalHashes() { total_hashes_ = 1; }

bool TaskBenchmark::parseHashcatOutputLine(std::string &output_line) {
  if (output_line.empty())
    return false;

  Logging::debugPrint(Logging::Detail::CustomOutput,
                      "Hashcat line: " + output_line);

  bool parsed = parseHashcatBenchmark(output_line);
  if (!parsed) {
    return false;
  }
  return true;
}

void TaskBenchmark::progress() {
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
