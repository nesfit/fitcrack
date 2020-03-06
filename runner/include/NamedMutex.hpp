/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef FC_RUNNER_NAMED_MUTEX_HPP
#define FC_RUNNER_NAMED_MUTEX_HPP

#include <string>

class NamedMutexHandle;

/**
 * @brief A system-wide named mutex
 * 
 */
class NamedMutex
{
public:
	/**
	 * @brief Construct a new Named Mutex object
	 * 
	 * @param name The name of the mutex
	 */
	explicit NamedMutex(const std::string &name);
	~NamedMutex();

	/**
	 * @brief Locks the mutex
	 * 
	 */
	void lock();

	/**
	 * @brief Unlocks the mutex
	 * 
	 */
	void unlock();
private:
	//! The mutex handle
	NamedMutexHandle *m_handle;
	//! Is mutext locked?
	bool m_locked;
	/**
	 * @brief No copy
	 * 
	 */
	NamedMutex(const NamedMutex&);
	/**
	 * @brief No assignment
	 * 
	 * @return NamedMutex& 
	 */
	NamedMutex &operator=(NamedMutex);
};

#endif //FC_RUNNER_NAMED_MUTEX_HPP