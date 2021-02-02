#pragma once
#include <memory>
#include "Mesh.h"

class Material;


typedef union
{
    struct
    {
        uint64_t matId;         // hashed from amb, diff, spec + nor filepaths
        uint64_t shdId;         // hashed from vs/ps filenames

    };

} rid128_t;

class Model
{
private:
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
    rid128_t m_renderID;

public:
    Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);     // renderID set with material!
    ~Model();

    const std::shared_ptr<Mesh> getMesh() const;
    const std::shared_ptr<Material> getMaterial() const;
    rid128_t getRenderID() const;

};

// Used for std::sort(models)
bool rid_cmp(const Model& lhs, const Model& rhs);
