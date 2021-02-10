#pragma once

#include "EngineDescription.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Terrain.h"
#include "Model.h"
#include "Font.h"
#include "Billboard.h"

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
	Terrain m_terrain;
	DirectionalLight m_dirLight;
	PointLight m_pointLight;

	std::vector<Text> m_texts;

	Billboard* m_test;

	//Model m_jeep;
	Model m_swords;

	int m_width = 0;
	int m_height = 0;
};