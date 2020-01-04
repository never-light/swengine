#pragma once

#include "Modules/Graphics/OpenGL/GLMaterial.h"

class Material
{
public:
    Material(std::unique_ptr<GLMaterial> gpuMaterial);

    const GLMaterial& getGpuMaterial() const;
    GLMaterial& getGpuMaterial();

private:
    std::unique_ptr<GLMaterial> m_gpuMaterial;
};

