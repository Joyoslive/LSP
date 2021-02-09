#pragma once
#include "DXCore.h"

class DXResource
{
protected:
	std::shared_ptr<DXCore> m_core;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_resource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;

public:
	DXResource(std::shared_ptr<DXCore> core, Microsoft::WRL::ComPtr<ID3D11Resource> resource);
	~DXResource() = default;

	void updateMapUnmap(void* data, unsigned int dataSize, unsigned int subresIdx = 0, D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD, unsigned int mapFlags = 0);
	void updateSubresource(void* data, unsigned int dstSubresIdx = 0, D3D11_BOX* box = nullptr, unsigned int srcRowPitch = 0, unsigned int srcDepthPitch = 0);

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getSRV();
	const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& getUAV();
	const Microsoft::WRL::ComPtr<ID3D11Resource>& getResource();

	void setSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void setUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav);

};

