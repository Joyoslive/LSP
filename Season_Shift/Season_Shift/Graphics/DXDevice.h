#pragma once
#include <memory>
#include "DXCore.h"
#include "DXResourceManager.h"


class DXBuffer;
class DXTexture;
class DXShader;

/*

Works as an abstract 'Device' for low-level graphics resource creations AND as a 'Device Context' for low level binds
- DXTexture
- DXBuffer
- etc.

*/
class DXDevice
{
private:
	std::shared_ptr<DXCore> m_core;

	std::unique_ptr<DXResourceManager> m_dxResourceManager;


	/*
	
	NOTE TO SELF:

	- Check out "nullBlob" in Wicked Engine DX11 for Unbinding purposes
	- Make function BindConstantBuffer that simply takes in an enum for shader stage and uses a switch to determine --> bind(vs, slot, res) (Again, check Wicked Engine for reference!)
	
	*/

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
	Simplified creation where we can avoid too much directly with descriptors
	
	Map/Unmap
	UpdateSubresource

	*/


	/*
	Defaults to clearing to black
	*/
	void clearScreen();

	/*
	Currently no vsync options
	*/
	void present();

};

