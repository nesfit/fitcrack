/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "AttackMask.hpp"

/* Private */

void AttackMask::addCharset(std::fstream& mask_file, const std::string& charset_name) {
    std::string content;

    if (config_.find(charset_name, content)) {
	content += ",";
	mask_file.write(content.data(), content.length());
	setHasCharsets();
    }
}

void AttackMask::addMask(std::fstream& mask_file) {
    std::string mask;

    if (config_.find("mask", mask)) {
	mask_file.write(mask.data(), mask.length());
    }
}

void AttackMask::createMaskFile() {
    std::fstream mask_file;

    File::openWriteStream(mask_file, HashcatConstant::MaskFileName, std::ios_base::trunc);

    addCharset(mask_file, "charset1");
    addCharset(mask_file, "charset2");
    addCharset(mask_file, "charset3");
    addCharset(mask_file, "charset4");
    
    addMask(mask_file);
    
    mask_file.close();

    directory_.scanForEntities();
}

void AttackMask::setHasCharsets() {
    if (!has_charsets_)
	has_charsets_ = true;
}

/* Protected */

void AttackMask::addSpecificArguments() {
    if (attack_submode_ == "0") {

        addArgument("--markov-disable");

    } else if (attack_submode_ == "1") {
    // This and next else-if could be rewritten but from this is easier to
    // read and say what are parameters for which attack

        addArgument("--markov-hcstat");
        addRequiredFile("markov");
        addArgument("--markov-classic");
        findAndAddOptional("markov_threshold", "--markov-threshold");

    } else if (attack_submode_ == "2") {

        addArgument("--markov-hcstat");
        addRequiredFile("markov");
        findAndAddOptional("markov_threshold", "--markov-threshold");

    } else {
        RunnerUtils::runtimeException("AttackMask: unsupported attack_submode = " + attack_submode_ + " attack_mode = " + attack_mode_ + " has no such attack_submode");
    }

    createMaskFile();
    addRequiredFile(HashcatConstant::MaskFileName);

    if (has_charsets_) {
	addArgument("--hex-charset");
    }
}


/* Public */ 
AttackMask::AttackMask(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory), has_charsets_(false) {
    addSpecificArguments();
}

