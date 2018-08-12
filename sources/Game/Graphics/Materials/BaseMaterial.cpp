#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(const std::string & name, 
	GpuProgram * gpuProgram)
	: m_name(name),
	m_gpuProgram(gpuProgram),
	m_graphicsPipelineState()
{
}

BaseMaterial::~BaseMaterial()
{
}

void BaseMaterial::bind()
{
	m_gpuProgram->bind();
}

void BaseMaterial::unbind()
{
	m_gpuProgram->unbind();
}

std::string BaseMaterial::getName() const
{
	return m_name;
}

bool BaseMaterial::isTransformsDataRequired() const
{
	return m_lightsDataRequired;
}

bool BaseMaterial::isLightsDataRequired() const
{
	return m_lightsDataRequired;
}

GpuProgram * BaseMaterial::getGpuProgram() const
{
	return m_gpuProgram;
}

const GraphicsPipelineState & BaseMaterial::getRequiredGraphicsPipelineState() const
{
	return m_graphicsPipelineState;
}
