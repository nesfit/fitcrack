/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "FilesysBase.hpp"

/* Private */
void FilesysBase::initialize() {
    current_directory_ = std::string("." ) + path_delimiter_;
    parent_directory_  = std::string("..") + path_delimiter_;
}

void FilesysBase::simplifyPath(std::string& path) {

    std::size_t rfound;
    std::size_t found;
    std::string current_directory = path_delimiter_ + current_directory_;
    std::string parent_directory  = path_delimiter_ + parent_directory_;

    Logging::debugPrint(Logging::Detail::DevelDebug, "Simplifying path " + path);

    if ((found = path.find(current_directory)) != std::string::npos) {
	Logging::debugPrint(Logging::Detail::DevelDebug, path + " found " + current_directory + " at index " + RunnerUtils::toString(found));
	Logging::debugPrint(Logging::Detail::DevelDebug, "Current directory size " + RunnerUtils::toString(current_directory.size()) + " replacing till " +RunnerUtils::toString(current_directory.size()-1));
        path.replace(found, current_directory.size(), path_delimiter_);
    }
    Logging::debugPrint(Logging::Detail::DevelDebug, "After curr_dir removal " + path );
    
    while ((found = path.find(parent_directory)) != std::string::npos) {
	rfound = path.rfind(path_delimiter_, found-1) + 1;	// Point to the first letter after /
        path.replace(rfound, (found + parent_directory.size()) - rfound, "");
    }

    Logging::debugPrint(Logging::Detail::DevelDebug, "Simplified path " + path );
}

/* Protected */
void FilesysBase::init(){
    relativeToAbsolute(relative_path_, absolute_path_);
    if (name_.empty()) {
	setName();
    }
}

std::string FilesysBase::parseName(const std::string& path) {
    // based on http://www.cplusplus.com/reference/string/string/find_last_of/
    Logging::debugPrint(Logging::Detail::DevelDebug, "Parsing name from: " + path );

    std::size_t found_at = path.find_last_of(path_delimiter_);
    if (found_at != std::string::npos)
	return path.substr(found_at+1);
    else
	return path;
}

/* Public */
FilesysBase::FilesysBase() : path_delimiter_(PATH_DELIMITER) { 
    initialize();
};

FilesysBase::FilesysBase(std::string path) : relative_path_(path), path_delimiter_(PATH_DELIMITER) { 
    initialize();
};

FilesysBase::FilesysBase(std::string path, std::string name) : relative_path_(path), name_(name), path_delimiter_(PATH_DELIMITER)  { 
    initialize();
};

void FilesysBase::changeTo(const std::string& relative_path) {
    relative_path_ = relative_path;
    init();
}

std::string FilesysBase::getName() const {
    return name_;
}

std::string FilesysBase::getAbsolutePath() const{
    return absolute_path_;
}

std::string FilesysBase::getRelativePath() const {
    return relative_path_;
}

bool FilesysBase::isDirectory(const std::string& path) {
    struct stat statbuf;

    if (stat(path.c_str(), &statbuf) != 0) {
	Logging::debugPrint(Logging::Detail::DevelDebug, path + " does not exist.");
        return false;
    } else if (statbuf.st_mode & S_IFDIR) {
	Logging::debugPrint(Logging::Detail::DevelDebug, path + " does exist.");
	return true;
    } else {
	Logging::debugPrint(Logging::Detail::DevelDebug, path + " is not a directory.");
	return false;
    }
}

bool FilesysBase::isFile(const std::string& path) {
    struct stat statbuf;

    if (stat(path.c_str(), &statbuf) != 0) {
	Logging::debugPrint(Logging::Detail::DevelDebug, path + " does not exist.");
        return false;
    } else if (statbuf.st_mode & S_IFREG) {
	return true;
    } else {
	Logging::debugPrint(Logging::Detail::DevelDebug, path + " is not a file.");
	return false;
    }
}

void FilesysBase::normalizePath(std::string& path) {
    size_t found_at;

    while (PATH_DELIMITER != "/" && (found_at = path.find("/", 0)) != std::string::npos) {
	path.replace(found_at, 1, std::string(PATH_DELIMITER));
    }
}

void FilesysBase::relativeToAbsolute(const std::string& relative_path, std::string& absolute_path) {
    // !!! WARNING: path is limited to PATH_MAX size set in the system
    if (relative_path.size() > 0 && relative_path[0] == '.') {
        char path_buffer[PATH_MAX];
	if (getcwd(path_buffer, PATH_MAX) != path_buffer) {
	    Logging::debugPrint(Logging::Detail::DevelDebug, std::string(path_buffer) + ": No such file or directory.");
	    RunnerUtils::runtimeException("getcwd() failed: ", errno);
	}

        absolute_path = std::string(path_buffer);
	if (*absolute_path.end() != path_delimiter_[0]) {
	    absolute_path += path_delimiter_;
	}
	absolute_path += relative_path;

    } else {
        absolute_path = relative_path;
    }

    Logging::debugPrint(Logging::Detail::DevelDebug, "Transforming relative path: " + relative_path + " to absolute path: " + absolute_path);

    simplifyPath(absolute_path);
}

void FilesysBase::setName() {
    setName(absolute_path_);
}

void FilesysBase::setName(const std::string& path) {

    name_ = parseName(path);

    Logging::debugPrint(Logging::Detail::DevelDebug, "Name set to: " + name_);
}
