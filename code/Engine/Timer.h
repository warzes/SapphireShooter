#pragma once

// Low-resolution operating system timer.
class Timer
{
public:
	Timer();

	// Return elapsed milliseconds.
	unsigned ElapsedMSec();
	// Reset the timer.
	void Reset();

private:
	// Starting clock value in milliseconds.
	unsigned m_startTime;
};

// High-resolution operating system timer used in profiling.
class HiresTimer
{
public:
	HiresTimer();
	// Return elapsed microseconds.
	long long ElapsedUSec();
	// Reset the timer.
	void Reset();

	// Perform one-time initialization to check support and frequency. Is called automatically at program start.
	static void Initialize();
	// Return if high-resolution timer is supported.
	static bool IsSupported() { return m_supported; }
	// Return high-resolution timer frequency if supported.
	static long long Frequency() { return m_frequency; }

private:
	// Starting clock value in CPU ticks.
	long long m_startTime;
	// High-resolution timer support flag.
	static bool m_supported;
	// High-resolution timer frequency.
	static long long m_frequency;
};

// Return a date/time stamp as a string.
std::string TimeStamp();
// Return current time as seconds since epoch.
unsigned CurrentTime();