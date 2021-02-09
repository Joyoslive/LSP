#pragma once

#include <string>
#include "DX/DXDevice.h"
#include "../Logger.h"

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
	struct BufferSet
	{
		std::shared_ptr<DXBuffer> vsBuffer;
		std::shared_ptr<DXBuffer> psBuffer;
	};

private:
	ShaderSet m_shaders;
	BufferSet m_buffers;
	PhongMaps m_textures;
	size_t m_textureHash;
	size_t m_shaderHash;
public:
	Material(ShaderSet shaders, PhongMaps maps, size_t textureHash, size_t shaderHash);
	~Material();

	void setBuffers(std::shared_ptr<DXBuffer> vsBuffer, std::shared_ptr<DXBuffer> psBuffer);

	void bindShader(DXDevice* device);
	void bindTextures(DXDevice* device);
	void bindBuffers(DXDevice* device);

	template<typename T>
	inline void updateBuffer(T& bufferData, DXShader::Type shaderType)
	{
		std::shared_ptr<DXBuffer> buffer;
		switch (shaderType)
		{
			case DXShader::Type::VS:
				buffer = m_buffers.vsBuffer;
				break;
			case DXShader::Type::PS:
				buffer = m_buffers.psBuffer;
				break;
			default:
				throw std::runtime_error("Material only supports vertex and pixel shaders");
		}
		if (sizeof(T) != buffer->getTotalSize())
		{
			throw std::invalid_argument("bufferData needs to be of the same size as the buffer targeted for updating");
		}
		buffer->updateMapUnmap(&bufferData, buffer->getTotalSize());
	}
};