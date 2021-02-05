#include "DebugCamera.h"

using namespace DirectX; // For multiplying m_speed

DebugCamera::DebugCamera(HWND wndHandle, std::shared_ptr<Camera> incomingCamera) {
	mousePos = { 0.0f, 0.0f };
	m_up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_speed = 0.005f;
	m_position = incomingCamera->getPosition();
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
	input.Init(wndHandle);
	camera = incomingCamera;
	camera->setPosition(m_position);
	camera->setRotation(m_pitch, m_roll, m_yaw);
	m_freecamMode = false;
}

DebugCamera::~DebugCamera() {

}

void DebugCamera::Move() {
	auto forward = camera->getForward();
	auto right = camera->getRight();
	input.Update();
	if (input.KeyBeingPressed(Input::W)) {
		m_position += m_speed * forward;
	}
	if (input.KeyBeingPressed(Input::S)) {
		m_position -= m_speed * forward;
	}
	if (input.KeyBeingPressed(Input::A)) {
		m_position -= m_speed * right;
	}
	if (input.KeyBeingPressed(Input::D)) {
		m_position += m_speed * right;
	}
	if (input.KeyBeingPressed(Input::R)) {
		m_position = { 0.0f, 0.0f, -5.0f, 0.0f };
	}
	if (input.KeyPressed(Input::L)) {
		input.LockMouse();
	}
	if (input.KeyPressed(Input::F)) {
		if (m_freecamMode == false) {
			m_freecamMode = true;
		}
		else {
			m_freecamMode = false;
		}
	}
	if (input.MousePressed(Input::LeftButton)) {
		m_speed += 0.01f;
	}
	if (input.MousePressed(Input::RightButton)) {
		if (m_speed -0.01f > 0.0f) {
			m_speed -= 0.01f;
		}
	}
	if (input.KeyBeingPressed(Input::Shift)) {
		m_position -= m_speed * m_up;
	}
	if (input.KeyBeingPressed(Input::Space)) {
		m_position += m_speed * m_up;
	}
	camera->setPosition(m_position);
}

void DebugCamera::Rotate() {
	//Get the Mouse Coordinate
	input.MouseMovement(m_pitch, m_yaw);
	if (m_freecamMode == false) {
		camera->setRotation(m_roll, m_pitch, m_yaw);
	}
	else {
		camera->setRotationFree(m_roll, m_pitch, m_yaw);
	}
}