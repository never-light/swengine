#include "Model.h"

Model::Model() : m_position(0, 0, 0), m_scale(1, 1, 1) {

}

Model::~Model() {

}

void Model::setMesh(Mesh* mesh) {
	m_mesh = mesh;
}

Mesh* Model::getMesh() const {
	return m_mesh;
}

void Model::setMaterial(Material* material) {
	m_material = material;
}

Material* Model::getMaterial() const {
	return m_material;
}

vector3 Model::getPosition() const {
	return m_position;
}

quaternion Model::getOrientation() const {
	return m_orientation;
}

void Model::setPosition(const vector3& position) {
	m_position = position;
}

void Model::setPosition(float32 x, float32 y, float32 z) {
	m_position = glm::vec3(x, y, z);
}

void Model::setOrientation(const quaternion& orientation) {
	m_orientation = orientation;
}

void Model::move(const vector3& movement) {
	m_position += movement;
}

void Model::move(float32 x, float32 y, float32 z) {
	m_position += vector3(x, y, z);
}

void Model::rotate(float32 angle, const vector3& axis) {
	m_orientation *= glm::angleAxis(glm::radians(angle), glm::normalize(axis));
}

void Model::rotate(float32 angle, float32 x, float32 y, float32 z) {
	rotate(angle, vector3(x, y, z));
}

void Model::setScale(const vector3& scale) {
	m_scale = scale;
}

vector3 Model::getScale() const {
	return m_scale;
}

void Model::scale(const vector3& scale) {
	m_scale *= scale;
}

matrix4 Model::getTransformationMatrix() const {
	return glm::mat4_cast(m_orientation) * glm::translate(matrix4(), m_position) * glm::scale(matrix4(), m_scale);
}