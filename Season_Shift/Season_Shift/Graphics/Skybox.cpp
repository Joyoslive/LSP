#include "Skybox.h"
#include "DX/GraphicsUtils.h"
#include "../../Camera.h"

Skybox::Skybox(std::shared_ptr<GfxRenderer> renderer) :
	m_renderer(renderer)
{
	auto dev = renderer->getDXDevice();

	loadSkybox("Textures/Skyboxes/yokohama");
	m_activeTexture = m_textures[0];

	// Load Skybox VS/PS
	m_vs = dev->createShader("SkyboxVS.cso", DXShader::Type::VS);
	m_ps = dev->createShader("SkyboxPS.cso", DXShader::Type::PS);

	// Set up Rasterizer State
	D3D11_RASTERIZER_DESC rsDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	rsDesc.FrontCounterClockwise = true;	// We are inside cube
	rsDesc.FillMode = D3D11_FILL_SOLID;
	m_rs = dev->createRasterizerState(rsDesc);

	// Set up Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	m_dss = dev->createDepthStencilState(dsDesc);

	// Set up VP buffer
	m_vpBuffer = dev->createConstantBuffer(sizeof(m_vpInfo), true, true);

	// Set up Sampler
	D3D11_SAMPLER_DESC sDesc = { };
	sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.MipLODBias = 0;
	sDesc.MaxAnisotropy = 1;	// not used
	sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_sampler = m_renderer->getDXDevice()->createSamplerState(sDesc);

}

void Skybox::loadSkybox(std::filesystem::path directoryPath)
{
	auto dev = m_renderer->getDXDevice();
	std::string dir(directoryPath.string());

	// Load X,Y,Z (+-) files
	TextureData textData[6] = { };
	textData[0] = loadFileToTexture(dir + "/posx.jpg");
	textData[1] = loadFileToTexture(dir + "/negx.jpg");

	textData[2] = loadFileToTexture(dir + "/posy.jpg");
	textData[3] = loadFileToTexture(dir + "/negy.jpg");

	textData[4] = loadFileToTexture(dir + "/posz.jpg");
	textData[5] = loadFileToTexture(dir + "/negz.jpg");
	
	// Assuming the dimensions are the same!
	// Load X,Y,,Z (+-) data to TextureCube (Release old ptr and create new)
	DXTexture::Desc texCubeDesc = { };
	texCubeDesc.type = DXTexture::Type::TEX2D;
	texCubeDesc.desc2D.Width = textData[0].width;
	texCubeDesc.desc2D.Height = textData[0].height;
	texCubeDesc.desc2D.MipLevels = 1;
	texCubeDesc.desc2D.ArraySize = 6;
	texCubeDesc.desc2D.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	texCubeDesc.desc2D.SampleDesc.Count = 1;
	texCubeDesc.desc2D.SampleDesc.Quality = 0;
	texCubeDesc.desc2D.Usage = D3D11_USAGE_DEFAULT;
	texCubeDesc.desc2D.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texCubeDesc.desc2D.CPUAccessFlags = 0;
	texCubeDesc.desc2D.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// Order important: https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-intro
	D3D11_SUBRESOURCE_DATA subresDat[6] = { };
	for (int i = 0; i < 6; ++i)
	{
		subresDat[i].pSysMem = textData[i].data;
		subresDat[i].SysMemPitch = textData[i].width * sizeof(uint32_t);	// RGBA
		subresDat[i].SysMemSlicePitch = 0;
	}

	m_textures.push_back(dev->createTexture(texCubeDesc, subresDat));

	for (int i = 0; i < 6; ++i)
	{
		free(textData[i].data);
	}
}

void Skybox::draw(const std::shared_ptr<Camera>& cam)
{
	auto dev = m_renderer->getDXDevice();

	// Update buffer with cam info
	m_vpInfo.viewMat = cam->getViewMatrix();
	m_vpInfo.projMat = cam->getProjectionMatrix();
	m_vpBuffer->updateMapUnmap(&m_vpInfo, sizeof(m_vpInfo));

	// Bind
	dev->bindInputLayout(nullptr);
	dev->bindRasterizerState(m_rs);
	dev->bindDepthStencilState(m_dss);
	dev->bindShaderConstantBuffer(DXShader::Type::VS, 5, m_vpBuffer);
	dev->bindShaderSampler(DXShader::Type::PS, 5, m_sampler);
	dev->bindShaderTexture(DXShader::Type::PS, 5, m_activeTexture);
	dev->bindShader(m_vs, DXShader::Type::VS);
	dev->bindShader(m_ps, DXShader::Type::PS);

	// Unbind all draw buffers
	dev->bindDrawIndexedBuffer(nullptr, nullptr, 0, 0);

	// Draw with immediate buffer
	dev->draw(36, 0);

	// Unbind
	dev->bindShader(nullptr, DXShader::Type::VS);
	dev->bindShader(nullptr, DXShader::Type::PS);
	dev->bindRasterizerState(nullptr);
	dev->bindDepthStencilState(nullptr);
}

