/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "Constants.hpp"

namespace BoincConstants {
  std::string ProjectName;
  std::string TrickleDeamonName;
};

const std::string RunnerConstants::HashcatMutexName = "FitcrackRunnerHashcatMutex";

void BoincConstants::setProjectName(const std::string& workunit_name) {
  if (workunit_name == "LocalWorkunit") {
    ProjectName = "standalone";
  } else {
    size_t found_at, last = 0;
    found_at = workunit_name.find("_", last);
    if (found_at == std::string::npos) {
      ProjectName = "unknown";
    } else {
      ProjectName = workunit_name.substr(last, found_at);
    }
  }
  TrickleDeamonName = ProjectName;
}

std::string BoincConstants::getHostConfigPath() {
  std::string HostConfigPath;
  #ifdef __WIN32
  HostConfigPath = "C:\\ProgramData\\BOINC\\" + BoincConstants::ProjectName + ".conf";
  #elif defined(__linux__) || defined(__APPLE__)
  HostConfigPath = "/etc/" + BoincConstants::ProjectName + ".conf"; // WARNING: CANNOT write there just CAN read from there
  #endif
  return HostConfigPath;
}
