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

bool TaskNormal::parseHashcatSpeed(std::string& progress_line) {
  /* example for parsing
  * STATUS	2	SPEED	0	1	EXEC_RUNTIME	0.182546	CURKU	0	PROGRESS	2028	17675	RECHASH 0	1	RECSALT 0	1	REJECTED	0
  * STATUS\t	%d\tSPEED\t<N x %PRIu64\t>1000\tEXEC_RUNTIME\t<N x %f\t>CURKU\t%PRIu64\tPROGRESS\t%PRIu64\t%PRIu64\tRECHASH\t%d\t%d\tRECSALT\t%d\t%d\tTEMP\t<N x %d\t>REJECTED\t%PRIu64\tUTIL\t<N x %d\t>
  * Above line is taken from
  * https://github.com/hashcat/hashcat/blob/8903adb164ce6e6c554dc9cb9634a725dabcceb0/src/terminal.c#L781
  */

  //Logging::debugPrint(Logging::Detail::CustomOutput, "Progress line : " + progress_line);
  if (progress_line.find("STATUS") != 0) {
    static const std::string invalidRuleMessageStart = "Cannot convert rule for use on OpenCL device in file";
    if(progress_line.substr(0, invalidRuleMessageStart.length()) == invalidRuleMessageStart)
    {
      invalidRuleCount += 1;
    }
    return false;
  }

  ProgressPair progress = parseSpeed(progress_line);
  if(progress.second == 0 && progress.first == 0)
  {
    return false;
  }

  /** When this is the first parsed line */
  HPU_ = progress.first;
  timeUnit_ = progress.second;

  return true;
}

bool TaskNormal::parseHashcatProgress(std::string& progress_line) {
  /* example for parsing
  * STATUS	2	SPEED	0	1	EXEC_RUNTIME	0.182546	CURKU	0	PROGRESS	2028	17675	RECHASH 0	1	RECSALT 0	1	REJECTED	0
  * STATUS\t	%d\tSPEED\t<N x %PRIu64\t>1000\tEXEC_RUNTIME\t<N x %f\t>CURKU\t%PRIu64\tPROGRESS\t%PRIu64\t%PRIu64\tRECHASH\t%d\t%d\tRECSALT\t%d\t%d\tTEMP\t<N x %d\t>REJECTED\t%PRIu64\tUTIL\t<N x %d\t>
  * Above line is taken from
  * https://github.com/hashcat/hashcat/blob/8903adb164ce6e6c554dc9cb9634a725dabcceb0/src/terminal.c#L781
  */

  //Logging::debugPrint(Logging::Detail::CustomOutput, "Progress line : " + progress_line);
  if (progress_line.find("STATUS") != 0) {
    static const std::string invalidRuleMessageStart = "Cannot convert rule for use on OpenCL device in file";
    if(progress_line.substr(0, invalidRuleMessageStart.length()) == invalidRuleMessageStart)
    {
      invalidRuleCount += 1;
    }
    return false;
  }

  ProgressPair progress = parseProgress(progress_line);
  if(progress.second == 0 && progress.first == 0)
  {
    return false;
  }

  /** When this is the first parsed line */
  if (total_hashes_ == 0) {
    setTotalHashesFromProgress(progress.first, progress.second, getSaltCountFromStatusLine(progress_line));
  }

  saveParsedProgress(progress.first);
  return true;
}

TaskNormal::ProgressPair TaskNormal::parseSpeed(const std::string& progress_line) {

  size_t speedLoc = progress_line.find("SPEED");    // Find position of SPEED

  if (speedLoc == std::string::npos) {
    return ProgressPair(0, 0);
  }

  size_t dataStart = progress_line.find('\t', speedLoc);    // Get position of \t after SPEED

  std::string::const_iterator dataEnd = std::find_if(progress_line.begin()+dataStart, progress_line.end(), ::isalpha);

  std::istringstream parser(progress_line.substr(dataStart, dataEnd-progress_line.begin()+dataStart));

  //HPU == Hashes per unit time
  uint64_t HPU, timeUnit;
  parser>>HPU>>timeUnit;
  return ProgressPair(HPU, timeUnit);

}

