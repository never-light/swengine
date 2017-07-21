#include "Model.h"

Model::Model() {

}

Model::~Model() {

}

void Model::setMesh(Mesh* mesh) {
	m_mesh = mesh;
}

Mesh* Model::getMesh() const {
	return m_mesh;
}

void Model::setShader(Shader* shader) {
	m_shader = shader;
}

Shader* Model::getShader() const {
	return m_shader;
}

void Model::setTexture(Texture* texture) {
	m_texture = texture;
}

Texture* Model::getTexture() const {
	return m_texture;
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

matrix4 Model::getTransformationMatrix() const {
	return glm::mat4_cast(m_orientation) * glm::translate(matrix4(), m_position);
}