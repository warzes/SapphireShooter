#pragma once

#include "EngineDescription.h"
#include "Camera3D.h"
#include "Model.h"

class TestApp
{
public:
	TestApp();
	~TestApp();

	EngineDescription InitConfig();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Close();

private:
	void resizeApp();

	Camera3D m_mainCamera;

	Model m_model;

	int m_width = 0;
	int m_height = 0;
};