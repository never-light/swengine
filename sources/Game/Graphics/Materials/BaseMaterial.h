#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Game\Graphics\GraphicsPipelineState.h>

#include "MaterialParameters.h"

class BaseMaterial {
public:
	BaseMaterial(const std::string& name, GpuProgram* gpuProgram);
	virtual ~BaseMaterial();

	virtual void bind();
	virtual void applySpecifier(const MaterialParameters* parameters) = 0;
	virtual void unbind();

	std::string getName() const;

	bool isTransformsDataRequired() const;
	bool isLightsDataRequired() const;

	GpuProgram* getGpuProgram() const;

	const GraphicsPipelineState& getRequiredGraphicsPipelineState() const;
protected:
	bool m_transformsDataRequired;
	bool m_lightsDataRequired;

protected:
	std::string m_name;

	GpuProgram * m_gpuProgram;
	GraphicsPipelineState m_graphicsPipelineState;
};