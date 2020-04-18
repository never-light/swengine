#include "precompiled.h"

#pragma hdrstop

#include "MaterialResource.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"
#include "Modules/ResourceManagement/ResourceManager.h"
#include "TextureResource.h"
#include "ShaderResource.h"

MaterialResource::MaterialResource()
{

}

MaterialResource::~MaterialResource()
{
  SW_ASSERT(m_material.use_count() <= 1);
}

void MaterialResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
  SW_ASSERT(m_material == nullptr);

  ARG_UNUSED(declaration);

  MaterialResourceParameters parameters = declaration.getParameters<MaterialResourceParameters>();

  if (std::get_if<ResourceSourceDeclaration>(&declaration.source)) {
    std::unique_ptr<GLMaterial> gpuMaterial = std::make_unique<GLMaterial>();

    auto vertexShader = resourceManager.
      getResourceFromInstance<ShaderResource>(parameters.shadersPipeline.vertexShaderId)->getShader();
    auto fragmentShader = resourceManager.
      getResourceFromInstance<ShaderResource>(parameters.shadersPipeline.fragmentShaderId)->getShader();

    std::shared_ptr<GLShadersPipeline>
      shadersPipeline = std::make_shared<GLShadersPipeline>(vertexShader, fragmentShader, nullptr);

    gpuMaterial->setShadersPipeline(shadersPipeline);

    gpuMaterial->setBlendingMode(parameters.gpuState.blendingMode);
    gpuMaterial->setDepthTestMode(parameters.gpuState.depthTestMode);
    gpuMaterial->setDepthWritingMode(parameters.gpuState.depthWritingMode);
    gpuMaterial->setFaceCullingMode(parameters.gpuState.faceCullingMode);
    gpuMaterial->setPolygonFillingMode(parameters.gpuState.polygonFillingMode);

    for (const auto& param : parameters.parameters) {
      switch (param.type) {
        case MaterialResourceParameters::ShaderParamType::Int:
          gpuMaterial->setShaderParameter(param.shaderType, param.name, std::get<int>(param.value));
          break;

        case MaterialResourceParameters::ShaderParamType::Float:
          gpuMaterial->setShaderParameter(param.shaderType, param.name, std::get<float>(param.value));
          break;

        case MaterialResourceParameters::ShaderParamType::Boolean:
          gpuMaterial->setShaderParameter(param.shaderType, param.name, std::get<int>(param.value));
          break;

        case MaterialResourceParameters::ShaderParamType::Texture: {
          auto textureParam = std::get<MaterialResourceParameters::ShaderParamTexture>(param.value);

          gpuMaterial->setShaderParameter(param.shaderType, param.name,
            GLMaterial::TextureParameter(resourceManager
                .getResourceFromInstance<TextureResource>(
                  textureParam.id)->getTexture(),
              textureParam.slotIndex));
          break;
        }

        default:
          SW_ASSERT(false);
          break;
      }
    }

    m_material = std::make_shared<Material>(std::move(gpuMaterial));
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh resource from invalid source");
  }
}

void MaterialResource::unload()
{
  SW_ASSERT(m_material.use_count() == 1);

  m_material.reset();
}

bool MaterialResource::isBusy() const
{
  return m_material.use_count() > 1 || m_material->getGpuMaterial().getShadersPipeline().use_count() > 1;
}

