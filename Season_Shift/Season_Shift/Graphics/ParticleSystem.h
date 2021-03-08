#pragma once
#include "GfxRenderer.h"
#include "DX\DXBuffer.h"

class ParticleSystem
{
public:
	struct EmittStructure
	{
		DirectX::SimpleMath::Vector3 pos;
		float startLifeTime;
		DirectX::SimpleMath::Vector3 randVec;
		float scale;
		DirectX::SimpleMath::Vector3 direction;
		unsigned int count;
		DirectX::SimpleMath::Vector3 color;
		float padding = 0;
		EmittStructure(float startLifeTime, DirectX::SimpleMath::Vector3 randVec, float scale, DirectX::SimpleMath::Vector3 direction, unsigned int count, DirectX::SimpleMath::Vector3 color);
	};

private:
	bool m_initialBind;
	unsigned int m_maxNumParticles;
	unsigned int m_maxParticleLifeTime;
	std::shared_ptr<GfxRenderer> m_renderer;

	//shaders
	std::shared_ptr<DXShader> m_simulationShader;
	std::shared_ptr<DXShader> m_emittShader;
	std::shared_ptr<DXShader> m_vertexShader;
	std::shared_ptr<DXShader> m_geometryShader;
	std::shared_ptr<DXShader> m_pixelShader;

	//structured buffers
	std::shared_ptr<DXBuffer> m_appendBuffer;
	std::shared_ptr<DXBuffer> m_consumeBuffer;

	//constant buffers
	std::shared_ptr<DXBuffer> m_emittCBuffer;
	std::shared_ptr<DXBuffer> m_particleCountCBuffer;
	std::shared_ptr<DXBuffer> m_simulationCBuffer;
	std::shared_ptr<DXBuffer> m_transformMatrixCBuffer;

	std::shared_ptr<DXBuffer> m_indirectArgsBuffer;

	struct ParticleCount
	{
		unsigned int count;
		unsigned int padding1;
		unsigned int padding2;
		unsigned int padding3;
	};
	ParticleCount m_partCount;

	struct SimulationInfo
	{
		float dt;
		float maxLifeTime;
		float padding1;
		float padding2;
	};

	struct Particle
	{
		DirectX::SimpleMath::Vector3 pos;
		float lifeTime;
		DirectX::SimpleMath::Vector3 velocity;
		float other;
		DirectX::SimpleMath::Vector3 color;
		float padding = 0;
	};

private:
	void simulate(float dt);
	void draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	void swapBuffers();

public:
	ParticleSystem(const std::shared_ptr<GfxRenderer>& renderer, const std::string& simShader, const std::string& emittShader, unsigned int maxCount, unsigned int particleLifeTime);
	~ParticleSystem();
	void emitt(EmittStructure emittData);


	void simulateAndDraw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, float dt);
};

