#pragma once
#include <memory>
#include "GfxRenderer.h"

class Camera;

class LineDrawer
{
private:
	std::shared_ptr<DXBuffer> m_pointsVB;		// dynamic vb
	std::shared_ptr<DXBuffer> m_lineDrawCB;
	std::shared_ptr<DXBuffer> m_lineSettingsCB;

	struct alignas(16) 
	{
		DirectX::SimpleMath::Vector3 currStartPos;
		DirectX::SimpleMath::Vector3 currEndPos;
	} m_linePoints;

	struct alignas(16) LineDrawInfo
	{
		DirectX::SimpleMath::Matrix viewMat;
		DirectX::SimpleMath::Matrix projMat;
		DirectX::SimpleMath::Vector3 color;
		float thicknessStart;
		DirectX::SimpleMath::Vector3 startOffset;
		float thicknessEnd;

	} m_lineDrawData;

	// Specific line shader
	std::shared_ptr<DXShader> m_vs;
	std::shared_ptr<DXShader> m_gs;
	std::shared_ptr<DXShader> m_ps;

	bool m_shouldRender;
	std::shared_ptr<GfxRenderer> m_renderer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_il;

public:
	LineDrawer(std::shared_ptr<GfxRenderer> renderer);
	~LineDrawer();

	void draw(const std::shared_ptr<Camera>& cam);

	/*
	This should be called every frame!
	*/
	void setPoints(const DirectX::SimpleMath::Vector3& worldP0, const DirectX::SimpleMath::Vector3& worldP1);

	void setRenderState(bool shouldRender);

	void setThickness(const DirectX::SimpleMath::Vector2& thickness);

	/*
	Range: [0, 1]
	*/
	void setColor(const DirectX::SimpleMath::Vector3& col);

	/*
	Offset in viewspace
	*/
	void setOffset(const DirectX::SimpleMath::Vector3& offset);

};

