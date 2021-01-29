#pragma once
#include "GfxCore.h"

class GfxRenderer
{
private:
	std::shared_ptr<GfxCore> m_gfxCore;

public:
	GfxRenderer(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~GfxRenderer();


	void clearScreen(float r, float g, float b);	// [0, 1]

};

