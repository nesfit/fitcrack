/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "Constants.hpp"

namespace BoincConstants {
    std::string ProjectName;
    std::string TrickleDeamonName;
};

void BoincConstants::setProjectName(const std::string& workunit_name) {
    if (workunit_name == "LocalWorkunit") {
	ProjectName = "standalone";
    } else {
	size_t found_at, last = 0;
	found_at = workunit_name.find("_", last);
	if (found_at == std::string::npos) {
	    ProjectName = "unknown";
	} else {
	    ProjectName = workunit_name.substr(last, found_at);
	}
    }
    TrickleDeamonName = ProjectName;
}
