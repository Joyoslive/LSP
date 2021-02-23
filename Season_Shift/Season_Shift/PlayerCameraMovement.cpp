#include "PlayerCameraMovement.h"
#include "Input.h"

using namespace DirectX::SimpleMath;

void PlayerCameraMovement::start()
{
	m_yaw = DirectX::XM_2PI / 2;
	m_pitch = 0.0f;
	m_roll = 0.0f;

	m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
}

void PlayerCameraMovement::update()
{
	Input::getInput().mouseMovement(m_pitch, m_yaw);
	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
}

void PlayerCameraMovement::resetCamera()
{
	m_roll = 0.0f;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
}


void PlayerCameraMovement::wallRunning(const bool& wallRunning, const Vector3& normal, const float& frameTime)
{
	constexpr float minRollOff = 0.01f;
	constexpr float rollWallCheck = 0.3f;
	constexpr float rollModifier = DirectX::XM_PI * 5.f / 7.f;

	if (!wallRunning)
	{
		if (m_roll > minRollOff)
		{
			m_roll -= frameTime;
		}
		else if (m_roll < -minRollOff)
		{
			m_roll += frameTime;
		}
		else
		{
			m_roll = 0.0;
		}
	}
	else
	{
		if (m_roll > -rollWallCheck && normal.x > 0)
		{
			m_roll -= normal.x * frameTime * rollModifier;
		}
		else if (m_roll < rollWallCheck && normal.x < 0)
		{
			m_roll -= normal.x * frameTime * rollModifier;
		}
		if (m_roll > -rollWallCheck && normal.z > 0)
		{
			m_roll -= normal.z * frameTime * rollModifier;
		}
		else if (m_roll < rollWallCheck && normal.z < 0)
		{
			m_roll -= normal.z * frameTime * rollModifier;
		}
	}
}
/*void PlayerCameraMovement::updatePlayerCamera()
{
	Input::getInput().mouseMovement(m_pitch, m_yaw);
	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
}*/