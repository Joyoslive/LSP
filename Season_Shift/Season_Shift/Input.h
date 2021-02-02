#pragma once
#include <Windows.h>
#include "Keyboard.h"
#include "Mouse.h"
#include <d3d11.h>
#include <SimpleMath.h>

class Input
{
public:
	enum Keys
	{
		A = DirectX::Keyboard::Keys::A,  B = DirectX::Keyboard::Keys::B, C = DirectX::Keyboard::Keys::C, D = DirectX::Keyboard::Keys::D, E = DirectX::Keyboard::Keys::E, F = DirectX::Keyboard::Keys::F,
		G = DirectX::Keyboard::Keys::G, H = DirectX::Keyboard::Keys::H, I = DirectX::Keyboard::Keys::I, J = DirectX::Keyboard::Keys::J, K = DirectX::Keyboard::Keys::K, L = DirectX::Keyboard::Keys::L,
		M = DirectX::Keyboard::Keys::M, N = DirectX::Keyboard::Keys::N, O = DirectX::Keyboard::Keys::O, P = DirectX::Keyboard::Keys::P, Q = DirectX::Keyboard::Keys::Q, 
		R = DirectX::Keyboard::Keys::R, S = DirectX::Keyboard::Keys::S, T = DirectX::Keyboard::Keys::T, U = DirectX::Keyboard::Keys::U, V = DirectX::Keyboard::Keys::V, W = DirectX::Keyboard::Keys::W,
		X = DirectX::Keyboard::Keys::X, Y = DirectX::Keyboard::Keys::Y, Z = DirectX::Keyboard::Keys::Z, Esc = DirectX::Keyboard::Keys::Escape, Space = DirectX::Keyboard::Keys::Space, 
		Shift = DirectX::Keyboard::Keys::LeftShift, F1 = DirectX::Keyboard::Keys::F1, F2 = DirectX::Keyboard::Keys::F2, F3 = DirectX::Keyboard::Keys::F3, F4 = DirectX::Keyboard::Keys::F4, 
		F5 = DirectX::Keyboard::Keys::F5, F6 = DirectX::Keyboard::Keys::F6, F7 = DirectX::Keyboard::Keys::F7, F8 = DirectX::Keyboard::Keys::F8, F9 = DirectX::Keyboard::Keys::F9, F10 = DirectX::Keyboard::Keys::F10,
		F11 = DirectX::Keyboard::Keys::F11, F12 = DirectX::Keyboard::Keys::F12
	};

	enum MouseKeys {
		LeftButton = 1, RightButton, MiddleButton
	};

	Input(HWND wndHandle);
	~Input();

	DirectX::SimpleMath::Vector2 MousePos();
	bool KeyBeingPressed(Keys key);
	bool KeyPressed(Keys key);
	bool MouseBeingPressed(MouseKeys key);
	bool MousePressed(MouseKeys key);

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	DirectX::Keyboard::KeyboardStateTracker m_keys;
	DirectX::Mouse::ButtonStateTracker m_mouseButtons;
};