#pragma once

#include <Engine\Components\SceneManager\SceneObject.h>

#include "Material.h"
#include "SubModel.h"

class Model : public SceneObject {
public:
	Model();
	~Model();

	void addSubModel(SubModel*);
	SubModel* getSubModel(size_t) const;
	SubModel* getSubModelByName(const std::string& name);
	const std::vector<SubModel*>& getSubModels() const;
	size_t countSubModels() const;

	void setMaterial(Material* material);

private:
	std::vector<SubModel*> m_subModels;
};