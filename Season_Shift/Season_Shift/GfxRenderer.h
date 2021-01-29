#pragma once
#include "DXDevice.h"

/*

Works as an high-level graphics "Context"
- Draws
- SetPipelines
- SetCamera
*/ 
class GfxRenderer
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

public:
	GfxRenderer(std::shared_ptr<DXDevice> dev);
	~GfxRenderer();

	void clearScreen(float r, float g, float b);	// [0, 1]

};

