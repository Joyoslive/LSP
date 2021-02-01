#pragma once

#include <string>
#include <fstream>
#include <assert.h>

struct TextureData
{
	void* data;							// has to be manually deleted!
	int width, height, channels;
};

std::string loadCompiledShader(const std::string& filePath);
TextureData loadFileToTexture(const std::string& filePath);


