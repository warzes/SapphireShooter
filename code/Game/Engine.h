#pragma once

#include "EngineDescription.h"
#include "Window.h"
#include "RenderSystem.h"
#include "Timer.h"

class Engine final
{
public:
	bool Init(const EngineDescription& desc);
	void BeginUpdate();
	void EndUpdate();
	void BeginFrame();
	void EndFrame();
	void Close() noexcept;

	static Engine& Get() noexcept
	{
		static Engine engineImpl;
		return engineImpl;
	}

	EngineDescription& GetDescription() noexcept
	{
		assert(m_isRun);
		return m_desc;
	}

	bool IsEnd() const noexcept { return m_isEnd; }
	bool IsRun() const noexcept { return m_isRun; }

	float GetDeltaTime() const { return m_deltaTime; }

private:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	EngineDescription m_desc;
	Window m_window;
	RenderSystem m_renderSystem;
	bool m_isRun = false;
	bool m_isEnd = false;
	HiresTimer m_frameTimer;
	float m_deltaTime = 0.0f;
};

inline EngineDescription& GetEngineDescription() noexcept
{
	return Engine::Get().GetDescription();
}