#pragma once

#include <string>
#include <memory>
#include <optional>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "GLShader.h"

class GLGraphicsContext;

class GLShadersPipeline {
 public:
  GLShadersPipeline(std::optional<ResourceHandle<GLShader>> vertexShader,
    std::optional<ResourceHandle<GLShader>> fragmentShader,
    std::optional<ResourceHandle<GLShader>> geometryShader);

  ~GLShadersPipeline();

  [[nodiscard]] bool hasShader(ShaderType type) const;
  [[nodiscard]] GLShader* getShader(ShaderType type) const;

 private:
  GLuint m_programPipeline;

  std::optional<ResourceHandle<GLShader>> m_vertexShader;
  std::optional<ResourceHandle<GLShader>> m_fragmentShader;
  std::optional<ResourceHandle<GLShader>> m_geometryShader;

 private:
  friend class GLGraphicsContext;
};
