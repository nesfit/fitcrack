/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "AttackCrackingBase.hpp"

AttackCrackingBase::AttackCrackingBase(const ConfigTask& config, Directory& directory) : AttackBase(config), directory_(directory) {

    /** For benchmark only allowed attack_mode value is 3 */
    findAndAddRequired("attack_mode", "-a");
    
    if (!config_.find("attack_submode", attack_submode_))
        RunnerUtils::runtimeException("attack_submode is missing in config");
    addSpecificArguments();
}

void AttackCrackingBase::addSpecificArguments() {

    addRequiredFile("data");

    findAndAddOptional("start_index", "-s");
    findAndAddOptional("hc_keyspace", "-l");
    
    addArgument("--status-timer="+RunnerUtils::toString(HashcatConstant::ProgressPeriod));

    addArgument("--outfile");
    addArgument(output_file_);

    addArgument("--outfile-format="+RunnerUtils::toString(HashcatConstant::OutputFormat));
    addArgument("--quiet");
    addArgument("--status");
    addArgument("--machine-readable");
    addArgument("--restore-disable");
    addArgument("--potfile-disable");
    addArgument("--logfile-disable");
//    addArgument("--gpu-temp-disable");
}

void AttackCrackingBase::addRequiredFile(const std::string& file_name) {

    Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

    File file;
    if (!directory_.find(file_name, file)) {
        RunnerUtils::runtimeException("Missing hashcat required file " + file_name);
    }

    addArgument(file.getRelativePath());
}

void AttackCrackingBase::addOptionalFile(const std::string& file_name, const std::string& argument) {

    Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name = " + file_name);

    File file;

    if (directory_.find(file_name, file)) {
        addArgument(argument);
        addArgument(file.getRelativePath());
    }
}
