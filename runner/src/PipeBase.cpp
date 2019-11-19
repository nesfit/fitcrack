/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "PipeBase.hpp"
#ifndef PROCESSBASE_HPP
#include "ProcessBase.hpp"
#endif

/* Public */

int PipeBase::closeAll() {
  int err;
  if ((err = closeRead()) != 0)
  return err;
  if ((err = closeWrite()) != 0)
  return err;
  return 0;
}

std::string PipeBase::readLine() {
  std::string line;
  char c;
  int read_chars = -1;

  while (canRead()) {
    read_chars = readChar(c);
    if (read_chars > 0) {
      line += c;
      if (c == '\n') break;
    } else if (read_chars == -1) {
      sleep(HashcatConstant::ProgressPeriod);
    }
  }

  return line;
}

std::string PipeBase::readLine(ProcessBase* process_) {
  std::string line;
  char c;
  int read_chars = -1;

  while (canRead()) {
    read_chars = readChar(c);
    if (read_chars > 0) {
      line += c;
      if (c == '\n') break;
    } else if (read_chars == -1) {
      if(!process_->isRunning()){
        break;
      }
      sleep(HashcatConstant::ProgressPeriod);
    }
  }

  //Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe read line : " + line);
  return line;
}

PipeBase::~PipeBase() {
  // Nothing
}
