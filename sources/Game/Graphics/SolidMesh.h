#pragma once

#include <Engine\Components\ResourceManager\Resource.h>
#include <Engine\Components\Graphics\RenderSystem\GeometryStore.h>
#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>
#include <Engine\Components\Physics\Colliders\OBB.h>

#include <Game\Graphics\Animation\Skeleton.h>

#include "DefaultMaterial.h"

#include <vector>

class SolidMesh : public Resource {
public:
	SolidMesh(GeometryStore* geometry, 
		const std::vector<size_t>& groupsOffsets, 
		const std::vector<DefaultMaterial*>& materials,
		const std::vector<OBB>& colliders,
		Skeleton* skeleton);
	virtual ~SolidMesh();

	virtual void render(GraphicsContext* graphicsContext, GpuProgram* gpuProgram);

	std::vector<OBB> getColliders() const;

	bool hasSkeleton() const;
	Skeleton* getSkeleton() const;
protected:
	void bindMaterial(GpuProgram* gpuProgram, const DefaultMaterial* material);

protected:
	std::vector<size_t> m_groupsOffsets;
	GeometryStore* m_geometry;

	std::vector<DefaultMaterial*> m_materials;
	std::vector<OBB> m_colliders;

	Skeleton* m_skeleton;
};