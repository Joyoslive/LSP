#pragma once
#include <memory>
#include <vector>
#include <array>
#include <string>
#include "GraphicsUtils.h"
#include "DXCore.h"
#include "DXBuffer.h"
#include "DXTexture.h"
#include "DXShader.h"
#include <SimpleMath.h>



#define COMPILED_SHADERS_DIRECTORY "Graphics/CompiledShaders/"

struct PipelineState
{
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	std::shared_ptr<DXShader> vs = nullptr;	
	std::shared_ptr<DXShader> hs = nullptr;
	std::shared_ptr<DXShader> ds = nullptr;
	std::shared_ptr<DXShader> gs = nullptr;
	std::shared_ptr<DXShader> ps = nullptr;
};

struct RenderPass
{
	std::shared_ptr<PipelineState> pipeline = nullptr;
	std::vector<DXTexture> textureOutputs = {};			// RTV
	std::vector<D3D11_RECT> scissorRects = {};
	std::vector<DXTexture> textureOutputsUAV = {};		// UAV
};

struct Vertex
{
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;
};

struct EngineMeshSubset
{
	unsigned int vertexCount;
	unsigned int vertexStart;

	unsigned int indexStart;
	unsigned int indexCount;

	std::string diffuseFilePath;
	std::string specularFilePath;
	std::string normalFilePath;
};

struct EngineMeshData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;			// 32 bit!
	std::vector<EngineMeshSubset> subsetsInfo;
};


/*

Works as an abstract 'Device' for low-level graphics resource creations AND as a 'Device Context' for low level binds
- DXTexture
- DXBuffer
- etc.

*/
class DXDevice
{
private:
	DXCore m_core;

	// Used to bind
	std::array<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> m_currTargetBind;
	unsigned int m_vOffsetBind;


	/*
	
	NOTE TO SELF:

	- Check out "nullBlob" in Wicked Engine DX11 for Unbinding purposes
	- Make function BindConstantBuffer that simply takes in an enum for shader stage and uses a switch to determine --> bind(vs, slot, res) (Again, check Wicked Engine for reference!)
	- Add explicit "= delete" for classes we know that are NOT allowed to be copied?
	- No great idea on how the resource manager should work for now.. Add, remove.. but what kind of identifier? string? number? what kind of datastructure? map? unordered_map?
		--> Depends on functionality and what it needs to be used for

	- This layer helps us keep a shadow-state for binding purposes so re-binds of the same resource do not occur!
	--> For example, by using this class when binding, we can identify nullptrs and avoid any unneccessary Graphics API calls.
	
	*/



public:
	DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~DXDevice();

	// Helpers
	std::shared_ptr<DXShader> createShader(const std::string& fileName, DXShader::Type shaderType);
	std::shared_ptr<DXTexture> createTexture(const DXTexture::Desc& desc, D3D11_SUBRESOURCE_DATA* subres);

