/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef PROCESS_HPP 
#define PROCESS_HPP

#include "Directory.hpp"

#include "File.hpp"

#include "ProcessLinux.hpp"
#include "ProcessWindows.hpp"

/** Namespace for creating new process via function */
namespace Process {

    /**
     * @brief   Creates new platform based process 
     * @param   arguments [in] Arguments to run hashcat with
     * @param   directory [in] Working directory
     * @return  Pointer to new Process object
     */
    ProcessBase* create(std::vector<char*>& arguments, Directory& directory);
};
#endif // PROCESS_HPP
