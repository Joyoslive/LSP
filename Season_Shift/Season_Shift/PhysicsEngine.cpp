#include "PhysicsEngine.h"

#include "SphereCollider.h"

using namespace DirectX;

PhysicsEngine::PhysicsEngine()
{
	/*Ref<GameObject> go1 = std::make_shared<GameObject>();
	go1->AddComponent(std::make_shared<Transform>());
	go1->getTransform()->setPosition(DirectX::XMFLOAT3(0, 0, 0));
	go1->AddComponent(std::make_shared<SphereCollider>(go1->getTransform()->getPosition(), 2.0f));

	Ref<GameObject> go2 = std::make_shared<GameObject>();
	go2->AddComponent(std::make_shared<Transform>());
	go2->getTransform()->setPosition(DirectX::XMFLOAT3(3, 0, 0));
	go2->AddComponent(std::make_shared<SphereCollider>(go2->getTransform()->getPosition(), 2.0f));

	m_tempScene.push_back(go1);
	m_tempScene.push_back(go2);

	int hej = 0;

	Ref<Collider> co1 = go1->getComponentType<Collider>(Component::ComponentEnum::SPHERE_COLLIDER);
	Ref<Collider> co2 = go2->getComponentType<Collider>(Component::ComponentEnum::COLLIDER);

	if (co1 == nullptr || co2 == nullptr)
	{
		hej = -1;
	}
	else if (co1->collide(co2))
	{
		hej = 10;
	}
	hej = 1;*/
}
PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::addScene(Ref<Scene> scene)
{
	m_scene = scene;
}

std::vector<Ref<Collider>> PhysicsEngine::checkCollide(Ref<Collider> collider) //notera att denna funktion inte tar hänsyn till om objekten redan har kolliderat
{
	std::vector<Ref<Collider>> colliderVec;
	for (auto& go : m_scene->getSceneGameObjects())
	{
		Ref<Collider> other = go->getComponentType<Collider>(Component::ComponentEnum::COLLIDER);
		if (other != nullptr)
		{
			if (collider->collide(other))
			{
				Ref<Logic> logic = go->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
				if (logic != nullptr)
				{
					logic->onCollision(collider);
				}
				logic = collider->getGameObject()->getComponentType<Logic>(Component::ComponentEnum::LOGIC);
				if (logic != nullptr)
				{
					logic->onCollision(other);
				}
				colliderVec.push_back(other);
			}
		}
	}
	return colliderVec;
}
