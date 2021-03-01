#include "stdafx.h"
#include "RenderSystem.h"
#include "Window.h"
#include "WGLFunc.h"
#include "Log.h"
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
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = 0;
	if (m_antiAliasingSamples == 0)
		pf = ChoosePixelFormat(m_deviceContext, &pfd);
	else if (m_antiAliasingSamples == BEST_ANTI_ALIASING_SAMPLES)
		gl::ChooseBestAntiAliasingPixelFormat(pf);
	else
		gl::ChooseAntiAliasingPixelFormat(pf, m_antiAliasingSamples);
	if (!pf)
		pf = ChoosePixelFormat(m_deviceContext, &pfd);

	if (!SetPixelFormat(m_deviceContext, pf, &pfd))
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


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_BLEND);

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
			wglMakeCurrent(m_deviceContext, 0);
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
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		SE_LOG("GL error code: " + std::to_string(err));	
}
//-----------------------------------------------------------------------------
void RenderSystem::SetVsync(bool enable)
{
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable ? 1 : 0);
}
//-----------------------------------------------------------------------------