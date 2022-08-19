/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#if defined(__linux__) || defined(__APPLE__)

#ifndef NAMEDPIPELINUX_HPP
#define NAMEDPIPELINUX_HPP

#include "PipeBase.hpp"
#include "RunnerUtils.hpp"

#include <fcntl.h>
#include <unistd.h>

/** Class representing PipeBase on Linux system */
class PipeLinux : public PipeBase {
    private:

        /**
         * @brief   Check if file descriptor (end of pipe) is open
         * @param   file_descriptor [in] File descriptor as in C language
         * @return  True when file descriptor is open, False otherwise
         */
        bool isEndOpen(int file_descriptor) const;

      	/**
      	 * @brief   Closes end of the pipe specified by argument
      	 * @param   pipe_end [in] Name of the pipe to close
      	 * @return  Error code  // TODO: remove and add exception
      	 */
      	int closeEnd(int pipe_end);

    protected:

        int read_;        /**< File descriptor of reading end of pipe */
        int write_;       /**< File descriptor of writing end of pipe */

        /**
         * @brief   Creates the pipe
         */
        void createPipe(bool is_NONBLOCK_);

        /**
         * @brief   Reads content of the pipe
         * @param   c [out] Character read from the pipe
         * @return  Number of read characters
         */
        int readChar(char& c);

        /**
         * @brief reads a char from the pipe. Blocks if it is not available
         * 
         * @return char The read character or EOF if the pipe has been closed
         * @throws std::runtime_error on other errors
         */
        int readChar();

        /**
         * @brief   Redirects end of the pipe to the target
         * @param   pipe_end [in] File descriptor of the end of pipe (write_,
         * read_)
         * @param   target [in] New end of pipe (ie. stdout, stderr etc.)
         */
        void redirectPipeEnd(int pipe_end, int target);

        /**
         * @brief   Constructor which creates the unblocking Linux pipe
         */
        PipeLinux(bool is_NONBLOCK_);

    public:

        /**
         * @brief   Destructor which calls close on both ends of pipe
         */
        ~PipeLinux();

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
         * @brief   Check if file descriptor (end of pipe) is open
         * @return  True when file descriptor is open, False otherwise
         */
        bool isReadOpen() const;

        /**
         * @brief   Check if file descriptor (end of pipe) is open
         * @return  True when file descriptor is open, False otherwise
         */
        bool isWriteOpen() const;

        virtual void waitForAvailableOutput() const;

        /**
         * @brief   Redirects read end of the pipe to another file descriptor
         * @param   target [in] New read end of the pipe (stdout, stderr etc.)
         */
        void redirectRead(int target);

        /**
         * @brief   Redirects write end of the pipe to another file descriptor
         * @param   target [in] New read end of the pipe (stdout, stderr etc.)
         */
        void redirectWrite(int target);

        /**
         * @brief   Writes message to the pipe
         * @param   message [in] String to be written to pipe
         * @return  Number of written characters
         */
        int writeMessage(std::string& message);
        /**
         * @brief   Creates blocking pipe
         * @return  Blocking pipe
         */
        static PipeBase *createBlockingPipe();
        /**
         * @brief   Creates non-blocking pipe
         * @return  Non-blocking pipe
         */
        static PipeBase *createNonBlockingPipe();
};
#endif // NAMEDPIPELINUX_HPP

#endif // __linux__
