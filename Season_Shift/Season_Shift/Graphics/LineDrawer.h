#pragma once
#include <memory>
#include "DX/DXDevice.h"

class Camera;

class LineDrawer
{
private:
	std::shared_ptr<DXBuffer> m_pointVB;		// dynamic vb

	DirectX::SimpleMath::Vector2 m_currStartPos;
	DirectX::SimpleMath::Vector2 m_currEndPos;

	// Specific line shader
	std::shared_ptr<DXShader> m_vs;
	std::shared_ptr<DXShader> m_gs;
	std::shared_ptr<DXShader> m_ps;

	std::shared_ptr<DXDevice> m_dev;


public:
	LineDrawer(std::shared_ptr<DXDevice> dev);
	~LineDrawer();

	void draw(const std::shared_ptr<Camera>& cam);

	void setPoints(DirectX::SimpleMath::Vector2 worldP0, DirectX::SimpleMath::Vector2 worldP1);



};

