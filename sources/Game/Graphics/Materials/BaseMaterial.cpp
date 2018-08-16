#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(const std::string & name, 
	GpuProgram * gpuProgram)
	: m_name(name),
	m_gpuProgram(gpuProgram),
	m_graphicsPipelineState(),
	m_supportDeferred(false),
	m_lightsDataRequired(false),
	m_transformsDataRequired(false)
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
	return m_transformsDataRequired;
}

bool BaseMaterial::isLightsDataRequired() const
{
	return m_lightsDataRequired;
}

bool BaseMaterial::isSupportDeferred() const
{
	return m_supportDeferred;
}

GpuProgram * BaseMaterial::getGpuProgram() const
{
	return m_gpuProgram;
}

const GraphicsPipelineState & BaseMaterial::getRequiredGraphicsPipelineState() const
{
	return m_graphicsPipelineState;
}
