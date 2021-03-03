#include "stdafx.h"
#if SE_OPENGL
#include "WGLFunc.h"

#if SE_PLATFORM_WINDOWS
//-----------------------------------------------------------------------------
#define LOAD_ENTRYPOINT(name, var, type)                    \
	if (!var)                                               \
	{                                                       \
		var = function_cast<type>(wglGetProcAddress(name)); \
		assert(var != 0);                                   \
	}
//-----------------------------------------------------------------------------
namespace
{
	// TODO: повтор
	const wchar_t* DummyGLWindowClass = L"DummyGLWindowClass";
	HINSTANCE gInstance = nullptr;
	HWND g_hWnd = nullptr;
	HDC g_hDC = nullptr;
	HGLRC g_hRC = nullptr;
	char g_szAAPixelFormat[32];

	LRESULT CALLBACK DummyGLWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
			break;

		case WM_DESTROY:
			if (g_hDC)
			{
				if (g_hRC)
				{
					wglMakeCurrent(g_hDC, 0);
					wglDeleteContext(g_hRC);
					g_hRC = 0;
				}

				ReleaseDC(hWnd, g_hDC);
				g_hDC = 0;
			}

			PostQuitMessage(0);
			return 0;

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	bool CreateDummyGLWindow()
	{
		gInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));

		WNDCLASSEX wcl = { 0 };
		wcl.cbSize = sizeof(wcl);
		wcl.style = CS_OWNDC;
		wcl.lpfnWndProc = DummyGLWndProc;
		wcl.hInstance = gInstance;
		wcl.lpszClassName = DummyGLWindowClass;
		if (!RegisterClassEx(&wcl))
			return false;

		g_hWnd = CreateWindow(DummyGLWindowClass, L"", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, gInstance, 0);
		if (!g_hWnd)
			return false;
		g_hDC = GetDC(g_hWnd);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pf = ChoosePixelFormat(g_hDC, &pfd);

		if (!SetPixelFormat(g_hDC, pf, &pfd))
			return false;

		if (!(g_hRC = wglCreateContext(g_hDC)))
			return false;

		if (!wglMakeCurrent(g_hDC, g_hRC))
			return false;

		return true;
	}

	void ChooseBestCSAAPixelFormat(int& pf)
	{
		struct CSAAPixelFormat
		{
			int numColorSamples;
			int numCoverageSamples;
			const char* pszDescription;
		};

		const CSAAPixelFormat csaaPixelFormats[] =
		{
			{ 4, 8,  "8x CSAA" },
			{ 4, 16, "16x CSAA" },
			{ 8, 8,  "8xQ (Quality) CSAA" },
			{ 8, 16, "16xQ (Quality) CSAA" }
		};

		const int totalCSAAFormats = static_cast<int>(sizeof(csaaPixelFormats) / sizeof(CSAAPixelFormat));

		int attributes[] =
		{
			WGL_SAMPLE_BUFFERS_ARB,  1,
			WGL_COLOR_SAMPLES_NV,    0,
			WGL_COVERAGE_SAMPLES_NV, 0,
			WGL_DOUBLE_BUFFER_ARB,   1,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = FALSE;

		for (int i = totalCSAAFormats - 1; i >= 0; --i)
		{
			attributes[3] = csaaPixelFormats[i].numColorSamples;
			attributes[5] = csaaPixelFormats[i].numCoverageSamples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);

			if (bStatus == TRUE && numFormats)
			{
				pf = returnedPixelFormat;
				strcpy(g_szAAPixelFormat, csaaPixelFormats[i].pszDescription);
				break;
			}
		}

		if (bStatus == FALSE)
			g_szAAPixelFormat[0] = '\0';
	}

	void ChooseBestMSAAPixelFormat(int& pf)
	{
		int attributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,     24,
			WGL_ALPHA_BITS_ARB,     8,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB,        0,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = FALSE;

		for (int samples = 16; samples > 0; samples /= 2)
		{
			attributes[17] = samples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);

			if (bStatus == TRUE && numFormats)
			{
				pf = returnedPixelFormat;
				sprintf(g_szAAPixelFormat, "%dx MSAA", samples);
				break;
			}
		}

		if (bStatus == FALSE)
			g_szAAPixelFormat[0] = '\0';
	}

	void ChooseCSAAPixelFormat(int& pf, int samples)
	{
		struct CSAAPixelFormat
		{
			int numColorSamples;
			int numCoverageSamples;
			const char* pszDescription;
		};

		CSAAPixelFormat csaaPixelFormats[] =
		{
			{ 4, 16, "16x CSAA" },
			{ 4, 8,  "8x CSAA" }
		};

		CSAAPixelFormat csaaQualityPixelFormats[] =
		{
			{ 8, 16, "16xQ (Quality) CSAA" },
			{ 8, 8,  "8xQ (Quality) CSAA" }
		};

		CSAAPixelFormat* pCSAAFormats = 0;

		int attributes[] =
		{
			WGL_SAMPLE_BUFFERS_ARB,  1,
			WGL_COLOR_SAMPLES_NV,    0,
			WGL_COVERAGE_SAMPLES_NV, 0,
			WGL_DOUBLE_BUFFER_ARB,   1,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = FALSE;

		if (samples >= 8)
			pCSAAFormats = csaaQualityPixelFormats;
		else
			pCSAAFormats = csaaPixelFormats;

		for (int i = 0; i < 2; ++i)
		{
			attributes[3] = pCSAAFormats[i].numColorSamples;
			attributes[5] = pCSAAFormats[i].numCoverageSamples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);

			if (bStatus == TRUE && numFormats)
			{
				pf = returnedPixelFormat;
				strcpy(g_szAAPixelFormat, pCSAAFormats[i].pszDescription);
				break;
			}
		}

		if (bStatus == FALSE)
			g_szAAPixelFormat[0] = '\0';
	}

	void ChooseMSAAPixelFormat(int& pf, int samples)
	{
		int attributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,     24,
			WGL_ALPHA_BITS_ARB,     8,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB,        samples,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);

		if (bStatus == TRUE && numFormats)
		{
			pf = returnedPixelFormat;
			sprintf(g_szAAPixelFormat, "%dx MSAA", samples);
		}
		else
		{
			g_szAAPixelFormat[0] = '\0';
		}
	}

	void DestroyDummyGLWindow()
	{
		if (g_hWnd)
		{
			PostMessage(g_hWnd, WM_CLOSE, 0, 0);

			BOOL bRet;
			MSG msg;

			while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		UnregisterClass(DummyGLWindowClass, gInstance);
	}

	bool ExtensionSupported(const char* pszExtensionName)
	{
		static const char* pszGLExtensions = 0;
		static const char* pszWGLExtensions = 0;

		if (!pszGLExtensions)
			pszGLExtensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

		if (!pszWGLExtensions)
		{
			// WGL_ARB_extensions_string.

			typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);
			PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = function_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(wglGetProcAddress("wglGetExtensionsStringARB"));
			if (wglGetExtensionsStringARB)
				pszWGLExtensions = wglGetExtensionsStringARB(wglGetCurrentDC());
		}

		if (!strstr(pszGLExtensions, pszExtensionName))
		{
			if (!strstr(pszWGLExtensions, pszExtensionName))
				return false;
		}

		return true;
	}
}
//-----------------------------------------------------------------------------
namespace gl
{
	void ChooseBestAntiAliasingPixelFormat(int& pf)
	{
		pf = 0;

		if (!CreateDummyGLWindow())
		{
			DestroyDummyGLWindow();
			return;
		}

		if (ExtensionSupported("GL_NV_multisample_coverage") && ExtensionSupported("WGL_NV_multisample_coverage"))
		{
			ChooseBestCSAAPixelFormat(pf);
		}
		else
		{
			ChooseBestMSAAPixelFormat(pf);
		}

		DestroyDummyGLWindow();
	}

