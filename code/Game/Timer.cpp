#include "stdafx.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
class TimerInitializer
{
public:
	TimerInitializer()
	{
		HiresTimer::Initialize();
#if SE_PLATFORM_WINDOWS
		timeBeginPeriod(1);
#endif
	}

	~TimerInitializer()
	{
#if SE_PLATFORM_WINDOWS
		timeEndPeriod(1);
#endif
	}
};
//-----------------------------------------------------------------------------
static TimerInitializer initializer;
//-----------------------------------------------------------------------------
bool HiresTimer::m_supported = false;
long long HiresTimer::m_frequency = 1000;
//-----------------------------------------------------------------------------
Timer::Timer()
{
	Reset();
}
//-----------------------------------------------------------------------------
unsigned Timer::ElapsedMSec()
{
#if SE_PLATFORM_WINDOWS
	unsigned currentTime = timeGetTime();
#else
	struct timeval time;
	gettimeofday(&time, 0);
	unsigned currentTime = time.tv_sec * 1000 + time.tv_usec / 1000;
#endif
	return currentTime - m_startTime;
}
//-----------------------------------------------------------------------------
void Timer::Reset()
{
#if SE_PLATFORM_WINDOWS
	m_startTime = timeGetTime();
#else
	struct timeval time;
	gettimeofday(&time, 0);
	m_startTime = time.tv_sec * 1000 + time.tv_usec / 1000;
#endif
}
//-----------------------------------------------------------------------------
HiresTimer::HiresTimer()
{
	Reset();
}
//-----------------------------------------------------------------------------
long long HiresTimer::ElapsedUSec()
{
	long long currentTime;

#if SE_PLATFORM_WINDOWS
	if (m_supported)
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		currentTime = counter.QuadPart;
	}
	else
		currentTime = timeGetTime();
#else
	struct timeval time;
	gettimeofday(&time, 0);
	currentTime = time.tv_sec * 1000000LL + time.tv_usec;
#endif

	long long elapsedTime = currentTime - m_startTime;

	// Correct for possible weirdness with changing internal frequency
	if (elapsedTime < 0)
		elapsedTime = 0;

	return (elapsedTime * 1000000LL) / m_frequency;
}
//-----------------------------------------------------------------------------
void HiresTimer::Reset()
{
#if SE_PLATFORM_WINDOWS
	if (m_supported)
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		m_startTime = counter.QuadPart;
	}
	else
		m_startTime = timeGetTime();
#else
	struct timeval time;
	gettimeofday(&time, 0);
	m_startTime = time.tv_sec * 1000000LL + time.tv_usec;
#endif
}
//-----------------------------------------------------------------------------
void HiresTimer::Initialize()
{
#if SE_PLATFORM_WINDOWS
	LARGE_INTEGER frequency;
	if (QueryPerformanceFrequency(&frequency))
	{
		m_frequency = frequency.QuadPart;
		m_supported = true;
	}
#else
	m_frequency = 1000000;
	m_supported = true;
#endif
}
//-----------------------------------------------------------------------------
std::string TimeStamp()
{
	time_t sysTime;
	time(&sysTime);
	const char* dateTime = ctime(&sysTime);
	std::string sTD = std::string(dateTime);
	return sTD.replace(sTD.begin(), sTD.end(), "\n", "");
}
//-----------------------------------------------------------------------------
unsigned CurrentTime()
{
	return (unsigned)time(nullptr);
}
//-----------------------------------------------------------------------------