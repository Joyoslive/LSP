#include "Input.h"


Input::Input(HWND wndHandle) {
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(wndHandle);
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