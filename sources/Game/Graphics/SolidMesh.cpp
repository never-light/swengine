#include "SolidMesh.h"

SolidMesh::SolidMesh(GeometryStore * geometry, 
	const std::vector<size_t>& groupsOffsets, 
	const std::vector<MaterialParameters*>& materials,
	const std::vector<OBB>& colliders,
	Skeleton* skeleton)
	: m_geometry(geometry), 
	m_groupsOffsets(groupsOffsets),
	m_materialsParameters(materials),
	m_colliders(colliders),
	m_skeleton(skeleton)
{
}

SolidMesh::~SolidMesh()
{
	for (MaterialParameters* parameters : m_materialsParameters)
		delete parameters;

	if (m_skeleton != nullptr)
		delete m_skeleton;
}

void SolidMesh::render(BaseMaterial* baseMaterial) {
	bool isAnimated = m_skeleton != nullptr;

	GpuProgram* gpuProgram = baseMaterial->getGpuProgram();
	gpuProgram->setParameter("animation.isAnimated", isAnimated);

	if (isAnimated) {
		size_t boneIndex = 0;

		for (const Bone& bone : m_skeleton->getBones()) {
			baseMaterial->getGpuProgram()->setParameter("animation.bones[" + std::to_string(boneIndex) + "]", bone.getCurrentPoseTransform());
			boneIndex++;
		}
	}

	m_geometry->bind();

	for (size_t i = 0; i < m_groupsOffsets.size(); i++) {
		baseMaterial->applySpecifier(m_materialsParameters[i]);
		
		size_t groupOffset = (i == 0) ? 0 : m_groupsOffsets[i - 1];
		size_t count = (i == 0) ? m_groupsOffsets[0] : m_groupsOffsets[i] - m_groupsOffsets[i - 1];

		m_geometry->drawElements(GeometryStore::DrawType::Triangles, groupOffset, count, 
			GeometryStore::IndicesType::UnsignedInt);
	}
}

std::vector<OBB> SolidMesh::getColliders() const
{
	return m_colliders;
}

bool SolidMesh::hasSkeleton() const
{
	return m_skeleton != nullptr;
}

Skeleton * SolidMesh::getSkeleton() const
{
	return m_skeleton;
}