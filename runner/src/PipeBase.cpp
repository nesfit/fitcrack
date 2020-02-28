/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "PipeBase.hpp"
#include "ProcessBase.hpp"

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
  char c = 0;

  while (c != '\n' && canRead()) {
      c = readChar();
      if(c == EOF)
      {
        break;
      }
      line += c;
  }

  //Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe read line : " + line);
  return line;
}
