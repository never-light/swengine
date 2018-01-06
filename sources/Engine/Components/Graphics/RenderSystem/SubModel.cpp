#include "SubModel.h"

SubModel::SubModel(Model* parent) : m_parent(parent), m_material(nullptr), m_mesh(nullptr) {

}

SubModel::~SubModel() {

}

Model* SubModel::getParent() const {
	return m_parent;
}

void SubModel::setMesh(Mesh* mesh) {
	m_mesh = mesh;
}

Mesh* SubModel::getMesh() const {
	return m_mesh;
}

void SubModel::setMaterial(Material* material) {
	m_material = material;
}

Material* SubModel::getMaterial() const {
	return m_material;
}

bool SubModel::hasMaterial() const {
	return (m_material != nullptr);
}

bool SubModel::hasMesh() const {
	return (m_mesh != nullptr);
}