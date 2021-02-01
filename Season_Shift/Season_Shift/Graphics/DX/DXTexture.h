#pragma once
#include "DXCore.h"

class DXTexture
{
public:
	struct Desc
	{
		enum class Type
		{
			TEX1D,
			TEX2D,
			TEX3D
		} type = Type::TEX2D;

		union
		{
			D3D11_TEXTURE1D_DESC desc1D;
			D3D11_TEXTURE2D_DESC desc2D;
			D3D11_TEXTURE3D_DESC desc3D;
		};
	};



private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

	DXTexture::Desc m_desc;

public:
	DXTexture(Microsoft::WRL::ComPtr<ID3D11Resource> texture, const DXTexture::Desc& desc);
	~DXTexture();

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getSRV();
	const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& getUAV();
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRTV();

	void setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);
	void setRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv);

};
