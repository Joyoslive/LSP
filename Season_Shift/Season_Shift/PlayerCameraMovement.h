#pragma once
#include "Logic.h"
#include "CameraComponent.h"

class PlayerCameraMovement : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
public:
	PlayerCameraMovement() = default;
	~PlayerCameraMovement() = default;

	void start() override;
	void update() override;
};

