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
    Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe not read, sleeping or checking. Process and is running : "+RunnerUtils::toString(process_)+", "+RunnerUtils::toString(process_ && process_->isRunning()));
      if (process_ && !process_->isRunning()) {
        break;
      }
      sleep(HashcatConstant::ProgressPeriod);
    }
  }

  //Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe read line : " + line);
  return line;
}
