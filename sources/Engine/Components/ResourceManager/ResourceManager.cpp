#include "ResourceManager.h"

#include <unordered_set>

#include "TextureLoader.h"
#include "GpuProgramLoader.h"
#include "FontLoader.h"
#include "MeshLoader.h"
#include "AnimationLoader.h"
#include "SoundLoader.h"

ResourceManager::ResourceManager(GraphicsContext* graphicsContext)
	: m_graphicsContext(graphicsContext)
{
	registerResourceLoader<Texture>(new TextureLoader(graphicsContext));
	registerResourceLoader<GpuProgram>(new GpuProgramLoader(graphicsContext));
	registerResourceLoader<Font>(new FontLoader(graphicsContext));
	registerResourceLoader<Mesh>(new MeshLoader(graphicsContext));
	registerResourceLoader<Animation>(new AnimationLoader());
	registerResourceLoader<RawImage>(new RawImageLoader());
	registerResourceLoader<Sound>(new SoundLoader());
}

ResourceManager::~ResourceManager() {
	for (auto&[alias, resourceInstance] : m_resources)
		delete resourceInstance;

	for (auto& [ typeId, resourceLoader ] : m_resourceLoaders)
		delete resourceLoader;
}

bool ResourceManager::isResourceLoaded(const std::string& alias) const
{
	return m_resources.find(alias) != m_resources.end();
}