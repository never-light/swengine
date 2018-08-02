#pragma once

#include <Engine\Components\GUI\Window.h>
#include <Engine\Components\Debugging\Logger.h>

#include "graphicsApiType.h"
#include "GraphicsResourceFactory.h"

class GraphicsSystem {
public:
	GraphicsSystem(Window* window, GraphicsAPI apiType, Logger* logger);
	~GraphicsSystem();

	GraphicsResourceFactory* getResourceFactory() const;
	GraphicsContext* getGraphicsContext() const;

private:
	Window* m_window;

	GraphicsResourceFactory* m_resourceFactory;
	GraphicsContext* m_graphicsContext;
};