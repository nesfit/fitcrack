/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskBenchmark.hpp"
/* Protected */

void TaskBenchmark::parseHashcatBenchmark(std::string& output_line) {

  /** READ THIS
    Relevant code from Hashcat:
    printf ("SPEED\t");
    for (int device_id = 0; device_id < hashcat_status->device_info_cnt; device_id++)
    {
      const device_info_t *device_info = hashcat_status->device_info_buf + device_id;
      if (device_info->skipped_dev == true) continue;
      printf ("%" PRIu64 "\t", (u64) (device_info->hashes_msec_dev * 1000));
      // that 1\t is for backward compatibility
      printf ("1000\t");
    }
    */

  Logging::debugPrint(Logging::Detail::CustomOutput, "Hashcat output line for parsing: " + output_line);

  //find the speeds
  {
    static const char marker[] = "SPEED\t";
    size_t found_at = output_line.find(marker);
    if (found_at != std::string::npos) {
      //make sure we don't double count
      device_speeds_.clear();
      //-1 for terminating NULL
      found_at += sizeof(marker)-1;
      std::istringstream parser(output_line.substr(found_at));
      Logging::debugPrint(Logging::Detail::CustomOutput, "to parse: " + output_line.substr(found_at));
      size_t speed;
      size_t dummy;
      while(parser>>speed && parser>>dummy && dummy == 1000)
      {
        Logging::debugPrint(Logging::Detail::CustomOutput, "parsed speed: " + RunnerUtils::toString(speed));
        device_speeds_.push_back(speed);
      }
      m_benchmarked_speeds.insert(getTotalSpeed());
    }
    else
    {
      Logging::debugPrint(Logging::Detail::CustomOutput, "Not found");
    }
  }

  //find the number of salts
  //HC prinst like so: printf ("RECSALT\t%d\t%d\t", hashcat_status->salts_done, hashcat_status->salts_cnt);
  if(m_saltCount <= 1)
  {
    m_saltCount = getSaltCountFromStatusLine(output_line);
  }

}

/* Public */

TaskBenchmark::TaskBenchmark (
  Directory& directory,
  ConfigTask& task_config,
  const std::string& host_config,
  const std::string& output_file,
  const std::string& workunit_name
):
  TaskComputeBase (directory, task_config, host_config, output_file, workunit_name),
  m_saltCount(1)
{
  mode_ = "b";
  initializeTotalHashes();
}

TaskBenchmark::TaskBenchmark (
  Directory& directory,
  ConfigTask& task_config,
  ConfigHost& host_config,
  const std::string& output_file,
  const std::string& workunit_name
):
  TaskComputeBase (directory, task_config, host_config, output_file, workunit_name),
  m_saltCount(1)
{
  mode_ = "b";
  initializeTotalHashes();
}

std::string TaskBenchmark::generateOutputMessage() {

  std::string output_message = "";

  exit_code_ = process_hashcat_->getExitCode();

  output_message += mode_ + "\n";

  if (exit_code_ == HashcatConstant::Succeded || exit_code_ == HashcatConstant::Exhausted || exit_code_ == HashcatConstant::RuntimeAbort) {

    output_message += ProjectConstants::TaskFinalStatus::Succeded + "\n";
    //divide by salt count. This is done since hashcat prints it and finding it out on the server would be a PITA
    uint64_t speed = getBenchmarkedSpeed()/m_saltCount;
    output_message += RunnerUtils::toString(speed) + "\n";
    output_message += RunnerUtils::toString(process_hashcat_->getExecutionTime()) + "\n";
    //communicate success to the outside world
    exit_code_ = HashcatConstant::Succeded;

  } else {

    output_message += ProjectConstants::TaskFinalStatus::Error + "\n";
    output_message += RunnerUtils::toString(process_hashcat_->getExitCode()) + "\n";
    output_message += getErrorMessage() + "\n";

  }

  return output_message;
}

unsigned long long TaskBenchmark::getTotalSpeed() {
  unsigned long long speed_sum = 0;

  /** Sum speeds of all OCL devices used by hashcat on given node */
  for (std::vector<unsigned long long>::iterator it = device_speeds_.begin(); it != device_speeds_.end(); it++) {
    speed_sum += *it;
  }

  return speed_sum;
}

uint64_t TaskBenchmark::getBenchmarkedSpeed()
{
  if(attack_->getExternalGeneratorName().empty())
  {
    return getTotalSpeed();
  }
  //if this is generator process, some magic is required, since hashcat 5+ can update status sporadically when fed by pipe
  //therefore we remove the extreme values and take the average of the rest.
  //zero speeds are worthless
  m_benchmarked_speeds.erase(0);
  std::set<uint64_t>::const_iterator iter = m_benchmarked_speeds.begin();
  if(m_benchmarked_speeds.size() == 0)
  {
    return 0;
  }
  size_t ignore_start = 0;
  size_t ignore_end = 0;
  if(m_benchmarked_speeds.size() > 1)
  {
    //there tends to be a huge number at first, we don't want that
    ignore_end += 1;
  }
  if(m_benchmarked_speeds.size() > 2)
  {
    //also ignore smallest value
    ++iter;
    ignore_start += 1;
  }
  //if we have enough, discard more of the extreme values
  if(m_benchmarked_speeds.size() > 7)
  {
    ignore_end += 1;
  }
  if(m_benchmarked_speeds.size() > 8)
  {
    ++iter;
    ignore_start += 1;
  }
  size_t total = 0;
  for(size_t i = ignore_start; i < m_benchmarked_speeds.size()-ignore_end; ++i)
  {
    total += *(iter++);
  }
  return total/(m_benchmarked_speeds.size()-ignore_end-ignore_start);
}

void TaskBenchmark::initializeTotalHashes() {
  total_hashes_ = 1;
}

bool TaskBenchmark::parseHashcatOutputLine(std::string& output_line) {
  parseHashcatBenchmark(output_line);
  return true;
}

void TaskBenchmark::progress() {
  std::string lines;
  std::string line;
  while (!(lines = process_hashcat_->readOutPipeAvailableLines()).empty()) {
    Logging::debugPrint(Logging::Detail::CustomOutput, "progress lines: "+lines);
    size_t found_at = 0, last = 0;
    do
    {
      found_at = lines.find("\n", last);
      line = lines.substr(last, found_at - last);

      parseHashcatOutputLine(line);

      last = found_at +1;
    } while(found_at != std::string::npos);
    actualizeComputedHashes(1);
  }
}
