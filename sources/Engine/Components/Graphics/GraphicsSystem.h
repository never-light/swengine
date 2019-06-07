#pragma once

#include <Engine/Components/Debugging/Logger.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsDevice.h>
#include <Engine/Components/Platform/Base/BaseWindow.h>
#include <Engine/config.h>

class GraphicsSystem {
public:
	GraphicsSystem(std::shared_ptr<sw::platform::base::Window> window, sw::GraphicsAPI apiType, Logger* logger);
	~GraphicsSystem();

	std::shared_ptr<GraphicsContext> getImmediateGraphicsContext() const;
	GraphicDevice* getGraphicDevice() const;

	void update(float delta);

private:
	std::shared_ptr<sw::platform::base::Window> m_window;
	std::unique_ptr<GraphicDevice> m_graphicsDevice;
};
