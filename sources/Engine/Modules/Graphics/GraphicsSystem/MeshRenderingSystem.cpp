#include "MeshRenderingSystem.h"

#include "Modules/ECS/ECS.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

MeshRenderingSystem::MeshRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext)
    : m_graphicsContext(graphicsContext)
{

}

MeshRenderingSystem::~MeshRenderingSystem()
{

}

void MeshRenderingSystem::configure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void MeshRenderingSystem::unconfigure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void MeshRenderingSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);
}

void MeshRenderingSystem::render(GameWorld* gameWorld)
{
    for (auto obj : gameWorld->allWith<MeshRendererComponent, TransformComponent>()) {
        ARG_UNUSED(obj);

        int b = 2 + 2;

        ARG_UNUSED(b);
    }
}
