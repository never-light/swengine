#include "GameApplication.h"

#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Graphics/Resources/MeshResource.h>
#include <Engine/Exceptions/EngineRuntimeException.h>

#include <spdlog/spdlog.h>

#include "Game/Game/PlayerComponent.h"

GameApplication::GameApplication(int argc, char* argv[])
    : BaseGameApplication(argc, argv, "Game", 640, 480)
{

}

GameApplication::~GameApplication()
{

}

void GameApplication::render()
{
    //auto resourceMgr = m_resourceManagementModule->getResourceManager();

    //auto resInst2 = resourceMgr->getResourceInstance("simple_mesh");
    //auto res2 = resInst2->getResource<MeshResource>();



    /*m_graphicsModule->getGraphicsContext()->executeRenderTask({ m_triangleGeometry->m_geometryStore.get(), m_triangleShadersPipeline,
                                                              m_triangleGeometry->getSubMeshIndicesOffset(1),
                                                              m_triangleGeometry->getSubMeshIndicesCount(1)});*/
}

void GameApplication::load()
{
    auto resourceMgr = m_resourceManagementModule->getResourceManager();

    resourceMgr->declareResourceType<ShaderResource>();
    resourceMgr->declareResourceType<MeshResource>();

    resourceMgr->declareResource<ShaderResource>("simple_vertex_shader", ResourceSource{
        ResourceSourceFile{ "../resources/shaders/test.glsl" },

        ShaderResourceParameters(GL_VERTEX_SHADER)
    });

    resourceMgr->declareResource<MeshResource>("simple_mesh", ResourceSource{
        ResourceSourceFile{ "test.mesh" },
        MeshResourceSourceParameters()
    });


    auto resInst = resourceMgr->getResourceInstance("simple_vertex_shader");
    auto res = resInst->getResource<ShaderResource>();

    m_triangleGeometry = std::make_shared<Mesh>();

    m_triangleGeometry->setVertices({
        glm::vec3(0.0f, 0.5f, 1.0f),
        glm::vec3(-0.5f, -0.5f, 1.0f),
        glm::vec3(0.5f, -0.5f, 1.0f),

        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(-0.2f, -0.2f, 1.0f),
        glm::vec3(0.1f, -0.3f, 1.0f),
    });

    m_triangleGeometry->setNormals({ {}, {}, {}, {}, {}, {} });
    m_triangleGeometry->setUV({ {}, {}, {}, {}, {}, {} });

    m_triangleGeometry->addSubMesh({ 0, 1, 2, 3, 4, 5 });
    m_triangleGeometry->addSubMesh({ 1, 2, 3 });


    const char* fragment_shader =
    "#version 450 core\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";

    std::shared_ptr<GLShader> fragmentShader = std::make_shared<GLShader>(GL_FRAGMENT_SHADER, fragment_shader);

    m_triangleShadersPipeline = std::make_shared<GLShadersPipeline>(
                res->getShader(), fragmentShader, nullptr);

    m_material = std::make_shared<Material>();
    m_material->setShadersPipeline(m_triangleShadersPipeline);

    GameObject* obj = m_gameWorld->createGameObject();
    auto transformHandle = obj->addComponent<TransformComponent>();

    transformHandle->getTransform()->move(0.0f, 0.3f, 0.0f);

    auto componentHandle = obj->addComponent<MeshRendererComponent>();
    componentHandle->setMeshInstance(m_triangleGeometry);
    componentHandle->setMaterialsInstances({ m_material, m_material });

    GameObject* player = m_gameWorld->createGameObject();
    player->addComponent<TransformComponent>();
    player->addComponent<CameraComponent>();
    player->addComponent<PlayerComponent>();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(100.0f);
    camera->setAspectRatio(static_cast<float>(m_graphicsModule->getGraphicsContext()->getBufferWidth())
                           / m_graphicsModule->getGraphicsContext()->getBufferHeight());
    camera->setFOVy(glm::pi<float>() / 4);

    camera->getTransform()->setPosition(0, 0, 0);
    camera->getTransform()->fixYAxis();
    camera->getTransform()->lookAt(0, 0, 1);

    player->getComponent<CameraComponent>()->setCamera(camera);
    m_sharedGraphicsState->setActiveCamera(camera);

    m_playerControlSystem = std::make_shared<PlayerControlSystem>(m_inputModule);
    m_gameWorld->addGameSystem(m_playerControlSystem);

    m_inputModule->registerAction("forward", KeyboardInputAction(SDLK_w));
    m_inputModule->registerAction("backward", KeyboardInputAction(SDLK_s));
    m_inputModule->registerAction("left", KeyboardInputAction(SDLK_a));
    m_inputModule->registerAction("right", KeyboardInputAction(SDLK_d));
}
