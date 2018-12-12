/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "Directory.hpp"
/* Private */
void Directory::init() {
    if (!relative_path_.empty() && !isDirectory(relative_path_))
	RunnerUtils::runtimeException("stat() failed on " + relative_path_, errno);
    FilesysBase::init();
}

/* Public */
Directory::Directory() : FilesysBase() {  }

Directory::Directory(const std::string& path) : FilesysBase(path) {
    init();
}

Directory::Directory(const std::string& path, const std::string& name) : FilesysBase(path, name) { 
    init();
}

bool Directory::exists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }

    return (info.st_mode & IFDIR_FLAG) != 0;
}

bool Directory::find(const std::string& key, Directory& value) const {

    // TODO: move this to the generic function and use it in ConfigTask too,

    /** Find item in the list and copy the stored value to 'value' */
    if (hasDirectories()) {

	std::map<std::string, Directory>::const_iterator found_it;

	found_it = directories_.find(key);
	if (found_it == directories_.end()) {
	    return false;
	}

        value = found_it->second;

	Logging::debugPrint(Logging::Detail::CustomOutput, " found subdirectory : " + value.getName() + "->" + value.getRelativePath());
    }
    return true;
}

bool Directory::find(const std::string& key, File& value) const {

    // TODO: move this to the generic function and use it in ConfigTask too,
    // alse make common interface for Directory and File for getting name, path
    // and such thing they have in common

    /** Find item in the list and copy the stored value to 'value' */
    if (hasFiles()) {
	std::map<std::string, File>::const_iterator found_it;

	found_it = files_.find(key);
	if (found_it == files_.end()) {
	    return false;
	}

        value = found_it->second;

	Logging::debugPrint(Logging::Detail::CustomOutput, " found file : " + value.getName() /* + "->" + value.getRelativePath()*/ );
    }
    return true;
}

bool Directory::findVersionedFile(const std::string& prefix, const std::string& extension, File& file) {
    for (std::map<std::string, File>::iterator it = files_.begin(); it != files_.end(); it++) {
        if (it->first.find(prefix, 0) == 0 && it->first.rfind(extension) == it->first.length() - extension.size()) {
            file = it->second;
	Logging::debugPrint(Logging::Detail::CustomOutput, " found versioned file : " + file.getName() /* + "->" + file.getRelativePath() */);
            return true;
        }
    }
    return false;
}

bool Directory::hasDirectories() const {
    return (directories_.size() > 0);
}

bool Directory::hasFiles() const {
    return (files_.size() > 0);
}

void Directory::scanForEntities() {

    if (!directories_.empty())
	directories_.clear();
    if (!files_.empty())
	files_.clear();

    if (relative_path_[relative_path_.size()-1] != path_delimiter_[0])
	relative_path_ += path_delimiter_;

    DIR *directory;
    struct dirent *entity;
    if ((directory = opendir (absolute_path_.c_str())) != NULL) {
        while ((entity = readdir (directory)) != NULL) {
            if (isDirectory(entity->d_name)) {
                directories_.insert(std::pair<std::string, Directory>(entity->d_name, Directory(relative_path_ + entity->d_name)));
            } else if (isFile(entity->d_name)) {
                files_.insert(std::pair<std::string, File>(entity->d_name, File(relative_path_ + entity->d_name)));
            }
        }
        closedir (directory);

    } else {
        /** Couldn't open directory */
        RunnerUtils::runtimeException("opendir() failed", errno);
    }
}

void Directory::makePath(const std::string& path) {
    int occurence_index;
    if (mkdir(path.c_str() DIR_CREATE_FLAGS) == 0) { // Missing comma isn't typo
	Logging::debugPrint(Logging::Detail::CustomOutput, "Created directory " + path);
        return;
    } 

    switch (errno)
    {
        case ENOENT:
            /** parent doesn't exist, try to create it */
            {
                if ((occurence_index = path.find_last_of(PATH_DELIMITER)) == std::string::npos)
                    RunnerUtils::runtimeException("mkdir() failed ", errno);
                makePath(path.substr(0, occurence_index));
            }
            /** All predecessors were create , try to create child again */
            makePath(path);
            break;

        case EEXIST:
	    Logging::debugPrint(Logging::Detail::DevelDebug, "Directory " + path + " already exists");
            return;

        default:
            RunnerUtils::runtimeException("mkdir() failed ", errno);
    }
}

void Directory::printDirectories() {
    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "Directories in directory " + name_ + ": ");

    for (std::map<std::string, Directory>::iterator it = directories_.begin(); it != directories_.end(); it++) {
	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "|| " + it->first);
//	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "||\t" + it->second.getPath()); 
    }

    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "Directory contains " + RunnerUtils::toString(directories_.size()) + " directories.") ;
}

void Directory::printFiles() {
    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "Files in directory " + name_ + ": ");

    for (std::map<std::string, File>::iterator it = files_.begin(); it != files_.end(); it++) {
	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "|| " + it->first);
//	Logging::debugPrint(Logging::Detail::ObjectContentRevision, "||\t" + it->second.getRelativePath() );
    }

    Logging::debugPrint(Logging::Detail::ObjectContentRevision, "Directory contains " + RunnerUtils::toString(files_.size()) + " files.");
}

