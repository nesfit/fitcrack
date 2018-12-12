/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "FileZip.hpp"
/** Private */

void FileZip::decodeEntry(size_t entry_number, zip_stat_t *entry_stats) {

    std::fstream file_descriptor; 
    std::string entry_name(entry_stats->name);
    std::vector<char> data_buffer;

    size_t read_bytes = 0;
    size_t written_bytes = 0;

    zip_file_t *entry = NULL;

    normalizePath(entry_name);

    data_buffer.resize(entry_stats->size);

    unsigned long long start = RunnerUtils::timeInMs();

    if (!(entry = zip_fopen_index(handle_, entry_number, ZIP_FL_ENC_GUESS))) {
        RunnerUtils::runtimeException("zip_fopen_index() failed during extraction on entry: " + entry_name, errno);
    }

    openWriteStream(file_descriptor, entry_name, std::ios_base::binary | std::ios_base::trunc);

    if ((read_bytes = zip_fread(entry, data_buffer.data(), entry_stats->size)) < 0) {
	RunnerUtils::runtimeException("zip_fread() failed during extraction on entry: " + entry_name, errno);
    }

    Logging::debugPrint(Logging::Detail::DevelDebug, 
	    "compressed size : " + RunnerUtils::toString(entry_stats->comp_size) + 
	    " real size : " + RunnerUtils::toString(entry_stats->size) +
	    " buffer size : " + RunnerUtils::toString(data_buffer.size()) +
	    " read size : " + RunnerUtils::toString(read_bytes)
	    );

    file_descriptor.write(data_buffer.data(), data_buffer.size());

    file_descriptor.close();

    Logging::debugPrint(Logging::Detail::DevelDebug, " entry decoding lasted: " + RunnerUtils::toString(RunnerUtils::timeInMs() - start) + "ms");

    if (zip_fclose(entry) < 0) {
        RunnerUtils::runtimeException("zip_fclose() failed during extraction on entry: " + std::string(entry_name), errno);
    }
}

void FileZip::extractEntry(size_t entry_number) {
    zip_stat_t entry_stats;

    unsigned long long start = RunnerUtils::timeInMs();
    if (zip_stat_index(handle_, entry_number, 0, &entry_stats) == 0) {
        std::string entry_name(current_directory_ + std::string(entry_stats.name));
	FilesysBase::normalizePath(entry_name);

	Logging::debugPrint(Logging::Detail::DevelDebug, " extracting file " + entry_name);

	size_t path_to_entry_end = entry_name.rfind(path_delimiter_);
	Directory::makePath(entry_name.substr(0, path_to_entry_end));

        if (entry_name[entry_name.length() -1] != path_delimiter_[0]) {
            decodeEntry(entry_number, &entry_stats);
        }
    }
    Logging::debugPrint(Logging::Detail::DevelDebug, " entry extraction lasted: " + RunnerUtils::toString(RunnerUtils::timeInMs() - start) + "ms");
}

/** Public */

FileZip::FileZip(const std::string& relative_path) : File(relative_path), flag_opened_(false), error_(0), handle_(NULL) {  };
FileZip::FileZip(const std::string& relative_path, const std::string& name) : File(relative_path, name), flag_opened_(false), error_(0), handle_(NULL) {  };
FileZip::FileZip(const File& file) : File(file), flag_opened_(false), error_(0), handle_(NULL) {  };

void FileZip::close() {
    if (zip_close(handle_) < 0) {
        RunnerUtils::runtimeException("zip_close() failed on archive:" + getRelativePath(), errno);
    }

    flag_opened_ = false;
}

void FileZip::extract() {
    bool hold_opened_flag = isOpen();
    if (!isOpen())
        open();

    size_t entries = zip_get_num_entries(handle_, ZIP_FL_ENC_GUESS);
    for (size_t entry_number = 0; entry_number < entries; entry_number++) {
        extractEntry(entry_number);
    }

    Logging::debugPrint(Logging::Detail::GeneralInfo, "FileZip: extracted " + RunnerUtils::toString(entries) + " entries from archive " + getRelativePath());

    /** Prevention of closing the archive which was opened out side of this
     * function */
    if (isOpen() && !hold_opened_flag)
        close();
}

bool FileZip::isOpen() const {
    return flag_opened_;
}

void FileZip::open() {
    if ((handle_ = zip_open(getRelativePath().c_str(), 0, &error_)) == NULL) {
        RunnerUtils::runtimeException("zip_open() failed on archive:" + getRelativePath() + " with zip_error code " + RunnerUtils::toString(error_), errno);
    }

    flag_opened_ = true;
}

