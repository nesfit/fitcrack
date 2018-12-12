/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "Logging.hpp"

namespace Logging {

    unsigned char debug_detail = Detail::CustomOutput; 
    bool position_in_code_enabled = false;

}

std::string Logging::parseClassName(const std::string& pretty_function) {
    size_t colons = pretty_function.find("::");
    if (colons == std::string::npos)
	return "::";
    size_t begin = pretty_function.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;

    return pretty_function.substr(begin,end);
}

void Logging::log(const int debug_level, const std::string& class_name, const std::string message) {
    std::string class_name_buffer;
    if (debug_detail >= debug_level) {
	/* Allow class name to be specified or to be passed as __PRETTY_FUNCTION__ */
	if ((class_name_buffer = parseClassName(class_name)) == "::")
	    class_name_buffer = "";

	std::clog << "[" << Logging::Detail::tag[debug_level] << "]: " << class_name_buffer << ": " << message << std::endl;
    }
}

void Logging::positionInCode(const std::string file, const int line) {
    if (position_in_code_enabled) {
	std::clog << "[" << Logging::Detail::tag[Detail::DevelDebug] << "]: " + file << " : line " << line << std::endl;
    }
}

