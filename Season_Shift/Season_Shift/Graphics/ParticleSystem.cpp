#include "pch.h"
#include "ParticleSystem.h"
#include "../Logger.h"

using namespace DirectX::SimpleMath;

constexpr unsigned int uavCount = (unsigned int)-1;
constexpr unsigned int empty = (unsigned int)0;

constexpr float simSize = 1024; //must match size in simulation computeshader
constexpr float emittSize = 8; //must match size in emitt computeshader

void ParticleSystem::simulate(float dt)
{
	//update constant buffers
	SimulationInfo simInfo = { dt, m_maxParticleLifeTime, 0, 0 };

	m_simulationCBuffer->updateMapUnmap(&simInfo, sizeof(SimulationInfo));
	

	//bind
	m_renderer->getDXDevice()->bindShader(m_simulationShader, DXShader::Type::CS);
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::CS, 0, m_simulationCBuffer);
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::CS, 1, m_particleCountCBuffer);


	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 0, uavCount, m_appendBuffer);
	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 1, uavCount, m_consumeBuffer);

	//copy consumeBuffers size to contantBuffer
	m_renderer->getDXDevice()->copyStructureCount(m_particleCountCBuffer, m_consumeBuffer);

	m_renderer->getDXDevice()->dispatch((unsigned int)ceil(m_maxNumParticles / simSize), 1, 1);

	//unbind
	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 0, 0, nullptr);
	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 1, 0, nullptr);

}

void ParticleSystem::draw(const Matrix& view, const Matrix& proj)
{
	//update constantBuffer
	Matrix cameraWorldMatrix = view;
	cameraWorldMatrix = cameraWorldMatrix.Invert();

	Matrix newData[3] = { cameraWorldMatrix ,view, proj };
	m_transformMatrixCBuffer->updateMapUnmap(&newData, sizeof(newData));


	//bind constantBuffer
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::VS, 0, m_transformMatrixCBuffer);
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::GS, 0, m_transformMatrixCBuffer);

	//bind appendBuffer
	m_renderer->getDXDevice()->bindShaderStructuredBuffer(DXShader::Type::VS, 0, m_appendBuffer);

	//bind shaders
	m_renderer->getDXDevice()->bindShader(m_vertexShader, DXShader::Type::VS);
	m_renderer->getDXDevice()->bindShader(m_geometryShader, DXShader::Type::GS);
	m_renderer->getDXDevice()->bindShader(m_pixelShader, DXShader::Type::PS);

	


	//copy number of particles in the appendBuffer
	m_renderer->getDXDevice()->copyStructureCount(m_indirectArgsBuffer, m_appendBuffer);

	//unset vertexBuffer
	m_renderer->getDXDevice()->bindDrawIndexedBuffer(nullptr, nullptr, 0, 0);

	m_renderer->getDXDevice()->bindInputTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//render
	m_renderer->getDXDevice()->drawInstancedIndirect(m_indirectArgsBuffer);

	//unbind
	m_renderer->getDXDevice()->bindShader(nullptr, DXShader::Type::GS);
	m_renderer->getDXDevice()->bindShaderStructuredBuffer(DXShader::Type::VS, 0, nullptr);
}

void ParticleSystem::swapBuffers()
{
	m_appendBuffer.swap(m_consumeBuffer);
}

ParticleSystem::ParticleSystem(const std::shared_ptr<GfxRenderer>& renderer, const std::string& simShader, const std::string& emittShader, unsigned int maxCount, unsigned int particleLifeTime)
{
	
	m_initialBind = true;
	m_partCount.count = 0;
	m_maxNumParticles = maxCount;
	m_maxParticleLifeTime = particleLifeTime;

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

	//create constant buffers
	m_emittCBuffer = dev->createConstantBuffer(sizeof(EmittStructure), true, true); //dynamic
	m_simulationCBuffer = dev->createConstantBuffer(sizeof(SimulationInfo), true, true); //dynamic
	m_particleCountCBuffer = dev->createConstantBuffer(sizeof(ParticleCount), true, false); //default
	m_transformMatrixCBuffer = dev->createConstantBuffer(3 * sizeof(DirectX::SimpleMath::Matrix), true, true); //dynamic

	//indirectArgBuffer
	m_indirectArgsBuffer = dev->createIndirectArgumentBuffer(0, 1, 0, 0);
	
	//structured buffers
	m_appendBuffer = dev->createAppendConsumeBuffer(maxCount, sizeof(Particle), false, true, nullptr);
	m_consumeBuffer = dev->createAppendConsumeBuffer(maxCount, sizeof(Particle), false, true, nullptr);

	//bind with empty to start with empty buffers
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 0, empty, m_appendBuffer);
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 1, empty, m_consumeBuffer);
	//unbind
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 0, 0, nullptr);
	dev->bindAppendConsumeBuffer(DXShader::Type::CS, 1, 0, nullptr);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::emitt(EmittStructure emittData)
{
	m_emittCBuffer->updateMapUnmap(&emittData, sizeof(EmittStructure));

	//copy consumeBuffers size to contantBuffer
	m_renderer->getDXDevice()->copyStructureCount(m_particleCountCBuffer, m_consumeBuffer);

	//bind
	m_renderer->getDXDevice()->bindShader(m_emittShader, m_emittShader->getType());
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::CS, 0, m_emittCBuffer);
	m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::CS, 1, m_particleCountCBuffer);
	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 0, uavCount, m_consumeBuffer); //append to consumeBuffer, simulate will consume from this buffer later

	//m_renderer->getDXDevice()->dispatch(1, 1, 1);
	m_renderer->getDXDevice()->dispatch((unsigned int)ceil(emittData.count / emittSize), 1, 1);

	//unbind
	m_renderer->getDXDevice()->bindAppendConsumeBuffer(DXShader::Type::CS, 0, 0, nullptr);

}

void ParticleSystem::simulateAndDraw(const Matrix& view, const Matrix& proj, float dt)
{
	simulate(dt);
	draw(view, proj);
	swapBuffers();
}

ParticleSystem::EmittStructure::EmittStructure(Vector3 pos, float startLifeTime, Vector3 randVec, float other, Vector3 direction, unsigned int count, DirectX::SimpleMath::Vector3 color)
{
	this->pos = pos;
	this->startLifeTime = startLifeTime;
	this->randVec = randVec;
	this->other = other;
	this->direction = direction;
	this->count = count;
	this->color = color;
}
