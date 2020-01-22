#include "ShaderResource.h"
#include "Exceptions/EngineRuntimeException.h"

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
        ENGINE_RUNTIME_ERROR("Trying to load shader resource from invalid source");
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

std::shared_ptr<GLShader> ShaderResource::loadFromFile(const std::string& path, const ShaderResourceParameters& parameters)
{
    std::string source = preprocessShaderSource(FileUtills::readFile(path));

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
    static std::unordered_map<std::string, GLenum> shadersTypesMap = {
        { "vertex", GL_VERTEX_SHADER },
        { "fragment", GL_FRAGMENT_SHADER },
        { "geometry", GL_GEOMETRY_SHADER },
    };

    ParametersType parameters = defaultParameters;

    // Shader type
    if (declarationNode.child("type")) {
        GLenum shaderType = ResourceDeclHelpers::getFilteredParameterValue(declarationNode, "type",
            shadersTypesMap, static_cast<GLenum>(GL_VERTEX_SHADER));

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
    [] (const std::smatch& match) {
        if (match.size() != 2) {
            ENGINE_RUNTIME_ERROR("Shader preprocessor: #include - syntax error");
        }

        std::string path = match[1].str();

        if (!FileUtills::isFileExists(path)) {
            ENGINE_RUNTIME_ERROR("Shader preprocessor: #include - file is not found: " + std::string(path));
        }

        return processIncludes(FileUtills::readFile(path));
    });
}

std::string ShaderResource::processMacros(const std::string& source)
{
    std::unordered_map<std::string, std::string> macros;

    std::string processedSource = StringUtils::regexReplace("#define\\s([a-zA-Z0-9]+)\\s(.*)\n", source,
    [&macros] (const std::smatch& match) {
        if (match.size() != 3) {
            ENGINE_RUNTIME_ERROR("Shader preprocessor: #define - syntax error");
        }

        macros[match[1].str()] = match[2].str();

        return "";
    });

    for (auto& [ macroName, macroValue ] : macros) {
        processedSource = StringUtils::replace(processedSource, macroName, macroValue);
    }

    return processedSource;
}
