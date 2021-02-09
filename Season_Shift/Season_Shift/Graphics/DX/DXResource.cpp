#include "DXResource.h"


DXResource::DXResource(std::shared_ptr<DXCore> core, Microsoft::WRL::ComPtr<ID3D11Resource> resource) :
    m_core(core),
    m_resource(resource),
    m_srv(nullptr),
    m_uav(nullptr)
{
}

void DXResource::updateMapUnmap(void* data, unsigned int dataSize, unsigned int subresIdx, D3D11_MAP mapType, unsigned int mapFlags)
{
    if (m_resource == nullptr)    assert(false);

    D3D11_MAPPED_SUBRESOURCE subres;

    HRCHECK(m_core->getImmediateContext()->Map(m_resource.Get(), subresIdx, mapType, mapFlags, &subres));

    std::memcpy(subres.pData, data, dataSize);

    m_core->getImmediateContext()->Unmap(m_resource.Get(), 0);
}

void DXResource::updateSubresource(void* data, unsigned int dstSubresIdx, D3D11_BOX* box, unsigned int srcRowPitch, unsigned int srcDepthPitch)
{
    if (m_resource == nullptr)    assert(false);

    m_core->getImmediateContext()->UpdateSubresource(m_resource.Get(), dstSubresIdx, box, data, srcRowPitch, srcDepthPitch);
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& DXResource::getSRV()
{
    return m_srv;
}

const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& DXResource::getUAV()
{
    return m_uav;
}

const Microsoft::WRL::ComPtr<ID3D11Resource>& DXResource::getResource()
{
    return m_resource;
}

void DXResource::setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
    if (m_srv != nullptr) assert(false);
    m_srv = srv;
}

void DXResource::setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
{
    if (m_uav != nullptr) assert(false);
    m_uav = uav;
}
