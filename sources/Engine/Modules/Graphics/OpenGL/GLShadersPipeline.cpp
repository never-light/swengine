#include "precompiled.h"

#pragma hdrstop

#include "GLShadersPipeline.h"

#include <utility>
#include "Exceptions/exceptions.h"
#include "GLDebug.h"

GLShadersPipeline::GLShadersPipeline(std::optional<ResourceHandle<GLShader>> vertexShader,
  std::optional<ResourceHandle<GLShader>> fragmentShader,
  std::optional<ResourceHandle<GLShader>> geometryShader)
  : m_vertexShader(std::move(vertexShader)),
    m_fragmentShader(std::move(fragmentShader)),
    m_geometryShader(std::move(geometryShader))
{
  GL_CALL_BLOCK_BEGIN();

  glCreateProgramPipelines(1, &m_programPipeline);

  if (m_vertexShader.has_value()) {
    if (m_vertexShader.value()->getType() != ShaderType::Vertex) {
      THROW_EXCEPTION(EngineRuntimeException, "Shader type mismatch for vertex shader in pipeline");
    }

    glUseProgramStages(m_programPipeline, GL_VERTEX_SHADER_BIT, m_vertexShader.value()->m_shaderProgram);
  }

  if (m_fragmentShader.has_value()) {
    if (m_fragmentShader.value()->getType() != ShaderType::Fragment) {
      THROW_EXCEPTION(EngineRuntimeException, "Shader type mismatch for fragment shader in pipeline");
    }

    glUseProgramStages(m_programPipeline, GL_FRAGMENT_SHADER_BIT, m_fragmentShader.value()->m_shaderProgram);
  }

  if (m_geometryShader.has_value()) {
    if (m_vertexShader.value()->getType() != ShaderType::Geometry) {
      THROW_EXCEPTION(EngineRuntimeException, "Shader type mismatch for geometry shader in pipeline");
    }

    glUseProgramStages(m_programPipeline, GL_GEOMETRY_SHADER_BIT, m_geometryShader.value()->m_shaderProgram);
  }

  GL_CALL_BLOCK_END();
}

GLShadersPipeline::~GLShadersPipeline()
{
  glDeleteProgramPipelines(1, &m_programPipeline);
}

bool GLShadersPipeline::hasShader(ShaderType type) const
{
  switch (type) {
    case ShaderType::Vertex:
      return m_vertexShader.has_value();
    case ShaderType::Fragment:
      return m_fragmentShader.has_value();
    case ShaderType::Geometry:
      return m_geometryShader.has_value();
  }

  THROW_EXCEPTION(EngineRuntimeException, "Trying to check existing of invalid shader type");
}

GLShader* GLShadersPipeline::getShader(ShaderType type) const
{
  switch (type) {
    case ShaderType::Vertex:
      return m_vertexShader.value().get();
    case ShaderType::Fragment:
      return m_fragmentShader.value().get();
    case ShaderType::Geometry:
      return m_geometryShader.value().get();
  }

  THROW_EXCEPTION(EngineRuntimeException, "Trying to get invalid shader type");
}
