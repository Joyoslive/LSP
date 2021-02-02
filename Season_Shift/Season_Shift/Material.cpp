#include "Material.h"

Material::Material(MapPaths maps)
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_textures = {nullptr, nullptr, nullptr, nullptr};
}

Material::~Material()
{}

void Material::bindShader(std::shared_ptr<DXDevice> device)
{}

void Material::bindTextures(std::shared_ptr<DXDevice> device)
{}
