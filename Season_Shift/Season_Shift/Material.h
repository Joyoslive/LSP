#pragma once

#include <string>
#include "Graphics/DX/DXTexture.h"
#include "Graphics/DX/DXDevice.h"

class Material
{
private:
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

public:
	struct MapPaths
	{
		std::string ambient;
		std::string diffuse;
		std::string specular;
		std::string normal;
	};
public:
	Material(MapPaths maps);
	~Material();

	void bindShader(std::shared_ptr<DXDevice> device);
	void bindTextures(std::shared_ptr<DXDevice> device);
};

