#include "Window.h"
#include "GameObject.h"


bool b = true;
int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
	// Game game();
	// while(game.run());

	Window win(inst, L"first win", 1280, 720);	
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
	MSG msg = { };
	while (!win.isClosed())
	{
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Do stuff
		if (b)
		{
			

			go->testAdd();
			std::shared_ptr<RigidBodyComponent> rg = go->getComponentType<RigidBodyComponent>();

			
			std::wstring hej = std::to_wstring(rg->getMass());
			OutputDebugString(hej.c_str());

			b = false;
		}

	}


	return 0;
}