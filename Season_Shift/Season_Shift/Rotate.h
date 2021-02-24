#pragma once
#include "Logic.h"


class Rotate : public Logic
{
private:
	DirectX::SimpleMath::Vector3 rot;
	float m_x, m_y, m_z;
public:
	Rotate();
	Rotate(float x, float y, float z);
	~Rotate();
	void update() override;
};