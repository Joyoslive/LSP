#include "pch.h"
#include "Scene2.h"
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

Scene2::Scene2(Graphics* graphics) : Scene(graphics)
{

}

Scene2::~Scene2()
{

}

void Scene2::setUpScene()
{
	// Post setup, like cameras and logic
	auto player = createGameObject("player", Vector3(0, 25, 0), Vector3(1,1,1), Vector3(0, 0, 30));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	auto playerComp = std::make_shared<Player>();
	playerComp->setRespawn({ 0, 25, 0 });
	player->AddComponent(playerComp);
	player->AddComponent(std::make_shared<CapsuleCollider>(1.0, 4));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));


	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0, 0, 0), Vector3(2, 2, 2));
	//playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));

	m_graphics->setLightDirection({0.0, -0.7, 1.0});
	m_mainCamera = player->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA)->getCamera();


	//Ref<GameObject> ground = createGameObject("ground", Vector3(0.0f, -1.0f, 0.0f), Vector3(1, 1, 1), Vector3(0,0,0));
	//ground->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	//ground->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(200, 2, 200)));

	//for (int i = 1; i < 10; i++)
	//{
	//	Vector3 c;
	//	c = Vector3(2.0f, 0.8f * 0.5f * i, 2.0f);
	//	Ref<GameObject> cube1 = createGameObject("brickCube", Vector3(i * 2* c.x, c.y, 0.0f), c);
	//	cube1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/cube/", "Cube.obj", GfxShader::DEFAULT));
	//	cube1->AddComponent(std::make_shared<OrientedBoxCollider>(2 * c));
	//}

	m_graphics->loadSkybox("Textures/Skyboxes/BRIGHTBOX");
	m_graphics->setSkybox(1);
}