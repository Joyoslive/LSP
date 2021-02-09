#include "DXPipeline.h"


DXPipeline::DXPipeline(Microsoft::WRL::ComPtr<ID3D11BlendState> bs, Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs, Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss, std::vector<std::pair<unsigned int, Microsoft::WRL::ComPtr<ID3D11SamplerState>>> samplers) :
	m_bs(bs),
	m_rs(rs),
	m_dss(dss),
	m_samplers(samplers),
	m_bsBlendFac({ 1.0, 1.0, 1.0, 1.0 }),
	m_bsSampleMask(0xffffffff),
	m_dssStencilRef(0)
{

}

void DXPipeline::bindPipeline(const std::shared_ptr<DXDevice>& dev)
{
	dev->bindInputLayout(m_inputLayout);
	dev->bindInputTopology(m_inputTopology);

	dev->bindBlendState(m_bs, m_bsBlendFac, m_bsSampleMask);
	dev->bindRasterizerState(m_rs);
	dev->bindDepthStencilState(m_dss, m_dssStencilRef);

	for (auto& sampler : m_samplers)
	{
		dev->bindShaderSampler(DXShader::Type::PS, sampler.first, sampler.second);
	}

	dev->bindShader(m_vs, DXShader::Type::VS);
	dev->bindShader(m_hs, DXShader::Type::HS);
	dev->bindShader(m_ds, DXShader::Type::DS);
	dev->bindShader(m_gs, DXShader::Type::GS);
	dev->bindShader(m_ps, DXShader::Type::PS);
}

void DXPipeline::setBlendFactor(std::array<float, 4> blendFactor)
{
	m_bsBlendFac = blendFactor;
}

void DXPipeline::setBlendSampleMask(unsigned int sampleMask)
{
	m_bsSampleMask = sampleMask;
}

void DXPipeline::setDepthStencilRef(unsigned int stencilRef)
{
	m_dssStencilRef = stencilRef;
}

void DXPipeline::attachInputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout)
{
	m_inputLayout = inputLayout;
}

void DXPipeline::attachInputTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_inputTopology = topology;
}

void DXPipeline::attachVS(std::shared_ptr<DXShader> vs)
{
	m_vs = vs;
}

void DXPipeline::attachHS(std::shared_ptr<DXShader> hs)
{
	m_hs = hs;
}

void DXPipeline::attachDS(std::shared_ptr<DXShader> ds)
{
	m_ds = ds;
}

void DXPipeline::attachGS(std::shared_ptr<DXShader> gs)
{
	m_gs = gs;
}

void DXPipeline::attachPS(std::shared_ptr<DXShader> ps)
{
	m_ps = ps;
}
