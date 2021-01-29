#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXColors.h>
#include <wrl/client.h>


static void HRCHECK(HRESULT hr)
{
	assert(SUCCEEDED(hr));
}


class DXCore
{
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immediateContext;
	Microsoft::WRL::ComPtr<ID3D11Debug> m_debug;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backbufferTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backbufferRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backbufferSRV;

	D3D11_VIEWPORT m_backbufferVP;

	HWND m_hwnd;
	UINT m_clientWidth, m_clientHeight;

private:
	void createDeviceAndSwapChain();

public:
	DXCore(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~DXCore();

	Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain();
	Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getImmediateContext();

	Microsoft::WRL::ComPtr<ID3D11Debug> getDebug();

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getBackbufferRTV();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getBackbufferSRV();

	D3D11_VIEWPORT* getBackBufferViewport();

	UINT getClientWidth();
	UINT getClientHeight();
};

