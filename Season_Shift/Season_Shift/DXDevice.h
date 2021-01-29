#pragma once
#include "DXCore.h"
#include <memory>

class DXBuffer;
class DXTexture;
class DXShader;

/*

Works as an abstract 'Device' for low-level graphics resource creations and as a 'Device Context' for low level binds
- DXTexture
- DXBuffer
- etc.

*/
class DXDevice
{
private:
	std::shared_ptr<DXCore> m_core;

public:
	DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~DXDevice();

	/*
	std::shared_ptr<DXShader> createShader();

	std::shared_ptr<DXBuffer> createVertexBuffer(unsigned int elementSize, unsigned int elementStride, bool dynamic, bool cpuUpdates, bool streamOut, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createIndexBuffer(unsigned int size, bool dynamic, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createConstantBuffer(unsigned int size, bool dynamic, bool updateOnCPU, D3D11_SUBRESOURCE_DATA* subres = nullptr);
	*/

	/*
	
	Resource creation functions --> Create Texture, Buffer, etc.
	Simplified creation where we can avoid working with descriptors
	

	Map/Unmap
	UpdateSubresource

	*/

	void clearScreen(float r, float g, float b);

};

