#include "precompiled.h"

#pragma hdrstop

#include "ShadingParameters.h"

#include <utility>
#include <glm/gtx/matrix_transform_2d.hpp>

void ShadingParametersGenericStorage::setShaderParameter(ShaderType shaderType,
  const std::string& name,
  const GenericParameterValue& value)
{
  m_parameters.insert({name, GenericParameter(shaderType, value)});
}

const ShadingParametersGenericStorage::GenericParameterValue& ShadingParametersGenericStorage::getShaderParameterValue(
  const std::string& name) const
{
  return m_parameters.at(name).value;
}

const std::unordered_map<std::string,
                         ShadingParametersGenericStorage::GenericParameter>& ShadingParametersGenericStorage::getParameters() const
{
  return m_parameters;
}

void ShadingParametersGenericSet::setShaderParameter(ShaderType shaderType,
  const std::string& name,
  const ShadingParametersGenericStorage::GenericParameterValue& value)
{
  m_parametersStorage.setShaderParameter(shaderType, name, value);
}

const ShadingParametersGenericStorage::GenericParameterValue& ShadingParametersGenericSet::getShaderParameterValue(const std::string& name) const
{
  return m_parametersStorage.getShaderParameterValue(name);
}

const std::unordered_map<std::string,
                         ShadingParametersGenericStorage::GenericParameter>& ShadingParametersGenericSet::getParameters() const
{
  return m_parametersStorage.getParameters();
}

void ShadingParametersGUI::setBackgroundColor(const glm::vec4& color)
{
  m_backgroundColor = color;
}

const glm::vec4& ShadingParametersGUI::getBackgroundColor() const
{
  return m_backgroundColor;
}

void ShadingParametersGUI::setBackgroundTexture(ResourceHandle<GLTexture> texture)
{
  m_backgroundTexture = std::move(texture);
}

ResourceHandle<GLTexture> ShadingParametersGUI::getBackgroundTexture() const
{
  return m_backgroundTexture;
}

void ShadingParametersGUI::setAlphaTexture(ResourceHandle<GLTexture> texture)
{
  m_alphaTexture = std::move(texture);
}

ResourceHandle<GLTexture> ShadingParametersGUI::getAlphaTexture() const
{
  return m_alphaTexture;
}

const glm::vec4& ShadingParametersOpaqueMesh::getBaseColorFactor() const
{
  return m_baseColorFactor;
}

void ShadingParametersOpaqueMesh::setBaseColorFactor(const glm::vec4& baseColorFactor)
{
  m_baseColorFactor = baseColorFactor;
}

ShaderParametersTextureEntry::ShaderParametersTextureEntry(const ResourceHandle<GLTexture>& texture)
  : m_texture(texture)
{

}

const std::optional<ShaderParametersTextureEntry>& ShadingParametersOpaqueMesh::getBaseColorMap() const
{
  return m_baseColorMap;
}

void ShadingParametersOpaqueMesh::setBaseColorMap(const std::optional<ShaderParametersTextureEntry>& baseColorMap)
{
  m_baseColorMap = baseColorMap;
}

const ResourceHandle<GLTexture>& ShaderParametersTextureEntry::getTexture() const
{
  return m_texture;
}

void ShaderParametersTextureEntry::setTexture(const ResourceHandle<GLTexture>& texture)
{
  m_texture = texture;
}

const std::optional<TextureTransform>& ShaderParametersTextureEntry::getTransformation() const
{
  return m_transformation;
}

void ShaderParametersTextureEntry::setTransformation(const std::optional<TextureTransform>& transformation)
{
  m_transformation = transformation;
  m_isTransformationMatrixOutdated = true;
}

const glm::mat3& ShaderParametersTextureEntry::getTransformationMatrix() const
{
  SW_ASSERT(m_transformation.has_value());

  if (m_isTransformationMatrixOutdated) {
    m_transformationMatrix = glm::translate(glm::identity<glm::mat3>(), m_transformation->offset) *
      glm::rotate(glm::identity<glm::mat3>(), m_transformation->rotation) *
      glm::scale(glm::identity<glm::mat3>(), m_transformation->scale);

    m_isTransformationMatrixOutdated = false;
  }

  return m_transformationMatrix;
}

bool ShaderParametersTextureEntry::hasTransformation() const
{
  return m_transformation.has_value();
}
