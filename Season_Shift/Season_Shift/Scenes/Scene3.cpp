#include "Scene3.h"
#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"
#include "../PlayerJumpTrigger.h"

using namespace DirectX::SimpleMath;

Scene3::Scene3(Graphics* graphics) : Scene(graphics)
{

}

Scene3::~Scene3()
{

}

void Scene3::setUpScene()
{
	Ref<GameObject> player = createGameObject("player", Vector3(0, 10, 0), Vector3(1.0f, 1.0f, 1.0f));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(std::make_shared<CapsuleCollider>(1, 4));
	//player->AddComponent(std::make_shared<SphereCollider>(1));
	
	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0,0,0), Vector3(2, 2, 2));
	playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));

	Ref<GameObject> brickCube = createGameObject("brickCube", Vector3(0, -50, 0.0f), Vector3(2*5, 1, 2*5));
	brickCube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/ground/", "200x2x200Box.obj", GfxShader::DEFAULT));
	brickCube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(400*5, 2, 400*5)));

	Ref<GameObject> start = createGameObject("brickCube", Vector3(0, -50, 0.0f), Vector3(2 * 2, 1, 2 * 2));
	start->AddComponent(m_graphics->getResourceDevice()->createModel("Models/ground/", "200x2x200Box.obj", GfxShader::DEFAULT));
	start->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(400 * 2, 2, 400 * 2)));


	Ref<GameObject> cube = createGameObject("brickCube", Vector3(0, 5.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube2 = createGameObject("brickCube", Vector3(50.0f, 15.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube3 = createGameObject("brickCube", Vector3(300.0f, 45.0f, -20.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube4 = createGameObject("brickCube", Vector3(150.0f, 40.0f, 5.0f), Vector3((1.0f / 10.0f) * 1.5, 1, 1.0f / 10.0f));
	cube4->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube4->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*1.5, 2, 20)));

	Ref<GameObject> cube5 = createGameObject("brickCube", Vector3(70.0f, 95.0f, 10.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube6 = createGameObject("brickCube", Vector3(-80.0f, 45.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube6->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube6->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube7 = createGameObject("brickCube", Vector3(50.0f, 20.0f, -60.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube7->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube7->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube8= createGameObject("brickCube", Vector3(150.0f, 30.0f, -200.0f), Vector3(1.0f / 10.0f, 1, (1.0f / 10.0f)*1.5));
	cube8->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube8->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20*1.5)));

	Ref<GameObject> cube9 = createGameObject("brickCube", Vector3(-200.0f, 1.0f, -20.0f), Vector3(1.0f / 10.0f, 1000, 1.0f / 10.0f));
	cube9->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube9->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2*1000, 20)));

	Ref<GameObject> cube10 = createGameObject("brickCube", Vector3(-200.0f, 1.0f, 20.0f), Vector3(1.0f / 10.0f, 500, 1.0f / 10.0f));
	cube10->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube10->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2*500, 20)));


	Ref<GameObject> cube11 = createGameObject("brickCube", Vector3(-300.0f, 200.0f, 250.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube11->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube11->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));


	Ref<GameObject> cube12 = createGameObject("brickCube", Vector3(300.0f, 100.0f, 250.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube12->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube12->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));


	Ref<GameObject> cube13 = createGameObject("brickCube", Vector3(-300.0f, 200.0f, -250.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube13->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube13->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));


	Ref<GameObject> cube14 = createGameObject("brickCube", Vector3(400.0f, 10.0f, 400.0f), Vector3((1.0f / 10.0f)*200, 100, 1.0f / 10.0f));
	cube14->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube14->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*200, 2*100, 20)));

	m_graphics->loadSkybox("Textures/Skyboxes/gl");
	m_graphics->setSkybox(1);	// 0 är tagen (default)

}