#pragma once

#if SE_PLATFORM_WINDOWS
inline void PlatformLogPrint(std::string_view str, const LogVerbosity verbosity) noexcept
{
#if SE_DEBUG
	OutputDebugStringA(str.data());
	OutputDebugStringA("\n");
#endif

	HANDLE handle;
	switch (verbosity)
	{
	case LogVerbosity::Fatal:
	case LogVerbosity::Error:
	case LogVerbosity::Warning:
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	case LogVerbosity::Debug:
	case LogVerbosity::Info:
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	default: return;
	}

	DWORD bytesWritten;
	WriteConsoleA(handle, str.data(), (DWORD)str.size(), &bytesWritten, nullptr);
	WriteConsoleA(handle, "\n", 1, &bytesWritten, nullptr);
}
#endif