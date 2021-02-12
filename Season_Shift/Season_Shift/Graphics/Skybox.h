#pragma once
#include "GfxRenderer.h"
#include <memory>
#include <string>
#include <filesystem>

class Skybox
{
private:
	std::shared_ptr<DXShader> m_vs;
	std::shared_ptr<DXShader> m_ps;

	std::vector<std::shared_ptr<DXTexture>> m_textures;

	std::shared_ptr<GfxRenderer> m_renderer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rs;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dss;

public:
	Skybox(std::shared_ptr<GfxRenderer> renderer);
	~Skybox() = default;

	void loadSkybox(std::filesystem::path directoryPath);
	void draw();


};

