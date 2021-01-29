#pragma once
#include "DXCore.h"

/*
Light wrapper for a DX11 Buffer
*/
class DXBuffer
{
public:
	enum class Type
	{
		VERTEX,
		INDEX,
		CONSTANT,
		STRUCTURED,
		APPEND_CONSUME,
		NONE/*,
		RAW,
		INDIRECTARGS*/
	};

private:
	DXBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, const D3D11_BUFFER_DESC& desc, DXBuffer::Type type, unsigned int elementCount, unsigned int elementStride);
	~DXBuffer();

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

	D3D11_BUFFER_DESC m_desc;
	DXBuffer::Type m_type;
	unsigned int m_totalSize;
	unsigned int m_elementCount;
	unsigned int m_elementStride;

public:
	unsigned int getElementCount() const;
	unsigned int getElementStride() const;
	unsigned int getSize() const;

	const DXBuffer::Type& getType() const;
	const D3D11_BUFFER_DESC& getDesc() const;

	/*
	Non-incremented pointer: Expresses non-ownership (Special case here for COM)
	*/
	const Microsoft::WRL::ComPtr<ID3D11Buffer>& getBuffer();
	
	//const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getSRV();
	//const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& getUAV();


};

