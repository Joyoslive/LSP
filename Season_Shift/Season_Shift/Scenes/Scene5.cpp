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

using namespace DirectX::SimpleMath;

Scene5::Scene5(Graphics* graphics) : Scene(graphics)
{}

void Scene5::setUpScene()
{
	SceneReader sr;
	auto vec = sr.readSceneFromFile("Scenes/SceneJSONs/scene.json");

	for (auto& object : vec)
	{
		if (object.name == "Checkpoint")
		{
			Ref<GameObject> cp1 = createGameObject("checkpoint", object.position, Vector3(2, 2, 2));
			cp1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/checkpoint/", "checkpoint.obj", GfxShader::DEFAULT));
			Ref<OrientedBoxCollider> r1 = std::make_shared<OrientedBoxCollider>(Vector3(10, 5, 10));
			r1->SetTriggerCollider(true);
			cp1->AddComponent(r1);
			cp1->AddComponent(std::make_shared<Rotate>(0, 20, 0));
			continue;
		}
		auto go = createGameObject(object.name, object.position, object.scale, object.rotation);
		auto model = m_graphics->getResourceDevice()->createModel(object.meshDirectory, object.meshFileName, GfxShader::DEFAULT);
		go->AddComponent(model);

		auto temp = object.boxCollider.x + object.boxCollider.y + object.boxCollider.z;
		if (!(-0.0001 < temp && temp < 0.0001))
		{
			auto collider = std::make_shared<OrientedBoxCollider>(object.boxCollider);
			go->AddComponent(collider);
		}
	}

	// Post setup, like cameras and logic
	auto player = createGameObject("player", Vector3(0, 25, 0));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	auto playerComp = std::make_shared<Player>();
	playerComp->setRespawn({0, 25, 0});
	player->AddComponent(playerComp);
	player->AddComponent(std::make_shared<CapsuleCollider>(1, 4));
	std::vector<std::string> v1;
	v1.push_back("Sounds/whoosh.wav");
	v1.push_back("Sounds/boing2.wav");
	v1.push_back("Sounds/hook.wav");
	v1.push_back("Sounds/dehook.wav");
	v1.push_back("Sounds/jump1a.wav");
	v1.push_back("Sounds/jump2a.wav");
	v1.push_back("Sounds/jump3a.wav");
	player->AddComponent(std::make_shared<Sound>(v1));

	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0, 0, 0), Vector3(2, 2, 2));
	playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));

	m_graphics->loadSkybox("Textures/Skyboxes/space");
	m_graphics->setSkybox(1);
	//Ref<GameObject> partSysGo = createGameObject("partSysGo", Vector3(0, 40, 0));
	//Ref<ParticleSystemComponent> partSys = std::dynamic_pointer_cast<ParticleSystemComponent>(
	//	partSysGo->AddComponent(std::make_shared<ParticleSystemComponent>(80, 4))
	//	);
	//partSys->addEmitter(20, 0, 200, Vector3(0,1,1));
	////	80 / 4 = 20

	auto audioObject = createGameObject("audio", Vector3(-4, 8, 0));
	std::vector<std::string> v;
	v.push_back("Sounds/Explo4.wav");
	v.push_back("Sounds/Explo1.wav");
	v.push_back("Sounds/NightAmbienceSimple_02.wav");
	Ref<Sound> sound = std::dynamic_pointer_cast<Sound>(
		audioObject->AddComponent(std::make_shared<Sound>(v))
		);
	//sound->play("Sounds/Explo4.wav"); //sorry

	m_graphics->setLightDirection({1.8, -1, -1});
}
