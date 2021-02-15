#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <memory>
#include <DirectXColors.h>
#include <wrl/client.h>


static inline void HRCHECK(HRESULT hr)
{
	assert(SUCCEEDED(hr));
}

/*

Works as a 'thin wrapper' around DirectX 11 and important core initialization.
User of this class is expected to use the getters to directly make use of the internal device/device context

*/
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

	const Microsoft::WRL::ComPtr<IDXGISwapChain>& getSwapChain();
	const Microsoft::WRL::ComPtr<ID3D11Device>& getDevice();
	const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& getImmediateContext();

	const Microsoft::WRL::ComPtr<ID3D11Debug>& getDebug();

	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getBackbufferRTV();
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getBackbufferSRV();
	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& getBackbufferTexture();

	/*
	Pointer for ease-of-access when binding viewports
	*/
	D3D11_VIEWPORT* getBackBufferViewport();

	UINT getClientWidth();
	UINT getClientHeight();
	
	void changeResolution(unsigned int clientWidth, unsigned int clientHeight);
	void onResize(UINT width, UINT height);
};

