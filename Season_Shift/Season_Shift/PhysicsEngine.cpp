#include "PhysicsEngine.h"

using namespace DirectX;

PhysicsEngine::PhysicsEngine()
{
	/*Ref<GameObject> go1 = std::make_shared<GameObject>();
	go1->AddComponent(std::make_shared<Transform>());
	go1->getTransform()->setPosition(DirectX::XMFLOAT3(0, 0, 0));
	go1->AddComponent(std::make_shared<Collider>(go1->getTransform()->getPosition()));

	Ref<GameObject> go2 = std::make_shared<GameObject>();
	go2->AddComponent(std::make_shared<Transform>());
	go2->getTransform()->setPosition(DirectX::XMFLOAT3(3, 0, 0));
	go2->AddComponent(std::make_shared<Collider>(go2->getTransform()->getPosition()));

	m_tempScene.push_back(go1);
	m_tempScene.push_back(go2);*/
}

PhysicsEngine::~PhysicsEngine()
{
}

Ref<Collider> PhysicsEngine::Collide(Ref<Collider> collider)
{
	
	return Ref<Collider>();
}
