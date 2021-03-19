#include "pch.h"

#include "Window.h"
#include "Graphics/Graphics.h"
#include <string>
#include "GameObject.h"
#include "PhysicsEngine.h"
#include "SceneManager.h"
#include "DebugCamera.h"
#include "CameraComponent.h"
#include "Logger.h"
#include "Input.h"
#include "Player.h"
#include "CameraSwitch.h"
#include "Rotate.h"
#include "FrameTimer.h"

using namespace DirectX::SimpleMath;

constexpr int resHeight = 720;
constexpr int resWidth = 1280;

//constexpr int resHeight = 1080;
//constexpr int resWidth = 1920;

//constexpr int resHeight = 1440;
//constexpr int resWidth = 2560;

//constexpr int resHeight = 2160;
//constexpr int resWidth = 3840;

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
	//_CrtDumpMemoryLeaks(); //memory leak detection
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(805);
	//_CrtSetBreakAlloc(1581);
	//_CrtSetBreakAlloc(1582);


	Window win(inst, L"Season Shift", resWidth, resHeight);	

	Graphics gph(win.getHWND(), win.getClientWidth(), win.getClientHeight());

	win.setOnResizeCallback([&gph](UINT width, UINT height) { gph.onResize(width, height); }); //onResize is not static, lamda solves this
	win.setFullScreenCallback([&gph](bool fullScreen) { gph.setFullScreen(fullScreen); }, [&gph]() -> bool { return gph.getFullScreenState(); } ); //not static, lamda solves this

	//Init singleton input with window handle
	Input::initInput(win.getHWND(), resWidth, resHeight);

	// Init the scene manager object
	SceneManager sceneManager(&gph, &win);

	// Change active scene to Main Menu
	sceneManager.changeScene(0); // Main Menu
	//Ref<Scene> scene = sceneManager.getActiveScene();

	//Ref<GameObject> player = scene->getGameObject("player");
	

	std::shared_ptr<PhysicsEngine> physicsEng = std::make_shared<PhysicsEngine>(1.0/120.0);
	sceneManager.addObserver(physicsEng);

	FrameTimer timer;

	Ref<Camera> cam = std::make_shared<Camera>(0, 0, -50, resWidth, resHeight);
	
	DebugCamera debugCamera(cam);

	std::shared_ptr<CameraSwitch> camSwitch = std::make_shared<CameraSwitch>();
	camSwitch->Init(&debugCamera, nullptr, cam);
	sceneManager.addObserver(camSwitch);

	// ImGUI
	int a = 500;
	bool b = false;
	float myFloats[3] = {};

	MSG msg = { };
	while (!win.isClosed())
	{
		timer.frameStart();
		/*ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();*/
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Example
		/*ImGui::Begin("App Statistics");
		{
			ImGui::Text("Elapsed Time = %f", &a);
			ImGui::Text("FPS = %f", 1.0f / timer.dt());
			ImGui::Text("DisplaySize = %f, %f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
			ImGui::Checkbox("My Checkbox", &b);
			ImGui::SliderFloat3("Float3", myFloats, 0.0, 5.0);
		}
		ImGui::End();*/

		sceneManager.updateActiveScene();

		if (!sceneManager.currentScenePaused())
			physicsEng->simulate(timer.dt());

		Input::getInput().update(timer.dt());
		if (Input::getInput().keyPressed(Input::F11))
		{
			gph.setFullScreen(!gph.getFullScreenState());
		}
		
		// Do stuff
		//input->update();
		//camSwitch.update(timer.dt());
		camSwitch->update(timer.dt());
		auto player = sceneManager.getActiveScene()->getGameObject("player");
		if (player)
		{
			player->getComponentType<Player>(Component::ComponentEnum::LOGIC)->setFrametime(timer.dt());
		}
		
		auto scene = sceneManager.getActiveScene();
		//gph.render(scene->getSceneModels(), scene->getSceneMainCamera(), timer.dt());
		gph.render(scene->getSceneModels(), camSwitch->getCamera(), timer.dt());
		//gph.render(scene->getSceneModels(), cam, timer.dt());
		timer.frameStop();

		if (sceneManager.shouldQuit())
		{
			win.quit();
		}
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}