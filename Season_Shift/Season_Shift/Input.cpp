#include "Input.h"


Input::Input(HWND wndHandle) {
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(wndHandle);
	keyboardCurrent[256] = { false };
	keyboardPrevious[256] = { false };
}

Input::~Input() {

}

bool Input::KeyBeingPressed(unsigned int key) {
	if (GetAsyncKeyState(key)) {
		return true;
	}
	return false;
}

bool Input::KeyPressed(unsigned int key) {
	keyboardCurrent[key] = GetAsyncKeyState(key);
	if (keyboardPrevious[key] == false && keyboardCurrent[key] == true) {
		keyboardPrevious[key] = true;
		return true;
	} 
	if (keyboardCurrent[key] == false) {
		keyboardPrevious[key] = false;
	}

	return false;
}