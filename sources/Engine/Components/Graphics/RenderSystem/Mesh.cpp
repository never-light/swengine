#include "Mesh.h"
#include <Engine\ServiceLocator.h>

Mesh::Mesh()
{
	m_geometryBuffer = ServiceLocator::getRenderer()->createHardwareBuffer();
}

Mesh::Mesh(HardwareBuffer* geometryBuffer)
	: m_geometryBuffer(geometryBuffer)
{

}

Mesh::~Mesh() {
	if (m_subMeshes.size()) {
		for (auto it = m_subMeshes.begin(); it != m_subMeshes.end(); it++) {
			delete (*it);
		}
	}

	if (m_geometryBuffer)
		delete m_geometryBuffer;
}

void Mesh::addSubMesh(Mesh* mesh) {
	m_subMeshes.push_back(mesh);
}

Mesh* Mesh::getSubMesh(size_t index) {
	return m_subMeshes.at(index);
}

const std::vector<Mesh*>& Mesh::getSubMeshesArray() const {
	return m_subMeshes;
}

void Mesh::setName(const std::string& name) {
	m_name = name;
}

const std::string& Mesh::getName() const {
	return m_name;
}

HardwareBuffer* Mesh::getGeometryBuffer() const {
	return m_geometryBuffer;
}