#pragma once

#if SE_COMPILER_MSVC && SE_DEBUG
#	define _DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	define new _DEBUG_NEW
#endif