#include "EnvironmentRenderingSystem.h"
#include "Modules/ECS/ECS.h"

EnvironmentComponent::EnvironmentComponent()
{

}

void EnvironmentComponent::setEnvironmentMaterial(const std::shared_ptr<Material> material)
{
    m_environmentMaterial = material;
}

Material* EnvironmentComponent::getEnvironmentMaterial() const
{
    return m_environmentMaterial.get();
}

EnvironmentRenderingSystem::EnvironmentRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                                                       std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                                                       std::shared_ptr<Mesh> environmentMesh)
    : m_graphicsContext(graphicsContext),
      m_sharedGraphicsState(sharedGraphicsState),
      m_environmentMesh(environmentMesh)
{

}

EnvironmentRenderingSystem::~EnvironmentRenderingSystem()
{

}

void EnvironmentRenderingSystem::configure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void EnvironmentRenderingSystem::unconfigure(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);
}

void EnvironmentRenderingSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);
}

void EnvironmentRenderingSystem::render(GameWorld* gameWorld)
{
    GameObject* environmentObject = gameWorld->findGameObject([] (GameObject* obj) {
        return obj->hasComponent<EnvironmentComponent>();
    });

    SW_ASSERT(environmentObject != nullptr);

    Material* material = environmentObject->getComponent<EnvironmentComponent>()->getEnvironmentMaterial();

    Camera* camera = m_sharedGraphicsState->getActiveCamera().get();

    if (camera != nullptr) {
        GLShader* vertexShader = material->getGpuMaterial().getShadersPipeline()->getShader(GL_VERTEX_SHADER);

        if (vertexShader->hasParameter("scene.worldToCamera")) {
            glm::mat4 untranslatedViewMatrix = camera->getViewMatrix();
            untranslatedViewMatrix[3] = glm::vec4(0, 0, 0, 1);

            vertexShader->setParameter("scene.worldToCamera", untranslatedViewMatrix);
            vertexShader->setParameter("scene.cameraToProjection", camera->getProjectionMatrix());
        }
    }

    m_sharedGraphicsState->getFrameStats().increaseSubMeshesCount(1);
    m_sharedGraphicsState->getFrameStats().increasePrimitivesCount(m_environmentMesh->getSubMeshIndicesCount(0) / 3);

    m_graphicsContext->executeRenderTask(RenderTask{ &material->getGpuMaterial(),
                                                     m_environmentMesh->getGeometryStore(),
                                                     0, m_environmentMesh->getSubMeshIndicesCount(0) });
}
