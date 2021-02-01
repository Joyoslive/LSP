
#include "DXBuffer.h"

DXBuffer::DXBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, const D3D11_BUFFER_DESC& desc, DXBuffer::Type type) :
    m_buffer(buffer),
    m_desc(desc),
    m_type(type),
    m_srv(nullptr),
    m_uav(nullptr)
{
}

DXBuffer::~DXBuffer()
{
}

const DXBuffer::Type DXBuffer::getType() const
{
    return m_type;
}

const D3D11_BUFFER_DESC& DXBuffer::getDesc() const
{
    return m_desc;
}

const Microsoft::WRL::ComPtr<ID3D11Buffer>& DXBuffer::getBuffer() const
{
    return m_buffer;
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& DXBuffer::getSRV()
{
    return m_srv;
}

const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& DXBuffer::getUAV()
{
    return m_uav;
}

void DXBuffer::setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
    if (m_srv != nullptr) assert(false);
    m_srv = srv;

}

void DXBuffer::setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
{
    if (m_uav != nullptr) assert(false);
    m_uav = uav;
}
