#include "Logic.h"


Logic::Logic()
{
	m_componentType = ComponentEnum::LOGIC;
}

Logic::~Logic()
{

}

void Logic::start()
{

}


void Logic::onCollision(Ref<Collider> collider)
{

}

void Test::start()
{

}

void Test::update()
{
	DirectX::SimpleMath::Vector3 pos = m_transform->getPosition();
	pos.y += 0.01f; //pos.z += 0.01f;
	DirectX::SimpleMath::Vector3 rotation = m_transform->getRotation();
	rotation.x += 0.25f;
	rotation.y += 1.0f;
	DirectX::SimpleMath::Vector3 scale = m_transform->getScale();
	scale.x *= 1.001f;//0.999f;
	scale.y *= 1.001f;//0.999f;
	scale.z *= 1.001f;//0.999f;
	//rotation.z += 0.5f;
	m_transform->setPosition(pos);
	m_transform->setRotation(rotation);
	m_transform->setScale(scale);
	//m_gameObject->getScene()->createGameObject();
}

void Test::onCollision(Ref<Collider> collider)
{

}