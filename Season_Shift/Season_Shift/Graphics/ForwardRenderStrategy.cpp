#include "ForwardRenderStrategy.h"

using namespace DirectX::SimpleMath;

ForwardRenderStrategy::ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer)
{
	OutputDebugStringW(L"Using forward render.\n");


	auto dev = renderer->getDXDevice();

	// Load triangle
	std::vector<Vertex> verts;
	verts.push_back({ Vector3(-0.75, 0.75, 0.0), Vector2(0.0, 0.0), Vector3(0.0, 0.0, -1.0) });
	verts.push_back({ Vector3(0.75, 0.75, 0.0), Vector2(1.0, 0.0), Vector3(0.0, 0.0, -1.0) });
	verts.push_back({ Vector3(0.75, -0.75, 0.0), Vector2(0.0, 1.0), Vector3(0.0, 0.0, -1.0) });
	verts.push_back({ Vector3(-0.75, -0.75, 0.0), Vector2(0.0, 1.0), Vector3(0.0, 0.0, -1.0) });

	D3D11_SUBRESOURCE_DATA subres; 
	ZeroMemory(&subres, sizeof(D3D11_SUBRESOURCE_DATA));
	subres.pSysMem = verts.data();
	vb = dev->createVertexBuffer(verts.size(), sizeof(Vertex), false, false, false, &subres);

	std::vector<uint32_t> vertIndices = { 
		0, 1, 2, 
		0, 2, 3 
	};
		
	subres.pSysMem = vertIndices.data();
	ib = dev->createIndexBuffer(vertIndices.size() * sizeof(uint32_t), false, &subres);

	// Load shaders
	vs = dev->createShader("DefaultVS.cso", DXShader::Type::VS);		// temp
	//ps = dev->createShader("DefaultPS.cso", DXShader::Type::PS);

	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
	ilDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });



	il = dev->createInputLayout(ilDesc, vs->getShaderData());

	// Create some sampler

	D3D11_SAMPLER_DESC sDesc = { };
	sDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.MipLODBias = 0;
	sDesc.MaxAnisotropy = 1;	// not used
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRCHECK(m_renderer->getDXDevice()->getDevice()->CreateSamplerState(&sDesc, m_sampler.GetAddressOf()));

	
	// Create a matrix buffer

	matrixBuffer = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 3, true, true);

	// Binds
	//dev->bindDrawBuffer(vb);
	//dev->bindDrawIndexedBuffer(vb, ib, 0, 0);

	dev->bindInputLayout(il);
	dev->bindInputTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dev->bindShaderSampler(DXShader::Type::PS, 0, m_sampler);

	//dev->bindShader(vs, DXShader::Type::VS);
	//dev->bindShader(ps, DXShader::Type::PS);

	m_tmpBuf = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 3, true, true);

	DXTexture::Desc depthDesc;
	depthDesc.desc2D = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, dev->getClientWidth(), dev->getClientHeight(), 1, 1, D3D11_BIND_DEPTH_STENCIL);
	depthDesc.type = DXTexture::Type::TEX2D;
	m_depthTexture = dev->createTexture(depthDesc, nullptr);

	//currDS_ = gphDev_->createTexture2D(
	//	"DSText1",
	//	§CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, gphDev_->getClientWidth(), gphDev_->getClientHeight(), 1, 1, D3D11_BIND_DEPTH_STENCIL),
	//	nullptr
	//);
	//D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc = { };
	//dsDesc.Format = currDS_->getFormat();
	//dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsDesc.Flags = 0;
	//dsDesc.Texture2D.MipSlice = 0;
	//gphDev_->createDSV(currDS_, dsDesc);


	/*
	
	Create the neccessary pipeline state, buffers and textures to allow for basic flat-shaded rendering of different meshes.

	MeshCBuffer
		wm
		vm
		pj

	PointSampler

	VS, PS

	InputLayout

	DSV

	Default Texture

	*/
}

ForwardRenderStrategy::~ForwardRenderStrategy()
{
}

void ForwardRenderStrategy::render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera)
{
	auto dev = m_renderer->getDXDevice();

	dev->clearScreen();

	// Do lots of rendering stuff with the help of Renderer and various Binder/Technique abstractions in mind! etc.
	
	dev->clearDepthTarget(m_depthTexture);

	dev->bindBackBufferAsTarget(m_depthTexture);


	DirectX::XMMATRIX matrices[3] = {{}, mainCamera->getViewMatrix(), mainCamera->getProjectionMatrix()};

	for (auto& mod : models)
	{
		for (auto& mat : mod->getSubsetsMaterial())
		{
			mat.material->bindShader(dev);
			mat.material->bindTextures(dev);

			matrices[0] = mod->getTransform()->getWorldMatrix();//DirectX::XMMatrixIdentity();

			//dev->updateResourcesMapUnmap(matrixBuffer->getResource().Get(), matrices, sizeof(matrices));
			matrixBuffer->updateMapUnmap(matrices, sizeof(matrices), 0, D3D11_MAP_WRITE_DISCARD, 0);

			m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::VS, 0, matrixBuffer);

			dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
			dev->drawIndexed(mat.indexCount, mat.indexStart, mat.vertexStart);

			//mod->getMaterial()->bindShader(m_renderer->getDXDevice());
			//mod->getMaterial()->bindTextures(m_renderer->getDXDevice());

			//dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
			//dev->drawIndexed(mod->getMesh()->getIB()->getElementCount(), 0, 0);
		}
	}




	dev->present();
}
