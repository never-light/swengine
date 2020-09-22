#include "precompiled.h"

#pragma hdrstop

#include "GLGraphicsContext.h"

#include <spdlog/spdlog.h>

#include "Modules/Graphics/GraphicsSystem/SharedGraphicsState.h"
#include "Exceptions/exceptions.h"
#include "options.h"

GLGraphicsContext::GLGraphicsContext(SDL_Window* window)
  : m_window(window)
{
  spdlog::info("Creating OpenGL context");

  m_glContext = SDL_GL_CreateContext(m_window);

  if (m_glContext == nullptr) {
    THROW_EXCEPTION(EngineRuntimeException, std::string(SDL_GetError()));
  }

  if (gl3wInit()) {
    THROW_EXCEPTION(EngineRuntimeException, "Failed to initialize OpenGL");
  }

  if (!gl3wIsSupported(4, 5)) {
    THROW_EXCEPTION(EngineRuntimeException, "Failed to load OpenGL functions");
  }

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
  glDebugMessageCallback(reinterpret_cast<GLDEBUGPROCARB>(&debugOutputCallback), this);

  int bufferWidth, bufferHeight;
  SDL_GetWindowSize(m_window, &bufferWidth, &bufferHeight);

  m_defaultFramebuffer = std::unique_ptr<GLFramebuffer>(new GLFramebuffer(bufferWidth, bufferHeight));

  m_ndcTexturedQuad = std::make_unique<GLGeometryStore>(
    std::vector<VertexPos3Norm3UV>{
      {{-1.0f, 1.0f, 1.0f}, glm::vec3(), {0.0f, 1.0f}},
      {{-1.0f, -1.0f, 1.0f}, glm::vec3(), {0.0f, 0.0f}},
      {{1.0f, 1.0f, 1.0f}, glm::vec3(), {1.0f, 1.0f}},
      {{1.0f, -1.0f, 1.0f}, glm::vec3(), {1.0f, 0.0f}},
    }, std::vector<std::uint16_t>{0, 2, 1, 1, 2, 3});

  spdlog::info("OpenGL context is created");
}

GLGraphicsContext::~GLGraphicsContext()
{
  SDL_GL_DeleteContext(m_glContext);
}

void GLGraphicsContext::enableWritingToDepthBuffer()
{
  glDepthMask(GL_TRUE);
  applyContextChange();
}

void GLGraphicsContext::disableWritingToDepthBuffer()
{
  glDepthMask(GL_FALSE);
  applyContextChange();
}

void GLGraphicsContext::enableScissorTest()
{
  glEnable(GL_SCISSOR_TEST);
  applyContextChange();
}

void GLGraphicsContext::disableScissorTest()
{
  glDisable(GL_SCISSOR_TEST);
  applyContextChange();
}

void GLGraphicsContext::setScissorRectangle(const RectI& rectangle)
{
  glScissor(rectangle.getOriginX(),
    m_defaultFramebuffer->getWidth() - rectangle.getOriginY() - rectangle.getHeight(),
    rectangle.getWidth(), rectangle.getHeight());

  applyContextChange();
}

void GLGraphicsContext::swapBuffers()
{
  SDL_GL_SwapWindow(m_window);
}

void GLGraphicsContext::setDepthTestMode(DepthTestMode mode)
{
  switch (mode) {
    case DepthTestMode::Disabled:
      glDisable(GL_DEPTH_TEST);
      break;

    case DepthTestMode::LessOrEqual:
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      break;

    case DepthTestMode::Less:
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      break;

    case DepthTestMode::NotEqual:
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_NOTEQUAL);

    default:
      break;
  }

  applyContextChange();
}

void GLGraphicsContext::setFaceCullingMode(FaceCullingMode mode)
{
  switch (mode) {
    case FaceCullingMode::Disabled:
      glDisable(GL_CULL_FACE);
      break;

    case FaceCullingMode::Back:
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      break;

    case FaceCullingMode::Front:
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      break;

    default:
      break;
  }

  applyContextChange();
}

void GLGraphicsContext::setPolygonFillingMode(PolygonFillingMode mode)
{
  switch (mode) {
    case PolygonFillingMode::Fill:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;

    case PolygonFillingMode::Wireframe:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;

    default:
      break;
  }

  applyContextChange();
}

void GLGraphicsContext::setBlendingMode(BlendingMode mode)
{
  switch (mode) {
    case BlendingMode::Disabled:
      glDisable(GL_BLEND);
      break;

    case BlendingMode::Alpha_OneMinusAlpha:
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;

    default:
      break;
  }

  applyContextChange();
}

