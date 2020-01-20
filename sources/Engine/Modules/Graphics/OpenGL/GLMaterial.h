#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "GLTexture.h"

#include "Modules/Graphics/Resources/ShaderResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

enum class DepthTestMode {
    Disabled, Unspecified, Less, LessOrEqual, NotEqual
};

enum class FaceCullingMode {
    Disabled, Unspecified, Back, Front
};

enum class PolygonFillingMode {
    Unspecified, Fill, Wireframe
};

enum class BlendingMode {
    Disabled, Unspecified, Alpha_OneMinusAlpha
};

enum class DepthWritingMode {
    Disabled, Unspecified, Enabled
};

class GLGraphicsContext;

class GLMaterial
{
public:
    struct TextureParameter {
        TextureParameter(std::shared_ptr<GLTexture> texture, size_t slotIndex)
            : texture(texture), slotIndex(slotIndex)
        {

        }

        std::shared_ptr<GLTexture> texture;
        size_t slotIndex;
    };

    using GenericParameterValue = std::variant<int, float, glm::vec3, glm::vec4, glm::mat3, glm::mat4, TextureParameter>;

    struct GenericParameter {
        GenericParameter(GLenum shaderType, const GenericParameterValue& value)
            : shaderType(shaderType), value(value)
        {

        }

        GLenum shaderType;
        GenericParameterValue value;
    };

public:
    GLMaterial();
    ~GLMaterial();

    void setShadersPipeline(std::shared_ptr<GLShadersPipeline> shadersPipeline);
    std::shared_ptr<GLShadersPipeline> getShadersPipeline() const;

    void setDepthTestMode(DepthTestMode mode);
    DepthTestMode getDepthTestMode() const;

    void setFaceCullingMode(FaceCullingMode mode);
    FaceCullingMode getFaceCullingMode() const;

    void setPolygonFillingMode(PolygonFillingMode mode);
    PolygonFillingMode getPolygonFillingMode() const;

    void setBlendingMode(BlendingMode mode);
    BlendingMode getBlendingMode() const;

    void setDepthWritingMode(DepthWritingMode mode);
    DepthWritingMode getDepthWritingMode() const;

    void setShaderParameter(GLenum shaderType, const std::string& name, const GenericParameterValue& value);
    const GenericParameterValue& getShaderParameterValue(const std::string& name) const;

private:
    std::unordered_map<std::string, GenericParameter> m_parameters;

    std::shared_ptr<GLShadersPipeline> m_shadersPipeline;

    DepthTestMode m_depthTestMode = DepthTestMode::Unspecified;
    FaceCullingMode m_faceCullingMode = FaceCullingMode::Unspecified;
    PolygonFillingMode m_polygonFillingMode = PolygonFillingMode::Unspecified;
    BlendingMode m_materialBlendingMode = BlendingMode::Unspecified;
    DepthWritingMode m_depthWritingMode = DepthWritingMode::Unspecified;

private:
    friend class GLGraphicsContext;
};
