/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef PROCESS_HPP 
#define PROCESS_HPP

#include "Directory.hpp"
#include "ProcessBase.hpp"

/** Namespace for creating new process via function */
namespace Process {

    /**
     * @brief   Creates new platform based process 
     * @param   arguments [in] Arguments to run hashcat with
     * @param   directory [in] Working directory
     * @return  Pointer to new Process object
     */
    ProcessBase* create(const std::vector<std::string>& arguments, Directory& directory);
    ProcessBase* create(std::string prefix, const std::vector<std::string>& arguments, Directory &directory);
};
#endif // PROCESS_HPP
