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
	//_CrtSetBreakAlloc(811);
	//_CrtSetBreakAlloc(810);
	//_CrtSetBreakAlloc(809);


	Window win(inst, L"Season Shift", resWidth, resHeight);	

	Graphics gph(win.getHWND(), win.getClientWidth(), win.getClientHeight());

	win.setOnResizeCallback([&gph](UINT width, UINT height) { gph.onResize(width, height); }); //onResize is not static, lamda solves this
	win.setFullScreenCallback([&gph](bool fullScreen) { gph.setFullScreen(fullScreen); }, [&gph]() -> bool { return gph.getFullScreenState(); } ); //not static, lamda solves this

	//Input input = Input(win.getHWND());
	//Ref<Input> input = std::make_shared<Input>(win.getHWND());
	//Init singleton with window handler
	Input::initInput(win.getHWND(), resWidth, resHeight);

	// Material

	SceneManager sceneManager = SceneManager(&gph);

	sceneManager.changeScene(4);
	Ref<Scene> scene = sceneManager.getActiveScene();

	Ref<GameObject> player = scene->getGameObject("player");
	
	//sceneManager.getActiveScene()->start();

	std::shared_ptr<PhysicsEngine> physicsEng = std::make_shared<PhysicsEngine>(1.0/300.0);
	sceneManager.addObserver(physicsEng);

	//// Material

	//// Geometry
	//std::vector<Vertex> verts;
	//verts.push_back({ Vector3(-0.75, 0.75, 0.0), Vector2(0.0, 0.0), Vector3(0.0, 0.0, -1.0) });
	//verts.push_back({ Vector3(0.75, 0.75, 0.0), Vector2(1.0, 0.0), Vector3(0.0, 0.0, -1.0) });
	//verts.push_back({ Vector3(0.75, -0.75, 0.0), Vector2(1.0, 1.0), Vector3(0.0, 0.0, -1.0) });
	//verts.push_back({ Vector3(-0.75, -0.75, 0.0), Vector2(0.0, 1.0), Vector3(0.0, 0.0, -1.0) });

	//std::vector<uint32_t> indices = {
	//	0, 1, 2,
	//	0, 2, 3
	//};

	//// Create mesh with id!
	//auto mesh = gph.getResourceDevice()->createMesh("quad", verts, indices);

	//// Assemble to model!
	////auto quadMod1 = gph.getResourceDevice()->assembleModel("quad", mat1);
	////auto quadMod2 = gph.getResourceDevice()->assembleModel("quad", mat1);

	//std::vector<std::shared_ptr<Model>> models;
	////models.push_back(quadMod2);


	//auto nanosuitMod = gph.getResourceDevice()->createModel("Models/nanosuit/" , "nanosuit.obj", GfxShader::DEFAULT);
	//models.push_back(nanosuitMod);

	//auto nanosuitMod2 = gph.getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	//models.push_back(nanosuitMod2);

	FrameTimer timer = FrameTimer();

	Ref<Camera> cam = std::make_shared<Camera>(0, 0, -50, resWidth, resHeight);
	
	DebugCamera debugCamera(cam);

	CameraSwitch camSwitch;
	camSwitch.Init(&debugCamera, player, cam);


	// ImGUI
	int a = 500;
	bool b = false;
	float myFloats[3] = {};

	MSG msg = { };
	while (!win.isClosed())
	{
		timer.frameStart();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Example
		ImGui::Begin("App Statistics");
		{
			ImGui::Text("Elapsed Time = %f", &a);
			ImGui::Text("FPS = %f", 1.0f / timer.dt());
			ImGui::Text("DisplaySize = %f, %f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
			ImGui::Checkbox("My Checkbox", &b);
			ImGui::SliderFloat3("Float3", myFloats, 0.0, 5.0);
		}
		ImGui::End();

		sceneManager.updateActiveScene();
		physicsEng->simulate(timer.dt());
		Input::getInput().update(timer.dt());
		if (Input::getInput().keyBeingPressed(Input::F11))
		{
			gph.setFullScreen(!gph.getFullScreenState());
		}
		
		// Do stuff
		//input->update();
		camSwitch.update(timer.dt());
		player->getComponentType<Player>(Component::ComponentEnum::LOGIC)->setFrametime(timer.dt());
		
		gph.render(sceneManager.getActiveScene()->getSceneModels(), camSwitch.getCamera(), timer.dt());
		timer.frameStop();

	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}