#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\SceneManager\SceneObject.h>
#include <Engine\Components\SceneManager\SceneNode.h>
#include "SubModel.h"

class Model : public SceneObject {
public:
	Model();
	~Model();

	void addSubModel(SubModel*);
	SubModel* getSubModel(size_t) const;
	std::vector<SubModel*> getSubModels() const;
	size_t countSubModels() const;

private:
	std::vector<SubModel*> m_subModels;
};