#if defined(__linux__) || defined(__APPLE__)

#include "NamedMutex.hpp"

#include "RunnerUtils.hpp"
#include "Logging.hpp"

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

//we will be using the flock() approach, since POSIX mutexes stay locked if the process dies unexpectedly

/**
 * @brief A RAII class to manage a handle to a named mutex
 * 
 */
class NamedMutexHandle
{
public:
	/**
	 * @brief Construct a new Named Mutex Handle object
	 * 
	 * @param name The global name of the mutex
	 */
	NamedMutexHandle(const std::string &name) {
          runner_mutex = "/tmp/FitcrackRunnerMutex_" + name;
          handle =
              open(runner_mutex.c_str(), O_CREAT | O_RDONLY | O_CLOEXEC,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

          // /tmp may not work on newer Linux systems (new security feature to protect /tmp)
		  // Try other directories...
          // https://github.com/BOINC/boinc/issues/4125
          if (handle == -1) {
            runner_mutex = "/var/lib/boinc-client/FitcrackRunnerMutex_" +
                           name; // Almost all distros.
            handle =
                open(runner_mutex.c_str(), O_CREAT | O_RDONLY | O_CLOEXEC,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

            if (handle == -1) {
              runner_mutex =
                  "/var/lib/boinc/FitcrackRunnerMutex_" + name; // Debian
              handle = open(
                  runner_mutex.c_str(), O_CREAT | O_RDONLY | O_CLOEXEC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            }
          }

          if (handle == -1) {
            RunnerUtils::runtimeException(
                "Failed to create a file for mutual exclusion purposes", errno);
          }

          Logging::debugPrint(Logging::Detail::CustomOutput,
                              "Named mutex created: " + runner_mutex);
        }
        //!Destructor
	~NamedMutexHandle()
	{
		close(handle);
		unlink(runner_mutex.c_str());
	}
	//!The file descriptor
	int handle;
	//!Path to named mutex
	std::string runner_mutex;
};

NamedMutex::NamedMutex(const std::string &name):
	m_handle(new NamedMutexHandle(name)),
	m_locked(false)
{}

NamedMutex::~NamedMutex()
{
	try
	{
		if(m_locked)
		{
			unlock();
		}
	}
	catch(...)
	{
		Logging::debugPrint(Logging::Detail::Important, "Unlocking the mutex failed");
	}
	delete m_handle;
}

void NamedMutex::lock()
{
	Logging::debugPrint(Logging::Detail::CustomOutput, "Locking named mutex");
	int result = flock(m_handle->handle, LOCK_EX);
	if(result == -1)
	{
		if(errno == EINTR)
		{
			//try again via tail recursion
			Logging::debugPrint(Logging::Detail::CustomOutput, "EINTR, retry");
			return lock();
		}
		RunnerUtils::runtimeException("Failed to wait for named mutex", errno);
	}
	m_locked = true;
	Logging::debugPrint(Logging::Detail::CustomOutput, "Locked named mutex");
}

/**
	* @brief Unlocks the mutex
	* 
	*/
void NamedMutex::unlock()
{
	if(flock(m_handle->handle, LOCK_UN) == -1)
	{
		RunnerUtils::runtimeException("Failed to release named mutex", errno);
	}
	m_locked = false;
	Logging::debugPrint(Logging::Detail::CustomOutput, "Unlocked named mutex");
}

#endif