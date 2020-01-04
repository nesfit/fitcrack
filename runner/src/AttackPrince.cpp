/*
* Author : Dávid Bolvanský
* Licence: MIT, see LICENSE
*/

#include "AttackPrince.hpp"

AttackPrince::AttackPrince(const ConfigTask &config, Directory &directory)
    : AttackCrackingBase(
          config, directory,
          /*attack_mode=*/"0") { // Change attack mode from 8 to 0
  addSpecificArguments();
}

void AttackPrince::addSpecificArguments() {
  if (attack_submode_ == "0" || attack_submode_ == "1") {
    // Do nothing
  } else {
    RunnerUtils::runtimeException(
        "Unsupported attack_submode = " + attack_submode_ +
        " attack_mode = " + attack_mode_ + " has no such attack_submode");
  }

  addPrinceRequiredFile("wordlist"); // Add wordlist
  addPrinceArgument("--dupe-check-disable"); // Disable checking for duplicates (faster loading)

  File filePrinceResources;
  if (directory_.find("resources", filePrinceResources)) {
    std::fstream fs;
    std::string skip, limit;
    File::openReadStream(fs, filePrinceResources.getRelativePath());
    File::readLine(fs, skip);
    skip = "--skip=" + skip;
    File::readLine(fs, limit);
    limit = "--limit=" + limit;
    addPrinceArgument(skip);
    addPrinceArgument(limit);
  }
}

void AttackPrince::addPrinceArgument(const std::string &argument){
  Logging::debugPrint(Logging::Detail::ObjectManipulation, "Adding Prince preprocessor argument '" + argument + "'");
  Prince_arguments_.push_back(argument);
}

void AttackPrince::addPrinceRequiredFile(const std::string& file_name) {

  Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

  File filePrince;
  if (!directory_.find(file_name, filePrince)) {
    RunnerUtils::runtimeException("Missing Prince preprocessor required file " + file_name);
  }

  addPrinceArgument(filePrince.getRelativePath());
}

const std::vector<std::string>& AttackPrince::getPrinceArguments() {
  return Prince_arguments_;
}

//
