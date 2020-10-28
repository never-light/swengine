#include "precompiled.h"

#pragma hdrstop

#include "ShadingParameters.h"


void ShadingParametersGenericStorage::setShaderParameter(ShaderType shaderType, const std::string& name, const GenericParameterValue& value)
{
  m_parameters.insert({name, GenericParameter(shaderType, value)});
}

const ShadingParametersGenericStorage::GenericParameterValue& ShadingParametersGenericStorage::getShaderParameterValue(const std::string& name) const
{
  return m_parameters.at(name).value;
}

const std::unordered_map<std::string, ShadingParametersGenericStorage::GenericParameter>& ShadingParametersGenericStorage::getParameters() const
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

const std::unordered_map<std::string, ShadingParametersGenericStorage::GenericParameter>& ShadingParametersGenericSet::getParameters() const
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
  m_alphaTexture = texture;
}

ResourceHandle<GLTexture> ShadingParametersGUI::getAlphaTexture() const
{
  return m_alphaTexture;
}

void ShadingParametersOpaqueMesh::setAlbedoTexture(ResourceHandle<GLTexture> texture)
{
  m_albedoTexture = texture;
}

ResourceHandle<GLTexture> ShadingParametersOpaqueMesh::getAlbedoTexture() const
{
  return m_albedoTexture;
}
