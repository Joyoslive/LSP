#pragma once
#include "GfxRenderer.h"
#include <memory>
#include <string>
#include <filesystem>

class Camera;

// Can be seen as a "specialized model" 
class Skybox
{
private:
	struct alignas(16) VPInfo
	{
		DirectX::XMMATRIX viewMat;
		DirectX::XMMATRIX projMat;
	} m_vpInfo;

	std::shared_ptr<DXShader> m_vs;
	std::shared_ptr<DXShader> m_ps;

	std::vector<std::shared_ptr<DXTexture>> m_textures;
	std::shared_ptr<DXTexture> m_activeTexture;

	std::shared_ptr<GfxRenderer> m_renderer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rs;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dss;

	std::shared_ptr<DXBuffer> m_vpBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;



public:
	Skybox(std::shared_ptr<GfxRenderer> renderer);
	~Skybox() = default;

	void loadSkybox(std::filesystem::path directoryPath);
	void draw(const std::shared_ptr<Camera>& cam);


};

