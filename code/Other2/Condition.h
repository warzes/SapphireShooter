#pragma once

#if SE_ENABLE_THREADING

// Condition on which a thread can wait.
class Condition
{
public:
	// Construct.
	Condition();

	// Destruct.
	~Condition();

	// Set the condition. Will be automatically reset once a waiting thread wakes up.
	void Set();

	// Wait on the condition.
	void Wait();

private:
#ifndef WIN32
	// Mutex for the event, necessary for pthreads-based implementation.
	void* mutex;
#endif
	// Operating system specific event.
	void* event;
};

#endif