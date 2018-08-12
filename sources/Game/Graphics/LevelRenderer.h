#pragma once

#include <Engine\Components\Graphics\GraphicsResourceFactory.h>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>
#include "Light.h"

#include <Game\Graphics\Materials\BaseMaterial.h>
#include <Game\Graphics\Renderable.h> 

class LevelRenderer {
public:
	LevelRenderer(GraphicsContext* graphicsContext, GraphicsResourceFactory* graphicsResourceFactory);
	~LevelRenderer();

	void addLightSource(const Light* lightSource);
	void removeLightSource(const Light* lightSource);

	void setActiveCamera(const Camera* camera);
	void render();

	void registerBaseMaterial(BaseMaterial* baseMaterial);
	
	void addRenderableObject(Renderable* object);
	void removeRenderableObject(Renderable* object);

protected:
	const Camera* m_activeCamera;

	GraphicsContext * m_graphicsContext;
	GraphicsResourceFactory* m_graphicsResourceFactory;

	std::vector<const Light*> m_lightsSources;

	std::vector<BaseMaterial*> m_baseMaterials;
	std::vector<Renderable*> m_renderableObjects;
};