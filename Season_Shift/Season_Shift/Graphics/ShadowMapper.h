#pragma once
#include "GfxRenderer.h"
#include <utility>
#include <array>

class Camera;
class Model;

class ShadowMapper
{
public:
	struct Cascade
	{
		DirectX::SimpleMath::Matrix projMat;
		float cascadeEnd;		// viewspace
		unsigned int resolution;
		std::shared_ptr<DXTexture> texture;
		D3D11_VIEWPORT viewport;
	};
	struct OrthoMatrices
	{
		std::array<DirectX::SimpleMath::Matrix, 3> orthoMats;
	}  m_generatedOrthos;
private:


	struct BufferInfo
	{
		DirectX::SimpleMath::Matrix worldMat;
		DirectX::SimpleMath::Matrix viewMat;
		DirectX::SimpleMath::Matrix projMat;		// this changes per cascade
	} m_bufferInfo;

	std::shared_ptr<GfxRenderer> m_renderer;
	std::shared_ptr<DXBuffer> m_bufferInfoCB;
	std::vector<Cascade> m_cascades;

	std::shared_ptr<DXShader> m_vs;
	std::shared_ptr<DXShader> m_ps;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_il;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dss;

	bool m_firstTime;

	void createResources();

	OrthoMatrices createOrthos(const std::shared_ptr<Camera>& cam, const DirectX::SimpleMath::Matrix& lightCamView);

public:
	ShadowMapper(std::shared_ptr<GfxRenderer> renderer);
	~ShadowMapper() = default;

	void setCascadeSettings(const std::vector<std::pair<float, unsigned int>>& cascadeEndDistancesAndResolution);

	const std::vector<Cascade>& generateCascades(const std::vector<std::shared_ptr<Model>>& casters, const std::shared_ptr<Camera>& playerCamera, 
		const DirectX::SimpleMath::Matrix& lightViewMatrix);


};

