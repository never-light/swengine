#include "GameLevel.h"

#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/EnvironmentRenderingSystem.h>

#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Graphics/Resources/TextureResource.h>
#include <Engine/Modules/Graphics/Resources/MaterialResource.h>

#include "Game/PlayerComponent.h"

GameLevel::GameLevel(std::shared_ptr<GameWorld> gameWorld,
                     std::shared_ptr<GLGraphicsContext> graphicsContext,
                     std::shared_ptr<ResourceManager> resourceManager)
    : m_gameWorld(gameWorld),
      m_graphicsContext(graphicsContext),
      m_resourceManager(resourceManager)
{
    m_player = m_gameWorld->createGameObject();
    m_player->addComponent<TransformComponent>();
    m_player->addComponent<CameraComponent>();
    m_player->addComponent<PlayerComponent>();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(100.0f);
    camera->setAspectRatio(m_graphicsContext->getDefaultFramebuffer().getAspectRatio());
    camera->setFOVy(glm::pi<float>() / 4);

    camera->getTransform()->setPosition(0, 0, 0);
    camera->getTransform()->fixYAxis();
    camera->getTransform()->lookAt(0, 0, 1);

    m_player->getComponent<CameraComponent>()->setCamera(camera);

    // Game objects
    std::shared_ptr<Material> material = m_resourceManager->getResourceFromInstance<MaterialResource>("phong_material")->
            getMaterial();

    {
        GameObject* obj = m_gameWorld->createGameObject();
        auto transformHandle = obj->addComponent<TransformComponent>();

        transformHandle->getTransform()->move(0.0f, 0.0f, 0.0f);

        std::shared_ptr<Mesh> cubeGeometry = m_resourceManager->getResourceFromInstance<MeshResource>("simple_mesh")->getMesh();

        auto componentHandle = obj->addComponent<MeshRendererComponent>();
        componentHandle->setMeshInstance(cubeGeometry);
        componentHandle->setMaterialsInstances({ material });

        componentHandle->updateBounds(*transformHandle->getTransform());
    }

    // Environment
    {
        GameObject* environmentObj = m_gameWorld->createGameObject();
        auto environment = environmentObj->addComponent<EnvironmentComponent>();

        environment->setEnvironmentMaterial(m_resourceManager->getResourceFromInstance<MaterialResource>("test_scene_environment")->getMaterial());
    }
}

GameObject* GameLevel::getPlayer() const
{
    return m_player;
}
