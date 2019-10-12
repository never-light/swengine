#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <unordered_map>

#include "GL.h"

class GLShadersPipeline;

class GLShader
{
public:
    GLShader(GLenum type, const std::string& source);
    ~GLShader();

    GLenum getType() const;

    void setParameter(const std::string& name, bool value);
    void setParameter(const std::string& name, int value);
    void setParameter(const std::string& name, float value);
    void setParameter(const std::string& name, const glm::vec2& value);
    void setParameter(const std::string& name, const glm::vec3& value);
    void setParameter(const std::string& name, const glm::vec4& value);

private:
    struct UniformInfo {
        GLint location;
        GLsizei size;
    };

private:
    void cacheUniformsLocations();

private:
    GLuint m_shaderProgram;
    GLenum m_type;

    std::unordered_map<std::string, UniformInfo> m_uniformsCache;

private:
    friend class GLShadersPipeline;
};

