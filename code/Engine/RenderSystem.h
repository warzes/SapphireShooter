#pragma once

#include "NonCopyable.h"
#include "OGLFunc.h"
#include "RenderDescription.h"
#include "Log.h"

class RenderSystem final : NonCopyable
{
public:
	RenderSystem() = default;
	~RenderSystem();

#if SE_PLATFORM_WINDOWS
	bool Init(HWND hwnd, const RenderDescription& config);
#endif
	void Close();

	void BeginFrame(int width, int height);
	void EndFrame();

	void SetVsync(bool enable);

private:

#if SE_PLATFORM_WINDOWS
#if SE_OPENGL
	HDC m_deviceContext = nullptr;
	HGLRC m_renderContext = nullptr;
#endif
	HWND m_hwnd = nullptr;
#endif

	int m_openGLMajorVersion = 0;
	int m_openGLMinorVersion = 0;

	// TODO:
	static const int BEST_ANTI_ALIASING_SAMPLES = -1;
	int m_antiAliasingSamples = 0;
	// TODO END

	bool m_isInit = false;
};