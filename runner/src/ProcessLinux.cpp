/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#if defined(__linux__) || defined(__APPLE__)

#include "ProcessLinux.hpp"

/* Private */

void ProcessLinux::launchSubprocess() {

  /** Execute application - print it before redirection so that it goes to
  * stderr.txt */
  Logging::debugPrint(Logging::Detail::GeneralInfo, "Executing: " + getReadableArguments());

  /** Child doesn't read from the pipes */
  out_pipe_->closeRead();
  err_pipe_->closeRead();

  //FD_CLOEXEC is set on the originals, but that's OK since dup doesn't copy that. New descriptors are not closed on exec
  /** Redirect childs stdout and stderr to the pipes */
  reinterpret_cast<PipeLinux*>(out_pipe_)->redirectWrite(fileno(stdout));
  reinterpret_cast<PipeLinux*>(err_pipe_)->redirectWrite(fileno(stderr));


  if (in_pipe_ != nullptr) {
    reinterpret_cast<PipeLinux*>(in_pipe_)->redirectRead(fileno(stdin));
  }

  std::vector<const char*> linuxArgs;
  linuxArgs.reserve(arguments_.size()+1);
  for(size_t i = 0; i < arguments_.size(); ++i)
  {
    linuxArgs.push_back(arguments_[i].c_str());
  }
  linuxArgs.push_back(nullptr);

  /** Execute application */
  //const cast is OK as explained here: https://stackoverflow.com/a/190208
  execv(linuxArgs[0], const_cast<char *const*>(linuxArgs.data()));

  /** This shouldn't be ever executed but if it does, throw error exception */
  RunnerUtils::runtimeException("execv() failed with", errno);
}

/* Public */

ProcessLinux::ProcessLinux(const std::string& exec_name, const std::vector<std::string>& exec_args, bool is_external_generator) : ProcessBase(exec_name, exec_args) {
  if (is_external_generator) {
    out_pipe_ = PipeLinux::createBlockingPipe();
  } else {
    out_pipe_ = PipeLinux::createNonBlockingPipe();
  }
  err_pipe_ = PipeLinux::createNonBlockingPipe();
  in_pipe_ = nullptr;
  process_identifier_ = -1;
}

ProcessLinux::~ProcessLinux() {
  delete out_pipe_;
  delete err_pipe_;
}

int ProcessLinux::getExitCode() const {
  return WEXITSTATUS(status_);
}

bool ProcessLinux::kill() const {
  return ::kill(process_identifier_, SIGTERM) == 0;
}

int ProcessLinux::getProcessIdentifier() const {
  return process_identifier_;
}

bool ProcessLinux::isRunning() {
  if (process_identifier_ == -1) return false;
  /** Waitpid with WNOHANG returns 0 when process still exists */
  return (waitpid(process_identifier_, &status_, WNOHANG) == 0);
}

int ProcessLinux::run() {
  setStartTime();
  process_identifier_ = fork();

  if (getProcessIdentifier() == -1) {
    RunnerUtils::runtimeException("fork() failed with", errno);
  } else if (process_identifier_ == 0){
    try {

      launchSubprocess();

    } catch (std::exception& e) {
      std::cerr << "Child process failed with std::runtime_error:\n what() : " << e.what() << std::endl;
      /** Exit child process on any exception, don't let child exception to be
      *  caught by the code that should be executed only by parent
      */
      exit(errno);
    }
  } else {
    /** Parrent doesn't write to pipes */
    out_pipe_->closeWrite();
    err_pipe_->closeWrite();
  }

  return 0;
}

void ProcessLinux::killIfRunning() {
  std::string proc = getExecutable();
  if (proc.find(".") == 0)
    proc = proc.erase(0, 2);
  std::string kill_cmd = "pkill -f " + proc;
  int ret = system(kill_cmd.c_str());
  (void)ret;
}

#endif // __linux__
