#include "ResourceManager.h"
#include "Exceptions/EngineRuntimeException.h"

#include <pugixml.hpp>

#include "Modules/Graphics/Resources/TextureResource.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

const ResourceDeclaration& ResourceManager::getResourceDeclaration(const std::string &resourceId)
{
    return m_resourcesSources.at(resourceId);
}

std::shared_ptr<ResourceInstance> ResourceManager::getResourceInstance(const std::string &resourceId)
{
    auto resourceInstanceIt = m_resourcesInstances.find(resourceId);

    if (resourceInstanceIt != m_resourcesInstances.end()) {
        return resourceInstanceIt->second;
    }

    const auto& typeIndex = m_resourcesTypesIds.at(resourceId);
    auto resource = std::unique_ptr<Resource>(m_resourcesFactories.at(typeIndex)());

    auto resourceInstance = std::make_shared<ResourceInstance>(resourceId, std::move(resource), shared_from_this());
    m_resourcesInstances.insert({ resourceId, resourceInstance });

    return resourceInstance;
}

void ResourceManager::addResourcesMap(const std::string& path)
{
    pugi::xml_document fontDescription;
    pugi::xml_parse_result result = fontDescription.load_file(path.c_str());

    if (!result) {
        ENGINE_RUNTIME_ERROR("Trying to load resources map from invalid source");
    }

    pugi::xml_node declarationsList = fontDescription.child("resources");

    for (pugi::xml_node declarationNode : declarationsList.children("resource")) {
        std::string resourceName = declarationNode.attribute("id").as_string();

        if (resourceName.empty()) {
            ENGINE_RUNTIME_ERROR("Resource declaration has invalid id");
        }

        std::string sourcePath = declarationNode.attribute("source").as_string();
        ResourceSourceFile source = { sourcePath };

        std::string resourceType = declarationNode.attribute("type").as_string();
        auto declarerIt = m_resourcesDeclarers.find(resourceType);

        if (declarerIt == m_resourcesDeclarers.end()) {
            ENGINE_RUNTIME_ERROR("Resource declaration has invalid type (" + resourceType + ")");
        }

        std::function resourceDeclarer = declarerIt->second;
        resourceDeclarer(resourceName, source, declarationNode);

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
