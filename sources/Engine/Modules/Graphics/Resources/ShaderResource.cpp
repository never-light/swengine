#include "ShaderResource.h"
#include "Exceptions/EngineRuntimeException.h"

#include <fstream>
#include <streambuf>

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
    std::ifstream file(path);
    std::string fileData((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    return std::make_shared<GLShader>(parameters.shaderType, fileData);
}

std::shared_ptr<GLShader> ShaderResource::loadFromRawString(const std::string& rawString, const ShaderResourceParameters& parameters)
{
    return std::make_shared<GLShader>(parameters.shaderType, rawString);
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
