#include "stdafx.h"
#include "Engine.h"
#include "GameApp.h"
#include "Timer.h"
#if SE_COMPILER_MSVC
#   pragma comment(lib, "OpenGL32.lib")
#   pragma comment(lib, "winmm.lib")
#endif
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	try
	{
		Engine& engine = Engine::Get();
		{
			GameApp game;
			if (engine.Init(game.InitConfig()))
			{
				game.Init();

				// TODO: dt перенести в Engine
				HiresTimer frameTimer;
				float dt = 0.0f;
				while (!engine.IsEnd())
				{
					frameTimer.Reset();
					engine.BeginUpdate();
					game.ProcessInput(dt);
					game.Update(dt);
					engine.EndUpdate();
					engine.BeginFrame();
					game.Render();
					engine.EndFrame();
					dt = frameTimer.ElapsedUSec() * 0.000001f;
				}
			}
			game.Close();
		}
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