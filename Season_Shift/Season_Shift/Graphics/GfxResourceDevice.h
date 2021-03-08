#pragma once
#include <utility>
#include "DX/DXDevice.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceRepository.h"
#include "Model.h"
#include "AssimpLoader.h"
#include "2D/Text.h"

/*

Works as a creator for high level graphics resources. Holds repositories for currently loaded graphics resources.

Adds (Assembles them with DXResources), removes, manages existing
- Models
- Meshes
- Material
*/


struct alignas(16) DefaultShader_VSDATA
{
	int a, b;
};

struct alignas(16) DefaultShader_PSDATA
{
	int a, b, c, d, e, f;
};


// Put this under Graphics namespace!
enum class GfxShader
{
	DEFAULT
};

enum class ScreenPos
{
	MIDDLE,
};

class GfxResourceDevice
{
private:
	std::shared_ptr<DXDevice> m_dxDev;
	std::unique_ptr<AssimpLoader> m_assimpLoader;

	ResourceRepository<std::size_t, Material::ShaderSet> m_shaderSetRepo;
	ResourceRepository<std::size_t, std::shared_ptr<Material>> m_materialRepo;
	ResourceRepository<std::string, std::shared_ptr<Mesh>> m_meshRepo;
	ResourceRepository<std::size_t, EngineMeshData> m_modelRepo;
	ResourceRepository<std::wstring, std::shared_ptr<DirectX::SpriteFont>> m_fontRepo;

	
private:
	std::shared_ptr<DXTexture> loadTexture(std::string filepath);
	std::shared_ptr<DXTexture> loadTextureSprite(std::string filepath);
	std::pair<std::size_t, Material::ShaderSet> loadShader(GfxShader shader);
	std::pair<std::shared_ptr<DXBuffer>, std::shared_ptr<DXBuffer>> loadBuffers(GfxShader shader);

	std::shared_ptr<Text> createTextElement(const std::string& text, const std::wstring& path);
	float getXFromScreenPos(ScreenPos pos);
	float getYFromScreenPos(ScreenPos pos);

public:
	GfxResourceDevice(std::shared_ptr<DXDevice> dev);
	~GfxResourceDevice();

	/*
	Create material to use for Model assembly
	*/
	std::shared_ptr<Material> createMaterial(GfxShader shader, const std::string& difPath, const std::string& specPath, const std::string& normPath);

	/*
	Create a mesh, store it with an ID for Model assembly
	*/
	std::shared_ptr<Mesh> createMesh(const std::string& meshID, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	/*
	Assumes you have created a mesh
	*/
	std::shared_ptr<Model> assembleModel(const std::string& meshID, std::shared_ptr<Material> material);

	/*
	Create single model (one mesh) from file
	*/
	std::shared_ptr<Model> createModel(const std::string& modelDirectory, const std::string& modelFileName, GfxShader shader);

	// 2D Resources
	/*
	Create a text element at a set position with or without a font
	*/
	std::shared_ptr<ISprite> createSprite(const std::string& text, const std::wstring& path, float x=0, float y=0);
	std::shared_ptr<ISprite> createSprite(const std::string& text, float x=0, float y=0);
	std::shared_ptr<ISprite> createSprite(const std::string& text, const std::wstring& path, ScreenPos screenPosX, float y=0);
	std::shared_ptr<ISprite> createSprite(const std::string& text, ScreenPos screenPosX, float y=0);
	std::shared_ptr<ISprite> createSprite(const std::string& text, const std::wstring& path, float x, ScreenPos screenPosY);
	std::shared_ptr<ISprite> createSprite(const std::string& text, float x, ScreenPos screenPosY);
	std::shared_ptr<ISprite> createSprite(const std::string& text, const std::wstring& path, ScreenPos screenPosX, ScreenPos screenPosY);
	std::shared_ptr<ISprite> createSprite(const std::string& text, ScreenPos screenPosX, ScreenPos screenPosY);

	std::shared_ptr<ISprite> createSpriteTexture(const std::string& textureName, float positionX = 0, float positionY = 0, float scaleX = 1, float scaleY = 1, float rotation = 0, float depth = 0);
};

