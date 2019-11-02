#include "GameApplication.h"

#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Exceptions/EngineRuntimeException.h>

#include <spdlog/spdlog.h>

GameApplication::GameApplication(int argc, char* argv[])
    : BaseGameApplication(argc, argv, "Game", 640, 480)
{

}

GameApplication::~GameApplication()
{

}

void GameApplication::render()
{
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_graphicsModule->getGraphicsContext()->executeRenderTask({ m_triangleGeometry, m_triangleShadersPipeline });
}

void GameApplication::load()
{
    auto resourceMgr = m_resourceManagementModule->getResourceManager();

    resourceMgr->declareResourceType<ShaderResource>();

    resourceMgr->declareResource<ShaderResource>("simple_vertex_shader", ResourceSource{
        ResourceSourceRawString{
            "#version 450 core\n"
            "layout (location = 0) in vec3 pos;"
            "out gl_PerVertex { vec4 gl_Position; };"
            "void main() {"
            "  gl_Position = vec4(pos, 1.0);"
            "}"
        },

        ShaderResourceParameters(GL_VERTEX_SHADER)
    });

    auto resInst = resourceMgr->getResourceInstance("simple_vertex_shader");


    auto res = resInst->getResource<ShaderResource>();

    m_triangleGeometry = new GLGeometryStore({
        { glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(), glm::vec2() },
        { glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(), glm::vec2() },
        { glm::vec3(0.5f, -0.5f, 1.0f), glm::vec3(), glm::vec2() },
    });

    const char* fragment_shader =
    "#version 450 core\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";

    std::shared_ptr<GLShader> fragmentShader = std::make_shared<GLShader>(GL_FRAGMENT_SHADER, fragment_shader);

    m_triangleShadersPipeline = new GLShadersPipeline(
                res->getShader(), fragmentShader, nullptr);
}
