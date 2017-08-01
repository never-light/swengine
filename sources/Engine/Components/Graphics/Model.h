#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "SubModel.h"

class Model {
public:
	Model();
	~Model();

	void addSubModel(SubModel*);
	SubModel* getSubModel(size_t) const;
	std::vector<SubModel*> getSubModels() const;
	size_t countSubModels() const;

	vector3 getPosition() const;
	quaternion getOrientation() const;

	void setPosition(const vector3&);
	void setPosition(float32, float32, float32);

	void setOrientation(const quaternion&);

	void move(const vector3&);
	void move(float32, float32, float32);

	void setScale(const vector3&);
	vector3 getScale() const;
	void scale(const vector3&);

	void rotate(float32, const vector3&);
	void rotate(float32, float32, float32, float32);

	matrix4 getTransformationMatrix() const;
private:
	vector3 m_position;
	quaternion m_orientation;
	vector3 m_scale;

	std::vector<SubModel*> m_subModels;
};