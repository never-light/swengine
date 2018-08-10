#include "SolidMesh.h"

SolidMesh::SolidMesh(GeometryStore * geometry, 
	const std::vector<size_t>& groupsOffsets, 
	const std::vector<DefaultMaterial*>& materials,
	const std::vector<OBB>& colliders,
	Skeleton* skeleton)
	: m_geometry(geometry), 
	m_groupsOffsets(groupsOffsets),
	m_materials(materials),
	m_colliders(colliders),
	m_skeleton(skeleton)
{
}

SolidMesh::~SolidMesh()
{
	if (m_skeleton != nullptr)
		delete m_skeleton;
}

void SolidMesh::render(GraphicsContext* graphicsContext, GpuProgram* gpuProgram) {
	m_geometry->bind();

	for (size_t i = 0; i < m_groupsOffsets.size(); i++) {
		bindMaterial(gpuProgram, m_materials[i]);
		
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

void SolidMesh::bindMaterial(GpuProgram* gpuProgram, const DefaultMaterial* material) {
	gpuProgram->setParameter("material.diffuseColor", material->getDiffuseColor());
	gpuProgram->setParameter("material.specularColor", material->getSpecularColor());
	gpuProgram->setParameter("material.specularFactor", material->getSpecularFactor());
	gpuProgram->setParameter("material.emissiveColor", material->getEmissiveColor());

	Texture* diffuseTexture = material->getDiffuseTexture();

	if (diffuseTexture != nullptr) {
		diffuseTexture->bind(0);

		gpuProgram->setParameter("material.useDiffuseTexture", true);
		gpuProgram->setParameter("material.diffuseTexture", 0);
	}
	else {
		gpuProgram->setParameter("material.useDiffuseTexture", false);
	}

	Texture* specularTexture = material->getSpecularTexture();

	if (specularTexture != nullptr) {
		specularTexture->bind(1);
		gpuProgram->setParameter("material.useSpecularTexture", true);
		gpuProgram->setParameter("material.specularTexture", 1);
	}

	Texture* normalMap = material->getNormalMap();

	if (normalMap != nullptr) {
		normalMap->bind(2);
		gpuProgram->setParameter("material.useNormalMapping", true);
		gpuProgram->setParameter("material.normalMap", 2);
	}
}