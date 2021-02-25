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

using namespace DirectX::SimpleMath;

Scene2::Scene2(Graphics* graphics) : Scene(graphics)
{

}

Scene2::~Scene2()
{

}

void Scene2::setUpScene()
{
	Ref<GameObject> player = createGameObject("player", Vector3(0, 10, 0), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	player->AddComponent(std::make_shared<CapsuleCollider>(1, 4));
	//player->AddComponent(std::make_shared<SphereCollider>(1));
	


	Ref<GameObject> ground = createGameObject("ground", Vector3(0.0f, 0.0f, 0.0f), Vector3(1, 1, 1), Vector3(0,0,0));
	ground->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	ground->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(200, 2, 200)));

	Ref<GameObject> cube = createGameObject("brickCube", Vector3(0, 5.0, 0.0f), Vector3(1.0f / 50.0f, 1, 1.0f / 50.0f));
	cube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(4, 2, 4)));

	/*Ref<GameObject> box = createGameObject("box", Vector3(5.0f, 4.0f, 0.0f), Vector3(1, 1, 1), Vector3(0, 0, 0));
	box->AddComponent(m_graphics->getResourceDevice()->createModel("Models/cube/", "Cube.obj", GfxShader::DEFAULT));
	box->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(2, 2, 2)));*/

	Ref<GameObject> go1 = createGameObject("object1", Vector3(0, 0, 20.0f));
	go1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));
}