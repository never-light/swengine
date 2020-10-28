#include "precompiled.h"

#pragma hdrstop

#include "GLGraphicsContext.h"

#include <spdlog/spdlog.h>

#include "Modules/Graphics/GraphicsSystem/FrameStats.h"
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

  int framebufferWidth = bufferWidth;
  int framebufferHeight = bufferHeight;

  std::shared_ptr<GLTexture> mrtAlbedo = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGBA8);

  std::shared_ptr<GLTexture> mrtNormals = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGB16F);

  std::shared_ptr<GLTexture> mrtPositions = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGB32F);

  std::shared_ptr<GLTexture> depthStencil = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::Depth24Stencil8);

  m_deferredFramebuffer = std::make_unique<GLFramebuffer>(framebufferWidth,
    framebufferHeight,
    std::vector{mrtAlbedo, mrtNormals, mrtPositions},
    depthStencil);

  std::shared_ptr<GLTexture> forwardAlbedo = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGBA8);

  m_forwardFramebuffer = std::make_unique<GLFramebuffer>(framebufferWidth, framebufferHeight,
    std::vector{forwardAlbedo}, depthStencil);

  spdlog::info("OpenGL context is created");
}

GLGraphicsContext::~GLGraphicsContext()
{
  SDL_GL_DeleteContext(m_glContext);
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

void GLGraphicsContext::setupScissorsTest(ScissorsTestMode mode)
{
  switch (mode) {
    case ScissorsTestMode::Disabled:
      glDisable(GL_SCISSOR_TEST);
      break;

    case ScissorsTestMode::Enabled:
      glEnable(GL_SCISSOR_TEST);
      break;

    default:
      break;
  }

  applyContextChange();
}

void GLGraphicsContext::setupGraphicsScene(std::shared_ptr<GraphicsScene> graphicsScene)
{
  m_graphicsScene = std::move(graphicsScene);
}

void GLGraphicsContext::scheduleRenderTask(const RenderTask& task)
{
  m_renderingQueues[static_cast<size_t>(task.material->getRenderingStage())].push_back(task);
}

void GLGraphicsContext::executeRenderTasks()
{
  // TODO: get rid of buffers clearing and copying as possible
  //  For example, use depth swap trick to avoid depth buffer clearing

  glDisable(GL_SCISSOR_TEST);

  m_deferredFramebuffer->clearColor({0.0f, 0.0f, 0.0f, 0.0f}, 0);
  m_deferredFramebuffer->clearColor({0.0f, 0.0f, 0.0f, 0.0f}, 1);
  m_deferredFramebuffer->clearColor({0.0f, 0.0f, 0.0f, 0.0f}, 2);

  m_deferredFramebuffer->clearDepthStencil(1.0f, 0);

  glEnable(GL_SCISSOR_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFramebuffer->getGLHandle());

  if (!m_renderingQueues[0].empty()) {
    applyGpuState(m_renderingQueues[0][0].material->getGpuStateParameters());
  }

  executeRenderingStageQueue(RenderingStage::Deferred);

  GLShadersPipeline* accumulationPipeline = &m_deferredAccumulationMaterial->getShadersPipeline();
  GLShader* accumulationFragmentShader = accumulationPipeline->getShader(ShaderType::Fragment);
  const GLFramebuffer& deferredFramebuffer = *m_deferredFramebuffer;

  accumulationFragmentShader->setParameter("gBuffer.albedo",
    *deferredFramebuffer.getColorComponent(0), 0);

  accumulationFragmentShader->setParameter("gBuffer.normals",
    *deferredFramebuffer.getColorComponent(1), 1);

  accumulationFragmentShader->setParameter("gBuffer.positions",
    *deferredFramebuffer.getColorComponent(2), 2);

  glBindFramebuffer(GL_FRAMEBUFFER, m_forwardFramebuffer->getGLHandle());

  applyGpuState(m_deferredAccumulationMaterial->getGpuStateParameters());
  m_deferredAccumulationMaterial->getGLParametersBinder()->bindParameters(*accumulationPipeline);

  getNDCTexturedQuad().drawRange(0, 6, GL_TRIANGLES);

  executeRenderingStageQueue(RenderingStage::Forward);
  executeRenderingStageQueue(RenderingStage::ForwardDebug);
  executeRenderingStageQueue(RenderingStage::ForwardEnvironment);
  executeRenderingStageQueue(RenderingStage::PostProcess);
  executeRenderingStageQueue(RenderingStage::GUI);

  glDisable(GL_SCISSOR_TEST);

  m_defaultFramebuffer->clearColor({0.0f, 0.0f, 0.0f, 1.0f});
  m_defaultFramebuffer->clearDepthStencil(0.0f, 0);

  m_forwardFramebuffer->copyColor(*m_defaultFramebuffer);
  m_forwardFramebuffer->copyDepthStencil(*m_defaultFramebuffer);

  glEnable(GL_SCISSOR_TEST);
}

void GLGraphicsContext::setupDeferredAccumulationMaterial(std::shared_ptr<GLShadersPipeline> pipeline)
{
  GpuStateParameters gpuState;

  gpuState.setBlendingMode(BlendingMode::Disabled);
  gpuState.setDepthTestMode(DepthTestMode::NotEqual);
  gpuState.setDepthWritingMode(DepthWritingMode::Disabled);
  gpuState.setFaceCullingMode(FaceCullingMode::Disabled);
  gpuState.setPolygonFillingMode(PolygonFillingMode::Fill);

  m_deferredAccumulationMaterial = std::make_unique<GLMaterial>(RenderingStage::Deferred,
    std::move(pipeline),
    gpuState,
    std::make_unique<ShadingParametersGenericSet>());
}

void GLGraphicsContext::applyGpuState(const GpuStateParameters& gpuState)
{
  setBlendingMode(gpuState.getBlendingMode());
  setDepthTestMode(gpuState.getDepthTestMode());
  setFaceCullingMode(gpuState.getFaceCullingMode());
  setPolygonFillingMode(gpuState.getPolygonFillingMode());
  setDepthWritingMode(gpuState.getDepthWritingMode());
  setupScissorsTest(gpuState.getScissorsTestMode());
}

void GLGraphicsContext::executeRenderingStageQueue(RenderingStage stage)
{
  auto& queue = m_renderingQueues[static_cast<size_t>(stage)];

  if (queue.empty()) {
    return;
  }

  applyGpuState(queue.begin()->material->getGpuStateParameters());

  for (const auto& renderingTask : queue) {
    GLShadersPipeline& shadersPipeline = renderingTask.material->getShadersPipeline();
    glBindProgramPipeline(shadersPipeline.m_programPipeline);

    const auto& shadingParametersBinder = renderingTask.material->getGLParametersBinder();

    GLShader* vertexShader = shadersPipeline.getShader(ShaderType::Vertex);

    if (&shadersPipeline != m_currentShadersPipeline) {
      Camera& activeCamera = *m_graphicsScene->getActiveCamera();

      vertexShader->setParameter("scene.worldToCamera", activeCamera.getViewMatrix());
      vertexShader->setParameter("scene.cameraToProjection", activeCamera.getProjectionMatrix());

      m_currentShadersPipeline = &shadersPipeline;
    }

    if (renderingTask.matrixPalette != nullptr && vertexShader->hasParameter("animation.palette[0]")) {
      vertexShader->setArrayParameter("animation.palette",
        renderingTask.matrixPalette, renderingTask.mesh->getSkeleton()->getBonesCount());
    }

    shadingParametersBinder->bindParameters(shadersPipeline);

    vertexShader->setParameter("transform.local", renderingTask.transform);

    if (renderingTask.material->getGpuStateParameters().getScissorsTestMode() == ScissorsTestMode::Enabled) {
      glScissor(renderingTask.scissorsRect.getOriginX(),
        m_defaultFramebuffer->getHeight() - renderingTask.scissorsRect.getOriginY()
          - renderingTask.scissorsRect.getHeight(),
        renderingTask.scissorsRect.getWidth(), renderingTask.scissorsRect.getHeight());
    }

    renderingTask.mesh->getGeometryStore()->drawRange(
      renderingTask.mesh->getSubMeshIndicesOffset(renderingTask.subMeshIndex),
      renderingTask.mesh->getSubMeshIndicesCount(renderingTask.subMeshIndex),
      renderingTask.primitivesType);
  }
}

int GLGraphicsContext::getViewportWidth() const
{
  return m_defaultFramebuffer->getWidth();
}

int GLGraphicsContext::getViewportHeight() const
{
  return m_defaultFramebuffer->getHeight();
}

