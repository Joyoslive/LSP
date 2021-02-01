#include "Input.h"


Input::Input(HWND wndHandle) {
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(wndHandle);
}

Input::~Input() {

}

void Input::InputUpdate() {
	auto kb = m_keyboard->GetState();
	if (kb.Escape)
	{
	//example code
	}
	auto mouse = m_mouse->GetState();

}