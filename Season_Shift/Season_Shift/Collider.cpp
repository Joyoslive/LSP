#include "pch.h"
#include "Collider.h"
#include "Transform.h"

using namespace DirectX;

Collider::Collider()
{
	m_componentType = ComponentEnum::COLLIDER;
	m_trigger = false;
}

Collider::~Collider()
{
}

void Collider::SetTriggerCollider(bool trigger)
{
	m_trigger = trigger;
}

bool Collider::GetTrigger() const
{
	return m_trigger;
}
