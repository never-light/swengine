#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <experimental\filesystem>
#include <filesystem>

#include <Engine\types.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>

#include "Resource.h"
#include "HoldingResource.h"
#include "ResourceLoader.h"

class ResourceManager {
public:
	ResourceManager(GraphicsResourceFactory* graphicsResourceFactory);
	~ResourceManager();

	template<class T>
	T* load(const std::string& filename);

	template<class T>
	T* load(const std::string& filename, const std::string& alias);

	template<class T> 
	T* getResource(const std::string& alias);

	bool isResourceLoaded(const std::string& alias) const;
	void registerResource(const std::string& alias, Resource* resource);
	
	void registerResourceLoader(ResourceLoader* resourceLoader, const std::string& extension);
	void registerResourceLoader(ResourceLoader* resourceLoader, const std::vector<std::string>& extensions);

private:
	template<class T>
	T* loadAndCacheResource(const std::string& filename, const std::string& alias);

private:
	ResourceLoader* getResourceLoaderByFileName(const std::string& filename);

private:
	std::unordered_map<std::string, std::unique_ptr<Resource>> m_resources;
	std::unordered_map<std::string, ResourceLoader*> m_resourceLoaders;
private:
	GraphicsResourceFactory* m_graphicsResourceFactory;
};

template<class T>
inline T * ResourceManager::load(const std::string & filename)
{
	auto resourceIt = m_resources.find(filename);
	if (resourceIt != m_resources.end())
		return getResource<T>(filename);

	return loadAndCacheResource<T>(filename, filename);
}

template<class T>
inline T * ResourceManager::load(const std::string & filename, const std::string & alias)
{
	auto resourceIt = m_resources.find(alias);
	if (resourceIt != m_resources.end())
		return getResource<T>(alias);

	return loadAndCacheResource<T>(filename, alias);
}

template<class T>
inline T * ResourceManager::getResource(const std::string & alias)
{
	Resource* resource = m_resources.at(alias).get();

	if (HoldingResource<T>* holdingResource = dynamic_cast<HoldingResource<T>*>(resource))
		return holdingResource->getHoldedResource();

	return dynamic_cast<T*>(resource);

}

template<class T>
inline T * ResourceManager::loadAndCacheResource(const std::string & filename, const std::string & alias)
{
	ResourceLoader* loader = getResourceLoaderByFileName(filename);

	if (loader == nullptr)
		throw std::exception(("Resource loading error (unknown type): " + filename).c_str());

	Resource* resource = loader->load(filename);

	m_resources.insert({ alias, std::unique_ptr<Resource>(resource) });

	return getResource<T>(alias);
}
