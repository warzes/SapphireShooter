#pragma once

<<<<<<< HEAD
=======
#if SE_ENABLE_THREADING

>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
// Operating system mutual exclusion primitive.
class Mutex
{
public:
	// Construct.
	Mutex();
	// Destruct.
	~Mutex();

	// Acquire the mutex. Block if already acquired.
	void Acquire();
	// Release the mutex.
	void Release();

private:
	// Mutex handle.
	void* handle;
};

// Lock that automatically acquires and releases a mutex.
class MutexLock
{
public:
	// Construct and acquire the mutex.
	MutexLock(Mutex& mutex);
	// Destruct. Release the mutex.
	~MutexLock();

private:
	// Prevent copy construction.
	MutexLock(const MutexLock& rhs);
	// Prevent assignment.
	MutexLock& operator=(const MutexLock& rhs);

	// Mutex reference.
	Mutex& mutex;
};

#endif