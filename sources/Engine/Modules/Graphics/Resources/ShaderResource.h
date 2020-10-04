#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/OpenGL/GLShader.h"

struct ShaderResourceParameters : ResourceSourceParameters {
  ShaderResourceParameters() = default;

  explicit ShaderResourceParameters(ShaderType shaderType) : shaderType(shaderType)
  {}

  ShaderType shaderType;
};

class ShaderResource : public Resource {
 public:
  using ParametersType = ShaderResourceParameters;

 public:
  ShaderResource();
  ~ShaderResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static std::shared_ptr<GLShader> loadFromFile(const std::string& path,
    const ShaderResourceParameters& parameters);

  static std::shared_ptr<GLShader> loadFromRawString(const std::string& rawString,
    const ShaderResourceParameters& parameters);

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
    const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<GLShader> getShader() const;

 private:
  static std::string preprocessShaderSource(const std::string& source);
  static std::string processIncludes(const std::string& source);
  static std::string processMacros(const std::string& source);

 private:
  std::shared_ptr<GLShader> m_shader;
};
