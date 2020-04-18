#include "precompiled.h"

#pragma hdrstop

#include "ResourceInstance.h"
#include "ResourceManager.h"

ResourceInstance::ResourceInstance(const std::string& resourceId,
  std::unique_ptr<Resource> resource,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_resourceId(resourceId),
    m_resource(std::move(resource)),
    m_resourceManager(resourceManager)
{
}

ResourceInstance::~ResourceInstance()
{

}

void ResourceInstance::loadResource()
{
  const ResourceDeclaration& declaration = m_resourceManager->getResourceDeclaration(m_resourceId);
  m_resource->performLoad(declaration, *m_resourceManager.get());
}

void ResourceInstance::unloadResource()
{
  m_resource->performUnload();
}
