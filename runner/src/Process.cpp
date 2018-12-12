/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "Process.hpp"

ProcessBase* Process::create(std::vector<char*>& arguments, Directory& directory) {
    File executable;
#ifdef PROCESSLINUX_HPP
    directory.findVersionedFile("hashcat", "bin", executable);
    executable.setExecutableMode();
    return new ProcessLinux(executable.getRelativePath(), arguments);
#elif defined (PROCESSWINDOWS_HPP)
    directory.findVersionedFile("hashcat", "exe", executable);
    return new ProcessWindows(executable.getRelativePath(), arguments);
#else
  #error "No viable Process implementation. This is caused by your target platfrom. Supported platforms are __linux__, _WIN32."
#endif
}

