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
	- Add explicit "= delete" for classes we know that are NOT allowed to be copied?
	- No great idea on how the resource manager should work for now.. Add, remove.. but what kind of identifier? string? number? what kind of datastructure? map? unordered_map?
		--> Depends on functionality and what it needs to be used for

	- How should we approach creation of Views for Buffers/Textures? Should be done afterwards through buffer/texture.setSRV()? Or immediately acquired during the creation and follow RAII?
	
	*/

public:
	DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~DXDevice();

	/*
	Resource creation functions --> Create Texture, Buffer, etc.
	Simplified creation where we can avoid too much directly with descriptors
	*/

	/*
	* 
	std::shared_ptr<DXShader> createShader();

	std::shared_ptr<DXBuffer> createVertexBuffer(unsigned int elementSize, unsigned int elementStride, bool dynamic, bool cpuUpdates, bool streamOut, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createIndexBuffer(unsigned int size, bool dynamic, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createConstantBuffer(unsigned int size, bool dynamic, bool updateOnCPU, D3D11_SUBRESOURCE_DATA* subres = nullptr);

	// Dont forget about Mips! (GenerateMips)
	std::shared_ptr<DXTexture> createTexture2D();
	std::shared_ptr<DXTexture> createTextureCube();

	ComPtr<ID3D11SamplerState> createSamplerState(desc);

	ComPtr<ID3D11InputLayout> createInputLayout(desc);

	ComPtr<ID3D11BlendState> createBlendState(desc);				--> If previous objects has been created with same state -> Previous instance returned by D3D11 instead of duplicate! (remarks on MSDN)
	ComPtr<ID3D11RasterizerState> createRasterizerState(desc);		--> Same as above (remarks on MSDN)


	*/

	/*

	
	// Graphics resource upload functions
	Map/Unmap
	UpdateSubresource

	// Fixed pipeline binds
	void bindInputAssemblerData(vb, ib, inputLayout, topology)		--> perhaps these 4 can be wrapped in an "InputAssemblerData" struct
	void bindOutputMergerData(blendState, rtvs, rtvsAndUavs);
	void bindRasterizerStageData(scissors, state, viewports);

	// Shader binds
	void bindConstantBuffer(shaderstage, slot, resource)
	void bindSampler(shaderstage, slot, resource)
	void bindTexture(shaderstage, slot, type, resource)

	// Utility
	void clearRTV();
	void clearDSV();
	void generateMips();

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

