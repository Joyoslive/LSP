#pragma once
#pragma once
#include "Logic.h"

#include <d3d11.h>
#include <SimpleMath.h>

class Bounce : public Logic
{
private:
	DirectX::SimpleMath::Vector3 m_angle;
	float m_power;
public:
	Bounce();
	Bounce(DirectX::SimpleMath::Vector3 angle, float power);
	~Bounce();
	DirectX::SimpleMath::Vector3 getAngle();
	float getPower();
};