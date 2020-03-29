/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "TaskComputeBase.hpp"
#include "Attack.hpp"
#include <cassert>

/* Protected */
void TaskComputeBase::getAllArguments() {

  hashcat_arguments_ = attack_->getArguments();
  
  external_generator_arguments_ = attack_->getExternalGeneratorArguments();

  host_config_.read();

  host_config_.print();

  host_config_.parseArguments(hashcat_arguments_);
}

/* Public */
TaskComputeBase::TaskComputeBase(
  Directory& directory,
  ConfigTask& task_config,
  const std::string& host_config,
  const std::string& output_file,
  const std::string& workunit_name
):
  TaskBase(directory, task_config, host_config, output_file, workunit_name),
  attack_(nullptr),
  process_hashcat_(nullptr),
  process_external_generator_(nullptr),
  hashcat_mutex_(RunnerConstants::HashcatMutexName),
  attack_type(Attack::detectAttackType(task_config_))
{}

TaskComputeBase::TaskComputeBase(
  Directory& directory,
  ConfigTask& task_config,
  ConfigHost& host_config,
  const std::string& output_file,
  const std::string& workunit_name
):
  TaskBase(directory, task_config, host_config, output_file, workunit_name),
  attack_(nullptr),
  process_hashcat_(nullptr),
  process_external_generator_(nullptr),
  hashcat_mutex_(RunnerConstants::HashcatMutexName),
  attack_type(Attack::detectAttackType(task_config_))
{}

TaskComputeBase::~TaskComputeBase() {
  delete attack_;

  delete process_hashcat_;

  delete process_external_generator_;
}

std::string TaskComputeBase::getErrorMessage() {
  return process_hashcat_->readErrPipeAvailableLines();
}

double TaskComputeBase::getRunTime() const {
  return process_hashcat_->getExecutionTime();
}

int TaskComputeBase::finish() {

  PRINT_POSITION_IN_CODE();

  exit_code_ = process_hashcat_->finish();

  PRINT_POSITION_IN_CODE();

  Logging::debugPrint(Logging::Detail::DevelDebug, "TaskComputeBase : trying to get hashcat stderr.");

  PRINT_POSITION_IN_CODE();

  if (exit_code_ == (unsigned char)-1) {
    printProcessErr();
    RunnerUtils::runtimeException("Hashcat failed to run! Exit code: -1. "
                                  "Invalid arguments or input files.");
  } else if (exit_code_ == (unsigned char)-2) {
    printProcessErr();
    RunnerUtils::runtimeException(
        "Hashcat failed to run! Exit code: -2. GPU-watchdog alarm - "
        "temperature limit was reached.");
  }

  PRINT_POSITION_IN_CODE();

  return exit_code_;
}

void TaskComputeBase::initialize() {

  directory_.printDirectories();
  directory_.printFiles();

  if (attack_ == nullptr) {
    attack_ = Attack::create(task_config_, directory_);
    getAllArguments();
  }

  if (process_hashcat_ == nullptr) {
    process_hashcat_ = Process::create(hashcat_arguments_, directory_);
  }
  //create external generator if necessary
  std::string externalGeneratorName = attack_->getExternalGeneratorName();
  if(!externalGeneratorName.empty() && !process_external_generator_)
  {
    process_external_generator_ = Process::create(externalGeneratorName, attack_->getExternalGeneratorArguments(), directory_);
  }
  if (process_external_generator_) {
    process_hashcat_->setInPipe(process_external_generator_->GetPipeOut());
  }
}

void TaskComputeBase::printProcessOut() {
  PRINT_POSITION_IN_CODE();
  Logging::debugPrint(Logging::Detail::Important, "Hashcat available stdout : \n" + process_hashcat_->readOutPipeAvailableLines());
  PRINT_POSITION_IN_CODE();
}

void TaskComputeBase::printProcessErr() {
  PRINT_POSITION_IN_CODE();
  Logging::debugPrint(Logging::Detail::Important,
                      "TaskComputeBase: hashcat available stderr : \n" +
                          process_hashcat_->readErrPipeAvailableLines());
  PRINT_POSITION_IN_CODE();
}

void TaskComputeBase::startComputation() {
  if (process_external_generator_ &&!process_external_generator_->isRunning()) {
    process_external_generator_->run();
    Logging::debugPrint(Logging::Detail::GeneralInfo,
                        "External generator process has started. Waiting for start of output");
    process_external_generator_->GetPipeOut()->waitForAvailableOutput();
    Logging::debugPrint(Logging::Detail::GeneralInfo, "Waiting for the external generator output ended");
  }
  if(!process_hashcat_->isRunning())
  {
    hashcat_mutex_.lock();
    process_hashcat_->run();
    Logging::debugPrint(Logging::Detail::GeneralInfo,
                        "Hashcat process has started.");
  }
}


void TaskComputeBase::unlockHashcatMutex() {
  hashcat_mutex_.unlock();
}