#pragma once

#if SE_OPENGL

#if SE_PLATFORM_WINDOWS

// GL_ARB_multisample

#define GL_MULTISAMPLE_ARB                 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB    0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB         0x809F
#define GL_SAMPLE_COVERAGE_ARB             0x80A0
#define GL_SAMPLE_BUFFERS_ARB              0x80A8
#define GL_SAMPLES_ARB                     0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB       0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB      0x80AB
#define GL_MULTISAMPLE_BIT_ARB             0x20000000

// GL_NV_multisample_coverage

#define GL_COVERAGE_SAMPLES_NV             0x80A9
#define GL_COLOR_SAMPLES_NV                0x8E20

// WGL_ARB_multisample

#define WGL_SAMPLE_BUFFERS_ARB             0x2041
#define WGL_SAMPLES_ARB                    0x2042

// WGL_ARB_pixel_format

#define WGL_NUMBER_PIXEL_FORMATS_ARB       0x2000
#define WGL_DRAW_TO_WINDOW_ARB             0x2001
#define WGL_DRAW_TO_BITMAP_ARB             0x2002
#define WGL_ACCELERATION_ARB               0x2003
#define WGL_NEED_PALETTE_ARB               0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB        0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB         0x2006
#define WGL_SWAP_METHOD_ARB                0x2007
#define WGL_NUMBER_OVERLAYS_ARB            0x2008
#define WGL_NUMBER_UNDERLAYS_ARB           0x2009
#define WGL_TRANSPARENT_ARB                0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB      0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB    0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB     0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB    0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB    0x203B
#define WGL_SHARE_DEPTH_ARB                0x200C
#define WGL_SHARE_STENCIL_ARB              0x200D
#define WGL_SHARE_ACCUM_ARB                0x200E
#define WGL_SUPPORT_GDI_ARB                0x200F
#define WGL_SUPPORT_OPENGL_ARB             0x2010
#define WGL_DOUBLE_BUFFER_ARB              0x2011
#define WGL_STEREO_ARB                     0x2012
#define WGL_PIXEL_TYPE_ARB                 0x2013
#define WGL_COLOR_BITS_ARB                 0x2014
#define WGL_RED_BITS_ARB                   0x2015
#define WGL_RED_SHIFT_ARB                  0x2016
#define WGL_GREEN_BITS_ARB                 0x2017
#define WGL_GREEN_SHIFT_ARB                0x2018
#define WGL_BLUE_BITS_ARB                  0x2019
#define WGL_BLUE_SHIFT_ARB                 0x201A
#define WGL_ALPHA_BITS_ARB                 0x201B
#define WGL_ALPHA_SHIFT_ARB                0x201C
#define WGL_ACCUM_BITS_ARB                 0x201D
#define WGL_ACCUM_RED_BITS_ARB             0x201E
#define WGL_ACCUM_GREEN_BITS_ARB           0x201F
#define WGL_ACCUM_BLUE_BITS_ARB            0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB           0x2021
#define WGL_DEPTH_BITS_ARB                 0x2022
#define WGL_STENCIL_BITS_ARB               0x2023
#define WGL_AUX_BUFFERS_ARB                0x2024
#define WGL_NO_ACCELERATION_ARB            0x2025
#define WGL_GENERIC_ACCELERATION_ARB       0x2026
#define WGL_FULL_ACCELERATION_ARB          0x2027
#define WGL_SWAP_EXCHANGE_ARB              0x2028
#define WGL_SWAP_COPY_ARB                  0x2029
#define WGL_SWAP_UNDEFINED_ARB             0x202A
#define WGL_TYPE_RGBA_ARB                  0x202B
#define WGL_TYPE_COLORINDEX_ARB            0x202C

// WGL_NV_multisample_coverage

#define WGL_COVERAGE_SAMPLES_NV            0x2042
#define WGL_COLOR_SAMPLES_NV               0x20B9

#define ERROR_INVALID_PROFILE_ARB					0x2096
#define ERROR_INVALID_VERSION_ARB					0x2095
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 	0x00000002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB			0x00000001
#define WGL_CONTEXT_DEBUG_BIT_ARB					0x0001
#define WGL_CONTEXT_FLAGS_ARB						0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB		0x0002
#define WGL_CONTEXT_LAYER_PLANE_ARB					0x2093
#define WGL_CONTEXT_MAJOR_VERSION_ARB				0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB				0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB				0x9126


void glSampleCoverageARB(GLclampf value, GLboolean invert);

BOOL wglGetPixelFormatAttribivARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int* piValues);
BOOL wglGetPixelFormatAttribfvARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT* pfValues);
BOOL wglChoosePixelFormatARB(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

HGLRC wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int* attribList);

BOOL wglSwapIntervalEXT(int interval);
int wglGetSwapIntervalEXT();

namespace gl
{
	// Returns the highest quality hardware accelerated anti-aliasing pixel format.
	// If a suitable MSAA or CSAA pixel format was found it will be returned in the
	// 'pf' integer reference argument. Otherwise '0' will be returned.
	void ChooseBestAntiAliasingPixelFormat(int& pf);

	// Returns a hardware accelerated anti-aliasing (MSAA or CSAA) pixel format
	// that supports the specified samples per pixel. If a matching MSAA or CSAA
	// pixel format was found it will be returned in the 'pf' integer reference
	// argument. Otherwise '0' will be returned.
	void ChooseAntiAliasingPixelFormat(int& pf, int samples);

	// Returns a string containing a description of the anti-aliasing pixel format
	// selected by the most recent call to ChooseBestAntiAliasingPixelFormat() or
	// ChooseAntiAliasingPixelFormat().
	const char* GetAntiAliasingPixelFormatString();
}

#endif

#endif // SE_OPENGL