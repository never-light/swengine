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

SubModel* Model::getSubModelByName(const std::string& name) {
	for (SubModel* model : m_subModels) {
		if (model->getMesh()->getName() == name) {
			return model;
		}
	}

	throw std::exception(std::string("Model" + name + " not found").c_str());
}

const std::vector<SubModel*>& Model::getSubModels() const {
	return m_subModels;
}

size_t Model::countSubModels() const {
	return m_subModels.size();
}

void Model::setMaterial(Material* material) {
	for (auto subModel : m_subModels) {
		subModel->setMaterial(material);
	}
}