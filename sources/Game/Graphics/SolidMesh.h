#pragma once

#include <Engine\Components\ResourceManager\Resource.h>
#include <Engine\Components\Graphics\RenderSystem\GeometryStore.h>
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>
#include <Engine\Components\Physics\Colliders\OBB.h>

#include <Game\Graphics\Animation\Skeleton.h>
#include <Game\Graphics\Materials\BaseMaterial.h>

#include <vector>

class SolidMesh : public Resource {
public:
	SolidMesh(GeometryStore* geometry, 
		const std::vector<size_t>& groupsOffsets, 
		const std::vector<MaterialParameters*>& materialsParameters,
		const std::vector<OBB>& colliders,
		Skeleton* skeleton);
	virtual ~SolidMesh();

	void render(BaseMaterial* baseMaterial);

	std::vector<OBB> getColliders() const;

	bool hasSkeleton() const;
	Skeleton* getSkeleton() const;

protected:
	std::vector<size_t> m_groupsOffsets;
	GeometryStore* m_geometry;

	std::vector<MaterialParameters*> m_materialsParameters;
	std::vector<OBB> m_colliders;

	Skeleton* m_skeleton;
};