#include "AttackExternalGenerator.hpp"

AttackExternalGenerator::AttackExternalGenerator(const ConfigTask &config, Directory &directory):
	AttackCrackingBase(
		config,
		directory,
		/*attack_mode=*/"0" //change the specific mode to dictionary mode for piping in
	)
{}

void AttackExternalGenerator::addGeneratorArgument(const std::string &argument){
  Logging::debugPrint(Logging::Detail::ObjectManipulation, "Adding external generator argument '" + argument + "'");
  generator_arguments_.push_back(argument);
}

void AttackExternalGenerator::addGeneratorRequiredFile(const std::string& file_name) {

  Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

  File filePCFG;
  if (!directory_.find(file_name, filePCFG)) {
    RunnerUtils::runtimeException("Missing external generator required file " + file_name);
  }

  addGeneratorArgument(filePCFG.getRelativePath());
}