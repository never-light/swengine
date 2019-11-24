#include "Material.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::setShadersPipeline(std::shared_ptr<GLShadersPipeline> shadersPipeline)
{
    m_shadersPipeline = shadersPipeline;
}

std::shared_ptr<GLShadersPipeline> Material::getShadersPipeline() const
{
    return m_shadersPipeline;
}
