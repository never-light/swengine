#pragma once

#include "Modules/Graphics/OpenGL/GLMaterial.h"
#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

#include "Transform.h"

class MeshRendererComponent
{
public:
    MeshRendererComponent();

    void setMeshInstance(std::shared_ptr<Mesh> instance);
    std::shared_ptr<Mesh> getMeshInstance() const;

    void setMaterialsInstances(const std::vector<std::shared_ptr<GLMaterial>>& instances);
    void setMaterialInstance(size_t subMeshIndex, std::shared_ptr<GLMaterial> instance);

    std::shared_ptr<GLMaterial> getMaterialInstance(size_t subMeshIndex) const;

    bool isCulled() const;
    void cull(bool culled=true);

    const AABB& getAABB() const;
    void updateBounds(Transform& transform);

private:
    std::shared_ptr<Mesh> m_meshInstance;
    std::vector<std::shared_ptr<GLMaterial>> m_materialsInstances;

    bool m_isCulled = false;

    AABB m_aabb;
};

