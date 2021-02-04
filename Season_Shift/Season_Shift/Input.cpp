#include "Input.h"
#include <algorithm>

Input::Input() {
}

void Input::Init(HWND wndHandle) {
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(wndHandle);
	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
}

Input::~Input() {

}

bool Input::KeyBeingPressed(Keys key) {
	auto kb = m_keyboard->GetState();
	DirectX::Keyboard::Keys dxkey;
	dxkey = (DirectX::Keyboard::Keys)key;
	return kb.IsKeyDown(dxkey);
}

bool Input::KeyPressed(Keys key) {
	auto kb = m_keyboard->GetState();
	m_keys.Update(kb);
	DirectX::Keyboard::Keys dxkey;
	dxkey = (DirectX::Keyboard::Keys)key;
	return m_keys.IsKeyPressed(dxkey);
}

DirectX::SimpleMath::Vector2 Input::MousePos() {
	auto mouse = m_mouse->GetState();
	DirectX::SimpleMath::Vector2 delta = DirectX::SimpleMath::Vector2(float(mouse.x), float(mouse.y));
	return delta;

}

void Input::MouseMovment(float &m_pitch, float &m_yaw) {
	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		DirectX::SimpleMath::Vector2 delta = DirectX::SimpleMath::Vector2(float(mouse.x), float(mouse.y))
			* 0.0004f;

		m_pitch -= delta.y;
		m_yaw -= delta.x;

		// limit pitch to straight up or straight down
		// with a little fudge-factor to avoid gimbal lock
		float limit = DirectX::XM_PI / 2.0f - 0.01f;
		m_pitch = max(-limit, m_pitch);
		m_pitch = min(+limit, m_pitch);

		// keep longitude in sane range by wrapping
		if (m_yaw > DirectX::XM_PI)
		{
			m_yaw -= DirectX::XM_PI * 2.0f;
		}
		else if (m_yaw < -DirectX::XM_PI)
		{
			m_yaw += DirectX::XM_PI * 2.0f;
		}
		DirectX::SimpleMath::Vector2 yawPitch = DirectX::SimpleMath::Vector2(m_pitch, m_yaw);
	}
	
}

bool Input::MouseBeingPressed(MouseKeys key) {
	auto mouse = m_mouse->GetState();
	m_mouseButtons.Update(mouse);
	switch (key)
	{
	case Input::LeftButton:
		if (m_mouseButtons.leftButton == DirectX::Mouse::ButtonStateTracker::HELD) {
			return true;
		}
		break;
	case Input::RightButton:
		if (m_mouseButtons.rightButton == DirectX::Mouse::ButtonStateTracker::HELD) {
			return true;
		}
		break;
	case Input::MiddleButton:
		if (m_mouseButtons.middleButton == DirectX::Mouse::ButtonStateTracker::HELD) {
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

bool Input::MousePressed(MouseKeys key) {
	auto mouse = m_mouse->GetState();
	m_mouseButtons.Update(mouse);
	switch (key)
	{
	case Input::LeftButton:
		if (m_mouseButtons.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			return true;
		}
		break;
	case Input::RightButton:
		if (m_mouseButtons.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			return true;
		}
		break;
	case Input::MiddleButton:
		if (m_mouseButtons.middleButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

void Input::LockMouse() {
	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == DirectX::Mouse::MODE_ABSOLUTE) {
		m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}
	else {
		m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
}