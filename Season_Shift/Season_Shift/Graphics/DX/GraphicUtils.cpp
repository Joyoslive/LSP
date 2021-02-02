#include "GraphicsUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::string loadCompiledShader(const std::string& filePath)
{
	std::string data;
	std::ifstream fileStream;
	fileStream.open(filePath.c_str(), std::ios_base::binary);

	if (fileStream.fail())
	{
		//OutputDebugStringW(L"Failed to read file or can't find file!");
		assert(false);
	}

	// reserve memory once
	fileStream.seekg(0, fileStream.end);
	int length = static_cast<int>(fileStream.tellg());
	data.reserve(length);

	fileStream.seekg(0, fileStream.beg);	// place back at beginning
	// copy binary data to string
	data.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

	fileStream.close();

	return data;
}

TextureData loadFileToTexture(const std::string& filePath)
{
	TextureData textData = { };
	textData.data = stbi_load(filePath.c_str(), &textData.width, &textData.height, &textData.channels, STBI_rgb_alpha);

	return textData;
}

