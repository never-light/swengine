#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLShader.h"

struct ShaderResourceConfig {
  ShaderResourceConfig() = default;

  std::string resourcePath;
  ShaderType shaderType = ShaderType::Vertex;
};

class ShaderResourceManager : public ResourceManager<GLShader, ShaderResourceConfig> {
 public:
  explicit ShaderResourceManager(ResourcesManager* resourcesManager);
  ~ShaderResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;

 private:
  static std::string preprocessShaderSource(const std::string& source);
  static std::string processIncludes(const std::string& source);
  static std::string processMacros(const std::string& source);

};
