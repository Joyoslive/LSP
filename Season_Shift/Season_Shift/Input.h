#pragma once
#include <Windows.h>
#include "Keyboard.h"
#include "Mouse.h"

class Input
{
public:
	Input(HWND wndHandle);
	~Input();
	void InputUpdate();
private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
};