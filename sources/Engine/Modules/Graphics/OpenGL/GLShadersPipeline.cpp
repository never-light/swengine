#include "precompiled.h"

#pragma hdrstop

#include "GLShadersPipeline.h"
#include "Exceptions/exceptions.h"
#include "GLDebug.h"

GLShadersPipeline::GLShadersPipeline(std::shared_ptr<GLShader> vertexShader,
  std::shared_ptr<GLShader> fragmentShader,
  std::shared_ptr<GLShader> geometrySHader)
  : m_vertexShader(vertexShader),
    m_fragmentShader(fragmentShader),
    m_geometryShader(geometrySHader)
{
  GL_CALL_BLOCK_BEGIN();

  glCreateProgramPipelines(1, &m_programPipeline);

  if (m_vertexShader != nullptr) {
    if (m_vertexShader->getType() != GL_VERTEX_SHADER) {
      THROW_EXCEPTION(EngineRuntimeException, "Shader type mismatch for vertex shader in pipeline");
    }

    glUseProgramStages(m_programPipeline, GL_VERTEX_SHADER_BIT, vertexShader->m_shaderProgram);
  }

  if (m_fragmentShader != nullptr) {
    if (m_fragmentShader->getType() != GL_FRAGMENT_SHADER) {
      THROW_EXCEPTION(EngineRuntimeException, "Shader type mismatch for fragment shader in pipeline");
    }

    glUseProgramStages(m_programPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShader->m_shaderProgram);
  }

  if (m_geometryShader != nullptr) {
    if (m_vertexShader->getType() != GL_GEOMETRY_SHADER) {
      THROW_EXCEPTION(EngineRuntimeException, "Shader type mismatch for geometry shader in pipeline");
    }

    glUseProgramStages(m_programPipeline, GL_GEOMETRY_SHADER_BIT, geometrySHader->m_shaderProgram);
  }

  GL_CALL_BLOCK_END();
}

GLShadersPipeline::~GLShadersPipeline()
{
  glDeleteProgramPipelines(1, &m_programPipeline);
}

bool GLShadersPipeline::hasShader(GLenum type) const
{
  switch (type) {
    case GL_VERTEX_SHADER:
      return m_vertexShader != nullptr;
    case GL_FRAGMENT_SHADER:
      return m_fragmentShader != nullptr;
    case GL_GEOMETRY_SHADER:
      return m_geometryShader != nullptr;
  }

  THROW_EXCEPTION(EngineRuntimeException, "Trying to check existing of invalid shader type");
}

GLShader* GLShadersPipeline::getShader(GLenum type) const
{
  switch (type) {
    case GL_VERTEX_SHADER:
      return m_vertexShader.get();
    case GL_FRAGMENT_SHADER:
      return m_fragmentShader.get();
    case GL_GEOMETRY_SHADER:
      return m_geometryShader.get();
  }

  THROW_EXCEPTION(EngineRuntimeException, "Trying to get invalid shader type");
}
