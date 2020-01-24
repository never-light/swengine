#include "precompiled.h"
#pragma hdrstop

#include "Material.h"

Material::Material(std::unique_ptr<GLMaterial> gpuMaterial)
    : m_gpuMaterial(std::move(gpuMaterial))
{

}

const GLMaterial& Material::getGpuMaterial() const
{
    return *m_gpuMaterial.get();
}

GLMaterial& Material::getGpuMaterial()
{
    return *m_gpuMaterial.get();
}
