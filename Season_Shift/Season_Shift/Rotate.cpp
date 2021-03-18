#include "pch.h"
#include "Rotate.h"
#include "GameObject.h"
#include "Transform.h"
#include "FrameTimer.h"

Rotate::Rotate()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	rot = { 0, 0, 0 };
}

Rotate::Rotate(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
	rot = { 0, 0, 0 };
}

Rotate::~Rotate()
{
}

void Rotate::update()
{
	float frameTime = static_cast<float>(FrameTimer::dt());
	rot.x += m_x * frameTime;
	rot.y += m_y * frameTime;
	rot.z += m_z * frameTime;
	m_gameObject->getTransform()->setRotation(rot);
}
