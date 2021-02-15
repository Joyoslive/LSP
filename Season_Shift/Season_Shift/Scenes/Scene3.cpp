#include "Scene3.h"
#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"

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
	player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(std::make_shared<SphereCollider>(1));
	


	Ref<GameObject> brickCube = createGameObject("brickCube", Vector3(0, 0, 0.0f), Vector3(2*5, 1, 2*5));
	brickCube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	brickCube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(400*5, 2, 400*5)));

	Ref<GameObject> cube = createGameObject("brickCube", Vector3(0, 5.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube2 = createGameObject("brickCube", Vector3(30.0f, 10.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	Ref<GameObject> cube3 = createGameObject("brickCube", Vector3(80.0f, 15.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	cube3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	cube3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));
}