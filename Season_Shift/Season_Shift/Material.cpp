#include "Material.h"

Material::Material(MapPaths maps)
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_textures = {nullptr, nullptr, nullptr, nullptr};

	// TODO: Set textures
}

Material::~Material()
{
	
}

void Material::bindShader(std::shared_ptr<DXDevice> device)
{
	device->bindShader(m_vertexShader, DXShader::Type::VS);
	device->bindShader(m_pixelShader, DXShader::Type::PS);
}

void Material::bindTextures(std::shared_ptr<DXDevice> device)
{
	device->bindShaderTexture(DXShader::Type::PS, 0, m_textures.ambient);
	device->bindShaderTexture(DXShader::Type::PS, 1, m_textures.diffuse);
	device->bindShaderTexture(DXShader::Type::PS, 2, m_textures.specular);
	device->bindShaderTexture(DXShader::Type::PS, 3, m_textures.normal);
}
