#pragma once

#include <Engine/Components/GUI/Window.h>
#include <Engine/Components/Debugging/Logger.h>

#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

#include "graphicsApiType.h"

class GraphicsSystem {
public:
	GraphicsSystem(Window* window, GraphicsAPI apiType, Logger* logger);
	~GraphicsSystem();

	GraphicsContext* getGraphicsContext() const;
	
	void update(float delta);

private:
	Window* m_window;

	GraphicsContext* m_graphicsContext;
};