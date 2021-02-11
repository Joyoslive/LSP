#pragma once
#include "DXShader.h"
#include "DXBuffer.h"
#include "DXTexture.h"
#include "DXDevice.h"

class DXPipeline
{
private:
	std::vector<std::pair<unsigned int, Microsoft::WRL::ComPtr<ID3D11SamplerState>>> m_samplers;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_bs;
	std::array<float, 4> m_bsBlendFac;
	unsigned int m_bsSampleMask;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dss;
	unsigned int m_dssStencilRef;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rs;


	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY m_inputTopology;

	std::shared_ptr<DXShader> m_vs;
	std::shared_ptr<DXShader> m_hs;
	std::shared_ptr<DXShader> m_ds;
	std::shared_ptr<DXShader> m_gs;
	std::shared_ptr<DXShader> m_ps;

public:
	DXPipeline();
	~DXPipeline() = default;

	void bindPipeline(DXDevice* dev);

	void attachBlendState(Microsoft::WRL::ComPtr<ID3D11BlendState> bs);
	void setBlendFactor(std::array<float, 4> blendFactor = { 1.0, 1.0, 1.0, 1.0 });
	void setBlendSampleMask(unsigned int sampleMask = 0xffffffff);
	
	void attachRasterizerState(Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs);

	void attachDepthStencilState(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss);
	void setDepthStencilRef(unsigned int stencilRef = 0);

	void attachSampler(DXShader::Type shaderStage, unsigned int slot, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	void attachInputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout);
	void setInputTopology(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS/PS + Input Layout/Topology may vary depending on Draw
	void attachVS(std::shared_ptr<DXShader> vs);
	void attachHS(std::shared_ptr<DXShader> hs);
	void attachDS(std::shared_ptr<DXShader> ds);
	void attachGS(std::shared_ptr<DXShader> gs);
	void attachPS(std::shared_ptr<DXShader> ps);
};

