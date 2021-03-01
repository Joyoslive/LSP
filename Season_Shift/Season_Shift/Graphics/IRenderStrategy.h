#pragma once
#include "GfxRenderer.h"
#include "Model.h"
#include "DX/DXRenderPass.h"
//#include "../Camera.h"
#include "DirectionalLight.h"

struct alignas(16) PostProcessVariables
{
	float elapsedTime;
	float deltaTime;
	int clientWidth;
	int clientHeight;
	double randomNumber;

	float speedlineRAD;
	float speedlineThickness;
	float speedlineSpeedFactor;

	int motionBlurSamples;
};

class Camera;
class Skybox;
class ParticleSystem;
/*

Works as an interface to be able to select different rendering implementations (e.g forward or deferred)
- Render Strategies create and use specific graphics resources in order to implement techniques.
- The meat of graphics techniques lies in a Render Strategy

*/
class IRenderStrategy
{
protected:
	std::shared_ptr<GfxRenderer> m_renderer;

public:
	IRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	virtual ~IRenderStrategy();
	virtual void render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera, long double dt) = 0;
	virtual void setSkybox(std::shared_ptr<Skybox> skybox) {};
	virtual void addParticleSystem(std::shared_ptr<ParticleSystem> particleSystem) {};
	virtual void present() {};
	virtual void setDirLight(std::shared_ptr<DirectionalLight> light) {};
	virtual void setPostProcessVariables(PostProcessVariables ppVar) {};
	virtual void setLineRenderSetttings(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos, bool shouldRender,
		const DirectX::SimpleMath::Vector3& offset, const DirectX::SimpleMath::Vector3& color, const DirectX::SimpleMath::Vector2& thickness) {};
	virtual void setUp() = 0;
};

