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
	m_landShake = false;
	m_stop = true;
	m_rollSecondTime = false;
	m_goToY = 0;

	m_playerCamera = m_gameObject->getComponentType<CameraComponent>(Component::ComponentEnum::CAMERA);
	m_baseCamPosY = 2.0f;
	m_camPosY = m_baseCamPosY;

	m_baseCameraFov = m_playerCamera->getCamera()->getFieldOfView();
	m_cameraFov = m_baseCameraFov;

	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
}

void PlayerCameraMovement::update()
{
	Input::getInput().mouseMovement(m_pitch, m_yaw);
	m_playerCamera->setRotation(m_roll, m_pitch, m_yaw);
	landShake();

	ImGui::Begin("Player Camera");
	{
		ImGui::Text("GoToRoll %f", m_goToRoll);
		ImGui::Text("Roll %f", m_roll);
		ImGui::Text("RunRoll %f", m_runRoll);
		ImGui::Text("MoveY %f", m_camPosY);
		ImGui::Text("FOV %f", m_cameraFov);
		ImGui::SliderFloat("Modifier", &temp, 0.01f, 1000.0f);
	}
	ImGui::End();
}

void PlayerCameraMovement::resetCamera()
{
	m_roll = 0.0f;
	m_pitch = 0.0f;
	m_yaw = DirectX::XM_2PI / 2;
	m_goToRoll = 0.0f;
	m_runMoveY = 0;
	m_runRoll = 0;
	m_runShake = false;
	m_landShake = false;
	m_rollSecondTime = false;
}

void PlayerCameraMovement::updateFrameTime(const float& frameTime)
{
	m_frameTime = frameTime;
}

