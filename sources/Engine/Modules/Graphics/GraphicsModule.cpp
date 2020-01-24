#include "precompiled.h"
#pragma hdrstop

#include "GraphicsModule.h"

GraphicsModule::GraphicsModule(SDL_Window* window)
    : m_graphicsContext(new GLGraphicsContext(window))
{

}

GraphicsModule::~GraphicsModule()
{

}

std::shared_ptr<GLGraphicsContext> GraphicsModule::getGraphicsContext() const
{
    return m_graphicsContext;
}
