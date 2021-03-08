#include "stdafx.h"
#include "Engine.h"
#include "GameApp.h"
#include "TestApp.h"
#include "GraphicsSystemTest.h"
#include "RendererSystemTest.h"
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

#if SE_OPENGL
#	pragma message("link to OpenGL32.lib")
#   pragma comment(lib, "OpenGL32.lib")
#endif
#if SE_D3D11
#	pragma message("link to D3D11.lib")
#   pragma comment(lib, "D3D11.lib")
#	pragma message("link to d3dcompiler.lib")
#   pragma comment(lib, "d3dcompiler.lib")
//#	pragma message("link to dxgi.lib")
//#   pragma comment(lib, "dxgi.lib")
#	pragma message("link to dxguid.lib")
#   pragma comment(lib, "dxguid.lib")
#endif
#	pragma message("link to winmm.lib")
#   pragma comment(lib, "winmm.lib")
#endif



как перетаскивтаь урхо
	- либо перетаскивать малосвязные системы (типа контейнеров)
	- либо сначала свести менеджеры, а потом по отдельности сущности

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
			RendererSystemTest game;
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