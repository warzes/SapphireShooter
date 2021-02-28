#include "stdafx.h"
#include "Engine.h"
#include "GameApp.h"
#include "Log.h"
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
				
				while (!engine.IsEnd())
				{
					engine.BeginUpdate();
					game.ProcessInput(engine.GetDeltaTime());
					game.Update(engine.GetDeltaTime());
					engine.EndUpdate();
					engine.BeginFrame();
					game.Render();
					engine.EndFrame();					
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
		SE_LOG(msg.str().c_str());
#if SE_PLATFORM_WINDOWS
		MessageBoxA(0, msg.str().c_str(), "Error", MB_ICONERROR);
#endif
	}

	return 0;
}
//-----------------------------------------------------------------------------