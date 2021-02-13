#include "precompiled.h"

#pragma hdrstop

#include "GLShader.h"
#include "Exceptions/exceptions.h"
#include "GLDebug.h"

#include <memory>
#include <vector>

GLShader::GLShader(ShaderType type, const std::string& source)
  : m_shaderProgram(0),
    m_type(type)
{
  GLenum shader = 0;

  switch (type) {
    case ShaderType::Vertex:
      shader = glCreateShader(GL_VERTEX_SHADER);
      break;
    case ShaderType::Fragment:
      shader = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    case ShaderType::Geometry:
      shader = glCreateShader(GL_GEOMETRY_SHADER);
      break;
    default:
      THROW_EXCEPTION(EngineRuntimeException, "Trying to create invalid shader with invalid type");
  }

  const char* rawSource = source.c_str();

  glShaderSource(shader, 1, &rawSource, nullptr);
  glCompileShader(shader);

  GLint compilationStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);

  if (!compilationStatus) {
    glDeleteProgram(shader);

    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::string message;
    message.resize(static_cast<std::string::size_type>(infoLogLength));

    glGetShaderInfoLog(shader, infoLogLength, nullptr, &message[0]);

    THROW_EXCEPTION(EngineRuntimeException, "Failed to compile shader: " + message);
  }

  m_shaderProgram = glCreateProgram();

  glAttachShader(m_shaderProgram, shader);
  glProgramParameteri(m_shaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

  glLinkProgram(m_shaderProgram);

  GLint linkingStatus;
  glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkingStatus);

  if (!linkingStatus) {
    glDeleteShader(shader);
    glDeleteProgram(m_shaderProgram);

    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::string message;
    message.resize(static_cast<std::string::size_type>(infoLogLength));

    glGetProgramInfoLog(shader, infoLogLength, nullptr, &message[0]);

    THROW_EXCEPTION(EngineRuntimeException, "Failed to link shader program: " + message);
  }

  glDetachShader(m_shaderProgram, shader);
  glDeleteShader(shader);

  cacheUniformsLocations();
}

GLShader::~GLShader()
{
  if (m_shaderProgram != 0) {
    glDeleteProgram(m_shaderProgram);
  }
}

ShaderType GLShader::getType() const
{
  return m_type;
}

void GLShader::setParameter(const std::string& name, bool value)
{
  glProgramUniform1i(m_shaderProgram, m_uniformsCache[name].location, value);
}

void GLShader::setParameter(const std::string& name, int value)
{
  glProgramUniform1i(m_shaderProgram, m_uniformsCache[name].location, value);
}

void GLShader::setParameter(const std::string& name, float value)
{
  glProgramUniform1f(m_shaderProgram, m_uniformsCache[name].location, value);
}

void GLShader::setParameter(const std::string& name, const glm::vec2& value)
{
  glProgramUniform2fv(m_shaderProgram, m_uniformsCache[name].location, 1, &value[0]);
}

void GLShader::setParameter(const std::string& name, const glm::vec3& value)
{
  glProgramUniform3fv(m_shaderProgram, m_uniformsCache[name].location, 1, &value[0]);
}

void GLShader::setParameter(const std::string& name, const glm::vec4& value)
{
  glProgramUniform4fv(m_shaderProgram, m_uniformsCache[name].location, 1, &value[0]);
}

void GLShader::setParameter(const std::string& name, const glm::mat3x3& value)
{
  glProgramUniformMatrix3fv(m_shaderProgram, m_uniformsCache[name].location, 1, GL_FALSE, &value[0][0]);
}

void GLShader::setParameter(const std::string& name, const glm::mat4x4& value)
{
  glProgramUniformMatrix4fv(m_shaderProgram, m_uniformsCache[name].location, 1, GL_FALSE, &value[0][0]);
}

void GLShader::setParameter(const std::string& name, const GLTexture& texture, size_t unitIndex)
{
  glBindTextureUnit(static_cast<GLuint>(unitIndex), texture.m_texture);
  setParameter(name, static_cast<int>(unitIndex));
}

void GLShader::setArrayParameter(const std::string& name, const glm::mat4x4* array, size_t arraySize)
{
  glProgramUniformMatrix4fv(m_shaderProgram, m_uniformsCache[name + "[0]"].location,
    static_cast<GLsizei>(arraySize), GL_FALSE, glm::value_ptr(array[0]));
}

void GLShader::setArrayParameter(const std::string& name, size_t valueIndex, const glm::mat4x4& value)
{
  glProgramUniformMatrix4fv(m_shaderProgram, m_uniformsCache[name + "[0]"].location + static_cast<GLint>(valueIndex),
    1, GL_FALSE, glm::value_ptr(value));
}

bool GLShader::hasParameter(const std::string& name) const
{
  return m_uniformsCache.find(name) != m_uniformsCache.end();
}

void GLShader::cacheUniformsLocations()
{
  GL_CALL_BLOCK_BEGIN();

  GLint uniformsCount = 0;
  glGetProgramiv(m_shaderProgram, GL_ACTIVE_UNIFORMS, &uniformsCount);

  if (uniformsCount != 0) {
    GLint maxNameLength = 0;
    GLsizei length = 0;
    GLsizei count = 0;
    GLenum type = GL_NONE;

    glGetProgramiv(m_shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

    std::vector<GLchar> uniformName(static_cast<size_t>(maxNameLength));

    for (GLint uniformIndex = 0; uniformIndex < uniformsCount; uniformIndex++) {
      glGetActiveUniform(m_shaderProgram, static_cast<GLuint>(uniformIndex),
        maxNameLength, &length,
        &count, &type,
        uniformName.data());

      UniformInfo uniformInfo = {
        glGetUniformLocation(m_shaderProgram, uniformName.data()),
        count
      };

      m_uniformsCache.insert({std::string(uniformName.data(),
        static_cast<std::string::size_type>(length)), uniformInfo});
    }
  }

  GL_CALL_BLOCK_END();
}

size_t GLShader::getArraySize(const std::string& name) const
{
  return static_cast<size_t>(m_uniformsCache.at(name + "[0]").size);
}
