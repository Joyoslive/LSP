#include "pch.h"
#include "PlayerCameraMovement.h"
#include "Input.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include <imgui_impl_dx11.h>

using namespace DirectX::SimpleMath;

void PlayerCameraMovement::start()
{
	m_yaw = DirectX::XM_2PI / 2;
	m_pitch = 0.0f;
	m_roll = 0.0f;
	m_goToRoll = 0.0f;

	m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
}

void PlayerCameraMovement::update()
{
	Input::getInput().mouseMovement(m_pitch, m_yaw);
	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);

	/*if (m_roll < m_goToRoll)
	{
		m_roll += m_frameTime * DirectX::XM_PI * 5.f / 7.f;
	}
	else if (m_roll > m_goToRoll)
	{
		m_roll -= m_frameTime * DirectX::XM_PI * 5.f / 7.f;
	}

	if ((m_goToRoll < 0 && m_goToRoll >= m_roll) || (m_goToRoll > 0 && m_goToRoll <= m_roll))
		m_goToRoll = 0;*/


	ImGui::Begin("Player Camera");
	{
		ImGui::Text("GoToRoll %f", m_goToRoll);
		ImGui::Text("Roll %f", m_roll);
	}
	ImGui::End();
}

void PlayerCameraMovement::resetCamera()
{
	m_roll = 0.0f;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_goToRoll = 0.0f;
}

void PlayerCameraMovement::updateFrameTime(const float& frameTime)
{
	m_frameTime = frameTime;
}

void PlayerCameraMovement::wallRunning(const bool& wallRunning, const Vector3& normal)
{
	constexpr float minRollOff = 0.01f;
	constexpr float rollWallCheck = 0.3f;
	constexpr float rollModifier = DirectX::XM_PI * 5.f / 7.f;

	if (!wallRunning)
	{
		if (m_roll > minRollOff)
		{
			m_roll -= m_frameTime;
		}
		else if (m_roll < -minRollOff)
		{
			m_roll += m_frameTime;
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
			m_roll -= normal.x * m_frameTime * rollModifier;
		}
		else if (m_roll < rollWallCheck && normal.x < 0)
		{
			m_roll -= normal.x * m_frameTime * rollModifier;
		}
		if (m_roll > -rollWallCheck && normal.z > 0)
		{
			m_roll -= normal.z * m_frameTime * rollModifier;
		}
		else if (m_roll < rollWallCheck && normal.z < 0)
		{
			m_roll -= normal.z * m_frameTime * rollModifier;
		}
	}
}

void PlayerCameraMovement::shake(Vector3 velocity, const Vector3& normal)
{
	velocity = normal * velocity;
	char msgbuf[1000];
	sprintf_s(msgbuf, "My variable is %f, %f, %f\n", velocity.x, velocity.y, velocity.z);
	OutputDebugStringA(msgbuf);
	if (velocity.y < 0)
	{
		OutputDebugStringA("Fisk\n");
		m_goToRoll = -DirectX::XM_PI * 5.f / 7.f;
		//m_roll = DirectX::XM_PI * 5.f / 7.f;
		/*if (m_roll < 0.3f)
		{
			m_roll -= m_frameTime * DirectX::XM_PI * 5.f / 7.f;
		}
		else if (m_roll > -0.3f)
		{
			m_roll += m_frameTime * DirectX::XM_PI * 5.f / 7.f;
		}*/
	}
}