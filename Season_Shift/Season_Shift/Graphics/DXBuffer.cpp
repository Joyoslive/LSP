
#include "DXBuffer.h"

DXBuffer::DXBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, const D3D11_BUFFER_DESC& desc, DXBuffer::Type type, unsigned int elementCount, unsigned int elementStride) :
    m_buffer(buffer),
    m_desc(desc),
    m_type(type),
    m_elementCount(elementCount),
    m_elementStride(elementStride),
    m_totalSize(elementCount * elementStride)
{
}

DXBuffer::~DXBuffer()
{
}

unsigned int DXBuffer::getElementCount() const
{
    return m_elementCount;
}

unsigned int DXBuffer::getElementStride() const
{
    return m_elementStride;
}

unsigned int DXBuffer::getSize() const
{
    return m_totalSize;
}

const DXBuffer::Type& DXBuffer::getType() const
{
    return m_type;
}

const D3D11_BUFFER_DESC& DXBuffer::getDesc() const
{
    return m_desc;
}

const Microsoft::WRL::ComPtr<ID3D11Buffer>& DXBuffer::getBuffer()
{
    return m_buffer;
}
