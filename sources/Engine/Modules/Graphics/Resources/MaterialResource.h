#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/GraphicsSystem/Material.h"

struct MaterialResourceParameters : ResourceSourceParameters {
  enum class ShaderParamType {
    Texture, Int, Float, Boolean
  };

  struct ShaderParamTexture {
    std::string id;
    size_t slotIndex;
  };

  struct ShaderParam {
    // TODO: replace GLenum with abstract enum here. GL data types should be used only inside GL layer
    GLenum shaderType;
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

class MaterialResource : public Resource {
 public:
  using ParametersType = MaterialResourceParameters;

 public:
  MaterialResource();
  ~MaterialResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
    const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<Material> getMaterial() const;

 private:
  std::shared_ptr<Material> m_material;
};
