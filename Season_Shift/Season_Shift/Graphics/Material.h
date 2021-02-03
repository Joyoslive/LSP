#pragma once

#include <string>
#include "DX/DXDevice.h"

class Model;

class Material
{
public:
	friend Model;

	struct PhongMaps
	{
		std::shared_ptr<DXTexture> diffuse;
		std::shared_ptr<DXTexture> specular;
		std::shared_ptr<DXTexture> normal;
	};

	struct ShaderSet
	{
		std::shared_ptr<DXShader> vs;
		std::shared_ptr<DXShader> ps;
	};

private:
	ShaderSet m_shaders;
	PhongMaps m_textures;
	size_t m_textureHash;
	size_t m_shaderHash;
public:
	Material(ShaderSet shaders, PhongMaps maps, size_t textureHash, size_t shaderHash);
	~Material();

	void bindShader(DXDevice* device);
	void bindTextures(DXDevice* device);
};

