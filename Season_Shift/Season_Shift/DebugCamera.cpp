#include "DebugCamera.h"

using namespace DirectX; // For multiplying m_speed

DebugCamera::DebugCamera(HWND wndHandle, std::shared_ptr<Camera> incomingCamera) 
{
	mousePos = { 0.0f, 0.0f };
	m_up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_speed = 0.005f;
	m_position = incomingCamera->getPosition();
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
	m_input.Init(wndHandle);
	m_camera = incomingCamera;
	m_camera->setPosition(m_position);
	m_camera->setRotation(m_pitch, m_roll, m_yaw);
	m_freecamMode = false;
}

DebugCamera::~DebugCamera() 
{

}

void DebugCamera::move() 
{
	auto forward = m_camera->getForward();
	auto right = m_camera->getRight();
	m_input.update();
	if (m_input.keyBeingPressed(Input::W)) 
	{
		m_position += m_speed * forward;
	}
	if (m_input.keyBeingPressed(Input::S)) 
	{
		m_position -= m_speed * forward;
	}
	if (m_input.keyBeingPressed(Input::A)) 
	{
		m_position -= m_speed * right;
	}
	if (m_input.keyBeingPressed(Input::D)) 
	{
		m_position += m_speed * right;
	}
	if (m_input.keyBeingPressed(Input::R)) 
	{
		m_position = { 0.0f, 0.0f, -5.0f, 0.0f };
	}
	if (m_input.keyPressed(Input::L)) 
	{
		m_input.lockMouse();
	}
	if (m_input.keyPressed(Input::F)) 
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
	if (m_input.mousePressed(Input::LeftButton)) 
	{
		m_speed += 0.01f;
	}
	if (m_input.mousePressed(Input::RightButton)) 
	{
		if (m_speed -0.01f > 0.0f) 
		{
			m_speed -= 0.01f;
		}
	}
	if (m_input.keyBeingPressed(Input::Shift)) 
	{
		m_position -= m_speed * m_up;
	}
	if (m_input.keyBeingPressed(Input::Space)) 
	{
		m_position += m_speed * m_up;
	}
	m_camera->setPosition(m_position);
}

void DebugCamera::rotate() 
{
	//Get the Mouse Coordinate
	m_input.mouseMovement(m_pitch, m_yaw);
	if (m_freecamMode == false) 
	{
		m_camera->setRotation(m_roll, m_pitch, m_yaw);
	}
	else
	{
		m_camera->setRotationFree(m_roll, m_pitch, m_yaw);
	}
}