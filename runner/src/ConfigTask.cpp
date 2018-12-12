/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "ConfigTask.hpp"

/* Private */

void ConfigTask::convertLineToOption(std::string& line) {
    PRINT_POSITION_IN_CODE();

    Logging::debugPrint(Logging::Detail::DevelDebug, "ConfigTask : parsed line : " + line);

    if (line.empty() || line == "\n" || line == "\r") 
	return;
    else {
	std::vector<std::string> fields(4);
	parseLine(line, fields);

	/** Verify recieved data and save key and values to options_ */
	if (verifyFields(fields)) {
	    saveFieldsToOptions(fields);
	} else {
	    RunnerUtils::runtimeException("ConfigTask: value lengths doesn't match or option isn't supported");
	}
    }
}

void ConfigTask::initSupported() {
    /** All the options from https://fitcrack.cz/trac/wiki/Fitcrack_Server_Komunikace_FromServer */
    supported_.push_back("mode"); 
    supported_.push_back("attack_mode");
    supported_.push_back("attack_submode");
    supported_.push_back("hash_type");
    supported_.push_back("name");
    supported_.push_back("charset1");
    supported_.push_back("charset2");
    supported_.push_back("charset3");
    supported_.push_back("charset4");
    supported_.push_back("rule_left");
    supported_.push_back("rule_right");
    supported_.push_back("mask");
    supported_.push_back("start_index");
    supported_.push_back("hc_keyspace");
    supported_.push_back("mask_hc_keyspace");
    supported_.push_back("dict_hc_keyspace");
}

bool ConfigTask::isSupported(const std::string& key) {
    for (std::vector<std::string>::iterator it = supported_.begin(); it != supported_.end(); it++) {
        if (*it == key) {
	    Logging::debugPrint(Logging::Detail::CustomOutput, "ConfigTask key: " + key + " is supported.");

            return true;
        }
    }

    Logging::debugPrint(Logging::Detail::CustomOutput, "ConfigTask key: " + key + " is not supported.");

    return false;
}

void ConfigTask::parseLine(const std::string& line, std::vector<std::string>& fields) {

    std::string fat_delimeter = "|||";
    std::string delimeter = "|";
    unsigned long last, next;

    /** Check for fat_delimeter at the start and end of the line */
    if (line.substr(0, fat_delimeter.length()) != fat_delimeter || 
            line.substr(line.length() - fat_delimeter.length(), fat_delimeter.length()) != fat_delimeter) {
        RunnerUtils::runtimeException("Invalid config syntax in file: " + path_);
    }

    last = 3;    // first 3 characters on the line are fat delimeter, no reason to read them again

    for (int i = 0; i < 4; i++) {

        /** If field is missing on the line, throw exception */
        if ((next = line.find(delimeter, last)) > line.length() - fat_delimeter.length()) 
            RunnerUtils::runtimeException("Invalid config syntax in file: " + path_);
        fields[i] = line.substr(last, next-last);

        /** Start after the last found delimeter */
        last = next + 1;
    }
}

void ConfigTask::saveFieldsToOptions(std::vector<std::string>& fields) {
    options_.insert(std::make_pair(fields[0],fields[3]));

    Logging::debugPrint(Logging::Detail::ObjectManipulation, "ConfigTask adding option: " + fields[0] + " => " + fields[3]);
}

bool ConfigTask::verifyFields(std::vector<std::string>& fields) {
    // Missing the checking for valid type but it isn't that trivial and also important at the moment
    if (isSupported(fields[0])) {

	Logging::debugPrint(Logging::Detail::CustomOutput, "ConfigTask verifying field : '" + fields[0] + "' value : '" + fields[3] + "' length: sent size = " + fields[2] + ", actual string size = " + RunnerUtils::toString(fields[3].length()));

        if (fields[3].length() == RunnerUtils::stoul(fields[2], nullptr, 10)) 
            return true;
    }
    return false;
}

/* Public */

ConfigTask::ConfigTask(const std::string config_path) {
    setPath(config_path);
    initSupported();
}

void ConfigTask::add(const std::string& key, const std::string& value) {
    if (isSupported(key)) {
	Logging::debugPrint(Logging::Detail::ObjectManipulation, "ConfigTask adding option: '" + key + "' => '" + value + "'");
	options_.insert(std::make_pair(std::string(key),std::string(value)));
    } else {
	Logging::debugPrint(Logging::Detail::ObjectManipulation, "ConfigTask option '" + key + "' is not supported ");
    }
}

bool ConfigTask::find(const std::string& key, std::string& value) const {
    // TODO: Same as in Directory find functions

    /** Find item in the list and copy the stored value to 'value' */
    if (isEmpty()) {
        return false;
    }

    std::map<std::string, std::string>::const_iterator found_it;

    found_it = options_.find(key);
    if (found_it == options_.end()) {
	return false;
    }

    value = options_.find(key)->second;

    Logging::debugPrint(Logging::Detail::CustomOutput, "key : " + key + ", value : " + value);

    return true;
}

std::string ConfigTask::find(const std::string& key) const {
    // TODO: Same as in Directory find functions

    /** Find item in the list and copy the stored value to 'value' */
    if (isEmpty()) {
	return std::string();
    }

    Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "key : " + key + ", value : " + options_.find(key)->second);

    std::map<std::string, std::string>::const_iterator found_it;

    found_it = options_.find(key);
    if (found_it == options_.end()) {
	return std::string();
    }

    return options_.find(key)->second;
}

std::string ConfigTask::getPath() const {
    return path_;
}

bool ConfigTask::isEmpty() const {
    if (options_.empty())
        return true;
    return false;
}

void ConfigTask::parse() {
    std::fstream file;
    std::string line;
    unsigned long len;

    File::openReadStream(file, path_); 

    /** While can readLine return new line get the lines and parse values form it and save tem to vector */
    while (!File::readLine(file, line).eof()) { 
	convertLineToOption(line);
    }

    if (file.is_open())
        file.close();
}

void ConfigTask::print() {
    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "ConfigTask options:");

    for (std::map<std::string, std::string>::iterator it = options_.begin(); it != options_.end(); it++) {
	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "|| " + it->first + " => " + it->second);
    }

    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "ConfigTask file contained " + RunnerUtils::toString(options_.size()) + " options.");
}

void ConfigTask::setPath(const std::string& config_path) {
    path_ = config_path;

    //Logging::debugPrint(Logging::Detail::ObjectManipulation, "ConfigTask set path: " + path_);
}

