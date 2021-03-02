#pragma once
#include "GfxRenderer.h"
#include <utility>

class Camera;
class Model;

class ShadowMapper
{
public:
	struct Cascade
	{
		DirectX::SimpleMath::Matrix projMat;
		float cascadeStart;		// viewspace
		unsigned int resolution;
	};
private:
	std::shared_ptr<GfxRenderer> m_renderer;
	std::vector<std::shared_ptr<DXTexture>> m_cascadeShadowMaps;
	std::vector<D3D11_VIEWPORT> m_cascadeViewports;
	std::shared_ptr<DXBuffer> m_projectionsCB;
	std::vector<Cascade> m_cascades;
	bool m_firstTime;

	void createResources();

public:
	ShadowMapper(std::shared_ptr<GfxRenderer> renderer);
	~ShadowMapper() = default;

	void setCascadeSettings(const std::vector<std::pair<float, unsigned int>>& cascadeEndDistancesAndResolution);

	const std::vector<Cascade>& generateCascades(const std::vector<std::shared_ptr<Model>>& casters, const std::shared_ptr<Camera>& playerCamera, 
		const DirectX::SimpleMath::Matrix& lightViewMatrix);


};

