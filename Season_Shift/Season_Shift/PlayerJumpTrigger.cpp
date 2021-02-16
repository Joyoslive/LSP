#include "PlayerJumpTrigger.h"

PlayerJumpTrigger::PlayerJumpTrigger(Ref<GameObject> gameObject)
{
	m_playerTransform = gameObject->getComponentType<Transform>(Component::ComponentEnum::TRANSFORM);
	m_player = gameObject->getComponentType<Player>(Component::ComponentEnum::LOGIC);
}

PlayerJumpTrigger::~PlayerJumpTrigger()
{

}

void PlayerJumpTrigger::start()
{
	m_myTransform = m_gameObject->getComponentType<Transform>(Component::ComponentEnum::TRANSFORM);
}

void PlayerJumpTrigger::update()
{
	m_myTransform->setPosition(m_playerTransform->getPosition() + DirectX::SimpleMath::Vector3(0, -1, 0));
	m_ground = false;
}

void PlayerJumpTrigger::onCollision(Ref<Collider> collider)
{
	m_ground = true;
	m_player->setWaitForJump();
}