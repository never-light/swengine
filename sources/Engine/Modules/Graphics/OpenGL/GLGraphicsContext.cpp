#include "GLGraphicsContext.h"

#include <spdlog/spdlog.h>
#include "Exceptions/EngineRuntimeException.h"
#include "Modules/Graphics/GraphicsSystem/SharedGraphicsState.h"

GLGraphicsContext::GLGraphicsContext(SDL_Window* window)
    : m_window(window)
{
    spdlog::info("Creating OpenGL context");

    m_glContext = SDL_GL_CreateContext(m_window);

    if (m_glContext == nullptr) {
        ENGINE_RUNTIME_ERROR(std::string(SDL_GetError()));
    }

    if (gl3wInit()) {
        ENGINE_RUNTIME_ERROR("Failed to initialize OpenGL");
    }

    if (!gl3wIsSupported(4, 5)) {
        ENGINE_RUNTIME_ERROR("Failed to load OpenGL functions");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageCallback(reinterpret_cast<GLDEBUGPROCARB>(&debugOutputCallback), this);

    spdlog::info("OpenGL context is created");
}

GLGraphicsContext::~GLGraphicsContext()
{
    SDL_GL_DeleteContext(m_glContext);
}

void GLGraphicsContext::enableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void GLGraphicsContext::disableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

void GLGraphicsContext::setDepthTestFunction(GLenum function)
{
    glDepthFunc(function);
}

void GLGraphicsContext::enableWritingToDepthBuffer()
{
    glDepthMask(GL_TRUE);
}

void GLGraphicsContext::disableWritingToDepthBuffer()
{
    glDepthMask(GL_FALSE);
}

void GLGraphicsContext::enableFaceCulling()
{
    glEnable(GL_CULL_FACE);
}

void GLGraphicsContext::disableFaceCulling()
{
    glDisable(GL_CULL_FACE);
}

void GLGraphicsContext::setFaceCullingMode(GLenum mode)
{
    glCullFace(mode);
}

void GLGraphicsContext::enableBlending()
{
    glEnable(GL_BLEND);
}

void GLGraphicsContext::disableBlending()
{
    glDisable(GL_BLEND);
}

void GLGraphicsContext::setBlendingMode(GLenum sourceAffect, GLenum destinationAffect)
{
    glBlendFunc(sourceAffect, destinationAffect);
}

void GLGraphicsContext::setBlendingEquation(GLenum equation)
{
    glBlendEquation(equation);
}

void GLGraphicsContext::enableScissorTest()
{
    glEnable(GL_SCISSOR_TEST);
}

void GLGraphicsContext::disableScissorTest()
{
    glDisable(GL_SCISSOR_TEST);
}

void GLGraphicsContext::enableWireframeRendering()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GLGraphicsContext::disableWireframeRendering()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLGraphicsContext::setScissorRectangle(const Rect& rectangle)
{
    glScissor(rectangle.getOriginX(),
        getBufferWidth() - rectangle.getOriginY() - rectangle.getHeight(),
        rectangle.getWidth(), rectangle.getHeight());
}

void GLGraphicsContext::swapBuffers()
{
    SDL_GL_SwapWindow(m_window);
}

int GLGraphicsContext::getBufferWidth() const
{
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);

    return width;
}

int GLGraphicsContext::getBufferHeight() const
{
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);

    return height;
}

void GLGraphicsContext::executeRenderTask(const RenderTask& task)
{
    GLShadersPipeline* shadersPipeline = task.shadersPipeline;

    glBindProgramPipeline(shadersPipeline->m_programPipeline);
    task.geometryStore->drawRange(task.startOffset, task.partsCount);
}

void APIENTRY GLGraphicsContext::debugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
    ARG_UNUSED(source);
    ARG_UNUSED(length);
    ARG_UNUSED(userParam);

    std::string debugMessage = "[OpenGL] ";

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        debugMessage += "error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        debugMessage += "deprecated behavior";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        debugMessage += "undefined behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        debugMessage += "portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        debugMessage += "perfomance";
        break;
    case GL_DEBUG_TYPE_OTHER:
        debugMessage += "common";
        break;
    }

    debugMessage += " (" + std::to_string(id) + ", ";

    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        debugMessage += "low";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        debugMessage += "medium";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        debugMessage += "high";
        break;
    }

    debugMessage += ") " + std::string(message);

    //GLGraphicsContext* context = static_cast<GLGraphicsContext*>(userParam);

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    case GL_DEBUG_TYPE_PORTABILITY:
        spdlog::error(debugMessage);
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    case GL_DEBUG_TYPE_PERFORMANCE:
        spdlog::warn(debugMessage);
        break;
    case GL_DEBUG_TYPE_OTHER:
        spdlog::debug(debugMessage);
        break;
    }
}
