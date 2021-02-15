#include "DXCore.h"



DXCore::DXCore(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_hwnd(hwnd),
    m_swapChain(nullptr),
    m_device(nullptr),
    m_immediateContext(nullptr),
    m_debug(nullptr),
    m_backbufferTexture(nullptr),
    m_backbufferRTV(nullptr),
    m_backbufferSRV(nullptr),
    m_clientWidth(clientWidth),
    m_clientHeight(clientHeight),
    m_backbufferVP({ 0.0, 0.0, (FLOAT)clientWidth, (FLOAT)clientHeight, 0.0, 1.0})
{
    createDeviceAndSwapChain();

	// Get backbuffer texture
	HRCHECK(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_backbufferTexture.GetAddressOf()));

	// Create RTV for backbuffer
	HRCHECK(m_device->CreateRenderTargetView(m_backbufferTexture.Get(), NULL, m_backbufferRTV.GetAddressOf()));

	// Create SRV for backbuffer
	HRCHECK(m_device->CreateShaderResourceView(m_backbufferTexture.Get(), NULL, m_backbufferSRV.GetAddressOf()));


#if _DEBUG
	HRCHECK(m_device.Get()->QueryInterface<ID3D11Debug>(m_debug.GetAddressOf()));
#endif

}

DXCore::~DXCore()
{
	m_immediateContext->ClearState();
}

void DXCore::createDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	scDesc.BufferDesc.Width = m_clientWidth;
	scDesc.BufferDesc.Height = m_clientHeight;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//scDesc.BufferDesc.RefreshRate.Numerator = 0;	
	//scDesc.BufferDesc.RefreshRate.Denominator = 0;
	//scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scDesc.SampleDesc.Count = 1;		// No multisamples
	//scDesc.SampleDesc.Quality = 0;

	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	scDesc.BufferCount = 2;		// Front buffer typically included in this value (MSDN)

	scDesc.OutputWindow = m_hwnd;
	scDesc.Windowed = TRUE;

	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// Enable app to use SC ResizeTarget


	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#if _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRCHECK(D3D11CreateDeviceAndSwapChain(
		NULL,						// Default video adapter
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,						// No software
		flags,						// Create flags
		NULL,						// Features targeted by this device, default: 11.0, 10.1, 10, 9.3, 9.2, 9.1 (6)
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(),
		NULL,	// we dont need to keep track of which feature level is supported right now
		m_immediateContext.GetAddressOf()
	));
}

const Microsoft::WRL::ComPtr<IDXGISwapChain>& DXCore::getSwapChain()
{
    return m_swapChain;
}

const Microsoft::WRL::ComPtr<ID3D11Device>& DXCore::getDevice()
{
    return m_device;
}

const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& DXCore::getImmediateContext()
{
    return m_immediateContext;
}

const Microsoft::WRL::ComPtr<ID3D11Debug>& DXCore::getDebug()
{
    return m_debug;
}

const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& DXCore::getBackbufferRTV()
{
    return m_backbufferRTV;
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& DXCore::getBackbufferSRV()
{
    return m_backbufferSRV;
}

const Microsoft::WRL::ComPtr<ID3D11Texture2D>& DXCore::getBackbufferTexture()
{
	return m_backbufferTexture;
}

D3D11_VIEWPORT* DXCore::getBackBufferViewport()
{
    return &m_backbufferVP;
}

UINT DXCore::getClientWidth()
{
    return m_clientWidth;
}

UINT DXCore::getClientHeight()
{
    return m_clientHeight;
}

void DXCore::changeResolution(unsigned int clientWidth, unsigned int clientHeight)
{
	/*
	To implement
	*/
}

void DXCore::onResize(UINT width, UINT height)
{
	m_backbufferRTV.Reset();
	m_backbufferSRV.Reset();
	m_backbufferTexture.Reset();

	HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	assert(SUCCEEDED(hr));


	//from constuctor

	// Get backbuffer texture
	HRCHECK(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_backbufferTexture.GetAddressOf()));

	// Create RTV for backbuffer
	HRCHECK(m_device->CreateRenderTargetView(m_backbufferTexture.Get(), NULL, m_backbufferRTV.GetAddressOf()));

	// Create SRV for backbuffer
	HRCHECK(m_device->CreateShaderResourceView(m_backbufferTexture.Get(), NULL, m_backbufferSRV.GetAddressOf()));
}

