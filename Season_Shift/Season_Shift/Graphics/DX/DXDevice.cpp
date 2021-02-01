#include "DXDevice.h"

using Microsoft::WRL::ComPtr;

DXDevice::DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_core(DXCore(hwnd, clientWidth, clientHeight))
{

}

DXDevice::~DXDevice()
{
}

std::shared_ptr<DXShader> DXDevice::createShader(const std::string& fileName, DXShader::Type shaderType)
{
	std::string shaderData = loadCompiledShader("CompiledShaders/" + fileName);

	ComPtr<ID3D11VertexShader> vs = nullptr;
	ComPtr<ID3D11HullShader> hs = nullptr;
	ComPtr<ID3D11DomainShader> ds = nullptr;
	ComPtr<ID3D11GeometryShader> gs = nullptr;
	ComPtr<ID3D11PixelShader> ps = nullptr;
	ComPtr<ID3D11ComputeShader> cs = nullptr;

	std::any shader;
	switch (shaderType)
	{
	case DXShader::Type::VS:
		HRCHECK(m_core.getDevice()->CreateVertexShader(shaderData.data(), shaderData.size(), nullptr, vs.GetAddressOf()));
		shader = vs;
		break;
	case DXShader::Type::HS:
		HRCHECK(m_core.getDevice()->CreateHullShader(shaderData.data(), shaderData.size(), nullptr, hs.GetAddressOf()));
		shader = hs;
		break;
	case DXShader::Type::DS:
		HRCHECK(m_core.getDevice()->CreateDomainShader(shaderData.data(), shaderData.size(), nullptr, ds.GetAddressOf()));
		shader = ds;
		break;
	case DXShader::Type::GS:
		HRCHECK(m_core.getDevice()->CreateGeometryShader(shaderData.data(), shaderData.size(), nullptr, gs.GetAddressOf()));
		shader = gs;
		break;
	case DXShader::Type::PS:
		HRCHECK(m_core.getDevice()->CreatePixelShader(shaderData.data(), shaderData.size(), nullptr, ps.GetAddressOf()));
		shader = ps;
		break;
	case DXShader::Type::CS:
		HRCHECK(m_core.getDevice()->CreateComputeShader(shaderData.data(), shaderData.size(), nullptr, cs.GetAddressOf()));
		shader = cs;
		break;
	default:
		assert(false);
	}

	std::shared_ptr<DXShader> shd = std::make_shared<DXShader>(shader, shaderType, shaderData);
	return shd;

}

std::shared_ptr<DXBuffer> DXDevice::createVertexBuffer(unsigned int elementSize, unsigned int elementStride, bool dynamic, bool cpuUpdates, bool streamOut, D3D11_SUBRESOURCE_DATA* subres)
{
	D3D11_BUFFER_DESC desc = { };
	desc.ByteWidth = elementSize * elementStride;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (streamOut)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	else
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (dynamic && cpuUpdates)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (dynamic)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else
	{
		if (subres == nullptr)	assert(false);	// Immutable but no initial data!?!?

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}

	ComPtr<ID3D11Buffer> buf = nullptr;
	HRCHECK(m_core.getDevice()->CreateBuffer(&desc, subres, buf.GetAddressOf()));

	std::shared_ptr<DXBuffer> toRet = std::make_shared<DXBuffer>(buf, desc);
	return toRet;

}

std::shared_ptr<DXBuffer> DXDevice::createIndexBuffer(unsigned int size, bool dynamic, D3D11_SUBRESOURCE_DATA* subres)
{
	D3D11_BUFFER_DESC desc = { };
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (dynamic)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		if (subres == nullptr)	assert(false);	// Immutable but no initial data!?!?

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}


	ComPtr<ID3D11Buffer> buf = nullptr;
	HRCHECK(m_core.getDevice()->CreateBuffer(&desc, subres, buf.GetAddressOf()));

	std::shared_ptr<DXBuffer> toRet = std::make_shared<DXBuffer>(buf, desc);
	return toRet;
}

std::shared_ptr<DXBuffer> DXDevice::createConstantBuffer(unsigned int size, bool dynamic, bool updateOnCPU, D3D11_SUBRESOURCE_DATA* subres)
{
	if (size % 16 != 0) assert(false);	// Not 16 byte aligned!

	D3D11_BUFFER_DESC desc = { };
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (dynamic && updateOnCPU)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (dynamic && !updateOnCPU)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else
	{
		if (subres == nullptr)	assert(false);	// Immutable but no initial data!?!?

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}


	ComPtr<ID3D11Buffer> buf = nullptr;
	HRCHECK(m_core.getDevice()->CreateBuffer(&desc, subres, buf.GetAddressOf()));

	std::shared_ptr<DXBuffer> toRet = std::make_shared<DXBuffer>(buf, desc);
	return toRet;
}

