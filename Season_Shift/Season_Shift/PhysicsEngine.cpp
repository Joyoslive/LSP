#include "PhysicsEngine.h"

#include "SphereCollider.h"

using namespace DirectX;

PhysicsEngine::PhysicsEngine()
{
	Ref<GameObject> go1 = std::make_shared<GameObject>();
	go1->AddComponent(std::make_shared<Transform>());
	go1->getTransform()->setPosition(DirectX::XMFLOAT3(0, 0, 0));
	go1->AddComponent(std::make_shared<SphereCollider>(go1->getTransform()->getPosition(), 2.0f));

	Ref<GameObject> go2 = std::make_shared<GameObject>();
	go2->AddComponent(std::make_shared<Transform>());
	go2->getTransform()->setPosition(DirectX::XMFLOAT3(3, 0, 0));
	go2->AddComponent(std::make_shared<SphereCollider>(go2->getTransform()->getPosition(), 2.0f));

	m_tempScene.push_back(go1);
	m_tempScene.push_back(go2);

	int hej = 4;
	if (go1->getComponentType<SphereCollider>()->collide(go2->getComponentType<SphereCollider>()))
	{
		hej = 10;
	}
	hej = 1;
}
PhysicsEngine::~PhysicsEngine()
{
}

Ref<Collider> PhysicsEngine::Collide(Ref<Collider> collider)
{
	
	return Ref<Collider>();
}
