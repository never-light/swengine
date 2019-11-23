#pragma once

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

class MeshRendererComponent
{
public:
    MeshRendererComponent();

    void setMeshInstance(std::shared_ptr<ResourceInstance> instance);
    std::shared_ptr<ResourceInstance> getMeshInstance() const;

    const Mesh* getMeshRawPtr() const;

private:
    std::shared_ptr<ResourceInstance> m_meshInstance;
};

