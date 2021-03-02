#pragma once

//=============================================================================
// Pre Engine header
//=============================================================================
#include "Config.h"
#include "Macros.h"
#include "InlineFunc.h"

//=============================================================================
// Standart header
//=============================================================================
SE_PRAGMA_WARNING_DISABLE_MSVC(4514)
SE_PRAGMA_WARNING_DISABLE_MSVC(4820)
SE_PRAGMA_WARNING_DISABLE_MSVC(5045)

SE_PRAGMA_WARNING_LEVEL(3)
SE_PRAGMA_WARNING_DISABLE_MSVC(4619)
SE_PRAGMA_WARNING_DISABLE_MSVC(5219)


#define _STL_WARNING_LEVEL 3

// TEMP
#define _CRT_SECURE_NO_WARNINGS 

// https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/math-constants?view=msvc-160
#define _USE_MATH_DEFINES   

#include <cstdint>
#include <cassert>
#include <cmath>

#include <sstream>
#include <fstream>
#include <iostream>

#include <exception>
#include <algorithm>
#include <functional>
#include <random>

#include <string>
#include <string_view>

#include <list>
#include <queue>
#include <vector>
#include <set>
#include <map>

#include <memory>

#include <thread>
#include <mutex>

#if SE_PLATFORM_WINDOWS
#	define WIN32_LEAN_AND_MEAN      // 
#	define WIN32_EXTRA_LEAN         // 
#	define VC_EXTRALEAN
#	define NOMINMAX
#	define _WIN32_WINNT 0x0600		// Windows Vista
#	include <windows.h>
#	include <MMSystem.h>
#	if SE_DEBUG
#		include <crtdbg.h>
#	endif
#	undef near
#	undef far
#	undef RGB
#	undef TRANSPARENT
#endif

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/noise.hpp>

#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <assimp\include\assimp\types.h>
#include <assimp\include\assimp\Importer.hpp>
#include <assimp\include\assimp\scene.h>
#include <assimp\include\assimp\postprocess.h>

SE_PRAGMA_WARNING_POP

//=============================================================================
// Post Engine header
//=============================================================================

// TODO
constexpr unsigned shadowSize = 2048;
constexpr unsigned samples = 1;
constexpr float cameraFar = 1000;
constexpr float cameraNear = 0.05f;
constexpr float shadowFar = 500;
constexpr float anisotropy = 0;
constexpr glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF