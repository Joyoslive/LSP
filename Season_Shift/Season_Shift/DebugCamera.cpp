#include "pch.h"
#include "DebugCamera.h"

using namespace DirectX; // For multiplying m_speed

DebugCamera::DebugCamera(std::shared_ptr<Camera> incomingCamera)
{
	mousePos = { 0.0f, 0.0f };
	m_up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_speed = 35.0f;
	m_position = incomingCamera->getPosition();
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
	//m_input = input;
	m_camera = incomingCamera;
	m_camera->setPosition(m_position);
	m_camera->setRotation(m_pitch, m_roll, m_yaw);
	m_freecamMode = false;
	m_frameTime = 0.0f;
	m_lock = true;
}

DebugCamera::~DebugCamera() 
{

}

void DebugCamera::move(long double m_frameTime)
{
	auto forward = m_camera->getForward();
	auto right = m_camera->getRight();
	
	if(Input::getInput().keyPressed(Input::Z))
	{
		if (m_lock == true)
		{
			m_lock = false;
		}
		else
		{
			m_lock = true;
		}
	}

	if (m_lock == true)
	{
		if (Input::getInput().keyBeingPressed(Input::W))
		{
			m_position += m_speed * m_frameTime * forward;
		}
		if (Input::getInput().keyBeingPressed(Input::S))
		{
			m_position -= m_speed * m_frameTime * forward;
		}
		if (Input::getInput().keyBeingPressed(Input::A))
		{
			m_position -= m_speed * m_frameTime * right;
		}
		if (Input::getInput().keyBeingPressed(Input::D))
		{
			m_position += m_speed * m_frameTime * right;
		}
		if (Input::getInput().keyBeingPressed(Input::R))
		{
			m_position = { 0.0f, 0.0f, -5.0f, 0.0f };
		}
		if (Input::getInput().keyPressed(Input::L))
		{
			Input::getInput().lockMouse();
		}
		if (Input::getInput().keyPressed(Input::F))
		{
			if (m_freecamMode == false)
			{
				m_freecamMode = true;
			}
			else
			{
				m_freecamMode = false;
			}
		}
		if (Input::getInput().mousePressed(Input::LeftButton))
		{
			m_speed += 3.0f;
		}
		if (Input::getInput().mousePressed(Input::RightButton))
		{
			if (m_speed - 0.01f > 0.0f)
			{
				m_speed -= 3.0f;
			}
		}
		if (Input::getInput().keyBeingPressed(Input::Shift))
		{
			m_position -= m_speed * m_frameTime * m_up;
		}
		if (Input::getInput().keyBeingPressed(Input::Space))
		{
			m_position += m_speed * m_frameTime * m_up;
		}
	}
	m_camera->setPosition(m_position);
}

void DebugCamera::rotate() 
{
	//Get the Mouse Coordinate
	Input::getInput().mouseMovement(m_pitch, m_yaw);
	if (m_freecamMode == false) 
	{
		m_camera->setRotation(m_roll, m_pitch, m_yaw);
	}
	else
	{
		m_camera->setRotationFree(m_roll, m_pitch, m_yaw);
	}
}