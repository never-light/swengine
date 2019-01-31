#include "Mesh.h"

Mesh::Mesh(const std::vector<SubMesh*>& subMeshes, Skeleton* skeleton)
	: m_subMeshes(subMeshes),
	m_skeleton(skeleton)
{
	calculateAABB();
}

Mesh::~Mesh()
{
	for (SubMesh* subMesh : m_subMeshes)
		delete subMesh;

	if (m_skeleton != nullptr)
		delete m_skeleton;
}

const std::vector<SubMesh*>& Mesh::getSubMeshes() const
{
	return m_subMeshes;
}

const AABB & Mesh::getAABB() const
{
	return m_aabb;
}

bool Mesh::hasSkeleton() const
{
	return m_skeleton != nullptr;
}

Skeleton * Mesh::getSkeleton() const
{
	return m_skeleton;
}

void Mesh::calculateAABB()
{
	vector3 minPoint{ std::numeric_limits<float>::max() };
	vector3 maxPoint{ std::numeric_limits<float>::min() };

	for (const SubMesh* subMesh : m_subMeshes) {
		if (!subMesh->hasAABB())
			continue;

		const AABB& subMeshAABB = subMesh->getAABB();
		vector3 subMeshAABBMin = subMeshAABB.getMin();
		vector3 subMeshAABBMax = subMeshAABB.getMax();

		minPoint.x = std::fmin(minPoint.x, subMeshAABBMin.x);
		minPoint.y = std::fmin(minPoint.y, subMeshAABBMin.y);
		minPoint.z = std::fmin(minPoint.z, subMeshAABBMin.z);

		maxPoint.x = std::fmax(maxPoint.x, subMeshAABBMax.x);
		maxPoint.x = std::fmax(maxPoint.y, subMeshAABBMax.y);
		maxPoint.x = std::fmax(maxPoint.z, subMeshAABBMax.z);
	}

	m_aabb.setMin(minPoint);
	m_aabb.setMax(maxPoint);
}
