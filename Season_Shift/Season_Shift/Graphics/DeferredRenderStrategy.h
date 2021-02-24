#pragma once
#include "IRenderStrategy.h"
#include "FullscreenQuad.h"
#include "DirectionalLight.h"

class SkyBox;

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

	DirectX::XMMATRIX m_gpMatrices[3];
	std::shared_ptr<DXBuffer> m_gpMatrixBuffer;
	std::shared_ptr<DXRenderPass> m_geometryPassSolid;
	std::shared_ptr<DXRenderPass> m_geometryPassWireframe;

	std::shared_ptr<DXRenderPass> m_lightPass;
	FullscreenQuad m_fsQuad;
	std::shared_ptr<DirectionalLight> m_dirLight;
	std::shared_ptr<DXBuffer> m_dirLightBuffer;
	std::shared_ptr<DXBuffer> m_cameraBuffer;

	std::shared_ptr<Skybox> m_skybox;

	bool m_usePostProcessing = false; // temporary assignment
	std::shared_ptr<DXRenderPass> m_postProcessPass;
	FullscreenQuad m_postProcessQuad;
	std::shared_ptr<DXTexture> m_prePostTexture;
	std::shared_ptr<DXBuffer> m_prevMatrices;

	void setupGeometryPass();
	void setupLightPass();
	void setupPostProcessPass();

public:
	DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~DeferredRenderStrategy() = default;

	void render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera) override;

	void setSkybox(std::shared_ptr<Skybox> skybox) override;

	void setDirLight(std::shared_ptr<DirectionalLight> light) override;

	void setUp() override;

	void present() override;

};

