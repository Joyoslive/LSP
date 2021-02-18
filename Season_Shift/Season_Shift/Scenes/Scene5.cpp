#include "Scene5.h"
#include "../SceneReader.h"
#include "../Player.h"
#include "../PlayerJumpTrigger.h"
#include "../Graphics/Graphics.h"

using namespace DirectX::SimpleMath;

Scene5::Scene5(Graphics* graphics) : Scene(graphics)
{}

void Scene5::setUpScene()
{
	SceneReader sr;
	auto vec = sr.readSceneFromFile("Scenes/SceneJSONs/scene.json");

	for (auto& object : vec)
	{
		auto go = createGameObject(object.name, object.position, object.scale/2, object.rotation);
		auto model = m_graphics->getResourceDevice()->createModel("Models/Cube/", "Cube.obj", GfxShader::DEFAULT);
		auto collider = std::make_shared<OrientedBoxCollider>(object.scale);
		go->AddComponent(model);
		go->AddComponent(collider);
	}
	
	// Post setup, like cameras and logic
	auto player = createGameObject("player", Vector3(0, 10, 0));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(std::make_shared<CapsuleCollider>(1, 4));

	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0, 0, 0), Vector3(2, 2, 2));
	playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));
}
