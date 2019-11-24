#pragma once

#include "Modules/Graphics/Resources/ShaderResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

class Material
{
public:
    Material();
    ~Material();

    void setShadersPipeline(std::shared_ptr<GLShadersPipeline> shadersPipeline);
    std::shared_ptr<GLShadersPipeline> getShadersPipeline() const;

private:
    std::shared_ptr<GLShadersPipeline> m_shadersPipeline;
};

