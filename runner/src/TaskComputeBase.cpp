/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "TaskComputeBase.hpp"
#include <cassert>

/* Protected */
void TaskComputeBase::getAllArguments() {

  hashcat_arguments_ = attack_->getArguments();

  switch (attack_type) {
  case AT_PCFG:
    external_generator_arguments_ =
        static_cast<AttackPCFG *>(attack_)->getPCFGArguments();
    break;
  case AT_Prince:
    external_generator_arguments_ =
        static_cast<AttackPrince *>(attack_)->getPrinceArguments();
    break;
  default:
    break;
  }

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
  hashcatMutex_("FitcrackRunnerHashcatMutex"),
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
  hashcatMutex_("FitcrackRunnerHashcatMutex"),
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

  printProcessErr();


  PRINT_POSITION_IN_CODE();

  return exit_code_;
}

void TaskComputeBase::initialize() {

  directory_.printDirectories();
  directory_.printFiles();

  if (attack_ == nullptr) {
    attack_ = Attack::create(task_config_, directory_, attack_type);
    getAllArguments();
  }

  if (process_hashcat_ == nullptr) {
    #ifdef PROCESSLINUX_HPP
    switch (attack_type) {
    case AT_PCFG: {
      if (process_external_generator_ == nullptr) {
        process_external_generator_ =
            ProcessPCFG::create(external_generator_arguments_, directory_);
      }
      break;
    }
    case AT_Prince: {
      if (process_external_generator_ == nullptr) {
        process_external_generator_ =
            ProcessPrince::create(external_generator_arguments_, directory_);
      }
      break;
    }
    default:
      break;
    }
    #endif

#ifdef PROCESSWINDOWS_HPP
    if (attack_type == AT_PCFG || attack_type == AT_Prince) {
      File hashcat_executable;
      File external_generator_executable;
      directory_.findVersionedFile("hashcat", "exe", hashcat_executable);
      std::string cmd = "cmd.exe";
      std::string hashcat_relative_path = hashcat_executable.getRelativePath();
      std::string external_generator_relative_path;
      if (attack_type == AT_PCFG) {
        directory_.findVersionedFile("pcfg", "exe",
                                     external_generator_executable);
      } else if (attack_type == AT_Prince) {
        directory_.findVersionedFile("prince", "exe",
                                     external_generator_executable);
      }
      external_generator_relative_path =
          external_generator_executable.getRelativePath();

      std::vector<std::string> cmd_arguments;
      cmd_arguments.push_back("/C");
      cmd_arguments.push_back(external_generator_relative_path);
      cmd_arguments.insert(cmd_arguments.end(),
                           external_generator_arguments_.begin(),
                           external_generator_arguments_.end());
      cmd_arguments.push_back("|");
      cmd_arguments.push_back(hashcat_relative_path);
      cmd_arguments.insert(cmd_arguments.end(), hashcat_arguments_.begin(),
                           hashcat_arguments_.end());

      if (attack_type == AT_PCFG) {
        directory_.findVersionedFile("pcfg", "exe",
                                     external_generator_executable);
      } else if (attack_type == AT_Prince) {
        directory_.findVersionedFile("prince", "exe",
                                     external_generator_executable);
      }
      
      process_hashcat_ = new ProcessWindows(cmd, cmd_arguments, true);
    }
#endif

    if (process_hashcat_ == nullptr)
      process_hashcat_ = Process::create(hashcat_arguments_, directory_);

    
    #ifdef PROCESSLINUX_HPP
    if (process_external_generator_) {
      assert(attack_type == AT_PCFG || attack_type == AT_Prince);
      process_hashcat_->initInPipe();
      process_hashcat_->setInPipe(process_external_generator_->GetPipeOut());
    }
    #endif
  }
}

void TaskComputeBase::printProcessOut() {
  PRINT_POSITION_IN_CODE();
  Logging::debugPrint(Logging::Detail::Important, "Hashcat available stdout : \n" + process_hashcat_->readOutPipeAvailableLines());
  PRINT_POSITION_IN_CODE();
}

void TaskComputeBase::printProcessErr() {
  PRINT_POSITION_IN_CODE();
  //Logging::debugPrint(Logging::Detail::Important, "TaskComputeBase: hashcat available stderr : \n" + process_hashcat_->readErrPipeAvailableLines());
  PRINT_POSITION_IN_CODE();
}

void TaskComputeBase::startComputation() {
  #ifdef PROCESSLINUX_HPP
  switch (attack_type) {
  case AT_Prince:
  case AT_PCFG: {
    if (!process_external_generator_->isRunning()) {
      process_external_generator_->run();
      Logging::debugPrint(Logging::Detail::GeneralInfo,
                          "External generator process has started.");
    }
    break;
  }
  default:
    break;
  }
  #endif

  if(!process_hashcat_->isRunning())
  {
    hashcatMutex_.lock();
    process_hashcat_->run();
    Logging::debugPrint(Logging::Detail::GeneralInfo,
                        "Hashcat process has started.");
  }
}
