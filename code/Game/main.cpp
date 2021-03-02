#include "stdafx.h"
#include "Engine.h"
#include "GameApp.h"
#include "TestApp.h"
#include "Log.h"
#if SE_COMPILER_MSVC
#	pragma message("link to Engine.lib")
#   pragma comment(lib, "Engine.lib")
#	pragma message("link to Other.lib")
#   pragma comment(lib, "Other.lib")
#	pragma message("link to Other2.lib")
#   pragma comment(lib, "Other2.lib")
#	pragma message("link to 3rdparty.lib")
#   pragma comment(lib, "3rdparty.lib")

#	pragma message("link to OpenGL32.lib")
#   pragma comment(lib, "OpenGL32.lib")
#	pragma message("link to winmm.lib")
#   pragma comment(lib, "winmm.lib")
#endif

Gedonia

идея такая - сделать большой ландшафт с открытым миром
сеттинг магически средневековый
вокруг полно монстров

игрок должен пройти путь по разрушенным аванпостам от старта до финиша - дерев

//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	try
	{
		Engine& engine = Engine::Get();
		{
#if 0
			GameApp game;
#else
			TestApp game;
#endif
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
		std::string str = "Application initialization failed!\n";
		str += e.what();
		SE_LOG(str);
#if SE_PLATFORM_WINDOWS
		MessageBoxA(0, str.c_str(), "Error", MB_ICONERROR);
#endif
	}

	return 0;
}
//-----------------------------------------------------------------------------