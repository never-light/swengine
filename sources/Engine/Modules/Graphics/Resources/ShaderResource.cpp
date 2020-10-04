#include "precompiled.h"

#pragma hdrstop

#include "ShaderResource.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>

#include "Utility/files.h"

ShaderResource::ShaderResource()
{

}

ShaderResource::~ShaderResource()
{
  SW_ASSERT(m_shader.use_count() <= 1);
}

void ShaderResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
  ARG_UNUSED(resourceManager);

  SW_ASSERT(m_shader == nullptr);

  ShaderResourceParameters parameters = declaration.getParameters<ShaderResourceParameters>();

  if (auto sourceString = std::get_if<ResourceSourceRawString>(&declaration.source)) {
    m_shader = loadFromRawString(sourceString->data, parameters);
  }
  else if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
    m_shader = loadFromFile(sourceFile->path, parameters);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load shader resource from invalid source");
  }
}

void ShaderResource::unload()
{
  SW_ASSERT(m_shader.use_count() == 1);

  m_shader.reset();
}

bool ShaderResource::isBusy() const
{
  return m_shader.use_count() > 1;
}

std::shared_ptr<GLShader> ShaderResource::loadFromFile(const std::string& path,
  const ShaderResourceParameters& parameters)
{
  std::string source = preprocessShaderSource(FileUtils::readFile(path));

  return std::make_shared<GLShader>(parameters.shaderType, source);
}

std::shared_ptr<GLShader> ShaderResource::loadFromRawString(const std::string& rawString,
  const ShaderResourceParameters& parameters)
{
  std::string source = preprocessShaderSource(rawString);

  return std::make_shared<GLShader>(parameters.shaderType, source);
}

ShaderResource::ParametersType ShaderResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
  const ParametersType& defaultParameters)
{
  static std::unordered_map<std::string, ShaderType> shadersTypesMap = {
    {"vertex", ShaderType::Vertex},
    {"fragment", ShaderType::Fragment},
    {"geometry", ShaderType::Geometry},
  };

  ParametersType parameters = defaultParameters;

  // Shader type
  if (declarationNode.child("type")) {
    ShaderType shaderType = ResourceDeclHelpers::getFilteredParameterValue(declarationNode,
      "type",
      shadersTypesMap,
      static_cast<ShaderType>(ShaderType::Vertex));

    parameters.shaderType = shaderType;
  }

  return parameters;
}

std::shared_ptr<GLShader> ShaderResource::getShader() const
{
  return m_shader;
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
