#include "pch.h"
#include "DXBuffer.h"

DXBuffer::DXBuffer(std::shared_ptr<DXCore> core, Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, const D3D11_BUFFER_DESC& desc, DXBuffer::Type type, unsigned int elementCount, unsigned int elementStride) :
    DXResource(core, buffer),
    m_desc(desc),
    m_type(type),
    m_srv(nullptr),
    m_uav(nullptr),
    m_elementCount(elementCount),
    m_elementStride(elementStride),
    m_totalSize(elementCount * elementStride)
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

const unsigned int& DXBuffer::getElementCount() const
{
    return m_elementCount;
}

const unsigned int& DXBuffer::getElementStride() const
{
    return m_elementStride;
}

const unsigned int& DXBuffer::getTotalSize() const
{
    return m_totalSize;
}