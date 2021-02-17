#include "stdafx.h"
#include "CoreLogThread.h"
#if SE_ENABLE_THREADING
#include "CoreLogThread.h"
#include "PlatformLog.h"
//-----------------------------------------------------------------------------
CoreLogThread::CoreLogThread()
{
#if !SE_PLATFORM_EMSCRIPTEN
	m_thread = std::thread(&CoreLogThread::LogLoop, this);
#endif
}
//-----------------------------------------------------------------------------
CoreLogThread::~CoreLogThread()
{
	close();
}
//-----------------------------------------------------------------------------
void CoreLogThread::print(const std::string& str, const LogVerbosity verbosity) const
{
	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_logQueue.push(commandLog(verbosity, str));
	lock.unlock();
	m_queueCondition.notify_all();
}
//-----------------------------------------------------------------------------
void CoreLogThread::LogLoop()
{
	for (;; )
	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
		while (m_logQueue.empty() && !m_quitCommand)
			m_queueCondition.wait(lock);

		if (m_quitCommand)
		{
			lock.unlock();
			break;
		}

		const auto command = std::move(m_logQueue.front());
		m_logQueue.pop();
		lock.unlock();

		PlatformLogPrint(command.str, command.verbosity);
	}
}
//-----------------------------------------------------------------------------
void CoreLogThread::close()
{
	if (m_isClose) return;

	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_quitCommand = true;
	lock.unlock();
	m_queueCondition.notify_all();

	if (m_thread.joinable()) m_thread.join();
	m_isClose = true;
}
//-----------------------------------------------------------------------------
#endif