#pragma once

#include "EngineDescription.h"
#include "Window.h"
#include "RenderSystem.h"

class Engine final
{
public:
	Engine() = default;
	bool Init(const EngineDescription& desc);
	void BeginUpdate();
	void EndUpdate();
	void BeginFrame();
	void EndFrame();
	void Close() noexcept;

	EngineDescription& GetDescription() noexcept
	{
		assert(!m_isRun);
		return m_desc;
	}

	bool IsEnd() const noexcept { return m_isEnd; }
	bool IsRun() const noexcept	{ return m_isRun; }

private:
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	EngineDescription m_desc;
	Window m_window;
	RenderSystem m_renderSystem;
	bool m_isRun = false;
	bool m_isEnd = false;
};

inline Engine& GetEngine() noexcept
{
	static Engine engineImpl;
	return engineImpl;
}

inline EngineDescription& GetEngineDescription() noexcept
{
	return GetEngine().GetDescription();
}