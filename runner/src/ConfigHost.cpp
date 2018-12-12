/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "ConfigHost.hpp"

ConfigHost::ConfigHost(std::string config_path) {
    setPath(config_path);
}

ConfigHost::ConfigHost(std::string& config_path) {
    setPath(config_path);
}

std::string ConfigHost::getContent() const {
    return content_;
}

std::string ConfigHost::getPath() const {
    return path_;
}

void ConfigHost::parseArguments(std::vector<char*>& arguments) {
    std::size_t next, last = 0;
    std::string delimeter = " ";
    std::string *argument;

    if (content_.empty()) {

	Logging::debugPrint(Logging::Detail::Important, "File wasn't read therefore its content can't be parsed.");
        return;
    }

    while (last != std::string::npos) {
        next = content_.find(delimeter, last);
        argument = new std::string(); 
        *argument = content_.substr(last, next - last);

	if (!argument->empty()) {
            arguments.push_back(TOCSTRING(argument->c_str()));

	    Logging::debugPrint(Logging::Detail::ObjectManipulation, "Adding argument : '" + *argument + "'");
	}

        if (next != std::string::npos) {
            last = next + 1;
        } else {
            last = next;
        }
    }
}

void ConfigHost::print() {
    Logging::debugPrint(Logging::Detail::GeneralInfo, "Content of file: " + content_);
}

void ConfigHost::read() {
    std::fstream file;

    if (File::exists(path_)) {
        File::openReadStream(file, path_); 

        File::readLine(file, content_);
        /** Cover the case when there is just empty line in the host config */
        if (content_ == "\n")
            content_ = "";
        file.close();
    } else {

	Logging::debugPrint(Logging::Detail::Important, "File doesn't exist at " + path_);

    }
}

void ConfigHost::setPath(const std::string& config_path) {
    path_ = config_path;

    Logging::debugPrint(Logging::Detail::ObjectManipulation, "Path set to " + path_);
}
