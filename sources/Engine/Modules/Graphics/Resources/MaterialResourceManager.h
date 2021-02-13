#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLMaterial.h"

struct MaterialResourceConfig {
  enum class ShaderParamType {
    Texture, Int, Float, Boolean, Color
  };

  enum class ParametersSetType {
    Generic, OpaqueMesh
  };

  struct ShaderParamTextureTransform {
    glm::vec2 offset{ 0.0f, 0.0f };
    glm::vec2 scale{ 1.0f, 1.0f };
    float rotation{};
  };

  struct ShaderParamTexture {
    std::string id;
    size_t slotIndex;

    std::optional<ShaderParamTextureTransform> transform;
  };

  struct ShaderParam {
    ShaderType shaderType;
    std::string name;
    ShaderParamType type;
    std::variant<ShaderParamTexture, int, float, glm::vec4> value;
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
