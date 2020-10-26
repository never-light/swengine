#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/OpenGL/GLShader.h"

struct ShaderResourceParameters {
  ShaderResourceParameters() = default;

  std::string resourcePath;
  ShaderType shaderType = ShaderType::Vertex;
};

class ShaderResource : public ResourceTypeManager<GLShader, ShaderResourceParameters> {
 public:
  explicit ShaderResource(ResourcesManager* resourcesManager);
  ~ShaderResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;

 private:
  static std::string preprocessShaderSource(const std::string& source);
  static std::string processIncludes(const std::string& source);
  static std::string processMacros(const std::string& source);

};
