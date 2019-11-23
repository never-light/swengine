#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent()
{

}

void MeshRendererComponent::setMeshInstance(std::shared_ptr<ResourceInstance> instance)
{
    m_meshInstance = instance;
}

std::shared_ptr<ResourceInstance> MeshRendererComponent::getMeshInstance() const
{
    return m_meshInstance;
}

const Mesh* MeshRendererComponent::getMeshRawPtr() const
{
    return m_meshInstance->getResource<MeshResource>()->getMesh().get();
}
