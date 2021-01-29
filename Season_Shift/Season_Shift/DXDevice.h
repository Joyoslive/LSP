#pragma once
#include "DXCore.h"
#include <memory>

class DXDevice
{
private:
	std::shared_ptr<DXCore> m_core;

public:
	DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~DXDevice();

	/*
	
	Resource creation functions --> Create Texture, Buffer, etc.
	

	Map/Unmap
	UpdateSubresource

	*/

	void clearScreen(float r, float g, float b);

};

