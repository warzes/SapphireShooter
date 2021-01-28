#pragma once

#include "OGLFunc.h"
#include "RenderDescription.h"


class RenderSystem
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

	void SetVsync();

private:
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem& operator=(RenderSystem&&) = delete;

#if SE_PLATFORM_WINDOWS
	HDC m_deviceContext = nullptr;
	HGLRC m_renderContext = nullptr;
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