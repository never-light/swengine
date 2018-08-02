#include "ResourceManager.h"

#include <unordered_set>

#include "TextureLoader.h"
#include "GpuProgramLoader.h"
#include "FontLoader.h"

ResourceManager::ResourceManager(GraphicsResourceFactory* graphicsResourceFactory)
	: m_graphicsResourceFactory(graphicsResourceFactory)
{
	registerResourceLoader(new TextureLoader(graphicsResourceFactory), 
		{ "png", "jpg", "tga" } );

	registerResourceLoader(new GpuProgramLoader(graphicsResourceFactory), "fx");
	registerResourceLoader(new FontLoader(graphicsResourceFactory), "font");
}

ResourceManager::~ResourceManager() {
	std::unordered_set<ResourceLoader*> resourceLoaders;

	for (const auto& it : m_resourceLoaders)
		resourceLoaders.insert(it.second);

	for (auto& loader : resourceLoaders)
		delete loader;
}

bool ResourceManager::isResourceLoaded(const std::string& name) const
{
	return m_resources.find(name) != m_resources.end();
}

void ResourceManager::registerResource(const std::string & alias, Resource * resource)
{
	m_resources.insert({ alias, std::unique_ptr<Resource>(resource) });
}

void ResourceManager::registerResourceLoader(ResourceLoader* resourceLoader, const std::string & extension)
{
	m_resourceLoaders.insert({ extension, resourceLoader });
}

void ResourceManager::registerResourceLoader(ResourceLoader* resourceLoader, const std::vector<std::string>& extensions)
{
	for (const auto& extension : extensions)
		registerResourceLoader(resourceLoader, extension);
}

ResourceLoader * ResourceManager::getResourceLoaderByFileName(const std::string& filename)
{
	namespace fs = std::experimental::filesystem;

	fs::path path(filename);

	std::string extension = path.extension().string();

	if (extension.empty())
		return nullptr;

	extension = extension.erase(0, 1);

	auto resourceLoaderIt = m_resourceLoaders.find(extension);
	if (resourceLoaderIt != m_resourceLoaders.end())
		return resourceLoaderIt->second;

	return nullptr;
}