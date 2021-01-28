#include "stdafx.h"
#include "Engine.h"
#include "Game.h"
#if SE_COMPILER_MSVC
#   pragma comment(lib, "OpenGL32.lib")
#endif
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
	try
	{
		Engine& engine = Engine::Get();

		Game game;
		if (engine.Init(game.InitConfig()))
		{
			game.Init();

			while (!engine.IsEnd())
			{
				float dt = 0.002f; // TODO:

				engine.BeginUpdate();
				game.ProcessInput(dt);
				game.Update(dt);
				engine.EndUpdate();
				engine.BeginFrame();
				game.Render();
				engine.EndFrame();
			}
		}
		game.Close();
		engine.Close();
	}
	catch (const std::exception& e)
	{
		std::ostringstream msg;
		msg << "Application initialization failed!" << std::endl << std::endl;
		msg << e.what();
#if SE_PLATFORM_WINDOWS
		MessageBoxA(0, msg.str().c_str(), "Error", MB_ICONERROR);
#endif
	}

	return 0;
}
//-----------------------------------------------------------------------------