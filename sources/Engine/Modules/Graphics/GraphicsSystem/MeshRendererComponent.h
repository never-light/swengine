#pragma once

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"
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

private:
    std::shared_ptr<Mesh> m_meshInstance;
    std::vector<std::shared_ptr<Material>> m_materialsInstances;
};

