#include "GLMaterial.h"

GLMaterial::GLMaterial()
{

}

GLMaterial::~GLMaterial()
{

}

void GLMaterial::setShadersPipeline(std::shared_ptr<GLShadersPipeline> shadersPipeline)
{
    m_shadersPipeline = shadersPipeline;
}

std::shared_ptr<GLShadersPipeline> GLMaterial::getShadersPipeline() const
{
    return m_shadersPipeline;
}

void GLMaterial::setDepthTestMode(DepthTestMode mode)
{
    m_depthTestMode = mode;
}

DepthTestMode GLMaterial::getDepthTestMode() const
{
    return m_depthTestMode;
}

void GLMaterial::setFaceCullingMode(FaceCullingMode mode)
{
    m_faceCullingMode = mode;
}

FaceCullingMode GLMaterial::getFaceCullingMode() const
{
    return m_faceCullingMode;
}

void GLMaterial::setPolygonFillingMode(PolygonFillingMode mode)
{
    m_polygonFillingMode = mode;
}

PolygonFillingMode GLMaterial::getPolygonFillingMode() const
{
    return m_polygonFillingMode;
}

void GLMaterial::setBlendingMode(BlendingMode mode)
{
    m_materialBlendingMode = mode;
}

BlendingMode GLMaterial::getBlendingMode() const
{
    return m_materialBlendingMode;
}

void GLMaterial::setDepthWritingMode(DepthWritingMode mode)
{
    m_depthWritingMode = mode;
}

DepthWritingMode GLMaterial::getDepthWritingMode() const
{
    return m_depthWritingMode;
}

void GLMaterial::setShaderParameter(GLenum shaderType, const std::string& name, const GenericParameterValue& value)
{
    m_parameters.insert({name, GenericParameter(shaderType, value)});
}

const GLMaterial::GenericParameterValue& GLMaterial::getShaderParameterValue(const std::string& name) const
{
    return m_parameters.at(name).value;
}
