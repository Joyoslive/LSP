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

using namespace DirectX::SimpleMath;

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
	Window win(inst, L"Season Shift", 1280, 720);	

	Graphics gph(win.getHWND(), win.getClientWidth(), win.getClientHeight());

	//Input input = Input(win.getHWND());
	//Ref<Input> input = std::make_shared<Input>(win.getHWND());
	//Init singleton with window handler
	Input::initInput(win.getHWND());

	PhysicsEngine pe = PhysicsEngine();
	// Material
	auto mat1 = gph.getResourceDevice()->createMaterial(GfxShader::DEFAULT,
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_basecolor.jpg",
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_basecolor.jpg",
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_normal.jpg");

	SceneManager sceneManager = SceneManager(&gph);
	Ref<Scene> scene = sceneManager.getActiveScene();

	Ref<GameObject> player = scene->createGameObject("player",  Vector3(-2, 0, -20), Vector3(0.2f, 0.2f, 0.2f));
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<SphereCollider>(2));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(gph.getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));
	
	sceneManager.getActiveScene()->start();

	std::shared_ptr<PhysicsEngine> physicsEng = std::make_shared<PhysicsEngine>();
	sceneManager.addObserver(physicsEng);

	// Material

	// Geometry
	std::vector<Vertex> verts;
	verts.push_back({ Vector3(-0.75, 0.75, 0.0), Vector2(0.0, 0.0), Vector3(0.0, 0.0, -1.0) });
	verts.push_back({ Vector3(0.75, 0.75, 0.0), Vector2(1.0, 0.0), Vector3(0.0, 0.0, -1.0) });
	verts.push_back({ Vector3(0.75, -0.75, 0.0), Vector2(1.0, 1.0), Vector3(0.0, 0.0, -1.0) });
	verts.push_back({ Vector3(-0.75, -0.75, 0.0), Vector2(0.0, 1.0), Vector3(0.0, 0.0, -1.0) });

	std::vector<uint32_t> indices = {
		0, 1, 2,
		0, 2, 3
	};

	// Create mesh with id!
	auto mesh = gph.getResourceDevice()->createMesh("quad", verts, indices);

	// Assemble to model!
	//auto quadMod1 = gph.getResourceDevice()->assembleModel("quad", mat1);
	//auto quadMod2 = gph.getResourceDevice()->assembleModel("quad", mat1);

	std::vector<std::shared_ptr<Model>> models;
	//models.push_back(quadMod2);


	auto nanosuitMod = gph.getResourceDevice()->createModel("Models/nanosuit/" , "nanosuit.obj", GfxShader::DEFAULT);
	models.push_back(nanosuitMod);

	auto nanosuitMod2 = gph.getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	models.push_back(nanosuitMod2);

	Timer m_timer = Timer();

	Ref<Camera> cam = std::make_shared<Camera>(0, 0, -50, 1280, 720);
	
	DebugCamera debugCamera(cam);

	Ref<GameObject> gameObject = sceneManager.getActiveScene()->getGameObject("Model4");

	bool m_cameraCheck = false;
	MSG msg = { };
	while (!win.isClosed())
	{
		m_timer.start();
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		
		sceneManager.updateActiveScene();
		Ref<RigidBody> temp = sceneManager.getActiveScene()->getGameObject("sphere")->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
		physicsEng->simulate(player->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY)/*temp*/, m_timer.dt());
		Input::getInput().update();
		// Do stuff
		//input->update();
		if (Input::getInput().keyPressed(Input::C)) 
		{
			if( m_cameraCheck == true)
			{
				m_cameraCheck = false;
			}
			else
			{
				m_cameraCheck = true;
			}
		}
		if (m_cameraCheck == true)
		{
			debugCamera.rotate();
			debugCamera.move();
			gph.render(sceneManager.getActiveScene()->getSceneModels(), cam);
		}
		else
		{
			gph.render(sceneManager.getActiveScene()->getSceneModels(), player->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera());//cam);//player->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera());//cam);
		}
			
			
		

		m_timer.stop();
	}



	return 0;
}