#pragma once
#include "Logic.h"
#include "Timer.h"

class Rotate : public Logic
{
private:
	DirectX::SimpleMath::Vector3 rot;
public:
	Rotate();
	~Rotate();
	void update() override;
};