void GLGraphicsContext::setDepthWritingMode(DepthWritingMode mode)
{
  switch (mode) {
    case DepthWritingMode::Disabled:
      glDepthMask(GL_FALSE);
      break;

    case DepthWritingMode::Enabled:
      glDepthMask(GL_TRUE);
      break;

    default:
      break;
  }

  applyContextChange();
}

void GLGraphicsContext::applyMaterial(const GLMaterial& material)
{
  // Blending mode
  setBlendingMode(material.getBlendingMode());

  // Depth test
  setDepthTestMode(material.getDepthTestMode());

  // Face culling mode
  setFaceCullingMode(material.getFaceCullingMode());

  // Polygon filling mode
  setPolygonFillingMode(material.getPolygonFillingMode());

  // Depth writing mode
  setDepthWritingMode(material.getDepthWritingMode());

  // Parameters
  GLShadersPipeline* shadersPipeline = material.getShadersPipeline().get();

  for (auto&[parameterName, genericValue] : material.m_parameters) {
    GLShader* shader = shadersPipeline->getShader(genericValue.shaderType);

    const auto& rawValue = genericValue.value;

    if (auto intVal = std::get_if<int>(&rawValue)) {
      shader->setParameter(parameterName, *intVal);
    }
    else if (auto floatVal = std::get_if<float>(&rawValue)) {
      shader->setParameter(parameterName, *floatVal);
    }
    else if (auto vec3Val = std::get_if<glm::vec3>(&rawValue)) {
      shader->setParameter(parameterName, *vec3Val);
    }
    else if (auto vec4Val = std::get_if<glm::vec4>(&rawValue)) {
      shader->setParameter(parameterName, *vec4Val);
    }
    else if (auto mat3Val = std::get_if<glm::mat3>(&rawValue)) {
      shader->setParameter(parameterName, *mat3Val);
    }
    else if (auto mat4Val = std::get_if<glm::mat4>(&rawValue)) {
      shader->setParameter(parameterName, *mat4Val);
    }
    else if (auto textureVal = std::get_if<GLMaterial::TextureParameter>(&rawValue)) {
      shader->setParameter(parameterName, *(textureVal->texture), textureVal->slotIndex);
    }
    else {
      THROW_EXCEPTION(EngineRuntimeException, "Failed to set generic material parameter");
    }
  }
}

void GLGraphicsContext::executeRenderTask(const RenderTask& task)
{
  if (m_currentFramebuffer != task.framebuffer) {
    m_currentFramebuffer = task.framebuffer;

    if (task.framebuffer == nullptr) {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else {
      glBindFramebuffer(GL_FRAMEBUFFER, task.framebuffer->getGLHandle());
    }
  }

  GLShadersPipeline* shadersPipeline = task.material->getShadersPipeline().get();
  glBindProgramPipeline(shadersPipeline->m_programPipeline);

  /* Compare only memory addresses */
  if (m_currentMaterial != task.material) {
    applyMaterial(*task.material);

    m_currentMaterial = task.material;
  }

  task.geometryStore->drawRange(task.startOffset, task.partsCount, task.primitivesType);
}

GLFramebuffer& GLGraphicsContext::getDefaultFramebuffer() const
{
  return *m_defaultFramebuffer.get();
}

GLGeometryStore& GLGraphicsContext::getNDCTexturedQuad() const
{
  return *m_ndcTexturedQuad.get();
}

void GLGraphicsContext::applyContextChange()
{
  resetMaterial();
}

void GLGraphicsContext::resetMaterial()
{
  m_currentMaterial = nullptr;
}

void APIENTRY GLGraphicsContext::debugOutputCallback(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  GLvoid* userParam)
{
  ARG_UNUSED(source);
  ARG_UNUSED(length);
  ARG_UNUSED(userParam);

  // Skip some debug information from GPU
  if constexpr (!LOG_GPU_ADDITIONAL_DEBUG_INFO_MESSAGES) {
    if (id == 131185) {
      return;
    }
  }

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
      debugMessage += "performance";
      break;
    case GL_DEBUG_TYPE_OTHER:
      debugMessage += "common";
      break;
    default:
      debugMessage += fmt::format("Unknown ({})", type);
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
    default:
      debugMessage += fmt::format("Unknown ({})", severity);
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
    default:
      spdlog::debug(debugMessage);
      break;
  }
}
