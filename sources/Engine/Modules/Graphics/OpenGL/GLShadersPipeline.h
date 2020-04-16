#pragma once

#include <string>
#include <memory>
#include "GLShader.h"

class GLGraphicsContext;

class GLShadersPipeline {
 public:
  GLShadersPipeline(std::shared_ptr<GLShader> vertexShader,
                    std::shared_ptr<GLShader> fragmentShader,
                    std::shared_ptr<GLShader> geometrySHader);

  ~GLShadersPipeline();

  [[nodiscard]] bool hasShader(GLenum type) const;
  [[nodiscard]] GLShader* getShader(GLenum type) const;

 private:
  GLuint m_programPipeline;

  std::shared_ptr<GLShader> m_vertexShader;
  std::shared_ptr<GLShader> m_fragmentShader;
  std::shared_ptr<GLShader> m_geometryShader;

 private:
  friend class GLGraphicsContext;
};
