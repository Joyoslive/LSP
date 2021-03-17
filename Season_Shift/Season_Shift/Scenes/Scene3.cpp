#include "pch.h"
#include "Scene3.h"
#include "../GameObject.h"
#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"
#include "../PlayerJumpTrigger.h"

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
#include "../SceneReader.h"
#include "../Bounce.h"
#include "../Move.h"

#include "../GoalLogic.h"
#include "../TableOfTimes.h"

using namespace DirectX::SimpleMath;

Scene3::Scene3(Graphics* graphics) : Scene(graphics)
{

}

Scene3::~Scene3()
{

}

void Scene3::setUpScene()
{
	SceneReader sr;
	auto vec = sr.readSceneFromFile("Scenes/SceneJSONs/FallScene.json");

	std::vector<Ref<GameObject>> moveGameObjects;

	for (auto& object : vec)
	{
		if (object.name == "Checkpoint")
		{
			Ref<GameObject> cp1 = createGameObject("checkpoint", object.position, Vector3(2, 2, 2));
			cp1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/checkpoint/", "checkpoint.obj", GfxShader::DEFAULT));
			Ref<OrientedBoxCollider> r1 = std::make_shared<OrientedBoxCollider>(Vector3(10, 5, 10));
			r1->SetTriggerCollider(true);
			cp1->AddComponent(r1);
			cp1->AddComponent(std::make_shared<Rotate>(0, 70, 0));
			continue;
		}
		auto go = createGameObject(object.name, object.position, object.scale, object.rotation);
		auto model = m_graphics->getResourceDevice()->createModel(object.meshDirectory, object.meshFileName, GfxShader::DEFAULT);

		go->AddComponent(model);
		if (object.name == "Collider")
			go->setIsVisible(false);
		if (object.name == "moving")
		{
			moveGameObjects.push_back(go);
		}

		for (auto& b : object.boxColliders)
		{
			auto collider = std::make_shared<OrientedBoxCollider>(b.scale, b.position);
			go->AddComponent(collider);
		}
		/*auto temp = object.boxCollider.x + object.boxCollider.y + object.boxCollider.z;
		if (!(-0.0001 < temp && temp < 0.0001))
		{
			auto collider = std::make_shared<OrientedBoxCollider>(object.boxCollider);
			go->AddComponent(collider);
		}*/
	}

	// Post setup, like cameras and logic
	auto player = createGameObject("player", Vector3(0, 25, -180.1f));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	auto playerComp = std::make_shared<Player>();
	playerComp->setRespawn({ 0, 25, 0 });
	player->AddComponent(playerComp);
	player->AddComponent(std::make_shared<CapsuleCollider>(0.5, 2));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));


	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0, 0, 0), Vector3(2, 2, 2));
	playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));


	Ref<GameObject> fallBox10 = createGameObject("trampoline", Vector3(-659.7f, 30.2f-2.0f, -193.f), Vector3(4, 4, 4));
	fallBox10->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberColorAlpha.obj", GfxShader::DEFAULT));
	Ref<OrientedBoxCollider> r8 = std::make_shared<OrientedBoxCollider>(Vector3(8, 8, 8));
	r8->SetTriggerCollider(true);
	fallBox10->AddComponent(r8);
	fallBox10->AddComponent(std::make_shared<Bounce>(Vector3(0, 1, 0), 150));

	Ref<GameObject> fallBox11 = createGameObject("trampoline", Vector3(-594.9f, 100.9f, 379.5f), Vector3(12, 12, 12), Vector3(45, 45.0f/2.0f, 0));
	fallBox11->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberColorAlpha.obj", GfxShader::DEFAULT));
	Ref<OrientedBoxCollider> r9 = std::make_shared<OrientedBoxCollider>(Vector3(24, 24, 24));
	r9->SetTriggerCollider(true);
	fallBox11->AddComponent(r9);
	fallBox11->AddComponent(std::make_shared<Bounce>(Vector3(0.2f, 0.6f, 0.6f), 700));

	Ref<GameObject> fallBox12 = createGameObject("trampoline", Vector3(-382.6f, 289.6f, 1310.9f), Vector3(4, 4, 4), Vector3(0, 0, 0));
	fallBox12->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberFilip2.obj", GfxShader::DEFAULT));
	Ref<OrientedBoxCollider> r10 = std::make_shared<OrientedBoxCollider>(Vector3(8, 8, 8));
	r10->SetTriggerCollider(true);
	fallBox12->AddComponent(r10);
	fallBox12->AddComponent(std::make_shared<Bounce>(Vector3(0.2f, 0.6f, 0.6f), 100));

	Ref<GameObject> fallBox13 = createGameObject("trampoline", Vector3(-401.9f, 310.5f, 1087.4f), Vector3(4, 4, 4), Vector3(0, 0, 0));
	fallBox13->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberFilip2.obj", GfxShader::DEFAULT));
	Ref<OrientedBoxCollider> r11 = std::make_shared<OrientedBoxCollider>(Vector3(8, 8, 8));
	r11->SetTriggerCollider(true);
	fallBox13->AddComponent(r11);
	fallBox13->AddComponent(std::make_shared<Bounce>(Vector3(0.6f, 0.6f, 0.6f), 2000));

	for (int i = 0; i < moveGameObjects.size(); ++i)
	{
		moveGameObjects[i]->AddComponent(std::make_shared<Move>(Vector3(8.0f, 0.0f, 8.0f), Vector3(25.0f, 0.0f, 25.0f)));
	}

	Ref<GameObject> go1 = createGameObject("goal", Vector3(766.f, 301.2f, 2462.5f), Vector3(2.0f, 2.0f, 2.0f));
	go1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Goal/", "goal4.obj", GfxShader::DEFAULT));
	go1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(4, 4, 4)));
	Ref<Component> r4 = go1->AddComponent(std::make_shared<GoalLogic>());
	Ref<TableOfTimes> goalTableOfTimes = std::dynamic_pointer_cast<GoalLogic>(r4)->getTableOfTimes();
	goalTableOfTimes->addGrade("Great", 60.0f);
	goalTableOfTimes->addGrade("Good", 80.0f);
	goalTableOfTimes->addGrade("Okay", 120.0f);
	goalTableOfTimes->addGrade("Bad", 180.0f);

	/*Ref<GameObject> go2 = createGameObject("goal", Vector3(766.f, 301.2f, 2462.5f));
	go2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/FallWall/", "wallFall4.obj", GfxShader::DEFAULT));*/

	/*Ref<GameObject> fallBox7 = createGameObject("brickCube", Vector3(-887.5f, 97.f, 283.8f), Vector3(6.996823f, 3.626153f, 6.999946f));
	fallBox7->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxAutumn/", "200x2x200Box.obj", GfxShader::DEFAULT));
	fallBox7->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(6.996823f, 3.626153f, 6.999946f)));
	fallBox7->AddComponent(std::make_shared<Move>(Vector3(0, 0.0f, 4.0f), Vector3(0, 0.0f, 50.0f)));*/

	/*Ref<GameObject> brickCube = createGameObject("brickCube", Vector3(0, -50, 0.0f), Vector3(2*5, 1, 2*5));
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
	cube14->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*200, 2*100, 20)));*/

	m_graphics->loadSkybox("Textures/Skyboxes/space");
	m_graphics->setSkybox(1);	// 0 är tagen (default)
	m_graphics->setLightDirection({ 1.8, -1, -1 });
	m_mainCamera = player->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera();
}