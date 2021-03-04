#include "stdafx.h"
#include "RenderSystem.h"
#include "Window.h"
#include "WGLFunc.h"
#include "Log.h"
//-----------------------------------------------------------------------------
#ifdef WIN32
// Prefer the high-performance GPU on switchable GPU systems
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
//-----------------------------------------------------------------------------
// TODO:
inline GLenum glCheckError()
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		SE_LOG_ERROR(error);
	}
	return errorCode;
}
//-----------------------------------------------------------------------------
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	//	(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
	//	type, severity, message);

	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
//-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
	if (m_isInit)
		Close();
}
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
bool RenderSystem::Init(HWND hwnd, const RenderDescription& config)
#endif
{
#if SE_PLATFORM_WINDOWS
	m_hwnd = hwnd;
	m_openGLMajorVersion = config.OpenGLMajorVersion;
	m_openGLMinorVersion = config.OpenGLMinorVersion;

	if (!(m_deviceContext = GetDC(hwnd)))
		throw std::runtime_error("GetDC() failed: Can not create context.");

	// Create and set a pixel format for the window.

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize        = sizeof(pfd);
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType   = PFD_TYPE_RGBA;
	pfd.cColorBits   = 32;
	pfd.cAlphaBits   = 0;
	pfd.cDepthBits   = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType   = PFD_MAIN_PLANE;

	int pixelFormat = 0;
	if (m_antiAliasingSamples == 0)
		pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
	else if (m_antiAliasingSamples == BEST_ANTI_ALIASING_SAMPLES)
		gl::ChooseBestAntiAliasingPixelFormat(pixelFormat);
	else
		gl::ChooseAntiAliasingPixelFormat(pixelFormat, m_antiAliasingSamples);
	if (!pixelFormat)
		pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
	if (!pixelFormat)
		throw std::runtime_error("Failed to choose pixel format for dummy window");

	if (!SetPixelFormat(m_deviceContext, pixelFormat, &pfd))
		throw std::runtime_error("SetPixelFormat() failed.");

	pfd.dwFlags |= PFD_SUPPORT_COMPOSITION;

	// Verify that this OpenGL implementation supports the required extensions.
	if (!gl::ExtensionSupported("WGL_ARB_create_context"))
		throw std::runtime_error("Required extension WGL_ARB_create_context is not supported.");

	while (1)
	{
		const int AttribList[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, m_openGLMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, m_openGLMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_renderContext = wglCreateContextAttribsARB(m_deviceContext, 0, AttribList);
		if (m_renderContext)
			break;
		else
		{
			m_openGLMinorVersion--;
			if (m_openGLMinorVersion < 0 && m_openGLMajorVersion == 4)
			{
				m_openGLMajorVersion = 3;
				m_openGLMinorVersion = 3;
			}
			else if (m_openGLMinorVersion < 3 && m_openGLMajorVersion == 3)
				throw std::runtime_error("OpenGL not support");	
		}
	}

	if (!wglMakeCurrent(m_deviceContext, m_renderContext))
		throw std::runtime_error("wglMakeCurrent() failed.");

#endif

	int major = 0;
	int minor = 0;
	gl::GetGLVersion(major, minor);
	const std::string strOGLVersion = "OpenGL version: " + std::to_string(major) + "." + std::to_string(minor);
	SE_LOG(strOGLVersion.c_str());
	const std::string strVendorVersion = "Vendor version: " + std::string((char*)glGetString(GL_RENDERER));
	SE_LOG(strVendorVersion.c_str());
	const std::string strGLSLVersion = "GLSL version: " + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	SE_LOG(strGLSLVersion.c_str());


	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glEnable(GL_BLEND);

#if SE_DEBUG
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

	m_isInit = true;
	return true;
}
//-----------------------------------------------------------------------------
void RenderSystem::Close()
{
	if (m_deviceContext)
	{
		if (m_renderContext)
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(m_renderContext);
			m_renderContext = 0;
		}

		ReleaseDC(m_hwnd, m_deviceContext);
		m_deviceContext = 0;
	}

	m_isInit = false;
}
//-----------------------------------------------------------------------------
void RenderSystem::BeginFrame(int width, int height)
{
	glViewport(0, 0, width, height);
	glClearColor(0.129f, 0.586f, 0.949f, 1.0f);
	//glClearDepth(1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void RenderSystem::EndFrame()
{
#if SE_PLATFORM_WINDOWS
	SwapBuffers(m_deviceContext);
#endif
	glCheckError();
	//GLenum err;
	//while ((err = glGetError()) != GL_NO_ERROR)
	//	SE_LOG("GL error code: " + std::to_string(err));	
}
//-----------------------------------------------------------------------------
void RenderSystem::SetVsync(bool enable)
{
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable ? 1 : 0);
}
//-----------------------------------------------------------------------------