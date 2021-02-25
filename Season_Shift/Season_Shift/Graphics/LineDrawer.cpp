#include "pch.h"
#include "LineDrawer.h"

LineDrawer::LineDrawer(std::shared_ptr<DXDevice> dev) :
	m_dev(dev)
{
}

LineDrawer::~LineDrawer()
{
}

void LineDrawer::draw(const std::shared_ptr<Camera>& cam)
{
	/*
	
	Set States

	DrawLine

	Unbind States


	*/


}

void LineDrawer::setPoints(DirectX::SimpleMath::Vector2 worldP0, DirectX::SimpleMath::Vector2 worldP1)
{
	m_currStartPos = worldP0;
	m_currEndPos = worldP1;
}
