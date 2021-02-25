#include "pch.h"
#include "Material.h"

Material::Material(ShaderSet shaders, PhongMaps maps, size_t textureHash, size_t shaderHash)
{
	m_shaders = shaders;
	m_textures = {maps.diffuse, maps.specular, maps.normal};
	m_textureHash = textureHash;
	m_shaderHash = shaderHash;
}

Material::~Material()
{
	
}

void Material::setBuffers(std::shared_ptr<DXBuffer> vsBuffer, std::shared_ptr<DXBuffer> psBuffer)
{
	m_buffers.vsBuffer = vsBuffer;
	m_buffers.psBuffer = psBuffer;
}

void Material::bindShader(DXDevice* device)
{
	device->bindShader(m_shaders.vs, DXShader::Type::VS);
	device->bindShader(m_shaders.ps, DXShader::Type::PS);
}

void Material::bindTextures(DXDevice* device)
{
	// Assumes reserved slots for phong
	device->bindShaderTexture(DXShader::Type::PS, 0, m_textures.diffuse);
	device->bindShaderTexture(DXShader::Type::PS, 1, m_textures.specular);
	device->bindShaderTexture(DXShader::Type::PS, 2, m_textures.normal);
}

void Material::bindBuffers(DXDevice* device)
{
	device->bindShaderConstantBuffer(DXShader::Type::VS, 10, m_buffers.vsBuffer);
	device->bindShaderConstantBuffer(DXShader::Type::PS, 10, m_buffers.psBuffer);
}
