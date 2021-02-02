#include "Input.h"


Input::Input(HWND wndHandle) {
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(wndHandle);
}

Input::~Input() {

}

bool Input::KeyBeingPressed(DirectX::Keyboard::Keys key) {
	auto kb = m_keyboard->GetState();
	return kb.IsKeyDown(key);
}

bool Input::KeyPressed(DirectX::Keyboard::Keys key) {
	auto kb = m_keyboard->GetState();
	m_keys.Update(kb);
	return m_keys.IsKeyPressed(key);
}