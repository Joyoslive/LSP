#pragma once
#include <memory>
#include "GfxRenderer.h"

class Camera;

class LineDrawer
{
private:
	std::shared_ptr<DXBuffer> m_pointsVB;		// dynamic vb
	std::shared_ptr<DXBuffer> m_camInfoCB;
	std::shared_ptr<DXBuffer> m_lineSettingsCB;

	struct alignas(16)
	{
		DirectX::SimpleMath::Vector3 topCol;
		DirectX::SimpleMath::Vector3 bottomCol;
		DirectX::SimpleMath::Vector3 leftCol;
		DirectX::SimpleMath::Vector3 rightCol;

		DirectX::SimpleMath::Vector3 startOffset;
		float thickness;
	} m_lineData;

	struct alignas(16) 
	{
		DirectX::SimpleMath::Vector3 currStartPos;
		DirectX::SimpleMath::Vector3 currEndPos;
	} m_linePoints;

	struct alignas(16) CamInfo
	{
		DirectX::SimpleMath::Matrix viewMat;
		DirectX::SimpleMath::Matrix projMat;
	};

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

};

