#include "Scene4.h"
#include "../Graphics/Graphics.h"
#include "../Player.h"
#include "../CameraComponent.h"
#include "../PlayerJumpTrigger.h"

using namespace DirectX::SimpleMath;

Scene4::Scene4(Graphics* graphics) : Scene(graphics)
{

}

Scene4::~Scene4()
{

}

void Scene4::setUpScene()
{
	Ref<GameObject> player = createGameObject("player", Vector3(0, 10, 0), Vector3(1.0f, 1.0f, 1.0f));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	//player->AddComponent(m_graphics->getResourceDevice()->createModel("Models/capsule/", "capsule.obj", GfxShader::DEFAULT));
	player->AddComponent(std::make_shared<CameraComponent>());
	player->AddComponent(std::make_shared<RigidBody>());
	player->AddComponent(std::make_shared<Player>());
	player->AddComponent(std::make_shared<CapsuleCollider>(1, 4));
	//player->AddComponent(std::make_shared<SphereCollider>(1));

	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger");
	playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(1));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));

	Ref<GameObject> brickCube = createGameObject("brickCube", Vector3(0, -50, 0.0f), Vector3(2 * 5, 1, 2 * 5));
	brickCube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/ground/", "200x2x200Box.obj", GfxShader::DEFAULT));
	brickCube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(400 * 5, 2, 400 * 5)));

	Ref<GameObject> start = createGameObject("brickCube", Vector3(0, 5.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	start->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	start->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	//Spring
	Ref<GameObject> springBox1 = createGameObject("brickCube", Vector3(10.0f + (20 * 5) / 2, 5.0f, 0.0f), Vector3((1.0f / 10.0f)*5, 1, 1.0f / 10.0f));
	springBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*5, 2, 20)));

	Ref<GameObject> springBox2 = createGameObject("brickCube", Vector3(10.0f + (20 * 35) / 2, 15.0f, 0.0f), Vector3((1.0f / 10.0f) * 5.3, 1*5.5, (1.0f / 10.0f)*1.5));
	springBox2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 5.3, 2*5.5, 20*1.5)));

	Ref<GameObject> springBox3 = createGameObject("brickCube", Vector3(10.0f + (20 * 30) / 2, 5.0f, 0.0f), Vector3((1.0f / 10.0f) * 2, 1 * 5.5, (1.0f / 10.0f) * 1.5));
	springBox3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 5.5, 20 * 1.5)));

	Ref<GameObject> springBox5 = createGameObject("brickCube", Vector3(10.0f + (20 * 60) / 2, 45.0f, 50.0f), Vector3((1.0f / 10.0f) * 5, 1 * 4.5, (1.0f / 10.0f) * 1.5));
	springBox5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 5, 2 * 4.5, 20 * 1.5)));

	Ref<GameObject> springBox4 = createGameObject("brickCube", Vector3(10.0f + (20 * 90) / 2, 70.0f, 0.0f), Vector3((1.0f / 10.0f) * 7.5, 1 * 7.5, (1.0f / 10.0f) * 1.2));
	springBox4->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox4->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 7.5, 2 * 7.5, 20 * 1.2)));

	Ref<GameObject> springBox6 = createGameObject("brickCube", Vector3(10.0f + (20 * 60) / 2, 45.0f, 70.0f), Vector3((1.0f / 10.0f) * 5, 1 * 4.5, (1.0f / 10.0f) * 1.5));
	springBox5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 5, 2 * 4.5, 20 * 1.5)));

	Ref<GameObject> springBox7 = createGameObject("brickCube", Vector3(10.0f + (20 * 60) / 2, 45.0f, 50.0f), Vector3((1.0f / 10.0f) * 5, 1 * 4.5, (1.0f / 10.0f) * 1.5));
	springBox5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 5, 2 * 4.5, 20 * 1.5)));


	//Winter
	Ref<GameObject> winterBox1 = createGameObject("brickCube", Vector3(0.0, 5.0f, 10.0f+(20*2)/2), Vector3((1.0f / 10.0f), 1, (1.0f / 10.0f)*2));
	winterBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxWinter/", "200x2x200Box.obj", GfxShader::DEFAULT));
	winterBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20*2)));


	//Summer
	Ref<GameObject> summerBox1 = createGameObject("brickCube", Vector3(0.0, 5.0f, -(10.0f + (20 * 2)/2)), Vector3((1.0f / 10.0f), 1, (1.0f / 10.0f) * 2));
	summerBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20 * 2)));

	//Fall
	Ref<GameObject> fallBox1 = createGameObject("brickCube", Vector3(-(10.0f + (20 * 2) / 2), 5.0f,0), Vector3((1.0f / 10.0f)*2, 1, (1.0f / 10.0f)));
	fallBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxAutumn/", "200x2x200Box.obj", GfxShader::DEFAULT));
	fallBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*2, 2, 20)));

}