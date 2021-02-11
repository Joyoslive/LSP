#pragma once
#include "DXResource.h"

class DXDevice;

class DXTexture : public DXResource
{
public:
	friend DXDevice;
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
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;

	Desc m_desc;

public:
	DXTexture(std::shared_ptr<DXCore> core, Microsoft::WRL::ComPtr<ID3D11Resource> texture, const DXTexture::Desc& desc);
	~DXTexture() = default;

	const DXTexture::Desc& getDesc();

	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRTV();
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDSV();

	void setRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv);
	void setDSV(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv);

};
