#include "pch.h"
#include "Move.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "OrientedBoxCollider.h"
#include "CapsuleCollider.h"
#include "Transform.h"
#include "FrameTimer.h"


Move::Move(DirectX::SimpleMath::Vector3 speed, DirectX::SimpleMath::Vector3 range)
{
	m_speed = speed;
	m_range = range;
	m_direction = 0;
}

Move::~Move()
{
}

void Move::start()
{
	m_move = m_gameObject->getTransform()->getPosition();
	m_original = m_move;
	m_direction = 1;
}

void Move::update()
{
	if (m_move.x > m_original.x + m_range.x || m_move.y > m_original.y + m_range.y || m_move.z > m_original.z + m_range.z)
	{
		m_direction = -1;
	}
	else if (m_move.x < m_original.x - m_range.x || m_move.y < m_original.y - m_range.y || m_move.z < m_original.z - m_range.z)
	{
		m_direction = 1;
	}
	m_move += m_speed * m_direction * static_cast<float>(FrameTimer::dt());
	m_gameObject->getTransform()->setPosition(m_move);
}

DirectX::SimpleMath::Vector3 Move::getSpeed() {
	return m_speed * m_direction;
}