#include "pch.h"
#include "Scene5.h"
#include "../SceneReader.h"
#include "../Player.h"
#include "../GameObject.h"
#include "../PlayerJumpTrigger.h"
#include "../Graphics/Graphics.h"
#include "../Rotate.h"

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
#include "../GoalLogic.h"
#include "../TableOfTimes.h"
#include "../Bounce.h"

using namespace DirectX::SimpleMath;

Scene5::Scene5(Graphics* graphics) : Scene(graphics)
{}


// V�r BANAN (Stage 1)
void Scene5::setUpScene()
{
	SceneReader sr;
	auto vec = sr.readSceneFromFile("Scenes/SceneJSONs/SpringScene.json");

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
		if (object.name == "Spring")
		{
			Ref<GameObject> spring = createGameObject("trampoline", object.position, Vector3(8, 8, 8));
			spring->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberColorAlpha.obj", GfxShader::DEFAULT));
			Ref<OrientedBoxCollider> trigger = std::make_shared<OrientedBoxCollider>(Vector3(16, 16, 16));
			trigger->SetTriggerCollider(true);
			spring->AddComponent(trigger);
			spring->AddComponent(std::make_shared<Bounce>(Vector3(0, 1, 0), 220));
			continue;
		}
		if (object.name == "Spring1")
		{
			Ref<GameObject> spring = createGameObject("trampoline", object.position, Vector3(8, 8, 8));
			spring->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberColorAlpha.obj", GfxShader::DEFAULT));
			Ref<OrientedBoxCollider> trigger = std::make_shared<OrientedBoxCollider>(Vector3(16, 16, 16));
			trigger->SetTriggerCollider(true);
			spring->AddComponent(trigger);
			spring->AddComponent(std::make_shared<Bounce>(Vector3(0.5, 1, -0.1), 220));
			continue;
		}
		if (object.name == "Spring2")
		{
			Ref<GameObject> spring = createGameObject("trampoline", object.position, Vector3(8, 8, 8), Vector3(12, 24, 65));
			spring->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Trampoline/", "bumberColorAlpha.obj", GfxShader::DEFAULT));
			Ref<OrientedBoxCollider> trigger = std::make_shared<OrientedBoxCollider>(Vector3(16, 16, 16));
			trigger->SetTriggerCollider(true);
			spring->AddComponent(trigger);
			spring->AddComponent(std::make_shared<Bounce>(Vector3(0, 2, 1), 110));
			continue;
		}
		if (object.name == "Goal")
		{
			Ref<GameObject> goal = createGameObject("goal", object.position, Vector3(8, 8, 8));
			goal->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Goal/", "goal4.obj", GfxShader::DEFAULT));
			Ref<OrientedBoxCollider> trigger = std::make_shared<OrientedBoxCollider>(Vector3(16, 16, 16));
			trigger->SetTriggerCollider(true);
			goal->AddComponent(trigger);
			Ref<Component> logic = goal->AddComponent(std::make_shared<GoalLogic>());
			Ref<TableOfTimes> goalTableOfTimes = std::dynamic_pointer_cast<GoalLogic>(logic)->getTableOfTimes();
			goal->AddComponent(std::make_shared<Rotate>(0, 30, 0));
			goalTableOfTimes->addGrade("Great", 60.0f);
			goalTableOfTimes->addGrade("Good", 80.0f);
			goalTableOfTimes->addGrade("Okay", 120.0f);
			goalTableOfTimes->addGrade("Bad", 180.0f);
			continue;
		}
		auto go = createGameObject(object.name, object.position, object.scale, object.rotation);
		auto model = m_graphics->getResourceDevice()->createModel(object.meshDirectory, object.meshFileName, GfxShader::DEFAULT);
		go->AddComponent(model);

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
		if (object.name == "Collider")
		{
			go->setIsVisible(false);
			continue;
		}
	}

	// Post setup, like cameras and logic
	auto player = createGameObject("player", Vector3(0, 25, 0), Vector3(0.5f, 0.5f, 0.5f));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	auto playerComp = std::make_shared<Player>();
	playerComp->setRespawn({0, 25, 0});
	player->AddComponent(playerComp);
	player->AddComponent(std::make_shared<CapsuleCollider>(0.5, 2));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));

	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0, 0, 0), Vector3(2, 2, 2));
	//playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));

	Ref<GameObject> spawnPartSys = createGameObject("hookParticleSystem", Vector3(0, 40,-12));
	Ref<ParticleSystemComponent> spawnPartSysComp = std::dynamic_pointer_cast<ParticleSystemComponent>(
		spawnPartSys->AddComponent(std::make_shared<ParticleSystemComponent>(10*5, 5.0f)));
	spawnPartSysComp->addEmitter(10, 60, 0.1f, Vector3(0.2f, 0.4f, 1));


	//Ref<GameObject> partSysGo = createGameObject("partSysGo", Vector3(0, 40, 0));
	//Ref<ParticleSystemComponent> partSys = std::dynamic_pointer_cast<ParticleSystemComponent>(
	//	partSysGo->AddComponent(std::make_shared<ParticleSystemComponent>(80, 4))
	//	);
	//partSys->addEmitter(20, 0, 200, Vector3(0,1,1));
	////	80 / 4 = 20

	/*Ref<GameObject> go1 = createGameObject("goal", Vector3(0.0f, 40.0f, 0.0f), Vector3(99.95461f / 20.0f, 99.99923f / 20.0f, 51.80217f / 20.0f), Vector3(87.966f, 0, 0));
	go1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Island/", "small_island.fbx", GfxShader::DEFAULT));
	go1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(10 * 2, 2 * 4, 10 * 2)));
	Ref<Component> r4 = go1->AddComponent(std::make_shared<GoalLogic>());
	Ref<TableOfTimes> goalTableOfTimes = std::dynamic_pointer_cast<GoalLogic>(r4)->getTableOfTimes();
	goalTableOfTimes->addGrade("Fantastic", 1.0f);
	goalTableOfTimes->addGrade("Bad", 2.0f);

	Ref<GameObject> go2 = createGameObject("island", Vector3(8.0f, 16.f, 0.0f), Vector3(5., 5., 5.), Vector3(87.966f, 0, 0));
	go2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Island/", "small_island.fbx", GfxShader::DEFAULT));
	go2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(0.02124478f, 0.02045011f, 0.01149386f)));

	Ref<GameObject> go3 = createGameObject("island", Vector3(200.0f, 20.0f, 0.0f), Vector3(934.4659f / 20.0f, 331.5767f / 20.0f, 159.5184f / 20.0f), Vector3(87.966f, 0, 0));
	go3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Island/", "long_island_spring_2.fbx", GfxShader::DEFAULT));
	go3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(0.02124478f, 0.02045011f, 0.01149386f)));

	Ref<GameObject> go4 = createGameObject("island", Vector3(-200.0f, 20.0f, 0.0f), Vector3(1401.552f / 20.0f, 439.1304f / 20.0f, 160.0005f / 20.0f), Vector3(87.966f, 0, 0));
	go4->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Island/", "long_island_spring_3.fbx", GfxShader::DEFAULT));
	go4->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(0.02124478f, 0.02045011f, 0.01149386f)));

	Ref<GameObject> go5 = createGameObject("island", Vector3(-200.0f, 20.0f, 200.0f), Vector3(100.f / 20.0f, 100.f / 20.0f, 100.f / 20.0f), Vector3(87.966f, 0, 0));
	go5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Island/", "vertical_island_fall.fbx", GfxShader::DEFAULT));
	go5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(0.02124478f, 0.02045011f, 0.01149386f)));*/

	//sound->play("Sounds/Explo4.wav"); //sorry
	//sound->playLoop("Sounds/Spring.wav");
	m_graphics->setSkybox(1);
	m_graphics->setLightDirection({1.8f, -1, 1});
	m_graphics->setSkyboxRotationAroundY(-65.f, 1);

	m_mainCamera = player->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera();

	// Clickable sprite test
	//auto clickableSprite = m_graphics->getResourceDevice()->createSpriteTexture("Textures/Sprites/Textures/Temp.png", 1100, 600, 0.3f, 0.3f, 0.f, 0.f,
	//	[]() { OutputDebugStringW(L"This sprite was clicked! o_o \n"); });		// Pass function to call in last param. Refer to (https://en.cppreference.com/w/cpp/utility/functional/function)
	//m_graphics->addToSpriteBatch(clickableSprite);

}
