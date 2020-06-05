/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#ifdef __WIN32

#include "ProcessWindows.hpp"

/* Protected */

void ProcessWindows::launchSubprocess() {
  std::string command;
  command = getReadableArguments();

  /** Execute application - print it before redirection so that it goes to
  * stderr.txt */
  Logging::debugPrint(Logging::Detail::GeneralInfo, "Executing: " + command);

  if (in_pipe_)
    startup_info_.hStdInput  = static_cast<PipeWindows*>(in_pipe_)->getReadHandle();

  if (!SetHandleInformation(startup_info_.hStdError, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) {
    RunnerUtils::runtimeException("SetHandleInformation() for stderr failed", GetLastError());
  }

  if (!SetHandleInformation(startup_info_.hStdOutput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) {
    RunnerUtils::runtimeException("SetHandleInformation() for stdout failed", GetLastError());
  }

  if (startup_info_.hStdInput != GetStdHandle(STD_INPUT_HANDLE) && !SetHandleInformation(startup_info_.hStdInput, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) {
    RunnerUtils::runtimeException("SetHandleInformation() for stdin failed", GetLastError());
  }

  /** Start the child process */
  if(!CreateProcess( NULL,   // No module name (use command line)
  (char*)(command.c_str()),        // Command line
  NULL,           // Process handle not inheritable
  NULL,           // Thread handle not inheritable
  TRUE,           // Set handle inheritance to FALSE
  0,              // No creation flags
  NULL,           // Use parent's environment block
  NULL,           // Use parent's starting directory
  &startup_info_,            // Pointer to STARTUPINFO structure
  &process_information_)           // Pointer to PROCESS_INFORMATION structure
  ) {
    err_pipe_->closeAll();
    out_pipe_->closeAll();
    RunnerUtils::runtimeException("CreateProcess() failed", GetLastError());
  }

  /* Close the pipes we passed on to the child */
  err_pipe_->closeWrite();
  out_pipe_->closeWrite();
  if(in_pipe_) {
    in_pipe_->closeRead();
  }
}
/* Public */

ProcessWindows::ProcessWindows(const std::string& exec_name, const std::vector<std::string>& exec_args, bool is_external_generator) : ProcessBase(exec_name, exec_args) {

  (void)is_external_generator;
  out_pipe_ = PipeWindows::createBlockingPipe();
  err_pipe_ = PipeWindows::createBlockingPipe();
  in_pipe_ = nullptr;

  ZeroMemory(&startup_info_, sizeof(startup_info_));
  ZeroMemory(&process_information_, sizeof(process_information_));

  startup_info_.cb = sizeof(STARTUPINFO);
  startup_info_.hStdError  = static_cast<PipeWindows*>(err_pipe_)->getWriteHandle();
  startup_info_.hStdOutput = static_cast<PipeWindows*>(out_pipe_)->getWriteHandle();
  startup_info_.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  startup_info_.dwFlags |= STARTF_USESTDHANDLES;
}

ProcessWindows::~ProcessWindows() {
  CloseHandle(process_information_.hProcess);
  CloseHandle(process_information_.hThread);
}

int ProcessWindows::getExitCode() const {
  DWORD exit_code;

  Logging::debugPrint(Logging::Detail::DevelDebug, "ProcessWindows : trying to get exit_code.");

  if(GetExitCodeProcess(process_information_.hProcess, &exit_code) == 0) {
    RunnerUtils::runtimeException("GetExitCodeProcess() failed", GetLastError());
  }

  Logging::debugPrint(Logging::Detail::DevelDebug, "ProcessWindows : got exit_code : " + RunnerUtils::toString(exit_code) +
  " which as int is : " + RunnerUtils::toString((int)exit_code)
);

return (int)exit_code;
}

bool ProcessWindows::isRunning() {
  DWORD exit_code;
  GetExitCodeProcess(process_information_.hProcess, &exit_code);

  return exit_code == STILL_ACTIVE;
}

int ProcessWindows::run() {
  killIfRunning();

  setStartTime();
  launchSubprocess();
  return 0;
}

void ProcessWindows::killIfRunning() {
  std::string proc = getExecutable();
  if (proc.find(".") == 0)
    proc = proc.erase(0, 2);
  std::string kill_cmd = "taskkill /F /IM " + proc + " >nul 2>&1";
  int ret = system(kill_cmd.c_str());
  (void)ret;
}

#endif // __WIN32
