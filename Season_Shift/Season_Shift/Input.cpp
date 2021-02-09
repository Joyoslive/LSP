#include "Input.h"
#include <algorithm>

Input Input::instance = Input();

Input::Input()
{
}

Input::~Input() 
{
}

Input& Input::getInput()
{
	return instance;
}

void Input::initInput(HWND wndHandle)
{
	instance.m_keyboard = std::make_unique<DirectX::Keyboard>();
	instance.m_mouse = std::make_unique<DirectX::Mouse>();
	instance.m_mouse->SetWindow(wndHandle);
	instance.m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
}

bool Input::keyBeingPressed(Keys key) 
{
	auto kb = m_keyboard->GetState();
	DirectX::Keyboard::Keys dxkey;
	dxkey = (DirectX::Keyboard::Keys)key;
	return kb.IsKeyDown(dxkey);
}

bool Input::keyPressed(Keys key) 
{
	DirectX::Keyboard::Keys dxkey;
	dxkey = (DirectX::Keyboard::Keys)key;
	return m_keys.IsKeyPressed(dxkey);
}

DirectX::SimpleMath::Vector2 Input::mousePos() 
{
	auto mouse = m_mouse->GetState();
	DirectX::SimpleMath::Vector2 delta = DirectX::SimpleMath::Vector2(float(mouse.x), float(mouse.y));
	return delta;

}

void Input::mouseMovement(float &m_pitch, float &m_yaw) 
{
	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		DirectX::SimpleMath::Vector2 delta = DirectX::SimpleMath::Vector2(float(mouse.x), float(mouse.y))
			* 0.0004f;

		m_pitch += delta.y;
		m_yaw += delta.x;

		// limit pitch to straight up or straight down
		// with a little fudge-factor to avoid gimbal lock
		float limit = DirectX::XM_PI / 2.0f - 0.01f;
		m_pitch = std::max(-limit, m_pitch);
		m_pitch = std::min(+limit, m_pitch);

		// keep longitude in sane range by wrapping
		if (m_yaw > DirectX::XM_PI)
		{
			m_yaw -= DirectX::XM_PI * 2.0f;
		}
		else if (m_yaw < -DirectX::XM_PI)
		{
			m_yaw += DirectX::XM_PI * 2.0f;
		}
	}
	
}

bool Input::mouseBeingPressed(MouseKeys key) 
{
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

bool Input::mousePressed(MouseKeys key) 
{
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

void Input::lockMouse() 
{
	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == DirectX::Mouse::MODE_ABSOLUTE) {
		m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}
	else {
		m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
}

void Input::update() 
{
	auto kb = m_keyboard->GetState();
	auto mouse = m_mouse->GetState();
	m_keys.Update(kb);
	m_mouseButtons.Update(mouse);
}