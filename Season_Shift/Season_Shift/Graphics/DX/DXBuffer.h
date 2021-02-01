#pragma once
#include "DXCore.h"

/*
Light wrapper for a DX11 Buffer
*/
class DXBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;

	D3D11_BUFFER_DESC m_desc;

public:
	DXBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer, const D3D11_BUFFER_DESC& desc);
	~DXBuffer();

	const D3D11_BUFFER_DESC& getDesc() const;

	const Microsoft::WRL::ComPtr<ID3D11Buffer>& getBuffer() const;
	
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getSRV();
	const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& getUAV();

	void setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);
};

