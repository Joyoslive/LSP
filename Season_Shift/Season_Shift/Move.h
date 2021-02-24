#pragma once
#include "Logic.h"

class Move : public Logic
{
private:
	DirectX::SimpleMath::Vector3 m_move;
	DirectX::SimpleMath::Vector3 m_speed;
	DirectX::SimpleMath::Vector3 m_range;
	DirectX::SimpleMath::Vector3 m_original;
	float m_direction;
public:
	Move();
	Move(DirectX::SimpleMath::Vector3 speed, DirectX::SimpleMath::Vector3 range);
	~Move();
	void update() override;
};