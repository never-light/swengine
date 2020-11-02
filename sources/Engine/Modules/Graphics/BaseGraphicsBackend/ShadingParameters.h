#pragma once

#include "Modules/Graphics/OpenGL/GLTexture.h"
#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"

class ShadingParametersGenericStorage {
 public:
  struct TextureParameter {
    TextureParameter(ResourceHandle<GLTexture> texture, size_t slotIndex)
      : texture(std::move(texture)), slotIndex(slotIndex)
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
  ShadingParametersGenericStorage() = default;

  void setShaderParameter(ShaderType shaderType, const std::string& name, const GenericParameterValue& value);
  [[nodiscard]] const GenericParameterValue& getShaderParameterValue(const std::string& name) const;

  [[nodiscard]] const std::unordered_map<std::string, GenericParameter>& getParameters() const;

 private:
  std::unordered_map<std::string, GenericParameter> m_parameters;
};



class ShadingParametersBaseSet {
 public:
  ShadingParametersBaseSet() = default;
  virtual ~ShadingParametersBaseSet() = default;
};

class ShadingParametersGenericSet : public ShadingParametersBaseSet {
 public:

  ShadingParametersGenericSet() = default;
  ~ShadingParametersGenericSet() override = default;

  void setShaderParameter(ShaderType shaderType,
    const std::string& name,
    const ShadingParametersGenericStorage::GenericParameterValue& value);

  [[nodiscard]] const ShadingParametersGenericStorage::GenericParameterValue& getShaderParameterValue(const std::string& name) const;

  [[nodiscard]] const std::unordered_map<std::string, ShadingParametersGenericStorage::GenericParameter>& getParameters() const;

 private:
  ShadingParametersGenericStorage m_parametersStorage;
};

class ShadingParametersGUI : public ShadingParametersBaseSet {
 public:

  ShadingParametersGUI() = default;
  ~ShadingParametersGUI() override = default;

  void setBackgroundColor(const glm::vec4& color);
  [[nodiscard]] const glm::vec4& getBackgroundColor() const;

  void setBackgroundTexture(ResourceHandle<GLTexture> texture);
  [[nodiscard]] ResourceHandle<GLTexture> getBackgroundTexture() const;

  void setAlphaTexture(ResourceHandle<GLTexture> texture);
  [[nodiscard]] ResourceHandle<GLTexture> getAlphaTexture() const;

 private:
  glm::vec4 m_backgroundColor{};
  ResourceHandle<GLTexture> m_backgroundTexture{};
  ResourceHandle<GLTexture> m_alphaTexture{};
};

class ShadingParametersOpaqueMesh : public ShadingParametersBaseSet {
 public:

  ShadingParametersOpaqueMesh() = default;
  ~ShadingParametersOpaqueMesh() override = default;

  void setAlbedoTexture(ResourceHandle<GLTexture> texture);
  [[nodiscard]] ResourceHandle<GLTexture> getAlbedoTexture() const;

 private:
  ResourceHandle<GLTexture> m_albedoTexture;
};