MaterialResource::ParametersType MaterialResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
  const ParametersType& defaultParameters)
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

  static std::unordered_map<std::string, GLenum> shaderTypesMap = {
    {"vertex", GL_VERTEX_SHADER},
    {"fragment", GL_FRAGMENT_SHADER},
  };

  static std::unordered_map<std::string, MaterialResourceParameters::ShaderParamType> paramTypesMap = {
    {"int", MaterialResourceParameters::ShaderParamType::Int},
    {"float", MaterialResourceParameters::ShaderParamType::Float},
    {"bool", MaterialResourceParameters::ShaderParamType::Boolean},
    {"texture", MaterialResourceParameters::ShaderParamType::Texture},
  };

  MaterialResourceParameters parameters = defaultParameters;

  // Shaders pipeline
  pugi::xml_node shadersNode = declarationNode.child("shaders_pipeline");

  if (shadersNode) {
    pugi::xml_node vertexShaderNode = shadersNode.child("vertex");

    if (vertexShaderNode) {
      parameters.shadersPipeline.vertexShaderId = vertexShaderNode.attribute("id").as_string();
    }

    pugi::xml_node fragmentShaderNode = shadersNode.child("fragment");

    if (fragmentShaderNode) {
      parameters.shadersPipeline.fragmentShaderId = fragmentShaderNode.attribute("id").as_string();
    }
  }

  // GPU state
  pugi::xml_node gpuStateNode = declarationNode.child("gpu_state");

  if (gpuStateNode) {
    if (gpuStateNode.child("blending")) {
      std::string
        blendingValue = StringUtils::toLowerCase(gpuStateNode.child("blending").attribute("mode").as_string());
      parameters.gpuState.blendingMode = ResourceDeclHelpers::getFilteredParameterValue(blendingValue,
        "blending",
        blendingModesMap,
        BlendingMode::Unspecified);
    }

    if (gpuStateNode.child("depth")) {
      pugi::xml_node depthNode = gpuStateNode.child("depth");

      if (depthNode.attribute("testMode")) {
        std::string depthTestMode = StringUtils::toLowerCase(depthNode.attribute("testMode").as_string());
        parameters.gpuState.depthTestMode = ResourceDeclHelpers::getFilteredParameterValue(depthTestMode,
          "testMode",
          depthTestModesMap,
          DepthTestMode::Unspecified);
      }

      if (depthNode.attribute("write")) {
        std::string depthWriteMode = StringUtils::toLowerCase(depthNode.attribute("write").as_string());
        parameters.gpuState.depthWritingMode =
          ResourceDeclHelpers::getFilteredParameterValue(depthWriteMode, "write", depthWritingModesMap,
            DepthWritingMode::Unspecified);
      }
    }

    if (gpuStateNode.child("face_culling")) {
      std::string
        faceCullingMode = StringUtils::toLowerCase(gpuStateNode.child("face_culling").attribute("mode").as_string());
      parameters.gpuState.faceCullingMode = ResourceDeclHelpers::getFilteredParameterValue(faceCullingMode,
        "face_culling",
        faceCullingModesMap,
        FaceCullingMode::Unspecified);
    }

    if (gpuStateNode.child("polygon_filling")) {
      std::string polygonFillingMode =
        StringUtils::toLowerCase(gpuStateNode.child("polygon_filling").attribute("mode").as_string());
      parameters.gpuState.polygonFillingMode =
        ResourceDeclHelpers::getFilteredParameterValue(polygonFillingMode, "polygon_filling", polygonFillingModesMap,
          PolygonFillingMode::Unspecified);
    }
  }

  // Shaders parameters
  if (declarationNode.child("params")) {
    for (auto paramNode : declarationNode.child("params").children()) {
      MaterialResourceParameters::ShaderParam param;

      std::string shaderType = StringUtils::toLowerCase(paramNode.attribute("shader").as_string());
      param.shaderType = ResourceDeclHelpers::getFilteredParameterValue(shaderType,
        "shader",
        shaderTypesMap,
        static_cast<GLenum>(GL_VERTEX_SHADER));

      param.name = paramNode.attribute("name").as_string();

      std::string paramType = StringUtils::toLowerCase(paramNode.attribute("type").as_string());
      param.type = ResourceDeclHelpers::getFilteredParameterValue(paramType,
        "type",
        paramTypesMap,
        MaterialResourceParameters::ShaderParamType::Int);

      switch (param.type) {
        case MaterialResourceParameters::ShaderParamType::Int:
          param.value = paramNode.attribute("value").as_int();
          break;

        case MaterialResourceParameters::ShaderParamType::Float:
          param.value = paramNode.attribute("value").as_float();
          break;

        case MaterialResourceParameters::ShaderParamType::Boolean:
          param.value = static_cast<int>(paramNode.attribute("value").as_bool());
          break;

        case MaterialResourceParameters::ShaderParamType::Texture:
          param.value = MaterialResourceParameters::ShaderParamTexture{
            paramNode.attribute("id").as_string(),
            paramNode.attribute("slot").as_uint()
          };
          break;

        default:
          SW_ASSERT(false);
          break;
      }

      auto parameterIt = std::find_if(parameters.parameters.begin(), parameters.parameters.end(),
        [&param](MaterialResourceParameters::ShaderParam existingParam) {
          return existingParam.name == param.name;
        });

      if (parameterIt != parameters.parameters.end()) {
        *parameterIt = param;
      }
      else {
        parameters.parameters.push_back(param);
      }
    }
  }

  return parameters;
}

std::shared_ptr<Material> MaterialResource::getMaterial() const
{
  return m_material;
}
