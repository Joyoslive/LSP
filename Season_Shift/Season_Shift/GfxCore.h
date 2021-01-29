#pragma once
#include "DXDevice.h"

class GfxCore
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

public:
	GfxCore(std::shared_ptr<DXDevice> dev);
	~GfxCore();

	void clearScreen(float r, float g, float b);

};

