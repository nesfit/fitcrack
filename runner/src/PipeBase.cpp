/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "PipeBase.hpp"

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
    char c;
    int read_chars = -1;

    while (c != '\n' && canRead()) {
	if ((read_chars = readChar(c)) > 0) {
            line += c; 
	} else if (read_chars == -1) {
	    sleep(HashcatConstant::ProgressPeriod);
	}
    }

    //Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe read line : " + line);
    return line;
}