//Shakes the camera when the player lands on the ground
void PlayerCameraMovement::landShake()
{
	constexpr float rollSpeed = DirectX::XM_PI * 5.f / 7.f;
	constexpr float moveYSpeed = 12.0f;
	//Half of max velocityY of player
	constexpr float moveYPosModifier = 50.0f;

	if (!m_stop)
	{
		if (m_roll < m_goToRoll)
		{
			m_roll += m_frameTime * rollSpeed;
		}
		else if (m_roll > m_goToRoll)
		{
			m_roll -= m_frameTime * rollSpeed;
		}

		//Checks so it doesn't go beyond where it should go
		if ((m_goToRoll < 0 && m_goToRoll >= m_roll) || (m_goToRoll > 0 && m_goToRoll <= m_roll))
			m_goToRoll = 0;

		//Checks if it has reached its roll and then changes direction
		if ((m_direction > 0 && m_roll <= 0) || (m_direction < 0 && m_roll >= 0) || m_direction == 0)
		{
			//Only does this one time per land shake
			if (!m_rollSecondTime)
			{
				setGoToRoll(m_rollSecondTime);
			}
			//Stops if it has gone both directions (left and right)
			if (m_rollLandShake)
			{
				m_stop = m_rollSecondTime;
			}
			m_roll = 0;
			m_rollLandShake = !m_rollSecondTime;
			m_rollSecondTime = !m_rollSecondTime;
		}
	}

	if (!m_moveStop)
	{
		//Sets the first y position
		if (m_goToY == 0)
			setRunMoveY(!m_moveSecondTime, fabs(m_velocityY) / moveYPosModifier);

		if (m_camPosY < m_goToY)
		{
			m_camPosY += m_frameTime * moveYSpeed;
		}
		else if (m_camPosY > m_goToY)
		{
			m_camPosY -= m_frameTime * moveYSpeed;
		}

		if ((m_camPosY < m_goToY && m_camMoveDirection < 0) || (m_camPosY > m_goToY && m_camMoveDirection > 0))
		{
			//Sets the second y position and we want it to be zero so we have a small number here
			if (!m_moveSecondTime)
			{
				setRunMoveY(m_moveSecondTime, 0.001f);
			}
			//Stops if it has gone both directions (down and up)
			if (m_moveLandShake)
			{
				m_moveStop = m_moveSecondTime;
				if (m_moveStop)
				{
					m_goToY = 0;
					m_camPosY = m_baseCamPosY;
				}
			}
			m_moveLandShake = !m_moveSecondTime;
			m_moveSecondTime = !m_moveSecondTime;
		}
		m_playerCamera->setOffset(0.0f, m_camPosY, 0.0f);
	}

	//landshake only becomes false if both roll and move shake are false (now the other shakes are allowed to do their shakes)
	m_landShake = m_moveLandShake || m_rollLandShake;
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void PlayerCameraMovement::changeFOV(const Vector3& velocity, const float& maxSpeedXZ, const float& maxSpeedY)
{
	constexpr float minYVelocity = 30.0f;
	constexpr float velocityXZModifier = 0.95f;
	constexpr float velocityYModifier = 0.7f;
	constexpr float diffModifier = 0.5;
	constexpr float deltaMinModifier = 3.0f;
	constexpr float deltaMaxModifier = 6.0f;

	Vector3 velocityXZ = velocity;
	velocityXZ.y = 0;
	Vector3 velocityY = velocity;
	velocityY.x = velocityY.z = 0;

	//Only use velocityY if it is higher than minYVelocity
	if (velocityY.Length() < minYVelocity)
		velocityY.y = 0;

	float diff = std::clamp(velocityXZ.Length() * velocityXZModifier / maxSpeedXZ + velocityY.Length() * velocityYModifier / maxSpeedY, 0.0f, 1.0f);
	float delta = m_frameTime * deltaMinModifier;

	if (diff < m_cameraFov - m_baseCameraFov)
		delta = m_frameTime * deltaMaxModifier;

	m_cameraFov = lerp(m_cameraFov, m_baseCameraFov + diff * diffModifier, delta);
	m_playerCamera->setFieldOfView(m_cameraFov);
}

void PlayerCameraMovement::wallRunning(const bool& wallRunning, const Vector3& normal)
{
	constexpr float minRollOff = 0.01f;
	constexpr float rollWallCheck = 0.3f;
	constexpr float rollModifier = DirectX::XM_PI * 5.f / 7.f;

	//Returns if the other shakes are happening
	if (m_landShake || m_runShake)
		return;

	//Returns the roll to 0
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
	//Moves to roll to the wallrunning roll when wallrunning
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
	//Sets up the landshake
	constexpr float minVelocity = -50.0f;
	velocity = normal * velocity;
	if (velocity.y < minVelocity)
	{
		m_stop = false;
		m_moveStop = false;
		m_landShake = true;
		m_rollSecondTime = false;
		m_runShake = false;
		m_rollLandShake = true;
		m_moveLandShake = true;
		setGoToRoll(!m_rollSecondTime);
		m_velocityY = velocity.y;
		m_moveSecondTime = false;
		m_goToY = 0.0f;
		m_camMoveDirection = 0;
		m_camPosY = m_baseCamPosY;
	}
}

void PlayerCameraMovement::setDirection(const float& roll, const bool& moveCam)
{
	//changes the direction
	if (!moveCam)
	{
		if (roll < 0)
			m_direction = -1;
		else if (roll > 0)
			m_direction = 1;
		else
			m_direction = 0;
	}
	else
	{
		if (roll < 0)
			m_camMoveDirection = -1;
		else if (roll > 0)
			m_camMoveDirection = 1;
		else
			m_camMoveDirection = 0;
	}
}

void PlayerCameraMovement::setGoToRoll(const bool& firstTime)
{
	constexpr float rollLandShake = DirectX::XM_PI / 26.0f;//36.0f;
	constexpr float modifier = 100.0f;

	//Sets the new go To Roll
	if (firstTime)
	{
		if (m_direction != 0)
			m_goToRoll = rollLandShake * m_direction * fabs(m_velocityY) / modifier;
		else
			m_goToRoll = -rollLandShake * fabs(m_velocityY) / modifier;
	}
	else
		m_goToRoll = rollLandShake * -1 * m_direction * fabs(m_velocityY) / (modifier * 2.0f);

	setDirection(m_goToRoll);
}

void PlayerCameraMovement::setRunRoll(const bool& firstTime)
{
	//Sets the runRoll in a new direction
	constexpr float rollRunShake = DirectX::XM_PI / 512.0f;//256.0f;
	if (firstTime)
	{
		if (m_direction != 0)
			m_runRoll = rollRunShake * m_direction;
		else
			m_runRoll = -rollRunShake;
	}
	else
	{
		if (m_direction != 0)
			m_runRoll = rollRunShake * -1 * m_direction;
		else
			m_runRoll = rollRunShake;
	}

	setDirection(m_runRoll);
}

void PlayerCameraMovement::runShake(const Vector3& moveDirection, const bool& onGround, const bool& wallRunning, const float& speed, const float& maxSpeed)
{
	if (m_landShake)
		return;

	//Changes the roll of the camera
	float rollSpeed = DirectX::XM_PI / 50.f * speed / maxSpeed;
	if (moveDirection != Vector3::Zero && onGround && !wallRunning && !m_landShake)
	{
		if (m_runRoll == 0)
			setRunRoll(m_rollSecondTime);

		if (m_roll < m_runRoll)
		{
			m_roll += m_frameTime * rollSpeed;
		}
		else if (m_roll > m_runRoll)
		{
			m_roll -= m_frameTime * rollSpeed;
		}

		//Changes the direction if roll has passed run Roll
		if ((m_runRoll < 0 && m_runRoll >= m_roll) || (m_runRoll > 0 && m_runRoll <= m_roll))
		{
			setRunRoll(!m_rollSecondTime);
			m_rollSecondTime = !m_rollSecondTime;
		}
		m_runShake = true;
		runMoveY(moveDirection, onGround, wallRunning, speed, maxSpeed);
	}
	else
		m_runShake = false;

}

void PlayerCameraMovement::runMoveY(const Vector3& moveDirection, const bool& onGround, const bool& wallRunning, const float& speed, const float& maxSpeed)
{
	constexpr float moveYSpeed = 1.0f;

	//Changes to move position
	if (moveDirection != Vector3::Zero && onGround && !wallRunning && !m_landShake)
	{
		if (m_goToY == 0)
			setRunMoveY(m_moveSecondTime);

		if (m_camPosY < m_goToY)
		{
			m_camPosY += m_frameTime * moveYSpeed;
		}
		else if (m_camPosY > m_goToY)
		{
			m_camPosY -= m_frameTime * moveYSpeed;
		}

		//Changes the direction if move position has passed go to move y
		if ((m_camPosY < m_goToY && m_camMoveDirection < 0) || (m_camPosY > m_goToY && m_camMoveDirection > 0))
		{
			setRunMoveY(m_moveSecondTime);
			m_moveSecondTime = !m_moveSecondTime;
		}
	}
	else
		m_camPosY = m_baseCamPosY;

	m_playerCamera->setOffset(0.0f, m_camPosY, 0.0f);
}

void PlayerCameraMovement::setRunMoveY(const bool& firstTime, float changeYPos)
{
	//Sets new go to move position
	if (firstTime)
	{
		if (m_camMoveDirection < 0)
			changeYPos /= 2;
		if (m_camMoveDirection != 0)
			m_goToY = m_baseCamPosY + changeYPos * m_camMoveDirection;
		else
			m_goToY = m_baseCamPosY + -changeYPos;
	}
	else
	{
		if (m_camMoveDirection > 0)
			changeYPos /= 2;
		if (m_camMoveDirection != 0)
			m_goToY = m_baseCamPosY + changeYPos * -1 * m_camMoveDirection;
		else
			m_goToY = m_baseCamPosY + changeYPos;
	}

	setDirection(m_goToY - m_baseCamPosY, true);
}

//char msgbuf[1000];
//sprintf_s(msgbuf, "My variable is %f\n", velocitySkipY.Length());
//OutputDebugStringA(msgbuf);