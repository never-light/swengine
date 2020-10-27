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
  : ResourceManager<Material, MaterialResourceConfig>(resourcesManager)
{

}

MaterialResourceManager::~MaterialResourceManager() = default;

void MaterialResourceManager::load(size_t resourceIndex)
{
  MaterialResourceConfig* config = getResourceConfig(resourceIndex);

  std::unique_ptr<GLMaterial> gpuMaterial = std::make_unique<GLMaterial>();

  auto vertexShader = getResourceManager()->
    getResource<GLShader>(config->shadersPipeline.vertexShaderId);
  auto fragmentShader = getResourceManager()->
    getResource<GLShader>(config->shadersPipeline.fragmentShaderId);

  std::shared_ptr<GLShadersPipeline>
    shadersPipeline = std::make_shared<GLShadersPipeline>(vertexShader, fragmentShader,
      std::optional<ResourceHandle<GLShader>>());

  gpuMaterial->setShadersPipeline(shadersPipeline);

  gpuMaterial->setBlendingMode(config->gpuState.blendingMode);
  gpuMaterial->setDepthTestMode(config->gpuState.depthTestMode);
  gpuMaterial->setDepthWritingMode(config->gpuState.depthWritingMode);
  gpuMaterial->setFaceCullingMode(config->gpuState.faceCullingMode);
  gpuMaterial->setPolygonFillingMode(config->gpuState.polygonFillingMode);

  for (const auto& param : config->parameters) {
    switch (param.type) {
      case MaterialResourceConfig::ShaderParamType::Int:
        gpuMaterial->setShaderParameter(param.shaderType, param.name, std::get<int>(param.value));
        break;

      case MaterialResourceConfig::ShaderParamType::Float:
        gpuMaterial->setShaderParameter(param.shaderType, param.name, std::get<float>(param.value));
        break;

      case MaterialResourceConfig::ShaderParamType::Boolean:
        gpuMaterial->setShaderParameter(param.shaderType, param.name, std::get<int>(param.value));
        break;

      case MaterialResourceConfig::ShaderParamType::Texture: {
        auto textureParam = std::get<MaterialResourceConfig::ShaderParamTexture>(param.value);

        gpuMaterial->setShaderParameter(param.shaderType, param.name,
          GLMaterial::TextureParameter(getResourceManager()
              ->getResource<GLTexture>(
                textureParam.id),
            textureParam.slotIndex));
        break;
      }

      default:
        SW_ASSERT(false);
        break;
    }
  }

  allocateResource<Material>(resourceIndex, std::move(gpuMaterial));
}



void MaterialResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  static std::unordered_map<std::string, BlendingMode> blendingModesMap = {
    {"disabled", BlendingMode::Disabled},
    {"unspecified", BlendingMode::Unspecified},
    {"alpha_one_minus_alpha", BlendingMode::Alpha_OneMinusAlpha},
  };

  static std::unordered_map<std::string, DepthTestMode> depthTestModesMap = {
    {"disabled", DepthTestMode::Disabled},
    {"unspecified", DepthTestMode::Unspecified},
    {"less", DepthTestMode::Less},
    {"less_or_equal", DepthTestMode::LessOrEqual},
    {"not_equal", DepthTestMode::NotEqual}
  };

  static std::unordered_map<std::string, DepthWritingMode> depthWritingModesMap = {
    {"disabled", DepthWritingMode::Disabled},
    {"unspecified", DepthWritingMode::Unspecified},
    {"enabled", DepthWritingMode::Enabled},
  };

  static std::unordered_map<std::string, FaceCullingMode> faceCullingModesMap = {
    {"disabled", FaceCullingMode::Disabled},
    {"unspecified", FaceCullingMode::Unspecified},
    {"back", FaceCullingMode::Back},
    {"front", FaceCullingMode::Front},
  };

  static std::unordered_map<std::string, PolygonFillingMode> polygonFillingModesMap = {
    {"unspecified", PolygonFillingMode::Unspecified},
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
  };

  MaterialResourceConfig* resourceConfig = createResourceConfig(resourceIndex);

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
        BlendingMode::Unspecified);
    }

    if (gpuStateNode.child("depth")) {
      pugi::xml_node depthNode = gpuStateNode.child("depth");

      if (depthNode.attribute("testMode")) {
        std::string depthTestMode = StringUtils::toLowerCase(depthNode.attribute("testMode").as_string());
        resourceConfig->gpuState.depthTestMode = ResourceDeclHelpers::getFilteredParameterValue(depthTestMode,
          "testMode",
          depthTestModesMap,
          DepthTestMode::Unspecified);
      }

      if (depthNode.attribute("write")) {
        std::string depthWriteMode = StringUtils::toLowerCase(depthNode.attribute("write").as_string());
        resourceConfig->gpuState.depthWritingMode =
          ResourceDeclHelpers::getFilteredParameterValue(depthWriteMode, "write", depthWritingModesMap,
            DepthWritingMode::Unspecified);
      }
    }

    if (gpuStateNode.child("face_culling")) {
      std::string
        faceCullingMode = StringUtils::toLowerCase(gpuStateNode.child("face_culling").attribute("mode").as_string());
      resourceConfig->gpuState.faceCullingMode = ResourceDeclHelpers::getFilteredParameterValue(faceCullingMode,
        "face_culling",
        faceCullingModesMap,
        FaceCullingMode::Unspecified);
    }

    if (gpuStateNode.child("polygon_filling")) {
      std::string polygonFillingMode =
        StringUtils::toLowerCase(gpuStateNode.child("polygon_filling").attribute("mode").as_string());
      resourceConfig->gpuState.polygonFillingMode =
        ResourceDeclHelpers::getFilteredParameterValue(polygonFillingMode, "polygon_filling", polygonFillingModesMap,
          PolygonFillingMode::Unspecified);
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

        case MaterialResourceConfig::ShaderParamType::Texture:
          param.value = MaterialResourceConfig::ShaderParamTexture{
            paramNode.attribute("id").as_string(),
            paramNode.attribute("slot").as_uint()
          };
          break;

        default:
          SW_ASSERT(false);
          break;
      }

      auto parameterIt = std::find_if(resourceConfig->parameters.begin(), resourceConfig->parameters.end(),
        [&param](const MaterialResourceConfig::ShaderParam& existingParam) {
          return existingParam.name == param.name;
        });

      if (parameterIt != resourceConfig->parameters.end()) {
        *parameterIt = param;
      }
      else {
        resourceConfig->parameters.push_back(param);
      }
    }
  }
}
