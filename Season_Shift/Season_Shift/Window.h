#pragma once
#include <windows.h>
#include <string>
#include <assert.h>

class Window
{
private:
	HINSTANCE m_hInst;
	HWND m_hwnd;

	UINT m_clientWidth, m_clientHeight;
	DWORD m_style, m_exStyle;
	std::wstring m_title;
	bool m_isClosed;

	void resizeToWindowToFitClient();
	
public:
	Window(HINSTANCE hInst, const std::wstring title, UINT clientWidth, UINT clientHeight);
	~Window();

	HWND getHWND() const;
	float getAspectRatio() const;
	
	bool isClosed() const;

	LRESULT handleProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


	static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

