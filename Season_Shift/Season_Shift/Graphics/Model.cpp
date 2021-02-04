#include "Model.h"

Model::Model(std::shared_ptr<Mesh> mesh, std::vector<SubsetMaterial> materials) :
    m_mesh(mesh),
    m_materials(materials),
    m_renderID({ 0, 0 })
{
    //m_renderID.shdId = material->m_shaderHash;
    //m_renderID.matId = material->m_textureHash;
}

Model::~Model()
{
}

const std::shared_ptr<Mesh> Model::getMesh() const
{ 
    return m_mesh;
}

const std::vector<SubsetMaterial>& Model::getSubsetsMaterial() const
{
    return m_materials;
}

rid128_t Model::getRenderID() const
{
    return m_renderID;
}

bool rid_cmp(const Model& lhs, const Model& rhs)
{
    if (lhs.getRenderID().shdId < rhs.getRenderID().shdId) return true;
    if (rhs.getRenderID().shdId < lhs.getRenderID().shdId) return false;

    // Checks this is same shdId
    if (lhs.getRenderID().matId < rhs.getRenderID().matId) return true;
    if (rhs.getRenderID().matId < lhs.getRenderID().matId) return false;

    return false;
 
}
