#include "pch.h"
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

void Input::initInput(HWND wndHandle, int width, int height)
{
	instance.m_keyboard = std::make_unique<DirectX::Keyboard>();
	instance.m_mouse = std::make_unique<DirectX::Mouse>();
	instance.m_mouse->SetWindow(wndHandle);
	instance.m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	instance.m_mouseX = 0.0f;
	instance.m_mouseY = 0.0f;
	instance.mouse = instance.m_mouse->GetState();
	instance.m_frameTime = 0.0f;
	instance.m_width = width;
	instance.m_height = height;
	instance.m_hwnd = wndHandle;
	SetCursorPos(width/2, height/2);
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

bool Input::keyReleased(Keys key)
{
	DirectX::Keyboard::Keys dxkey;
	dxkey = (DirectX::Keyboard::Keys)key;
	return m_keys.IsKeyReleased(dxkey);
}

bool Input::mouseReleased(MouseKeys key)
{
	switch (key)
	{
	case Input::LeftButton:
		if (m_mouseButtons.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED) {
			return true;
		}
		break;
	case Input::RightButton:
		if (m_mouseButtons.rightButton == DirectX::Mouse::ButtonStateTracker::RELEASED) {
			return true;
		}
		break;
	case Input::MiddleButton:
		if (m_mouseButtons.middleButton == DirectX::Mouse::ButtonStateTracker::RELEASED) {
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

DirectX::SimpleMath::Vector2 Input::mousePos() 
{
	mouse = m_mouse->GetState();
	DirectX::SimpleMath::Vector2 delta = DirectX::SimpleMath::Vector2(float(mouse.x), float(mouse.y));
	return delta;

}

void Input::mouseMovement(float &m_pitch, float &m_yaw)
{
	if (mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{

		m_pitch += m_mouseY * m_frameTime * 1.0;
		m_yaw += m_mouseX* m_frameTime * 1.0;

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

void Input::lockMouse(int code) 
{
	if (code == 0)
	{
		m_latestCode = code;
		if (mouse.positionMode == DirectX::Mouse::MODE_ABSOLUTE)
		{
			m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
			while (ShowCursor(0) > 0);
		}
		else
		{
			m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
			ShowCursor(1);
		}
	}
	else if (code == 1)
	{
		m_latestCode = code;

		m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
 		while(ShowCursor(0) > 0);
	}
	else if (code == 2)
	{
		m_latestCode = code;

		m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
		ShowCursor(1);
	}
}

void Input::update(long double dt) 
{
	m_frameTime = dt;
	auto kb = m_keyboard->GetState();
	mouse = m_mouse->GetState();
	if (mouse.positionMode == DirectX::Mouse::MODE_RELATIVE) 
	{
		POINT ref_p;
		ref_p.x = m_width / 2;
		ref_p.y = m_height / 2;
		ClientToScreen(m_hwnd, &ref_p);
		SetCursorPos(ref_p.x, ref_p.y);
	}
	m_mouseY = mouse.y;
	m_mouseX = mouse.x;

	m_keys.Update(kb);
	m_mouseButtons.Update(mouse);
}

int Input::getLatestCode()
{
	return m_latestCode;
}

long double Input::getTime() {
	return m_frameTime;
}

void Input::setModeAbsolute()
{
	ShowCursor(1);
	m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
}
