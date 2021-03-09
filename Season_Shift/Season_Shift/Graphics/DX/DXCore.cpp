#include "pch.h"
#include "DXCore.h"
#include "../../Logger.h"


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

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui_ImplDX11_Init(m_device.Get(), m_immediateContext.Get());
	ImGui::StyleColorsDark();


#if _DEBUG
	HRCHECK(m_device.Get()->QueryInterface<ID3D11Debug>(m_debug.GetAddressOf()));
#endif

	m_shouldBeFullScreen = false;
	HRCHECK(m_swapChain.Get()->GetFullscreenState(&m_isFullScreen, nullptr));
	assert(m_shouldBeFullScreen == m_isFullScreen); //should not start in fullscreen


	IDXGIFactory* factory = nullptr;
	HRCHECK(m_swapChain->GetParent(__uuidof(IDXGIFactory), (void**)&factory));
	factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	factory->Release();

	checkMonitorRes();
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

void DXCore::checkMonitorRes()
{
	m_maxHeight = 0;
	m_maxWidth = 0;

	IDXGIOutput* outPut = nullptr;
	HRESULT hr = this->m_swapChain->GetContainingOutput(&outPut);
	assert(SUCCEEDED(hr));


	DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;

	UINT numModes = 0;
	hr = outPut->GetDisplayModeList(format, 0, &numModes, 0);
	assert(SUCCEEDED(hr));
	DXGI_MODE_DESC* modeList = new DXGI_MODE_DESC[numModes];
	hr = outPut->GetDisplayModeList(format, 0, &numModes, modeList);
	assert(SUCCEEDED(hr));

	Logger log = Logger::getLogger();
	log.setFile("modeList.txt");
	for (int i = 0; i < numModes; i++)
	{
		if (modeList[i].Width >= m_maxWidth) m_maxWidth = modeList[i].Width;
		if (modeList[i].Height >= m_maxHeight) m_maxHeight = modeList[i].Height;


		log.addLog("width:\t\t" + std::to_string(modeList[i].Width) + "\n");
		log.addLog("height:\t\t" + std::to_string(modeList[i].Height) + "\n");
		log.addLog("RefreshRate:\t" + std::to_string((float)modeList[i].RefreshRate.Numerator / (float)modeList->RefreshRate.Denominator) + "\n");
		log.addLog("Scaling:\t\t" + std::to_string(modeList[i].Scaling) + "\n");
		log.addLog("ScanlineOrdering:\t" + std::to_string(modeList[i].ScanlineOrdering) + "\n");
		log.addLog("Format:\t\t" + std::to_string(modeList[i].Format) + "\n\n");
	}

	log.dumpLogs();
	delete[] modeList;
	outPut->Release();

	Logger::getLogger().debugLog("Monitor resolution: " + std::to_string(m_maxWidth) + "x" + std::to_string(m_maxHeight) + "\n");
}

bool DXCore::setFullScreen(BOOL fullScreen)
{

	HRCHECK(m_swapChain.Get()->GetFullscreenState(&m_isFullScreen, nullptr));
	if (m_isFullScreen != m_shouldBeFullScreen) //check if fullscreen has changed, ignore input argument
	{
		fullScreen = m_isFullScreen;
	}
	else if (fullScreen == m_isFullScreen) //early exit if fullscreen is already at a desired state 
	{
		return false;
	}
	
	Logger::getLogger().debugLog("setFullScreen to ");
	Logger::getLogger().debugLog(fullScreen ? "true\n" : "false\n");
	if (fullScreen)
	{
		resizeTarget(m_maxWidth, m_maxHeight);
	}
	else
	{
		HRCHECK(m_swapChain->SetFullscreenState(false, nullptr)); //force it to accept resize
		resizeTarget(1280, 720);
	}

	HRCHECK(m_swapChain->SetFullscreenState(fullScreen, nullptr));
	m_shouldBeFullScreen = fullScreen;
	m_isFullScreen = fullScreen;
	return true;
}

bool DXCore::getFullScreenState() const
{
	assert(m_shouldBeFullScreen == m_isFullScreen);
	return m_shouldBeFullScreen;
}

void DXCore::resizeTarget(UINT width, UINT height)
{
	DXGI_MODE_DESC modeDesc = {};

	DXGI_MODE_DESC prefModeDesc = {};

	prefModeDesc.Width = width;
	prefModeDesc.Height = height;
	prefModeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	prefModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;

	IDXGIOutput* outPut = nullptr;
	HRCHECK(m_swapChain->GetContainingOutput(&outPut));

	HRCHECK(outPut->FindClosestMatchingMode(&prefModeDesc, &modeDesc, m_device.Get()));
	Logger::getLogger().debugLog("FindClosestMatchingMode:\tResolution: " + std::to_string(modeDesc.Width) + "x" + std::to_string(modeDesc.Height) +
		"\tRefreshRate: " + std::to_string((float)modeDesc.RefreshRate.Numerator / (float)modeDesc.RefreshRate.Denominator) + "\n");


	HRCHECK(m_swapChain->ResizeTarget(&modeDesc));
	outPut->Release();
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
	

	bool changed = setFullScreen(m_shouldBeFullScreen); //should not matter if argument is true or false, setFullScreen() will figure it out
	if (changed && m_shouldBeFullScreen)
	{
		width = m_maxWidth;
		height = m_maxHeight;
	}

	m_clientHeight = height;
	m_clientWidth = width;

	Logger::getLogger().debugLog("onResize: " + std::to_string(width) + "x" + std::to_string(height) + "\n");

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


	//get desplay modes

	/*IDXGIOutput* outPut = nullptr;
	hr = this->m_swapChain->GetContainingOutput(&outPut);
	assert(SUCCEEDED(hr)); */


	/*DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
	
	UINT numModes = 0;
	hr = outPut->GetDisplayModeList(format, 0, &numModes, 0);
	assert(SUCCEEDED(hr));
	DXGI_MODE_DESC* modeList = new DXGI_MODE_DESC[numModes];
	hr = outPut->GetDisplayModeList(format, 0, &numModes, modeList);
	assert(SUCCEEDED(hr));
	Logger log = Logger::getLogger();
	log.setFile("modeList.txt");
	for (int i = 0; i < numModes; i++)
	{
		log.addLog("width:\t\t" + std::to_string(modeList[i].Width) + "\n");
		log.addLog("height:\t\t" + std::to_string(modeList[i].Height) + "\n");
		log.addLog("RefreshRate:\t" + std::to_string((float)modeList[i].RefreshRate.Numerator/(float)modeList->RefreshRate.Denominator) + "\n");
		log.addLog("Scaling:\t\t" + std::to_string(modeList[i].Scaling) + "\n");
		log.addLog("ScanlineOrdering:\t" + std::to_string(modeList[i].ScanlineOrdering) + "\n");
		log.addLog("Format:\t\t" + std::to_string(modeList[i].Format) + "\n\n");
		
	}
	log.dumpLogs();
	delete[] modeList;*/




	//något annat
	/*DXGI_MODE_DESC modeDesc = {};

	DXGI_MODE_DESC prefModeDesc = {};

	prefModeDesc.Width = width;
	prefModeDesc.Height = height;
	prefModeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	prefModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	hr = outPut->FindClosestMatchingMode(&prefModeDesc, &modeDesc, this->m_device.Get());
	assert(SUCCEEDED(hr));


	BOOL fullscreen;
	hr = this->m_swapChain.Get()->GetFullscreenState(&fullscreen, nullptr);
	assert(SUCCEEDED(hr));*/
}

