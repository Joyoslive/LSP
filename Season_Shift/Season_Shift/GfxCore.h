#pragma once
#include "DXDevice.h"

class GfxCore
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

public:
	GfxCore(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~GfxCore();

	void clearScreen(float r, float g, float b);

};

