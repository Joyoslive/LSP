#pragma once
#include <vector>
#include <array>
#include <string>
#include "GraphicsUtils.h"
#include "DXCore.h"
#include "DXBuffer.h"
#include "DXTexture.h"
#include "DXShader.h"
#include <SimpleMath.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

#define COMPILED_SHADERS_DIRECTORY "Graphics/CompiledShaders/"


//struct RenderPass
//{
//	std::shared_ptr<PipelineState> pipeline = nullptr;
//	std::vector<DXTexture> textureOutputs = {};			// RTV
//	std::vector<D3D11_RECT> scissorRects = {};
//	std::vector<DXTexture> textureOutputsUAV = {};		// UAV
//};

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
	std::shared_ptr<DXCore> m_core;

	// Used to bind
	std::array<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> m_currTargetBind;
	unsigned int m_vOffsetBind;


	std::shared_ptr<DXTexture> m_backbufferText;

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
	std::shared_ptr<DXBuffer> createAppendConsumeBuffer(unsigned int count, unsigned int structSize, bool cpuWritable, bool gpuWritable, D3D11_SUBRESOURCE_DATA* subres);
	std::shared_ptr<DXBuffer> createIndirectArgumentBuffer(unsigned int vertexCountPerInstance, unsigned int instanceCount, unsigned int startVertexLocation, unsigned int startInstanceLocation);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> createInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, const std::string& shaderData);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> createSamplerState(D3D11_SAMPLER_DESC desc);
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> createRasterizerState(D3D11_RASTERIZER_DESC desc);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> createDepthStencilState(D3D11_DEPTH_STENCIL_DESC desc);
	Microsoft::WRL::ComPtr<ID3D11BlendState> createBlendState(D3D11_BLEND_DESC desc);

	// 2D Helpers
	std::shared_ptr<DirectX::SpriteBatch> createSpriteBatch();
	std::shared_ptr<DirectX::SpriteFont> createSpriteFont(const std::wstring& fontPath);

	void bindShader(const std::shared_ptr<DXShader>& shader, DXShader::Type stage);
	void bindShaderConstantBuffer(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXBuffer>& res);
	void bindShaderStructuredBuffer(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXBuffer>& res);
	void bindAppendConsumeBuffer(DXShader::Type stage, unsigned int slot, unsigned int offset, const std::shared_ptr<DXBuffer>& res);
	void bindShaderSampler(DXShader::Type stage, unsigned int slot, const Microsoft::WRL::ComPtr<ID3D11SamplerState>& res);
	void bindShaderTexture(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXTexture> res);

	void bindInputLayout(const Microsoft::WRL::ComPtr<ID3D11InputLayout>& il);
	void bindInputTopology(D3D11_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void bindDepthStencilState(const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& dss, unsigned int stencilRef = 0);
	void bindBlendState(const Microsoft::WRL::ComPtr<ID3D11BlendState>& bs, std::array<float, 4> blendFac = { 1.0, 1.0, 1.0, 1.0 }, unsigned int sampleMask = 0xffffffff);
	void bindRasterizerState(const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rss);		

	void bindRenderTargets(const std::vector<std::shared_ptr<DXTexture>>& targets, const std::shared_ptr<DXTexture>& depthTarget);				// RTV/DSV pair
	// UnorderedAccess OM target bind not implemented for now.

	// Assumes single VB/IB pair per Draw or single VB per Draw! (Can be changed later if needed)
	void bindDrawIndexedBuffer(const std::shared_ptr<DXBuffer>& vb, const std::shared_ptr<DXBuffer>& ib, unsigned int vbOffset, unsigned int ibOffset);
	void bindDrawBuffer(const std::shared_ptr<DXBuffer>& vb);

	void bindViewports(const std::vector<D3D11_VIEWPORT>& vps);

	void copyStructureCount(const std::shared_ptr<DXBuffer>& constantBuffer, const std::shared_ptr<DXBuffer>& structuredBuffer);

	void draw(unsigned int vtxCount, unsigned int vbStartIdx = 0);
	void drawIndexed(unsigned int idxCount, unsigned int ibStartIdx, unsigned int vbStartIdx);
	void drawIndexedInstanced(unsigned int idxCountPerInst, unsigned int instCount, unsigned int ibStartIdx, unsigned int vbStartIdx, unsigned int instStartIdx = 0);

	void drawInstancedIndirect(const std::shared_ptr<DXBuffer>& argumentBuffer, unsigned int alignedByteOffsetForArgs = 0);

	void dispatch(unsigned int threadGroupCountX, unsigned int threadGroupCountY, unsigned int threadGroupCountZ);

	void clearRenderTarget(const std::shared_ptr<DXTexture>& target, float color[4]);
	void clearDepthTarget(const std::shared_ptr<DXTexture>& depthTarget, unsigned int clearFlag = D3D11_CLEAR_DEPTH, float depth = 1.0, float stencil = 0.0);

	void onResize(UINT width, UINT height);

	/*
	Defaults to clearing to black
	*/
	void clearScreen();

	/*
	Currently no vsync options
	*/
	void present();

	// No scissors for now

	//void bindBackBufferAsTarget(const std::shared_ptr<DXTexture>& depthTarget = nullptr);

	const std::shared_ptr<DXTexture>& getBackbuffer();
	D3D11_VIEWPORT* getBackbufferViewport();

	//const std::shared_ptr<DXCore>& getCore();

	UINT getClientWidth();
	UINT getClientHeight();




	/*

	std::shared_ptr<DXRenderPass>			createRenderPass(renderPassDesc)
	DXRenderPassDesc
	{
		pipelineState

		vector<viewports>*	OM			Part to draw to in said Output Textures
		scissors*			RS			Part to cut

		Output Textures
		vector<rtvs>*		OM			
		vector<uavs>*		OM
	}

	*/


	/*
	

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
