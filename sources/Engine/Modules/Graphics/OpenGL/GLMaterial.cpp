#include "precompiled.h"

#pragma hdrstop

#include "GLMaterial.h"

#include <utility>

GLMaterial::GLMaterial(RenderingStage renderingStage,
  std::shared_ptr<GLShadersPipeline> shadersPipeline,
  GpuStateParameters gpuState,
  std::unique_ptr<ShadingParametersBaseSet> shadingParameters)
  : m_shadersPipeline(std::move(shadersPipeline)),
    m_renderingStage(renderingStage),
    m_gpuStateParameters(gpuState),
    m_shadingParametersBinder(createShadingParametersBinder(std::move(shadingParameters)))
{

}

GLMaterial::~GLMaterial()
{

}

std::unique_ptr<GLShadingParametersBaseBinder> GLMaterial::createShadingParametersBinder(
  std::unique_ptr<ShadingParametersBaseSet> shadingParameters)
{
  if (dynamic_cast<ShadingParametersGenericSet*>(shadingParameters.get())) {
    return std::make_unique<GLShadingParametersGenericBinder>(std::move(shadingParameters));
  }
  else if (dynamic_cast<ShadingParametersOpaqueMesh*>(shadingParameters.get())) {
    return std::make_unique<GLShadingParametersOpaqueMeshBinder>(std::move(shadingParameters));
  }
  else if (dynamic_cast<ShadingParametersGUI*>(shadingParameters.get())) {
    return std::make_unique<GLShadingParametersGUIBinder>(std::move(shadingParameters));
  }
  else {
    THROW_EXCEPTION(NotImplementedException, "Shading parameters set type is not supported");
  }
}

const GLShadersPipeline& GLMaterial::getShadersPipeline() const
{
  return *m_shadersPipeline;
}

RenderingStage GLMaterial::getRenderingStage() const
{
  return m_renderingStage;
}

const GpuStateParameters& GLMaterial::getGpuStateParameters() const
{
  return m_gpuStateParameters;
}

GLShadingParametersBaseBinder* GLMaterial::getGLParametersBinder() const
{
  return m_shadingParametersBinder.get();
}

GLShadersPipeline& GLMaterial::getShadersPipeline()
{
  return *m_shadersPipeline;
}

ShadingParametersBaseSet& GLMaterial::getParametersSet()
{
  return m_shadingParametersBinder->getParametersSet();
}

const ShadingParametersBaseSet& GLMaterial::getParametersSet() const
{
  return m_shadingParametersBinder->getParametersSet();
}

GLShadingParametersBaseBinder::GLShadingParametersBaseBinder(
  std::unique_ptr<ShadingParametersBaseSet> parametersSet)
  : m_parametersSet(std::move(parametersSet))
{

}

const ShadingParametersBaseSet& GLShadingParametersBaseBinder::getParametersSet() const
{
  return *m_parametersSet;
}

ShadingParametersBaseSet& GLShadingParametersBaseBinder::getParametersSet()
{
  return *m_parametersSet;
}

GLShadingParametersGenericBinder::GLShadingParametersGenericBinder(
  std::unique_ptr<ShadingParametersBaseSet> parametersSet)
  : GLShadingParametersBaseBinder(std::move(parametersSet))
{

}

void GLShadingParametersGenericBinder::bindParameters(GLShadersPipeline& shadersPipeline)
{
  auto& parametersGenericSet =
    dynamic_cast<ShadingParametersGenericSet&>(getParametersSet());

  for (const auto& parameterIt : parametersGenericSet.getParameters()) {
    const std::string& parameterName = parameterIt.first;

    GLShader* shader = shadersPipeline.getShader(parameterIt.second.shaderType);

    const auto& rawValue = parameterIt.second.value;

    std::visit([&parameterName, shader](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, int>) {
        shader->setParameter(parameterName, int(arg));
      }
      else if constexpr (std::is_same_v<T, float>) {
        shader->setParameter(parameterName, float(arg));
      }
      else if constexpr (std::is_same_v<T, glm::vec3>) {
        shader->setParameter(parameterName, glm::vec3(arg));
      }
      else if constexpr (std::is_same_v<T, glm::vec4>) {
        shader->setParameter(parameterName, glm::vec4(arg));
      }
      else if constexpr (std::is_same_v<T, glm::mat3>) {
        shader->setParameter(parameterName, glm::mat3(arg));
      }
      else if constexpr (std::is_same_v<T, glm::mat4>) {
        shader->setParameter(parameterName, glm::mat4(arg));
      }
      else if constexpr (std::is_same_v<T, ShadingParametersGenericStorage::TextureParameter>) {
        const ShadingParametersGenericStorage::TextureParameter& textureParameter = arg;

        shader->setParameter(parameterName, *(textureParameter.texture), textureParameter.slotIndex);
      }
      else {
        SW_ASSERT(false);
      }
    }, rawValue);
  }
}

GLShadingParametersGUIBinder::GLShadingParametersGUIBinder(
  std::unique_ptr<ShadingParametersBaseSet> parametersSet)
  : GLShadingParametersBaseBinder(std::move(parametersSet))
{

}

void GLShadingParametersGUIBinder::bindParameters(GLShadersPipeline& shadersPipeline)
{
  auto& parametersSet =
    dynamic_cast<ShadingParametersGUI&>(getParametersSet());

  GLShader* fragmentShader = shadersPipeline.getShader(ShaderType::Fragment);

  bool hasBackgroundTexture = parametersSet.getBackgroundTexture().get() != nullptr;

  fragmentShader->setParameter("widget.backgroundColor", parametersSet.getBackgroundColor());
  fragmentShader->setParameter("widget.useBackgroundTexture", hasBackgroundTexture);

  if (hasBackgroundTexture) {
    fragmentShader->setParameter("widget.backgroundTexture", *parametersSet.getBackgroundTexture(), 0);
  }

  bool hasColorAlphaTexture = parametersSet.getAlphaTexture().get() != nullptr;

  fragmentShader->setParameter("widget.useColorAlphaTexture", hasColorAlphaTexture);

  if (hasColorAlphaTexture) {
    fragmentShader->setParameter("widget.colorAlphaTexture", *parametersSet.getAlphaTexture(), 1);
  }
}

GLShadingParametersOpaqueMeshBinder::GLShadingParametersOpaqueMeshBinder(
  std::unique_ptr<ShadingParametersBaseSet> parametersSet)
  : GLShadingParametersBaseBinder(std::move(parametersSet))
{

}

void GLShadingParametersOpaqueMeshBinder::bindParameters(GLShadersPipeline& shadersPipeline)
{
  auto& parametersSet =
    dynamic_cast<ShadingParametersOpaqueMesh&>(getParametersSet());

  GLShader* fragmentShader = shadersPipeline.getShader(ShaderType::Fragment);
  GLShader* vertexShader = shadersPipeline.getShader(ShaderType::Vertex);

  const auto& baseColorTextureEntry = parametersSet.getBaseColorMap();

  fragmentShader->setParameter("base_color", parametersSet.getBaseColorFactor());
  fragmentShader->setParameter("use_base_color_map", baseColorTextureEntry.has_value());

  if (baseColorTextureEntry.has_value()) {
    fragmentShader->setParameter("base_color_map", *baseColorTextureEntry.value().getTexture(), 0);

    if (baseColorTextureEntry->hasTransformation()) {
      vertexShader->setParameter("base_color_map_uv_transform", baseColorTextureEntry->getTransformationMatrix());
    }

    vertexShader->setParameter("use_base_color_map_uv_transform", baseColorTextureEntry->hasTransformation());
  }


}
