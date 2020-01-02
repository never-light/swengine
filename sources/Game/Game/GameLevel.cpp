#include "GameLevel.h"

#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>

#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Graphics/Resources/TextureResource.h>

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
    camera->setAspectRatio(static_cast<float>(m_graphicsContext->getBufferWidth())
                           / m_graphicsContext->getBufferHeight());
    camera->setFOVy(glm::pi<float>() / 4);

    camera->getTransform()->setPosition(0, 0, 0);
    camera->getTransform()->fixYAxis();
    camera->getTransform()->lookAt(0, 0, 1);

    m_player->getComponent<CameraComponent>()->setCamera(camera);

    // Game objects

    std::shared_ptr<GLShadersPipeline> phongPipeline = std::make_shared<GLShadersPipeline>(
                m_resourceManager->getResourceFromInstance<ShaderResource>("phong_vertex_shader")->getShader(),
                m_resourceManager->getResourceFromInstance<ShaderResource>("phong_fragment_shader")->getShader(), nullptr);

    std::shared_ptr<GLMaterial> material = std::make_shared<GLMaterial>();
    material->setShadersPipeline(phongPipeline);
    material->setBlendingMode(BlendingMode::Alpha_OneMinusAlpha);
    material->setDepthTestMode(DepthTestMode::Enabled);
    material->setFaceCullingMode(FaceCullingMode::Back);
    material->setPolygonFillingMode(PolygonFillingMode::Fill);

    std::shared_ptr<GLTexture> texture = m_resourceManager->getResourceFromInstance<TextureResource>("simple_texture")->getTexture();
    material->setShaderParameter(GL_FRAGMENT_SHADER, "tex", GLMaterial::TextureParameter(texture, 0));

    GameObject* obj = m_gameWorld->createGameObject();
    auto transformHandle = obj->addComponent<TransformComponent>();

    transformHandle->getTransform()->move(0.0f, 0.0f, 0.0f);

    std::shared_ptr<Mesh> cubeGeometry = m_resourceManager->getResourceFromInstance<MeshResource>("simple_mesh")->getMesh();

    auto componentHandle = obj->addComponent<MeshRendererComponent>();
    componentHandle->setMeshInstance(cubeGeometry);
    componentHandle->setMaterialsInstances({ material });

}

GameObject* GameLevel::getPlayer() const
{
    return m_player;
}
