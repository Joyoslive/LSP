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

void Logic::update()
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
	++pos.x; --pos.y; pos.z = pos.x - pos.y;
	m_transform->setPosition(pos);
}

void Test::onCollision(Ref<Collider> collider)
{

}