TaskNormal::ProgressPair TaskNormal::parseProgress(const std::string& progress_line) {

  size_t progressLoc = progress_line.find("PROGRESS");    // Find position of PROGRESS

  if (progressLoc == std::string::npos) {
    return ProgressPair(0, 0);
  }

  size_t dataStart = progress_line.find('\t', progressLoc);	    // Get position of \t after PROGRESS

  //find the delimiter for the next field
  std::string::const_iterator dataEnd = std::find_if(progress_line.begin()+dataStart, progress_line.end(), ::isalpha);
  //everything between is what we're interested in
  std::istringstream parser(progress_line.substr(dataStart, dataEnd-progress_line.begin()+dataStart));

  uint64_t currentProgress, totalHashes;
  parser>>currentProgress>>totalHashes;
  return ProgressPair(currentProgress, totalHashes);

}

void TaskNormal::saveParsedProgress(uint64_t currentProgress) {

  uint64_t difference_since_last_actualization = currentProgress-start_index_-computed_hashes_;
  Logging::debugPrint(Logging::Detail::CustomOutput, "Computed_hashes_index: " + RunnerUtils::toString(currentProgress));
  Logging::debugPrint(Logging::Detail::CustomOutput, "Difference_since_last_actualization: " + RunnerUtils::toString(difference_since_last_actualization));

  actualizeComputedHashes(difference_since_last_actualization);
}

void TaskNormal::setTotalHashesFromProgress(uint64_t current, uint64_t max, uint64_t saltCount) {

  std::string val;
  uint64_t limit = 0;
  if(
    task_config_.find(ConfigTask::HC_KEYSPACE, val) ||
    task_config_.find(ConfigTask::DICT_HC_KEYSPACE, val) ||
    task_config_.find(ConfigTask::MASK_HC_KEYSPACE, val)
  )
  {
    limit = RunnerUtils::stoull(val);
  }
  if(max)
  {
    if(limit)
    {
      uint64_t skip = 0;
      if(task_config_.find("start_index", val))
      {
        skip = RunnerUtils::stoull(val);
      }
      uint64_t totalLimit = skip+limit;
      start_index_ = max/totalLimit*skip;
    }
    else
    {
      //we have no way of finding out where it started, so just say that it was here ¯\_(ツ)_/¯
      start_index_ = current;
    }
    total_hashes_ = max-start_index_;
  }
  else
  {
    //fed by pipe, no skip and no limit
    start_index_ = 0;
    total_hashes_ = limit;
    if(total_hashes_ == 0)
    {
      //well, if we don't have any info about password count, we're screwed
      return;
    }
    File foundFile;
    if(directory_.find("rules", foundFile))
    {
      std::ifstream rulesStream(foundFile.getRelativePath().c_str());
      size_t ruleCount = 0;
      std::string line;
      while(std::getline(rulesStream, line))
      {
        if(!line.empty())
        {
          ++ruleCount;
        }
      }
      //subtract invalid rule count, as those are not applied
      ruleCount -= invalidRuleCount;
      total_hashes_ *= ruleCount;
    }
    total_hashes_ *= saltCount;
  }

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

TaskNormal::TaskNormal (Directory& directory, ConfigTask& task_config, const std::string& host_config, const std::string& output_file, const std::string& workunit_name):
  TaskComputeBase(directory, task_config, host_config, output_file, workunit_name),
  start_index_(0),
  invalidRuleCount(0)
{
  mode_ = "n";
  initializeTotalHashes();
}

TaskNormal::TaskNormal (Directory& directory, ConfigTask& task_config, ConfigHost& host_config, const std::string& output_file, const std::string& workunit_name):
  TaskComputeBase (directory, task_config, host_config, output_file, workunit_name),
  start_index_(0),
  invalidRuleCount(0)
{
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
    output_info += RunnerUtils::toString(process_hashcat_->getExecutionTime()) + "\n";
    output_info += passwords_;

  } else if (exit_code_ == HashcatConstant::Exhausted) {

    output_info += ProjectConstants::TaskFinalStatus::Exhausted + "\n";
    output_info += RunnerUtils::toString(process_hashcat_->getExecutionTime()) + "\n";

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

bool TaskNormal::parseHashcatOutputLine(std::string& output_line) {
  if (!output_line.empty()) {
    Logging::debugPrint(Logging::Detail::CustomOutput, "Hashcat line: " + output_line);
  } else {
    return false;
  }

  if (!parseHashcatProgress(output_line)) {
    return false;
  }

  if (!parseHashcatSpeed(output_line)) {
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
