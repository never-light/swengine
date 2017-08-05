#include "Model.h"

Model::Model() : SceneObject(SceneObjectType::Model) {

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