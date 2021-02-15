#pragma once
#include "IRenderStrategy.h"
#include "Skybox.h"
#include "FullscreenQuad.h"
#include "DirectionalLight.h"

class DeferredRenderStrategy final : public IRenderStrategy
{
private:
	struct alignas(16) CameraBuffer
	{
		DirectX::SimpleMath::Vector3 position;
	};
private:
	struct GBuffers
	{
		std::shared_ptr<DXTexture> gbPosWS;		// Position
		std::shared_ptr<DXTexture> gbNorWS;		// Normal
		std::shared_ptr<DXTexture> gbUV;			// UV
		std::shared_ptr<DXTexture> gbDiffuse;		// Diffuse

		void clear(DXDevice* dev)
		{
			float clearCol[] = { 0.0, 0.0, 0.0, 0.0 };
			dev->clearRenderTarget(gbPosWS, clearCol);
			dev->clearRenderTarget(gbNorWS, clearCol);
			dev->clearRenderTarget(gbUV, clearCol);
			dev->clearRenderTarget(gbDiffuse, clearCol);
		}
	} m_gbuffers;

	std::shared_ptr<DXBuffer> m_gpMatrixBuffer;
	std::shared_ptr<DXRenderPass> m_geometryPassSolid;
	std::shared_ptr<DXRenderPass> m_geometryPassWireframe;

	std::shared_ptr<DXRenderPass> m_lightPass;
	FullscreenQuad m_fsQuad;
	DirectionalLight m_dirLight;
	std::shared_ptr<DXBuffer> m_dirLightBuffer;
	std::shared_ptr<DXBuffer> m_cameraBuffer;

	Skybox m_skybox;

	void setupGeometryPass();
	void setupLightPass();
	void setupPostProcessPass();

public:
	DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~DeferredRenderStrategy() = default;

	void render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera) override;


};

