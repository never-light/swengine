#include "Model.h"

Model::Model() : m_position(0, 0, 0), m_scale(1, 1, 1) {

}

Model::~Model() {
	for (SubModel* subModel : m_subModels) {
		delete subModel;
	}
}

void Model::addSubModel(SubModel* subModel) {
	m_subModels.push_back(subModel);
}

SubModel* Model::getSubModel(size_t index) const {
	return m_subModels.at(index);
}

std::vector<SubModel*> Model::getSubModels() const {
	return m_subModels;
}

size_t Model::countSubModels() const {
	return m_subModels.size();
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