#pragma once
//#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "../Component.h"

class Material;


typedef union
{
    struct
    {
        uint64_t matId;         // hashed from amb, diff, spec + nor filepaths
        uint64_t shdId;         // hashed from vs/ps filenames

    };

} rid128_t;

struct SubsetMaterial
{
    unsigned int vertexCount;
    unsigned int vertexStart;

    unsigned int indexStart;
    unsigned int indexCount;

    std::shared_ptr<Material> material;
};

class Model : public Component
{
private:
	std::shared_ptr<Mesh> m_mesh;
	
    std::vector<SubsetMaterial> m_materials;

    rid128_t m_renderID;

public:
    Model(std::shared_ptr<Mesh> mesh, std::vector<SubsetMaterial> material);     // renderID set with material!
    ~Model();

    const std::shared_ptr<Mesh> getMesh() const;
    
    const std::vector<SubsetMaterial>& getSubsetsMaterial() const;

    rid128_t getRenderID() const;

};

// Used for std::sort(models)
bool rid_cmp(const Model& lhs, const Model& rhs);
