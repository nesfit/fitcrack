/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#ifdef _WIN32

#include "PipeWindows.hpp"

/* Private */

bool PipeWindows::isEndOpen(HANDLE file_descriptor) const {
  return file_descriptor != NULL;
}

int PipeWindows::closeEnd(HANDLE pipe_end) {
  /** Check if pipe end is still open before closing it */
  int err = 0;
  if (isEndOpen(pipe_end)) {
    err = CloseHandle(pipe_end);
    if (err == 0) {
      pipe_end = NULL;
    }
  }
  return err;
}

/* Protected */

void PipeWindows::createPipe(bool is_NONBLOCK_) {
  /** Prepare security attributes */
  SECURITY_ATTRIBUTES security_attributes;

  security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  security_attributes.bInheritHandle = FALSE;
  security_attributes.lpSecurityDescriptor = NULL;

  if (!CreatePipe(&read_, &write_, &security_attributes, 0))
  RunnerUtils::runtimeException("CreatePipe() failed", GetLastError());

  if (is_NONBLOCK_) {
    DWORD mode = PIPE_NOWAIT;
    if (!SetNamedPipeHandleState(read_, &mode, NULL, NULL)) {
       RunnerUtils::runtimeException("SetNamedPipeHandleState() failed", GetLastError());
    }
  }
}

int PipeWindows::readChar(char& c) {

  unsigned long read_size = READ_SIZE;
  unsigned long n_read_chars = 0;

  /** Try to read N chars but don't really read it yet */
  if (PeekNamedPipe(read_, NULL, read_size, NULL, &n_read_chars, NULL) == 0) {
    if ((error_ = GetLastError()) == ERROR_BROKEN_PIPE) {
      Logging::debugPrint(Logging::Detail::DevelDebug, POSITION_IN_CODE + " Got ERROR_BROKEN_PIPE: ");
    } else {
      RunnerUtils::runtimeException("PeekPipe() failed", GetLastError());
    }
  }

  Logging::debugPrint(Logging::Detail::DevelDebug, POSITION_IN_CODE + " Characters to read: " + RunnerUtils::toString(n_read_chars));

  if (n_read_chars >= read_size) {
    //call the blocking version since we know a char is ready
    c = readChar();
    Logging::debugPrint(Logging::Detail::DevelDebug, POSITION_IN_CODE + " Characters read: " + RunnerUtils::toString(n_read_chars));
    return n_read_chars;
  } else {
    return -1;
  }
}

int PipeWindows::readChar()
{
  DWORD n_read_chars;
  char c;
  if (ReadFile(read_, &c, sizeof(c), &n_read_chars, NULL) == 0) {
    if ((error_ = GetLastError()) == ERROR_BROKEN_PIPE) {
      return EOF;
    } else {
      RunnerUtils::runtimeException("ReadFile() failed", GetLastError());
    }
  }
  return c;
}

/* Public */

PipeWindows::PipeWindows(bool is_NONBLOCK_) : error_(ERROR_SUCCESS), read_(NULL), write_(NULL) {
  createPipe(is_NONBLOCK_);
}

PipeWindows::~PipeWindows() {
  closeAll();
}

bool PipeWindows::canRead() const {
  return error_ == ERROR_SUCCESS;
}

int PipeWindows::closeRead() {
  return closeEnd(read_);
}

int PipeWindows::closeWrite() {
  return closeEnd(write_);
}

HANDLE PipeWindows::getReadHandle () const {
  return read_;
}

HANDLE PipeWindows::getWriteHandle () const {
  return write_;
}

bool PipeWindows::isReadOpen() const {
  return isEndOpen(read_);
}

bool PipeWindows::isWriteOpen() const {
  return isEndOpen(write_);
}

void PipeWindows::waitForAvailableOutput() const
{
  DWORD availableChars = 0;
  while(true)
  {
    if (PeekNamedPipe(read_, NULL, 0, NULL, &availableChars, NULL) == 0) {
      if (GetLastError() == ERROR_BROKEN_PIPE) {
        Logging::debugPrint(Logging::Detail::DevelDebug, POSITION_IN_CODE + " Got ERROR_BROKEN_PIPE: ");
        return;
      } else {
        RunnerUtils::runtimeException("PeekPipe() failed", GetLastError());
      }
    }
    if(availableChars > 0)
    {
      return;
    }
    //sleep for half a second, then try again
    Sleep(500);
  }
}

int PipeWindows::writeMessage(std::string& message) {
  DWORD written_chars = 0;

  Logging::debugPrint(Logging::Detail::DevelDebug, POSITION_IN_CODE + "Pipe writing message : " + message);

  if (!WriteFile(write_, message.data(), message.length(), &written_chars, NULL)) {
    DWORD error = GetLastError();
    if(error == ERROR_BROKEN_PIPE)
    {
      return -1;
    }
    else
    {
      RunnerUtils::runtimeException("WriteFile() failed", GetLastError());
    }
  }

  return written_chars;
}

PipeBase *PipeWindows::createBlockingPipe() { return new PipeWindows(false); }
PipeBase *PipeWindows::createNonBlockingPipe() { return new PipeWindows(true); }
#endif // _WIN32
