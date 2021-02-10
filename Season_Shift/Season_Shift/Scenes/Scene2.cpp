#include "Scene2.h"
#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"

using namespace DirectX::SimpleMath;

Scene2::Scene2(Graphics* graphics) : Scene(graphics)
{

}

Scene2::~Scene2()
{

}

void Scene2::setUpScene()
{
	Ref<GameObject> player = createGameObject("player", Vector3(0, 10, 0), Vector3(0.2f, 0.2f, 0.2f));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));
	player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<Player>());
	//player->AddComponent(std::make_shared<CapsuleCollider>(1, 4));
	player->AddComponent(std::make_shared<SphereCollider>(1));
	


	Ref<GameObject> brickCube = createGameObject("brickCube", Vector3(0, -50, 0.0f), Vector3(1, 1, 1));
	brickCube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/cube/", "Cube.obj", GfxShader::DEFAULT));
	brickCube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(100, 2, 100)));

	Ref<GameObject> go1 = createGameObject("object1", Vector3(0, 0, 20.0f));
	go1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));
}