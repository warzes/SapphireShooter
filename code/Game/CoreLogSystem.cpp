#include "stdafx.h"
#include "CoreLogSystem.h"
#include "PlatformLog.h"
//-----------------------------------------------------------------------------
void CoreLogSystem::Print(std::string_view str, const LogVerbosity verbosity)
{
#if !SE_ENABLE_THREADING
	PlatformLogPrint(str, verbosity);
#else
	CoreLogSystem::Get().print(str.data(), verbosity);
#endif
}
//-----------------------------------------------------------------------------