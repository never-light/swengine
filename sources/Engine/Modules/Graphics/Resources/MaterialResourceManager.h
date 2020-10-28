#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLMaterial.h"

struct MaterialResourceConfig {
  enum class ShaderParamType {
    Texture, Int, Float, Boolean
  };

  enum class ParametersSetType {
    Generic, OpaqueMesh
  };

  struct ShaderParamTexture {
    std::string id;
    size_t slotIndex;
  };

  struct ShaderParam {
    ShaderType shaderType;
    std::string name;
    ShaderParamType type;
    std::variant<ShaderParamTexture, int, float> value;
  };

  struct {
    std::string vertexShaderId;
    std::string fragmentShaderId;
  } shadersPipeline;

  struct {
    BlendingMode blendingMode = BlendingMode::Disabled;
    DepthTestMode depthTestMode = DepthTestMode::LessOrEqual;
    DepthWritingMode depthWritingMode = DepthWritingMode::Enabled;
    FaceCullingMode faceCullingMode = FaceCullingMode::Disabled;
    PolygonFillingMode polygonFillingMode = PolygonFillingMode::Fill;
  } gpuState;

  std::unordered_map<std::string, ShaderParam> parameters;
  RenderingStage renderingStage;
  ParametersSetType parametersSetType;
};

class MaterialResourceManager : public ResourceManager<GLMaterial, MaterialResourceConfig> {
 public:
  explicit MaterialResourceManager(ResourcesManager* resourcesManager);
  ~MaterialResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