std::shared_ptr<DXBuffer> DXDevice::createStructuredBuffer(unsigned int count, unsigned int structSize, bool cpuWritable, bool gpuWritable, D3D11_SUBRESOURCE_DATA* subres)
{
	D3D11_BUFFER_DESC desc = { };
	desc.ByteWidth = count * structSize;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = structSize;

	if (!cpuWritable && !gpuWritable)
	{
		if (subres == nullptr)	assert(false);	// Immutable but no initial data!?!?

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}
	else if (cpuWritable && !gpuWritable)
	{
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (!cpuWritable && gpuWritable)
	{
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else // CPU and GPU writable.. can't do this simultaneously
	{
		assert(false);
	}

	ComPtr<ID3D11Buffer> buf = nullptr;
	HRCHECK(m_core.getDevice()->CreateBuffer(&desc, subres, buf.GetAddressOf()));

	std::shared_ptr<DXBuffer> toRet = std::make_shared<DXBuffer>(buf, desc);
	return toRet;
}

std::shared_ptr<DXTexture> DXDevice::createTexture(const DXTexture::Desc& desc, D3D11_SUBRESOURCE_DATA* subres)
{
	std::shared_ptr<DXTexture> tex = nullptr;


	if (desc.type == DXTexture::Type::TEX1D)
	{
		ComPtr<ID3D11Texture1D> t = nullptr;
		HRCHECK(m_core.getDevice()->CreateTexture1D(&desc.desc1D, subres, t.GetAddressOf()));
		tex = std::make_shared<DXTexture>(t, desc);
	}
	else if (desc.type == DXTexture::Type::TEX2D)
	{
		ComPtr<ID3D11Texture2D> t = nullptr;
		HRCHECK(m_core.getDevice()->CreateTexture2D(&desc.desc2D, subres, t.GetAddressOf()));
		tex = std::make_shared<DXTexture>(t, desc);
	}
	// TEX3D
	else
	{
		ComPtr<ID3D11Texture3D> t = nullptr;
		HRCHECK(m_core.getDevice()->CreateTexture3D(&desc.desc3D, subres, t.GetAddressOf()));
		tex = std::make_shared<DXTexture>(t, desc);
	}
	
	return tex;
}

void DXDevice::bindShaderConstantBuffer(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXBuffer>& res)
{
	if (!(res->getDesc().BindFlags & D3D11_BIND_CONSTANT_BUFFER)) assert(false);

	switch (stage)
	{
	case DXShader::Type::VS:
		m_core.getImmediateContext()->VSSetConstantBuffers(slot, 1, res->getBuffer().GetAddressOf());
		break;
	case DXShader::Type::HS:
		m_core.getImmediateContext()->HSSetConstantBuffers(slot, 1, res->getBuffer().GetAddressOf());
		break;
	case DXShader::Type::DS:
		m_core.getImmediateContext()->DSSetConstantBuffers(slot, 1, res->getBuffer().GetAddressOf());
		break;
	case DXShader::Type::GS:
		m_core.getImmediateContext()->GSSetConstantBuffers(slot, 1, res->getBuffer().GetAddressOf());
		break;
	case DXShader::Type::PS:
		m_core.getImmediateContext()->PSSetConstantBuffers(slot, 1, res->getBuffer().GetAddressOf());
		break;
	case DXShader::Type::CS:
		m_core.getImmediateContext()->CSSetConstantBuffers(slot, 1, res->getBuffer().GetAddressOf());
		break;
	default:
		assert(false);
		break;
	}
}

void DXDevice::bindShaderSampler(DXShader::Type stage, unsigned int slot, const Microsoft::WRL::ComPtr<ID3D11SamplerState>& res)
{
	switch (stage)
	{
	case DXShader::Type::VS:
		m_core.getImmediateContext()->VSSetSamplers(slot, 1, res.GetAddressOf());
		break;
	case DXShader::Type::HS:
		m_core.getImmediateContext()->HSSetSamplers(slot, 1, res.GetAddressOf());
		break;
	case DXShader::Type::DS:
		m_core.getImmediateContext()->DSSetSamplers(slot, 1, res.GetAddressOf());
		break;
	case DXShader::Type::GS:
		m_core.getImmediateContext()->GSSetSamplers(slot, 1, res.GetAddressOf());
		break;
	case DXShader::Type::PS:
		m_core.getImmediateContext()->PSSetSamplers(slot, 1, res.GetAddressOf());
		break;
	case DXShader::Type::CS:
		m_core.getImmediateContext()->CSSetSamplers(slot, 1, res.GetAddressOf());
		break;
	default:
		assert(false);
		break;
	}
}

void DXDevice::bindShaderTexture(DXShader::Type stage, unsigned int slot, const std::shared_ptr<DXTexture> res)
{
	switch (stage)
	{
	case DXShader::Type::VS:
		m_core.getImmediateContext()->VSSetShaderResources(slot, 1, res->getSRV().GetAddressOf());
		break;
	case DXShader::Type::HS:
		m_core.getImmediateContext()->HSSetShaderResources(slot, 1, res->getSRV().GetAddressOf());
		break;
	case DXShader::Type::DS:
		m_core.getImmediateContext()->DSSetShaderResources(slot, 1, res->getSRV().GetAddressOf());
		break;
	case DXShader::Type::GS:
		m_core.getImmediateContext()->GSSetShaderResources(slot, 1, res->getSRV().GetAddressOf());
		break;
	case DXShader::Type::PS:
		m_core.getImmediateContext()->PSSetShaderResources(slot, 1, res->getSRV().GetAddressOf());
		break;
	case DXShader::Type::CS:
		m_core.getImmediateContext()->CSSetShaderResources(slot, 1, res->getSRV().GetAddressOf());
		break;
	default:
		assert(false);
		break;
	}
}

void DXDevice::bindInputLayout(const Microsoft::WRL::ComPtr<ID3D11InputLayout>& il)
{
	m_core.getImmediateContext()->IASetInputLayout(il.Get());
}

void DXDevice::bindInputTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_core.getImmediateContext()->IASetPrimitiveTopology(topology);
}

void DXDevice::bindDepthStencilState(const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& dss, unsigned int stencilRef)
{
	m_core.getImmediateContext()->OMSetDepthStencilState(dss.Get(), stencilRef);

}

void DXDevice::bindBlendState(const Microsoft::WRL::ComPtr<ID3D11BlendState>& bs, std::array<float, 4> blendFac, unsigned int sampleMask)
{
	m_core.getImmediateContext()->OMSetBlendState(bs.Get(), blendFac.data(), sampleMask);
}

void DXDevice::bindRasterizerState(const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rss)
{
	m_core.getImmediateContext()->RSSetState(rss.Get());
}


void DXDevice::bindRenderTargets(const std::array<const std::shared_ptr<DXTexture>, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT>& targets, const std::shared_ptr<DXTexture>& depthTarget)
{
	if (depthTarget->getDesc().type != DXTexture::Type::TEX2D || depthTarget->getDSV() == nullptr) assert(false);

	for (auto& target : targets)
	{
		if (target->getDesc().type != DXTexture::Type::TEX2D || target->getRTV() == nullptr) assert(false);
	}
	for (unsigned int i = 0; i < targets.size(); ++i)
	{
		m_currTargetBind[i] = targets[i]->getRTV().Get();
	}

	m_core.getImmediateContext()->OMSetRenderTargets(targets.size(), m_currTargetBind.data(), depthTarget->getDSV().Get());

}

void DXDevice::bindDrawBuffers(std::vector<std::pair<const std::shared_ptr<DXBuffer&>, const std::shared_ptr<DXBuffer&>>> vbIbs)
{

	// todo
}

ComPtr<ID3D11InputLayout> DXDevice::createInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, const std::string& shaderData)
{
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;

	HRCHECK(m_core.getDevice()->CreateInputLayout(elements.data(), static_cast<unsigned int>(elements.size()), shaderData.data(), static_cast<unsigned int>(shaderData.size()), inputLayout.GetAddressOf()));

	return inputLayout;
}




void DXDevice::clearScreen()
{
	FLOAT color[] = { 0.0, 0.0, 0.0, 1.0 };
	m_core.getImmediateContext()->ClearRenderTargetView(m_core.getBackbufferRTV().Get(), color);
}

void DXDevice::present()
{
	m_core.getSwapChain()->Present(0, 0);
}
