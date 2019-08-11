/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef PCFGPIPE_HPP
#define PCFGPIPE_HPP

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/** Namespace with function for creating named pipe for pcfg attack */
namespace PCFGpipe {

  int createNamedPipe(const char * pipename);

  void closeNamedPipe(int pipefd);
};

#endif // PCFGPIPE_HPP