	std::shared_ptr<DXBuffer> createVertexBuffer(unsigned int elementCount, unsigned int elementStride, bool dynamic, bool cpuUpdates, bool streamOut, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createIndexBuffer(unsigned int size, bool dynamic, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createConstantBuffer(unsigned int size, bool dynamic, bool updateOnCPU, D3D11_SUBRESOURCE_DATA* subres = nullptr);
	std::shared_ptr<DXBuffer> createStructuredBuffer(unsigned int count, unsigned int structSize, bool cpuWritable, bool gpuWritable, D3D11_SUBRESOURCE_DATA* subres);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> createInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, const std::string& shaderData);

	/*
	createPipelineState
	createRenderPass

	*/

	void bindShader(const std::shared_ptr<DXShader>& shader, DXShader::Type stage);
	// Bind resource for shader
	void bindShaderConstantBuffer(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXBuffer>& res);
	void bindShaderSampler(DXShader::Type stage, unsigned int slot, const Microsoft::WRL::ComPtr<ID3D11SamplerState>& res);
	void bindShaderTexture(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXTexture> res);

	void bindInputLayout(const Microsoft::WRL::ComPtr<ID3D11InputLayout>& il);
	void bindInputTopology(D3D11_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void bindDepthStencilState(const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& dss, unsigned int stencilRef = 0);
	void bindBlendState(const Microsoft::WRL::ComPtr<ID3D11BlendState>& bs, std::array<float, 4> blendFac = { 1.0, 1.0, 1.0, 1.0 }, unsigned int sampleMask = 0xffffffff);
	void bindRasterizerState(const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rss);		

	void bindRenderTargets(const std::array<std::shared_ptr<DXTexture>, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT>& targets, const std::shared_ptr<DXTexture>& depthTarget);				// RTV/DSV pair
	// UnorderedAccess OM target bind not implemented for now.

	// Assumes single VB/IB pair per Draw or single VB per Draw! (Can be changed later if needed)
	void bindDrawIndexedBuffer(const std::shared_ptr<DXBuffer>& vb, const std::shared_ptr<DXBuffer>& ib, unsigned int vbOffset, unsigned int ibOffset);
	void bindDrawBuffer(const std::shared_ptr<DXBuffer>& vb);

	void bindViewports(const std::vector<D3D11_VIEWPORT>& vps);

	void draw(unsigned int vtxCount, unsigned int vbStartIdx = 0);
	void drawIndexed(unsigned int idxCount, unsigned int ibStartIdx, unsigned int vbStartIdx);
	void drawIndexedInstanced(unsigned int idxCountPerInst, unsigned int instCount, unsigned int ibStartIdx, unsigned int vbStartIdx, unsigned int instStartIdx = 0);

	void clearRenderTarget(const std::shared_ptr<DXTexture>& target, float color[4]);
	void clearDepthTarget(const std::shared_ptr<DXTexture>& depthTarget, unsigned int clearFlag = D3D11_CLEAR_DEPTH, float depth = 0.0, float stencil = 0.0);

	/*
	Defaults to clearing to black
	*/
	void clearScreen();

	/*
	Currently no vsync options
	*/
	void present();

	// No scissors for now

	void bindBackBufferAsTarget(const std::shared_ptr<DXTexture>& depthTarget = nullptr);

	void MapUpdate(const Microsoft::WRL::ComPtr<ID3D11Resource>& resource, void* data, unsigned int dataSize, D3D11_MAP mapType, unsigned int subresIdx = 0, unsigned int mapFlag = 0);

	const Microsoft::WRL::ComPtr<ID3D11Device>& getDevice();


	/*
	std::shared_ptr<DXShader>				createShader(id, shaderstage);

	// These two below are most important as they are closely tied to specific game assets (VBs, IBs and Material for mesh) --> May require std::hash string to std::size_t in GfxResourceDevice
	std::shared_ptr<DXBuffer>				createBuffer(id, desc)

	std::shared_ptr<Buffer> createVertexBuffer(const std::string& name, unsigned int elementSize, unsigned int elementStride, bool dynamic, bool cpuUpdates, bool streamOut, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<Buffer> createIndexBuffer(const std::string& name, unsigned int size, bool dynamic, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<Buffer> createConstantBuffer(const std::string& name, unsigned int size, bool dynamic, bool updateOnCPU, D3D11_SUBRESOURCE_DATA* subres = nullptr);
	std::shared_ptr<Buffer> createStructuredBuffer(const std::string& name, unsigned int count, unsigned int structSize, bool cpuWritable, bool gpuWritable, D3D11_SUBRESOURCE_DATA* subres);

	std::shared_ptr<DXTexture>				createTexture(id, desc);				// Dont forget about mip levels! (e.g Generate Mips)

	ComPtr<ID3D11InputLayout>				createInputLayout(desc);				// We can let this live in PipelineState

	// NO NEED FOR THESE 4 TO BE MADE
	ComPtr<ID3D11BlendState>				createBlendState(desc);					--> If previous objects has been created with same state -> Previous instance returned by D3D11 instead of duplicate! (remarks on MSDN)
	ComPtr<ID3D11RasterizerState>			createRasterizerState(desc);			--> Same as above (remarks on MSDN)
	ComPtr<ID3D11DepthStencilState>			createDepthStencilState(desc);			--> Same as above
	ComPtr<ID3D11SamplerState>				createSamplerState(desc);				--> Same as above
	// Duplicate resources given back by D3D11 requires no repository as it simply gives us a reference to an existing resource we have.
	// We can create above 4 resources without having to worry about duplicate resources.



	std::shared_ptr<DXPipelineState>		createPipelineState(pipelineStateDesc);
	DXPipelineStateDesc
	{
		il*,		IA					
		dss*,		OM					Toggle depth/stencil, depth op
		bs*,		OM					...
		rs*,		RS					FillMode (Wireframe), Toggle scissors, AA line, MS, 
		it*,		IA
		vs*, hs*, ds*, gs*, ps*			IL and IT closely related to VS/GS
	}

	std::shared_ptr<DXRenderPass>			createRenderPass(renderPassDesc)
	DXRenderPassDesc
	{
		pipelineState
		vector<rtvs>*		OM			Output Textures
		vector<viewports>*	OM			Part to draw to in said Output Textures
		scissors*			RS
		vector<uavs>*		OM
	}

	*/


	/*
	
	// Graphics resource upload functions
	Map/Unmap
	UpdateSubresource


	// Shader binds
	void bindConstantBuffer(shaderstage, slot, resource)
	void bindSampler(shaderstage, slot, resource)
	void bindTexture(shaderstage, slot, type, resource)



	// Changes for every new geometry to be drawn
	void bindInputBuffers(vb, ib)

	// Fixed Pipeline Stuff (that changes less often --> these changes are more dependent on technique passes rather than different draw calls)
	// For example, 'input layout' and 'input topology' may change once we want to use a "line renderer" instead of a traditional "triangle renderer"
	// Resources bound below are expected to live in a PipelineState

	// Resources inside a "Pipeline State"
	void bindInputLayout()			// IA		--> Draw bound
	void bindInputTopology()		// IA		--> Draw bound
	void bindDepthStencilState();	// OM
	void bindBlendState();			// OM
	void bindRasterizerState();		// RS

	// Resources inside a "Render Pass" --> Uses a Pipeline State?
	void bindRTV();					// OM
	void bindRTVUAV();				// OM
	void bindViewports();			// RS
	void bindScissors();			// RS


	// Utility
	void clearRTV();
	void clearDSV();
	void generateMips();


	*/



};
