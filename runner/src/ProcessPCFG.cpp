/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "ProcessPCFG.hpp"

ProcessBase* ProcessPCFG::create(std::vector<char*>& arguments ,Directory& directory) {
  File executable;
  #ifdef PROCESSLINUX_HPP
  directory.findVersionedFile("pcfg", "bin", executable);
  executable.setExecutableMode();
  return new ProcessLinux(executable.getRelativePath(), arguments, true);
  #elif defined (PROCESSWINDOWS_HPP)
  directory.findVersionedFile("pcfg", "exe", executable);
  return new ProcessWindows(executable.getRelativePath(), arguments, true);
  #else
  #error "No viable Process implementation. This is caused by your target platfrom. Supported platforms are __linux__, _WIN32."
  #endif
}
