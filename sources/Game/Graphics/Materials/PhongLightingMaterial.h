#pragma once

#include "BaseMaterial.h"

class PhongLightingMaterial : public BaseMaterial {
public:
	PhongLightingMaterial(const std::string& name, GpuProgram* gpuProgram);
	virtual ~PhongLightingMaterial();

	virtual void applySpecifier(const MaterialParameters* parameters);
protected:
};