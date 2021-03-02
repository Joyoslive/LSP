#include "pch.h"
#include "ParticleSystem.h"

using namespace DirectX::SimpleMath;

/*constexpr*/ unsigned int uavCount = (unsigned int)-1;
/*constexpr*/ unsigned int empty = (unsigned int)0;

void ParticleSystem::emitt(EmittStructure emittData)
{
	m_emittCBuffer->updateMapUnmap(&emittData, sizeof(EmittStructure));


	m_renderer->getDXDevice()->bindShader(m_emittShader, m_emittShader->getType());
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::CS, 0, m_emittCBuffer);
	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 0, uavCount, m_consumeBuffer);
	
}

void ParticleSystem::simulate(float dt)
{
}

void ParticleSystem::draw(const Matrix& view, const Matrix& proj)
{





}

void ParticleSystem::swapBuffers()
{
	m_appendBuffer.swap(m_consumeBuffer);
}

ParticleSystem::ParticleSystem(const std::shared_ptr<GfxRenderer>& renderer, const std::string& simShader, const std::string& emittShader, unsigned int maxCount)
{
	
	m_initialBind = true;
	m_renderer = renderer;
	auto dev = m_renderer->getDXDevice();

	m_vertexShader = dev->createShader("ParticleVS.cso", DXShader::Type::VS);
	m_geometryShader = dev->createShader("ParticleQuadGS.cso", DXShader::Type::GS);
	m_pixelShader = dev->createShader("ParticlePS.cso", DXShader::Type::PS);

	if (simShader.empty())
		m_simulationShader = dev->createShader("ParticleDefaultSimCS.cso", DXShader::Type::CS);
	else
		m_simulationShader = dev->createShader(simShader, DXShader::Type::CS);

	if (emittShader.empty())
		m_emittShader = dev->createShader("ParticleDefaultEmittCS.cso", DXShader::Type::CS);
	else
		m_emittShader = dev->createShader(emittShader, DXShader::Type::CS);

	m_emittCBuffer = dev->createConstantBuffer(sizeof(EmittStructure), true, true);

	m_appendBuffer = dev->createAppendConsumeBuffer(maxCount, sizeof(Particle), false, true, nullptr);
	m_consumeBuffer = dev->createAppendConsumeBuffer(maxCount, sizeof(Particle), false, true, nullptr);

	//to bind with empty
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 0, empty, m_appendBuffer);
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 1, empty, m_consumeBuffer);

	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 0, 0, nullptr);
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 1, 0, nullptr);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::SimulateAndDraw(const Matrix& view, const Matrix& proj, float dt)
{

}

ParticleSystem::EmittStructure::EmittStructure(Vector3 pos, float lifeTime, Vector3 randVec, float other, Vector3 direction, unsigned int count)
{
	this->pos = pos;
	this->lifeTime = lifeTime;
	this->randVec = randVec;
	this->other = other;
	this->direction = direction;
	this->count = count;
}
