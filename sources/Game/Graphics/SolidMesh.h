#pragma once

#include <Engine\Components\ResourceManager\Resource.h>
#include <Engine\Components\Graphics\RenderSystem\GeometryStore.h>
#include "DefaultMaterial.h"

#include <vector>

class SolidMesh : public Resource {
public:
	SolidMesh(GeometryStore* geometry, 
		const std::vector<size_t>& groupsOffsets, 
		const std::vector<DefaultMaterial*> materials);
	virtual ~SolidMesh();

	virtual void render(GpuProgram* gpuProgram);

protected:
	void bindMaterial(GpuProgram* gpuProgram, const DefaultMaterial* material);

protected:
	std::vector<size_t> m_groupsOffsets;
	GeometryStore* m_geometry;

	std::vector<DefaultMaterial*> m_materials;
};