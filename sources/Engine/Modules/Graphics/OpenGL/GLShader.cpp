#include "GLShader.h"
#include "Exceptions/EngineRuntimeException.h"
#include "GLDebug.h"

#include <memory>

GLShader::GLShader(GLenum type, const std::string &source)
    : m_shaderProgram(0),
      m_type(type)
{
    GLenum shader = 0;

    switch (type) {
    case GL_VERTEX_SHADER:
        shader = glCreateShader(GL_VERTEX_SHADER);
        break;
    case GL_FRAGMENT_SHADER:
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case GL_GEOMETRY_SHADER:
        shader = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    default:
        ENGINE_RUNTIME_ERROR("Tring to create invalid shader with invalid type");
    }

    const char* rawSource = source.c_str();

    glShaderSource(shader, 1, &rawSource, nullptr);
    glCompileShader(shader);

    GLint compilationStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);

    if (!compilationStatus) {
        glDeleteProgram(shader);

        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::string message;
        message.resize(static_cast<std::string::size_type>(infoLogLength));

        glGetShaderInfoLog(shader, infoLogLength, nullptr, &message[0]);

        ENGINE_RUNTIME_ERROR("Failed to compile shader: " + message);
    }

    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, shader);
    glProgramParameteri(m_shaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

    glLinkProgram(m_shaderProgram);

    GLint linkingStatus;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkingStatus);

    if (!linkingStatus) {
        glDeleteShader(shader);
        glDeleteProgram(m_shaderProgram);

        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::string message;
        message.resize(static_cast<std::string::size_type>(infoLogLength));

        glGetProgramInfoLog(shader, infoLogLength, nullptr, &message[0]);

        ENGINE_RUNTIME_ERROR("Failed to link shader program: " + message);
    }

    glDetachShader(m_shaderProgram, shader);
    glDeleteShader(shader);

    cacheUniformsLocations();
}

GLShader::~GLShader()
{
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
    }
}

GLenum GLShader::getType() const
{
    return m_type;
}

void GLShader::setParameter(const std::string &name, bool value)
{
    glProgramUniform1i(m_shaderProgram, m_uniformsCache[name].location, value);
}

void GLShader::setParameter(const std::string &name, int value)
{
    glProgramUniform1i(m_shaderProgram, m_uniformsCache[name].location, value);
}

void GLShader::setParameter(const std::string &name, float value)
{
    glProgramUniform1f(m_shaderProgram, m_uniformsCache[name].location, value);
}

void GLShader::setParameter(const std::string &name, const glm::vec2 &value)
{
    glProgramUniform2fv(m_shaderProgram, m_uniformsCache[name].location, 1, &value[0]);
}

void GLShader::setParameter(const std::string &name, const glm::vec3 &value)
{
    glProgramUniform3fv(m_shaderProgram, m_uniformsCache[name].location, 1, &value[0]);
}

void GLShader::setParameter(const std::string &name, const glm::vec4 &value)
{
    glProgramUniform4fv(m_shaderProgram, m_uniformsCache[name].location, 1, &value[0]);
}

void GLShader::cacheUniformsLocations()
{
    GL_CALL_BLOCK_BEGIN();

    GLint uniform_count = 0;
    glGetProgramiv(m_shaderProgram, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count != 0) {
        GLint max_name_len = 0;
        GLsizei length = 0;
        GLsizei count = 0;
        GLenum type = GL_NONE;

        glGetProgramiv(m_shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        auto uniformName = std::make_unique<char[]>(static_cast<size_t>(max_name_len));

        for (GLint i = 0; i < uniform_count; ++i) {
            glGetActiveUniform(m_shaderProgram, static_cast<GLuint>(i),
                               max_name_len, &length,
                               &count, &type,
                               uniformName.get());

            UniformInfo uniformInfo = {
                glGetUniformLocation(m_shaderProgram, uniformName.get()),
                count
            };

            m_uniformsCache.insert({ std::string(uniformName.get(),
                                     static_cast<std::string::size_type>(length)), uniformInfo });
        }
    }

    GL_CALL_BLOCK_END();
}