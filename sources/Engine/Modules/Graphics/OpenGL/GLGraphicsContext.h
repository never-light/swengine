#pragma once

#include <memory>
#include <span>

#include "GL.h"
#include "Modules/Math/Rect.h"

#include "GLGeometryStore.h"
#include "GLShadersPipeline.h"
#include "GLMaterial.h"
#include "GLFramebuffer.h"
#include "Mesh.h"

#include "Modules/Graphics/GraphicsSystem/GraphicsScene.h"
#include "Modules/Graphics/GraphicsSystem/Transform.h"

class SharedGraphicsState;

class Transform;

struct RenderTask {
  GLMaterial* material{};
  Mesh* mesh{};
  uint16_t subMeshIndex = 0;

  const glm::mat4* transform{};
  const glm::mat4* matrixPalette{};

  GLenum primitivesType = GL_TRIANGLES;
  RectI scissorsRect{};
};

class GLGraphicsContext;

struct SDLGLContext {
 public:
  SDLGLContext() = default;
  ~SDLGLContext();

 private:
  SDL_GLContext m_glContext;

 private:
  friend class GLGraphicsContext;
};

class GLGraphicsContext {
 public:
  explicit GLGraphicsContext(SDL_Window* window);
  ~GLGraphicsContext();

  [[nodiscard]] int getViewportWidth() const;
  [[nodiscard]] int getViewportHeight() const;

  void setupGraphicsScene(std::shared_ptr<GraphicsScene> graphicsScene);

  void setupDeferredAccumulationMaterial(std::shared_ptr<GLShadersPipeline> pipeline);

  void swapBuffers();

  void setDepthTestMode(DepthTestMode mode);
  void setFaceCullingMode(FaceCullingMode mode);
  void setPolygonFillingMode(PolygonFillingMode mode);
  void setBlendingMode(BlendingMode mode);
  void setDepthWritingMode(DepthWritingMode mode);
  void setupScissorsTest(ScissorsTestMode mode);

  void applyGpuState(const GpuStateParameters& gpuState);

  void scheduleRenderTask(const RenderTask& task);
  void executeRenderTasks();
  void executeRenderingStageQueue(RenderingStage stage);

  [[nodiscard]] GLGeometryStore& getNDCTexturedQuad() const;

  void setGUIProjectionMatrix(const glm::mat4& projection);
  [[nodiscard]] const glm::mat4& getGUIProjectionMatrix() const;

  void unloadResources();

 private:
  void applyContextChange();
  void resetMaterial();

 private:
  SDLGLContext m_sdlGLContext;

  SDL_Window* m_window;

  GLMaterial* m_currentMaterial = nullptr;
  GLFramebuffer* m_currentFramebuffer = nullptr;

  GLShadersPipeline* m_currentShadersPipeline{};

  std::unique_ptr<GLFramebuffer> m_defaultFramebuffer;
  std::unique_ptr<GLGeometryStore> m_ndcTexturedQuad;

  std::shared_ptr<GraphicsScene> m_graphicsScene;

  std::unique_ptr<GLFramebuffer> m_deferredFramebuffer;
  std::unique_ptr<GLFramebuffer> m_forwardFramebuffer;

  std::array<std::vector<RenderTask>, 6> m_renderingQueues;

  std::unique_ptr<GLMaterial> m_deferredAccumulationMaterial;

  glm::mat4 m_guiProjectionMatrix = glm::identity<glm::mat4>();

 private:
  static void APIENTRY debugOutputCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    GLvoid* userParam);
};
