/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "ConfigHost.hpp"

#include "boinc/str_util.h"

ConfigHost::ConfigHost(std::string config_path) {
  path_ = config_path;

  load();
}

void ConfigHost::load() {
  std::fstream file;

  if (File::exists(path_)) {
    File::openReadStream(file, path_);

    std::string line, key, value;
    while (!File::readLine(file, line).eof()) {
      size_t pos = line.find('=');
      if (pos == std::string::npos) continue;

      key = line.substr(0, pos);
      value = line.substr(pos + 1);

      strip_whitespace(key);
      strip_whitespace(value);

      configuration_[key] = value;
    }
  } else {

    Logging::debugPrint(Logging::Detail::Important,
                        "Config file doesn't exist at " + path_);
  }
}

std::vector<std::string> ConfigHost::getExtraHashcatArgs() {
  auto config_item_it = configuration_.find("extra_hashcat_args");
  if (config_item_it == configuration_.end())
    return {};

  std::string extraHashcatArgs = config_item_it->second;
  return split(extraHashcatArgs, ' ');
}

std::string ConfigHost::getPrestoredDictsPath() {
  auto config_item_it = configuration_.find("prestored_dicts_path");
  if (config_item_it == configuration_.end())
    return "";

  return config_item_it->second;
}
