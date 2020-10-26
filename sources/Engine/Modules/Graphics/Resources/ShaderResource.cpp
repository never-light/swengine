#include "precompiled.h"

#pragma hdrstop

#include "ShaderResource.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>

#include "Utility/files.h"

ShaderResource::ShaderResource(ResourcesManager* resourcesManager)
  : ResourceTypeManager<GLShader, ShaderResourceParameters>(resourcesManager)
{

}

ShaderResource::~ShaderResource() = default;

void ShaderResource::load(size_t resourceIndex)
{
  ShaderResourceParameters* config = getResourceConfig(resourceIndex);

  std::string source = preprocessShaderSource(FileUtils::readFile(config->resourcePath));

  allocateResource<GLShader>(resourceIndex, config->shaderType, source);
}

std::string ShaderResource::preprocessShaderSource(const std::string& source)
{
  std::string processedSource = processIncludes(source);
  processedSource = processMacros(processedSource);

  return processedSource;
}

std::string ShaderResource::processIncludes(const std::string& source)
{
  return StringUtils::regexReplace("#include[\\s]+\"([a-zA-Z0-9/\\.]*)\"", source,
    [](const std::smatch& match) {
      if (match.size() != 2) {
        THROW_EXCEPTION(EngineRuntimeException,
          "Shader preprocessor: #include - syntax error");
      }

      std::string path = match[1].str();

      if (!FileUtils::isFileExists(path)) {
        THROW_EXCEPTION(EngineRuntimeException,
          "Shader preprocessor: #include - file is not found: "
            + std::string(path));
      }

      return processIncludes(FileUtils::readFile(path));
    });
}

std::string ShaderResource::processMacros(const std::string& source)
{
  std::unordered_map<std::string, std::string> macros;

  std::string processedSource = StringUtils::regexReplace("#define\\s([a-zA-Z0-9]+)\\s(.*)\n",
    source,
    [&macros](const std::smatch& match) {
      if (match.size() != 3) {
        THROW_EXCEPTION(EngineRuntimeException,
          "Shader preprocessor: #define - syntax error");
      }

      macros[match[1].str()] = match[2].str();

      return "";
    });

  for (auto&[macroName, macroValue] : macros) {
    processedSource = StringUtils::replace(processedSource, macroName, macroValue);
  }

  return processedSource;
}

void ShaderResource::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  ShaderResourceParameters* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Animation clip resource refer to not existing file", resourceConfig->resourcePath));
  }

  static std::unordered_map<std::string, ShaderType> shadersTypesMap = {
    {"vertex", ShaderType::Vertex},
    {"fragment", ShaderType::Fragment},
    {"geometry", ShaderType::Geometry},
  };

  // Shader type
  if (configNode.child("type")) {
    ShaderType shaderType = ResourceDeclHelpers::getFilteredParameterValue(configNode,
      "type",
      shadersTypesMap,
      static_cast<ShaderType>(ShaderType::Vertex));

    resourceConfig->shaderType = shaderType;
  }
}

