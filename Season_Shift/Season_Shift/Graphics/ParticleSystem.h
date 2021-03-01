#pragma once
#include "GfxRenderer.h"
#include "DX\DXBuffer.h"

class ParticleSystem
{
private:
	bool m_initialBind;

	std::shared_ptr<GfxRenderer> m_renderer;
	std::shared_ptr<DXShader> m_simulationShader;
	std::shared_ptr<DXShader> m_emittShader;
	std::shared_ptr<DXShader> m_vertexShader;
	std::shared_ptr<DXShader> m_geometryShader;
	std::shared_ptr<DXShader> m_pixelShader;

	std::shared_ptr<DXBuffer> m_appendBuffer;
	std::shared_ptr<DXBuffer> m_consumeBuffer;


	struct Particle
	{
		DirectX::SimpleMath::Vector3 pos;
		float lifeTime;
		DirectX::SimpleMath::Vector3 color;
		float other;
	};
	std::vector<Particle> m_particleVec;

private:
	void emitt(DirectX::SimpleMath::Vector3 pos);
	void simulate(float dt);
	void draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	void swapBuffers();

public:
	ParticleSystem(const std::shared_ptr<GfxRenderer>& renderer, const std::string& simShader, const std::string& emittShader, unsigned int maxCount);
	~ParticleSystem();

	void SimulateAndDraw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, float dt);
};

