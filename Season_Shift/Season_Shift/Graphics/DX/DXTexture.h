#pragma once
#include "DXCore.h"

class DXTexture
{
public:
	enum class Type
	{
		TEX1D,
		TEX2D,
		TEX3D
	};


	struct Desc
	{
		Type type;

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
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;

	Desc m_desc;

public:
	DXTexture(Microsoft::WRL::ComPtr<ID3D11Resource> texture, const DXTexture::Desc& desc);
	~DXTexture();

	const DXTexture::Desc& getDesc();

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getSRV();
	const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& getUAV();
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRTV();
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDSV();

	void setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);
	void setRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv);
	void setDSV(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv);

};
