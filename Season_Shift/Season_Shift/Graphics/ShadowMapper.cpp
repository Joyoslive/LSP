#include "pch.h"
#include "ShadowMapper.h"
#include "Model.h"

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

    // Buffer for view matrix for each split
    m_projectionsCB = dev->createConstantBuffer(sizeof(Matrix) * 3, true, true);


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
    // Bind 


    // for each cascade, draw all shadow casters
    for (int i = 0; i < m_cascades.size(); ++i) {
        for (auto& mod : casters)
        {
            for (auto& mat : mod->getSubsetsMaterial())
            {
                //mat.material->bindShader(dev);
                //mat.material->bindTextures(dev);

                //m_gpMatrices[0] = mod->getTransform()->getWorldMatrix();
                //m_gpMatrixBuffer->updateMapUnmap(m_gpMatrices, sizeof(m_gpMatrices), 0, D3D11_MAP_WRITE_DISCARD, 0);
                //dev->bindShaderConstantBuffer(DXShader::Type::VS, 0, m_gpMatrixBuffer);

                //dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
                //dev->drawIndexed(mat.indexCount, mat.indexStart, mat.vertexStart);
            }
        }
    }




    // Manipulate m_cascades..


    // Return
    return m_cascades;
}