	void ChooseAntiAliasingPixelFormat(int& pf, int samples)
	{
		pf = 0;

		if (!CreateDummyGLWindow())
		{
			DestroyDummyGLWindow();
			return;
		}

		if (ExtensionSupported("GL_NV_multisample_coverage") && ExtensionSupported("WGL_NV_multisample_coverage"))
		{
			ChooseCSAAPixelFormat(pf, samples);
		}
		else
		{
			ChooseMSAAPixelFormat(pf, samples);
		}

		DestroyDummyGLWindow();
	}

	const char* GetAntiAliasingPixelFormatString()
	{
		return g_szAAPixelFormat;
	}
}
//-----------------------------------------------------------------------------
void glSampleCoverageARB(GLclampf value, GLboolean invert)
{
	typedef void (APIENTRY* PFNGLSAMPLECOVERAGEARBPROC)(GLclampf value, GLboolean invert);
	static PFNGLSAMPLECOVERAGEARBPROC pfnSampleCoverageARB = 0;
	LOAD_ENTRYPOINT("glSampleCoverageARB", pfnSampleCoverageARB, PFNGLSAMPLECOVERAGEARBPROC);
	pfnSampleCoverageARB(value, invert);
}
//-----------------------------------------------------------------------------
BOOL wglGetPixelFormatAttribivARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int* piValues)
{
	typedef BOOL(WINAPI* PFNWGLGETPIXELFORMATATTRIBIVARBPROC)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int* piValues);
	static PFNWGLGETPIXELFORMATATTRIBIVARBPROC pfnGetPixelFormatAttribivARB = 0;
	LOAD_ENTRYPOINT("wglGetPixelFormatAttribivARB", pfnGetPixelFormatAttribivARB, PFNWGLGETPIXELFORMATATTRIBIVARBPROC);
	return pfnGetPixelFormatAttribivARB(hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
}
//-----------------------------------------------------------------------------
BOOL wglGetPixelFormatAttribfvARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT* pfValues)
{
	typedef BOOL(WINAPI* PFNWGLGETPIXELFORMATATTRIBFVARBPROC)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT* pfValues);
	static PFNWGLGETPIXELFORMATATTRIBFVARBPROC pfnGetPixelFormatAttribfvARB = 0;
	LOAD_ENTRYPOINT("wglGetPixelFormatAttribfvARB", pfnGetPixelFormatAttribfvARB, PFNWGLGETPIXELFORMATATTRIBFVARBPROC);
	return pfnGetPixelFormatAttribfvARB(hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
}
//-----------------------------------------------------------------------------
BOOL wglChoosePixelFormatARB(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats)
{
	typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
	static PFNWGLCHOOSEPIXELFORMATARBPROC pfnChoosePixelFormatARB = 0;
	LOAD_ENTRYPOINT("wglChoosePixelFormatARB", pfnChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC);
	return pfnChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
}
//-----------------------------------------------------------------------------
HGLRC wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int* attribList)
{
	// An OpenGL 3.1 rendering context is created using the new
	// wglCreateContextAttribsARB() function. This new function was introduced
	// in OpenGL 3.0 to maintain backwards compatibility with existing OpenGL
	// 2.1 and older applications. To create an OpenGL 3.1 rendering context
	// first create an OpenGL 2.1 or older rendering context using the
	// wglCreateContext() function. Activate the context and then call the new
	// wglCreateContextAttribsARB() function to create an OpenGL 3.1 rendering
	// context. Once the context is created activate it to enable OpenGL 3.1
	// functionality.
	//
	// For further details see:
	// http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt

	typedef HGLRC(APIENTRY* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);
	static PFNWGLCREATECONTEXTATTRIBSARBPROC pfnCreateContextAttribsARB = 0;

	HGLRC hContext = 0;
	HGLRC hCurrentContext = wglGetCurrentContext();

	if (!hCurrentContext)
	{
		if (!(hCurrentContext = wglCreateContext(hDC)))
			return 0;

		if (!wglMakeCurrent(hDC, hCurrentContext))
		{
			wglDeleteContext(hCurrentContext);
			return 0;
		}

		LOAD_ENTRYPOINT("wglCreateContextAttribsARB", pfnCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC);

		if (pfnCreateContextAttribsARB)
			hContext = pfnCreateContextAttribsARB(hDC, hShareContext, attribList);

		wglMakeCurrent(hDC, 0);
		wglDeleteContext(hCurrentContext);
	}
	else
	{
		if (!wglMakeCurrent(hDC, hCurrentContext))
			return 0;

		LOAD_ENTRYPOINT("wglCreateContextAttribsARB", pfnCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC);

		if (pfnCreateContextAttribsARB)
			hContext = pfnCreateContextAttribsARB(hDC, hShareContext, attribList);
	}

	return hContext;
}
//-----------------------------------------------------------------------------
BOOL wglSwapIntervalEXT(int interval)
{
	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
	static PFNWGLSWAPINTERVALEXTPROC pfnSwapIntervalEXT = 0;
	LOAD_ENTRYPOINT("wglSwapIntervalEXT", pfnSwapIntervalEXT, PFNWGLSWAPINTERVALEXTPROC);
	return pfnSwapIntervalEXT(interval);
}
//-----------------------------------------------------------------------------
int wglGetSwapIntervalEXT()
{
	typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC)(void);
	static PFNWGLGETSWAPINTERVALEXTPROC pfnGetSwapIntervalEXT = 0;
	LOAD_ENTRYPOINT("wglGetSwapIntervalEXT", pfnGetSwapIntervalEXT, PFNWGLGETSWAPINTERVALEXTPROC);
	return pfnGetSwapIntervalEXT();
}
//-----------------------------------------------------------------------------
#endif

#endif // SE_OPENGL