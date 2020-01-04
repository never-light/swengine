#pragma once

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

#include "Transform.h"
#include "Material.h"

class MeshRendererComponent
{
public:
    MeshRendererComponent();

    void setMeshInstance(std::shared_ptr<Mesh> instance);
    std::shared_ptr<Mesh> getMeshInstance() const;

    void setMaterialsInstances(const std::vector<std::shared_ptr<Material>>& instances);
    void setMaterialInstance(size_t subMeshIndex, std::shared_ptr<Material> instance);

    std::shared_ptr<Material> getMaterialInstance(size_t subMeshIndex) const;

    bool isCulled() const;
    void cull(bool culled=true);

    const AABB& getAABB() const;
    void updateBounds(Transform& transform);

private:
    std::shared_ptr<Mesh> m_meshInstance;
    std::vector<std::shared_ptr<Material>> m_materialsInstances;

    bool m_isCulled = false;

    AABB m_aabb;
};

