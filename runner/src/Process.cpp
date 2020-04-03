/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "Process.hpp"

#include "File.hpp"

#include "ProcessLinux.hpp"
#include "ProcessWindows.hpp"

ProcessBase* Process::create(const std::vector<std::string>& arguments, Directory& directory) {
  return Process::create("hashcat", arguments, directory);
}

ProcessBase* Process::create(std::string prefix, const std::vector<std::string>& arguments, Directory &directory)
{
  File executable;
  #ifdef PROCESSLINUX_HPP
  directory.findVersionedFile(prefix, "bin", executable);
  executable.setExecutableMode();
  return new ProcessLinux(executable.getRelativePath(), arguments, false);
  #elif defined (PROCESSWINDOWS_HPP)
  directory.findVersionedFile(prefix, "exe", executable);
  return new ProcessWindows(executable.getRelativePath(), arguments, false);
  #else
  #error "No viable Process implementation. This is caused by your target platfrom. Supported platforms are __linux__, _WIN32."
  #endif
}
