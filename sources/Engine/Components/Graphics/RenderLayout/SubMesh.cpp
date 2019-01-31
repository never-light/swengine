#include "SubMesh.h"

SubMesh::SubMesh(GeometryInstance* instance, Material* material) 
	: m_material(material), m_geometryInstance(instance)
{
}

SubMesh::~SubMesh()
{
	delete m_geometryInstance;
}

GeometryInstance * SubMesh::getGeometryInstance() const
{
	return m_geometryInstance;
}

void SubMesh::setMaterial(Material * material)
{
	m_material = material;
}

Material * SubMesh::getMaterial() const
{
	return m_material;
}

bool SubMesh::isSkinned() const
{
	return m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::JointsIds) 
		&& m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::JointsWeights);
}

bool SubMesh::hasNormals() const
{
	return m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::Normal);
}

bool SubMesh::hasTangents() const
{
	return m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::Tangent);
}

bool SubMesh::hasBitangents() const
{
	return m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::Bitangent);
}

bool SubMesh::hasUV() const
{
	return m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::UV);
}

bool SubMesh::hasColors() const
{
	return m_geometryInstance->hasAttribute(GeometryInstance::CommonAttrs::UV);
}

bool SubMesh::hasAABB() const
{
	return m_aabb.has_value();
}

void SubMesh::setAABB(const AABB & aabb)
{
	m_aabb = aabb;
}

const AABB & SubMesh::getAABB() const
{
	return m_aabb.value();
}
