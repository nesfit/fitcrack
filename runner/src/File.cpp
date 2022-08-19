/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "File.hpp"

/* Private */
void File::init() {
  /** Create file when it doesn't exist */
  if (!exists(relative_path_)) {
    createNew(relative_path_);
  } else {
    relative_path_ = resolveSoftlink();
  }

  if (!isFile(relative_path_)) {
    RunnerUtils::runtimeException(relative_path_ + " is not file");
  }

  FilesysBase::init();
}

/* Public */

File::File() : FilesysBase() {  };

File::File(const std::string& relative_path) : FilesysBase(relative_path) {
  init();
}

File::File(const std::string& relative_path, const std::string& name) : FilesysBase(relative_path, name){
  init();
}

void File::createNew(const std::string& path) {
  Logging::debugPrint(Logging::Detail::DevelDebug, "Creating new file " + path);
  std::fstream new_file(path.c_str(), std::ios::out);
  new_file.close();
}

bool File::exists(const std::string& path) {
  return isFile(path);
}

int File::openReadStream(std::fstream &file, const std::string &file_name, std::ios_base::openmode mode) {
  // we want to set eof and get to the end file and detect it withou exception
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  return openStream(file, std::ios_base::in | mode, file_name);
}

int File::openWriteStream(std::fstream &file, const std::string &file_name, std::ios_base::openmode mode) {
  file.exceptions (std::ofstream::failbit | std::ofstream::badbit);
  return openStream(file, std::ios_base::out | mode, file_name);
}

std::string File::parseExtension(const std::string& path) {
  std::size_t found_at = path.find_last_of(".");
  if (found_at != std::string::npos) {
    return path.substr(found_at+1);
  } else {
    return std::string("");
  }
}

void File::parseSoftlinkFileContent(std::string& path) {
  std::string start_tag   = "<soft_link>";
  std::string end_tag	    = "</soft_link>";

  // Cut the value from the tagged element
  if (path.find(start_tag) != std::string::npos && path.find(end_tag) != std::string::npos) {
    path = path.substr(start_tag.size(), path.size() - (start_tag.size() + end_tag.size()));
  } else {
    path = "";
  }
}

std::string File::resolveSoftlink() {
  std::string path_buffer;
  std::fstream soft_link_file;

  try {
    openReadStream(soft_link_file, relative_path_);
  } catch ( std::ios_base::failure &e ) {
    Logging::debugPrint(Logging::Detail::CustomOutput, "Can't resolve file " + relative_path_);
    return relative_path_;
  }

  readLine(soft_link_file, path_buffer);

  parseSoftlinkFileContent(path_buffer);

  if (path_buffer.empty()) {
    return relative_path_;
  } else {
    Logging::debugPrint(Logging::Detail::DevelDebug, "Resolving softlink relative path: " + relative_path_ + " resolved path: " + path_buffer);
    if (!File::isFile(path_buffer)) {
      createNew(path_buffer);
    }
    return path_buffer;
  }
}

int File::openStream(std::fstream &file, std::fstream::openmode mode, const std::string &file_name) {
  Logging::debugPrint(Logging::Detail::DevelDebug, "Trying to open fstream: " + file_name);

  file.open(file_name.c_str(), mode);
  if (!file.is_open()) {
    RunnerUtils::runtimeException("open() failed", errno);
  }

  Logging::debugPrint(Logging::Detail::DevelDebug, "Sucessfully opened fstream: " + file_name);

  return 0;
}

std::fstream& File::readLine(std::fstream& input_stream, std::string& line) {
  if (!line.empty())
  line.clear();
  /**
  * The characters in the stream are read one-by-one using a std::streambuf.
  * It is faster than reading them one-by-one using the std::istream.
  * Code that uses streambuf this way must be guarded by a sentry object.
  * The sentry object performs various tasks, such as thread
  * synchronization and updating the stream state.
  */

  std::istream::sentry se(input_stream, true);
  std::streambuf* sb = input_stream.rdbuf();

  for(;;) {
    int c = sb->sbumpc();
    switch (c) {
      case '\n':
      return input_stream;
      case '\r':
      if(sb->sgetc() == '\n')
      sb->sbumpc();
      return input_stream;
      case EOF:
      /** Also handle the case when the last line has no line ending */
      if(line.empty())
      input_stream.setstate(std::ios::eofbit);
      return input_stream;
      default:
      line += (char)c;
    }
  }

  return input_stream;
}

void File::setExecutableMode() {
#if defined(__linux__) || defined(__APPLE__)
  if (chmod(getRelativePath().c_str(), 0755) == -1) {
    RunnerUtils::runtimeException("chmod() failed for file: " + name_);
  }
#endif
}
