#include "Rotate.h"
#include "GameObject.h"
#include "Transform.h"

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
	rot.x += m_x;
	rot.y += m_y;
	rot.z += m_z;
	m_gameObject->getTransform()->setRotation( rot);
}
