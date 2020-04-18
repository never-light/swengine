#pragma once

#include <memory>

#include "GL.h"
#include "Modules/Math/Rect.h"

#include "GLGeometryStore.h"
#include "GLShadersPipeline.h"
#include "GLMaterial.h"
#include "GLFramebuffer.h"

class SharedGraphicsState;

class Transform;

struct RenderTask {
  GLMaterial* material;
  GLGeometryStore* geometryStore;

  size_t startOffset;
  size_t partsCount;

  GLenum primitivesType = GL_TRIANGLES;
  GLFramebuffer* framebuffer = nullptr;
};

class GLGraphicsContext {
 public:
  explicit GLGraphicsContext(SDL_Window* window);
  ~GLGraphicsContext();

  void enableWritingToDepthBuffer();
  void disableWritingToDepthBuffer();

  void enableScissorTest();
  void disableScissorTest();

  void setScissorRectangle(const Rect& rectangle);

  void swapBuffers();

  void setDepthTestMode(DepthTestMode mode);
  void setFaceCullingMode(FaceCullingMode mode);
  void setPolygonFillingMode(PolygonFillingMode mode);
  void setBlendingMode(BlendingMode mode);
  void setDepthWritingMode(DepthWritingMode mode);

  void applyMaterial(const GLMaterial& material);
  void executeRenderTask(const RenderTask& task);

  [[nodiscard]] GLFramebuffer& getDefaultFramebuffer() const;
  [[nodiscard]] GLGeometryStore& getNDCTexturedQuad() const;

 private:
  void applyContextChange();
  void resetMaterial();

 private:
  SDL_Window* m_window;
  SDL_GLContext m_glContext;

  GLMaterial* m_currentMaterial = nullptr;
  GLFramebuffer* m_currentFramebuffer = nullptr;

  std::unique_ptr<GLFramebuffer> m_defaultFramebuffer;
  std::unique_ptr<GLGeometryStore> m_ndcTexturedQuad;

 private:
  static void APIENTRY debugOutputCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    GLvoid* userParam);
};
