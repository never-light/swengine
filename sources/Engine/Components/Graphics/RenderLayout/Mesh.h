#pragma once

#include <vector>

#include <Engine/Components/Graphics/Animation/Skeleton.h>
#include "SubMesh.h"

class Mesh {
public:
	Mesh(const std::vector<SubMesh*>& subMeshes, Skeleton* skeleton);
	~Mesh();

	const std::vector<SubMesh*>& getSubMeshes() const;

	const AABB& getAABB() const;

	bool hasSkeleton() const;
	Skeleton* getSkeleton() const;

private:
	void calculateAABB();

private:
	std::vector<SubMesh*> m_subMeshes;
	Skeleton* m_skeleton;

	AABB m_aabb;
};