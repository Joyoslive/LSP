#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
	// Game game();
	// while(game.run());

	Window win(inst, L"first win", 1280, 720);	

	MSG msg = { };
	while (!win.isClosed())
	{
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Do stuff


	}


	return 0;
}