#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <experimental/filesystem>
#include <filesystem>

#include <Engine/Utils/typeid.h>

#include <Engine/types.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

#include <Engine/Components/ResourceManager/RawImage.h>
#include <Engine/Components/GUI/Font.h>

#include <Engine/Utils/files.h>

#include "ResourceLoader.h"
#include "RawImageLoader.h"
#include "ResourceLoadingException.h"

#include "ResourceInstance.h"
#include "ResourceHandle.h"

class ResourceManager {
public:
	ResourceManager(GraphicsContext* graphicsContext);
	~ResourceManager();

	template<class T>
	void registerResourceLoader(ResourceLoader* resourceLoader);

	template<class T>
	void unregisterResourceLoader();

	template<class ResourceType>
	ResourceHandle<ResourceType> loadResource(const std::string& path);

	template<class ResourceType>
	ResourceHandle<ResourceType> loadResource(const std::string& path, const std::string& alias);

	template<class ResourceType>
	ResourceHandle<ResourceType> loadResource(const std::string& path, const std::string& alias, std::optional<std::any> options);

	template<class ResourceType>
	ResourceHandle<ResourceType> getResource(const std::string& alias);

	bool isResourceLoaded(const std::string& alias) const;
private:
	std::unordered_map<TypeId, ResourceLoader*> m_resourceLoaders;
	std::unordered_map<std::string, BaseResourceInstance*> m_resources;

private:
	GraphicsContext* m_graphicsContext;
};

template<class T>
inline void ResourceManager::registerResourceLoader(ResourceLoader * resourceLoader)
{
	m_resourceLoaders.insert({ getTypeId<T>(), resourceLoader });
}

template<class T>
inline void ResourceManager::unregisterResourceLoader()
{
	m_resourceLoaders.erase(getTypeId<T>());
}

template<class ResourceType>
inline ResourceHandle<ResourceType> ResourceManager::loadResource(const std::string& path)
{
	return loadResource<ResourceType>(path, path, std::optional<std::any>());
}

template<class ResourceType>
inline ResourceHandle<ResourceType> ResourceManager::loadResource(const std::string & path, const std::string & alias)
{
	return loadResource<ResourceType>(path, alias, std::optional<std::any>());
}

template<class ResourceType>
inline ResourceHandle<ResourceType> ResourceManager::loadResource(const std::string & path, 
	const std::string & alias, std::optional<std::any> options)
{
	if (!FilesUtils::isExists(path))
		throw ResourceLoadingException(ResourceLoadingError::FileNotAvailable, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	ResourceLoader* loader = m_resourceLoaders[getTypeId<ResourceType>()];

	if (loader == nullptr)
		throw ResourceLoadingException(ResourceLoadingError::InvalidType, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	BaseResourceInstance* resource = loader->load(path, options);

	m_resources.insert({ alias, resource });

	return ResourceHandle<ResourceType>(static_cast<ResourceInstance<ResourceType>*>(resource));
}

template<class ResourceType>
inline ResourceHandle<ResourceType> ResourceManager::getResource(const std::string & alias)
{
	auto resourceIt = m_resources.find(alias);

	if (resourceIt == m_resources.end())
		return ResourceHandle<ResourceType>(nullptr);

	ResourceInstance<ResourceType>* resourceInstnace =
		dynamic_cast<ResourceInstance<ResourceType>*>(resourceIt->second);

	return ResourceHandle<ResourceType>(resourceInstnace);
}
