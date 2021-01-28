#pragma once

#include "EngineDescription.h"

class Game
{
public:
	Game();
	~Game();

	EngineDescription InitConfig();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Close();

private:
	void loadResources();
};