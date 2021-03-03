#pragma once
#include "IRenderStrategy.h"
#include "FullscreenQuad.h"
#include "DirectionalLight.h"
#include "ParticleSystem.h"
#include "2D/SpriteRenderer.h"


class SkyBox;
class LineDrawer;

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

	std::vector<std::shared_ptr<ParticleSystem>> m_partSysVec;
	std::shared_ptr<LineDrawer> m_lineDrawer;

	bool m_usePostProcessing = true; // temporary assignment
	std::shared_ptr<DXRenderPass> m_postProcessPass;
	FullscreenQuad m_postProcessQuad;
	std::shared_ptr<DXTexture> m_prePostTexture;
	PostProcessVariables m_postProcessVariables;
	std::shared_ptr<DXBuffer> m_postProcessVariableBuffer;
	float m_resetTimer;
	std::shared_ptr<DXBuffer> m_prevMatrices;

	bool m_viewUI = true;
	std::shared_ptr<SpriteRenderer> m_spriteRenderer;
	std::vector<std::shared_ptr<ISprite>> m_sprites;

	void setupGeometryPass();
	void setupLightPass();
	void setupPostProcessPass();
	void setupUIRenderer();

public:
	DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~DeferredRenderStrategy() = default;

	void render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera, long double dt) override;

	void setSkybox(std::shared_ptr<Skybox> skybox) override;

	void setDirLight(std::shared_ptr<DirectionalLight> light) override;

	void setPostProcessVariables(PostProcessVariables ppVar) override;

	void setLineRenderSetttings(const LineVariables& settings) override;

	void addParticleSystem(std::shared_ptr<ParticleSystem> particleSystem) override;

	void addToSpriteBatch(std::shared_ptr<ISprite> sprite) override;

	void setUp() override;

	void present() override;

};

