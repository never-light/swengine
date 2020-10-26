#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <utility>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "GLTexture.h"

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/Resources/ShaderResource.h"
#include "Modules/Math/Rect.h"

enum class DepthTestMode {
  Disabled, Unspecified, Less, LessOrEqual, NotEqual
};

enum class FaceCullingMode {
  Disabled, Unspecified, Back, Front
};

enum class PolygonFillingMode {
  Unspecified, Fill, Wireframe
};

enum class BlendingMode {
  Disabled, Unspecified, Alpha_OneMinusAlpha
};

enum class DepthWritingMode {
  Disabled, Unspecified, Enabled
};

enum class ScissorsTestMode {
  Disabled, Unspecified, Enabled
};

class GLGraphicsContext;

class GLMaterial : public Resource {
 public:
  struct TextureParameter {
    TextureParameter(ResourceHandle<GLTexture> texture, size_t slotIndex)
      : texture(texture), slotIndex(slotIndex)
    {

    }

    ResourceHandle<GLTexture> texture;
    size_t slotIndex;
  };

  using GenericParameterValue = std::variant<int, float, glm::vec3, glm::vec4, glm::mat3, glm::mat4, TextureParameter>;

  struct GenericParameter {
    GenericParameter(ShaderType shaderType, GenericParameterValue value)
      : shaderType(shaderType), value(std::move(value))
    {

    }

    ShaderType shaderType;
    GenericParameterValue value;
  };

 public:
  GLMaterial();
  ~GLMaterial() override;

  void setShadersPipeline(std::shared_ptr<GLShadersPipeline> shadersPipeline);
  [[nodiscard]] std::shared_ptr<GLShadersPipeline> getShadersPipeline() const;

  void setDepthTestMode(DepthTestMode mode);
  [[nodiscard]] DepthTestMode getDepthTestMode() const;

  void setFaceCullingMode(FaceCullingMode mode);
  [[nodiscard]] FaceCullingMode getFaceCullingMode() const;

  void setPolygonFillingMode(PolygonFillingMode mode);
  [[nodiscard]] PolygonFillingMode getPolygonFillingMode() const;

  void setBlendingMode(BlendingMode mode);
  [[nodiscard]] BlendingMode getBlendingMode() const;

  void setDepthWritingMode(DepthWritingMode mode);
  [[nodiscard]] DepthWritingMode getDepthWritingMode() const;

  void setScissorsTestMode(ScissorsTestMode mode);
  [[nodiscard]] ScissorsTestMode getScissorsTestMode() const;

  void setShaderParameter(ShaderType shaderType, const std::string& name, const GenericParameterValue& value);
  [[nodiscard]] const GenericParameterValue& getShaderParameterValue(const std::string& name) const;

 private:
  std::unordered_map<std::string, GenericParameter> m_parameters;

  std::shared_ptr<GLShadersPipeline> m_shadersPipeline;

  DepthTestMode m_depthTestMode = DepthTestMode::Unspecified;
  FaceCullingMode m_faceCullingMode = FaceCullingMode::Unspecified;
  PolygonFillingMode m_polygonFillingMode = PolygonFillingMode::Unspecified;
  BlendingMode m_materialBlendingMode = BlendingMode::Unspecified;
  DepthWritingMode m_depthWritingMode = DepthWritingMode::Unspecified;
  ScissorsTestMode m_scissorsTestMode = ScissorsTestMode::Disabled;

 private:
  friend class GLGraphicsContext;
};
