#pragma once

// Platform OS
#define SE_PLATFORM_ANDROID        0
#define SE_PLATFORM_EMSCRIPTEN     0
#define SE_PLATFORM_LINUX          0
#define SE_PLATFORM_WINDOWS        0
#if defined(__ANDROID__) || defined(ANDROID) // Android compiler defines __linux__
#	undef  SE_PLATFORM_ANDROID
#	define SE_PLATFORM_ANDROID __ANDROID_API__
#elif defined(__EMSCRIPTEN__)
#	undef  SE_PLATFORM_EMSCRIPTEN
#	define SE_PLATFORM_EMSCRIPTEN 1
#elif  defined(__linux__)
#	undef  SE_PLATFORM_LINUX
#	define SE_PLATFORM_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#	undef  SE_PLATFORM_WINDOWS
#	define SE_PLATFORM_WINDOWS 1
#else
#	error Unknown platform.
#endif // SE_PLATFORM_*

// Debug
#define SE_DEBUG 0
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(NDEBUG)
#	undef  SE_DEBUG
#	define SE_DEBUG 1
#endif // SE_DEBUG

// Compiler
#define SE_COMPILER_MSVC 0
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#	undef  SE_COMPILER_MSVC
#	define SE_COMPILER_MSVC _MSC_VER
#else
#	error "Unknown compiler."
#endif // PE_COMPILER_*

#if SE_COMPILER_MSVC
#	define SE_PRAGMA_WARNING_PUSH             __pragma(warning(push))
#	define SE_PRAGMA_WARNING_LEVEL(level)     __pragma(warning(push, level))
#	define SE_PRAGMA_WARNING_POP              __pragma(warning(pop))
#	define SE_PRAGMA_WARNING_DISABLE_MSVC(id) __pragma(warning(disable: id))
#else
#	define SE_PRAGMA_WARNING_PUSH
#	define SE_PRAGMA_WARNING_LEVEL(level)
#	define SE_PRAGMA_WARNING_POP
#	define SE_PRAGMA_WARNING_DISABLE_MSVC(id)
#endif // SE_COMPILER_*

#if SE_ENABLE_THREADING && SE_PLATFORM_WINDOWS && SE_COMPILER_MSVC
#	define SE_THREADTYPE __declspec(thread)
#else
#	define SE_THREADTYPE
#endif