#pragma once

#if SE_ENABLE_THREADING
#if !SE_PLATFORM_WINDOWS
typedef unsigned ThreadID;
#else
typedef pthread_t ThreadID;
#endif
#endif

/// Operating system thread.
class Thread
{
public:
#if SE_ENABLE_THREADING
	/// Construct. Does not start the thread yet.
	Thread();
	/// Destruct. If running, stop and wait for thread to finish.
	virtual ~Thread();

	/// The function to run in the thread.
	// интересная идея на наследовании треда вместо колбэка
	virtual void ThreadFunction() = 0;

	/// Start running the thread. Return true on success, or false if already running or if can not create the thread.
	bool Run();
	/// Set the running flag to false and wait for the thread to finish.
	void Stop();
	/// Set thread priority. The thread must have been started first.
	void SetPriority(int priority);

	/// Return whether thread exists.
	bool IsStarted() const { return handle != nullptr; }

	/// Sleep the current thread for the specified amount of milliseconds. 0 to just yield the timeslice.
	static void Sleep(unsigned mSec);
	/// Set the current thread as the main thread.
	static void SetMainThread();
	/// Return the current thread's ID.
	static ThreadID CurrentThreadID();
	/// Return whether is executing in the main thread.
#endif
	static bool IsMainThread();

protected:
#if SE_ENABLE_THREADING
	/// Thread handle.
	void* handle;
	/// Running flag.
	volatile bool shouldRun;

	/// Main thread's thread ID.
	static ThreadID mainThreadID;
#endif
};