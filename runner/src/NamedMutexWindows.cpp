#ifdef _WIN32

#include "NamedMutex.hpp"

#include "RunnerUtils.hpp"
#include "Logging.hpp"

#include <windows.h>

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
	NamedMutexHandle(const std::string &name):
		handle(CreateMutexA(nullptr, false, name.c_str()))
	{
		if(!handle)
		{
			RunnerUtils::runtimeException("Failed to create a named mutex", GetLastError());
		}
	}
	~NamedMutexHandle()
	{
		CloseHandle(handle);
	}
	//!The handle of the named mutex
	HANDLE handle;
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
	DWORD result = WaitForSingleObject(m_handle->handle, INFINITE);
	if(result == WAIT_FAILED)
	{
		RunnerUtils::runtimeException("Failed to wait for named mutex", GetLastError());
	}
	m_locked = true;
	//other errors are OK, seeing as TIMEOUT can't happen and ABANDONED is OK, since the process is dead
	Logging::debugPrint(Logging::Detail::CustomOutput, "Locked named mutex");
}

/**
	* @brief Unlocks the mutex
	* 
	*/
void NamedMutex::unlock()
{
	if(!ReleaseMutex(m_handle->handle))
	{
		RunnerUtils::runtimeException("Failed to release named mutex", GetLastError());
	}
	m_locked = false;
	Logging::debugPrint(Logging::Detail::CustomOutput, "Unlocked named mutex");
}

#endif