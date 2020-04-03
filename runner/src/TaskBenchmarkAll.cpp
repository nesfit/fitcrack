/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "TaskBenchmarkAll.hpp"
#include <algorithm>
/* Public */
TaskBenchmarkAll::TaskBenchmarkAll (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) :
  TaskBase(directory, task_config, host_config, output_file, workunit_name),
  m_hcProcess(nullptr),
  m_hcMutex(RunnerConstants::HashcatMutexName)
{
  mode_ = "a";
}

TaskBenchmarkAll::~TaskBenchmarkAll()
{
  delete m_hcProcess;
}

std::string TaskBenchmarkAll::generateOutputMessage() {

  std::ostringstream msg;

  Logging::debugPrint(Logging::Detail::ObjectContentRevision, "last benchmark exit code is " + RunnerUtils::toString(exit_code_));

  msg<<mode_<<'\n';

  if (exit_code_ == HashcatConstant::Succeded) {

    msg<<ProjectConstants::TaskFinalStatus::Succeded<<'\n';
    msg<<m_hcProcess->getExecutionTime()<<'\n';

    for(std::map<std::string, uint64_t>::iterator cur = m_results.begin(); cur != m_results.end(); ++cur)
    {
      msg<<cur->first<<':'<<cur->second<<'\n';
    }

  } else {

    msg<<ProjectConstants::TaskFinalStatus::Error<<'\n';
    msg<<exit_code_<<'\n';
    msg<<m_hcProcess->readErrPipeAvailableLines()<<'\n';
    msg<<ProjectConstants::TaskFinalStatus::Error<<'\n';

  }

  return msg.str();
}

void TaskBenchmarkAll::initializeTotalHashes() {
  std::string hashtype;
  
  //get all the hashmodes
  {
    std::vector<std::string> args;
    args.push_back("--example-hashes");
    ProcessBase *hc_proc = Process::create(args, directory_);
    //doesn't use GPU, no need to lock
    hc_proc->run();
    PipeBase *output = hc_proc->GetPipeOut();
    static const std::string hashLineStart = "MODE: ";
    while(output->canRead())
    {
      std::string line = hc_proc->GetPipeOut()->readLine();
      if(line.substr(0, hashLineStart.length()) == hashLineStart)
      {
        total_hashes_ += 1;
      }
    }
    hc_proc->finish();
    delete hc_proc;
  }
}

void TaskBenchmarkAll::initialize() {
  //nothing else to do
  initializeTotalHashes();
}

void TaskBenchmarkAll::startComputation() {
  std::vector<std::string> args;
  args.push_back("-b");
  args.push_back("--benchmark-all");
  args.push_back("--machine-readable");
  m_hcProcess = Process::create(args, directory_);
  m_hcMutex.lock();
  m_hcProcess->run();
}

int TaskBenchmarkAll::finish() {
  exit_code_ = m_hcProcess->finish();
  m_hcMutex.unlock();
  return exit_code_;
}

void TaskBenchmarkAll::progress() {
  PipeBase *output = m_hcProcess->GetPipeOut();
  while(output->canRead())
  {
    std::string line = output->readLine();
    //format of line is "%d:%u:%d:%d:%.2f:%" PRIu64, device_id + 1, hash_mode, device_info->corespeed_dev, device_info->memoryspeed_dev, device_info->exec_msec_dev, (u64) (device_info->hashes_msec_dev_benchmark * 1000
    if(std::count(line.begin(), line.end(), ':') != 5)
    {
      continue;
    }
    std::istringstream parser(line);
    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "line to parse is " + line);
    std::string component;
    size_t componentIndex = -1;
    uint64_t *hashSpeed = nullptr;
    while(std::getline(parser, component, ':'))
    {
      componentIndex += 1;
      switch(componentIndex)
      {
      case 1:
        hashSpeed = &m_results[component];
        break;
      case 5:
        *hashSpeed += RunnerUtils::stoull(component);
        break;
      default:
        break;
      }
    }
    if(componentIndex == 5)
    {
      //if this was parsed successfully, report progress to the server
      //this way of reporting will report progress upon sucessful parsing of the first device rather than the last
      //but hc should print them all pretty much at the same time, so it shouldn't matter much
      actualizeComputedHashes(m_results.size()-computed_hashes_);
      reportProgress();
    }
  }
}
