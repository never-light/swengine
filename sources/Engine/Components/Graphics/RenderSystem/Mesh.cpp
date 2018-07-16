#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

std::vector<vector3> Mesh::getPositions() const
{
	return m_positions;
}

void Mesh::setPositions(const std::vector<vector3>& positions)
{
	m_positions = positions;
}

std::vector<vector3> Mesh::getNormals() const
{
	return m_normals;
}

void Mesh::setNormals(const std::vector<vector3>& normals)
{
	m_normals = normals;
}

std::vector<vector3> Mesh::getTangents() const
{
	return m_tangents;
}

void Mesh::setTangents(const std::vector<vector3>& tangents)
{
	m_tangents = tangents;
}

std::vector<vector2> Mesh::getUV() const
{
	return m_uv;
}

void Mesh::setUV(const std::vector<vector2>& uv)
{
	m_uv = uv;
}

std::vector<uint32> Mesh::getIndices() const
{
	return m_indices;
}

void Mesh::setIndices(const std::vector<uint32>& indices)
{
	m_indices = indices;
}

std::string Mesh::getName() const
{
	return m_name;
}

void Mesh::setName(const std::string & name)
{
	m_name = name;
}
