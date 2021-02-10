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
	Ref<GameObject> go5 = createGameObject("player", Vector3(0, 0, 0), Vector3(1.f, 1.f, 1.f));
	go5->AddComponent(m_graphics->getResourceDevice()->createModel("Models/Cube/", "Cube.obj", GfxShader::DEFAULT));
	go5->AddComponent(std::make_shared<CameraComponent>());

	Ref<GameObject> go1 = createGameObject("object", Vector3(0, 0, 20.0f));
	go1->AddComponent(m_graphics->getResourceDevice()->createModel("Models/nanosuit/", "nanosuit.obj", GfxShader::DEFAULT));
}