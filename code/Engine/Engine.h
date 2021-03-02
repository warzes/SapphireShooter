#pragma once

#include "NonCopyable.h"
#include "EngineDescription.h"
#include "Window.h"
#include "RenderSystem.h"
#include "Timer.h"

class Engine final : NonCopyable
{
public:
	static Engine& Get() noexcept
	{
		static Engine engineImpl;
		return engineImpl;
	}

	bool Init(const EngineDescription& desc);
	void BeginUpdate();
	void EndUpdate();
	void BeginFrame();
	void EndFrame();
	void Close() noexcept;

	EngineDescription& GetDescription() noexcept
	{
		return m_desc;
	}

	bool IsEnd() const noexcept { return m_isEnd; }

	float GetDeltaTime() const noexcept { return m_deltaTime; }
	double GetTime() const noexcept { return m_time; }

private:
	Engine() = default;

	EngineDescription m_desc;
	Window m_window;
	RenderSystem m_renderSystem;
	bool m_isEnd = false;
	HiresTimer m_frameTimer;
	float m_deltaTime = 0.0f;
	double m_time = 0.0;
};

inline EngineDescription& GetEngineDescription() noexcept
{
	return Engine::Get().GetDescription();
}