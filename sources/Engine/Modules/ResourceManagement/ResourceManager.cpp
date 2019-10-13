#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::declareResource(const std::string &resourceId, const ResourceSource &source)
{
    m_resourcesSources.insert({ resourceId, source });
}

const ResourceSource& ResourceManager::getResourceSource(const std::string &resourceId)
{
    return m_resourcesSources.at(resourceId);
}
