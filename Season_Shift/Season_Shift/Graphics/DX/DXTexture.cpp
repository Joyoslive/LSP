#include "DXTexture.h"

DXTexture::DXTexture(Microsoft::WRL::ComPtr<ID3D11Resource> texture, const DXTexture::Desc& desc) :
	m_texture(nullptr),
	m_srv(nullptr),
	m_uav(nullptr),
	m_rtv(nullptr)
{
}

DXTexture::~DXTexture()
{
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& DXTexture::getSRV()
{
    return m_srv;
}

const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& DXTexture::getUAV()
{
    return m_uav;
}

const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& DXTexture::getRTV()
{
    return m_rtv;
}

void DXTexture::setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
    if (m_srv != nullptr) assert(false);
    m_srv = srv;

}

void DXTexture::setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
{
    if (m_uav != nullptr) assert(false);
    m_uav = uav;
}

void DXTexture::setRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv)
{
    if (m_rtv != nullptr) assert(false);
    m_rtv = rtv;
}