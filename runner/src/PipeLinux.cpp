/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#if defined(__linux__) || defined(__APPLE__)

#include "PipeLinux.hpp"

#include <poll.h>

/* Private */

bool PipeLinux::isEndOpen(int file_descriptor) const {
  /** Checks if file descriptor is open and errno isn't containing error */
  return fcntl(file_descriptor, F_GETFD) != -1 || errno != EBADF;
}

void PipeLinux::redirectPipeEnd(int pipe_end, int target) {
  if (dup2(pipe_end, target) < 0) {
    RunnerUtils::runtimeException("dup2 failed with target", errno);
  }
}

int PipeLinux::closeEnd(int pipe_end) {
  /** Check if pipe end is still open before closing it */
  if (isEndOpen(pipe_end))
  return close(pipe_end);
  else
  return 0;
}
/* Protected */

void PipeLinux::createPipe(bool is_NONBLOCK_) {
  /** Prevent replacement of active pipe byt new one */
  if (read_ == 0 && write_ == 0) {
    std::vector<int> file_descriptor(2);

    /** Create new pipe */
    if (pipe(file_descriptor.data()) < 0) {
      RunnerUtils::runtimeException("pipe failed", errno);
    }

    /** Set pipe to non-blocking state */
    if(is_NONBLOCK_){
      if (fcntl(file_descriptor[0], F_SETFL, fcntl(file_descriptor[0], F_GETFL) | O_NONBLOCK) < 0) {
        RunnerUtils::runtimeException("fcntl failed", errno);
      }
    }
    else{
      if (fcntl(file_descriptor[0], F_SETFL, fcntl(file_descriptor[0], F_GETFL)) < 0) {
        RunnerUtils::runtimeException("fcntl failed", errno);
      }
    }
    //make sure descriptors are not inherited accidentally
    for(int i = 0; i < 2; ++i)
    {
      if (fcntl(file_descriptor[i], F_SETFD, fcntl(file_descriptor[i], F_GETFD)|FD_CLOEXEC) < 0) {
        RunnerUtils::runtimeException("fcntl failed", errno);
      }
    }
    /** Save values into object member properties */
    read_ = file_descriptor[0];
    write_ = file_descriptor[1];
  }
}


bool PipeLinux::isReadOpen() const {
  return isEndOpen(read_);
}

/*int PipeLinux::getReadFD(){
return read_;
}
*/
bool PipeLinux::isWriteOpen() const {
  return isEndOpen(write_);
}

void PipeLinux::waitForAvailableOutput() const
{
  pollfd pollstruct;
  pollstruct.fd = read_;
  pollstruct.events = POLLIN;
  pollstruct.revents = 0;
  do
  {
    int res = poll(&pollstruct, 1, -1);
    if(res == -1 && errno != EINTR)
    {
      RunnerUtils::runtimeException("poll failed unexpectedly", errno);
    }
  } while((pollstruct.revents&(POLLHUP|POLLIN)) == 0);
}

int PipeLinux::readChar(char& c) {
  // Based on article
  // https://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/

  int read_size = READ_SIZE;
  int n_read_chars = 0;
  std::vector<char> ch(read_size);

  n_read_chars = read(read_, ch.data(), read_size);

  switch (n_read_chars) {
    case -1:
    /* Nothing new to read from pipe */
    if (errno == EAGAIN) {
      // sleep(1);
      break;
    } else {
      RunnerUtils::runtimeException("read() failed", errno);
      return 0;
    }
    case 0:
    closeRead();
    break;
    default:
    c = ch[0];
  }

  return n_read_chars;
}

int PipeLinux::readChar()
{
  waitForAvailableOutput();
  char c;
  int res;
  if((res = readChar(c)) != 1)
  {
    if(res == 0)
    {
      return EOF;
    }
    RunnerUtils::runtimeException("readChar failed even when pipe should be readable", errno);
  }
  return c;
}

/* Public */

PipeLinux::PipeLinux(bool is_NONBLOCK_) : read_(0), write_(0) {
  createPipe(is_NONBLOCK_);
}

PipeLinux::~PipeLinux() {
  closeAll();
}

bool PipeLinux::canRead() const {
  return isReadOpen();
}

int PipeLinux::closeRead() {
  int err = closeEnd(read_);
  if (err)
  read_ = 0;
  return err;
}

int PipeLinux::closeWrite() {
  int err = closeEnd(write_);
  if (err)
  write_ = 0;
  return err;
}

void PipeLinux::redirectRead(int target) {
  redirectPipeEnd(read_, target);
}

void PipeLinux::redirectWrite(int target) {
  redirectPipeEnd(write_, target);
}

int PipeLinux::writeMessage(std::string& message) {
  Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe writing message : " + message);
  int result = write(write_, message.data(), message.length());
  if (result == -1) {
    if (errno == EPIPE) {
      return -1;
    } else {
      RunnerUtils::runtimeException("writeMessage: broken pipe", errno);
    }
  }
  return result;
}

PipeBase *PipeLinux::createBlockingPipe() { return new PipeLinux(false); }
PipeBase *PipeLinux::createNonBlockingPipe() { return new PipeLinux(true); }

#endif // __linux__
