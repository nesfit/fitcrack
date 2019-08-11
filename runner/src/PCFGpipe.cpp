/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */
#include "PCFGpipe.hpp"

int PCFGpipe::createNamedPipe(const char* pipename){
  int fd;
  mkfifo(pipename, 0777);
  //returns fd of named pipe
  fd = open(pipename, O_RDWR);
  printf("fd in open is: %d\n", fd);
  return fd;
}

void PCFGpipe::closeNamedPipe(int pipefd){
  int err_code;
  printf("fd in close is: %d\n", pipefd);
  err_code = close(pipefd);
  printf("err close code is: %d\n", err_code);
}
