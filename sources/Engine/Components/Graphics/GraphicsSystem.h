#pragma once

#include <Engine\Components\GUI\Window.h>

#include "graphicsApiType.h"
#include "GraphicsResourceFactory.h"

class GraphicsSystem {
public:
	GraphicsSystem(Window* window, GraphicsAPI apiType);
	~GraphicsSystem();

	GraphicsResourceFactory* getResourceFactory() const;
	GraphicsContext* getGraphicsContext() const;

private:
	Window* m_window;

	GraphicsResourceFactory* m_resourceFactory;
	GraphicsContext* m_graphicsContext;
};