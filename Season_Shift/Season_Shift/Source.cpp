#include "Window.h"
#include "Graphics/Graphics.h"
#include <string>
#include "GameObject.h"
#include "PhysicsEngine.h"
#include "SceneManager.h"

using namespace DirectX::SimpleMath;

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
	Window win(inst, L"Season Shift", 1280, 720);	

	Graphics gph(win.getHWND(), win.getClientWidth(), win.getClientHeight());


	SceneManager sceneManager = SceneManager();
	Ref<Scene> scene = sceneManager.getActiveScene();

	Ref<GameObject> player = scene->createGameObject("player");
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<SphereCollider>(2));

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
	auto quadMod1 = gph.getResourceDevice()->assembleModel("quad", mat1);
	auto quadMod2 = gph.getResourceDevice()->assembleModel("quad", mat1);

	std::vector<std::shared_ptr<Model>> models;
	//models.push_back(quadMod2);


	auto nanosuitMod = gph.getResourceDevice()->createModel("Models/nanosuit/" , "nanosuit.obj", GfxShader::DEFAULT);
	models.push_back(nanosuitMod);



	MSG msg = { };
	while (!win.isClosed())
	{
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		sceneManager.updateActiveScene();
		Ref<RigidBody> temp = player->getComponentType<RigidBody>(Component::ComponentEnum::RIGID_BODY);
		physicsEng->simulate(temp);

		// Do stuff
		gph.render(models);

	}



	return 0;
}