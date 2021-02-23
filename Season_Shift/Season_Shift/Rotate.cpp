#include "Rotate.h"

Rotate::Rotate() 
{
	rot = { 0, 0, 0 };
}

Rotate::~Rotate()
{
}

void Rotate::update() 
{
	rot.y += 0.15;
	rot.z += 0.05;
	m_gameObject->getTransform()->setRotation( rot);
}
