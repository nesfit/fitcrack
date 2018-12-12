/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifdef __linux__

#include "PipeLinux.hpp"

/* Private */

bool PipeLinux::isEndOpen(int file_descriptor) const {
    /** Checks if file descriptor is open and errno isn't containing error */
    return fcntl(file_descriptor, F_GETFD) != -1 || errno != EBADF;
}

void PipeLinux::redirectPipeEnd(int pipe_end, int target) {
    if (dup2(pipe_end, target) < 0) {
        RunnerUtils::runtimeException("dup2 failed with target", errno);
    }
}

int PipeLinux::closeEnd(int pipe_end) {
    /** Check if pipe end is still open before closing it */
    if (isEndOpen(pipe_end))
        return close(pipe_end);
    else
        return 0;
}
/* Protected */

void PipeLinux::createPipe() {
    /** Prevent replacement of active pipe byt new one */
    if (read_ == 0 && write_ == 0) {
        std::vector<int> file_descriptor(2); 

        /** Create new pipe */
        if (pipe(file_descriptor.data()) < 0) {
            RunnerUtils::runtimeException("pipe failed", errno);
        }

        /** Set pipe to non-blocking state */
        if (fcntl(file_descriptor[0], F_SETFL, fcntl(file_descriptor[0], F_GETFL) | O_NONBLOCK) < 0) {
            RunnerUtils::runtimeException("fcntl failed", errno);
        } 

        /** Save values into object member properties */
        read_ = file_descriptor[0];
        write_ = file_descriptor[1];
    }
}

bool PipeLinux::isReadOpen() const {
    return isEndOpen(read_);
}

bool PipeLinux::isWriteOpen() const {
    return isEndOpen(write_);
}

int PipeLinux::readChar(char& c) {
    // Based on article
    // https://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/

    int read_size = READ_SIZE;
    int n_read_chars = 0;
    std::vector<char> ch(read_size);

    n_read_chars = read(read_, ch.data(), read_size);
    
    switch (n_read_chars) {
        case -1:
	    /* Nothing new to read from pipe */
            if (errno == EAGAIN) {
                // sleep(1);
                break;
            } else {
                RunnerUtils::runtimeException("read() failed", errno);
            }
        case 0:
            closeRead();
            break;
        default:
            c = ch[0];
    }

    return n_read_chars;
}

/* Public */

PipeLinux::PipeLinux() : read_(0), write_(0) {
    createPipe();
}

PipeLinux::~PipeLinux() {
    closeAll();
}

bool PipeLinux::canRead() const {
    return isReadOpen();
}

int PipeLinux::closeRead() {
    int err = closeEnd(read_);
    if (err)
	read_ = 0;
    return err;
}

int PipeLinux::closeWrite() {
    int err = closeEnd(write_);
    if (err)
	write_ = 0;
    return err;
}

void PipeLinux::redirectRead(int target) {
    redirectPipeEnd(read_, target);
}

void PipeLinux::redirectWrite(int target) {
    redirectPipeEnd(write_, target);
}

int PipeLinux::writeMessage(std::string& message) {
    Logging::debugPrint(Logging::Detail::CustomOutput, POSITION_IN_CODE + "Pipe writing message : " + message);
    return write(write_, message.data(), message.length());
}

#endif // __linux__
