#include "pch.h"
#include "PlayerJumpTrigger.h"
#include "GameObject.h"
#include "Transform.h"

PlayerJumpTrigger::PlayerJumpTrigger(Ref<GameObject> gameObject)
{
	m_playerTransform = gameObject->getComponentType<Transform>(Component::ComponentEnum::TRANSFORM);
	m_player = gameObject->getComponentType<Player>(Component::ComponentEnum::LOGIC);
	m_oldCollider = nullptr;
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
	m_myTransform->setPosition(m_playerTransform->getPosition() + DirectX::SimpleMath::Vector3(0, -3, 0));
}

void PlayerJumpTrigger::onCollision(Ref<Collider> collider)
{
	if (m_oldCollider == collider)
		m_player->setWaitForJump();
	m_oldCollider = collider;
}