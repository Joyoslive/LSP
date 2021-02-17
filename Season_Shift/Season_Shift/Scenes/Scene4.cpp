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

	Ref<GameObject> playerJumpTrigger = createGameObject("playerJumpTrigger", Vector3(0, 0, 0), Vector3(2,2,2));
	playerJumpTrigger->AddComponent(m_graphics->getResourceDevice()->createModel("Models/sphere/", "sphere.obj", GfxShader::DEFAULT));
	playerJumpTrigger->AddComponent(std::make_shared<SphereCollider>(2));
	playerJumpTrigger->AddComponent(std::make_shared<PlayerJumpTrigger>(player));

	Ref<GameObject> brickCube = createGameObject("brickCube", Vector3(0, -50, 0.0f), Vector3(2 * 50, 1, 2 * 50));
	brickCube->AddComponent(m_graphics->getResourceDevice()->createModel("Models/ground/", "200x2x200Box.obj", GfxShader::DEFAULT));
	brickCube->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(400 * 50, 2, 400 * 50)));

	Ref<GameObject> start = createGameObject("brickCube", Vector3(0, 5.0f, 0.0f), Vector3(1.0f / 10.0f, 1, 1.0f / 10.0f));
	start->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	start->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20)));

	//Spring
	Ref<GameObject> springBox1 = createGameObject("brickCube", Vector3(10.0f + (20 * 5) / 2, 5.0f, 0.0f), Vector3((1.0f / 10.0f)*5, 1, 1.0f / 10.0f));
	springBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*5, 2, 20)));

	Ref<GameObject> springBox2 = createGameObject("brickCube", Vector3(10.0f + (20 * 35) / 2, 15.0f, 0.0f), Vector3((1.0f / 10.0f) * 5.3, 1*5.5*3, (1.0f / 10.0f)*1.5));
	springBox2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 5.3, 2*5.5*3, 20*1.5)));

	Ref<GameObject> springBox3 = createGameObject("brickCube", Vector3(10.0f + (20 * 30) / 2, 5.0f, 0.0f), Vector3((1.0f / 10.0f) * 2, 1 * 5.5 * 3, (1.0f / 10.0f) * 1.5));
	springBox3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 5.5 * 3, 20 * 1.5)));

	Ref<GameObject> springBox5 = createGameObject("brickCube", Vector3(10.0f + (20 * 60) / 2, 45.0f, 50.0f), Vector3((1.0f / 10.0f) * 5, 1 * 4.5 * 3, (1.0f / 10.0f) * 1.5));
	springBox5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 5, 2 * 4.5 * 3, 20 * 1.5)));

	Ref<GameObject> springBox4 = createGameObject("brickCube", Vector3(10.0f + (20 * 90) / 2, 70.0f, 0.0f), Vector3((1.0f / 10.0f) * 7.5, 1 * 7.5 * 3, (1.0f / 10.0f) * 1.2));
	springBox4->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox4->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 7.5, 2 * 7.5 * 3, 20 * 1.2)));

	Ref<GameObject> springBox6 = createGameObject("brickCube", Vector3(10.0f + (20 * 125) / 2, 45.0f, 70.0f), Vector3((1.0f / 10.0f) * 4, 1 * 3.5 * 3, (1.0f / 10.0f) * 1.25));
	springBox6->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox6->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 4, 2 * 3.5 * 3, 20 * 1.25)));

	Ref<GameObject> springBox7 = createGameObject("brickCube", Vector3(10.0f + (20 * 155) / 2, 25.0f, 90.0f), Vector3((1.0f / 10.0f) * 3, 1 * 2.5 * 3, (1.0f / 10.0f) * 1));
	springBox7->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSpring/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBox7->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 3, 2 * 2.5 * 3, 20 * 1)));

	Ref<GameObject> springBoxTower = createGameObject("brickCube", Vector3(10.0f + (20 * 195) / 2, 50.0f, 90.0f), Vector3((1.0f / 10.0f) * 2, 1 * 200, (1.0f / 10.0f) * 2));
	springBoxTower->AddComponent(m_graphics->getResourceDevice()->createModel("Models/tower/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTower->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 200, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform1 = createGameObject("brickCube", Vector3(10.0f + (20 * 181) / 2, 50.0f, 90.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform2 = createGameObject("brickCube", Vector3(10.0f + (20 * 195) / 2, 70.0f, 150.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform3 = createGameObject("brickCube", Vector3(10.0f + (20 * 195) / 2, 120.0f, 30.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform4 = createGameObject("brickCube", Vector3(10.0f + (20 * 209) / 2, 110.0f, 90.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform4->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform4->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform5 = createGameObject("brickCube", Vector3(10.0f + (20 * 181) / 2, 130.0f, 90.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform6 = createGameObject("brickCube", Vector3(10.0f + (20 * 195) / 2, 150.0f, 150.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform6->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform6->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform7 = createGameObject("brickCube", Vector3(10.0f + (20 * 195) / 2, 210.0f, 30.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform7->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform7->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform8 = createGameObject("brickCube", Vector3(10.0f + (20 * 209) / 2, 190.0f, 90.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform8->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform8->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));

	Ref<GameObject> springBoxTowerPlatform9 = createGameObject("brickCube", Vector3(10.0f + (20 * 181) / 2, 230.0f, 90.0f), Vector3((1.0f / 10.0f) * 2, 1 * 2, (1.0f / 10.0f) * 2));
	springBoxTowerPlatform9->AddComponent(m_graphics->getResourceDevice()->createModel("Models/box/", "200x2x200Box.obj", GfxShader::DEFAULT));
	springBoxTowerPlatform9->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 2, 2 * 2, 20 * 2)));


	Ref<GameObject> go1 = createGameObject("goal", Vector3(10.0f + (20 * 195) / 2, 280.0f, 90.0f));
	go1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));
	go1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(10 * 2, 2 * 2, 10 * 2)));

	//Winter
	Ref<GameObject> winterBox1 = createGameObject("brickCube", Vector3(0.0, 5.0f, 10.0f+(20*2)/2), Vector3((1.0f / 10.0f), 1, (1.0f / 10.0f)*2));
	winterBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxWinter/", "200x2x200Box.obj", GfxShader::DEFAULT));
	winterBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20*2)));


	//Summer
	Ref<GameObject> summerBox1 = createGameObject("brickCube", Vector3(0.0, 5.0f, -(10.0f + (20 * 2)/2)), Vector3((1.0f / 10.0f), 1, (1.0f / 10.0f) * 2));
	summerBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20, 2, 20 * 2)));

	Ref<GameObject> summerBox2 = createGameObject("brickCube", Vector3(0.0, 8.0f, -(10.0f + (20 * 12) / 2)), Vector3((1.0f / 10.0f)*0.95, 1, (1.0f / 10.0f) * 1.8));
	summerBox2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.95, 2, 20 * 1.8)));

	Ref<GameObject> summerBox3 = createGameObject("brickCube", Vector3(0.0, 11.0f, -(10.0f + (20 * 22) / 2)), Vector3((1.0f / 10.0f) * 0.90, 1, (1.0f / 10.0f) * 1.6));
	summerBox3->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox3->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.90, 2, 20 * 1.6)));

	Ref<GameObject> summerBox4 = createGameObject("brickCube", Vector3(0.0, 14.0f, -(10.0f + (20 * 32) / 2)), Vector3((1.0f / 10.0f) * 0.85, 1, (1.0f / 10.0f) * 1.4));
	summerBox4->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox4->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.85, 2, 20 * 1.4)));

	Ref<GameObject> summerBox5 = createGameObject("brickCube", Vector3(0.0, 17.0f, -(10.0f + (20 * 42) / 2)), Vector3((1.0f / 10.0f) * 0.80, 1, (1.0f / 10.0f) * 1.2));
	summerBox5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox5->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.80, 2, 20 * 1.2)));

	Ref<GameObject> summerBox6 = createGameObject("brickCube", Vector3(0.0, 20.0f, -(10.0f + (20 * 52) / 2)), Vector3((1.0f / 10.0f) * 0.75, 1, (1.0f / 10.0f) * 1.0));
	summerBox6->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox6->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.75, 2, 20 * 1.0)));

	Ref<GameObject> summerBox7 = createGameObject("brickCube", Vector3(0.0, 23.0f, -(10.0f + (20 * 62) / 2)), Vector3((1.0f / 10.0f) * 0.70, 1, (1.0f / 10.0f) * 0.8));
	summerBox7->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox7->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.70, 2, 20 * 0.8)));

	Ref<GameObject> summerBox8 = createGameObject("brickCube", Vector3(0.0, 26.0f, -(10.0f + (20 * 72) / 2)), Vector3((1.0f / 10.0f) * 0.65, 1, (1.0f / 10.0f) * 0.6));
	summerBox8->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox8->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.65, 2, 20 * 0.6)));

	Ref<GameObject> summerBox9 = createGameObject("brickCube", Vector3(0.0, 29.0f, -(10.0f + (20 * 82) / 2)), Vector3((1.0f / 10.0f) * 0.60, 1, (1.0f / 10.0f) * 0.4));
	summerBox9->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox9->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.60, 2, 20 * 0.4)));

	Ref<GameObject> summerBox10 = createGameObject("brickCube", Vector3(0.0, 32.0f, -(10.0f + (20 * 92) / 2)), Vector3((1.0f / 10.0f) * 0.55, 1, (1.0f / 10.0f) * 0.2));
	summerBox10->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxSummer/", "200x2x200Box.obj", GfxShader::DEFAULT));
	summerBox10->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20 * 0.55, 2, 20 * 0.2)));

	Ref<GameObject> go2 = createGameObject("goal", Vector3(0.0, 32.0f, -(10.0f + (20 * 102) / 2)));
	go2->AddComponent(m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));
	go2->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(10 * 2, 2 * 2, 10 * 2)));

	//Fall
	Ref<GameObject> fallBox1 = createGameObject("brickCube", Vector3(-(10.0f + (20 * 2) / 2), 5.0f,0), Vector3((1.0f / 10.0f)*2, 1, (1.0f / 10.0f)));
	fallBox1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/boxAutumn/", "200x2x200Box.obj", GfxShader::DEFAULT));
	fallBox1->AddComponent(std::make_shared<OrientedBoxCollider>(Vector3(20*2, 2, 20)));

}