#include "Graphics.h"

Graphics::Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight)
{
	// API
	m_dxDev = std::make_shared<DXDevice>(hwnd, clientWidth, clientHeight);
	
	/* 
	Utilize the DirectX API in various components
	*/
	m_renderer = std::make_shared<GfxRenderer>(m_dxDev);
	m_currRenderStrat = std::make_unique<ForwardRenderStrategy>(m_renderer);

}

Graphics::~Graphics()
{

}

void Graphics::render()		
{
	// view frustum culling is done outside this! --> Makes use of the bounding box and camera, non-graphical components.
	
	m_currRenderStrat->render();

}

std::shared_ptr<Mesh> Graphics::createMesh(const std::string& meshID, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	return m_gfxDevice->createMesh(meshID, vertices, indices);
}

std::shared_ptr<Model> Graphics::assembleModel(const std::string& meshID, std::shared_ptr<Material> material)
{
	// create mesh from vertices and indices
	//auto mesh = m_gfxDevice->createMesh(vertices, indices);

	// here we can check if model already exists somehow before creating..
	//std::shared_ptr<Model> mod = std::make_shared<Model>(mesh, material);

	//return mod;
	return nullptr;
}
