#pragma once
#include <Windows.h>
#include "Keyboard.h"
#include "Mouse.h"

class Input
{
public:
	enum Buttons
	{
		A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Esc = 27, Space = 32, Shift = 16, F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
	Input(HWND wndHandle);
	~Input();
	void InputUpdate();
	bool KeyBeingPressed(unsigned int key);
	bool KeyPressed(unsigned int key);
private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	bool keyboardCurrent[256];
	bool keyboardPrevious[256];
};