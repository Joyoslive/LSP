#include "DXTexture.h"



DXTexture::DXTexture(std::shared_ptr<DXCore> core, Microsoft::WRL::ComPtr<ID3D11Resource> texture, const DXTexture::Desc& desc) :
    DXResource(core, texture),
    m_desc(desc),
	m_rtv(nullptr),
    m_dsv(nullptr)
{
}

const DXTexture::Desc& DXTexture::getDesc()
{
    return m_desc;
}

const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& DXTexture::getRTV()
{
    return m_rtv;
}

const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DXTexture::getDSV()
{
    return m_dsv;
}

void DXTexture::setRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv)
{
    if (m_rtv != nullptr) assert(false);
    m_rtv = rtv;
}

void DXTexture::setDSV(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv)
{
    if (m_dsv != nullptr) assert(false);
    m_dsv = dsv;
}
