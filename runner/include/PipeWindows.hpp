/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifdef _WIN32

#ifndef NAMEDPIPEWINDOWS_HPP 
#define NAMEDPIPEWINDOWS_HPP

#include "PipeBase.hpp"

#include <windows.h>

/** Class representing PipeBase on Linux system */
class PipeWindows : public PipeBase {
    private:


        /**
         * @brief   Check if file descriptor (end of pipe) is open
         * @param   file_descriptor [in] File descriptor as in C language
         * @return  True when file descriptor is open, False otherwise
         */
        bool isEndOpen(HANDLE file_descriptor) const;

	/**
	 * @brief   Closes end of the pipe specified by argument
	 * @param   pipe_end [in] Name of the pipe to close
	 * @return  Error code  // TODO: remove and add exception
	 */
	int closeEnd(HANDLE pipe_end);

    protected:

	DWORD error_;	    /**< Contains save GetLastError() from reads, ERROR_SUCESS, ERROR_BROKEN_PIPE allowed */

	HANDLE read_;       /**< File descriptor of reading end of pipe */
	HANDLE write_;      /**< File descriptor of writing end of pipe */

        /**
         * @brief   Creates the pipe
         */
        void createPipe();

        /**
         * @brief   Reads content of the pipe
         * @param   c [out] Character read from the pipe
         * @return  Number of read characters
         */
        int readChar(char& c);

    public:

        /**
         * @brief   Constructor which creates the unblocking Linux pipe
         */
        PipeWindows();

        /**
         * @brief   Destructor which calls close on both ends of pipe
         */
        ~PipeWindows();

	/**
	 * @brief   States whether we can read from the pipe
	 * @return  True when we can, False otherwise 
	 */
	bool canRead() const;

        /**
         * @brief   Closes read-end of pipe
         * @return  0 or error number of system close operation 
         */
        int closeRead();

        /**
         * @brief   Closes write-end of pipe
         * @return  0 or error number of system close operation 
         */
        int closeWrite();

	/**
	 * @brief   Getter of read_ HANDLE
	 */
	HANDLE getReadHandle() const;

	/**
	 * @brief   Getter of write_ HANDLE
	 */
	HANDLE getWriteHandle() const;

        /**
         * @brief   Check if file descriptor (end of pipe) is open
         * @return  True when file descriptor is open, False otherwise
         */
        bool isReadOpen() const;

        /**
         * @brief   Check if file descriptor (end of pipe) is open
         * @return  True when file descriptor is open, False otherwise
         */
        bool isWriteOpen() const;

        /**
         * @brief   Writes message to the pipe
         * @param   message [in] String to be written to pipe
         * @return  Number of written characters
         */
        int writeMessage(std::string& message);
};
#endif // NAMEDPIPEWINDOWS_HPP

#endif // __WIN32
