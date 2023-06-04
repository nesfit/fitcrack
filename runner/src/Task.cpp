/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "Task.hpp"

namespace Task {
  std::string HostConfigPath;
}

TaskBase *Task::create(File &config_file, Directory &directory) {
  std::string mode;
  std::string workunit_name;
  ConfigTask task_config(config_file.getRelativePath());
  task_config.parse();

  task_config.print();
  task_config.find("mode", mode);

  workunit_name = getWorkunitName();
  BoincConstants::setProjectName(workunit_name);
  setHostConfigPath();

  ConfigHost host_config(HostConfigPath);

  if (mode == "a") {
    return new TaskBenchmarkAll(directory, task_config, host_config, BoincConstants::ResultFileName, workunit_name);
  } else if (mode == "b") {
    return new TaskBenchmark(directory, task_config, host_config, BoincConstants::ResultFileName, workunit_name);
  } else if (mode == "n") {
    return new TaskNormal(directory, task_config, host_config, BoincConstants::ResultFileName, workunit_name);
  } else {
    RunnerUtils::runtimeException("Unsupported mode of the task");
  }
  return nullptr;
}

std::string Task::getWorkunitName() {
  std::string workunit_name;

  if (RunnerUtils::isStandalone()) {
    workunit_name = "LocalWorkunit";
  } else {
    workunit_name = parseWorkunitName();
  }

  Logging::debugPrint(Logging::Detail::ObjectManipulation, "Workunit name is " + workunit_name);

  return workunit_name;
}

std::string Task::parseWorkunitName() {
  std::string line;
  std::fstream init_data_xml;

  File::openReadStream(init_data_xml, INIT_DATA_FILE);

  while (!File::readLine(init_data_xml, line).eof() && line.find("<wu_name>", 0) == std::string::npos);

  if (init_data_xml.eof()) {
    return "";
  }

  init_data_xml.close();

  size_t start = line.find(">", 0) + 1;
  size_t end	 = line.find("<", start);

  return line.substr(start, end - start);
}

void Task::setHostConfigPath() {
  HostConfigPath = BoincConstants::getHostConfigPath();
  Logging::debugPrint(Logging::Detail::ObjectManipulation, "HostConfigPath is " + HostConfigPath);
}
