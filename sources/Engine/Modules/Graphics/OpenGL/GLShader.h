#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <span>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "GL.h"
#include "GLTexture.h"
#include "ShaderType.h"

class GLShadersPipeline;

class GLShader : public Resource {
 public:
  GLShader(ShaderType type, const std::string& source);
  ~GLShader() override;

  [[nodiscard]] ShaderType getType() const;

  void setParameter(const std::string& name, bool value);
  void setParameter(const std::string& name, int value);
  void setParameter(const std::string& name, float value);
  void setParameter(const std::string& name, const glm::vec2& value);
  void setParameter(const std::string& name, const glm::vec3& value);
  void setParameter(const std::string& name, const glm::vec4& value);
  void setParameter(const std::string& name, const glm::mat3x3& value);
  void setParameter(const std::string& name, const glm::mat4x4& value);
  void setParameter(const std::string& name, const GLTexture& texture, size_t unitIndex);

  [[nodiscard]] size_t getArraySize(const std::string& name) const;

  void setArrayParameter(const std::string& name, const glm::mat4x4* array, size_t arraySize);
  void setArrayParameter(const std::string& name, size_t valueIndex, const glm::mat4x4& value);

  [[nodiscard]] bool hasParameter(const std::string& name) const;
 private:
  struct UniformInfo {
    GLint location;
    GLsizei size;
  };

 private:
  void cacheUniformsLocations();

 private:
  GLuint m_shaderProgram;
  ShaderType m_type;

  std::unordered_map<std::string, UniformInfo> m_uniformsCache;

 private:
  friend class GLShadersPipeline;
};

