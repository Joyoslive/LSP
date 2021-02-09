#pragma once
#include "DXResource.h"

/*
Light wrapper for a DX11 Buffer
*/
class DXBuffer : public DXResource
{
public:
	enum class Type
	{
		Vertex,
		Index,
		Constant,
		Structured
	};

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;
	
	unsigned int m_totalSize;
	unsigned int m_elementStride;
	unsigned int m_elementCount;
	Type m_type;
	D3D11_BUFFER_DESC m_desc;

public:
	DXBuffer(std::shared_ptr<DXCore> core, Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, const D3D11_BUFFER_DESC& desc, Type type, unsigned int elementCount, unsigned int elementStride);
	~DXBuffer();

	const DXBuffer::Type getType() const;
	const D3D11_BUFFER_DESC& getDesc() const;

	const unsigned int& getElementCount() const;
	const unsigned int& getElementStride() const;
	const unsigned int& getTotalSize() const;

};

