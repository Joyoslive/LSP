#include "DXPipeline.h"


DXPipeline::DXPipeline() :
	m_bs(nullptr),
	m_rs(nullptr),
	m_dss(nullptr),
	m_bsBlendFac({ 1.0, 1.0, 1.0, 1.0 }),
	m_bsSampleMask(0xffffffff),
	m_dssStencilRef(0),
	m_inputTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{

}

void DXPipeline::bindPipeline(DXDevice* dev)
{
	dev->bindInputLayout(m_inputLayout);
	dev->bindInputTopology(m_inputTopology);

	dev->bindBlendState(m_bs, m_bsBlendFac, m_bsSampleMask);
	dev->bindRasterizerState(m_rs);
	dev->bindDepthStencilState(m_dss, m_dssStencilRef);

	dev->bindShader(m_vs, DXShader::Type::VS);
	dev->bindShader(m_hs, DXShader::Type::HS);
	dev->bindShader(m_ds, DXShader::Type::DS);
	dev->bindShader(m_gs, DXShader::Type::GS);
	dev->bindShader(m_ps, DXShader::Type::PS);
}

void DXPipeline::attachBlendState(Microsoft::WRL::ComPtr<ID3D11BlendState> bs)
{
	m_bs = bs;
}

void DXPipeline::setBlendFactor(std::array<float, 4> blendFactor)
{
	m_bsBlendFac = blendFactor;
}

void DXPipeline::setBlendSampleMask(unsigned int sampleMask)
{
	m_bsSampleMask = sampleMask;
}

void DXPipeline::attachRasterizerState(Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs)
{
	m_rs = rs;
}

void DXPipeline::setDepthStencilRef(unsigned int stencilRef)
{
	m_dssStencilRef = stencilRef;
}

void DXPipeline::attachInputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout)
{
	m_inputLayout = inputLayout;
}

void DXPipeline::setInputTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_inputTopology = topology;
}

void DXPipeline::attachVS(std::shared_ptr<DXShader> vs)
{
	m_vs = vs;
}

void DXPipeline::attachDepthStencilState(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss)
{
	m_dss = dss;
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
