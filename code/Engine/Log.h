#pragma once

#include "CoreLogSystem.h"

#define SE_LOG(str)         {CoreLogSystem::Print(str, LogVerbosity::Info);}
#define SE_LOG_ERROR(str)   {CoreLogSystem::Print(str, LogVerbosity::Error);}
#define SE_LOG_WARNING(str) {CoreLogSystem::Print(str, LogVerbosity::Warning);}
#define SE_LOG_INFO(str)    {CoreLogSystem::Print(str, LogVerbosity::Info);}