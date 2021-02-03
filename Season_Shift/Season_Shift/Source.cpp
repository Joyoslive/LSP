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

	PhysicsEngine pe = PhysicsEngine();
	pe.addScene(sceneManager.getActiveScene());

	Ref<GameObject> go1 = std::make_shared<GameObject>();
	go1->AddComponent(std::make_shared<Transform>());
	go1->getTransform()->setPosition(DirectX::XMFLOAT3(-3, 0, 0));
	go1->AddComponent(std::make_shared<SphereCollider>(2.0f));

	std::vector<Ref<Collider>> testVec = pe.checkCollide(go1->getComponentType<Collider>(Component::ComponentEnum::COLLIDER));
	if (testVec.size() > 0)
	{
		std::wstring outPut = L"det fungerar";
		OutputDebugString(outPut.c_str());
	}

	// Material
	auto mat1 = gph.getResourceDevice()->createMaterial(GfxShader::DEFAULT,
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_basecolor.jpg",
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_basecolor.jpg",
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_normal.jpg");

	//auto mat2 = gph.getResourceDevice()->createMaterial("DefaultVS.cso", "DefaultPS.cso",
	//	"Textures/Stylized_02_Stone_Ground/Stylized_02_Stone_Ground_basecolor.jpg",
	//	"Textures/Stylized_02_Stone_Ground/Stylized_02_Stone_Ground_basecolor.jpg",
	//	"Textures/Stylized_02_Stone_Ground/Stylized_02_Stone_Ground_basecolor.jpg",
	//	"Textures/Stylized_02_Stone_Ground/Stylized_02_Stone_Ground_normal.jpg");

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
	models.push_back(quadMod2);

	MSG msg = { };
	while (!win.isClosed())
	{
		while (PeekMessageW(&msg, win.getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		sceneManager.updateActiveScene();

		// Do stuff
		gph.render(models);

	}



	return 0;
}