#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "Mesh.h"
#include "Material.h"

class Model {
public:
	Model();
	~Model();

	void setMesh(Mesh*);
	Mesh* getMesh() const;

	void setMaterial(Material*);
	Material* getMaterial() const;

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

	Mesh* m_mesh;
	Material* m_material;
};