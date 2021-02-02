#pragma once

#include <string>
#include "DX/DXDevice.h"

class Material
{
public:
	struct PhongMaps
	{
		std::shared_ptr<DXTexture> ambient;
		std::shared_ptr<DXTexture> diffuse;
		std::shared_ptr<DXTexture> specular;
		std::shared_ptr<DXTexture> normal;
	};
private:
	std::shared_ptr<DXShader> m_vertexShader,  m_pixelShader;
	PhongMaps m_textures;
	size_t m_hash;
public:
	explicit Material(PhongMaps maps, size_t hash);
	~Material();

	void bindShader(std::shared_ptr<DXDevice> device);
	void bindTextures(std::shared_ptr<DXDevice> device);
};

