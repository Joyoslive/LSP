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
	vs = dev->createShader("DefaultVS.cso", DXShader::Type::VS);
	ps = dev->createShader("DefaultPS.cso", DXShader::Type::PS);

	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
	ilDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	il = dev->createInputLayout(ilDesc, vs->getShaderData());


	// Binds
	//dev->bindDrawBuffer(vb);
	dev->bindDrawIndexedBuffer(vb, ib, 0, 0);

	dev->bindInputLayout(il);
	dev->bindInputTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dev->bindShader(vs, DXShader::Type::VS);
	dev->bindShader(ps, DXShader::Type::PS);


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

void ForwardRenderStrategy::render()
{
	auto dev = m_renderer->getDXDevice();

	dev->clearScreen();

	// Do lots of rendering stuff with the help of Renderer and various Binder/Technique abstractions in mind! etc.
	
	dev->bindBackBufferAsTarget();


	dev->DrawIndexed(6, 0, 0);


	/*

	m_renderer->setPipelineState(defaultForward);
	
	for (auto& go : gameObjects)
	{
		auto& mesh = go.mesh;			--> vb, ib
		auto& material = go.material;	--> ps textures + shader

		m_renderer->drawMesh(mesh, material);
		
	}
	
	*/



	dev->present();
}
