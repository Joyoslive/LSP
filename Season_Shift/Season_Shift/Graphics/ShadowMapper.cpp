#include "pch.h"
#include "ShadowMapper.h"

using namespace DirectX::SimpleMath;


ShadowMapper::ShadowMapper(std::shared_ptr<GfxRenderer> renderer) :
    m_renderer(renderer),
    m_firstTime(true)
{

    createResources();
}

void ShadowMapper::createResources()
{
    auto dev = m_renderer->getDXDevice();

    // Buffer for geometry instance split
    m_projectionsCB = dev->createConstantBuffer(3 * sizeof(Matrix), true, true);        // Set to 3


}


void ShadowMapper::setCascadeSettings(const std::vector<std::pair<float, unsigned int>>& cascadeEndDistancesAndResolution)
{
    if (m_firstTime)
    {
        for (auto cascadeInfo : cascadeEndDistancesAndResolution)
        {
            m_cascades.push_back({ Matrix::Identity, cascadeInfo.first, cascadeInfo.second });
            m_cascadeViewports.push_back({ 0.f, 0.f, static_cast<float>(cascadeInfo.second) , static_cast<float>(cascadeInfo.second), 0.f, 1.f });
        }

        // must be here because the argument is dependent on the Camera which can vary.
        // Textures for shadow map
        for (auto& viewport : m_cascadeViewports)
        {
            DXTexture::Desc desc{};
            desc.type = DXTexture::Type::TEX2D;

            desc.desc2D.Width = viewport.Width;
            desc.desc2D.Height = viewport.Height;
            desc.desc2D.MipLevels = 1;
            desc.desc2D.ArraySize = 1;
            desc.desc2D.Format = DXGI_FORMAT_R32_TYPELESS;
            desc.desc2D.SampleDesc.Count = 1;
            desc.desc2D.SampleDesc.Quality = 0;
            desc.desc2D.Usage = D3D11_USAGE_DEFAULT;
            desc.desc2D.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            desc.desc2D.CPUAccessFlags = 0;
            desc.desc2D.MiscFlags = 0;

            m_cascadeShadowMaps.push_back(
                m_renderer->getDXDevice()->createTexture(desc, nullptr)
            );
        }

        m_firstTime = false;
    }
    else
    {
        if (cascadeEndDistancesAndResolution.size() != m_cascades.size()) assert(false);

        for (int i = 0; i < m_cascades.size(); ++i)
        {
            m_cascades[i].cascadeStart = cascadeEndDistancesAndResolution[i].first;
            m_cascades[i].resolution = cascadeEndDistancesAndResolution[i].second;
        }
    }



}

const std::vector<ShadowMapper::Cascade>& ShadowMapper::generateCascades(const std::vector<std::shared_ptr<Model>>& casters, const std::shared_ptr<Camera>& playerCamera, const DirectX::SimpleMath::Matrix& lightViewMatrix)
{
    // Do the stuff

    // Manipulate m_cascades..


    // Return
    return m_cascades;
}
