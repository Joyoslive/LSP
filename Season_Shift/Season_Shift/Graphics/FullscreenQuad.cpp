#include "FullscreenQuad.h"

using namespace DirectX::SimpleMath;

FullscreenQuad::FullscreenQuad(DXDevice* device)
{
	std::vector<Vertex> vertices;
	vertices.push_back({Vector3(-1, 1, 0.0), Vector2(0.0, 0.0), Vector3(0.0, 0.0, -1.0)});
	vertices.push_back({Vector3(1, 1, 0.0), Vector2(1.0, 0.0), Vector3(0.0, 0.0, -1.0)});
	vertices.push_back({Vector3(1, -1, 0.0), Vector2(1.0, 1.0), Vector3(0.0, 0.0, -1.0)});
	vertices.push_back({Vector3(-1, -1, 0.0), Vector2(0.0, 1.0), Vector3(0.0, 0.0, -1.0)});

	std::vector<uint32_t> indices = {
		0, 1, 2,
		0, 2, 3
	};

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices.data();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();

	m_vertexBuffer = device->createVertexBuffer(vertices.size(), sizeof(Vertex), false, false, false, &vertexData);
	m_indexBuffer = device->createIndexBuffer(indices.size() * sizeof(uint32_t), false, &indexData);
	
	m_mesh = std::make_shared<Mesh>(m_vertexBuffer, m_indexBuffer);
}

FullscreenQuad::~FullscreenQuad()
{

}

const std::shared_ptr<DXBuffer>& FullscreenQuad::getVB() const
{
	return m_vertexBuffer;
}

const std::shared_ptr<DXBuffer>& FullscreenQuad::getIB() const
{
	return m_indexBuffer;
}
