#include "precompiled.h"

#pragma hdrstop

#include "MaterialResourceManager.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"
#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "TextureResourceManager.h"
#include "ShaderResourceManager.h"

MaterialResourceManager::MaterialResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<GLMaterial, MaterialResourceConfig>(resourcesManager)
{

}

MaterialResourceManager::~MaterialResourceManager() = default;

void MaterialResourceManager::load(size_t resourceIndex)
{
  MaterialResourceConfig* config = getResourceConfig(resourceIndex);

  auto vertexShader = getResourceManager()->
    getResource<GLShader>(config->shadersPipeline.vertexShaderId);
  auto fragmentShader = getResourceManager()->
    getResource<GLShader>(config->shadersPipeline.fragmentShaderId);

  std::shared_ptr<GLShadersPipeline>
    shadersPipeline = std::make_shared<GLShadersPipeline>(vertexShader, fragmentShader,
    std::optional<ResourceHandle<GLShader>>());

  GpuStateParameters gpuStateParameters;

  gpuStateParameters.setBlendingMode(config->gpuState.blendingMode);
  gpuStateParameters.setDepthTestMode(config->gpuState.depthTestMode);
  gpuStateParameters.setDepthWritingMode(config->gpuState.depthWritingMode);
  gpuStateParameters.setFaceCullingMode(config->gpuState.faceCullingMode);
  gpuStateParameters.setPolygonFillingMode(config->gpuState.polygonFillingMode);

  std::unique_ptr<ShadingParametersBaseSet> parametersSet;

  if (config->parametersSetType == MaterialResourceConfig::ParametersSetType::Generic) {
    auto genericParametersSet = std::make_unique<ShadingParametersGenericSet>();

    for (const auto& paramIt : config->parameters) {
      auto& param = paramIt.second;

      switch (param.type) {
        case MaterialResourceConfig::ShaderParamType::Int:
          genericParametersSet->setShaderParameter(param.shaderType, param.name, std::get<int>(param.value));
          break;

        case MaterialResourceConfig::ShaderParamType::Float:
          genericParametersSet->setShaderParameter(param.shaderType, param.name, std::get<float>(param.value));
          break;

        case MaterialResourceConfig::ShaderParamType::Boolean:
          genericParametersSet->setShaderParameter(param.shaderType, param.name, std::get<int>(param.value));
          break;

        case MaterialResourceConfig::ShaderParamType::Color:
          genericParametersSet->setShaderParameter(param.shaderType, param.name, std::get<glm::vec4>(param.value));
          break;

        case MaterialResourceConfig::ShaderParamType::Texture: {
          auto textureParam = std::get<MaterialResourceConfig::ShaderParamTexture>(param.value);

          genericParametersSet->setShaderParameter(param.shaderType, param.name,
            ShadingParametersGenericStorage::TextureParameter(getResourceManager()
                ->getResource<GLTexture>(
                  textureParam.id),
              textureParam.slotIndex));

          if (textureParam.transform.has_value()) {
            SW_ASSERT(false && "Texture transforms are not supported for generic materials");
          }
          break;
        }

        default:
          SW_ASSERT(false);
          break;
      }
    }

    parametersSet = std::move(genericParametersSet);
  }
  else {
    auto opaqueMeshParametersSet = std::make_unique<ShadingParametersOpaqueMesh>();

    if (config->parameters.contains("base_color_map")) {
      auto baseColorTextureParam = std::get<MaterialResourceConfig::ShaderParamTexture>(
        config->parameters.at("base_color_map").value);

      ShaderParametersTextureEntry baseColorTextureEntry(getResourceManager()->getResource<GLTexture>(
        baseColorTextureParam.id));

      if (baseColorTextureParam.transform.has_value()) {
        baseColorTextureEntry.setTransformation(TextureTransform{
          .offset = baseColorTextureParam.transform->offset,
          .scale = baseColorTextureParam.transform->scale,
          .rotation = baseColorTextureParam.transform->rotation
        });
      }

      opaqueMeshParametersSet->setBaseColorMap(baseColorTextureEntry);
    }

    if (config->parameters.contains("base_color")) {
      opaqueMeshParametersSet->setBaseColorFactor(std::get<glm::vec4>(
        config->parameters.at("base_color").value));
    }

    parametersSet = std::move(opaqueMeshParametersSet);
  }

  allocateResource<GLMaterial>(resourceIndex, config->renderingStage, shadersPipeline,
    gpuStateParameters, std::move(parametersSet));
}

void MaterialResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  static std::unordered_map<std::string, BlendingMode> blendingModesMap = {
    {"disabled", BlendingMode::Disabled},
    {"alpha_one_minus_alpha", BlendingMode::Alpha_OneMinusAlpha},
  };

  static std::unordered_map<std::string, DepthTestMode> depthTestModesMap = {
    {"disabled", DepthTestMode::Disabled},
    {"less", DepthTestMode::Less},
    {"less_or_equal", DepthTestMode::LessOrEqual},
    {"not_equal", DepthTestMode::NotEqual}
  };

  static std::unordered_map<std::string, DepthWritingMode> depthWritingModesMap = {
    {"disabled", DepthWritingMode::Disabled},
    {"enabled", DepthWritingMode::Enabled},
  };

  static std::unordered_map<std::string, FaceCullingMode> faceCullingModesMap = {
    {"disabled", FaceCullingMode::Disabled},
    {"back", FaceCullingMode::Back},
    {"front", FaceCullingMode::Front},
  };

  static std::unordered_map<std::string, PolygonFillingMode> polygonFillingModesMap = {
    {"fill", PolygonFillingMode::Fill},
    {"wireframe", PolygonFillingMode::Wireframe},
  };

  static std::unordered_map<std::string, ShaderType> shaderTypesMap = {
    {"vertex", ShaderType::Vertex},
    {"fragment", ShaderType::Fragment},
  };

  static std::unordered_map<std::string, MaterialResourceConfig::ShaderParamType> paramTypesMap = {
    {"int", MaterialResourceConfig::ShaderParamType::Int},
    {"float", MaterialResourceConfig::ShaderParamType::Float},
    {"bool", MaterialResourceConfig::ShaderParamType::Boolean},
    {"texture", MaterialResourceConfig::ShaderParamType::Texture},
    {"color", MaterialResourceConfig::ShaderParamType::Color},
  };

  static std::unordered_map<std::string, MaterialResourceConfig::ParametersSetType> parametersSetTypesMap = {
    {"generic", MaterialResourceConfig::ParametersSetType::Generic},
    {"opaque_mesh", MaterialResourceConfig::ParametersSetType::OpaqueMesh}
  };

  static std::unordered_map<std::string, RenderingStage> renderingStagesTypesMap = {
    {"deferred", RenderingStage::Deferred},
    {"forward", RenderingStage::Forward},
    {"forward_environment", RenderingStage::ForwardEnvironment},
    {"post_process", RenderingStage::PostProcess}
  };

  MaterialResourceConfig* resourceConfig = createResourceConfig(resourceIndex);

  // Rendering configuration
  resourceConfig->parametersSetType = ResourceDeclHelpers::getFilteredParameterValue(
    configNode.attribute("parameters_set").as_string(),
    "parameters_set",
    parametersSetTypesMap,
    MaterialResourceConfig::ParametersSetType::Generic);

  resourceConfig->renderingStage = ResourceDeclHelpers::getFilteredParameterValue(
    configNode.attribute("rendering_stage").as_string(),
    "rendering_stage",
    renderingStagesTypesMap,
    RenderingStage::Deferred);

  if (resourceConfig->parametersSetType == MaterialResourceConfig::ParametersSetType::OpaqueMesh) {
    resourceConfig->gpuState.blendingMode = BlendingMode::Disabled;
    resourceConfig->gpuState.depthWritingMode = DepthWritingMode::Enabled;
    resourceConfig->gpuState.depthTestMode = DepthTestMode::Less;
    resourceConfig->gpuState.faceCullingMode = FaceCullingMode::Back;
  }

  // Shaders pipeline
  pugi::xml_node shadersNode = configNode.child("shaders_pipeline");

  if (shadersNode) {
    pugi::xml_node vertexShaderNode = shadersNode.child("vertex");

    if (vertexShaderNode) {
      resourceConfig->shadersPipeline.vertexShaderId = vertexShaderNode.attribute("id").as_string();
    }

    pugi::xml_node fragmentShaderNode = shadersNode.child("fragment");

    if (fragmentShaderNode) {
      resourceConfig->shadersPipeline.fragmentShaderId = fragmentShaderNode.attribute("id").as_string();
    }
  }

  // GPU state
  pugi::xml_node gpuStateNode = configNode.child("gpu_state");

  if (gpuStateNode) {
    if (gpuStateNode.child("blending")) {
      std::string
        blendingValue = StringUtils::toLowerCase(gpuStateNode.child("blending").attribute("mode").as_string());
      resourceConfig->gpuState.blendingMode = ResourceDeclHelpers::getFilteredParameterValue(blendingValue,
        "blending",
        blendingModesMap,
        BlendingMode::Disabled);
    }

    if (gpuStateNode.child("depth")) {
      pugi::xml_node depthNode = gpuStateNode.child("depth");

      if (depthNode.attribute("testMode")) {
        std::string depthTestMode = StringUtils::toLowerCase(depthNode.attribute("testMode").as_string());
        resourceConfig->gpuState.depthTestMode = ResourceDeclHelpers::getFilteredParameterValue(depthTestMode,
          "testMode",
          depthTestModesMap,
          DepthTestMode::LessOrEqual);
      }

      if (depthNode.attribute("write")) {
        std::string depthWriteMode = StringUtils::toLowerCase(depthNode.attribute("write").as_string());
        resourceConfig->gpuState.depthWritingMode =
          ResourceDeclHelpers::getFilteredParameterValue(depthWriteMode, "write", depthWritingModesMap,
            DepthWritingMode::Enabled);
      }
    }

    if (gpuStateNode.child("face_culling")) {
      std::string
        faceCullingMode = StringUtils::toLowerCase(gpuStateNode.child("face_culling").attribute("mode").as_string());
      resourceConfig->gpuState.faceCullingMode = ResourceDeclHelpers::getFilteredParameterValue(faceCullingMode,
        "face_culling",
        faceCullingModesMap,
        FaceCullingMode::Disabled);
    }

    if (gpuStateNode.child("polygon_filling")) {
      std::string polygonFillingMode =
        StringUtils::toLowerCase(gpuStateNode.child("polygon_filling").attribute("mode").as_string());
      resourceConfig->gpuState.polygonFillingMode =
        ResourceDeclHelpers::getFilteredParameterValue(polygonFillingMode, "polygon_filling", polygonFillingModesMap,
          PolygonFillingMode::Fill);
    }
  }

  // Shaders parameters
  if (configNode.child("params")) {
    for (auto paramNode : configNode.child("params").children()) {
      MaterialResourceConfig::ShaderParam param;

      std::string shaderType = StringUtils::toLowerCase(paramNode.attribute("shader").as_string());
      param.shaderType = ResourceDeclHelpers::getFilteredParameterValue(shaderType,
        "shader",
        shaderTypesMap,
        static_cast<ShaderType>(ShaderType::Vertex));

      param.name = paramNode.attribute("name").as_string();

      std::string paramType = StringUtils::toLowerCase(paramNode.attribute("type").as_string());
      param.type = ResourceDeclHelpers::getFilteredParameterValue(paramType,
        "type",
        paramTypesMap,
        MaterialResourceConfig::ShaderParamType::Int);

      switch (param.type) {
        case MaterialResourceConfig::ShaderParamType::Int:
          param.value = paramNode.attribute("value").as_int();
          break;

        case MaterialResourceConfig::ShaderParamType::Float:
          param.value = paramNode.attribute("value").as_float();
          break;

        case MaterialResourceConfig::ShaderParamType::Boolean:
          param.value = static_cast<int>(paramNode.attribute("value").as_bool());
          break;

        case MaterialResourceConfig::ShaderParamType::Color:
          param.value = StringUtils::stringToVec4(paramNode.attribute("value").as_string());
          break;

        case MaterialResourceConfig::ShaderParamType::Texture: {
          auto paramValue = MaterialResourceConfig::ShaderParamTexture{
            paramNode.attribute("id").as_string(),
            paramNode.attribute("slot").as_uint()
          };

          if (paramNode.attribute("scale") || paramNode.attribute("offset") ||
            paramNode.attribute("rotation")) {
            paramValue.transform = MaterialResourceConfig::ShaderParamTextureTransform{};

            if (paramNode.attribute("scale")) {
              paramValue.transform->scale = StringUtils::stringToVec2(paramNode.attribute("scale").as_string());
            }

            if (paramNode.attribute("offset")) {
              paramValue.transform->offset = StringUtils::stringToVec2(paramNode.attribute("offset").as_string());
            }

            if (paramNode.attribute("rotation")) {
              paramValue.transform->rotation = paramNode.attribute("rotation").as_float();
            }
          }

          param.value = paramValue;
        }
          break;

        default:
          SW_ASSERT(false);
          break;
      }

      auto parameterIt = std::find_if(resourceConfig->parameters.begin(), resourceConfig->parameters.end(),
        [&param](const auto& existingParam) {
          return existingParam.second.name == param.name;
        });

      if (parameterIt != resourceConfig->parameters.end()) {
        parameterIt->second = param;
      }
      else {
        resourceConfig->parameters.insert({param.name, param});
      }
    }
  }
}
