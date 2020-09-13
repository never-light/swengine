#include "precompiled.h"

#pragma hdrstop

#include "ResourceInstance.h"

#include <utility>

#include "options.h"
#include "ResourceManager.h"

ResourceInstance::ResourceInstance(std::string resourceId,
  std::unique_ptr<Resource> resource,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_resourceId(std::move(resourceId)),
    m_resource(std::move(resource)),
    m_resourceManager(std::move(resourceManager))
{
}

ResourceInstance::~ResourceInstance() = default;

void ResourceInstance::loadResource()
{
  const ResourceDeclaration& declaration = m_resourceManager->getResourceDeclaration(m_resourceId);

  if constexpr (LOG_RESOURCES_LOADING) {
    if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
      spdlog::debug("Load resource {} from file {}", m_resourceId, sourceFile->path);
    }
    else {
      spdlog::debug("Load resource {} from unknown source", m_resourceId);
    }
  }

  m_resource->performLoad(declaration, *m_resourceManager);
}

void ResourceInstance::unloadResource()
{
  spdlog::debug("Unload resource {}", m_resourceId);

  m_resource->performUnload();
}
