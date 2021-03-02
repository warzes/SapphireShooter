#pragma once

#if SE_ENABLE_THREADING
#	include "CoreLogThread.h"
#else
#	include "LogVerbosity.h"
#endif
#include "Singleton.h"

class CoreLogSystem :
#if SE_ENABLE_THREADING
	public CoreLogThread,
#endif
	public Singleton<CoreLogSystem>
{
public:
	static void Print(std::string_view str, const LogVerbosity verbosity = LogVerbosity::Info);
};