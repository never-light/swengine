#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GraphicsSystem/Material.h"

struct MaterialResourceParameters {
  enum class ShaderParamType {
    Texture, Int, Float, Boolean
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
    BlendingMode blendingMode = BlendingMode::Unspecified;
    DepthTestMode depthTestMode = DepthTestMode::Unspecified;
    DepthWritingMode depthWritingMode = DepthWritingMode::Unspecified;
    FaceCullingMode faceCullingMode = FaceCullingMode::Unspecified;
    PolygonFillingMode polygonFillingMode = PolygonFillingMode::Unspecified;
  } gpuState;

  std::vector<ShaderParam> parameters;
};

class MaterialResource : public ResourceTypeManager<Material, MaterialResourceParameters> {
 public:
  explicit MaterialResource(ResourcesManager* resourcesManager);
  ~MaterialResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
