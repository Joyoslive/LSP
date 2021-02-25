#include "pch.h"
#include "../Camera.h"
#include "LineDrawer.h"

using DirectX::SimpleMath::Vector3;

LineDrawer::LineDrawer(std::shared_ptr<GfxRenderer> renderer) :
	m_renderer(renderer)
{
	auto dev = renderer->getDXDevice();

	// Setup dynamic vertex buffer
	m_pointsVB = dev->createVertexBuffer(2, sizeof(Vector3), true, true, false, nullptr);

	m_camInfoCB = dev->createConstantBuffer(sizeof(CamInfo), true, true);

	// Setup Shaders
	m_vs = dev->createShader("LineVS.cso", DXShader::Type::VS);
	m_gs = dev->createShader("LineGS.cso", DXShader::Type::GS);
	m_ps = dev->createShader("LinePS.cso", DXShader::Type::PS);

	// Setup input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDescs;
	ilDescs.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	m_il = dev->createInputLayout(ilDescs, m_vs->getShaderData());
	
}

LineDrawer::~LineDrawer()
{
}

void LineDrawer::draw(const std::shared_ptr<Camera>& cam)
{
	auto dev = m_renderer->getDXDevice();
	/*
	
	Update Vertex Buffer (Dynamic VB)

	Set VB
	Set VS
	Set InputLayout
	Set GS
	Set PS

	Draw(2)

	Unbind States

	*/

	setPoints(Vector3(0., 0., 0.), Vector3(0., 0., 1.));

	CamInfo camInfo = { cam->getViewMatrix(), cam->getProjectionMatrix() };
	m_camInfoCB->updateMapUnmap(&camInfo, sizeof(camInfo));
	m_pointsVB->updateMapUnmap(&m_linePoints, sizeof(m_linePoints));

	dev->bindShaderConstantBuffer(DXShader::Type::GS, 0, m_camInfoCB);
	dev->bindDrawBuffer(m_pointsVB);
	dev->bindShader(m_vs, DXShader::Type::VS);
	dev->bindInputLayout(m_il);
	dev->bindShader(m_gs, DXShader::Type::GS);
	dev->bindShader(m_ps, DXShader::Type::PS);

	dev->bindInputTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	dev->draw(2, 0);


}

void LineDrawer::setPoints(const DirectX::SimpleMath::Vector3& worldP0, const DirectX::SimpleMath::Vector3& worldP1)
{
	m_linePoints.currStartPos = worldP0;
	m_linePoints.currEndPos = worldP1;
}
