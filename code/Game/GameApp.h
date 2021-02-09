#pragma once

#include "EngineDescription.h"
#include "Camera.h"
#include "Framebuffer.h"

class GameApp
{
public:
	GameApp();
	~GameApp();

	EngineDescription InitConfig();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Close();

private:
	void resizeApp();
	Camera m_mainCamera;
	Framebuffer m_framebuffer;

	int m_width = 0;
	int m_height = 0;
};