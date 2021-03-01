#include "pch.h"
#include "Scene1.h"
#include "../GameObject.h"
#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"

#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"
#include "../PlayerJumpTrigger.h"
#include "../Rotate.h"
#include "../RigidBody.h"
#include "../Transform.h"
#include "../Logic.h"
#include "../Collider.h"
#include "../SphereCollider.h"
#include "../OrientedBoxCollider.h"
#include "../CapsuleCollider.h"
#include "../Sound.h"
#include "../Graphics/Model.h"
#include "../ParticleSystemComponent.h"

using namespace DirectX::SimpleMath;

Scene1::Scene1(Graphics * graphics) : Scene(graphics)
{

}

Scene1::~Scene1()
{

}

void Scene1::setUpScene()
{
	Ref<Model> model = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	Ref<Model> model2 = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	Ref<Model> model3 = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	Ref<Model> model4 = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	Ref<Model> model5 = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);
	Ref<Model> model6 = m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT);

	createGameObject();
	createGameObject("GameObject1");
	Ref<GameObject> gObj = createGameObject("GameObject2", Vector3(12, 4, 6));
	gObj->AddComponent(std::make_shared<RigidBody>());
	Ref<Test> test = std::make_shared<Test>();
	gObj->AddComponent(test);
	Ref<Logic> logic = gObj->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
	//destroyGameObject(gObj);

	Ref<GameObject> partSysGo = createGameObject("partSysGo", Vector3(12, 4, 6));
	Ref<ParticleSystem> partSys = std::dynamic_pointer_cast<ParticleSystem>(
		partSysGo->AddComponent(std::make_shared<ParticleSystemComponent>(200))
		);



	Ref<GameObject> sphere = createGameObject("sphere", Vector3(0, 0, -40), Vector3(0.2f, 0.2f, 0.2f), Vector3(0, 180, 0));
	sphere->AddComponent(std::make_shared<SphereCollider>(1.0f));
	sphere->AddComponent(std::make_shared<RigidBody>());
	sphere->AddComponent(model5);

	Ref<GameObject> collider = createGameObject("colliderTest1", Vector3(0, -5.0f, -40), Vector3(0.2f, 0.2f, 0.2f), Vector3(0, 0.0, 0));
	collider->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(1.0f, 1.0f, 1.0f)));
	collider->AddComponent(model6);
	collider->AddComponent(std::make_shared<Test>());

	Ref<GameObject> go1 = createGameObject("colliderTest1", Vector3(2, 0, -40), Vector3(0.2f, 0.2f, 0.2f), Vector3(0, 180, 0));
	go1->AddComponent(std::make_shared<SphereCollider>(2.0f));
	go1->AddComponent(model);
	go1->AddComponent(std::make_shared<Test>());

	Ref<GameObject> go2 = createGameObject("colliderTest1", Vector3(-2, 0, -40), Vector3(0.2f, 0.2f, 0.2f), Vector3(0, 90, 0));
	go2->AddComponent(model2);

	Ref<GameObject> go3 = createGameObject("colliderTest1", Vector3(-6, 0, -40), Vector3(0.2f, 0.2f, 0.2f), Vector3(0, 270, 0));
	go3->AddComponent(model3);

	Ref<GameObject> go4 = createGameObject("Model4", Vector3(6, 0, -40), Vector3(0.2f, 0.2f, 0.2f));
	go4->AddComponent(model4);

	Ref<GameObject> player = createGameObject("player", Vector3(-2, 0, -20), Vector3(0.2f, 0.2f, 0.2f));
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<SphereCollider>(2));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));

	Ref<GameObject> coll = createGameObject("collider", Vector3(-2, -8, -20), Vector3(100, 100, 100), Vector3(90, 0, 0));
	coll->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(120, 120, 1)));
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
	auto mesh = m_graphics->getResourceDevice()->createMesh("quad", verts, indices);

	auto mat1 = m_graphics->getResourceDevice()->createMaterial(GfxShader::DEFAULT,
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_basecolor.jpg",
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_basecolor.jpg",
		"Textures/Stylized_01_Bricks/Stylized_01_Bricks_normal.jpg");

	// Assemble to model!
	auto quadMod1 = m_graphics->getResourceDevice()->assembleModel("quad", mat1);

	coll->AddComponent(quadMod1);
	//coll->AddComponent(m_graphics->getResourceDevice()->createModel("Models/cube/", "Cube.obj", GfxShader::DEFAULT));
	Ref<GameObject> go5 = createGameObject("Box", Vector3(0, 0, 0), Vector3(1.f, 1.f, 1.f));
	go5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Cube/", "Cube.obj", GfxShader::DEFAULT));
	go5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(1, 1, 1)));
}