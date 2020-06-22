/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "TaskBenchmarkAll.hpp"
#include <algorithm>
/* Public */
TaskBenchmarkAll::TaskBenchmarkAll (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name) :
  TaskBase(directory, task_config, host_config, output_file, workunit_name),
  m_totalExecutionTime(0),
  m_hcMutex(RunnerConstants::HashcatMutexName)
{
  mode_ = "a";
}

TaskBenchmarkAll::~TaskBenchmarkAll()
{
}

std::string TaskBenchmarkAll::generateOutputMessage() {

  std::ostringstream msg;

  Logging::debugPrint(Logging::Detail::ObjectContentRevision, "last benchmark exit code is " + RunnerUtils::toString(exit_code_));

  msg<<mode_<<'\n';

  if (exit_code_ == HashcatConstant::Succeded) {

    msg<<ProjectConstants::TaskFinalStatus::Succeded<<'\n';
    msg<<m_totalExecutionTime<<'\n';

    for(std::map<std::string, uint64_t>::iterator cur = m_results.begin(); cur != m_results.end(); ++cur)
    {
      msg<<cur->first<<':'<<cur->second<<'\n';
    }

  } else {

    msg<<ProjectConstants::TaskFinalStatus::Error<<'\n';
    msg<<exit_code_<<'\n';
    msg<<m_lastErrString<<'\n';
    msg<<ProjectConstants::TaskFinalStatus::Error<<'\n';

  }

  return msg.str();
}

void TaskBenchmarkAll::initializeTotalHashes()
{}

void TaskBenchmarkAll::initialize() {
  std::vector<std::string> args;
  args.push_back("--example-hashes");
  ProcessBase *process = Process::create(args, directory_);
  process->run();
  PipeBase *output = process->GetPipeOut();
  static const std::string hashLineStart = "MODE: ";
  while(output->canRead())
  {
    std::string line = output->readLine();
    if(line.substr(0, hashLineStart.length()) != hashLineStart)
    {
      continue;
    }
    while(std::isspace(line[line.length()-1])) line.erase(line.length()-1);
    std::string nextLine = output->readLine();
    //avoid errors
    if(nextLine.find("(null)") != std::string::npos || nextLine.find("Plaintext") != std::string::npos)
    {
      continue;
    }
    m_modes.push_back(line.substr(hashLineStart.length()));
  }
  total_hashes_ = m_modes.size();
  delete process;
}

void TaskBenchmarkAll::startComputation() {
}

int TaskBenchmarkAll::finish() {
  return exit_code_;
}

void TaskBenchmarkAll::progress() {
  std::vector<std::string> args;
  args.push_back("-b");
  args.push_back("-m");
  args.push_back("0");
  args.push_back("--machine-readable");
  //set code to error at first
  exit_code_ = HashcatConstant::Error;
  for(size_t i = 0; i < m_modes.size(); ++i)
  {
    const std::string &mode = m_modes[i];
    args[2] = mode;
    ProcessBase *hcProcess = Process::create(args, directory_);
    m_hcMutex.lock();
    hcProcess->run();
    PipeBase *output = hcProcess->GetPipeOut();
    while(output->canRead())
    {
      std::string line = output->readLine();
      //format of line is "%d:%u:%d:%d:%.2f:%" PRIu64, device_id + 1, hash_mode, device_info->corespeed_dev, device_info->memoryspeed_dev, device_info->exec_msec_dev, (u64) (device_info->hashes_msec_dev_benchmark * 1000)
      if(std::count(line.begin(), line.end(), ':') != 5)
      {
        continue;
      }
      std::istringstream parser(line);
      Logging::debugPrint(Logging::Detail::CustomOutput, "line to parse is " + line);
      std::string component;
      size_t componentIndex = -1;
      uint64_t *hashSpeed = &m_results[mode];
      *hashSpeed = 0;
      while(std::getline(parser, component, ':'))
      {
        componentIndex += 1;
        if(componentIndex == 5)
        {
          *hashSpeed += RunnerUtils::stoull(component);
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
    int code = hcProcess->finish();
    m_hcMutex.unlock();
    m_totalExecutionTime += hcProcess->getExecutionTime();
    if(code)
    {
      std::ostringstream err;
      m_lastErrString = hcProcess->readErrPipeAvailableLines();
      err<<"Hashcat failed for mode "<<mode<<": "<<m_lastErrString;
      Logging::debugPrint(Logging::Detail::Important, err.str().c_str());
    }
    else
    {
      //at least one success
      exit_code_ = 0;
    }
    delete hcProcess;
  }
}
