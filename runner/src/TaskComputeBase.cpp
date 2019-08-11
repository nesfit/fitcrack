/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskComputeBase.hpp"
#include "PCFGpipe.hpp"

/* Protected */
void TaskComputeBase::getAllArguments() {

    hashcat_arguments_ = attack_->getArguments();

    //if PCFG
    if(isPCFG_){
      PCFGmanager_arguments_ = attack_->getPCFGArguments();
    }

    host_config_.read();

    host_config_.print();

    host_config_.parseArguments(hashcat_arguments_);

    //host_config_.parseArguments(PCFGmanager_arguments_);
}

/* Public */
TaskComputeBase::TaskComputeBase (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) : TaskBase(directory, task_config, host_config, output_file, workunit_name), attack_(nullptr), process_(nullptr) {  }

TaskComputeBase::TaskComputeBase (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name) : TaskBase(directory, task_config, host_config, output_file, workunit_name), attack_(nullptr), process_(nullptr) {  }

TaskComputeBase::~TaskComputeBase() {
    if (attack_ != nullptr) {
	delete attack_;
	attack_ = nullptr;
    }

    if (process_ != nullptr) {
	delete process_;
	process_ = nullptr;
    }

    for (std::vector<char*>::iterator it = hashcat_arguments_.begin(); it != hashcat_arguments_.end(); it++) {
        delete []*it;
    }
    //if PCFG
    if(isPCFG_){
      for (std::vector<char*>::iterator it = PCFGmanager_arguments_.begin(); it != PCFGmanager_arguments_.end(); it++) {
          delete []*it;
        }
    }
}

std::string TaskComputeBase::getErrorMessage() {
    return process_->readErrPipeAvailableLines();
}

double TaskComputeBase::getRunTime() const {
    return process_->getExecutionTime();
}

int TaskComputeBase::finish() {

    PRINT_POSITION_IN_CODE();

    exit_code_ = process_->finish();

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

    int pipefd;

    if (attack_ == nullptr) {

        //create a named pipe with a name "termspipe" where stdout will be redirected
        pipefd = PCFGpipe::createNamedPipe("termspipe");

        attack_ = Attack::create(task_config_, directory_, isPCFG_);
        //isPCFG_ = attack_->checkPCFG();
        if(!isPCFG_){
          //if attack is not PCFG, there's no need for open pipeline
          printf("closing the pipe\n");
          PCFGpipe::closeNamedPipe(pipefd);
        }
        getAllArguments();
    }

    if (process_ == nullptr) {
      if(isPCFG_){
        process_ = ProcessPCFG::create(PCFGmanager_arguments_, directory_);
        TaskComputeBase::startComputation();

        //after pcfg-manager ends, delete process pointer
        //close pipe for writing (from stdout)
        //closeNamedPipeWrite("termspipe");
        process_ = nullptr;
      };

        process_ = Process::create(hashcat_arguments_, directory_);

        if(isPCFG_){
          //should end after hashcat ends
          PCFGpipe::closeNamedPipe(pipefd);
        }
    }
}

void TaskComputeBase::printProcessOut() {
    PRINT_POSITION_IN_CODE();
    Logging::debugPrint(Logging::Detail::Important, "Hashcat available stdout : \n" + process_->readOutPipeAvailableLines());
    PRINT_POSITION_IN_CODE();
}

void TaskComputeBase::printProcessErr() {
    PRINT_POSITION_IN_CODE();
    Logging::debugPrint(Logging::Detail::Important, "TaskComputeBase: hashcat available stderr : \n" + process_->readErrPipeAvailableLines());
    PRINT_POSITION_IN_CODE();
}

void TaskComputeBase::startComputation() {
    if (!process_->isRunning()) {
        process_->run(isPCFG_);
	Logging::debugPrint(Logging::Detail::GeneralInfo, "Process has started.");
    }
}
