#include "precompiled.h"

#pragma hdrstop

#include "ResourceManager.h"
#include "Exceptions/exceptions.h"

#include <pugixml.hpp>

#include "Modules/Graphics/Resources/TextureResource.h"
#include "Utility/files.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

const ResourceDeclaration& ResourceManager::getResourceDeclaration(const std::string& resourceId)
{
  return m_resourcesDeclarations.at(resourceId);
}

std::shared_ptr<ResourceInstance> ResourceManager::getResourceInstance(const std::string& resourceId)
{
  auto resourceInstanceIt = m_resourcesInstances.find(resourceId);

  if (resourceInstanceIt != m_resourcesInstances.end()) {
    return resourceInstanceIt->second;
  }

  auto resourceTypeIdIt = m_resourcesTypesIds.find(resourceId);

  if (resourceTypeIdIt == m_resourcesTypesIds.end()) {
    THROW_EXCEPTION(EngineRuntimeException, "Failed to get unknown resource instance: " + resourceId);
  }

  const auto& typeIndex = resourceTypeIdIt->second;

  auto resource = std::unique_ptr<Resource>(m_resourcesFactories.at(typeIndex)());

  auto resourceInstance = std::make_shared<ResourceInstance>(resourceId, std::move(resource), shared_from_this());
  m_resourcesInstances.insert({resourceId, resourceInstance});

  return resourceInstance;
}

void ResourceManager::loadResourcesMap(const std::string& content)
{
  pugi::xml_document resourcesMap;
  pugi::xml_parse_result result = resourcesMap.load_string(content.c_str());

  if (!result) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load resources map from invalid source");
  }

  pugi::xml_node declarationsList = resourcesMap.child("resources");
  loadResourcesMap(declarationsList);
}

void ResourceManager::loadResourcesMapFile(const std::string& path)
{
  if (!FileUtils::isFileExists(path)) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to read not existing resource map file " + path);
  }

  pugi::xml_document resourcesMap;
  pugi::xml_parse_result result = resourcesMap.load_file(path.c_str());

  if (!result) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load resources map from invalid source");
  }

  pugi::xml_node declarationsList = resourcesMap.child("resources");
  loadResourcesMap(declarationsList);
}

void ResourceManager::loadResourcesMap(const pugi::xml_node& declarationsList)
{
  for (pugi::xml_node declarationNode : declarationsList.children("resource")) {
    std::string resourceName = declarationNode.attribute("id").as_string();

    if (resourceName.empty()) {
      THROW_EXCEPTION(EngineRuntimeException, "Resource declaration has invalid id");
    }

    std::string resourceType = declarationNode.attribute("type").as_string();
    auto declarerIt = m_resourcesDeclarers.find(resourceType);

    if (declarerIt == m_resourcesDeclarers.end()) {
      THROW_EXCEPTION(EngineRuntimeException, "Resource declaration has invalid type (" + resourceType + ")");
    }

    std::function resourceDeclarer = declarerIt->second;

    pugi::xml_attribute sourceAttribute = declarationNode.attribute("source");

    const ResourceDeclaration* parentDeclaration = nullptr;
    pugi::xml_attribute extendsAttribute = declarationNode.attribute("extends");

    if (extendsAttribute) {
      auto parentDeclarationIt = m_resourcesDeclarations.find(extendsAttribute.as_string());

      if (parentDeclarationIt == m_resourcesDeclarations.end()) {
        THROW_EXCEPTION(EngineRuntimeException,
          std::string("Resource declaration has invalid parent id: ") + extendsAttribute.as_string());
      }

      parentDeclaration = &parentDeclarationIt->second;
    }

    ResourceSource source;

    if (sourceAttribute) {
      std::string sourcePath = declarationNode.attribute("source").as_string();

      if (!FileUtils::isFileExists(sourcePath)) {
        THROW_EXCEPTION(EngineRuntimeException,
          "Resource file does not exist: resource " + resourceName + ", path " + sourcePath);
      }

      source = ResourceSourceFile{sourcePath};

      if (FileUtils::isFileExists(sourcePath)) {
        source = ResourceSourceFile{sourcePath};
      }

      source = ResourceSourceFile{sourcePath};
    }
    else {
      if (parentDeclaration != nullptr) {
        source = parentDeclaration->source;
      }
      else {
        source = ResourceSourceDeclaration{};
      }
    }

    resourceDeclarer(resourceName, source, declarationNode, parentDeclaration);

    bool isPreloadRequired = declarationNode.attribute("preload").as_bool();

    if (isPreloadRequired) {
      std::shared_ptr resourceInstance = getResourceInstance(resourceName);
      resourceInstance->loadResource();

      bool isResourcePersistent = declarationNode.attribute("persistent").as_bool();

      if (isResourcePersistent) {
        resourceInstance->getResource<Resource>()->setPersistent(true);
      }
    }
  }

}
